/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/agl.c,v 1.1 2003/12/27 18:03:16 csc Exp $

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

#include <string.h>
#include <ctype.h>

#include "system.h"
#include "mem.h"
#include "error.h"
#include "mfileio.h"
#include "agl.h"
#include "dpxutil.h"

#define AGL_DEBUG_STR "AGL"
#define AGL_DEBUG     5

static unsigned int __verbose = 0;

static AGList *AGList_new      (void);
static void    AGList_release  (AGList *agl);
static void    AGList_insert   (AGList *agl, char *name, unsigned short code);
static unsigned int   get_hash        (char *key);

void
AGL_set_verbose (void)
{
  __verbose++;
}

char *
AGName_strip_suffix (char *glyphname)
{
  char *dot;

  dot = strchr(glyphname, '.');
  if (dot) {
    *dot = '\0';
    return dot+1;
  } else
    return NULL;
}

int
AGName_is_unicode (char *glyphname)
{
  char c;

  if (!glyphname || strlen(glyphname) < 7 ||
      strncmp(glyphname, "uni", 3))
    return 0;
  c = *(glyphname + 3);
  if (!isdigit(c) && (c < 'A' || c > 'F'))
    return 0;

  return 1;
}

unsigned short
AGName_convert_uni (char *glyphname)
{
  unsigned short ucv = 0;
  char   *p;

  if (!AGName_is_unicode(glyphname))
    return 0;

  if (strlen(glyphname) > 7 && *(glyphname+7) != '.') {
    WARN("Mapping to multiple Unicode codes not supported.");
    return 0;
  }

  p = glyphname + 3;
  while (*p && *p != '.') {
    if (!isdigit(*p) && (*p < 'A' || *p > 'F')) {
      WARN("Invalid char %c for Unicode value in the glyph name %s.", *p, glyphname);
      return 0;
    }
    ucv <<= 4;
    ucv += isdigit(*p) ? *p - '0' : *p - 'A' + 10;
    p++;
  }

  if (ucv > 0xD7FFu && ucv < 0xE000) {
    WARN("Invalid Unicode code value U+%04X.", ucv);
    ucv = 0;
  }

  return ucv;
}


#define AGL_FLAG_CHAR_END    0
#define AGL_FLAG_NEXT_IS_ALT 1
#if 0
#define AGL_FLAG_MORE_BYTES  2
#define AGL_FLAG_CHAR_DECOMP 3
#endif

struct AGList {
  char    *name;
  unsigned short  code;
  int      flags;
  struct AGList *next;
};

static AGList *
AGList_new (void)
{
  AGList *agl;

  agl = NEW(1, AGList);
  agl->name   = NULL;
  agl->code   = 0;
  agl->flags  = AGL_FLAG_CHAR_END;
  agl->next   = NULL;

  return agl;
}

static void
AGList_release (AGList *agl)
{
  while (agl) {
    if (agl->name)
      RELEASE(agl->name);
    agl = agl->next;
  }
}

char *
AGList_get_name (AGList *agl)
{
  if (!agl)
    return NULL;
  return agl->name;
}

unsigned short
AGList_get_code (AGList *agl)
{
  if (!agl)
    return 0;
  return agl->code;
}

AGList *
AGList_next_alternative (AGList *agl)
{
  if (agl->flags == AGL_FLAG_NEXT_IS_ALT && agl->next)
    return agl->next;
  else
    return NULL;
}

static void
AGList_insert (AGList *agl, char *name, unsigned short code)
{
  AGList *save = NULL;

  ASSERT(agl);
  while (agl) {
    if (!strcmp(agl->name, name)) {
      if (agl->code == code)
	return;
      while (agl->flags == AGL_FLAG_NEXT_IS_ALT) {
	if (agl->code == code)
	  return;
	agl = agl->next;
      }
      break;
    }
    save = agl;
    agl  = agl->next;
  }
  if (!agl) {
    agl = AGList_new();
    agl->name = strdup(name);
    agl->code = code;
    if (save)
      save->next = agl;
  } else {
    save = agl->next;
    agl->flags = AGL_FLAG_NEXT_IS_ALT;
    agl->next  = AGList_new();
    agl = agl->next;
    agl->name = strdup(name);
    agl->code = code;
    agl->next = save;
  }
}

struct AGLmap {
  char    *ident;
  AGList **map;
};

#define HASH_TABLE_SIZE 503

static unsigned int
get_hash (char *key)
{
  unsigned int h = 0;

  while (*key)
    h = 33*h + (*key++);

  return h % HASH_TABLE_SIZE;
}

AGLmap *
AGLmap_new (void)
{
  AGLmap *aglm;

  aglm = NEW(1, AGLmap);
  aglm->ident = NULL;
  aglm->map   = NEW(HASH_TABLE_SIZE, AGList *);
  memset(aglm->map, 0, HASH_TABLE_SIZE*sizeof(AGList *));

  return aglm;
}

void
AGLmap_release (AGLmap *aglm)
{
  if (aglm) {
    if (aglm->ident)
      RELEASE(aglm->ident);
    if (aglm->map) {
      int i;
      for (i = 0; i < HASH_TABLE_SIZE; i++) {
	if (aglm->map[i])
	  AGList_release(aglm->map[i]);
      }
      RELEASE(aglm->map);
    }
  }
}

/*
 * format unused.
 */
#define WBUF_SIZE 1024
int
AGLmap_read (AGLmap *aglm, FILE *fp, int format)
{
  int   count = 0;
  char  wbuf[WBUF_SIZE];
  char *start = NULL, *end, *next;

  ASSERT(aglm);

  if (__verbose > AGL_DEBUG)
    MESG("\n%s: Reading file...\n", AGL_DEBUG_STR);

  while ((start = mfgets(wbuf, WBUF_SIZE, fp)) != NULL) {
    AGList  *agl;
    char    *name;
    unsigned short  code;
    end = start + strlen(start);
    skip_white_spaces((unsigned char **)&start, end);
    /* Need table version check. */
    if (!start || *start == '#' ||
	!(next = strchr(start, ';')) || next == start) {
      if (__verbose > AGL_DEBUG)
	MESG("%s: skipping line... %s\n", AGL_DEBUG_STR, start);
      continue;
    }
    *next = '\0';
    name  = start;
    start = next+1;
    code  = strtoul(start, &next, 16);
    if (next == start)
      continue;
    start = next;
    skip_white_spaces((unsigned char **)&start, end);
    if (isxdigit(*start)) { /* Decomposition */
      if (__verbose > AGL_DEBUG)
	WARN("%s: Decomposition found: %s --> %04X %s",
	     AGL_DEBUG_STR, name, code, start);
      continue;
    } else {
      unsigned int idx = get_hash(name);
      agl = aglm->map[idx];
      if (!agl) {
	agl = AGList_new();
	agl->name = strdup(name);
	agl->code = code;
	aglm->map[idx] = agl;
      } else
	AGList_insert(agl, name, code);
      if (__verbose > AGL_DEBUG)
	MESG("%s: %s --> %04X\n", AGL_DEBUG_STR, name, code);
      count++;
    }
  }

  if (__verbose > AGL_DEBUG)
    MESG("\n%s: %d glyph list entries found.\n", AGL_DEBUG_STR, count);

  return count;
}

AGList *
AGLmap_lookup (AGLmap *aglm, char *glyphname)
{
  unsigned int   hkey = 0;
  AGList *agl;

  if (!aglm)
    return NULL;

  hkey = get_hash(glyphname);
  agl = (aglm->map)[hkey];
  while (agl) {
    if (!strcmp(agl->name, glyphname))
      break;
    agl = agl->next;
  }
  return agl;
}

#define CACHE_ALLOC_SIZE 8u
#define CHECK_ID(n) do {\
                        if (! __cache)\
                           ERROR("%s: Not initialized.", AGL_DEBUG_STR);\
                        if ((n) < 0 || (n) >= __cache->num)\
                           ERROR("%s: Invalid ID %d", AGL_DEBUG_STR, (n));\
                    } while (0)

struct AGLmapCache
{
  int      num;
  int      max;
  AGLmap **maps;
};

static struct AGLmapCache *__cache = NULL;

void
AGLmap_cache_init (void)
{
  if (__cache)
    ERROR("%s: Already initialized.", AGL_DEBUG_STR);

  if (__verbose > AGL_DEBUG)
    MESG("%s: Initialize\n", AGL_DEBUG_STR);

  __cache = NEW(1, struct AGLmapCache);
  __cache->max  = CACHE_ALLOC_SIZE;
  __cache->num  = 0;
  __cache->maps = NEW(__cache->max, struct AGLmap *);
}

AGLmap *
AGLmap_cache_get (int id)
{
  CHECK_ID(id);
  return __cache->maps[id];
}

int
AGLmap_cache_find (const char *name)
{
  int     id;
  AGLmap *aglm;
  char   *fullname;
  FILE   *fp;

  if (!__cache)
    AGLmap_cache_init();
  ASSERT(__cache);

  for (id = 0; id < __cache->num; id++) {
    aglm = __cache->maps[id];
    if (!strcmp(name, aglm->ident))
      return id;
  }

  fullname = kpse_find_file(name, kpse_program_text_format, 0);
  if (!fullname) {
    char *altname = NEW(strlen(name)+strlen(".txt")+1, char);
    sprintf(altname, "%s.txt", name);
    fullname = kpse_find_file(altname, kpse_program_text_format, 0);
    RELEASE(altname);
  }

  if (!fullname || !(fp = MFOPEN(fullname, FOPEN_R_MODE)))
    return -1;

  if (__verbose) {
    MESG("(AGL:%s", name);
    if (__verbose > 1)
      MESG("[%s]", fullname);
  }

  aglm = AGLmap_new();
  if (AGLmap_read(aglm, fp, 0) <= 0) {
    AGLmap_release(aglm);
    MFCLOSE(fp);
    return -1;
  }
  MFCLOSE(fp);

  if (__cache->num >= __cache->max) {
    __cache->max += CACHE_ALLOC_SIZE;
    __cache->maps = RENEW(__cache->maps, __cache->max, struct AGLmap *);
  }

  aglm->ident = strdup(name);
  id = __cache->num;
  __cache->maps[id] = aglm;
  (__cache->num)++;

  if (__verbose)
    MESG(")");

  return id;
}

void
AGLmap_cache_close (void)
{
  if (__verbose > AGL_DEBUG)
    MESG("%s: Close\n", AGL_DEBUG_STR);

  if (__cache) {
    int id;
    for (id = 0; id < __cache->num; id++)
      AGLmap_release(__cache->maps[id]);
    RELEASE(__cache->maps);
    RELEASE(__cache);
    __cache = NULL;
  }
}
