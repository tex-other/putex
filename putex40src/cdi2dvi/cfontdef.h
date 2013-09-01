#ifndef CFONTDEF_H_
#define CFONTDEF_H_

#include "types.h"
#include "exception.h"

class CJKfontDefTable {
public:
	CJKfontDefTable () : max_cfont_num(-1) {}
	~CJKfontDefTable ();

	void init (dvi_uint maxcf);
	cfont_def* operator[] (int cfn) const { return cftbl[cfn]; }
	bool isset (int cfn) const { return cftbl[cfn] != 0; }
	dvi_scaled get_dsize (int cfn) const { return cftbl[cfn]->dsize; }
	dvi_byte get_charset (int cfn) const { return cftbl[cfn]->charset; }
	int size () { return _size; }
	dvi_uint insert ();
	void set_max_cfont_num (int m) { max_cfont_num = m; }
	int get_max_cfont_num () const { return max_cfont_num; }
	cfont_def* operator[] (dvi_pair cfont_num) { return cftbl[cfont_num]; }
	bool is_defined (dvi_pair cfont_num) { check(cfont_num); return cftbl[cfont_num] == 0; }

protected:
	inline void check (dvi_pair cfont_num);
	cfont_def** cftbl;
	dvi_uint _size;
	int max_cfont_num;
};

inline void CJKfontDefTable::check (dvi_pair cfont_num)
{
	if (cfont_num >= size())
		throw(EBadCdiFormat("Illegal CFont number"));	//???
}

extern CJKfontDefTable cfontTable;

#endif