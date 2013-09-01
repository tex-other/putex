/* fontused.h: handling DVI output file

   Copyright (C) 1997-2004 Chey-Woei Tsay <cwtsay@pu.edu.tw>

   This file is part of the cdi2dvi program.

   The cdi2dvi program is a free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The cdi2dvi program is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX Core Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.
*/

#pragma once

#ifndef FONTUSED_H_
#define FONTUSED_H_

#include <memory.h>
#include <string>
#include "types.h"
#include "exception.h"
#include "dvifile.h"

using std::string;

class FontUsedTbl {
public:
	FontUsedTbl () : max_used(0), next_slot(0), _size(0),
		             used(0), defined(0), efont_def(0) { }
	~FontUsedTbl ();

	void init (dvi_uint n);

	dvi_uint size() { return _size; }

	dvi_uint get_avail();

	inline void set(dvi_uint fn);

	inline void save_efont_def(dvi_uint fn, const char *buf, int size);

	bool is_efont_defined (dvi_uint fn) { return defined[fn]; }

	void efont_defined (dvi_uint fn) { defined[fn] = true; }

	efont_info *get_efont_info (dvi_uint fn) { return efont_def[fn]; }

	int get_efont_info_size (dvi_uint fn) { return efont_def[fn]->size; }

private:
	bool *used;
	bool *defined;
	efont_info **efont_def;
	dvi_uint _size;
	dvi_uint next_slot;
	dvi_uint max_used;
};

inline void FontUsedTbl::set(dvi_uint fn)
{
	if (fn > _size)
		throw(EFatal("Exceed font capacity"));
	used[fn] = true;
	if (fn > max_used) max_used = fn;
}

inline void FontUsedTbl::save_efont_def(dvi_uint fn, const char *buf, int size)
{
	if (efont_def[fn])
		throw(EFatal("Duplicated font definition"));
	efont_def[fn] = new efont_info;
	efont_def[fn]->size = size;
	efont_def[fn]->def = new char[size];
	memcpy(efont_def[fn]->def, buf, size);
}


extern FontUsedTbl font_used;

#endif
