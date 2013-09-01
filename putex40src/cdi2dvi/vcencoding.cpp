#include <windows.h>
#include <iostream>
#include "types.h"
#include "exception.h"
#include "cdifile.h"
#include "vcencoding.h"

VCCharSetPtr charsetTable[256];
VCCharSetPtr cur_charset;

void init_charset_table ()
{
	charsetTable[0] = new NullCharSet;
	charsetTable[BIG5] = new Big5CharSet;
	charsetTable[GBK] = new GBKCharSet;

	for (int k = 3; k < 256; k++)
		charsetTable[k] = new NullCharSet;

	cur_charset = charsetTable[BIG5];
}



//
// ================ BIG-5 (Traditional Chinese) ---------------------
//
int NullCharSet::check_code (dbcode &c)
{
	throw(EUnsupported("Charset is not support at the present time"));
	return c.value();
}

//
// ================ BIG-5 (Traditional Chinese) ---------------------
//

const int big5_hibyte_min = 0X81;
const int big5_hibyte_max = 0XFE;
const int big5_lobyte_min1 = 0X40;
const int big5_lobyte_max1 = 0X7E;
const int big5_lobyte_min2 = 0XA1;
const int big5_lobyte_max2 = 0XFE;

Big5CharSet::Big5CharSet ()
	: VCCharSet()
{
	default_dbcode.set(0XA1,0XBD);
	codepage = CHINESEBIG5_CHARSET;
}

int Big5CharSet::check_code (dbcode &c)
{
	if ((big5_hibyte_min <= c.hibyte && c.hibyte <= big5_hibyte_max) &&
		((big5_lobyte_min1 <= c.lobyte && c.lobyte <= big5_lobyte_max1) ||
		 (big5_lobyte_min2 <= c.lobyte && c.lobyte <= big5_lobyte_max2)))

		return c.value();
	else {
		cerr << "Invalid Big5 code at position " << cdi.tellg()
		     << " and was replaced by the default character" << endl;
		c = default_dbcode;
		return default_dbcode.value();
	}
}


//
// ================ GB 2312-80 (Simplified Chinese) ---------------------
//

const int gb_hibyte_min = 0XA1;
const int gb_hibyte_max = 0XFE;
const int gb_lobyte_min = gb_hibyte_min;
const int gb_lobyte_max = gb_hibyte_max;

GBKCharSet::GBKCharSet ()
	: VCCharSet()
{
	default_dbcode.set(0XA1,0XF6);
	codepage = GB2312_CHARSET;
}

int GBKCharSet::check_code (dbcode &c)
{
	if ((gb_hibyte_min <= c.hibyte && c.hibyte <= gb_hibyte_max) &&
		(gb_lobyte_min <= c.lobyte && c.lobyte <= gb_lobyte_max))
		return c.value();
	else {
		cerr << "Invalid GBK code at position " << cdi.tellg()
		     << " and was replaced by the default character" << endl;
		c = default_dbcode;
		return default_dbcode.value();
	}
}

//
// ================ Shift JIS (Japanese) ---------------------
//

const int jis_hibyte_min1 = 0X81;
const int jis_hibyte_max1 = 0X9F;
const int jis_hibyte_min2 = 0XE0;
const int jis_hibyte_max2 = 0XFC;
const int jis_lobyte_min = 0X40;
const int jis_lobyte_max = 0XFC;	// except 0X7F

ShiftJISCharSet::ShiftJISCharSet ()
	: VCCharSet()
{
	default_dbcode.set(0XA1,0XF6);
}

int ShiftJISCharSet::check_code (dbcode &c)
{
	throw(EUnsupported("Shift JIS is not support at the present time"));
	return c.value();
}
