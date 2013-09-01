/* VCFontDrv.h: Font Driver virtual class

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

#ifndef VCFONTDRV_H_
#define VCFONTDRV_H_

#include "types.h"

struct bitmap_info {  
    int width;					// bitmap width in bits
    int height;					// bitmap height in bits
	int byteWidth;				// width of bitmap in bytes (must be even for Win32)
	int logByteWidth;			// logical byte width (= (width + 7) / 8)
	unsigned char *bits;		// bitmap pixel data
	unsigned char *bitsEnd;		// the bitmap pixel data
	// int min_x, max_x, min_y, max_y;		// glyph bounding box
};

// character metric in pixels
struct char_metric {
	int width;
	int height;
	int depth;
	int accent;
};

class VCFontDrv {
public:
	VCFontDrv () {}
	int char_accent() const { return charAccent; }
	virtual void choose_cfont (cfont_def&, char_metric&) = 0;
	virtual void release_cfont () = 0;
	virtual bitmap_info* getBitmap (dbcode c) = 0;
protected:
	int x_dpi, y_dpi;
	bitmap_info bi;
	int bgColor;
	int charWidth, charHeight, charDepth, charAccent;
	double x_resolution, y_resolution;
};

#endif