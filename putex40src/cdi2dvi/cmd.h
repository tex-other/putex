#ifndef CMD_H_
#define CMD_H_

#include "types.h"

const dvi_cmd	set_char_0 = 0;
const dvi_cmd	set_char_1 = 1;
const dvi_cmd	set1 = 128;
const dvi_cmd	set2 = 129;
const dvi_cmd	set3 = 130;
const dvi_cmd	set4 = 131;
const dvi_cmd	set_rule = 132;
const dvi_cmd	put1 = 133;
const dvi_cmd	put2 = 134;
const dvi_cmd	put3 = 135;
const dvi_cmd	put4 = 136;
const dvi_cmd	put_rule = 137;
const dvi_cmd	nop	= 138;
const dvi_cmd	bop	= 139;
const dvi_cmd	eop	= 140;
const dvi_cmd	push = 141;
const dvi_cmd	pop	= 142;
const dvi_cmd	right1 = 143;
const dvi_cmd	right2 = 144;
const dvi_cmd	right3 = 145;
const dvi_cmd	right4 = 146;
const dvi_cmd	w0 = 147;
const dvi_cmd	w1 = 148;
const dvi_cmd	w2 = 149;
const dvi_cmd	w3 = 150;
const dvi_cmd	w4 = 151;
const dvi_cmd	x0 = 152;
const dvi_cmd	x1 = 153;
const dvi_cmd	x2 = 154;
const dvi_cmd	x3 = 155;
const dvi_cmd	x4 = 156;
const dvi_cmd	down1 = 157;
const dvi_cmd	down2 = 158;
const dvi_cmd	down3 = 159;
const dvi_cmd	down4 = 160;
const dvi_cmd	y0 = 161;
const dvi_cmd	y1 = 162;
const dvi_cmd	y2 = 163;
const dvi_cmd	y3 = 164;
const dvi_cmd	y4 = 165;
const dvi_cmd	z0 = 166;
const dvi_cmd	z1 = 167;
const dvi_cmd	z2 = 168;
const dvi_cmd	z3 = 169;
const dvi_cmd	z4 = 170;
const dvi_cmd	fnt_num_0 = 171;
// fnt_num_1 .. fnt_num_63
const dvi_cmd	fnt1 = 235;
const dvi_cmd	fnt2 = 236;
const dvi_cmd	fnt3 = 237;
const dvi_cmd	fnt4 = 238;
const dvi_cmd	xxx1 = 239;
const dvi_cmd	xxx2 = 240;
const dvi_cmd	xxx3 = 241;
const dvi_cmd	xxx4 = 242;
const dvi_cmd	fnt_def1 = 243;
const dvi_cmd	fnt_def2 = 244;
const dvi_cmd	fnt_def3 = 245;
const dvi_cmd	fnt_def4 = 246;
const dvi_cmd	pre	= 247;
const dvi_cmd	post = 248;
const dvi_cmd	post_post = 249;

// cdi commands					// argvs :
const dvi_cmd cfnt = 250;		// k[2]
const dvi_cmd cfnt_def = 251; 	// k[2] c[1] a[1] n[a] size[4] dsize[4] weight[2] style[1]
				// width[4] height[4] depth[4]
  				// fw_width[4] fw_height[4] fw_depth[4].
#define CMD_GROUP(x)	(-(x)+1)

extern int cmd_type[256];
extern void init_cmd_type ();

class CCdiCmd {
public:
	CCdiCmd () {}
	// void init ();
	CCdiCmd (dvi_cmd cmd) { set(cmd); }
	void set (dvi_cmd cmd) { _cmd = cmd; }
	dvi_cmd name () { return _cmd; }
	bool operator== (dvi_cmd cmd) { return _cmd == cmd; }
	bool operator!= (dvi_cmd cmd) { return _cmd != cmd; }
	bool operator< (dvi_cmd cmd) { return _cmd < cmd; }
	bool is_verbose () { return cmd_type[_cmd] >= 0; }
	int type() { return cmd_type[_cmd]; }
	int para_len () { return cmd_type[_cmd]; }
	int first_para_len () { return _cmd + cmd_type[_cmd]; }
	inline bool is_set_char ()
	{
		int t = type();
		return (0 <= _cmd && _cmd < set2) || (t == CMD_GROUP(put1));
	}
	inline bool is_fntk ()
	{
		return type() == CMD_GROUP(fnt1);
	}
	inline bool is_fnt_num ()
	{
		return type() == -fnt_num_0;
	}
	inline bool is_def_font ()
	{
		return type() == CMD_GROUP(fnt_def1);
	}
private:
	dvi_cmd _cmd;
};


#endif
