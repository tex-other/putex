// obsolete
#if 0
#ifndef VCCMAP_H_
#define VCCMAP_H_

#include "types.h"
#include "cfontdef.h"
#include "vcencoding.h"


// This virtual class defines the interface for classes that convert DBCS
// codes to TeX codes.
//
class VCCmap {
public:
	VCCmap () {}
	virtual ~VCCmap () {}
	virtual void init (VCCharSet *enc, CJKfontDefTable& cfontTable) { encoding = enc; }

	// lookup function converts a double-byte code to a single-byte TeX code.
	//     cfontNum:	font number of the double-byte code
	//     code:		double-byte code
	//     fontNum:		return the converted font number
	//     byteCode:	return the converted  single-byte TeX code
	// The function returns true if the fontNum is a new font number; otherwise, it returns false.

	virtual bool lookup (dvi_pair cfontNum, dbcode code, dvi_font_num &fontNum, dvi_byte &byteCode) = 0;

	// get property of the new TeX font.
	virtual void new_font_def (dvi_font_num fontNum, dvi_pair cfont_num, font_def& fd)
	{
		fd.fontNum = fontNum;
		fd.checksum = 0;
		fd.size = cfontTable[cfont_num]->size;
		fd.dsize = cfontTable[cfont_num]->dsize;
		// other fields are filled by the overrided function of subclass.
	}
	virtual const char* get_mapped_font_name () = 0;
	virtual void clean_up () = 0;
protected:
	VCCharSet *encoding;
};

#endif
#endif