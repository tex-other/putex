/* Copyright 2007 TeX Users Group
   Copyright 2014 Clerk Ma
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301 USA.  */

#ifdef _WINDOWS
  #define NOCOMM
  #define NOSOUND
  #define NODRIVERS
  #define STRICT
  #pragma warning(disable:4115) // kill rpcasync.h complaint
  #include <windows.h>
  #define MYLIBAPI __declspec(dllexport)
#endif

#pragma warning(disable:4996)
#pragma warning(disable:4131) // old style declarator
#pragma warning(disable:4135) // conversion between different integral types
#pragma warning(disable:4127) // conditional expression is constant

#include <setjmp.h>
#include <assert.h>

#define EXTERN extern

#include "texd.h"

#include <io.h>    // needed for _finddata_t
#include <ctype.h> // needed for isascii and isalpha

#define NAME_MAX 255      // max size of name component

#define ISALPHA(c) (isascii (c) && isalpha(c))

#define PATH_SEP              '/'
#define PATH_SEP_STRING       "/"
#define PATH_DELIMITER        ';'
#define PATH_DELIMITER_STRING ";"

// default paths to look for things

#define TEXPATH    "C:/yandy/yandytex/"
#define TEXFORMATS "C:/yandy/yandytex/fmt"
#define TEXPOOL    "C:/yandy/yandytex/pool"
#define TEXFONTS   "C:/yandy/yandytex/tfm"
#define TEXINPUTS  TEXPATH "tex//;" "C:/tex;" "C:/texinput"

// structure used by fontmap

typedef struct map_element_struct
{
  char * key;
  char * value;
  struct map_element_struct * next;
} map_element_type;

typedef map_element_type **map_type;

extern bool usesourcedirectory; /* in local.c */

extern bool workingdirectory;   /* in local.c */

bool absolute_p (string filename);
string readable (string name);
string truncate_pathname (string name);
char * file_p(string fn);
bool dir_p(string fn);
string *find_dir_list (string path);
void add_directory (string **dir_list_ptr, unsigned *dir_count_ptr, string dir);
void expand_subdir (string **dir_list_ptr, unsigned *dir_count_ptr, string dirname, struct _finddata_t findt, integer recurseflag);
void save_dir_list (string path,  string *dir_list);
string *initialize_path_list (string env_name,  string default_path);
int xfind_path_filename (string buffer, string filename,  string * dir_list);
string expand_default (string env_path, string default_path);
map_type map_create (string *dir_list);
char *map_lookup (map_type map, char *key);

// the following do *not* use MALLOC

extern char * xconcat  (char *buffer, char *s1, char *s2);           /* openinou.c */
extern char * xconcat3 (char *buffer, char *s1, char *s2, char *s3); /* openinou.c */

/////////////////////////////////////////////////////////////////////////

// used only in jump_out in tex0.c, and in texbody in itex.c
// and main in texmf.c and a few other abort situations in texmf.c
/* texk/web2c/lib/uexit.c */
void uexit (int unix_code)
{
  int final_code;

#ifndef _WINDOWS
  fflush(stdout);
#endif

  if (unix_code == 0)
    final_code = EXIT_SUCCESS;
  else if (unix_code == 1)
    final_code = EXIT_FAILURE;
  else
    final_code = unix_code;

  if (jump_used)
  {
    show_line("Jump Buffer already used\n", 1);
    exit(1);
  }

  jump_used++;
  exit(final_code);
}
/* texk/web2c/lib/zround.c */
integer zround (double r)
{
  integer i;

  if (r > 2147483647.0)
    i = 2147483647;
  else if (r < -2147483647.0)
    i = -2147483647;
  else if (r >= 0.0)
    i = (integer)(r + 0.5);
  else
    i = (integer)(r - 0.5);

  return i;
}
/* texk/web2c/lib/eofeoln.c */
bool test_eof (FILE * file)
{
  int c;
/* Maybe we're already at the end?  */
  if (!file)
    return true;

  if (feof (file))
    return true;

  if ((c = getc (file)) == EOF)
    return true;
/* We weren't at the end.  Back up.  */
  (void) ungetc (c, file);

  return false;
}
/* texk/web2c/lib/eofeoln.c */
bool eoln (FILE * file)
{
  register int c;

  if (feof (file))
    return true;

  c = getc (file);

  if (c != EOF)
    (void) ungetc (c, file);

  return c == '\n' || c == '\r' || c == EOF; // ???
/* Type mismatch (return) (int/enum) ? */
}

char * read_a_line (FILE *f,  char *line, int limit)
{
  int c;
  int loc = 0;

  while ((c = getc (f)) != EOF)
  {
    if (c == '\n' || c == '\r')
    {
      if (loc > 0) break;
      else continue;        /* ignore \r\n and blank lines */
    }

    line[loc] = (char) c;
    loc++;

    if (loc == limit - 1)
    {
      sprintf(log_line, " ERROR: line too long\n");
      show_line(log_line, 1);
      show_line(line, 0);
      show_line("\n", 0);
      break;
    }
  }

  if (c != EOF || loc > 0)
  {
    line[loc] = '\0';       /* terminate */
    return line;          /* and return */
  }
  else
    return(NULL);          /* true EOF */
}
/****************************************************************************************/

/* from ourpaths.c */

#define BUILDNAMEDIRECT /* avoid malloc for string concat */

#define CACHEFILENAME   /* cache last full path/file name 96/Nov/16 */
                        /* to speed up LaTeX 2e which opens files twice */

/* `path_dirs' is initialized in `set_paths', to a null-terminated array
   of directories to search for.  */

static string *path_dirs[LAST_PATH];

/* This sets up the paths, by either copying from an environment variable
   or using the default path, which is defined as a preprocessor symbol
   (with the same name as the environment variable) in `site.h'.  The
   parameter PATH_BITS is a logical or of the paths we need to set.  */
void set_paths (int path_bits)
{
  int n;                          /* 97/Apr/2 */
  char *s, *t, *u;                /* 94/Jan/6 */
  char buffer[PATH_MAX];


  if (path_bits & TEXFORMATPATHBIT)
  {
    s = "TEXFORMATS";
    t = TEXFORMATS;

    if (grabenv(s) == NULL)
    {
      strcpy(buffer, texpath);
      strcat(buffer, PATH_SEP_STRING); 
      strcat(buffer, "fmt");

      if (trace_flag)
      {
        sprintf(log_line, "Checking `%s' = %s %s %s\n", buffer, texpath, PATH_SEP_STRING, "fmt");
        show_line(log_line, 0);
      }

      if (dir_p(buffer))
        t = xstrdup(buffer);
      else
      {
        s = "TEXFMTS";
        if (getenv(s) == NULL)  s = "TEXFMT";
      }

      if (trace_flag)
      {
        sprintf(log_line, "\nSetting up %s (default %s) ", "TEXFORMATS", t);
        show_line(log_line, 0);
      }
    }

    path_dirs[TEXFORMATPATH] = initialize_path_list (s, t);
  }

  if (path_bits & TFMFILEPATHBIT)
  {
    s = "TEXFONTS";

    if ((u = grabenv("ENCODING")) != NULL)
    {
      encoding_name = u;

      if ((t = grabenv(u)) != NULL)
      {
        if (strchr(t, ':') != NULL && sscanf(t, "%d", &n) == 0)
        {
          s = u;
        }
      }
    }

    t = TEXFONTS;

    if (grabenv(s) == NULL)
    {
      strcpy(buffer, texpath);
      strcat(buffer, PATH_SEP_STRING); 
      strcat(buffer, "tfm");

      if (trace_flag)
      {
        sprintf(log_line, "Checking `%s' = %s %s %s\n",
            buffer, texpath, PATH_SEP_STRING, "tfm");
        show_line(log_line, 0);
      }

      if (dir_p(buffer))
        t = xstrdup(buffer);
      else
      {
        s = "TEXTFMS";
        if (getenv(s) == NULL) s = "TEXTFM";
      }

      if (trace_flag)
      {
        sprintf(log_line, "\nSetting up %s (default %s) ", "TEXFONTS", t);
        show_line(log_line, 0);
      }
    }

    path_dirs[TFMFILEPATH] = initialize_path_list (s, t);
  }

  if (path_bits & TEXINPUTPATHBIT)
  {
    if (format_specific)
    {
      s = format_name;

      if (grabenv(s) == NULL)
        s = "TEXINPUTS";
    }
    else
      s = "TEXINPUTS";

    if (grabenv(s) == NULL)
    {
      s = "TEXINPUT";

      if (trace_flag)
      {
        sprintf(log_line, "\nSetting up %s ", "TEXINPUTS");
        show_line(log_line, 0);
      }
    }

    path_dirs[TEXINPUTPATH]  = initialize_path_list (s, TEXINPUTS);
  }
}

#ifdef CACHEFILENAME
  char last_filename[PATH_MAX] = "";  /* last full path / file name found C */
  char last_name[PATH_MAX]     = "";  /* last file name searched for C */
  int last_path_index          = -1;  /* last path_index */
#endif

/* Look for NAME, a C string (no longer Pascal), in the colon-separated list 
   of directories given by `path_dirs[PATH_INDEX]'.  If the search is
   successful, leave the full pathname in NAME (which therefore must
   have enough room for such a pathname), padded with blanks.
   Otherwise, or if NAME is an absolute or relative pathname, just leave
   it alone.  */

/*  changed to take C string 97/June/5 - used to take Pascal strings */
/*  now expects null terminated strings */
/* used only in open_input.*/
bool test_read_access (unsigned char *name, int path_index)
{ 
#ifdef BUILDNAMEDIRECT
  char buffer[PATH_MAX];  /* for constructing name 1996/Jan/20 */
  int foundflag;          /* true if path found */
#else
  string foundname;
#endif  

  if (*name == '\0')
    return FALSE;

#ifdef CACHEFILENAME
/*  If file name and path_index matches - and saved filename exists */
/*  then use cached full path / file name 96/Nov/16 */
  if (cache_file_flag)
  {
    if (path_index == last_path_index &&
        strcmp((const char *)name, last_name) == 0 && *last_filename != '\0')
    {
      if (open_trace_flag)
      {
        sprintf(log_line, "\nFOUND `%s' (%d) IN CACHE: `%s' ", name, path_index, last_filename); 
        show_line(log_line, 0);
      }

      strcpy((char *)name, last_filename); 
      return TRUE;
    }

    last_path_index = path_index;
    strcpy(last_name, (const char *)name);
    *last_filename = '\0';          /* in case not found */
  }
#endif

#ifdef BUILDNAMEDIRECT
  foundflag = xfind_path_filename (buffer, (char *)name, path_dirs[path_index]);
#else
  foundname = find_path_filename (name, path_dirs[path_index]);
#endif

#ifdef BUILDNAMEDIRECT
  if (foundflag == 0 && path_index == TFMFILEPATH)
#else
  if (foundname == NULL && path_index == TFMFILEPATH)
#endif
  {
    char *mapped_name;
    static map_type fontmap = NULL;   /* GLOBAL, so won't recreate */

    if (fontmap == NULL)
    {
      if (trace_flag)
      {
        sprintf(log_line, "Loading in texfonts.map file for %s\n", name);
        show_line(log_line, 0);
      }

      fontmap = map_create (path_dirs[path_index]);
    }

    mapped_name = map_lookup (fontmap, (char *) name);

    if (mapped_name)
    {
#ifdef BUILDNAMEDIRECT
      foundflag = xfind_path_filename (buffer, mapped_name, path_dirs[path_index]);
#else
      foundname = find_path_filename (mapped_name, path_dirs[path_index]);
#endif
    }
  }

  if (open_trace_flag)
  {
    show_line("\n", 0);
  }

  if (open_trace_flag)
  {
#ifdef BUILDNAMEDIRECT
    if (foundflag != 0)
    {
      sprintf(log_line, "`%s' in test_read_access\n", buffer);
      show_line(log_line, 0);
    }
#else
    if (foundname != NULL)
    {
      sprintf(log_line, "`%s' in test_read_access\n", foundname);
      show_line(log_line, 0);
    }
#endif
  }

/*  If we found it somewhere, save it.  */
#ifdef BUILDNAMEDIRECT
  if (foundflag != 0)
  {
    strcpy ((char *)name, buffer); 

#ifdef CACHEFILENAME
    if (cache_file_flag)
    {
      strcpy(last_filename, buffer);  /* full path */
    }
#endif
  }
#else
  if (foundname != NULL)
  {
    strcpy (name, foundname);
#ifdef CACHEFILENAME
    if (cache_file_flag)
    {
      strcpy(last_filename, foundname); /* full path */
      last_namelength = strlen(buffer);
    }
#endif
  }
#endif

#ifdef BUILDNAMEDIRECT
  return foundflag;
#else
  if (foundname == NULL) return FALSE;
  else {
    if (foundname != name) free (foundname);  /* copied, now free ??? 96/Jan/10 */
    return TRUE;
  }
#endif
}

/********************************************************************/
/* from fontmap.c */
/* Fontname mapping.  We use a straightforward hash table.  */

#define MAP_SIZE 199

/* The hash function.  We go for simplicity here.  */
/* NOT USED! */
static unsigned map_hash (char * key)
{
  unsigned n = 0;

/*  There are very few font names which are anagrams of each other 
    so no point in weighting the characters.  */
  while (*key != 0) n += *key++;
  n %= MAP_SIZE;
  return n;
}

/* Look up STR in MAP.  Return the corresponding `value' or NULL.  */
/* NOT USED! */
static char *map_lookup_str (map_type map, char *key)
{
  map_element_type *p;
  unsigned n = map_hash (key);

  for (p = map[n]; p != NULL; p = p->next)
    if (strcmp(key, p->key) == 0) return p->value;

  return NULL;
}


/* Look up KEY in MAP; if it's not found, remove any suffix from KEY and
   try again.  */
/* NOT USED! */
char *map_lookup (map_type map, char *key)
{
  string suffix = find_suffix (key);
  string ret = map_lookup_str (map, key);

  if (! ret) {
/*    OK, the original KEY didn't work.  Let's check for the KEY without
    an extension -- perhaps they gave foobar.tfm, but the mapping only
    defines `foobar'.  */
    if (suffix) {
      string base_key = remove_suffix (key);
      ret = map_lookup_str (map, base_key);
      free (base_key);  // it's safe to free copy
    }
  }

/*  Append the same suffix we took off, if necessary.  */
/*  what if suffix is NULL ??? */ /* what if we didn't take off suffix ??? */
/*  if (ret) */
  if (ret && suffix) {            /* 1994/March/18 */
    ret = extend_filename (ret, suffix);
//    the above creates a newly allocated string ... should free old ?
//    except extend_filename may return the old one ?
  }
  return ret;
}

/* If KEY is not already in MAP, insert it and VALUE.  */
/* This was very buggy (when hash codes collided) - rewritten 94/March/18 */
/* NOT USED! */
void map_insert (map_type map, char *key, char *value)
{
  unsigned n = map_hash (key);
  map_element_type **ptr = &map[n];
/*  map_element_type ***trailer = &p; */

  while (*ptr != NULL && !(strcmp(key, (*ptr)->key) == 0)) {
/*       *p = (*p)->next; */
    ptr = &((*ptr)->next);
/*       trailer = &p; */
  }

  if (*ptr == NULL)
  {
    *ptr = (map_element_type *) xmalloc (sizeof(map_element_type));
    (*ptr)->key = xstrdup (key);
    (*ptr)->value = xstrdup (value);
    (*ptr)->next = NULL;
  }
}

/* Open and read the mapping file FILENAME, putting its entries into
   MAP. Comments begin with % and continue to the end of the line.  Each
   line of the file defines an entry: the first word is the real
   filename (e.g., `ptmr'), the second word is the alias (e.g.,
   `Times-Roman'), and any subsequent words are ignored.  .tfm is added
   if either the filename or the alias have no extension.  This is the
   same order as in Dvips' psfonts.map; unfortunately, we can't have TeX
   read that same file, since most of the real filenames start with an
   `r', because of the virtual fonts Dvips uses.  */

/* Modified 97/May/17 to avoid malloc for each line read */

#ifndef MALLOCLINE
  #define MAXLINE 256
#endif
/* NOT USED! */
int map_file_parse (map_type map, char *map_filename)
{
  char *l;
  unsigned map_lineno = 0;
  FILE *f; 
#ifndef MALLOCLINE
  char line[MAXLINE];             /* 97/May/17 */
#endif  

  if (trace_flag) {
    sprintf(log_line, "Opening %s\n",  map_filename); /* 97/May/17 */
    show_line(log_line, 0);
  }
//  f = xfopen (map_filename, FOPEN_R_MODE);
  f = fopen (map_filename, FOPEN_R_MODE);
  if (f == NULL) {
    perrormod(map_filename);  // should not happen, since we tested
    return -1;          // failed
  }
#ifdef MALLOCLINE
  while ((l = read_line (f)) != NULL) 
#else
    while ((l = read_a_line (f, line, sizeof(line))) != NULL) /* 97/May/17 */
#endif
    {
      string filename;
      string comment_loc;

/*      comment_loc = strrchr (l, '%'); */
      comment_loc = strchr (l, '%');      /* 96/Nov/16 */
/*      if (comment_loc == NULL) comment_loc = strrchr (l, ';'); */
      if (comment_loc == NULL) comment_loc = strchr (l, ';'); /* fixed */

/*      Ignore anything after a % or;  */
/*      if (comment_loc) *comment_loc = 0; */
      if (comment_loc != NULL) *comment_loc = '\0';

      map_lineno++;

/*      If we don't have any filename, that's ok, the line is blank.  */
      filename = strtok (l, " \t");
/*      if (filename)  */
      if (filename != NULL) {
        string alias = strtok (NULL, " \t");

/*        But if we have a filename and no alias, something's wrong.  */
        if (alias == NULL || *alias == 0) {
          sprintf(log_line,
              " Have file name `%s', but no mapping (line %u in file %s).\n",
              filename, map_lineno, map_filename);
          show_line(log_line, 1);
        }
        else  {
/*          We've got everything.  Insert the new entry.  */
          map_insert (map, alias, filename);
        }
      }
#ifdef MALLOCLINE
      free (l);
#endif
    }
//  xfclose (f, map_filename);
  (void) fclose (f);    // we don't care about errors at this stage
  return 0;       // success
}
/* NOT USED! */
void unshroud_string (char *, char *, int); /* in texmf.c */
/* Look for the file `texfonts.map' in each of the directories in
   DIR_LIST.  Entries in earlier files override later files.  */

/* This is probably quite silly - but what the hell lets leave it in */
/* NOT USED! */
map_type map_create (string *dir_list)
{
  map_type map = (map_type) xcalloc (MAP_SIZE, sizeof (map_element_type *));

  while (*dir_list) {
    char filename[PATH_MAX];

/*     We don't bother with the filename truncation that `readable' in
         `pathsrch.c' does, since we ourselves are giving the filename,
         and I don't think it's worth worrying about too-long
         intermediate directory names in the path.  */
    strcpy (filename, *dir_list);
/*      strcat (filename, "texfonts.map"); */   /* 1993/Nov/20 */
    unshroud_string (filename+strlen(filename),
            "ufygpout/nbq", PATH_MAX - strlen(filename));

/*    testing access first so xfopen won't fail... */
/*    maybe do this another way to avoid using `access' ? */
    if (file_method) {
      if (file_p (filename) != NULL) {    /* use file_p the new way */
        (void)  map_file_parse (map, filename);
      }
    }
    else {  
/*      if (access (filename, R_OK) == 0) */  /* use access the old way */
      if (_access (filename, R_OK) == 0) {  /* 1999/Jan/8 ??? */
/*      if (readable (filename) != NULL) */
        (void) map_file_parse (map, filename);
      }
    }
    dir_list++;
  }
  return map;
}
/**********************************************************************/

/* #pragma optimize ("g", off) *//* try and avoid compiler bug here _dos_find */

/* NOTE: _dos_find... prevents running under Windows NT ??? */
/* This is called if file_method != 0 */ /* which is currently the default */

#ifdef MSDOS
/* see whether a file exists, is readable and is not a directory */
/* 1994/Feb/13 may be faster than `access' in `readable' */
/* returns NULL or the name filename passed in ??? */
/* kpathsea/file_p.c */
char *file_p (string fn)
{
  struct _finddata_t fi;
  long hFind;
  int ret;

  if (open_trace_flag)
  {
    sprintf(log_line, "Is `%s' a readable file? ", fn);
    show_line(log_line, 0);
  }

/*  allow for `normal' (_A_NORMAL) as well as `read-only' files */

  hFind = _findfirst (fn, &fi);

  if (hFind > 0)
  {
    ret = 0;
    _findclose (hFind);
  }
  else
    ret = -1;

/*  check whether found and whether *not* a sub-directory */
  if (ret == 0) {
    if ((fi.attrib & _A_SUBDIR) == 0) {
      if (open_trace_flag) {
        sprintf(log_line, "`%s' IS a readable file. ", fn);
        show_line(log_line, 0);
      }
      return fn;    /* true - its a file, not a dir */
    }
    else {
      if (open_trace_flag) {
        sprintf(log_line, "`%s' is a subdirectory. ", fn);
        show_line(log_line, 0);
      }
      return NULL;  /* false - directory */
    }
  }
  else {
    if (open_trace_flag) {
      sprintf(log_line, "`%s' is NOT a readable file. ", fn);
      show_line(log_line, 0);
    }
    return NULL;  /* false - not found or no read access */
  }
}

#endif /* DOS */

/* #pragma optimize ("g",)  */  /* try and avoid compiler bug here _dos_find */
/* #pragma optimize ("g",)*/  /* try and avoid compiler bug here _dos_find */
// #pragma optimize ("", on)    /* 96/Sep/15 */


/**************************************************************************/

/********************************************************************************/

/* pathsrch.c */

/* If FILENAME is absolute or explicitly relative (i.e., starts with
   `/', `./', or `../'), or if DIR_LIST is null, we return whether
   FILENAME is readable as-is.  Otherwise, we test if FILENAME is in any of
   the directories listed in DIR_LIST.  (The last entry of DIR_LIST must
   be null.)  We return the complete path if found, NULL else.
   
   In the interests of doing minimal work here, we assume that each
   element of DIR_LIST already ends with a `/'.
   
   DIR_LIST is most conveniently made by calling `initialize_path_list'.
   This is a separate routine because we allow recursive searching, and
   it may take some time to discover the list of directories.  
   We do not want to incur that overhead every time we want to look for
   a file.
   
   (Actually, `/' is not hardwired into this routine; we use PATH_SEP,
   defined above.)  */

/* xfind_path_filename is used now */

/* Called only from test_read_access(...) in ourpaths.c */

#ifdef BUILDNAMEDIRECT

/* this string allocation / concatination is silly - use fixed buffer! */
/* NOT USED£¡ */
int xfind_path_filename (string buffer, string filename, string * dir_list)
{
  string found_name = NULL;

  if (buffer == filename)
  {
    show_line("buffer == filename\n", 1);
  }

  *buffer = '\0';       /* "" in case we fail */

  if (open_trace_flag)
  {
    sprintf(log_line, "Find path for `%s' ", filename);
    show_line(log_line, 0);
  }

/*  ignore current directory for TFM files ? */ /* 1994/Jan/24 */
  if (!current_tfm &&  strstr(filename, ".tfm") != NULL && strcmp(*dir_list, "./") == 0)
  {
    if (open_trace_flag)
    {
      sprintf(log_line, "Ignoring `.' for %s ", filename);
      show_line(log_line, 0);
    }

    dir_list++;           /* step over first entry in dir list */
  }

  if (trace_flag && open_trace_flag)
  {
    char **pstrs;
    pstrs = dir_list;
    show_line("\n", 0);
    sprintf(log_line, "Find path for `%s' ", filename);
    show_line(log_line, 0);
    show_line("- IN: ", 0);

    while (*pstrs != NULL)
    {
      sprintf(log_line, "%s ", *pstrs);
      show_line(log_line, 0);
      pstrs++;
    }

    show_line("\n", 0);
  }

/*  Do this before testing for absolute-ness, as a leading ~ will be an
  absolute pathname.  */  /* forget this for DOS ! */
#ifndef MSDOS
  filename = expand_tilde (filename);
#endif

#ifdef MSDOS
/*  is this always safe?  That is, is filename writable and its OK to modify */
/*  unixify(filename);   */ /* done `in place' */
  if (deslash) unixify(filename);   /* made conditional 94/Feb/24 */
#endif

/*  following addded in attempt to catch `nul' */   /* 94/Jan/6 bkph */
/*  could also try and catch `nul.tex' first - but who cares about speed ? */
/*  needed to add this since `access' gets the wrong answer */
/*  that is, `nul' is a file that can be opened, but `access' says not so */
  if (strcmp(filename, "nul") == 0)
    strcpy(buffer, filename); 
/*  If FILENAME is absolute or explicitly relative, or if DIR_LIST is
    null, only check if FILENAME is readable.  */
/*  if (absolute_p (filename) || dir_list == NULL) */ /* 94/Jan/6 */
  else if (absolute_p (filename) || dir_list == NULL)
  {
    if (file_method)
      found_name = file_p (filename);
    else
      found_name = readable (filename);

    if (found_name != NULL)
      strcpy(buffer, found_name);
    else
      *buffer = '\0';
  }
  else  /* Test if FILENAME is in any of the directories in DIR_LIST.  */
  {
    char *s;
    int sourceflag;
    int firsttime=1;

    while (*dir_list != NULL) 
    {
/* if item is current directory, look in source file directory first */
/* provided usesourcedirectory flag is set and workingdirectory in use */
      s = *dir_list;
      sourceflag = 0;

      if (strcmp(s, "./") == 0)
      {
        if (firsttime && usesourcedirectory && workingdirectory &&
            source_direct != NULL && *source_direct != '\0')
        {
          s = source_direct;

          if (trace_flag)
          {
            sprintf(log_line, "Using %s dir %s %s\n", "source", s, "X");
            show_line(log_line, 0);
          }

          sourceflag = 1;     /* avoid increment of list below */
          firsttime = 0;      /* special stuff only first time */
        }
        else if (trace_flag)
        {
          sprintf(log_line, "Using %s dir %s %s\n", "current",  s, "X");
          show_line(log_line, 0);
        }
      }

      if (trace_flag)
      {
        sprintf(log_line, "XCONCAT %s %s in find_path_filename\n",
            s, filename);
        show_line(log_line, 0);
      }
/*      filename = concat (*dir_list, save_filename); */
      (void) xconcat (buffer, s, filename);
      if (file_method) found_name = file_p (buffer);  /* new way */
      else found_name = readable (buffer);      /* slow old way */
      if (found_name == NULL) {
        *buffer = '\0';
        if (! sourceflag)   /* 98/Sep/29 repeat in current dir */
          dir_list++;       /* try next */
      }
      else {
        if (found_name != buffer)
          strcpy(buffer, found_name);       /* success */
        break;
      }
    }
  }
  return (*buffer != '\0');     /* true if not empty string */
}

#else
/* We are dealing with a C string here for filename presumably ... */
/* Also, this returns a string that was allocated --- */
/* unless it happens to equal the filename sent in ... */
/* this needs to be freed later - unless it happens to be ... */

string find_path_filename (string filename,  string * dir_list)
{
  string found_name = NULL;
  
  if (open_trace_flag) {
//    printf("Find path for `%s' ", filename);
    sprintf(log_line, "Find path for `%s' ", filename);
    show_line(log_line, 0);
  }

/*  ignore current directory for TFM files ? */ /* 1994/Jan/24 */
  if (!current_tfm &&
      strstr(filename, ".tfm") != NULL &&
        strcmp(*dir_list, "./") == 0) {
    if (open_trace_flag) {
      sprintf(log_line, "Ignoring `.' for %s ", filename);
      show_line(log_line, 0);
    }
    dir_list++;           /* step over first entry in dir list */
  }

  if (trace_flag && open_trace_flag) {    /* debugging trace 1994/Jan/8 */
    char **pstrs;
    pstrs = dir_list;
    show_line("\n", 0);
    sprintf(log_line, "Find path for `%s' ", filename);
    show_line(log_line, 0);
    show_line("- IN: ", 0);
    while (*pstrs != NULL) {
//      printf("%s ", *pstrs);
      sprintf(log_line, "%s ", *pstrs);
      show_line(log_line, 0);
      pstrs++;
    }
    show_line("\n", 0);
  }

/*  Do this before testing for absolute-ness, as a leading ~ will be an
    absolute pathname.  */  /* forget this for DOS ! */
#ifndef MSDOS
  filename = expand_tilde (filename);
#endif

#ifdef MSDOS
/*  is this always safe?  That is, is filename writable and its OK to modify */
/*  unixify(filename);   */ /* done `in place' */
  if (deslash) unixify(filename);    /* made conditional 94/Feb/24 */
#endif
/*  following addded in attempt to catch `nul' */   /* 94/Jan/6 bkph */
/*  could also try and catch `nul.tex' first - but who cares about speed ? */
/*  needed to add this since `access' gets the wrong answer */
/*  that is, `nul' is a file that can be opened, but `access' says not so */
  if (strcmp(filename, "nul") == 0) found_name = filename; 
/*  If FILENAME is absolute or explicitly relative, or if DIR_LIST is
    null, only check if FILENAME is readable.  */
/*  if (absolute_p (filename) || dir_list == NULL) */ /* 94/Jan/6 */
  else if (absolute_p (filename) || dir_list == NULL) {
    if (file_method)  found_name = file_p (filename); /* new way 94/Feb/13 */
    else found_name = readable (filename);        /* slow old way */
  }
  else { /* Test if FILENAME is in any of the directories in DIR_LIST.  */
    string save_filename = filename;
    char *s;
    int sourceflag;
    int firsttime=1;
    while (*dir_list != NULL)  {
/* if item is current directory, look in source file directory first */
/* provided usesourcedirectory flag is set and workingdirectory in use */
      s = *dir_list;
      sourceflag = 0;
      if (strcmp(s, "./") == 0) {
        if (firsttime && usesourcedirectory && workingdirectory &&
          source_direct != NULL && *source_direct != '\0') {
          s = source_direct;
          if (trace_flag) {
            sprintf(log_line, "Using %s dir %s %s\n", "source", s, "F");
            show_line(log_line, 0);
          }
          sourceflag = 1;     /* avoid increment of list below */
          firsttime = 0;      /* special stuff only first time */
        }
        else if (trace_flag) {
          sprintf(log_line, "Using %s dir %s %s\n", "current", s, "F");
          show_line(log_line, 0);
        }
      }
      if (trace_flag) {
        sprintf(log_line, "CONCAT %s %s in find_path_filename\n",
                  s, save_filename); /* 1996/Jan/20 */
        show_line(log_line, 0);
      }
      filename = concat (s, save_filename);
/*          found_name = readable (filename); */
      if (file_method) found_name = file_p (filename);  /* new way */
      else found_name = readable (filename);      /* slow old way */
      if (found_name == NULL)  {
        free (filename);    /* if this is not it, free it again */
        if (! sourceflag)   /* 98/Sep/29 repeat in current dir */
          dir_list++;
      }
      else {
        if (found_name != filename)
          free (filename);  /* don't free if is the one passed in */
        break;
      }
    }
  }
  return found_name;        /* return the allocated name - free later */
}
#endif

/* If NAME is readable, return it.  If the error is ENAMETOOLONG,
   truncate any too-long path components and return the result (unless
   there were no too-long components, i.e., a overall too-long name
   caused the error, in which case return NULL).  On any other error,
   return NULL.
   
   POSIX invented this brain-damage of not necessarily truncating
   pathname components; the system's behavior is defined by the value of
   the symbol _POSIX_NO_TRUNC, but you can't change it dynamically!  */

/* Using access (and dir_p)  is considerably slower than using dosfind */
/* NOTE: this is only called from find_path_file,
   and then ONLY if file_method is false (-G) */

/* returns NULL or the file name passed in ??? */

/* static string readable (name) */
/* kpathsea/readable.c */
string readable (string name)
{
  string ret;

  if (open_trace_flag) {
    sprintf(log_line, "is %s readable? ", name);
    show_line(log_line, 0);
  }

/*  Check first whether we have read access, then */
/*  need to test if directory, since access always says OK for directory */
/*  BUT: readable is called only from find_path_file */
/*  So we never call this with directory, so why waste time ? bkph */
/*  BUT: can be caught out by idiot with a directory called myfile.tex ? */
  
  if (_access (name, R_OK) == 0)  {
    if (test_dir_access) {    /* check only if we are asked to ... */
      if (dir_p (name)) {
        if (open_trace_flag) {
          sprintf(log_line, "tested read access of directory `%s' ", name);
          show_line(log_line, 0);
        }
        ret = NULL;
      }
      else ret = name;
    }
    else ret = name;
  }
/*  if (_access (name, R_OK) == 0 && !dir_p (name))   ret = name; */
#ifdef ENAMETOOLONG
  else if (errno == ENAMETOOLONG) { 
    ret = truncate_pathname (name);
/* Perhaps some other error will occur with the truncated name, so
         let's call access again.  */
    if (!(_access (ret, R_OK) == 0 && !dir_p (ret))) { /* Failed.  */
      free (ret);
      ret = NULL;
    }
  }
#endif
  else if (errno == EACCES) {
    if (trace_flag) show_line("Access denied!\n", 0);
    ret = NULL;
  }
  else if (errno == ENOENT) {
    if (trace_flag) show_line("File or path name not found!\n", 1);
    ret = NULL;
  }
  else {
    if (trace_flag) {
      sprintf(log_line, "Unknown access error %d!\n", errno);
      show_line(log_line, 0);
    }
    ret = NULL;
  }
  return ret;
}

#ifdef ENAMETOOLONG
/* Truncate any too-long path components in NAME, returning the result.  */

string truncate_pathname (string name)
{
  unsigned c_len = 0;       /* Length of current component.  */
  unsigned ret_len = 0;   /* Length of constructed result.  */
  string ret = (string) xmalloc (PATH_MAX + 1);

  for (; *name; name++)
    {
      if (*name == PATH_SEP)      /* not in DOS */
        { /* At a directory delimiter, reset component length.  */
          c_len = 0;
        }
      else if (c_len > NAME_MAX)
        { /* If past the max for a component, ignore this character.  */
          continue;
        }

      /* If we've already copied the max, give up.  */
      if (ret_len == PATH_MAX)
        {
          free (ret);
          return NULL;
        }

      /* Copy this character.  */
      ret[ret_len++] = *name;
      c_len++;
    }
  ret[ret_len] = 0;

  return ret;
}
#endif  /* end of ifdef ENAMETOOLONG */


/* Return true if FILENAME is absolute or explicitly relative, else false.  */
/* Absolute: in DOS name starts with PATH_SEP, or with DRIVE letter and colon */
/* Explicitly relative: starts with ./ or ../ */
/* kpathsea/absolute.c */
// static bool absolute_p (string filename) {
bool absolute_p (string filename)
{
  bool absolute;
  bool explicit_relative;

#ifdef MSDOS
/*  absolute = (*filename == PATH_SEP) */     /* 1994/Mar/1 */
  absolute = (*filename == PATH_SEP || *filename == '\\')
                      || ((filename[1] == ':') && ISALPHA (*filename));
/*                      || ISALPHA (*filename) && filename[1] == ':'; */
#else
  absolute = (*filename == PATH_SEP);
#endif
  if (absolute) return true;      /* don't waste any more time */

#ifdef MSDOS
  explicit_relative = (*filename == '.'
    && ((filename[1] == PATH_SEP || filename[1] == '\\')
           || (filename[1] == '.' &&
         (filename[2] == PATH_SEP || filename[2] == '\\'))));
#else
  explicit_relative = (*filename == '.'  && (filename[1] == PATH_SEP
           || (filename[1] == '.' && filename[2] == PATH_SEP))); 
#endif

  return explicit_relative;
/*  return absolute || explicit_relative; */ /* slight rewrite 1994/Feb/13 */
}

#ifdef MSDOS
/*  note: this strips off trailing white space in actual environment var ... */
void striptrailing (string env_value, string env_name, string default_path)
{
  char *s;
  if (env_name == NULL) {         /* 1994/Feb/24 */
    if (trace_flag) {
      sprintf(log_line, "WARNING: no env_name noted, using default %s\n",
        default_path);
      show_line(log_line, 0);
    }
    return;
  }
  if (env_value == NULL) {
    if (trace_flag) {
      sprintf(log_line, "WARNING: %s not defined in environment, using default %s\n",
        env_name, default_path);
      show_line(log_line, 0);
    }
    return;
  }
  if (strlen(env_value) == 0) return;
  s = env_value + strlen(env_value) - 1;
/*  while (*s <= ' ') *s-- = '\0'; */
  while (s >= env_value && *s <= ' ')*s-- = '\0';   /* 94/Feb/24 */
}
#endif

/* convert /! and /!! to / and // 97/Mar/22 */

#ifdef MSDOS
/* 97/Mar/22 */
void convertexclam (string env_value)
{
  char *s;
  if (env_value == NULL) return;
  s = env_value;
  if (strchr(s, '!') == NULL) return;
  while ((s = strchr(s, '!')) != NULL)
  {
    if (*(s+1) == '!')
    {  /* double !! */
      if (*(s+2) == PATH_DELIMITER || *(s+2) == '\0')
      {
        if (s > env_value && *(s-1) == PATH_SEP)
        {
          *s = PATH_SEP;    /* convert the first ! */
          strcpy(s+1, s+2); /* flush the second ! */
        }
      }
    }
    else
    {    /* single ! */  /* assume already unixified */
      if (*(s+1) == PATH_DELIMITER || *(s+1) == '\0')
      {
        if (s > env_value && *(s-1) == PATH_SEP)
          strcpy(s, s+1); /* just flush the ! */
      }
    }
    s++;
  }
  if (trace_flag)
  {
    sprintf(log_line,"Now is %s\n", env_value);
    show_line(log_line, 0);
  }
}
#endif

/* Return a NULL-terminated array of directory names, each name ending
   with PATH_SEP, created by parsing the PATH_DELIMITER-separated list
   in the value of the environment variable ENV_NAME, or DEFAULT_PATH if
   the env var is not set.
   
   A leading or trailing PATH_DELIMITER in the value of ENV_NAME is replaced
   by DEFAULT_PATH.
   
   Any element of the path that ends with double PATH_SEP characters
   (e.g., `foo//') is replaced by all its subdirectories.

   If ENV_NAME is null, only parse DEFAULT_PATH.  If both are null, do
   nothing and return NULL.  */

string * initialize_path_list (string env_name,  string default_path)
{
  string dir, path;
  string *dir_list;
  unsigned dir_count = 0;
  string env_value;
  string orig_path;
  struct _finddata_t findt;
/*  _finddata_t structure *can* be reused, unlike _find_t 95/Jan/31 */
/*  so save on stack space, by having one copy here, not one per expand_subdir*/

/*  env_value = env_name ?  getenv (env_name)  : NULL; */
  env_value = env_name ?  grabenv (env_name)  : NULL; /* 1994/May/19 */

/*  need to convert \ to / as soon as possible to avoid confusion */
/*  we may be modifying DOS environment variable here ... is it always safe ? */
#ifdef MSDOS
  if (deslash) unixify (env_value);       /* 1994/Feb/24 */
#endif
  if (trace_flag) {
    if (env_name) {     /* only if env_name is non-null 94/Feb/24 */
      sprintf(log_line, "\nSet %s=", env_name);
      show_line(log_line, 0);
      if (env_value) {  /* only if env_name value is set */
        show_line(env_value, 0);
      }
      show_line("\n", 0);
    }
  }
#ifdef MSDOS
/*  strip off trailing white space which would confuse things - bkph  */
  striptrailing (env_value, env_name, default_path);
  convertexclam (env_value);              /* 97/Mar/22 */
#endif
  orig_path = expand_default (env_value, default_path);

  if (orig_path == NULL || *orig_path == 0)  return NULL;

/*  need to convert \ to / as soon as possible to avoid confusion */
#ifdef MSDOS
  if (deslash)  unixify (orig_path);  /* redundant ? */
#endif

/*  If we've already seen this PATH_DELIMITER-separated list, then just get
    it back instead of going back to the filesystem.  */
  dir_list = find_dir_list (orig_path);
  if (dir_list != NULL) return dir_list;
  
/*  Be sure `path' is in writable memory.  */ /* if not, copy it */
  path = (orig_path == env_value || orig_path == default_path
          ? xstrdup (orig_path) : orig_path); 

/*  Find each element in the path in turn.  */
#ifdef MSDOS
/*  if (!switchflag) */ 
  if (current_flag) {   /* suppress adding current directory - debugging */
    if (trace_flag) {
      sprintf(log_line, "Adding directory `%s'\n", "."); /* 95/Jan/24 */
      show_line(log_line, 0);
    }
    add_directory(&dir_list, &dir_count, ".");
  }
#endif
  for (dir = strtok (path, PATH_DELIMITER_STRING); dir != NULL;
    dir = strtok (NULL, PATH_DELIMITER_STRING)) {
    int len;

// #ifdef MYDEBUG
  if (trace_flag) {
    sprintf(log_line, "dir %s\n", dir);
    show_line(log_line, 0);
  }
// #endif
      /* If the path starts with ~ or ~user, expand it.  Do this
         before calling `expand_subdir' or `add_directory', so that
         1) we don't expand the same ~ for every subdirectory; and 
         2) pathnames in `expand_subdir' don't have a `~' in them
            (since the system won't grok `~/foo' as a directory).  */
#ifndef MSDOS
    dir = expand_tilde (dir);
#endif
    len = strlen (dir);

      /* If `dir' is the empty string, ignore it.  */
    if (len == 0)  continue;

      /* If `dir' ends in double PATH_SEP, do subdirectories (and remove
         the second PATH_SEP, so the final pathnames we return don't look
         like foo//bar).  Because we obviously want to do subdirectories
         of `dir', we don't check if it is a leaf.  This means that if
         `dir' is `foo//', and `foo' contains only symlinks (so our leaf
         test below would be true), the symlinks are chased.  */

/* modified to treat single PATH_SEP as expand subdirs without recursion */
/* modified to treat double PATH_SEP as expand subdirs *with*  recursion */

#ifdef MSDOS
    if (len > 2 &&                /* 1994/Mar/1 */
      (dir[len - 1] == PATH_SEP || dir[len - 1] == '\\')
        && (dir[len - 2] == PATH_SEP || dir[len - 2] == '\\'))
#else
    if (len > 2 && dir[len - 1] == PATH_SEP && dir[len - 2] == PATH_SEP) 
#endif
    {
      if (open_trace_flag) {
        sprintf(log_line, "Double backslash on `%s' ", dir);  /* bkph */
        show_line(log_line, 0);
      }

      dir[len - 1] = 0;
      if (dir_p (dir)) {
        if (trace_flag) {
          sprintf(log_line, "Adding directory `%s'\n", dir);
          show_line(log_line, 0);
        }
        add_directory (&dir_list, &dir_count, dir);
/* local variable 'findt' used without having been initialized ? &findt ? */
        expand_subdir (&dir_list, &dir_count, dir, findt, 1);  /* 95/Jan/31 */
      }
    }
/* following is new to find only directories to one level 1994/Jan/24 */
#ifdef MSDOS
    else if (len > 1 &&         /* 1994/Mar/1 */
      (dir[len - 1] == PATH_SEP || dir[len - 1] == '\\'))
#else
    else if (len > 1 && dir[len - 1] == PATH_SEP) 
#endif
        {
      if (open_trace_flag) {
        sprintf(log_line, "Single backslash on `%s' ", dir);  /* bkph */
        show_line(log_line, 0);
      }

/*      dir[len - 1] = 0; */
      if (dir_p (dir)) {
        if (trace_flag) {
          sprintf(log_line, "Adding directory `%s'\n", dir);
          show_line(log_line, 0);
        }
        add_directory (&dir_list, &dir_count, dir);
        expand_subdir (&dir_list, &dir_count, dir,
            findt, 0); /* 95/Jan/31 */
      }
    }
      else { /* Don't bother to add the directory if it doesn't exist.  */
      if (dir_p (dir)) {
        if (trace_flag) {
          sprintf(log_line, "Adding directory `%s'\n", dir);
          show_line(log_line, 0);
        }
        add_directory (&dir_list, &dir_count, dir);
      }
    }
  }
  
// #ifdef MYDEBUG
  if (trace_flag) {
    show_line("Adding terminating null\n", 0);
  }
// #endif

/*  Add the terminating null entry to `dir_list'.  */
  dir_count++;
  XRETALLOC (dir_list, dir_count, string);
  dir_list[dir_count - 1] = NULL;
  
/*  Save the directory list we just found.  */
  save_dir_list (orig_path, dir_list);

  return dir_list;
}

/* Subroutines for `initialize_path_list'.  */

/* Add a newly-allocated copy of DIR to the end of the array pointed to
   by DIR_LIST_PTR. Increment DIR_COUNT_PTR.  Append a `/' to DIR if
   necessary.  We assume DIR is a directory, to avoid an unnecessary
   call to `stat'.  */

void add_directory (string **dir_list_ptr, unsigned *dir_count_ptr, string dir)
{
  if (dir == NULL) return;        /* paranoia 1995/Jan/24 */
  /* If `dir' does not end with a `/', add it.  We can't just
     write it in place, since that would overwrite the null that
     strtok may have put in.  So we ALWAYS make a copy of DIR.  */
#ifdef MSDOS
  dir = (dir[strlen (dir) - 1] == PATH_SEP ||   /* 1994/Mar/1 */
      dir[strlen (dir) - 1] == '\\') ?
        xstrdup (dir) : concat (dir, PATH_SEP_STRING);
#else
  dir = (dir[strlen (dir) - 1] == PATH_SEP ? xstrdup (dir)
         : concat (dir, PATH_SEP_STRING)); 
#endif
#ifdef MSDOS
  if (deslash) unixify (dir);     /* redundant ? bkph */ 
#endif

// #ifdef MYDEBUG
  if (trace_flag) {
    sprintf(log_line, "Adding directory `%s'\n", dir);
    show_line(log_line, 0);
  }
// #else
//     if (open_trace_flag) {
//    sprintf(log_line, "Adding directory `%s' ", dir);
//    show_line(log_line, 0);
//  }
// #endif

  /* Add `dir' to the list of the directories.  */
  (*dir_count_ptr)++;
  XRETALLOC (*dir_list_ptr, *dir_count_ptr, string);
  (*dir_list_ptr)[*dir_count_ptr - 1] = dir;
}
/* 1994/Jan/25 */
void lowercase (char *s)
{
  while (*s)
    *s++ = (char) tolower(*s);
}


/* These routines, while not strictly needed to be exported, are
   plausibly useful to be called by outsiders.  */

/* Replace a leading or trailing PATH_DELIMITER in ENV_PATH with
   DEFAULT_PATH.  If neither is present, return ENV_PATH if that is 
   non-null, else DEFAULT_PATH.  */

string  expand_default (string env_path, string default_path)
{
  string expansion;
  
  if (env_path == NULL) expansion = default_path;
  else if (*env_path == PATH_DELIMITER)
    expansion = concat (default_path, env_path);
  else if (env_path[strlen (env_path) - 1] == PATH_DELIMITER)
    expansion = concat (env_path, default_path);
  else expansion = env_path;
  
  if (trace_flag) {               /* 1994/Jan/8 */
    if (env_path == NULL) {
      sprintf(log_line, "Using the default %s\n", expansion);
      show_line(log_line, 0);
    }
    else if (expansion == env_path) {
      sprintf(log_line, "Using %s (default was %s)\n", expansion, default_path);
      show_line(log_line, 0);
    }
    else {                /* expansion != env_path */
      sprintf(log_line, "Expanded %s (default was %s) to %s\n",
        env_path, default_path, expansion);
      show_line(log_line, 0);
    }
  }
  return expansion;
}


#ifndef MSDOS
/* Expand a leading ~ or ~user, Unix-style, unless we are some weirdo
   operating system.  */

string expand_tilde (string name)
{
#if defined (MSDOS) || defined (VMS) || defined (VMCMS)
  return name;
#else
  string expansion;
  string home;
  
  /* If no leading tilde, do nothing.  */
  if (*name != '~')
    expansion = name;
  
  /* If `~' or `~/', use $HOME if it exists, or `.' if it doesn't.  */
  else if (name[1] == PATH_SEP || name[1] == 0)     /* not in DOS */
    {
      home = getenv ("HOME");             /* not in DOS */
      if (home == NULL)
        home = ".";
        
      expansion
        = name[1] == 0 ? home : concat3 (home, PATH_SEP_STRING, name + 2);
    }
  
  /* If `~user' or `~user/', look up user in the passwd database.  */
  else
    {
      struct passwd *p;
      string user;
      unsigned c = 2;
      while (name[c] != PATH_SEP && name[c] != 0) /* not in DOS */
        c++;
      
      user = (string) xmalloc (c);
      strncpy (user, name + 1, c - 1);
      user[c - 1] = 0;
      
      /* We only need the cast here for those (old deficient) systems
         which do not declare `getpwnam' in <pwd.h>.  */
      p = (struct passwd *) getpwnam (user);
      free (user);
      /* If no such user, just use `.'.  */
      home = p == NULL ? "." : p->pw_dir;
      
      expansion = name[c] == 0 ? home : concat (home, name + c);
    }
  
  return expansion;
#endif /* not (DOS or VMS or VM/CMS) */
}
#endif

// structure used for manipulation dir lists

typedef struct {
  string path;
  string *dir_list;
} saved_path_entry;

/* Routines to save and retrieve a directory list keyed by the original
   PATH_DELIMITER-separated path.  This is useful because 1) it can take a
   significant amount of time to discover all the subdirectories of a
   given directory, and 2) many paths all have the same basic default,
   and thus would recompute the directory list.  */

static saved_path_entry *saved_paths = NULL;
static unsigned saved_paths_length = 0;

/* We implement the data structure as a simple linear list, since it's
   unlikely to ever be more than a dozen or so elements long.  We don't
   bother to check here if PATH has already been saved; we always add it
   to our list.  */
/* NOT USED! */
void save_dir_list (string path,  string *dir_list)
{
//  saved_paths_length++;
  XRETALLOC (saved_paths, saved_paths_length+1, saved_path_entry);
  saved_paths[saved_paths_length].path = path;
  saved_paths[saved_paths_length].dir_list = dir_list;
  saved_paths_length++;
}

/* When we retrieve, just check the list in order.  */
/* NOT USED! */
string *find_dir_list (string path)
{
  unsigned p;
  
// #ifdef MYDEBUG
  if (trace_flag) {
    sprintf(log_line, "Find Dir List for path: %s\n", path);
    show_line(log_line, 0);
  }
// #endif

  for (p = 0; p < saved_paths_length; p++) {
    if (strcmp (saved_paths[p].path, path) == 0)
      return saved_paths[p].dir_list;
  }
  return NULL;
}

/* Unixify filename and path (turn \ into /) --- assumes null terminated */
/* NEED HACK! */
char *unixify (char * t)
{
  char * s = t;
  if (s == NULL) return s;    /* paranoia -- 1993/Apr/10 */
#ifdef MSDOS
  if (t != '\0') {
    while (*s != '\0') {        /* paranoia -- 1997/Oct/23 */
/*      if (*s == '\\') *s = '/'; */
      if (*s == '\\') *s = PATH_SEP;
      s++;
    }       /* endwhile */
  }
// #ifdef MYDEBUG
  if (trace_flag) {
    sprintf(log_line, "Unixified name: %s\n", t);
    show_line(log_line, 0);
  }
// #endif
#endif /* DOS */
  return t;
}

/****************************************************************************/

/* moved here to avoid problems with pragma */

/* struct _finddata_t findt; */ /* make global, can be reused unlike _find_t */
                                /* avoids heavy stack usage in tree search */
                                /* but ties up some global fixed space ... */

//#pragma optimize ("g", off)   /* try and avoid compiler bug here _dos_find */

/* Add DIRNAME to DIR_LIST and look for subdirectories, possibly recursively.
   We assume DIRNAME is the name of a directory.  */

/* NOTE: _dos_find... prevents running under Windows NT as console app ??? */
/* Yes, so lets flush it! use _findfirst, _findnext, _findclose instead */

/* called only from initialize_path_list  (and recursively) */
/* kpathsea/elt-dirs.c */
/* renamed to do_subdir */
void expand_subdir (string **dir_list_ptr, unsigned *dir_count_ptr, string dirname,
          struct _finddata_t findt, integer recurseflag)
{
#ifdef MSDOS
/*  struct _finddata_t findt; */
  long hFind;
  int ret;
  int len;
/*  char buffer[PATH_MAX]; */   /* pretty long? potential recursion problem? */
  char buffer[FILENAME_MAX];  /* this is DOS and Windows NT after all ... */
  char *potential;
#endif  /* DOS */

  if (trace_flag)
  {
    sprintf(log_line, "\nExpanding sub dir %s ", dirname);
    show_line(log_line, 0);
  }

#ifdef MSDOS
  strcpy(buffer, dirname);
  len = strlen(dirname);

#ifdef MSDOS
/*  if (buffer[len-1] == PATH_SEP) strcat(buffer, "*.*"); */
  if (buffer[len-1] == PATH_SEP || buffer[len-1] == '\\')
    strcat(buffer, "*.*");            /* 1994/Mar/1 */
  else strcat(buffer, PATH_SEP_STRING "*.*");
#else
  if (buffer[len-1] == PATH_SEP) strcat(buffer, "*");
  else strcat(buffer, PATH_SEP_STRING "*");
#endif  /* MSDOS */

/*  Note: the _A_SUBDIR means we get ordinary files PLUS sub-directories */
  if (open_trace_flag)  {
    sprintf(log_line, "\nDIRNAME `%s' ", dirname);
    show_line(log_line, 0);
  }
/*  we'll need to step over `.' and `..' up front of directory list */
  hFind = _findfirst(buffer, &findt);
  if (hFind > 0) ret = 0;
  else ret = -1;
/*  _dos_findnext( &findt ); */
/*  while(_dos_findnext(&findt)== 0)  { */
  while (ret == 0)  {
/*    if (open_trace_flag) */
    if (open_trace_flag && trace_flag) {
      sprintf(log_line, "NEXT `%s' (%0x) ", findt.name, findt.attrib);
      show_line(log_line, 0);
    }
/*    if (strchr(findt.name, '.') != NULL) continue; *//* not needed */
    if (findt.name[0] != '.' &&   /* ignore "." and ".." */
      findt.attrib & _A_SUBDIR){  /* only look at SUBDIRs */
      if (open_trace_flag)  {
        sprintf(log_line, "\nDIRNAME `%s' ", dirname);
        show_line(log_line, 0);
      }
#ifdef MSDOS
      potential = concat3(dirname,
        (dirname[len-1] == PATH_SEP || dirname[len-1] == '\\')
          ? "" : PATH_SEP_STRING, findt.name);
#else
      potential = concat3(dirname, dirname[len-1] == PATH_SEP 
        ? "" : PATH_SEP_STRING, findt.name);
#endif  /* DOS */
      lowercase (potential);          /* make look nicer ? */
      if (open_trace_flag) {
        sprintf(log_line, "POTENTIAL `%s' ", potential);
        show_line(log_line, 0);
      }
      if (trace_flag) {
        sprintf(log_line, "Adding directory `%s'\n", potential); /* 95/Jan/24 */
        show_line(log_line, 0);
      }
      add_directory(dir_list_ptr, dir_count_ptr, potential);
      if (recurseflag) 
        expand_subdir(dir_list_ptr, dir_count_ptr,
            potential, findt, 1);  /* 95/Jan/31 */
      free(potential);
    }     /* end of findt.attrib & _A_SUBDIR != 0 */
    ret = _findnext(hFind, &findt);
  }
#ifdef MSDOS
  if (hFind > 0) _findclose (hFind);
#endif

#ifndef MSDOS
  _dos_findclose(&findt);
#endif

#else  /* end of MSDOS (way up there) */

/*  This is how we do this if we are NOT using DOS */
  DIR *dir;
  struct dirent *e;
  unsigned length;
  char potential[PATH_MAX];
  struct _stat st;
  
   /* We will be looking at its contents.  */
  dir = opendir (dirname);
  if (dir == NULL)
    return;
  
  /* Compute the length of DIRNAME, since it's loop-invariant.  */
  length = strlen (dirname);

  /* Construct the part of the pathname that doesn't change.  */
  strcpy (potential, dirname);
  if (potential[length - 1] != PATH_SEP)  /* not in DOS */
    {
      potential[length] = PATH_SEP;
      potential[length + 1] = 0;
      length++;
    }
  
/* about to use _stat --- shouldn't get here when using MSDOS anyway */

  while ((e = readdir (dir)) != NULL)
    { /* If it's . or .., never mind.  */
      if (!(e->d_name[0] == '.'
            && (e->d_name[1] == 0
                || (e->d_name[1] == '.' && e->d_name[2] == 0))))
        { /* If it's not a directory, we will skip it on the
             recursive call.  */
          strcat (potential, e->d_name);

          /* If we can't _stat it, or if it isn't a directory, continue.  */
          if (_stat (potential, &st) == 0 && S_ISDIR (st.st_mode))
            { /* It's a subdirectory; add `potential' to the list.  */
        if (trace_flag) {
          sprintf(log_line, "Adding directory `%s'\n", potential); /* 95/Jan/24 */
          show_line(log_line, 0);
        }
              add_directory (dir_list_ptr, dir_count_ptr, potential);

              /* If it's not a leaf, quit.  Assume that leaf
                 directories have two links (one for . and one for ..).
                 This means that symbolic links to directories do not affect
                 the leaf-ness.  This is arguably wrong, but the only
                 alternative I know of is to _stat every entry in the
                 directory, and that is unacceptably slow.  */
              if (st.st_nlink > 2)
                { /* All criteria are met; find subdirectories.  */
                  expand_subdir (dir_list_ptr, dir_count_ptr, potential,
            findt, 1);  /* 95/Jan/31 */
                }
            }

          /* ``Remove'' the directory entry name.  */
          potential[length] = 0;
        }
    }
  
  closedir (dir);
#endif  /* end of *not* DOS case */
}

char *get_env_shroud (char *);    /* in texmf.c */
