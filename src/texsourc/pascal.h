/* pascal.h: implement various bits of standard Pascal that we use.  */

/* Copyright 1992 Karl Berry
   Copyright 2007 TeX Users Group

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

#ifndef PASCAL_H
#define PASCAL_H

/* Absolute value.  Without the casts to integer here, the Ultrix and
   AIX compilers (at least) produce bad code (or maybe it's that I don't
   understand all the casting rules in C) for tests on memory fields. 
   Specifically, a test in diag_round (in Metafont) on a quarterword
   comes out differently without the cast, thus causing the trap test to
   fail.  (A path at line 86 is constructed slightly differently).  */
#define	abs(x)((integer)(x)>=0?(integer)(x):(integer)-(x))

/* above conflicts with math.h ... */

/* Other standard predefined routines.  */
#define	chr(x)		(x)
#define ord(x)		(x)
#define	odd(x)		((x) % 2)
#define	round(x)	zround ((double) (x))
#define trunc(x)	((integer) (x))

/* File routines.  */
#define	reset(f, n, mode)	((f) ? fclose (f) : 0), \
			 (f) = xfopen_pas ((char *) (n), "r" mode)
#define rewrite(f, n, mode)	(f) = xfopen_pas ((char *) (n), "w" mode)
#define flush(f)	(void) fflush (f)

/* flush - used only in itex.c and conflicts with io.h -- bkph 93/Nov/26 */

/* #define read(f, b)	((b) = getc (f)) */
/* #define	readln(f)	{ register c; \
                          while ((c = getc (f)) != '\n' && c != EOF); } */

/* We hope this will be efficient than the `x = x - 1' that decr would
   otherwise be translated to.  Likewise for incr.  */
#define	decr(x)		--(x)
#define	incr(x)		++(x)

/* PatGen 2 uses this.  */
#define	input2ints(a, b)  zinput2ints (&a, &b)

/* We need this routine only if TeX is being debugged.  */
#define	input3ints(a, b, c)  zinput3ints (&a, &b, &c)

/* Pascal has no address-of operator, and we need pointers to integers
   to set up the option table.  */
/* #define addressofint(x)	((int *) &(x)) */ /* not used */

/* Not all C libraries have fabs, so we'll roll our own.  */
#define	fabs(x)		((x) >= 0.0 ? (x) : -(x))

/* above conflicts with math.h ... */

/* The fixwrites program outputs this.  */
#define	Fputs(f, s)	(void) fputs (s, f)

/* Tangle removes underscores from names, but the `struct option'
   structure has a field name with an underscore.  So we have to put it
   back.  Ditto for the other names.  */
#define getoptlongonly	getopt_long_only
#define hasarg		has_arg

/* Same underscore problem.  */
#define PATHMAX		PATH_MAX

/* We need a new type for the argument parsing, too.  */
typedef struct option getoptstruct;

#define printreal(r, n, m)  fprintreal (stdout, r, n, m)
#define	putbyte(x, f)	putc ((char) (x) & 255, f)

#define	toint(x)	((integer) (x))

/* For throwing away input from the file F.  */
#define vgetc(f)	(void) getc (f)

/* If we don't care that strcpy(3) returns A.  Likewise for strcat.  */
#define vstrcpy(a, b)	(void) strcpy (a, b)
#define vstrcat(a, b)	(void) strcat (a, b)

/* Write out elements START through END of BUF to the file F.  */
/* could use some error checking --- but not used ... bkph */
#define writechunk(f, buf, start, end) \
  (void) fwrite (&buf[start], sizeof (buf[start]), end - start + 1, f)

/* Like fseek(3), but cast the arguments and ignore the return value.  */
#define checkedfseek(f, n, w)  (void) fseek(f, (long) n, (int) w)

/* For faking arrays.  */
typedef unsigned char *pointertobyte;
#define casttobytepointer(e) ((pointertobyte) e)

/* For some initializations of constant strings.  */
typedef char *ccharpointer;

/* `real' is used for noncritical floating-point stuff.  */
typedef double real;

/* C doesn't distinguish between text files and other files.  */
typedef FILE *text, *file_ptr, *alpha_file;

/* `a_open_in' is used both for input files and pool files, so it
   needs to know what path to use.  */
#define a_open_in(f, p)	open_input (&(f), p, FOPEN_R_MODE)
#define a_open_out(f)	open_output (&(f), FOPEN_W_MODE)

/* Closing files is even easier; we don't bother to check the return
   status from fclose(3).  */
/* #define a_close(f)	if (f) (void) fclose (f) */
/* At least for output, we SHOULD check error status !!! */
/* Ideally should check this DURING output from time to time */
/* #define a_close(f) if (f) { if (ferror (f)) {perror(""); exit(1);} } if (f) (void) fclose (f) */
/* #define a_close(f)	if (f) (void) checkfclose (f) */
#define a_close(f)	checkfclose (f)

#ifdef BibTeX
/* See bibtex.ch for why these are necessary.  */
extern FILE *standardinput;
extern FILE *standardoutput;
#endif

#endif /* not PASCAL_H */
