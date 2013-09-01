#ifndef CDIFILE_H_
#define CDIFILE_H_

#include <fstream>
#include <string>
#include <List>
#include "types.h"
#include "exception.h"

using namespace std;

// const dvi_byte OLD_CDI_ID = 2;  (no longer support)
const dvi_byte CDI_ID = 100;
const dvi_byte DVI_SIGATURE = 0XDF;	// 233

struct page_info {
	void set (int pn, int bn, int p, bool o = false)
	{
		page_num = pn; blk_num = bn; position = p; out = o;
	}
	int page_num, blk_num;
	dvi_uint position;
	bool out;
};

class DviFile;

class CdiFile : public ifstream {
	friend void convert(CdiFile&, DviFile&);
public:
	CdiFile () : ifstream() {}
	void open (const char* fName, const string& pagg_rng);
	bool next_output_page ();

	inline dvi_byte id () { return _id; }
	inline dvi_byte charset () { return _charset; }
	inline dvi_uint num () { return _num; }
	inline dvi_uint den () { return _den; }
	inline dvi_uint mag () { return _mag; }
	
	inline dvi_byte readByte ();
	inline dvi_uint readPair ();
	inline dvi_uint readTrio ();
	inline dvi_uint readQuad ();

	inline dvi_byte readByte (char *);
	inline dvi_uint readPair (char *);
	inline dvi_uint readTrio (char *);
	inline dvi_uint readQuad (char *);

	inline dvi_uint readPara (int n);

	inline void reset() { seekg(0,ios_base::beg); }
	inline dvi_byte getCmd() { return readByte(); }
	inline void skip (int nBytes) { seekg(nBytes,ios_base::cur); }
private:
	void chkfmt ();
	void select_pages (const string& pagg_rng);
	void read_font_info ();
	bool is_select_page () { return next_page != 0; }
	string _fName;
	dvi_byte _id, _charset;
	dvi_uint _post_position;
	dvi_uint _num;
	dvi_uint _den;
	dvi_uint _mag;
	list<page_info> page_list;
	list<page_info>::iterator next_page;
};


inline dvi_byte
CdiFile::readByte()
{
	dvi_byte b = get() & 0XFF;
	return b;
}


inline dvi_byte
CdiFile::readByte(char *buf)
{
	dvi_byte b = get() & 0XFF;
	buf[0] = b;
	return b;
}


inline dvi_uint
CdiFile::readPair()
{
	dvi_uint n = readByte();
	n = (n << 8) | readByte();
	return n;
}

inline dvi_uint
CdiFile::readPair(char *buf)
{
	buf[0] = readByte();
	buf[1] = readByte();

	dvi_uint n = buf[0];
	n = (n << 8) | buf[1];
	return n;
}



inline dvi_uint
CdiFile::readTrio()
{
	dvi_uint n = readByte();
	n = (n << 8) | readByte();
	n = (n << 8) | readByte();
	return n;
}

inline dvi_uint
CdiFile::readTrio(char *buf)
{
	buf[0] = readByte();
	buf[1] = readByte();
	buf[2] = readByte();

	dvi_uint n = buf[0];
	n = (n << 8) | buf[1];
	n = (n << 8) | buf[2];
	return n;
}

inline dvi_uint
CdiFile::readQuad()
{
	dvi_uint n = readByte();
	n = (n << 8) | readByte();
	n = (n << 8) | readByte();
	n = (n << 8) | readByte();
	return n;
}

inline dvi_uint
CdiFile::readQuad(char *buf)
{
	buf[0] = readByte();
	buf[1] = readByte();
	buf[2] = readByte();
	buf[4] = readByte();

	dvi_uint n = buf[0];
	n = (n << 8) | buf[1];
	n = (n << 8) | buf[2];
	n = (n << 8) | buf[3];
	return n;
}

inline dvi_uint CdiFile::readPara (int n)
{
	switch (n) {
	case 1: return readByte();
	case 2: return readPair();
	case 3: return readTrio();
	case 4: return readQuad();
	default:
		throw(EFatal("Illegal parameter length of dvi command"));
	}
	return 0;	// for suppressing waring message
}



class CPageRangeParser {
public:
	CPageRangeParser (const string& page_rng, int blk_num);
	~CPageRangeParser () { delete [] rngStr; }
	void select_pages (list<page_info> &page_list, int max_blk);
private:
	enum {EOS = 0, ODD, EVEN , SINGLE, RANGE, ERR, NON_NUM = -1, NUM = -2, PGNUM = -3};
	int get_next_range (int &blk1, int &pn1, int &blk2, int &pn2);
	int next_tok (int& tokVal);
	int next_page_num (int& blk, int &pn);
	void push_back () { cur_p = last_p; }
	int default_blk_num;
	char *rngStr;
	char *cur_p;
	char *last_p;
};

extern CdiFile cdi;


#endif