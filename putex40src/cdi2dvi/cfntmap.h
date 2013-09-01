/***********************************************************
*  FILE: cfntmap.h
*
***********************************************************/

#ifndef CFNTMAP_H_
#define CFNTMAP_H_

const int NO_ERR = 0;
const int FILE_OPEN_ERR = 1;
const int MISSING_PHYSICAL_FONT_NAME_ERR = 2;


struct FontPair {
	FontPair () : logFace(0), phyFace(0) {}
	char *logFace;
	char *phyFace;
};

class CJKfontMap {
public:
	CJKfontMap() : errCode(NO_ERR) {}
	~CJKfontMap();
	void init (int sz);
	void parse (const char *fileName); 
	inline int isReady () const;
	const char* operator[] (const char *logFaceName);
	char *errMsg () const;
private:
	// friend int qcmp (const void *a, const void *b);
	friend int bcmp (const void *a, const void *b);
	void reset(void);
	int nFont;
	int errCode;
	char *default_cfont;
	FontPair *fontMapTbl;
	int size;
};

inline int CJKfontMap::isReady () const
{
	return (errCode == NO_ERR)? 1 : 0;
}
#endif