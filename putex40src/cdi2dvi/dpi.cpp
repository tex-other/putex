#include <stdlib.h>
#include <limits.h>
#include "exception.h"
#include "dpi.h"

CDpiTable *dpiTbl;

CDpiTable::CDpiTable (int sz)
{
	existSizes = new int[sz];
	if (!existSizes)
		throw(EMemErr("existSizes (CDpiTable::CDpiTable)"));
	
	memset(existSizes, '\0', sizeof(int)*sz);

	existSizes[1] = INT_MAX;
}


// dpi check
int CDpiTable::dpicheck (int dpi, int bdpi) 
{
	adddpi (bdpi);
	for (int i = 0; dpi > existSizes[i]; i++)
		;
	int margin = 1 + dpi / 500;
	if ( (existSizes[i] - dpi) <= margin)
		return (existSizes[i]);
	else
		if ( (dpi - existSizes[i-1]) <= margin)
			return   (existSizes[i-1]) ;
		else
			return (dpi);
}


void CDpiTable::addsiz (int hsize)
{
	int *p = existSizes;

	while (*p < hsize) p++;
	if (*p == hsize) return;
	do {
		int t = *p;
		*p++ = hsize;
		hsize = t;
    } while (hsize);
}

void CDpiTable::adddpi (int hsize)
{
	addsiz (hsize) ;
	addsiz ((hsize * (long) 116161 + (long) 53020) / (long) 106040);
	int a = hsize;
	int b = 0 ;
	int c = 1 ;
	for (int i = 0; i < 9; i++)
    {
		b = 6 * b + (a % 5) * c;
		a = a + a / 5;
		c = c * 5;
		if (b > c)
		{
			b -= c;
			a++;
		}
		if (b + b >= c)
			addsiz (a + 1);
		else
			addsiz (a);
    }
}

