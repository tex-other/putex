/* lib.h: declarations for shared routines.

   Copyright 1992, 1993 Karl Berry
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

#ifndef LIB_H
#define LIB_H

//#include "types.h"
#include <kpathsea/types.h>

/* for original boolean */
typedef int bool;

#define FATAL_PERROR(s) do { perrormod (s); exit (errno); } while (0)

extern string concat (string s1, string s2);
extern string concat3 (string, string, string);
extern string concat4 (string, string, string, string);
extern string concat5 (string, string, string, string, string);
extern string xstrdup (string s);
extern bool same_file_p (string filename1, string filename2);
extern string find_suffix (string name);
extern string remove_suffix (string name);
extern string make_suffix (string s, string suffix);
extern string make_prefix (string stem_prefix, string name);
extern string extend_filename (string name, string suffix);
extern FILE *xfopen (string filename, string mode);
extern void xfclose (FILE *, string filename);
extern void xfseek (FILE *, long, int, string filename);
extern long xftell (FILE *, string filename);

extern address xmalloc (unsigned size);
extern address xrealloc (address old_address, unsigned new_size);
extern address xcalloc (unsigned nelem, unsigned elsize);

/* (Re)Allocate N items of type T using xmalloc/xrealloc.  */
#define XRETALLOC(addr, n, t) ((addr) = (t *) xrealloc (addr, (n) * sizeof(t)))
#endif /* not LIB_H */
