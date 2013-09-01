#include <windows.h>
#include <iostream>
#include "exception.h"
#include "cdifile.h"
#include "cfontdef.h"
#include "cfntmap.h"

using namespace std;

extern CdiFile cdi;

void CJKfontDefTable::init (dvi_uint maxcf)
{
	_size = maxcf+1;
	cftbl = new cfont_def*[_size];
	if (!cftbl)
		throw(EMemErr("cftbl (CJKfontDefTable::init)"));
	memset(cftbl, '\0', _size*sizeof(cfont_def*));
}

CJKfontDefTable::~CJKfontDefTable ()
{
	for (int i = 0; i < _size; i++)
		delete cftbl[i];
	delete [] cftbl;
}

dvi_uint
CJKfontDefTable::insert ()
{
	extern CJKfontMap cfontmap;

	char face_name[257];
	cfont_def *ci = new cfont_def;
	if (!ci) throw(EInternal("Insufficient memory"));

	dvi_pair cfont_num = cdi.readPair();
	check(cfont_num);

//	cout << "cfont_num = " << cfont_num << endl;

	int a = cdi.readByte();
	cdi.read(face_name, a);
	face_name[a] = '\0';
	ci->face_name = cfontmap[face_name];
	ci->charset = cdi.readByte();

	// cout << (int) ci->charset << endl;

	switch (ci->charset) {
		case BIG5: ci->win_charset = CHINESEBIG5_CHARSET; break;
		case GBK: ci->win_charset = GB2312_CHARSET; break;
		default: throw(EUnsupported("CharSet"));
	}

	ci->size = cdi.readQuad();
	ci->dsize = cdi.readQuad();
	ci->weight = cdi.readPair();
	ci->style = cdi.readByte();
	ci->width = cdi.readQuad();
	ci->height = cdi.readQuad();
	ci->depth = cdi.readQuad();
	ci->fw_width = cdi.readQuad();
	ci->fw_height = cdi.readQuad();
	ci->fw_depth = cdi.readQuad();
	cftbl[cfont_num] = ci;
	if (cfont_num > max_cfont_num)
		max_cfont_num = cfont_num;
	return cfont_num;
}