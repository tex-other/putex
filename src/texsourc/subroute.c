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
#include <kpathsea/absolute.h>
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

extern bool usesourcedirectory; /* in local.c */

extern bool workingdirectory;   /* in local.c */

string truncate_pathname (string name);
char * file_p(string fn);
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
    i = (integer) (r + 0.5);
  else
    i = (integer) (r - 0.5);

  return i;
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

  return c == '\n' || c == '\r' || c == EOF;
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

#define BUILDNAMEDIRECT /* avoid malloc for string concat */

/* Modified 97/May/17 to avoid malloc for each line read */

#ifndef MALLOCLINE
  #define MAXLINE 256
#endif

void unshroud_string (char *, char *, int); /* in texmf.c */

/* NOTE: _dos_find... prevents running under Windows NT ??? */
/* This is called if file_method != 0 */ /* which is currently the default */

#ifdef MSDOS
/* see whether a file exists, is readable and is not a directory */
/* 1994/Feb/13 may be faster than `access' in `readable' */
/* returns NULL or the name filename passed in ??? */
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

/* Add a newly-allocated copy of DIR to the end of the array pointed to
   by DIR_LIST_PTR. Increment DIR_COUNT_PTR.  Append a `/' to DIR if
   necessary.  We assume DIR is a directory, to avoid an unnecessary
   call to `stat'.  */

/* 1994/Jan/25 */
void lowercase (char *s)
{
  while (*s)
    *s++ = (char) tolower(*s);
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

/* Unixify filename and path (turn \ into /) --- assumes null terminated */
/* NEED HACK! */
char *unixify (char * t)
{
  char * s = t;

  if (s == NULL)
    return s;    /* paranoia -- 1993/Apr/10 */

#ifdef MSDOS
  if (t != '\0')
  {
    while (*s != '\0') {        /* paranoia -- 1997/Oct/23 */
/*      if (*s == '\\') *s = '/'; */
      if (*s == '\\')
        *s = PATH_SEP;

      s++;
    }       /* endwhile */
  }
// #ifdef MYDEBUG
  if (trace_flag)
  {
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

char *get_env_shroud (char *);    /* in texmf.c */