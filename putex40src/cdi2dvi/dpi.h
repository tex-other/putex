#ifndef DPI_H_
#define DPI_H_

#include "types.h"

class CDpiTable {
public:
	CDpiTable (int sz = 256);
	~CDpiTable () { delete [] existSizes; }
	int dpicheck (int dpi, int bdpi);
private:
	void addsiz (int hsize);
	void adddpi (int hsize);
	int *existSizes;
};

extern CDpiTable *dpiTbl;
#endif