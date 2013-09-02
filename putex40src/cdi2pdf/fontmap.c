/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/fontmap.c,v 1.1 2003/12/27 18:03:16 csc Exp $

    This is dvipdfmx, an eXtended version of dvipdfm by Mark A. Wicks.

    Copyright (C) 2004 by Chey-Woei Tsay (cwtsay@pu.edu.tw)

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

#include "mem.h"
#include "error.h"
#include "mfileio.h"
#include "dpxutil.h"
#include "pdfparse.h"
#include "pdflimits.h"
#include "subfont.h"

/* add -- cdipdfmx */
static char *doc_encoding = NULL;
/* end -- cdipdfmx */
static unsigned char verbose = 0;

void fontmap_set_verbose (void)
{
  if (verbose < 255) verbose++;
}

static int num_fontmap = 0, max_fontmap = 0;

static struct map_record {
  char *tex_name, *font_name, *enc_name, *sfd_name;
  double slant, extend;
  int remap;
  int mapc;
/* add -- cdipdfmx */
  int ascent, descent; /* sTypoAscender and sTypoDescender of the TTF font. */
/* end -- cdipdfmx */
} *fontmap = NULL;

static void fontmaps_need (int n)
{
  if (n > max_fontmap) {
    max_fontmap = MAX (max_fontmap + MAX_FONTS, n);
    fontmap = RENEW (fontmap, max_fontmap, struct map_record);
  }
}

/* add -- cdipdfmx */
/* defalut ascent and descent values for most CJK fonts */
#define DEFAULT_CJK_ASCENT 801
#define DEFAULT_CJK_DESCENT -199
/* end -- cdipdfmx */

static void init_map_record (struct map_record *r)
{
  r->tex_name = NULL;
  r->enc_name = NULL;
  r->font_name = NULL;
  r->sfd_name = NULL;
  r->slant = 0.0;
  r->extend = 1.0;
  r->remap = 0;
  r->mapc  = -1;
/* add -- cdipdfmx */
  r->ascent = DEFAULT_CJK_ASCENT;
  r->descent = DEFAULT_CJK_DESCENT;
/* end -- cdipdfmx */
}

void release_map_record (void)
{
  int i;

  for (i = 0; i < num_fontmap; i++) {
    if (fontmap[i].tex_name) RELEASE (fontmap[i].tex_name);
    if (fontmap[i].enc_name) RELEASE (fontmap[i].enc_name);
    if (fontmap[i].font_name) RELEASE (fontmap[i].font_name);
    /* Do not need to release sfd_name because it is included in tex_name */
  }
  if (fontmap) RELEASE (fontmap);
}

static void fill_in_defaults (struct map_record *r)
{
  if (r->enc_name &&
      (!strcmp(r->enc_name, "default") || !strcmp(r->enc_name, "none"))) {
    RELEASE (r->enc_name);
    r->enc_name = NULL;
  }
  if (r->font_name &&
      (!strcmp(r->font_name, "default") || !strcmp(r->font_name, "none"))) {
    RELEASE (r->font_name);
    r->font_name = NULL;
  }
  /* We *must* fill font_name either explicitly or by default */
  if (r->font_name == NULL) {
    /* In the case of subfonts, the base name (before the character '@')
     * will be used as a font_name by default.
     * Otherwise tex_name will be used as a font_name by default. */
    if (r->sfd_name != NULL) {
      int len = r->sfd_name - r->tex_name;
      r->font_name = NEW (len, char);
      strncpy(r->font_name, r->tex_name, len-1);
      *(r->font_name+len) = '\0';
    }
    else {
      r->font_name = NEW (strlen(r->tex_name)+1, char);
      strcpy(r->font_name, r->tex_name);
    }
  }
}

/* add -- cdipdfmx */
void set_doc_encoding (char *enc_name)
{
	doc_encoding = strdup(enc_name);
}
#if 0
void read_cdipdfmx_mapfile (char *filename)
{
  FILE *mapfile = NULL;
  char *full_map_filename, *start = NULL, *end, *p, *tex_name;

#ifdef MIKTEX
  if (!miktex_find_app_input_file("dvipdfm", filename, full_map_filename = work_buffer))
    full_map_filename = NULL;
#else
  full_map_filename = kpse_find_file(filename, kpse_program_text_format, 0);
#endif
  if (full_map_filename == NULL || (mapfile = MFOPEN (full_map_filename, FOPEN_R_MODE)) == NULL)
    fprintf(stderr, "**Warning: Couldn't open the font map file '%s'.\n", filename);

	if (mapfile) {
    while ((start = mfgets(work_buffer, WORK_BUFFER_SIZE, mapfile)) != NULL) {
      end = work_buffer + strlen(work_buffer);
      skip_white(&start, end);
      if (start >= end) continue;
      if (*start == '%') continue;
      if ((tex_name = parse_dbcs_name(&start, end)) == NULL) continue;
      fontmaps_need(num_fontmap + 1);
      init_map_record(fontmap + num_fontmap);

			fontmap[num_fontmap].tex_name = tex_name;
#if 0
			fontmap[num_fontmap].enc_name = strdup(doc_encoding);
      skip_white(&start, end);
			fontmap[num_fontmap].font_name = parse_val_ident(&start, end);
#endif
      skip_white(&start, end);
      if (*start != '-') { /* May be NULL */
				fontmap[num_fontmap].enc_name = parse_ident(&start, end);
				skip_white(&start, end);
      }
      if (*start != '-') { /* May be NULL */
      /* parse_val_ident() allows inclusion of '/' */
      	fontmap[num_fontmap].font_name = parse_val_ident(&start, end);
      	skip_white(&start, end);
      }
	    num_fontmap += 1;
    }
	}
}
#endif
/* end -- cdipdfmx */
void read_mapfile (char *filename)
{
  FILE *mapfile = NULL;
  char *full_map_filename, *start = NULL, *end, *p, *tex_name;

  if (verbose) fprintf(stderr, "<%s", filename);

#ifdef MIKTEX
  if (!miktex_find_app_input_file("dvipdfm", filename, full_map_filename = work_buffer))
    full_map_filename = NULL;
#else
  full_map_filename = kpse_find_file(filename, kpse_program_text_format, 0);
#endif
  if (full_map_filename == NULL ||
      (mapfile = MFOPEN (full_map_filename, FOPEN_R_MODE)) == NULL)
    fprintf(stderr, "**Warning: Couldn't open the font map file '%s'.\n", filename);

  if (mapfile) {
    while ((start = mfgets(work_buffer, WORK_BUFFER_SIZE, mapfile)) != NULL) {
      end = work_buffer + strlen(work_buffer);
      skip_white(&start, end);
      if (start >= end) continue;
      if (*start == '%') continue;
/* mod -- cdipdfmx */
      if ((tex_name = parse_dbcs_name(&start, end)) == NULL) continue;
/* end -- cdipdfmx */
      /* Parse record line in map file.  First two fields (after TeX font
       * name) are position specific.  Arguments start at the first token
       * beginning with a  '-' */
      fontmaps_need(num_fontmap + 1);
      init_map_record(fontmap + num_fontmap);

      /* Subfonts are identified with the tex_name having
       * the character '@'. */
/* mod -- cdipdfmx (make sure @ is not a part of dbcs code) */
      if ((p = strchr(tex_name, '@')) && p[1] != '\0' && (p == tex_name || p[-1] & 0x80 == 0)) {
/* end -- cdipdfmx */
        /* Set the beginning of sfd_name. */
        fontmap[num_fontmap].sfd_name = ++p;
        /* Mark the end of sfd_name. */
        if ((p = strstr(fontmap[num_fontmap].sfd_name, ".sfd")) != NULL) *p = '\0';
	else if ((p = strchr(fontmap[num_fontmap].sfd_name, '@')) != NULL) *p = '\0';
      }
      fontmap[num_fontmap].tex_name = tex_name;
      skip_white(&start, end);
      if (*start != '-') { /* May be NULL */
	fontmap[num_fontmap].enc_name = parse_ident(&start, end);
	skip_white(&start, end);
      }
      if (*start != '-') { /* May be NULL */
	/* parse_val_ident() allows inclusion of '/' */
	fontmap[num_fontmap].font_name = parse_val_ident(&start, end);
	skip_white(&start, end);
      }

      /* Parse any remaining arguments */
      while (start+1 < end && *start == '-') {
	char *number;
	switch (*(start+1)) {
	case 's': /* Slant option */
	  start += 2;
	  skip_white(&start, end);
	  if (start < end && (number = parse_number(&start, end))) {
	    fontmap[num_fontmap].slant = atof(number);
	    RELEASE (number);
	  } else
	    WARN("Missing slant value in map file (%s).", tex_name);
	  break;
	case 'e': /* Extend option */
	  start += 2;
	  skip_white(&start, end);
	  if (start < end && (number = parse_number(&start, end))) {
	    fontmap[num_fontmap].extend = atof(number);
	    RELEASE (number);
	  } else
	    WARN("Missing extend value in map file (%s).", tex_name);
	  break;
	case 'r': /* Remap option */
	  start += 2;
	  skip_white(&start, end);
	  fontmap[num_fontmap].remap = 1;
	  break;
	case 'm': /* Map single bytes char 0xab to double byte char 0xcdab  */
	  start += 2;
	  skip_white(&start, end);
	  if (start+4     > end ||
	      *start     != '<' ||
	      *(start+3) != '>')
	    WARN("Invalid hex value for option -m in map file (%s).", tex_name, start);
	  start++;
	  if ((fontmap[num_fontmap].mapc = getxpair((unsigned char **)&start)) < 0)
	    WARN("Invalid value for option -m in map file '%s'.", tex_name);
	  start++;
	  break;
/* add -- cdipdfmx */
	case 'a': /* ascent option */
	  start += 2;
	  skip_white(&start, end);
	  if (start < end && (number = parse_number(&start, end))) {
	    fontmap[num_fontmap].ascent = atoi(number);
	    RELEASE (number);
	  } else
	    WARN("Missing ascent value in map file (%s).", tex_name);
	  break;
	case 'd': /* descent option */
	  start += 2;
	  skip_white(&start, end);
	  if (start < end && (number = parse_number(&start, end))) {
	    fontmap[num_fontmap].descent = atoi(number);
	    RELEASE (number);
	  } else
	    WARN("Missing descent value in map file (%s).", tex_name);
	  break;
/* end -- cdipdfmx */
	default:
	  WARN("Unrecognized option in map file '%s': -->%s<--", tex_name, start);
	  start = end;
	}
	skip_white(&start, end);
      }
      fill_in_defaults(fontmap + num_fontmap);
#ifndef WITHOUT_COMPAT
      /*
       * Compatibility to older version:
       *
       * We use strncmp() here to allow UCS2, UniJIS, UCS-GB, etc.
       */
      if (fontmap[num_fontmap].sfd_name && fontmap[num_fontmap].enc_name &&
	  (!strncmp(fontmap[num_fontmap].sfd_name, "Uni"  , strlen("Uni"  )) ||
	   !strncmp(fontmap[num_fontmap].sfd_name, "UCS"  , strlen("UCS"  )) ||
	   !strncmp(fontmap[num_fontmap].sfd_name, "UKS"  , strlen("UKS"  )) ||
	   !strncmp(fontmap[num_fontmap].sfd_name, "UGB"  , strlen("UGB"  )) ||
	   !strncmp(fontmap[num_fontmap].sfd_name, "UB5"  , strlen("UB5"  )) ||
	   !strncmp(fontmap[num_fontmap].sfd_name, "UJIS" , strlen("UJIS" )) ||
	   !strncmp(fontmap[num_fontmap].sfd_name, "UBg5" , strlen("UBg5" )) ||
	   !strncmp(fontmap[num_fontmap].sfd_name, "UBig5", strlen("UBig5"))) &&
	  (!strcmp(fontmap[num_fontmap].enc_name, "Identity-H") ||
	   !strcmp(fontmap[num_fontmap].enc_name, "Identity-V"))
	  ) {
	if (!(p = strchr(fontmap[num_fontmap].font_name, '/')) || !strstr(p, "UCS")) {
	  /*
	   * This will never be printed for default fontmaps.
	   * fontmap_set_verbose() comes after read_mapfile()...
	   */
	  if (verbose > 1) {
	    MESG("\n");
	    MESG("Sfd name @%s@ is interpreted as UCS2.\n", fontmap[num_fontmap].sfd_name);
	    MESG("/UCS will be assumed for font \"%s\".\n", fontmap[num_fontmap].font_name);
	  }
	  fontmap[num_fontmap].font_name = RENEW(fontmap[num_fontmap].font_name,
						 strlen(fontmap[num_fontmap].font_name)+strlen("/UCS")+1,
						 char);
	  strcat(fontmap[num_fontmap].font_name, "/UCS");
	}
      }
#endif /* WITHOUT_COMPAT */
      num_fontmap += 1;
    }
    MFCLOSE (mapfile);

    if (verbose) fprintf(stderr, ">");
  }
}

/* get_map_record() returns two IDs, map_id and subfont_id if exist. */
int get_map_record (char *tex_name, int *subfont_id)
{
  int i, len;

  *subfont_id = -1;

  if (!fontmap) return -1;

  for (i = 0; i < num_fontmap; i++) {
    if (fontmap[i].sfd_name != NULL) { /* a map entry for subfonts */
      len = fontmap[i].sfd_name - fontmap[i].tex_name - 1;
      if (len < strlen(tex_name) &&
          !strncmp(fontmap[i].tex_name, tex_name, len) &&
          (*subfont_id = load_sfd_record(fontmap[i].sfd_name, tex_name + len)) >= 0)
        return i;
    } else if (!strcmp(fontmap[i].tex_name, tex_name))
        return i;
  }

  return -1;
}

char *fontmap_tex_name (int map_id)
{
  if (map_id >= 0 && map_id < num_fontmap)
    return fontmap[map_id].tex_name;
  else
    return NULL;
}

char *fontmap_enc_name (int map_id)
{
  if (map_id >= 0 && map_id < num_fontmap)
    return fontmap[map_id].enc_name;
  else
    return NULL;
}

char *fontmap_font_name (int map_id)
{
  if (map_id >= 0 && map_id < num_fontmap)
    return fontmap[map_id].font_name;
  else
    return NULL;
}

double fontmap_extend (int map_id)
{
  if (map_id >= 0 && map_id < num_fontmap)
    return fontmap[map_id].extend;
  else
    return 1.0;
}

double fontmap_slant (int map_id)
{
  if (map_id >= 0 && map_id < num_fontmap)
    return fontmap[map_id].slant;
  else
    return 0.0;
}

int fontmap_remap (int map_id)
{
  if (map_id >= 0 && map_id < num_fontmap)
    return fontmap[map_id].remap;
  else
    return 0;
}

/* add -- cdipdfmx */
int fontmap_ascent (int map_id)
{
  if (map_id >= 0 && map_id < num_fontmap)
    return fontmap[map_id].ascent;
  else
    return -1;
}

int fontmap_descent (int map_id)
{
  if (map_id >= 0 && map_id < num_fontmap)
    return fontmap[map_id].descent;
  else
    return -1;
}

/* end -- cdipdfmx */

int fontmap_mapc (int map_id)
{
  if (map_id >= 0 && map_id < num_fontmap)
    return fontmap[map_id].mapc;
  else
    return -1;
}