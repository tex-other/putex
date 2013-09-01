#include <iostream>
#include <limits.h>
#include <string.h>
#include "exception.h"
#include "types.h"
#include "cmd.h"
#include "cdifile.h"
#include "fontused.h"
#include "cfontdef.h"

// ==== PUBLIC MEMBER FUNCTIONS
void
CdiFile::open (const char* fName, const string& page_rng)
{
	ifstream::open(fName, ios::binary);
	if(bad())
		throw (EBadFile("Fail to open", fName));
	_fName = fName;
	chkfmt();
	select_pages (page_rng);
	read_font_info();

	extern dev_info dev;
	dev.scale_x = _mag / 1000.0 * dev.dpi_x / 72.27 / 65536.0;
	dev.scale_y = _mag / 1000.0 * dev.dpi_y / 72.27 / 65536.0;
	dev.dpi_mag = double(_mag) * dev.dpi_x / 1000.0;
}


// ==== PRIVATE MEMBER FUNCTIONS
void
CdiFile::chkfmt ()
{
	// check the preamble
	if (getCmd() != pre)
		throw(EBadCdiFormat("missing 'pre' command"));

	_id = readByte();
	if (_id != CDI_ID)
		throw(EBadCdiFormat("illegal id number"));

	_charset = BIG5;
	_charset = readByte();
	if (_charset >= Unknown_Enc)
		throw(EBadCdiFormat("Unknown charset"));

	_num = readQuad();
	_den = readQuad();
	_mag = readQuad();


	// check the postamble
	int count = 0;
	seekg(-1,ios_base::end);
	while (peek() == DVI_SIGATURE)		// find the post_post command position
	{
		seekg(-1,ios_base::cur);
		count++;
	}
	if (count < 4 || eof()) // should have at least four '233'
		throw (EBadCdiFormat("illegal tail sigature"));

	if (peek() != _id)
		throw(EBadCdiFormat("unmatched id number"));

	seekg(-6,ios_base::cur);

	if (peek() != post_post)
		throw(EBadCdiFormat("missing 'post_post' command"));

	seekg(1,ios_base::cur);
	_post_position = readQuad();
}

void CdiFile::read_font_info ()
{
	seekg(_post_position, ios_base::beg);
	if (getCmd() != post)
		throw(EBadCdiFormat("missing 'post' command"));
	skip(28);
	CCdiCmd cmd(getCmd());
	while (cmd.is_def_font()) {
		dvi_uint font_num;
		char buf[300];
		char *p = buf;

		// copy font definition to buf
		*p++ = cmd.name();
		switch (cmd.name()) {
			case fnt_def1:
				font_num = readByte(p);
				p++;
				break;
			case fnt_def2:
				font_num = readPair(p);
				p += 2;
				break;
			case fnt_def3:
				font_num = readTrio(p);
				p += 3;
				break;
			case fnt_def4:
				font_num = readQuad(p);
				p += 4;
				break;
		}
		read(p, 12); p += 12;
		*p++ = readByte();
		*p++ = readByte();
		int k = p[-2] + p[-1];
		read(p, k); p += k;
		// *p = '\0';

		font_used.set(font_num);
		font_used.save_efont_def(font_num, buf, p-buf);

		cmd.set(getCmd());
	}

	while (cmd.name() == cfnt_def) {
		cfontTable.insert();
		cmd.set(getCmd());
	}

	if (cmd.name() != post_post)
		throw(EBadCdiFormat("missing 'post_post' command"));
}


void CdiFile::select_pages (const string& page_rng)
{
	if (page_rng.empty()) {
		next_page = 0;
		return;
	}

	page_info pg;
	int next_page_num, last_page_num, cur_blk_num = 0;

	seekg(_post_position+1, ios_base::beg);
	dvi_uint prev_bop = readQuad();
	seekg(prev_bop+1, ios_base::beg);
	next_page_num = readQuad();
	pg.set(next_page_num, cur_blk_num, prev_bop);
	page_list.push_front(pg);
	seekg(36, ios_base::cur);
	prev_bop = readQuad();
	while (prev_bop != 0XFFFFFFFF) {
		last_page_num = next_page_num;
		seekg(prev_bop+1, ios_base::beg);
		next_page_num = readQuad();
		if (next_page_num > last_page_num)
			cur_blk_num++;
		pg.set(next_page_num, cur_blk_num, prev_bop);
		page_list.push_front(pg);
		seekg(36, ios_base::cur);
		prev_bop = readQuad();
	}
	if (cur_blk_num == 0) cur_blk_num++;
	list<page_info>::iterator p;
	for (p = page_list.begin(); p != page_list.end(); p++)
		p->blk_num = cur_blk_num - p->blk_num;

	CPageRangeParser pg_rgn_parser(page_rng, 1);
	pg_rgn_parser.select_pages(page_list, cur_blk_num);

	next_page = page_list.begin();

#if 0
	for (p = page_list.begin(); p != page_list.end(); p++) {
		if (p->out)
			cerr << p->blk_num << ' ' << p->page_num << endl;
	}
#endif
}

bool CdiFile::next_output_page ()
{
	if (is_select_page()) {
		list<page_info>::iterator p = next_page;
		while (p != page_list.end()) {
			if (p->out) break;
			++p;
		}
		if (p == page_list.end()) {
			seekg(_post_position, ios_base::beg); // go to post section
			return false;
		}
		if (p != next_page) {
			next_page = p; ++next_page;
			seekg(p->position, ios_base::beg);
			dvi_byte cmd = readByte();
			if (cmd != bop)
				throw(EBadCdiFormat("missing bop command"));
		}
	}
	return true;
}

CPageRangeParser::CPageRangeParser (const string& page_rng, int blk_num)
	: default_blk_num (blk_num)
{
	rngStr = new char[page_rng.size()+1];
	if (!rngStr)
		throw(EMemErr("rngStr (CPageRangeParser::CPageRangeParser)"));
	strcpy(rngStr, page_rng.c_str());
	last_p = cur_p = rngStr;
}

void CPageRangeParser::select_pages (list<page_info> &page_list, int max_blk)
{
	list<page_info>::iterator p;
	int blk1, blk2, pn1, pn2;
	bool go = true;
	while (go) {
		switch (get_next_range(blk1, pn1, blk2, pn2)) {
		case ODD:
			for (p = page_list.begin(); p != page_list.end(); p++)
				if (p->page_num % 2 == 1)
					p->out = true;
			break;
		case EVEN:
			for (p = page_list.begin(); p != page_list.end(); p++)
				if (p->page_num % 2 == 0)
					p->out = true;
			break;
		case SINGLE:
			for (p = page_list.begin(); p != page_list.end(); p++)
				if (p->blk_num == blk1 && p->page_num == pn1) {
					p->out = true;
					break;
				}
			break;
		case RANGE:
			if (blk1 > blk2 || (blk1 == blk2 && pn1 > pn2) || blk1 > max_blk) {
				cerr << "Illegal page output range: " << blk1 << ':' << pn1 << '-' << blk2 << ':' << pn2 << endl;
				continue;
			}
			
			for (p = page_list.begin(); p != page_list.end() && blk1 > p->blk_num; p++)
				;
			if (blk1 == p->blk_num) {
				while (p != page_list.end() && p->page_num < pn1)
					p++;
			}
			
			while (p != page_list.end() && p->blk_num < blk2) {
				p->out = true;
				p++;
			}
			
			if (blk2 == p->blk_num) {
				while (p != page_list.end() && p->page_num <= pn2) {
					p->out = true;
					p++;
				}
			}
			break;
		case ',': break;
		default: go = false; break;
		}
	}
}


int CPageRangeParser::get_next_range (int &blk1, int &pn1, int &blk2, int &pn2)
{
	int tokType, tokVal;
	blk1 = default_blk_num;
	pn1 = 1;

	tokType = next_page_num (blk1, pn1);
	if (tokType == ODD || tokType == EVEN || tokType == EOS || tokType == ERR)
		return tokType;
	if (tokType == PGNUM) {
		tokType = next_tok(tokVal);
		if (tokType == ',' || tokType == '\0') {	// single page
			if (tokType == EOS)
				push_back();
			return SINGLE;
		}
		else if (tokType == '-') {
			blk2 = blk1; pn2 = pn1;
			if (next_page_num (blk2, pn2) != PGNUM) {// b1:p1-
				blk2 = blk1+1; pn2 = INT_MIN;
			}
			// b1:p1-b2:p2
		}
		else
			return ERR;
	}
	else if (tokType == '-') {
		blk2 = blk1;
		if (next_page_num (blk2, pn2) == PGNUM)	{	// -b2:p2							
				blk1 = blk2 - 1; pn1 = INT_MAX;
		}
		else
			return ERR;					// -
	}
	else 
		return ERR;

	return RANGE;
}

int CPageRangeParser::next_page_num (int& blk, int &pn)
{
	int tokType, tokVal1, tokVal2;

	tokType = next_tok(tokVal1);
	if (tokType != NUM)
		return tokType;

	tokType = next_tok(tokVal2);
	if (tokType == ':') {
		blk = tokVal1;
		tokType = next_tok(tokVal2);
		if (tokType != NUM) {
			cerr << "Missing page number: " << tokVal1 << ":???" << endl;
			cerr << "Treat as " << tokVal1 << ":" << pn << endl;
			push_back();
		}
		else {
			pn = tokVal2;
		}
	}
	else {
		pn = tokVal1;
		push_back();
	}
	return PGNUM;
}

int CPageRangeParser::next_tok (int& tokVal)
{
	while (isspace(*cur_p)) cur_p++;
	last_p = cur_p;
	if (isdigit(*cur_p)) {
		tokVal = 0;
		do {
			tokVal = tokVal * 10 + *cur_p - '0';
			cur_p++;
		} while (isdigit(*cur_p));
		return NUM;
	}
	int tokType;
	switch (*cur_p) {
		case 'o': case 'O': tokType = ODD; break;
		case 'e': case 'E': tokType = EVEN; break;
		case '-': case ':': case ',': tokType = *cur_p; break;
		case '\0': return EOS;
		default: tokType = ERR; break;
	}
	cur_p++;
	return tokType;
}