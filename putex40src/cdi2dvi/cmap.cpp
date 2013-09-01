/* cmap.cpp: Map CJK char/font to Tex char/font

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

#include <iostream>
#include "exception.h"
#include "cmap.h"
#include "tfmfile.h"
#include "pkfile.h"
#include "cdifile.h"
#include "vcencoding.h"

using namespace std;

//
// FontMapTable PUBLIC MEMBER FUNCTIONS
//

FontMapTable::~FontMapTable ()
{
	for (int i = 0; i < size; i++)
		delete cfMapTbl[i];
	delete cfMapTbl;
	size = 0;
}

void FontMapTable::init (CJKfontDefTable& cfontDefTbl)
{
	size = cfontDefTbl.get_max_cfont_num()+1;
	cfMapTbl = new cfont_code_tbl * [size];
	if (!cfMapTbl)
		throw(EMemErr("cfMapTbl (FontMapTable::init)"));

	for (int i = 0; i < size; i++)
		if (cfontDefTbl.isset(i)) {
			cfont_code_tbl *cct = new cfont_code_tbl;
			if (!cct)
				throw(EMemErr("cct (FontMapTable::init)"));
			cfMapTbl[i] = cct;
		}
		else
			cfMapTbl[i] = 0;
}

bool
FontMapTable::add_code (dvi_uint cfont_num, dbcode code, dvi_uint &font_num, dvi_byte &byte_code)
{
	if (!cfMapTbl[cfont_num])
		throw(EFatal("Undefined CJK font face"));
	cfont_code_tbl *cctp = cfMapTbl[cfont_num];
	if (cctp->is_full()) {
		ship_out(cfont_num);
		cctp->reset();
	}
	return cctp->add_code(code, font_num, byte_code);
}

void
FontMapTable::ship_out (dvi_uint cfont_num)
{
	extern string tfm_dir;

	cfont_code_tbl* cctp = cfMapTbl[cfont_num];

	// write tfm file
	TfmFile tfmFile;
	string font_name = cfMapTbl[cfont_num]->get_font_name();
	tfmFile.open(tfm_dir, font_name);
	tfmFile.makeit (0, cctp->used-1, cfontTable[cfont_num]->dsize, cfontTable[cfont_num]->fw_width,
		            cfontTable[cfont_num]->fw_height, cfontTable[cfont_num]->fw_depth);

	// write PK font file
	cctp->code[cctp->used].set_null();
	PKFile pk_file;

	extern dev_info dev;
	extern CdiFile cdi;

	pk_file.open(dev, *cfontTable[cfont_num], font_name);
	pk_file.output(dev, *cfontTable[cfont_num], cctp->code);
	pk_file.close();
}

void
FontMapTable::ship_out_all ()
{
	for (int i = 0; i < size; i++)
		if (cfMapTbl[i] && !cfMapTbl[i]->is_empty())
			ship_out(i);
}

//
// CJKCharMapping PUBLIC MEMBER FUNCTIONS
//

void CJKCharMapping::init (CJKfontDefTable& cfontTable)
{
	map_size = 65536;
	map = new map_node*[map_size];
	if (!map)
		throw(EMemErr("map (CJKCharMapping::init)"));

	memset(map, '\0', map_size*sizeof(map_node *));

	cfont_map_tbl.init(cfontTable);
}


CJKCharMapping::~CJKCharMapping ()
{
	for (int i = 0; i < map_size; i++) {
		map_node *nd = map[i];
		while (nd) {
			map_node *t = nd;
			nd = nd->next;
			delete t;
		}
	}
}

bool CJKCharMapping::lookup (dvi_pair cfont_num, dbcode code, dvi_font_num& font_num, dvi_byte& byte_code)
{
	int k;

	k = cur_charset->check_code(code);;

	map_node *nd = map[k];
	while (nd) {
		if (cfont_num == nd->cfont_num) {
			font_num = nd->font_num;
			byte_code = nd->byte_code;
			return false;
		}
		nd = nd->next;
	}

	// insert a new <cfont_num, code> pair
	nd = new map_node(cfont_num);
	if (!nd)
		throw(EMemErr("nd (CJKCharMapping::lookup)"));

	bool is_new_font = cfont_map_tbl.add_code(cfont_num, code, font_num, byte_code);

	// insert to the map[k] list
	nd->set(cfont_num, font_num, byte_code, map[k]);
	map[k] = nd;
	return is_new_font;
}

void CJKCharMapping::new_font_def  (dvi_font_num fontNum, dvi_pair cfont_num, font_def& fd)
{
	fd.fontNum = fontNum;
	fd.checksum = 0;
	fd.size = cfontTable[cfont_num]->size;
	fd.dsize = cfontTable[cfont_num]->dsize;
	fd.fName = get_mapped_font_name();
}


const char* CJKCharMapping::get_mapped_font_name ()
{
	return cfont_map_tbl[cur_cfont]->get_font_name();
}

