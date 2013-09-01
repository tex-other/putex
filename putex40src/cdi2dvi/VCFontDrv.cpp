#include "VCFontDrv.h"

void VCFontDrv::release_cfont ()
{
	delete [] bi.bits;
}
