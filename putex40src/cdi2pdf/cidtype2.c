/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/cidtype2.c,v 1.1 2003/12/27 18:03:16 csc Exp $
    
    This is dvipdfmx, an eXtended version of dvipdfm by Mark A. Wicks.

    Copyright (C) 2002 by Jin-Hwan Cho and Shunsaku Hirata,
    the dvipdfmx project team <dvipdfmx@project.ktug.or.kr>
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
*/

/*
 * Change: Use CIDToGIDMap (20031118)
 *
 *  Using CIDToGIDMap is better for "sparse" character sets like Unicode.
 *
 */

#include <stdlib.h>
#include <limits.h>

#include "system.h"
#include "numbers.h"
#include "mem.h"
#include "mfileio.h"
#include "error.h"

/* mangle_name() */
#include "dpxutil.h"

#include "pdfobj.h"

/* TrueType */
#include "sfnt.h"
#include "tt_aux.h"
#include "tt_build.h"
#include "tt_cmap.h"
#include "tt_gsub.h"
#include "tt_table.h"

/* CID font */
#include "cmap.h"
#include "type0.h"
#include "cid.h"
#include "cid_p.h"
#include "cidtype2.h"

static int verbose = 0;

void
CIDFont_type2_set_verbose (void)
{
  verbose++;
}

/*
 * PDF_NAME_LEN_MAX: see, Appendix C of PDF Ref. v1.3, 2nd. ed.
 * This is Acrobat implementation limit.
 */
#define PDF_NAME_LEN_MAX 127

/*
 * PDF viewer applications use following tables (CIDFontType 2)
 *
 *  head, hhea, loca, maxp, glyf, hmtx, fpgm, cvt_, prep
 *
 *                                         - from PDF Ref. v.1.3, 2nd ed.
 *
 * The fpgm, cvt_, and prep tables appears only when TrueType instructions
 * requires them. Those tables must be preserved if they exist.
 * We use must_exist flag to indicate `preserve it if present'
 * and to make sure not to cause an error when it does not exist.
 *
 * post and name table must exist in ordinary TrueType font file,
 * but when a TrueType font is converted to CIDFontType 2 font, those tables
 * are no longer required.
 *
 * The OS/2 table (required for TrueType font for Windows and OS/2) contains
 * liscencing information, but PDF viewers seems not using them.
 *
 * TODO:
 *  Adobe says that vmtx is not used by PDF viewers. We must extract
 *  information corresponds to W2 and DW2 in font dictionary from vmtx and
 *  bounding box of each glyphs.
 */

static struct
{
  char *name;
  int   must_exist;
} required_table[] = {
  {"OS/2", 1}, {"head", 1}, {"hhea", 1}, {"loca", 1}, {"maxp", 1},
  {"glyf", 1}, {"hmtx", 1}, {"fpgm", 0}, {"cvt ", 0}, {"prep", 0},
  {NULL, 0}
};

/*
 * In PDF 1.2 or higher
 *
 *  All characters not printable in ASCII and some characters that have
 *  specail meaning in PDF must be converted to #xx form in name string,
 *  where xx is a hexadecimal notion of a character code.
 *  The null character (0) is not allowed.
 *
 * In PDF 1.1
 *
 *  Remove all characters which can not be used in name string.
 *
 *  The new name strings does not contains '\0' in the middle of them.
 *
 * We does not support PDF-1.1 output.
 */

static void
validate_name (char **name, int len)
{
  char *tmp;
  int   i, pos;

  if (len > PDF_NAME_LEN_MAX)
    ERROR("Name string length too large");

  /* eg, 0x5b -> #5b */
  tmp = NEW(3*len + 1, char);

  pos = 0;
  for (i = 0; i < len; i++) {
    if (*(*name+i) == 0)
      continue;
    else if (*(*name+i) < '!' || *(*name+i) > '~' ||
	     /*         ^ `space' is here */
	     strchr("/()[]<>{}#", *(*name+i)) != NULL) {
      sprintf(tmp+pos, "#%02x", (unsigned char) *(*name+i));
      pos += 3;
    } else {
      *(tmp+pos) = *(*name+i);
      pos++;
    }
  }

  if (pos != len)
    *name = RENEW(*name, pos+1, char);
  memmove(*name, tmp, pos);
  *(*name+pos) = '\0';

  RELEASE(tmp);

  /* For some fonts that have bad PS name. ad hoc. remove me. */
  if ((tmp = strstr(*name, "-WIN-RKSJ-H")) != NULL &&
      (tmp != *name)) {
    *tmp = '\0';
  } else if ((tmp = strstr(*name, "-WINP-RKSJ-H")) != NULL &&
	     (tmp != *name)) {
    *tmp = '\0';
  } else if ((tmp = strstr(*name, "-WING-RKSJ-H")) != NULL &&
	     (tmp != *name)) {
    *tmp = '\0';
  } else if ((tmp = strstr(*name, "-90pv-RKSJ-H")) != NULL &&
	     (tmp != *name)) {
    *tmp = '\0';
  }

  if (strlen(*name) == 0)
    ERROR("No valid character found in name string.");
}

/*
 * We will follow the convension for finding ToUnicode CMap found in PDF Reference
 * 4th ed., page 432. The name of "ToCode" (not limited to Unicode here) CMap is
 * obtained by concatenating the registry, ordering, and the name of encoding.
 */

#define WIN_UCS2 0
static struct
{
  unsigned short  platform;
  unsigned short  encoding;
  char    *pdfname;
} known_encodings[] = {
  {TT_WIN, TT_WIN_UNICODE,  "UCS2"},                /* -,   UNI  (WIN) */
  {TT_WIN, TT_WIN_SJIS,     "90ms-RKSJ"},           /* JPN, SJIS (WIN) */
  {TT_WIN, TT_WIN_RPC,      "GBK-EUC"},             /* GB,  RPC  (WIN) */ 
  {TT_WIN, TT_WIN_BIG5,     "ETen-B5"},             /* CNS, BIG5 (WIN) */
  {TT_WIN, TT_WIN_WANSUNG,  "KSCms-UHC"},           /* KRN, ?    (WIN) */
  {TT_MAC, TT_MAC_JAPANESE, "90pv-RKSJ"},           /* JPN, SJIS (MAC) */
  {TT_MAC, TT_MAC_TRADITIONAL_CHINESE, "B5pc"},     /* CNS, BIG5 (MAC) */
  {TT_MAC, TT_MAC_SIMPLIFIED_CHINESE,  "GBpc-EUC"}, /* GB,  RPC  (MAC) */ 
  {TT_MAC, TT_MAC_KOREAN,   "KSCpc-EUC"},           /* KRN, ?    (MAC) */
  {0, 0, NULL}
};

static CMap *
find_tocode_cmap (char *reg, char *ord, char *encoding)
{
  int   len, id;
  char *str;

  if ((reg == NULL) || (ord == NULL) || (encoding == NULL))
    ERROR("Character set unknown.");

  /*
   * UCS2 to UCS2 conversion is Identity.
   */
#if 0
  if ((strlen(ord) > 3 && !strcmp(ord+strlen(ord)-3, "UCS")) ||
      (strlen(ord) > 4 && !strcmp(ord+strlen(ord)-4, "UCS2")))
    return NULL;
#else
  if (!strcmp(ord, "UCS") || !strcmp(ord, "UCS2"))
    return NULL;
#endif

  len = strlen(reg) + strlen(ord) + strlen(encoding) + 2;
  if (len > PDF_NAME_LEN_MAX)
    ERROR("name string length too large");
  str = NEW(len + 1, char);
  sprintf(str, "%s-%s-%s", reg, ord, encoding);
  id = CMap_cache_find(str); /* id = -1 for not found */
  if (id < 0)
    ERROR("Could not find \"ToCode\" CMap \"%s\".", str);
  RELEASE(str);

  return CMap_cache_get(id);
}

static void
do_w_array (pdf_obj *w_array, struct tt_glyphs *g, unsigned short dw,
	    unsigned char *cidtogidmap, CID last_cid)
{
  CID cid, start = 0, prev = 0;
  unsigned short  gid;
  pdf_obj *an_array = NULL;
  double   width;

  cid = 0;
  do {
    gid = (cidtogidmap[2*cid] << 8)|cidtogidmap[2*cid+1];
    width = (g->used_glyphs)[gid].width; /* gid == new_gid here */
    if (width == dw) {
      if (an_array) {
	pdf_add_array(w_array, pdf_new_number(start));
	pdf_add_array(w_array, an_array);
	an_array = NULL;
      }
    } else {
      if (cid != prev + 1) {
	if (an_array) {
	  pdf_add_array(w_array, pdf_new_number(start));
	  pdf_add_array(w_array, an_array);
	  an_array = NULL;
	}
      }
      if (an_array == NULL) {
	an_array = pdf_new_array();
	start = cid;
      }
      pdf_add_array(an_array, pdf_new_number(ROUND(1000.0*(width)/(g->emsize), 1)));
      prev = cid;
    }
  } while (cid++ != last_cid);

  if (an_array) {
    pdf_add_array(w_array, pdf_new_number(start));
    pdf_add_array(w_array, an_array);
  }

  /*
   * W array may not contain any objects. In this case, the value of W array
   * becomes null object, then the entry W is omitted as described in PDF
   * reference v.1.3, section 3.2.8..
   */
}

void
CIDFont_type2_dofont (CIDFont *font)
{
  sfnt    *sfont;
  pdf_obj *stream_dict;
  char    *stream_data;
  char    *h_used_chars, *v_used_chars, *used_chars;
  struct tt_glyphs *glyphs;
  int      parent_id;
  CMap    *cmap = NULL;
  tt_cmap *ttcmap = NULL;
  unsigned long   offset = 0, length = 0;
  CID      cid, last_cid;
  unsigned char   cidtogidmap[(CID_MAX+1)*2];


  if (!font->indirect)
    return;

  pdf_add_dict(font->fontdict, 
	       pdf_new_name("FontDescriptor"), pdf_ref_obj(font->descriptor));

  if (CIDFont_is_BaseFont(font))
    return;

  /*
   * CIDSystemInfo comes here since Supplement can be increased.
   */
  {
    pdf_obj *csi_dict = pdf_new_dict ();

    pdf_add_dict(csi_dict,
		 pdf_new_name("Registry"),
		 pdf_new_string(font->csi->registry, strlen(font->csi->registry)));
    pdf_add_dict(csi_dict,
		 pdf_new_name("Ordering"),
		 pdf_new_string(font->csi->ordering, strlen(font->csi->ordering)));
    pdf_add_dict(csi_dict,
		 pdf_new_name("Supplement"),
		 pdf_new_number(font->csi->supplement));
    pdf_add_dict(font->fontdict, pdf_new_name("CIDSystemInfo"), csi_dict);
  }

  /*
   * It does not write W nor W2. This is bug.
   */
  if (!CIDFont_get_embedding(font))
    return;

  if ((sfont = sfnt_open(font->filename)) == NULL)
    ERROR("Could not open TTF file: %s", font->filename);

  switch (sfont->type) {
  case SFNT_TYPE_TTC:
    if ((offset = ttc_read_offset(sfont, font->options->index)) == 0)
      ERROR("Invalid TTC index in %s.", font->ident);
    break;
  case SFNT_TYPE_TRUETYPE:
    if (font->options->index > 0)
      ERROR("Found TrueType font file while expecting TTC file (%s).", font->ident);
    offset = 0;
    break;
  default:
    ERROR("Not a TrueType/TTC font (%s)?", font->ident);
    break;
  }

  if (sfnt_read_table_directory(sfont, offset) < 0)
    ERROR("Could not read TrueType table directory (%s).", font->filename);

  /*
   * Select TrueType cmap table, find ToCode CMap for each TrueType encodings.
   */
  {
    int i;
    /*
     * This part contains a bug. It may choose SJIS encoding TrueType cmap
     * table for Adobe-GB1.
     */
    for (i = 0; known_encodings[i].pdfname; i++) {
      if ((ttcmap = tt_cmap_read(sfont,
				 known_encodings[i].platform,
				 known_encodings[i].encoding)) != NULL) {
	break;
      }
    }
    if (ttcmap == NULL)
      ERROR("No acceptable TrueType cmap table found (%s).", font->filename);
    /*
     * NULL is returned if CMap is Identity CMap.
     */
    cmap = find_tocode_cmap(font->csi->registry,
			    font->csi->ordering,
			    known_encodings[i].pdfname);
  }

  glyphs = tt_build_init();
  used_chars = NULL;

  /*
   * Map CIDs to GIDs.
   * Horizontal and vertical used_chars are merged.
   */
  memset(cidtogidmap, 0, 2*(CID_MAX+1));
  last_cid = 0;
  used_chars = h_used_chars = v_used_chars = NULL;
  /*
   * Horizontal
   */
  if ((parent_id = CIDFont_get_parent_id(font, 0)) >= 0) {
    Type0Font *parent = Type0Font_cache_get(parent_id);

    if (!parent || !(h_used_chars = Type0Font_get_usedchars(parent)))
      ERROR("Unexpected error.");
    used_chars = h_used_chars;
    cid = 1;
    do {
      if (is_used_char2(h_used_chars, cid)) {
	unsigned short code, gid;
	code = cmap ? CMap_lookup_char2(cmap, cid) : cid;
	gid  = tt_cmap_lookup(ttcmap, code);
	if (gid == 0)
	  WARN("Glyph missing in font. (CID=%u, code=%u)", cid, code);
	if (last_cid < cid)
	  last_cid = cid;
	gid = tt_add_to_used(glyphs, gid);
	cidtogidmap[2*cid  ] = gid >> 8;
	cidtogidmap[2*cid+1] = gid & 0xff;
      }
    } while (cid++ != CID_MAX);
  }

  /*
   * Vertical:
   *
   *  Clearly, current implementation does not work well with Unicode.
   *  Characters that is also used in vertical writing may be replaced by
   *  vertical one if vertical version of glyphs are available.
   */
  if ((parent_id = CIDFont_get_parent_id(font, 1)) >= 0) {
    Type0Font *parent = Type0Font_cache_get(parent_id);
    tt_gsub_t  gsub   = NULL;

    if (!parent || !(v_used_chars = Type0Font_get_usedchars(parent)))
      ERROR("Unexpected error.");
    /*
     * Require `vrt2' or `vert'.
     */
    if ((gsub = tt_gsub_require_feature(sfont, "vrt2")) == NULL &&
	(gsub = tt_gsub_require_feature(sfont, "vert")) == NULL)
      WARN("GSUB feature vrt2/vert not found.");
    cid = 1;
    do {
      if (is_used_char2(v_used_chars, cid) &&
	  !(h_used_chars && is_used_char2(h_used_chars, cid))) {
	unsigned short code, gid;
	code = cmap ? CMap_lookup_char2(cmap, cid) : cid;
	gid  = tt_cmap_lookup(ttcmap, code);
	if (gid == 0)
	  WARN("Glyph missing in font. (CID=%u, code=%u)", cid, code);
	if (gsub)
	  tt_gsub_substitute(gsub, &gid);
	if (last_cid < cid)
	  last_cid = cid;
	gid = tt_add_to_used(glyphs, gid);
	if (used_chars)
	  add_to_used_chars2(used_chars, cid);
	cidtogidmap[2*cid  ] = gid >> 8;
	cidtogidmap[2*cid+1] = gid & 0xff;
      }
    } while (cid++ != CID_MAX);
    if (gsub)
      tt_gsub_release(gsub);
    if (used_chars == NULL) /* We have no horizontal. */
      used_chars = v_used_chars;
  }
  if (used_chars == NULL)
    ERROR("Unexpected error.");
  tt_cmap_release(ttcmap);

  if (tt_build_tables(sfont, glyphs) < 0)
    ERROR("Could not created FontFile stream.");
  if (verbose > 1)
    MESG("[%u glyphs (Max CID: %u)]", glyphs->num_glyphs, last_cid);

#define PDFUNIT(w) ROUND(1000.0*((double)(w))/glyphs->emsize, 1)


  /*
   * DW and W
   */
  pdf_add_dict(font->fontdict,
	       pdf_new_name("DW"),
	       pdf_new_number(PDFUNIT((glyphs->used_glyphs)[0].width)));
  {
    pdf_obj *w_array;

    w_array = pdf_new_array();
    do_w_array(w_array, glyphs, (glyphs->used_glyphs)[0].width, cidtogidmap, last_cid);
    pdf_add_dict(font->fontdict,
		 pdf_new_name ("W"),
		 pdf_link_obj(pdf_ref_obj(w_array)));
    pdf_release_obj(w_array);
  }

  /*
   * Finally create font.
   */
  tt_build_finish(glyphs);
  {
    int i;
    for (i=0; required_table[i].name; i++) {
      if (sfnt_require_table(sfont,
			     required_table[i].name,
			     required_table[i].must_exist) < 0) {
	ERROR("Some required TrueType table does not exist.");
      }
    }
  }
  length = sfnt_get_size(sfont);
  stream_data = NEW(length, char);
  if (sfnt_build_font(sfont, stream_data, length) == NULL)
    ERROR("Could not created FontFile stream.");

  sfnt_close(sfont);

  /*
   * Now flush all PDF objects.
   */

  /*
   * CIDSet
   */
  {
    pdf_obj *cidset;

    cidset = pdf_new_stream(STREAM_COMPRESS);
    pdf_add_stream(cidset, used_chars, last_cid/8 + 1);
    pdf_add_dict(font->descriptor,
		 pdf_new_name("CIDSet"),
		 pdf_link_obj(pdf_ref_obj(cidset)));
    pdf_release_obj(cidset);
  }

  /*
   * CIDToGIDMap
   */
  {
    pdf_obj *c2gmstream;

    c2gmstream = pdf_new_stream(STREAM_COMPRESS);
    pdf_add_stream(c2gmstream, cidtogidmap, (last_cid+1)*2);
    pdf_add_dict(font->fontdict,
		 pdf_new_name("CIDToGIDMap"),
		 pdf_ref_obj(c2gmstream));
    pdf_release_obj(c2gmstream);
  }

  /*
   * FontFile2
   */
  {
    pdf_obj *fontfile;

    fontfile = pdf_new_stream(STREAM_COMPRESS);
    stream_dict = pdf_stream_dict(fontfile);
    pdf_add_dict(stream_dict,
		 pdf_new_name("Length1"),
		 pdf_new_number(length));
    pdf_add_stream(fontfile, stream_data, length);
    pdf_add_dict(font->descriptor,
		 pdf_new_name("FontFile2"),
		 pdf_link_obj(pdf_ref_obj(fontfile)));
    pdf_release_obj(fontfile);
    RELEASE(stream_data); /* add_stream does memcpy */
  }

  /*
   * Everything is now ok.
   */
  if (verbose > 1)
    MESG("[%ld bytes]", length);

  return;
}

int
CIDFont_type2_open (CIDFont *font, char *name, CIDSysInfo *cmap_csi, cid_opt *opt)
{
  char    *fullname, *fontname;
  sfnt    *sfont;
  unsigned long   offset = 0;

  ASSERT(font);

  if ((fullname = kpse_find_file(name, kpse_truetype_format, 1)) == NULL)
    return -1;

  if ((sfont = sfnt_open(fullname)) == NULL)
    return -1;

  switch (sfont->type) {
  case SFNT_TYPE_TTC:
    offset = ttc_read_offset(sfont, opt->index);
    break;
  case SFNT_TYPE_TRUETYPE:
    if (opt->index > 0)
      return -1;
    else
      offset = 0;
    break;
  default:
    sfnt_close(sfont);
    return -1;
    break;
  }

  if (sfnt_read_table_directory(sfont, offset) < 0)
    ERROR("Reading TrueType table directory failed.");

  {
    char *short_fontname;
    long  namelen;
    /* MAC-ROMAN-EN-POSTSCRIPT or WIN-UNICODE-EN(US)-POSTSCRIPT */
    short_fontname = NEW(PDF_NAME_LEN_MAX, char);
    namelen = tt_get_ps_fontname(sfont, short_fontname, PDF_NAME_LEN_MAX);
    if (namelen == 0) {
      memset(short_fontname, 0, PDF_NAME_LEN_MAX);
      strncpy(short_fontname, name, PDF_NAME_LEN_MAX);
      namelen = strlen(short_fontname);
    }
    validate_name(&short_fontname, namelen); /* for SJIS, UTF-16, ... string */
    /*
     * Strlen works, after validate_named string.
     * Mangled name requires more 7 bytes.
     * Style requires more 11 bytes.
     */
    fontname = NEW(strlen(short_fontname)+19, char);
    strcpy(fontname, short_fontname);
    RELEASE(short_fontname);
  }

  if (opt->embed && opt->style != FONT_STYLE_NONE) {
    WARN("Embedding disabled due to style option for %s.", name);
    opt->embed = 0;
  }
  switch (opt->style) {
  case FONT_STYLE_BOLD:
    strcat(fontname, ",Bold");
    break;
  case FONT_STYLE_ITALIC:
    strcat(fontname, ",Italic");
    break;
  case FONT_STYLE_BOLDITALIC:
    strcat(fontname, ",BoldItalic");
    break;
  }
  /*
   * CIDSystemInfo is determined from CMap or from map record option.
   */
  font->fontname = fontname;
  font->filename = strdup(fullname);
  font->subtype  = CIDFONT_TYPE2;
  font->csi      = NEW(1, CIDSysInfo);
  if (opt->csi) {
    if (cmap_csi) {
      if (strcmp(opt->csi->registry, cmap_csi->registry) ||
	  strcmp(opt->csi->ordering, cmap_csi->ordering)) {
	MESG("\nCharacter collection mismatched:\n");
	MESG("\tFont: %s-%s-%d\n", opt->csi->registry, opt->csi->ordering, opt->csi->supplement);
	MESG("\tCMap: %s-%s-%d\n", cmap_csi->registry, cmap_csi->ordering, cmap_csi->supplement);
	ERROR("Incompatible CMap specified for this font.");
      }
      if (opt->csi->supplement < cmap_csi->supplement) {
	WARN("Supplmement value in CIDSystemInfo increased.");
	WARN("Some characters may not shown.");
	opt->csi->supplement = cmap_csi->supplement;
      }
    }
    font->csi->registry   = strdup(opt->csi->registry);
    font->csi->ordering   = strdup(opt->csi->ordering);
    font->csi->supplement = opt->csi->supplement;
  } else if (cmap_csi) {
    font->csi->registry   = strdup(cmap_csi->registry);
    font->csi->ordering   = strdup(cmap_csi->ordering);
    font->csi->supplement = cmap_csi->supplement;
  } else
    ERROR("Cannot create CIDFontType 2 font from TrueType font without user-supplied CIDSystemInfo.");

  font->fontdict = pdf_new_dict();
  pdf_add_dict(font->fontdict,
	       pdf_new_name ("Type"),
	       pdf_new_name ("Font"));
  pdf_add_dict(font->fontdict,
	       pdf_new_name ("Subtype"),
	       pdf_new_name ("CIDFontType2"));

  if ((font->descriptor
       = tt_get_fontdesc(sfont, &(opt->embed), 2)) == NULL)
    ERROR("Could not obtain neccesary font info.");

  if (opt->embed)
    mangle_name(fontname);

  pdf_add_dict(font->descriptor,
	       pdf_new_name("FontName"),
	       pdf_new_name(fontname));
  pdf_add_dict(font->fontdict, 
	       pdf_new_name ("BaseFont"),
	       pdf_new_name (fontname));

  sfnt_close(sfont);

  /*
   * Don't write fontdict here.
   * CIDSystemInfo -> Supplement may change.
   */
  return 0;
}

void
CIDFont_type2_release (CIDFont *font)
{
  return;
}
