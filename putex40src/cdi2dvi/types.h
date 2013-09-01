#ifndef TYPES_H_
#define TYPES_H_

#include <string>

using namespace std;

typedef unsigned char dvi_byte;
typedef dvi_byte dvi_cmd;
typedef unsigned short dvi_pair;
typedef unsigned int dvi_trio;
typedef unsigned int dvi_quad;
typedef int dvi_int;
typedef dvi_quad dvi_font_num;
typedef unsigned int dvi_uint;
typedef unsigned int dvi_scaled;
typedef unsigned int dvi_fixedwd;

enum {UCS2 = 0, BIG5, GBK, Shift_JIS, KS_C_5601, Unknown_Enc};

struct dbcode {
	dbcode () {}

	dbcode (dvi_byte hi, dvi_byte lo) { set(hi, lo); }

	void set (dvi_byte hi, dvi_byte lo) { hibyte = hi; lobyte = lo; }

	void set (int value) { set(value/256, value%256); }

	int value () { return hibyte*256+lobyte; }

	bool is_null () { return hibyte == 0 && lobyte == 0; }

	void set_null () { hibyte = 0; lobyte = 0; }

	dvi_byte hibyte;
	dvi_byte lobyte;
};

struct cfont_def {
	string face_name;
	dvi_byte charset;
	dvi_pair weight;
	dvi_byte style;
	dvi_scaled size;
	dvi_scaled dsize;
	dvi_scaled width, height, depth;
	dvi_fixedwd fw_width, fw_height, fw_depth;
	int win_charset;	// windows charset value
};

struct font_def {
	dvi_font_num fontNum;
	dvi_quad checksum;
	dvi_scaled size;
	dvi_scaled dsize;
	string area;
	string fName;
};

struct efont_info {
	int size;
	char *def;
};

struct dev_info {
	string mode;
	int dpi_x;
	int dpi_y;
	double scale_x, scale_y;
	double dpi_mag;
};

#endif