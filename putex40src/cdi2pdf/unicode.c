/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/unicode.c,v 1.1 2003/12/27 18:03:16 csc Exp $

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
 * Unicode related:
 *  Conversion between UTF-* and UCS-*.
 *  ToUnicode CMap
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>

#include "system.h"
#include "mem.h"
#include "error.h"

#include "pdfobj.h"
#include "pdfresource.h"

#include "encodings.h"
#include "agl.h"
#include "unicode.h"

static int __verbose = 0;

#define UC_DEBUG     3
#define UC_DEBUG_STR "UC"

static struct
{
  struct {
    long lower; /* lower bound */
    long upper; /* upper bound */
  } range;
  char tag;
} utf8def[] = {
  { {0x00000000, 0x0000007f}, 0x00 },
  { {0x00000080, 0x000007ff}, 0xc0 },
  { {0x00000800, 0x0000ffff}, 0xe0 },
  { {0x00010000, 0x001fffff}, 0xf0 },
  { {0x00200000, 0x03ffffff}, 0xfc },
  { {0x04000000, 0x7fffffff}, 0xfe }
};

int
UC_put_utf8 (char **s, long ucv)
{
  int  idx = 0;
  char tag;

  while (idx < 6 && ucv > utf8def[idx].range.upper) {
    idx++;
  } 

  if (idx == 6)
    return 0;

  tag = utf8def[idx].tag;
  do {
    if (idx == 0)
      *((*s)+idx) = ((char) ucv) | tag;
    else
      *((*s)+idx) = (((char) ucv) & 0x3f) | 0x80;
    ucv >>= 6;
  } while (idx-- > 0);

  *s += idx+1;
  return idx+1;
}

int
UC_is_valid (unsigned short ucv)
{
  if (ucv > 0xD7FFu && ucv < 0xE000)
    return 0;
  return 1;
}

/*
 * ToUnicode CMap
 */

#define TOUNICODE_PART1 "\
/CIDInit /ProcSet findresource begin\n\
12 dict begin\n\
begincmap\n\
/CIDSystemInfo <<\n\
/Registry (Adobe)\n/Ordering (UCS)\n/Supplement 0\n\
>> def\n\
/CMapType 2 def\n\
"
/*
 * PAET1.99: CMapName.
 */
#define TOUNICODE_PART2 "\
1 begincodespacerange\n\
<00> <FF>\n\
endcodespacerange\n\
"
/*
 * PART3: CMap mapping.
 */
#define TOUNICODE_PART4 "\
endcmap\n\
CMapName currentdict /CMap defineresource pop\n\
end\n\
end\n\
"

/*
 * The wbuf requires at least 1217 bytes.
 *  100 beginbfchar
 *  <XX> <XXXX>
 *  ... 100 bfchar entries ...
 *  endbfchar
 */
pdf_obj *
UC_make_ToUnicode_CMap (char *cmap_name, char **enc_vec, AGLmap *aglm)
{
  pdf_obj  *cmap;
  char      wbuf[4096], *p;
  int       code, count;

  if (!cmap_name || !enc_vec)
    ERROR("%s: CMap name/Encoding vector not defined.", UC_DEBUG_STR);

  if (!aglm) {
    int aglm_id;
    aglm_id = AGLmap_cache_find(AGLMAP_DEFAULT_GLYPHLIST);
    if (aglm_id >= 0)
      aglm = AGLmap_cache_get(aglm_id);
    if (!aglm) {
      if (__verbose > UC_DEBUG)
	WARN("%s: Glyph name to Unicode mapping not available.", UC_DEBUG_STR);
      return NULL;
    }
  }

  cmap = pdf_new_stream(STREAM_COMPRESS);

  pdf_add_stream(cmap, TOUNICODE_PART1, strlen(TOUNICODE_PART1));
  sprintf(wbuf, "/CMapName /%s def\n", cmap_name);
  pdf_add_stream(cmap, wbuf, strlen(wbuf));
  pdf_add_stream(cmap, TOUNICODE_PART2, strlen(TOUNICODE_PART2));

  count = 0;
  p     = wbuf;
  for (code = 0; code < 256; code++) {
    if (enc_vec[code] && strcmp(enc_vec[code], ".notdef")) {
      AGList  *agl;
      unsigned short  ucv;
      if (AGName_is_unicode(enc_vec[code]))
	ucv = AGName_convert_uni(enc_vec[code]);
      else if ((agl = AGLmap_lookup(aglm, enc_vec[code])) != NULL)
	ucv = AGList_get_code(agl);
      if (!UC_is_valid(ucv))
	WARN("%s: Glyph \"%s\" mapped to invalid Unicode value U+%04X.",
	     enc_vec[code], ucv);
      else {
	p += sprintf(p, "<%02X> <%04X>\n", code, ucv);
	count++;
      }
    }
    /*
     * Flush buffer.
     */
    if (count == 100) {
      pdf_add_stream(cmap, "100 beginbfchar\n", strlen("100 beginbfchar\n"));
      pdf_add_stream(cmap, wbuf, strlen(wbuf));
      pdf_add_stream(cmap, "endbfchar\n", strlen("endbfchar\n"));
      count = 0;
      p     = wbuf;
    }
  }

  /*
   * Flush buffer.
   */
  if (count != 0) {
    char tmp[17];
    sprintf(tmp, "%3d beginbfchar\n", count);
    pdf_add_stream(cmap, tmp,  strlen(tmp));
    pdf_add_stream(cmap, wbuf, strlen(wbuf));
    pdf_add_stream(cmap, "endbfchar\n", strlen("endbfchar\n"));
  }

  pdf_add_stream(cmap, TOUNICODE_PART4, strlen(TOUNICODE_PART4));

  return cmap;
}

void
UC_add_ToUnicode_CMap (pdf_obj *fontdict, Encoding *encoding)
{
  char    *encoding_name, **encoding_vec;
  char    *cmap_name;
  pdf_obj *cmap_ref;

  ASSERT(fontdict);
  ASSERT(encoding);

  if (Encoding_is_predefined(encoding))
    return;

  encoding_name = Encoding_get_name(encoding);
  encoding_vec  = Encoding_get_encoding(encoding);
  ASSERT(encoding_name && encoding_vec);

  cmap_name = NEW(strlen(encoding_name)+strlen("-UCS2")+1, char);
  strcpy(cmap_name, encoding_name);
  strcat(cmap_name, "-UCS2");

  if (!PDF_findresource(cmap_name, PDF_RES_TYPE_CMAP)) {
    pdf_obj *cmap;
    AGLmap  *aglm = NULL;
    int      aglm_id;

    aglm_id = AGLmap_cache_find(AGLMAP_DEFAULT_GLYPHLIST);
    if (aglm_id >= 0) {
      aglm = AGLmap_cache_get(aglm_id);
      cmap = UC_make_ToUnicode_CMap(cmap_name, encoding_vec, aglm);
      if (cmap)
	PDF_defineresource(cmap_name, cmap, PDF_RES_TYPE_CMAP);
    }
  }

  cmap_ref = PDF_findresource(cmap_name, PDF_RES_TYPE_CMAP);
  if (cmap_ref)
    pdf_add_dict(fontdict, pdf_new_name("ToUnicode"), cmap_ref);

  RELEASE(cmap_name);

  return;
}
