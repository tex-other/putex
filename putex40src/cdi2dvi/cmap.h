#ifndef CMAP_H_
#define CMAP_H_

#include <memory.h>
#include "types.h"
#include "fontused.h"
#include "cfontdef.h"

extern int cur_cfont;

struct cfont_code_tbl {
	cfont_code_tbl () : used(0) {}

	bool is_full () const { return used == 256; }

	bool is_empty () const { return used == 0; }

	void reset () { used = 0; }

	inline bool add_code (dbcode c, dvi_uint &fn, dvi_byte &b);

	const char* get_font_name () { return font_name.c_str(); }

	dbcode code[257];
	int used;
	dvi_font_num codeFont;
	string font_name;
};

inline bool cfont_code_tbl::add_code (dbcode c, dvi_uint &fn, dvi_byte &b)
{
	bool is_new_font = false;
	if (is_empty()) {
		codeFont = font_used.get_avail();
		char buf[257];
		sprintf(buf, "cdi%dx%d", codeFont, cfontTable.get_dsize(cur_cfont) >> 16);
		font_name = buf;
		is_new_font = true;
	}
	fn = codeFont;
	b = used;
	code[used++] = c;
	return is_new_font;
}

class FontMapTable {
public:
	FontMapTable () : cfMapTbl(0), size(0) {}
	~FontMapTable ();
	void init (CJKfontDefTable& cfontTable);
	bool add_code (dvi_uint cfont_num, dbcode code, dvi_uint &font_num, dvi_byte &byte_code);
	void ship_out (dvi_uint cfont_num);
	void ship_out_all ();
	cfont_code_tbl* operator[] (int k) const { return cfMapTbl[k]; }
private:
	cfont_code_tbl **cfMapTbl;
	int size;
};

struct map_node {
	map_node () : next(0) {}

	map_node (dvi_uint cn) : cfont_num(cn) {}

	void set (dvi_uint cf, dvi_uint f, dvi_byte c, map_node *p)
	{
		cfont_num = cf; font_num = f; byte_code = c; next = p;
	}

	dvi_uint cfont_num;
	dvi_uint font_num;
	dvi_byte byte_code;
	map_node *next;
};


class CJKCharMapping {
public:
	CJKCharMapping () { }
	~CJKCharMapping ();

	void init (CJKfontDefTable& cfontTable);

	// lookup function converts a double-byte code to a single-byte TeX code.
	//     cfontNum:	font number of the double-byte code
	//     code:		double-byte code
	//     fontNum:		return the converted font number
	//     byteCode:	return the converted  single-byte TeX code
	// The function returns true if the fontNum is a new font number; otherwise, it returns false.

	bool lookup (dvi_pair cfont_num, dbcode code, dvi_font_num& font_num, dvi_byte& byte_code);

	void new_font_def (dvi_font_num fontNum, dvi_pair cfont_num, font_def& fd);

	const char* get_mapped_font_name ();

	void clean_up () { cfont_map_tbl.ship_out_all (); }
private:
	map_node** map;
	int map_size;
	FontMapTable cfont_map_tbl;
};




#endif
