#include "exception.h"
#include "fontused.h"

FontUsedTbl::~FontUsedTbl ()
{
	delete [] used;
	delete [] defined;

	for (int i = 0; i < _size; i++)
		if (efont_def[i])
			delete [] efont_def[i]->def;

	delete [] efont_def;
}


void
FontUsedTbl::init (dvi_uint n)
{
	_size = n+1;
	used = new bool[_size];
	if (!used)
		throw(EMemErr("used (FontUsedTbl::init)"));
	memset(used, '\0', _size*sizeof(bool));

	defined = new bool[_size];
	if (!defined)
		throw(EMemErr("defined (FontUsedTbl::init)"));
	memset(defined, '\0', _size*sizeof(bool));

	efont_def = new efont_info*[_size];
	if (!efont_def)
		throw(EMemErr("efont_def (FontUsedTbl::init)"));
	memset(efont_def, '\0', _size*sizeof(efont_info*));
}

dvi_uint
FontUsedTbl::get_avail ()
{
	do {
		next_slot++;
	} while (used[next_slot] && next_slot < size());

	if (next_slot == size())
		throw(EFatal("Exceed font capacity"));
	if (next_slot > max_used)
		max_used = next_slot;
	used[next_slot] = true;
	return next_slot;
}