/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/type1c.c,v 1.1 2003/12/27 18:03:16 csc Exp $

    This is dvipdfmx, an eXtended version of dvipdfm by Mark A. Wicks.

    Copyright (C) 2002 by Jin-Hwan Cho and Shunsaku Hirata,
    the dvipdfmx project team <dvipdfmx@project.ktug.or.kr>

    Copyright (C) 1998, 1999 by Mark A. Wicks <mwicks@kettering.edu>

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
 * CFF/OpenType Font support:
 *
 * Encoding:
 *
 *  Many CFF/OpenType does not have meaningful/correct encoding.
 *  Encoding must be expilicitly supplied in the fontmap.
 *
 * Limitation:
 *
 *  Unused string is not removed from String Index.
 *
 * BUGS:
 *  Many.
 */ 

#include <string.h>

#include "system.h"
#include "mfileio.h"
#include "mem.h"
#include "error.h"
#include "pdfobj.h"

#include "tfm.h"
#include "encodings.h"
#include "unicode.h"

/* mangle_name() */
#include "dpxutil.h"

/* Font info. from OpenType tables */
#include "sfnt.h"
#include "tt_aux.h"

#include "cff_types.h"
#include "cff_limits.h"
#include "cff.h"
#include "cff_dict.h"
#include "cs_type2.h"

#include "type1c.h"

#ifndef kpse_opentype_format
#define kpse_opentype_format kpse_program_binary_format
#endif

static int __verbose = 0;

#define TYPE1CFONT_DEBUG_STR "Type1C"
#define TYPE1CFONT_DEBUG     3

void
Type1CFont_set_verbose (void)
{
  __verbose++;
}

struct Type1CFont 
{
  char    *ident;
  char    *fontname;
  char    *filename;
  int      embed;
  int      encoding_id;
  char    *used_chars;
  /* PDF Objects */
  pdf_obj *indirect;
  pdf_obj *fontdict;
  pdf_obj *descriptor;
};

Type1CFont *
Type1CFont_new (void)
{
  Type1CFont *font;

  font = NEW(1, Type1CFont);
  font->ident    = NULL;
  font->fontname = NULL;
  font->filename = NULL;
  font->embed    = 1;
  font->encoding_id = -1;
  font->used_chars  = NULL;

  font->indirect   = NULL;
  font->fontdict   = NULL;
  font->descriptor = NULL;

  return font;
}

void
Type1CFont_flush (Type1CFont *font)
{
  if (!font)
    return;

  if (font->indirect)   pdf_release_obj(font->indirect);
  if (font->fontdict)   pdf_release_obj(font->fontdict);
  if (font->descriptor) pdf_release_obj(font->descriptor);

  font->indirect   = NULL;
  font->fontdict   = NULL;
  font->descriptor = NULL;

  return;
}

void
Type1CFont_release (Type1CFont *font)
{
  if (!font)
    return;

  if (font->ident)      RELEASE(font->ident);
  if (font->fontname)   RELEASE(font->fontname);
  if (font->filename)   RELEASE(font->filename);
  if (font->used_chars) RELEASE(font->used_chars);

  if (font->indirect)   ERROR("%s: Object not flushed.", TYPE1CFONT_DEBUG_STR);
  if (font->fontdict)   ERROR("%s: Object not flushed.", TYPE1CFONT_DEBUG_STR);
  if (font->descriptor) ERROR("%s: Object not flushed.", TYPE1CFONT_DEBUG_STR);

  return;
}

static char *
new_used_chars (void)
{
  char *used_chars;;

  used_chars = NEW(256, char);
  memset(used_chars, 0, 256*sizeof(char));

  return used_chars;
}

pdf_obj *
Type1CFont_get_resource (Type1CFont *font)
{
  ASSERT(font);

  if (!font->indirect)
    font->indirect = pdf_ref_obj(font->fontdict);

  return pdf_link_obj(font->indirect);
}

char *
Type1CFont_get_usedchars (Type1CFont *font)
{
  ASSERT(font);

  return font->used_chars;
}

int
Type1CFont_open (Type1CFont *font, const char *name, int encoding_id, int embed)
{
  char       *fontname;
  sfnt       *sfont;
  cff_font   *cff;
  char       *fullname = NULL;
  unsigned long offset = 0;

  ASSERT(font);

  {
    char *shortname;

    shortname = NEW(strlen(name)+5, char);
    memset(shortname, 0, strlen(name)+5);
    strcpy(shortname, name);
    if (strlen(shortname) < 5 || strcmp(shortname+strlen(shortname)-4, ".otf"))
      strcat(shortname, ".otf");
#ifdef MIKTEX
    if (!miktex_find_app_input_file("dvipdfm", shortname, fullname = work_buffer)) {
      RELEASE(shortname);
      return -1;
    }
#else
    if ((fullname = kpse_find_file(shortname, kpse_opentype_format, 1))
	== NULL) {
      RELEASE(shortname);
      return -1;
    }
#endif
    RELEASE(shortname);
  }

  if ((sfont = sfnt_open(fullname)) == NULL   ||
      sfont->type != SFNT_TYPE_POSTSCRIPT     ||
      sfnt_read_table_directory(sfont, 0) < 0 ||
      (offset = sfnt_find_table_pos(sfont, "CFF ")) == 0)
    ERROR("%s: Not a CFF/OpenType font?", TYPE1CFONT_DEBUG_STR);

  if ((cff = cff_open(sfont->stream, offset, 0)) == NULL)
    ERROR("%s: Cannot read CFF font data", TYPE1CFONT_DEBUG_STR);

  if (cff->flag & FONTTYPE_CIDFONT) {
    cff_close(cff);
    sfnt_close(sfont);
    return -1;
  }

  {
    char *short_fontname = cff_get_name(cff);
    if (!short_fontname)
      ERROR("%s: No valid FontName found.", TYPE1CFONT_DEBUG_STR);
    /*
     * Mangled name requires more 7 bytes.
     * Style requires more 11 bytes.
     */
    fontname = NEW(strlen(short_fontname)+19, char);
    memset(fontname, 0, strlen(short_fontname)+19);
    strcpy(fontname, short_fontname);
    RELEASE(short_fontname);
  }
  cff_close(cff);

  /*
   * Font like AdobePiStd does not have meaningful built-in encoding.
   * Some software generate CFF/OpenType font with incorrect encoding.
   */
  if (encoding_id < 0)
    WARN("Built-in encoding used for CFF/OpenType.");

  font->ident       = strdup(name);
  font->fontname    = fontname;
  font->filename    = strdup(fullname);
  font->encoding_id = encoding_id;
  font->embed       = embed;
  font->fontdict    = pdf_new_dict();

  pdf_add_dict(font->fontdict,
	       pdf_new_name("Type"),
	       pdf_new_name("Font"));
  pdf_add_dict(font->fontdict,
	       pdf_new_name("Subtype"),
	       pdf_new_name("Type1"));

  /*
   * Create font descriptor from OpenType tables.
   * We can also use CFF TOP DICT/Private DICT for this.
   */
  if ((font->descriptor
       = tt_get_fontdesc(sfont, &(font->embed), 1)) == NULL)
    ERROR("%s: Could not obtain neccesary font info from OpenType table.", TYPE1CFONT_DEBUG_STR);

  if (font->embed)
    mangle_name(font->fontname);

  pdf_add_dict(font->fontdict,
	       pdf_new_name("Type"),
	       pdf_new_name("Font"));
  pdf_add_dict(font->fontdict,
	       pdf_new_name("Subtype"),
	       pdf_new_name("Type1"));
  pdf_add_dict(font->fontdict,
	       pdf_new_name("BaseFont"),
	       pdf_new_name(font->fontname));
  pdf_add_dict(font->descriptor,
	       pdf_new_name("FontName"),
	       pdf_new_name(font->fontname));
  if (font->embed)
    font->used_chars = new_used_chars();

  sfnt_close(sfont);

  return 0;
}

/*
 * TODO List:
 *
 *   1. Remove unused string from String INDEX.
 *   2. ToUnicode CMap for built-in encoding.
 *   ... not hard task but I don't have enough time to do.
 *
 *   3. Duplicated glyphs...
 */

void
Type1CFont_dofont (Type1CFont *font)
{
  pdf_obj   *stream_dict;
  sfnt      *sfont;
  char     **enc_vec;
  cff_font  *cff;
  cff_index *charstrings, *topdict, *cs_idx;
  cff_charsets *charset  = NULL;
  cff_encoding *encoding = NULL;
  long   topdict_offset, private_size;
  long   charstring_len, max_len;
  long   destlen = 0;
  long   size, offset = 0;
  card8 *dest, *data;
  card16 num_glyphs;
  card16 cs_count, code;

  ASSERT(font);

  if (!font->indirect)
    return;

  if (!font->embed)
    ERROR("%s: Only embedded font supported for CFF/OpenType font.", TYPE1CFONT_DEBUG_STR);

  /* 
   * Encoding is still unknown for font with encoding_id < 0.
   */
  if (font->encoding_id >= 0) {
    Encoding *enc = Encoding_cache_get(font->encoding_id);
    if (Encoding_is_predefined(enc)) {
      pdf_add_dict(font->fontdict,
		   pdf_new_name("Encoding"),
		   pdf_new_name(Encoding_get_name(enc)));
    } else {
#if 0
      /*
       * Gs not working with this.
       */
      pdf_add_dict(font->fontdict,
		   pdf_new_name("Encoding"),
		   Encoding_get_resource(enc));
#endif
      UC_add_ToUnicode_CMap(font->fontdict, Encoding_cache_get(font->encoding_id));
    }
  }

  pdf_add_dict(font->fontdict, 
	       pdf_new_name("FontDescriptor"),
	       pdf_link_obj(pdf_ref_obj(font->descriptor)));

  if (!Type1CFont_get_usedchars(font))
    ERROR("%s: Cannot create embedable font without used_chars.", TYPE1CFONT_DEBUG_STR);

  if ((sfont = sfnt_open(font->filename)) == NULL)
    ERROR("%s: Could not open OpenType font file: %s", TYPE1CFONT_DEBUG_STR, font->filename);
  if (sfnt_read_table_directory(sfont, 0) < 0)
    ERROR("%s: Could not read OpenType table directory.", TYPE1CFONT_DEBUG_STR);
  if (sfont->type != SFNT_TYPE_POSTSCRIPT ||
      (offset = sfnt_find_table_pos(sfont, "CFF ")) == 0)
    ERROR("%s: Not a CFF/OpenType font ?", TYPE1CFONT_DEBUG_STR);

  if ((cff = cff_open(sfont->stream, offset, 0)) == NULL)
    ERROR("%s: Could not open CFF font.", TYPE1CFONT_DEBUG_STR);
  if (cff->flag & FONTTYPE_CIDFONT)
    ERROR("%s: This is CIDFont...", TYPE1CFONT_DEBUG_STR);

  /* Offsets from DICTs */
  cff_read_charsets(cff);
  if (font->encoding_id < 0)
    cff_read_encoding(cff);
  cff_read_private(cff);

  cff_read_subrs(cff);

  offset = (long) cff_dict_get(cff->topdict, "CharStrings", 0);
  cff_seek_set(cff, offset);
  cs_idx = cff_get_index_header(cff);
  /* offset is now absolute offset ... bad */
  offset = ftell(cff->stream);
  
  if ((cs_count = cs_idx->count) < 2)
    ERROR("%s: No valid charstring data found.", TYPE1CFONT_DEBUG_STR);

  /* New Charsets data */
  charset = NEW(1, cff_charsets);
  charset->format      = 0;
  charset->num_entries = 0;
  charset->data.glyphs = NEW(256, s_SID);

  /* New Encoding data */
  encoding = NEW(1, cff_encoding);
  encoding->format      = 0;
  encoding->num_entries = 0;
  encoding->data.codes  = NEW(256, card8);
  encoding->num_supps   = 0;
  encoding->supp        = NULL;

  /* New CharStrings INDEX */
  charstrings       = cff_new_index(256);
  max_len           = 2 * CS_STR_LEN_MAX;
  charstrings->data = NEW(max_len, card8);
  charstring_len    = 0;

  data = NEW(CS_STR_LEN_MAX, card8);
  {
    /*
     * .notdef
     */
    if (__verbose > 2)
      MESG("[glyphs:/.notdef");

    if ((size = (cs_idx->offset)[1] - (cs_idx->offset)[0]) > CS_STR_LEN_MAX)
      ERROR("%s: Charstring too long: gid=%u", 0, TYPE1CFONT_DEBUG_STR);
    (charstrings->offset)[0] = charstring_len + 1;
    seek_absolute(cff->stream, offset + (cs_idx->offset)[0] - 1);
    fread(data, 1, size, cff->stream);
    charstring_len += cs_copy_charstring(charstrings->data + charstring_len,
					 max_len - charstring_len,
					 data, size,
					 cff->gsubr, (cff->subrs)[0]);
  }

  enc_vec = NULL;
  if (font->encoding_id < 0) {
    /*
     * Create enc_vec for built-in encoding.
     */
    enc_vec = NEW(256, char *);
    for (code = 0; code < 256; code++) {
      if (font->used_chars[code]) {
	card16 gid = cff_encoding_lookup(cff, code);
	enc_vec[code] = cff_get_string(cff, cff_charsets_lookup_inverse(cff, gid));
      } else
	enc_vec[code] = NULL;
    }
  } else
    enc_vec = Encoding_get_encoding(Encoding_cache_get(font->encoding_id));

  /*
   * ToUnicode CMap support not available for built-in encoding yet.
   */

  num_glyphs = 1;
  for (code = 0; code < 256; code++) {
    if (font->used_chars[code] && enc_vec[code] &&
	strcmp(enc_vec[code], ".notdef")) {
      unsigned short gid;
      s_SID          sid;
      sid = cff_get_sid(cff, enc_vec[code]);
      gid = cff_charsets_lookup(cff, sid);
      if (gid == 0) {
	WARN("Glyph \"%s\" missing in font \"%s\".", enc_vec[code], font->ident);
	WARN("Maybe incorrect encoding specified.");
	continue;
      }
      if (__verbose > 2)
        MESG("/%s", enc_vec[code]);
      if ((size = (cs_idx->offset)[gid+1] - (cs_idx->offset)[gid])
	  > CS_STR_LEN_MAX)
	ERROR("%s: Charstring too long: gid=%u", gid, TYPE1CFONT_DEBUG_STR);
      if (charstring_len + CS_STR_LEN_MAX >= max_len) {
	max_len = charstring_len + 2 * CS_STR_LEN_MAX;
	charstrings->data = RENEW(charstrings->data, max_len, card8);
      }
      (charstrings->offset)[num_glyphs] = charstring_len + 1;
      seek_absolute(cff->stream, offset + (cs_idx->offset)[gid] - 1);
      fread(data, 1, size, cff->stream);
      charstring_len += cs_copy_charstring(charstrings->data + charstring_len,
					   max_len - charstring_len,
					   data, size,
					   cff->gsubr, (cff->subrs)[0]);
      charset->data.glyphs[charset->num_entries]  = sid;
      charset->num_entries  += 1;
      encoding->data.codes[encoding->num_entries] = code;
      encoding->num_entries += 1;
      num_glyphs++;
    }
  }
  if (__verbose > 2)
    MESG("]");

  RELEASE(data);

  if (font->encoding_id < 0 && enc_vec) {
    for (code = 0; code < 256; code++)
      if (enc_vec[code])
	RELEASE(enc_vec[code]);
    RELEASE(enc_vec);
  }

  cff_release_index(cs_idx);

  (charstrings->offset)[num_glyphs] = charstring_len + 1;
  charstrings->count = num_glyphs;
  charstring_len     = cff_index_size(charstrings);
  cff->num_glyphs    = num_glyphs;

  /*
   * Discard old one, set new data.
   */
  cff_release_charsets(cff->charsets);
  cff->charsets = charset;
  if (cff->encoding)
    cff_release_encoding(cff->encoding);
  cff->encoding = encoding;

  /*
   * No Global subr
   */
  if (cff->gsubr)
    cff_release_index(cff->gsubr);
  cff->gsubr = cff_new_index(0);

  /*
   * Flag must be reset since cff_pack_encoding(charset) does not write
   * encoding(charset) if HAVE_STANDARD_ENCODING(CHARSET) is set. We are
   * re-encoding font.
   */
  cff->flag = FONTTYPE_FONT;
  /*  DICT sizes (offset set to long int) */
  topdict = cff_new_index(1);
  /*
   * Force existence of Encoding.
   */
  if (!cff_dict_known(cff->topdict, "Encoding"))
    cff_dict_add(cff->topdict, "Encoding", 1);
  (topdict->offset)[1] = cff_dict_pack(cff->topdict,
				       (card8 *) work_buffer,
				       WORK_BUFFER_SIZE) + 1;
  if (cff->subrs[0]) {
    cff_release_index(cff->subrs[0]);
    cff->subrs[0] = NULL;
  }
  private_size = 0;
  if ((cff->private)[0]) {
    cff_dict_remove((cff->private)[0], "Subrs"); /* no Subrs */
    private_size = cff_dict_pack((cff->private)[0],
				 (card8 *) work_buffer, WORK_BUFFER_SIZE);
  }

  destlen = 4; /* header size */
  destlen += cff_set_name(cff, font->fontname);
  destlen += cff_index_size(topdict);
  destlen += cff_index_size(cff->string);
  destlen += cff_index_size(cff->gsubr);
  destlen += (encoding->num_entries) + 1 + 1;  /* encoding format 0 */
  destlen += (charset->num_entries) * 2 + 1;   /* charset format 0 */
  destlen += charstring_len;
  destlen += private_size;

  dest = NEW(destlen, card8);

  offset = 0;
  /* Header */
  offset += cff_put_header(cff, dest + offset, destlen - offset);
  /* Name */
  offset += cff_pack_index(cff->name, dest + offset, destlen - offset);
  /* Top DICT */
  topdict_offset = offset;
  offset += cff_index_size(topdict);
  /* Strings */
  offset += cff_pack_index(cff->string, dest + offset, destlen - offset);
  /* Global Subrs */
  offset += cff_pack_index(cff->gsubr, dest + offset, destlen - offset);
  /* Encoding */
  cff_dict_set(cff->topdict, "Encoding", 0, offset);
  offset += cff_pack_encoding(cff, dest + offset, destlen - offset);
  /* charset */
  cff_dict_set(cff->topdict, "charset", 0, offset);
  offset += cff_pack_charsets(cff, dest + offset, destlen - offset);
  /* CharStrings */
  cff_dict_set(cff->topdict, "CharStrings", 0, offset);
  offset += cff_pack_index(charstrings, dest + offset, charstring_len);
  cff_release_index(charstrings);
  /* Private */
  cff_dict_set(cff->topdict, "Private", 1, offset);
  if ((cff->private)[0] && private_size > 0)
    private_size = cff_dict_pack((cff->private)[0], dest + offset, private_size);
  cff_dict_set(cff->topdict, "Private", 0, private_size);
  offset += private_size;

  /* Finally Top DICT */
  topdict->data = NEW(topdict->offset[1] - 1, card8);
  cff_dict_pack(cff->topdict, topdict->data, topdict->offset[1] - 1);
  cff_pack_index(topdict, dest + topdict_offset, cff_index_size(topdict));
  cff_release_index(topdict);

  /* Copyright and Trademark Notice ommited. */

  cff_close(cff);
  sfnt_close(sfont);

  if (__verbose > 1)
    MESG("[%u/%u glyphs][%ld bytes]", num_glyphs, cs_count, offset);

  /*
   * Maybe writing Charset is recommended for subsetted font.
   */

  /*
   * FontFile
   */
  {
    pdf_obj *fontfile;

    fontfile    = pdf_new_stream(STREAM_COMPRESS);
    stream_dict = pdf_stream_dict(fontfile);
    pdf_add_dict(font->descriptor,
		 pdf_new_name("FontFile3"),
		 pdf_link_obj(pdf_ref_obj(fontfile)));
    pdf_add_dict(stream_dict,
		 pdf_new_name("Subtype"),
		 pdf_new_name("Type1C"));
    pdf_add_stream(fontfile, (char *) dest, offset);
    pdf_release_obj(fontfile);
    RELEASE(dest);
  }

  return;
}


/******************************** CACHE ********************************/

#define CACHE_ALLOC_SIZE 16u

struct FontCache {
  int num;
  int max;
  Type1CFont **fonts;
};

static struct FontCache *__cache = NULL;

void
Type1CFont_cache_init (void)
{
  if (__cache)
    ERROR("%s: Already initialized.", TYPE1CFONT_DEBUG_STR);

  if (__verbose > TYPE1CFONT_DEBUG)
    MESG("%s: Initialize\n", TYPE1CFONT_DEBUG_STR);

  __cache = NEW(1, struct FontCache);
  __cache->num   = 0;
  __cache->max   = CACHE_ALLOC_SIZE;
  __cache->fonts = NEW(__cache->max, Type1CFont *);
}

#define CHECK_ID(n) do {\
                        if (! __cache)\
                           ERROR("%s: Font cache not initialized.", TYPE1CFONT_DEBUG_STR);\
                        if ((n) < 0 || (n) >= __cache->num)\
                           ERROR("%s: Invalid ID %d", TYPE1CFONT_DEBUG_STR, (n));\
                    } while (0)

Type1CFont *
Type1CFont_cache_get (int font_id)
{
  CHECK_ID(font_id);

  return __cache->fonts[font_id];
}

/*
 * tfm_id, remap ... 
 */
int
Type1CFont_cache_find (const char *map_name, char *res_name, int encoding_id, int tfm_id, int remap)
{
  Type1CFont *font;
  int      font_id;

  if (!__cache)
    Type1CFont_cache_init();
  ASSERT(__cache);

  for (font_id = 0; font_id < __cache->num; font_id++) {
    font = __cache->fonts[font_id];
    if (map_name && font->ident && !strcmp(font->ident, map_name)
	&& (encoding_id == font->encoding_id))
      return font_id;
  }

  font_id = __cache->num;
  font    = Type1CFont_new();
  if (Type1CFont_open(font, map_name, encoding_id, 1) < 0) {
    Type1CFont_release(font);
    return -1;
  }

  if (remap)
    ERROR("%s: Obsolete option remap not supported.", TYPE1CFONT_DEBUG_STR);

  /*
   * This section depends on TFM.
   */
  {
    pdf_obj *tmp1;
    int i, firstchar, lastchar;

    firstchar = tfm_get_firstchar(tfm_id);
    lastchar  = tfm_get_lastchar (tfm_id);
    pdf_add_dict(font->fontdict,
		 pdf_new_name("FirstChar"),
		 pdf_new_number(firstchar));
    pdf_add_dict(font->fontdict,
		 pdf_new_name("LastChar"),
		 pdf_new_number(lastchar));
    tmp1 = pdf_new_array();
    for (i = firstchar; i <= lastchar; i++) {
      pdf_add_array(tmp1,
		    pdf_new_number(ROUND(tfm_get_width(tfm_id, i)*1000.0,1.)));
    }
    pdf_add_dict(font->fontdict, pdf_new_name("Widths"), tmp1);
  }

  if (__cache->num >= __cache->max) {
    __cache->max  += CACHE_ALLOC_SIZE;
    __cache->fonts = RENEW(__cache->fonts, __cache->max, Type1CFont *);
  }
  __cache->fonts[font_id] = font;
  (__cache->num)++;

  return font_id;
}

void
Type1CFont_cache_close (void)
{
  if (__cache) {
    int i;
    for (i = 0; i < __cache->num; i++) {
      Type1CFont *font = __cache->fonts[i];
      if (__verbose) {
	MESG("(Type1C:%s", font->ident);
	if (__verbose > 1)
	  MESG("[%s][%s]", font->filename, font->fontname);
      }
      Type1CFont_dofont (font);
      Type1CFont_flush  (font);
      Type1CFont_release(font);
      if (__verbose)
	MESG(")");
    }
    RELEASE(__cache);
  }
  if (__verbose > TYPE1CFONT_DEBUG)
    MESG("%s: Close\n", TYPE1CFONT_DEBUG_STR);
}


/******************************** COMPAT ********************************/

void
type1c_disable_partial (void)
{
  WARN("Only subsetted embedding supported for CFF/OpenType font.");
}

void
type1c_set_verbose (void)
{
  Type1CFont_set_verbose();
}

pdf_obj *
type1c_font_resource (int font_id)
{
  return Type1CFont_get_resource(Type1CFont_cache_get(font_id));
}

char *
type1c_font_used (int font_id)
{
  return Type1CFont_get_usedchars(Type1CFont_cache_get(font_id));
}

int
type1c_font (const char *tex_name, int tfm_id, char *resource_name, int encoding_id, int remap)
{
  return Type1CFont_cache_find(tex_name, resource_name, encoding_id, tfm_id, remap);
}

void
type1c_set_mapfile (const char *name)
{
  return;
}

void
type1c_close_all (void)
{
  Type1CFont_cache_close();
}
