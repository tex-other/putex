#include <iostream>
#include <list>
#include "types.h"
#include "exception.h"
#include "cfontdef.h"
#include "cdifile.h"
#include "dvifile.h"
#include "cmd.h"
#include "cdi2dvi.h"
#include "cmap.h"
#include "vcencoding.h"

const dvi_uint invalid_font = 0XFFFFFFFF;

static const int buf_size = 256;
static char buf[buf_size];
static list<font_def> new_font_list;


int cur_font = invalid_font, cur_cfont = invalid_font, cur_efont = invalid_font;

// copy n (<= buf_size) bytes from cdi to dvi.
inline void copy (CdiFile& cdi, DviFile& dvi, int n)
{
	if (n > 0) {
		cdi.read(buf, n);
		dvi.write(buf, n);
	}
}

// copy n (> buf_size) bytes from cdi to dvi.
static void slow_copy (CdiFile& cdi, DviFile& dvi, dvi_uint n)
{
	while (n > buf_size) {
		cdi.read(buf, buf_size);
		dvi.write(buf, buf_size);
		n -= buf_size;
	}
	if (n != 0) copy(cdi, dvi, n);
}

// copy 1 byte from cdi to dvi and return this byte.
inline dvi_byte copy (CdiFile& cdi, DviFile& dvi)
{
	dvi_byte b = cdi.readByte();
	dvi.putByte(b);
	return b;
}

// copy k[1] x[k] from cdi to dvi.
inline void k1copy (CdiFile& cdi, DviFile& dvi)
{
	dvi_byte b = cdi.readByte();
	dvi.put(b);
	copy(cdi, dvi, b);
}

// 
inline void copy_efont_def (CCdiCmd &cmd, CdiFile& cdi, DviFile& dvi, bool check_defined = false)
{
	extern FontUsedTbl font_used;

	int k = cmd.first_para_len();
	dvi_uint font_num = cdi.readPara(k);

	if (check_defined) {
		if (font_used.is_efont_defined(font_num))
			dvi.putEFontInfo(font_used.get_efont_info(font_num));
	}
	else {
		dvi.putEFontInfo(font_used.get_efont_info(font_num));
		font_used.efont_defined(font_num);
	}
	cdi.skip(font_used.get_efont_info_size(font_num)-k-1);
}

inline dvi_uint get_first_para (int n, CdiFile& cdi, DviFile& dvi, char *buf)
{
	if (n <= 0) throw(EUnknown(__LINE__, __FILE__));

	dvi_byte b = cdi.readByte();
	buf[0] = b;
	dvi_uint k = b;
	n--;
	if (n == 0) return k;

	b = cdi.readByte();
	buf[1] = b;
	k = (k << 8) + b;
	n--;
	if (n == 0) return k;

	b = cdi.readByte();
	buf[2] = b;
	k = (k << 8) + b;
	n--;
	if (n == 0) return k;

	b = cdi.readByte();
	buf[3] = b;
	k = (k << 8) + b;
	n--;
	if (n == 0) return k;
	else throw(EUnknown(__LINE__, __FILE__));
}

inline dvi_uint copy_first_para (CCdiCmd &cmd, CdiFile& cdi, DviFile& dvi)
{
	int n = cmd.first_para_len();
	char buf[4];

	dvi_uint v = get_first_para (n, cdi, dvi, buf);
	dvi.write(buf, n);
	return v;
}

inline dvi_byte map_dbchar (CdiFile& cdi, DviFile& dvi)
{
	extern CJKCharMapping cjkCharMapper;

	dvi_font_num fontNum;
	dvi_byte byteCode;
	dvi_byte hi = cdi.readByte(), lo = cdi.readByte();
	dbcode code(hi, lo);

	if (cjkCharMapper.lookup(cur_cfont, code, fontNum, byteCode)) { // a new TeX font
		font_def fd;
		cjkCharMapper.new_font_def(fontNum, cur_cfont, fd);
		dvi.putFontDef(fd);
		new_font_list.push_back(fd); 
	}

	if (fontNum != cur_font) {
		dvi.putFontNum(fontNum);
		cur_font = fontNum;
	}
	return byteCode;
}

void convert (CdiFile& cdi, DviFile& dvi)
{
	extern FontUsedTbl font_used;
	
	int page_count = 0;

	CCdiCmd cmd;
	cdi.reset();
	long last_bop = -1;

	// process the preamble
	// pre i[1] (e[1]) num[4] den[4] mag[4] k[1] x[k]
	cmd.set(cdi.getCmd());
	dvi.putCmd(cmd.name());
	cdi.getCmd();	// DVI ID
	dvi.putId();
	if (cdi._id == CDI_ID)
		cdi.skip(1);
	copy(cdi, dvi, 12);
	k1copy(cdi, dvi);
	
	// dvi command processing loop
	bool not_reach_post = true;
	while (not_reach_post) {
		cmd.set(cdi.getCmd());
		if (cmd == set2) {
			dvi_byte byte_code = map_dbchar(cdi, dvi);
			if (byte_code < set1)
				dvi.putCmd(byte_code);
			else {
				dvi.putCmd(set1);
				dvi.putByte(byte_code);
			}
		}
		else if (cmd.is_verbose()) {
			dvi.putCmd(cmd.name());
			copy(cdi, dvi, cmd.para_len());
		}
		else if (cmd == put2) {
			dvi_byte byte_code = map_dbchar(cdi, dvi);
			dvi.putCmd(put1);
			dvi.putByte(byte_code);
		}
		else if (cmd.is_set_char()) {
			if (cur_font != cur_efont) {
				dvi.putFontNum(cur_efont);
				cur_font = cur_efont;
			}
			dvi.putCmd(cmd.name());
			if (cmd < set1)
				continue;
			else
				copy(cdi, dvi, cmd.first_para_len());
		}
		else if (cmd.is_fnt_num()) {
			cur_efont = cmd.name() - fnt_num_0;
			if (!font_used.is_efont_defined(cur_efont)) {
				dvi.putEFontInfo(font_used.get_efont_info(cur_efont));
				font_used.efont_defined(cur_efont);
			}
			dvi.putCmd(cmd.name());
		}
		else if (cmd.is_fntk()) {
			int n = cmd.first_para_len();
			char buf[4];
			cur_efont = get_first_para (n, cdi, dvi, buf);
			if (!font_used.is_efont_defined(cur_efont)) {
				dvi.putEFontInfo(font_used.get_efont_info(cur_efont));
				font_used.efont_defined(cur_efont);
			}
			dvi.putCmd(cmd.name());
			dvi.write(buf, n);
		}
		else {
			switch (-cmd.type()) {
				case cfnt:
					{
						cur_cfont = cdi.readPair();
						dvi_byte cs = cfontTable.get_charset(cur_cfont);
						cur_charset = cur_charset->switchTo(cs);
					}
					break;
				case cfnt_def:
					{
						int cfn = cdi.readPair();
						// +++ check if it is also defined in post area

						// skip the other bytes
						int k = cdi.readByte();
						cdi.skip(k+36);
					}
					break;
				case bop:
					if (cdi.next_output_page()) {
						int page_num = cdi.readQuad();
						page_count++;
						cout << "[" << page_num << "] ";
						if (page_count % 15 == 0) cout << "\n";
						
						long cur_pos = dvi.tellp();
						dvi.putCmd(bop);
						dvi.putInt(page_num);
						copy(cdi, dvi, 36);
						cdi.skip(4);
						dvi.putInt(last_bop);
						last_bop = cur_pos;
						
						cur_font = invalid_font;
					}
					break;
				case -CMD_GROUP(xxx1):
					{
						dvi.putCmd(cmd.name());
						dvi_uint k = copy_first_para (cmd, cdi, dvi);
						if (k < buf_size)
							copy(cdi, dvi, k);
						else
							slow_copy(cdi, dvi, k);
					}
					break;
				case -CMD_GROUP(fnt_def1):
					copy_efont_def(cmd, cdi, dvi);
					break;
				case post:
					dvi.putCmd(post);
					not_reach_post = false;
					break;
				default:
					throw(EBadCdiFormat("illegal command' command"));
			}
		}
	}

	// post
	long post_pos = long(dvi.tellp()) - 1;
	dvi.putInt(last_bop);
	cdi.skip(4);
	copy(cdi, dvi, 24);

	// copy TeX font definitions
	cmd.set(cdi.getCmd());
	while (cmd.is_def_font()) {
		copy_efont_def(cmd, cdi, dvi, true);
		cmd.set(cdi.getCmd());
	}

	// write converted CJK font definitions
	list<font_def>::iterator fdp;
	for (fdp = new_font_list.begin(); fdp != new_font_list.end(); fdp++)
		dvi.putFontDef(*fdp);

	// post-post
	dvi.putCmd(post_post);
	dvi.putInt(post_pos);
	dvi.putId();
	long n = dvi.tellp();
	n = ((n % 4) == 0) ? 4 : 8 - n % 4;
	for (int i = 0; i < n; i++)
		dvi.putSigature();
}

