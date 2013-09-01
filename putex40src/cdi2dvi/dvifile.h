/* dvifile.h: handling DVI output file

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

#ifndef DVIFILE_H_
#define DVIFILE_H_

#include <fstream>
#include <string>
#include "types.h"
#include "cmd.h"
#include "texofstream.h"

using namespace std;

const dvi_byte DVI_ID = 2;
const dvi_byte DVI_SIGNATURE = 223;

class DviFile : public texofstream {
//	friend void convert(CdiFile&, DviFile&);
public:
	DviFile () : texofstream() {}
	const string& fileName() const
		{ return _fName; }

	void putCmd (dvi_byte b)
		{ put(b); }

	void putId ()
		{ putByte(DVI_ID); }

	void putSigature ()
		{ putByte(DVI_SIGNATURE); }

	inline void putFontNum (dvi_uint);

	inline void putFontDef (font_def& fd);

	inline void putEFontInfo (efont_info *efnt);
private:
	string _fName;
};

inline void DviFile::putFontNum (dvi_uint fn)
{
	if(fn < 64)
		putByte(fn+fnt_num_0);
	else if (fn < 256) {
		putCmd(fnt1);
		putByte(fn);
	}
	else if  (fn <= 0XFFFF) {
		putCmd(fnt2);
		putPair(fn);
	}
	else if  (fn <= 0XFFFFFF) {
		putCmd(fnt3);
		putTrio(fn);
	}
	else {
		putCmd(fnt4);
		putQuad(fn);
	}
}

inline void DviFile::putFontDef (font_def& fd)
{
	if (fd.fontNum < 256) {
		putCmd(fnt_def1);
		putByte(fd.fontNum);
	}
	else if (fd.fontNum <= 0XFFFF) {
		putCmd(fnt_def2);
		putPair(fd.fontNum);
	}
	else if (fd.fontNum <= 0XFFFFFF) {
		putCmd(fnt_def3);
		putTrio(fd.fontNum);
	}
	else {
		putCmd(fnt_def4);
		putQuad(fd.fontNum);
	}
	putQuad(fd.checksum);
	putQuad(fd.size);
	putQuad(fd.dsize);
	putByte(fd.area.size());
	putByte(fd.fName.size());
	putString(fd.area);
	putString(fd.fName);
}

inline void DviFile::putEFontInfo (efont_info *efnt)
{
	if (efnt && efnt->size > 0)
		write(efnt->def, efnt->size);
}

#endif