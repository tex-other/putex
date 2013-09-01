#ifndef VCENCODING_H_
#define VCENCODING_H_

#include "types.h"

class VCCharSet {
public:
	VCCharSet () {}
	dbcode def_dbcode () const { return default_dbcode; }
	virtual int check_code (dbcode &c) = 0;
	int get_codepage() { return codepage; }
	inline VCCharSet * switchTo (int charset);

protected:
	dbcode default_dbcode;		// used for replacing invalid dbcode
	int codepage;				// Windows code page
};

typedef VCCharSet * VCCharSetPtr;

extern VCCharSetPtr charsetTable[256];
extern VCCharSetPtr cur_charset;

inline VCCharSet * VCCharSet::switchTo (int cs)
{
	return charsetTable[cs];
}


class NullCharSet : public VCCharSet {
public:
	NullCharSet () : VCCharSet () {}
	virtual int check_code (dbcode &c);
};

class Big5CharSet : public VCCharSet {
public:
	Big5CharSet ();
	virtual int check_code (dbcode &c);
};

class GBKCharSet : public VCCharSet {
public:
	GBKCharSet ();
	virtual int check_code (dbcode &c);
};

class ShiftJISCharSet : public VCCharSet {
public:
	ShiftJISCharSet ();
	virtual int check_code (dbcode &c);
};

extern void init_charset_table ();
#endif