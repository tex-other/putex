/* texofstream.h: ofstream subclass for handling TeX output

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

#ifndef TEXOFSTREAM_H_
#define TEXOFSTREAM_H_

#include <fstream>
#include <string>
#include "types.h"

using namespace std;

class texofstream : public ofstream {
public:
	texofstream () : ofstream() {}
	inline void open (const string& fName);
	inline void putByte (dvi_byte b);
	inline void putPair (dvi_pair w);
	inline void putTrio (dvi_trio n);
	inline void putQuad (dvi_uint n);
	inline void putInt (dvi_int n);
	inline void putCString (const char *s, int len);
	inline void putString (string& s);
};

inline void texofstream::open (const string& fName)
{
	ofstream::open(fName.c_str(), out | binary);
}

inline void texofstream::putByte (dvi_byte b)
{
	put(b);
}

inline void texofstream::putPair (dvi_pair w)
{
	put(dvi_byte((w >> 8) & 0XFF)); put(dvi_byte(w & 0XFF));
}

inline void texofstream::putTrio (dvi_trio n)
{
	putByte((n >> 16) & 0XFF);
	putByte((n >> 8) & 0XFF);
	putByte(n & 0XFF);
}

inline void texofstream::putInt (dvi_int n)
{
	putByte((n >> 24) & 0XFF);
	putByte((n >> 16) & 0XFF);
	putByte((n >> 8) & 0XFF);
	putByte(n & 0XFF);
}

inline void texofstream::putQuad (dvi_uint n)
{
	putByte((n >> 24) & 0XFF);
	putByte((n >> 16) & 0XFF);
	putByte((n >> 8) & 0XFF);
	putByte(n & 0XFF);
}

inline void texofstream::putCString (const char *s, int len)
{
	if (len) write(s, len);
}

inline void texofstream::putString (string& s)
{
	putCString(s.c_str(), s.size());
}



#endif

