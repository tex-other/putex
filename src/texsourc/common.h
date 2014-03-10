/* common.h: Definitions and declarations common both to the change
   files and to web2c itself.  This is included from config.h, which
   everyone includes. 

   Copyright 1992 Karl Berry
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

#ifndef COMMON_H
#define COMMON_H

#include "getopt.h"
#include "lib.h"
#include "ourpaths.h"
#include "pascal.h"
#include "types.h"


/* We never need the `link' system call, which is sometimes declared in
   <unistd.h>, but we do have lots of variables called `link' in the web
   sources.  */
#ifdef link
  #undef link
#endif
//#define link link_var


/* On VMS, `exit (1)' is success.  */
#ifndef EXIT_SUCCESS_CODE
  #ifdef VMS
    #define EXIT_SUCCESS_CODE 1
  #else
    #define EXIT_SUCCESS_CODE 0
  #endif
#endif /* not EXIT_SUCCESS_CODE */

/* Some features are only needed in certain programs.  */

#ifdef TeX
/* The type `glueratio' should be a floating point type which won't
   unnecessarily increase the size of the memory_word structure.  This is
   the basic requirement.  On most machines, if you're building a
   normal-sized TeX, then glueratio must probably meet the following
   restriction: sizeof(glueratio) <= sizeof(integer).  Usually, then,
   glueratio must be `float'.  But if you build a big TeX, you can (on
   most machines) and should make it `double' to avoid loss of precision
   and conversions to and from double during calculations.  (All this
   also goes for Metafont.)  Furthermore, if you have enough memory, it
   won't hurt to have this defined to be `double' for running the
   trip/trap tests.
   
   This type is set automatically to `float' by configure if a small TeX
   is built.  */
#ifndef GLUERATIO_TYPE
  #define GLUERATIO_TYPE double
#endif
  typedef GLUERATIO_TYPE glueratio;
#endif

/* Declarations for the routines we provide ourselves.  */

extern integer zround (double);			/* extern integer zround(); */

/* File routines.  */
extern bool test_eof (FILE * file);
extern bool eoln (FILE * file);
extern bool open_input (FILE **f, path_constant_type path_index, char *fopen_mode);
extern bool open_output (FILE **f, char *fopen_mode);
extern void set_paths (int path_bits);
extern int check_fclose (FILE * f);	/* in openinou.c 93/Nov/20 */

/* Argument handling, etc.  */
/* extern int argc; */
/* extern char **gargv; */
extern void argv(); 		/* only to prevent accidental use of argv[] ??? */
extern char *versionstring;
extern char *version;

#endif /* not COMMON_H */
