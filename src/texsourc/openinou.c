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

/* openinout.c: open input and output files.  These routines used by
   TeX, Metafont, and BibTeX.  */

/* #include <sys\stat.h> */       /* debugging 94/Mar/2 */
/* #include <sys\types.h> */      /* debugging 94/Mar/2 */
/* #include <conio.h> */        /* for getch */

// #include "config.h"
// #include "c-namemx.h"
// #include "c-pathch.h"

#define BUILDNAMEDIRECT         /* avoid malloc for string concat */

bool test_read_access (unsigned char *, int);   /* in ourpaths.c - bkph */
/* bool test_read_access (char *, int, int); */ /* in ourpaths.c - bkph */

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
/* patch path if (i) path not empty (ii) name not qualified (iii) ext match */
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

/* void check_short_name(void) { */       /* 1995/Feb/20 */
void check_short_name (unsigned char *s)
{         /* 1995/Sep/26 */
  unsigned char *star, *sdot;
  int n;

/*  if ((star = strrchr(name_of_file+1, '\\')) != NULL) star++;
  else if ((star = strrchr(name_of_file+1, '/')) != NULL) star++;
  else if ((star = strchr(name_of_file+1, ':')) != NULL) star++;
  else star = name_of_file+1; */        /* 1995/Sep/26 */
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

void retwiddle (unsigned char *s)
{ /* assumes null terminated - 97/June/5 */
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
/*  This only applies if a preloaded TeX (or Metafont) is being made;
  it allows for automatic creation of the core dump (typing ^\
  requires manual intervention).  */
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
    check_short_name(name_of_file + 1);           /* 95/Sep/26 */
/*    space_terminate (name_of_file + 1); */
  }
#endif  /* MSDOS */

#ifdef BibTeX
  if (path_index == NO_FILE_PATH)
  {
    unsigned temp_length;

/*      null_terminate (name_of_file + 1); */
/*  if share_flag is non-zero and we are opening for reading use fsopen */
/*  but we can assume here that we are opening for *input* */
/*      *f = fopen (name_of_file + 1, fopen_mode); */
    if (share_flag == 0)
      *f = fopen (name_of_file + 1, fopen_mode);
    else
      *f = _fsopen (name_of_file + 1, fopen_mode, share_flag);
    temp_length = strlen (name_of_file + 1);
/*      space_terminate (name_of_file + 1); */

    if (*f != NULL)
    {
      name_length = temp_length;
      openable = true;
    }
  }

  else
#endif /* BibTeX */
  
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
    if (path_index == TFMFILEPATH)
    { /* See comments in ctex.ch for why we need this.  */
/*          extern integer tfm_temp; */ /* see texd.h for definition */
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
        *source_direct='\0';
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

/* the string program is unreferenced in DOS NO_MAKETEX */

static bool
make_tex_file (string program)
{
#ifdef NO_MAKETEX
  return 0;
#else
  char cmd[NAME_MAX + 1 + PATH_MAX + 1];
  unsigned cmd_len;
  int ret;
  unsigned i = 1; /* For copying from `name_of_file'.  */

  /* Wrap another sh around the invocation of the MakeTeX program, so we
     can avoid `sh: MakeTeXTFM: not found' errors confusing the user.
     We don't use fork/exec ourselves, since we'd have to call sh anyway
     to interpret the script.  */
#ifdef MSDOS
  strcpy (cmd, "command.com ");
#else
  strcpy (cmd, "sh -c ");
#endif
  
/*  strcat (cmd, program); */ /* shrouded 93/Nov/20 */
  strcat (cmd, "Make");
#ifndef MSDOS
  strcat (cmd, "TeX");
#endif
  strcat (cmd, program);
  cmd_len = strlen (cmd);
  cmd[cmd_len++] = ' ';

  while (name_of_file[i] != ' ')
    cmd[cmd_len++] = name_of_file[i++];

  /* Add terminating null.  */
  cmd[cmd_len] = 0;

  /* Don't show any output.  */
#ifdef MSDOS
  strcat (cmd, "> nul");  /* ? 93/Nov/20 */
#else
  strcat (cmd, ">/dev/null 2>&1");
#endif

/* Run the command, and return whether or not it succeeded.  */
  ret = system (cmd);
  return ret == EXIT_SUCCESS_CODE;
#endif /* not NO_MAKE_TEX */
}

#define TEXONLY

/* This is called by TeX if an \input resp. TFM file can't be opened.  */

bool maketextex (void)          /* called in tex3.c and tex8.c */
{
/*  return make_tex_file ("MakeTeXTeX"); */
  return make_tex_file ("TeX"); 
}

bool maketextfm (void)          /* called in tex3.c */
{
/*  return make_tex_file ("MakeTeXTFM"); */
  return make_tex_file ("TFM");
}

#ifndef TEXONLY
bool maketexmf (void)
{
/*  return make_tex_file ("MakeTeXMF"); */
  return make_tex_file ("MF");
}
#endif /* ifndef TEXONLY */

char *get_env_shroud (char *);    /* defined in texmf.c */

/* char outputdirectory[PATH_MAX]; */       /* defined in local.c */

extern char *dvi_directory;       /* defined in local.c */
extern char *log_directory;       /* defined in local.c */
extern char *aux_directory;       /* defined in local.c */

#ifdef IGNORED
/* Try and figure out if can write to current directory */
bool isitsafe (char *name)
{
/*  struct stat statbuf; */         /* debugging 94/Mar/2 */
/*  Can't test access on file, since fails if not exist */
/*  Can't test access on `nul', since always fails */ 
/*  Can   test access on `.', but its true on locked diskette! */
/*  stat on directory always says read an write permission */
  return true;        /* for now */
}
#endif

/* open_output moved to end to avoid pragma problems 96/Sep/15 */

/* used only in start_input in tex3.c, and in open_or_close_in in tex8.c */
/* modified 97/June/5 to take null terminated (C) string */

#ifdef IGNORED
bool extensionirrelevantaux (char *base, char *suffix)
{ 
  bool ret;
/*  make_c_string (&base);  */
/*  base[nlen+1] = '\0'; */     /* null terminate */
#ifdef MSDOS
/*  In DOS, an extension is irrelevant if there already is an extension ! */
/*  MAY NEED TO REVISE IN WIN32 where we can have foo.bar.chomp.tex ??? */
  {               /* simplification 1996/Jan/20 ??? */
    char *s, *t;
    if ((s = strrchr (base, '.')) == NULL)
      ret = 0; /* no extension */
    else
    {
      if ((t = strrchr (base, PATH_SEP)) != NULL ||
          (t = strrchr (base, '\\')) != NULL ||
          (t = strrchr (base, ':')) != NULL)
      {
        if (t > s)
          ret = 0; /* last dot occurs in path - no extension */
        else
          ret = 1;     /* last dot occurs in file name itself */
      }
      else
        ret = 1;       /* name not qualified and has dot */
    }
  }
#else /*  not MSDOS */
  {
    char temp[PATH_MAX];
    strcpy (temp, base);
    strcat (temp, ".");
    strcat (temp, suffix);
    ret = same_file_p (base, temp);
  }
#endif /* end of not MSDOS */
/*  make_pascal_string (&base); */
/*  base[nlen+1] = ' '; */      /* space terminate */
  return ret;
}
#endif /* IGNORED */

/* Test if the Pascal string BASE concatenated with the extension
   `.SUFFIX' is the same file as just BASE.  SUFFIX is a C string.  */

/* used in `start_input' (tex3.c) and open_or_close_in (tex8.c) */
/* used to always return true, since in DOS can have only one extension */
/* modified 98/Feb/7 to always return false */

bool extensionirrelevantp (unsigned char *base, int nlen, char *suffix)
{ 
#ifdef IGNORED
  bool ret;
  base[nlen+1] = '\0';      /* null terminate */
  ret = extensionirrelevantaux(base+1, suffix);
  base[nlen+1] = ' ';     /* space terminate */
  return ret;
#endif
  return false;
}

/* #define a_close(f) if (f) { if (ferror (f)) {perror(""); exit(1);} } if (f) (void) fclose (f) */
/* #define a_close(f) if (f) (void) check_fclose (f) */

/* At least check for I/O error (such as disk full) when closing */
/* Would be better to check while writing - but this is better than nothing */
/* This is used for both input and output files, but never mind ... */

/* now a_close returns -1 on error --- which could be used by caller */
/* probably want to ignore on input files ... */

void perrormod (char *s);       /* in local.c */

// check_fclose not used by anything
/* 1993/Nov/20 - bkph */
int check_fclose (FILE *f)
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

/* #pragma optimize ("g", off) *//* try and avoid compiler bug here */

/* Open an output file F either in the current directory or in
   $TEXMFOUTPUT/F, if the environment variable `TEXMFOUTPUT' exists.
   (Actually, this applies to the BibTeX output files, also, but
   `TEXMFBIBOUTPUT' was just too long.)  The filename is in the global
   `name_of_file', as a Pascal string.  We return whether or not the open
   succeeded.  If it did, the global `name_length' is set to the length
   of the actual filename.  */

bool open_output (FILE **f, char *fopen_mode)
{
  unsigned temp_length;

/*  null_terminate (name_of_file + 1);  */  /* moved here 95/Sep/26  */
  name_of_file[name_length + 1] = '\0'; /* null terminate */

/* reinsert '~' and ' ' in file names -  95/June/5 */
/* done late to prevent problems with  null_terminate / space_terminate */  
  if (pseudo_tilde != 0 || pseudo_space !=  0)
  {
    retwiddle(name_of_file + 1);
  }

#ifdef MSDOS
/* 8 + 3 file names on Windows NT 95/Feb/20 */
  if (shorten_file_name)
  {
    check_short_name(name_of_file + 1);         /* 95/Sep/26 */
  }
#endif

/*  null_terminate (name_of_file + 1); */ /* Make the filename into a C string.  */

/*  if (debug_flag) try_and_open(name_of_file+1); */  /* debugging 94/Mar/20 */

#ifdef MSDOS
/* write into user specified output directory if given on command line */
/* following code added 1993/Dec/12 */ /* separated 1996/Jan/20 */
  if (prepend_path_if (name_of_file+1, name_of_file+1, ".dvi", (unsigned char *) dvi_directory) ||
      prepend_path_if (name_of_file+1, name_of_file+1, ".log", (unsigned char *) log_directory) ||
      prepend_path_if (name_of_file+1, name_of_file+1, ".aux", (unsigned char *) aux_directory))
  {
    if (open_trace_flag)
    {
      sprintf(log_line, "After prepend %s\n", name_of_file+1);
      show_line(log_line, 0);
    }
  }
#endif

/* name_length recomputed below so don't need to do it yet */

  if (open_trace_flag)
  {
    sprintf(log_line, " Open `%s' for output ", name_of_file + 1); /* C string */
    show_line(log_line, 0);
  }   // debugging only

/* Is the filename openable as given?  */

/*  if share_flag is non-zero and we are opening for reading use fsopen */
/*  but we can assume this is opening here for *output* */
  *f = fopen((char *) name_of_file + 1, fopen_mode);

/* Can't open as given.  Try the envvar.  */
  if (*f == NULL)
  {
/*    string temp_dir = getenv ("TEXMFOUTPUT"); */  /* 93/Nov/20 */
/*    string temp_dir = getenv ("TEXMFOUT"); */ /* 93/Nov/20 */
/*      string temp_dir = get_env_shroud ("UFYNGPVUQVU"); */
    string temp_dir = get_env_shroud ("UFYNGPVU");

/*    if (deslash) unixify(temp_dir); */    /* deslashify 93/Dec/28 */

    if (temp_dir != NULL)
    {
#ifdef BUILDNAMEDIRECT
      unsigned char temp_name[PATH_MAX];
      xconcat3((char *) temp_name, temp_dir, PATH_SEP_STRING, (char *)name_of_file + 1);
#else
/*          string temp_name = concat3 (temp_dir, "/", name_of_file + 1); */
      string temp_name = concat3 (temp_dir, PATH_SEP_STRING, name_of_file + 1);
#endif
      if (deslash) unixify((char *) temp_name);     /* deslashify 93/Dec/28 */
/*  If share_flag is non-zero and we are opening for reading use fsopen */
/*  but we can assume this is opening here for *output* */
      *f = fopen((char*)temp_name, fopen_mode);
/*  If this succeeded, change name_of_file accordingly.  */
      if (*f) strcpy((char*) name_of_file + 1, (char *) temp_name);
#ifndef BUILDNAMEDIRECT
      free (temp_name);
#endif
    }
  }

//  show_line(name_of_file+1, 1);   // debugging only
//  New code to remember complete dvi_file name and log_file_name
//  To remember for output at the end 2000 June 18
  if (strstr((char *)name_of_file + 1, ".dvi") != NULL)
  {
    if (qualified(name_of_file+1))
      *log_line = '\0';
    else
    {
      (void) _getcwd(log_line, sizeof(log_line));
      strcat(log_line, PATH_SEP_STRING);
    }
    strcat(log_line, (char*) name_of_file+1);
    unixify(log_line);
    dvi_file_name = xstrdup(log_line);
//    show_line(dvi_file_name, 1);  // debugging only
  }
  else if (strstr((char *)name_of_file + 1, ".log") != NULL)
  {
    if (qualified(name_of_file+1))
      *log_line = '\0';
    else
    {
      (void) _getcwd(log_line, sizeof(log_line));
      strcat(log_line, PATH_SEP_STRING);
    }
    strcat(log_line, (char *) name_of_file + 1);
    unixify(log_line);
    log_file_name = xstrdup(log_line);
//    show_line(log_file_name, 1);  // debugging only
  }
/* Back into a Pascal string, but first get its length.  */
  temp_length = strlen ((char *)name_of_file + 1);
/*  space_terminate (name_of_file + 1); */
  name_of_file[temp_length+1] = ' ';  /* space terminate */

/* Only set `name_length' if we succeeded.  I'm not sure why.  */
  if (*f)               /* TEST ? 94/MAR/2 */
    name_length = temp_length;
  
  return *f != NULL;
}

/* #pragma optimize ("g",)*/  /* try and avoid compiler bug here */
/* #pragma optimize ("", on) */   /* try and avoid compiler bug here */
