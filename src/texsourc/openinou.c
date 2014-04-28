/* Copyright 1992 Karl Berry
   Copyright 2007 TeX Users Group
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

#include "texwin.h"

#pragma warning(disable:4996)
#pragma warning(disable:4131) // old style declarator
#pragma warning(disable:4135) // conversion between different integral types
#pragma warning(disable:4127) // conditional expression is constant

#include <setjmp.h>

#include <direct.h>           /* for _getcwd() */

#define EXTERN extern

#include "texd.h"

#define PATH_SEP        '/'
#define PATH_SEP_STRING "/"

/* openinout.c: open input and output files. */

#define BUILDNAMEDIRECT         /* avoid malloc for string concat */

bool test_read_access (unsigned char *, int);   /* in ourpaths.c - bkph */

extern char *unixify (char *);      /* in pathsrch.c bkph */

extern void try_and_open (char *);    /* in local.c bkph */

extern int shorten_file_name;       /* in local.c bkph */

#ifdef FUNNY_CORE_DUMP
/* This is defined in ./texmf.c.  */
extern void funny_core_dump();
#endif /* FUNNY_CORE_DUMP */

#ifdef MSDOS

#ifdef BUILDNAMEDIRECT
/* kpathsea/concat.c */
/* kpathsea/concat3.c */
/* similar to concat, but AVOIDS using malloc, pass in place to put result */
char *xconcat (char *buffer, char *s1, char *s2)
{
  int n1 = strlen(s1);
  int n2 = strlen(s2);

  if (buffer == s2)
  {     /* treat special case of overlap */
    memmove (buffer + n1, buffer, n2 + 1); /* trailing null ! */
    strncpy (buffer, s1, n1);
  }
  else
  {
    strcpy(buffer, s1);
    strcat(buffer + n1, s2);
  }

  return buffer;
}
/* similar to concat3, but avoids using malloc, pass in place to put result */
char *xconcat3 (char *buffer, char *s1, char *s2, char *s3)
{
  int n1 = strlen(s1);
  int n2 = strlen(s2);
  int n3 = strlen(s3);

  if (buffer == s3)
  {     /* treat special case of overlap */
    memmove (buffer + n1 + n2, buffer, n3 + 1); /* trailing null ! */
    strncpy (buffer, s1, n1);
    strncpy (buffer + n1, s2, n2);
  }
  else
  {
    strcpy(buffer, s1);
    strcat(buffer + n1, s2);
    strcat(buffer + n1 + n2, s3);
  }
  return buffer;
}
#endif /* end of BUILDNAMEDIRECT  */

#endif  /* end of ifdef MSDOS ??? */

#ifdef MSDOS
/* separated out 1996/Jan/20 to make easier to read */
/* assumes path does not end in PATH_SEP */
void patch_in_path (unsigned char *buffer, unsigned char *name, unsigned char *path)
{
#ifdef BUILDNAMEDIRECT
  if (*path == '\0')
    strcpy((char *) buffer, (char *) name);
  else
    xconcat3((char *) buffer, (char *) path, PATH_SEP_STRING, (char *) name);
#else
  string temp_name;
  temp_name = concat3 (path, PATH_SEP_STRING, name);
  strcpy (buffer, temp_name);
  free (temp_name);
#endif
}

int qualified (unsigned char * name)
{
  if (strchr((char *) name, PATH_SEP) != NULL ||
      strchr((char *) name, '\\') != NULL ||
      strchr((char *) name, ':') != NULL)
    return 1;
  else
    return 0;
}
/* patch path if 
    (i)   path not empty
    (ii)  name not qualified
    (iii) ext match
*/
int prepend_path_if (unsigned char *buffer, unsigned char *name, char *ext, unsigned char *path)
{
  if (path == NULL)
    return 0;

  if (*path == '\0')
    return 0;

  if (qualified(name))
    return 0;

  if (strstr((char *)name, ext) == NULL)
    return 0;

  patch_in_path(buffer, name, path);

  return 1;
}
#endif      /* end of MSDOS */

/*  Following works on null-terminated strings */

void check_short_name (unsigned char *s)
{
  unsigned char *star, *sdot;
  int n;

  if ((star = (unsigned char *) strrchr((char *) s, '\\')) != NULL)
    star++;
  else if ((star = (unsigned char *) strrchr((char *) s, '/')) != NULL)
    star++;
  else if ((star = (unsigned char *) strchr((char *) s, ':')) != NULL)
    star++;
  else
    star = s;

  if ((sdot = (unsigned char *) strchr((char *) star, '.')) != NULL)
    n = sdot - star;
  else
    n = strlen((char *) star);

  if (n > 8)
    strcpy((char *) star + 8, (char *) star + n);

  if ((sdot = (unsigned char *) strchr((char *) star, '.')) != NULL)
  {
    star = sdot + 1;

    n = strlen((char *) star);

    if (n > 3)
      *(star + 3) = '\0';
  }
}

/* Following works on both null-terminated names */
/* reconvert 254 to '~' in file name 95/Sep/26 */
/* reconvert 255 to ' ' in file name 95/Sep/26 */
/* we do this in tex3.c start_input() -> scan_file_name() now 95/Sep/26 */
/* kpathsea/tilde.c */
void retwiddle (unsigned char *s)
{
/* assumes null terminated - 97/June/5 */
/*  while (*s != '\0' && *s != ' ') { */
  while (*s != '\0')
  {
    if (*s == (unsigned char) pseudo_tilde)
      *s = '~';
    else if (*s == (unsigned char) pseudo_space)
      *s = ' ';
    s++;
  }
}

/* #endif */ /* ??? */

/* Open an input file F, using the path PATHSPEC and passing
   FOPEN_MODE to fopen.  The filename is in `name_of_file', as a Pascal
   string. We return whether or not the open succeeded.  If it did, we
   also set `name_length' to the length of the full pathname that we
   opened.  */
/* in lib/openclose.c */
bool open_input (FILE **f, path_constant_type path_index, char *fopen_mode)
{
  bool openable = false;

#if defined (FUNNY_CORE_DUMP) && !defined (BibTeX)
  if (path_index == TEXINPUTPATH &&
      strncmp (name_of_file + 1, "HackyInputFileNameForCoreDump.tex", 33) == 0)
    funny_core_dump();
#endif /* FUNNY_CORE_DUMP and not BibTeX */

#ifdef MSDOS
  if (return_flag)
  {
    if (strcmp(fopen_mode, "r") == 0)
      fopen_mode = "rb";    /* so can catch `return' bkph */
  }
#endif /* MSDOS */

  name_of_file[name_length + 1] = '\0'; /* null terminate */

/* reinsert '~' and ' ' in file names -  95/June/5 */
/* done late to prevent problems with  null_terminate / space_terminate */  
  if (pseudo_tilde != 0 || pseudo_space != 0)
    retwiddle(name_of_file + 1);

#ifdef MSDOS
/* 8 + 3 file names on Windows NT 95/Feb/20 */
  if (shorten_file_name)
  {
    check_short_name(name_of_file + 1);
  }
#endif  /* MSDOS */
  
  if (open_trace_flag)
  {
    sprintf(log_line, " Open `%s' for input ", name_of_file + 1); /* Pascal */
    show_line(log_line, 0);
  }

  if (test_read_access(name_of_file + 1, path_index))
  {
/*  if (test_read_access(name_of_file, name_length, path_index)) */

/*    We can assume `name_of_file' is openable, */
/*    since `test_read_access' just returned true.  */
/*    *f = xfopen_pas (name_of_file, fopen_mode); */
    *f = xfopen((char *) name_of_file + 1, fopen_mode);

//    should we check *f == NULL ??? (should be OK because of test_read_access)

/*    If we found the file in the current directory, don't leave the
        `./' at the beginning of `name_of_file', since it looks dumb when
        TeX says `(./foo.tex ...)', and analogously for Metafont.  */
#ifdef MSDOS
    if (name_of_file[1] == '.' &&         /* 1994/Mar/1 */
      (name_of_file[2] == PATH_SEP || name_of_file[2] == '\\'))
#else
    if (name_of_file[1] == '.' && name_of_file[2] == PATH_SEP) 
#endif
    {
      unsigned i = 1;
/*        while (name_of_file[i + 2] != ' ') */
      while (name_of_file[i + 2] != '\0')
      {
        name_of_file[i] = name_of_file[i + 2];
        i++;
      }
/*      name_of_file[i] = ' '; */
      name_of_file[i] = '\0';
      name_length = i - 1;
    }
    else
/*      name_length = strchr(name_of_file + 1, ' ') - (name_of_file + 1); */
      name_length = strlen((char *) name_of_file + 1);
      
#ifdef TeX
/*    If we just opened a TFM file, we have to read the first byte,
        since TeX wants to look at it.  What a kludge.  */
/*    See comments in ctex.ch for why we need this.  */
    if (path_index == TFMFILEPATH)
    {
      tfm_temp = getc (*f);
    }
#endif /* TeX */  

#ifdef MSDOS
/*    code added 94/June/21 to show 'fmt' file opening in log */
    if (strstr((char *) name_of_file + 1, ".fmt") != NULL)
    {
      if (format_file == NULL)
      {
        format_file = xstrdup((char *) name_of_file + 1);
      }
    } /* remember full format file name with path */
    else if (strstr((char *)name_of_file+1, ".poo") != NULL)
    {
      if (string_file == NULL)
      {
        string_file = xstrdup((char *) name_of_file + 1);
      }
    } /* remember full pool file name with path */
    else if (strstr((char *)name_of_file+1, ".tfm") != NULL)
    {
      if (show_tfm_flag && log_opened)
      {
#ifdef WRAPLINES
        int old_setting = selector;
        char *s = name_of_file + 1;
        selector = log_only;
        print_char(' ');
        print_char('(');

        while (*s != '\0')
          print_char (*s++);

        print_char(')');
        selector = old_setting;
#else
        int n; 
        n = strlen((char *) name_of_file + 1);

        if (file_offset + n > max_print_line)
        {
          putc('\n', log_file);
          file_offset = 0;
        } /* somewhat risky ? */
        else
          putc(' ', log_file);

        fprintf(log_file, "(%s)", name_of_file + 1);
        file_offset += n+3;
/*        space_terminate (name_of_file + 1);  */
#endif  /*  end of WRAPLINES */
      }
    }
/*    code added 98/Sep/29 to catch first file input */
/*    is there a problem if this file bombs ? */
    else if (source_direct == NULL) /* 98/Sep/29 */
    {
      char *s;

      source_direct = xstrdup((char *) name_of_file + 1);

      if (trace_flag)
      {
        sprintf(log_line, "Methinks the source %s is `%s'\n", "file", source_direct);
        show_line(log_line, 0);
      }

      if ((s = strrchr(source_direct, '/')) == NULL)
        *source_direct = '\0';
      else
        *(s+1) = '\0';

      if (trace_flag)
      {
        sprintf(log_line, "Methinks the source %s is `%s'\n", "directory", source_direct);
        show_line(log_line, 0);
      }
    }
#endif  /* end of MSDOS */
    openable = true;
  }
/*  space_terminate (name_of_file + 1); */
  {
    unsigned temp_length = strlen((char *) name_of_file + 1);
    name_of_file[temp_length + 1] = ' ';  /* space terminate */
/*    set up name_length ??? */
  }

  return openable;
}

/* Call the external program PROGRAM, passing it `name_of_file'.  */
/* This nonsense probably only works for Unix anyway. bkph */
/* For one thing, MakeTeXTFM etc is more than 8 characters ! */

#ifdef MSDOS
  #define NO_MAKETEX
#endif

#define TEXONLY

char *get_env_shroud (char *);    /* defined in texmf.c */

/* char outputdirectory[PATH_MAX]; */       /* defined in local.c */

extern char * dvi_directory; /* defined in local.c */
extern char * log_directory; /* defined in local.c */
extern char * aux_directory; /* defined in local.c */
extern char * fmt_directory; /* defined in local.c */
extern char * pdf_directory; /* defined in local.c */

/* At least check for I/O error (such as disk full) when closing */
/* Would be better to check while writing - but this is better than nothing */
/* This is used for both input and output files, but never mind ... */

/* now a_close returns -1 on error --- which could be used by caller */
/* probably want to ignore on input files ... */

void perrormod (char *s);       /* in local.c */

// check_fclose not used by anything
/* 1993/Nov/20 - bkph */
int check_fclose (FILE * f)
{
  if (f == NULL)
    return 0;      // sanity check

  if (ferror(f) || fclose (f))
  {
    perrormod("\n! I/O Error");
    uexit (1);    // ???
  }

  return 0;
}

/* open_output moved down here to avoid potential pragma problem */

bool open_output (FILE **f, char *fopen_mode)
{
  unsigned temp_length;

  name_of_file[name_length + 1] = '\0'; /* null terminate */

  if (pseudo_tilde != 0 || pseudo_space !=  0)
  {
    retwiddle(name_of_file + 1);
  }

#ifdef MSDOS
/* 8 + 3 file names on Windows NT 95/Feb/20 */
  if (shorten_file_name)
  {
    check_short_name(name_of_file + 1);
  }
#endif

#ifdef MSDOS

  if (prepend_path_if (name_of_file + 1, name_of_file + 1, ".dvi", (unsigned char *) dvi_directory) ||
      prepend_path_if (name_of_file + 1, name_of_file + 1, ".log", (unsigned char *) log_directory) ||
      prepend_path_if (name_of_file + 1, name_of_file + 1, ".aux", (unsigned char *) aux_directory) ||
      prepend_path_if (name_of_file + 1, name_of_file + 1, ".fmt", (unsigned char *) fmt_directory) ||
      prepend_path_if (name_of_file + 1, name_of_file + 1, ".pdf", (unsigned char *) pdf_directory))
  {
    if (open_trace_flag)
    {
      sprintf(log_line, "After prepend %s\n", name_of_file+1);
      show_line(log_line, 0);
    }
  }
#endif

  if (open_trace_flag)
  {
    sprintf(log_line, " Open `%s' for output ", name_of_file + 1);
    show_line(log_line, 0);
  }

/* Is the filename openable as given?  */

/*  if share_flag is non-zero and we are opening for reading use fsopen */
/*  but we can assume this is opening here for *output* */
  *f = fopen((char *) name_of_file + 1, fopen_mode);

/* Can't open as given.  Try the envvar.  */
  if (*f == NULL)
  {
    string temp_dir = get_env_shroud ("UFYNGPVU");

/*    if (deslash) unixify(temp_dir); */    /* deslashify 93/Dec/28 */

    if (temp_dir != NULL)
    {
#ifdef BUILDNAMEDIRECT
      unsigned char temp_name[PATH_MAX];
      xconcat3((char *) temp_name, temp_dir, PATH_SEP_STRING, (char *) name_of_file + 1);
#else
/*    string temp_name = concat3 (temp_dir, "/", name_of_file + 1); */
      string temp_name = concat3 (temp_dir, PATH_SEP_STRING, name_of_file + 1);
#endif
      if (deslash)
        unixify((char *) temp_name);     /* deslashify 93/Dec/28 */
/*  If share_flag is non-zero and we are opening for reading use fsopen */
/*  but we can assume this is opening here for *output* */
      *f = fopen((char*)temp_name, fopen_mode);
/*  If this succeeded, change name_of_file accordingly.  */
      if (*f)
        strcpy((char*) name_of_file + 1, (char *) temp_name);
#ifndef BUILDNAMEDIRECT
      free (temp_name);
#endif
    }
  }

  if (strstr((char *) name_of_file + 1, ".dvi") != NULL)
  {
    if (qualified(name_of_file + 1))
      *log_line = '\0';
    else
    {
      (void) _getcwd(log_line, sizeof(log_line));
      strcat(log_line, PATH_SEP_STRING);
    }

    strcat(log_line, (char*) name_of_file + 1);
    unixify(log_line);
    dvi_file_name = xstrdup(log_line);
  }
  else if (strstr((char *)name_of_file + 1, ".log") != NULL)
  {
    if (qualified(name_of_file + 1))
      *log_line = '\0';
    else
    {
      (void) _getcwd(log_line, sizeof(log_line));
      strcat(log_line, PATH_SEP_STRING);
    }

    strcat(log_line, (char *) name_of_file + 1);
    unixify(log_line);
    log_file_name = xstrdup(log_line);
  }

  temp_length = strlen ((char *)name_of_file + 1);
  name_of_file[temp_length+1] = ' ';

  if (*f)
    name_length = temp_length;
  
  return *f != NULL;
}