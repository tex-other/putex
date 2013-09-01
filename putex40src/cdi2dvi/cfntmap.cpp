/***********************************************************
*  FILE: cfntmap.cpp
*
***********************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "exception.h"

#include "cfntmap.h"


static char emsg[][40] = {
	"No error",
		"Can't open font mapping file",
		"Missing physical font face name"
};


static int qcmp (const void *a, const void *b)
{
	return strcmp(((FontPair *) a)->logFace, ((FontPair *) b)->logFace);
}


void CJKfontMap::init (int sz)
{
	fontMapTbl = new FontPair[sz];
	if (!fontMapTbl)
		throw(EMemErr("fontMapTbl"));
	size =sz;
}

// EXPORT: destructor
CJKfontMap::~CJKfontMap ()
{
	reset();
	errCode = NO_ERR;
}

// PRIVATE
void CJKfontMap::reset (void)
{
	for (int i =0; i < nFont; i++) {
		delete [] fontMapTbl[i].logFace;
		delete [] fontMapTbl[i].phyFace;
	}
	delete [] fontMapTbl;
	nFont = 0;
}


static int bcmp (const void *logFaceName, const void *b)
{
	return strcmp((char *) logFaceName, ((FontPair *) b)->logFace);
}


const char* CJKfontMap::operator[] (const char *logFaceName)
{
	FontPair *p = (FontPair *) bsearch(logFaceName, fontMapTbl, nFont, sizeof(FontPair), &bcmp);
	return (p)? p->phyFace : fontMapTbl[0].phyFace;
}


const char comment_char = '%';

inline int ISSPACE (char c)
{
	return (c==' '||c =='\t'||c=='\n')? 1 : 0; 
}

// advance to the location of next character
inline char* next_char_loc (char *p)
{
	return (*p < 0)? p+2 : p+1;
}

// PRIVATE
void CJKfontMap::parse (const char *fileName)
{
	FILE *fp;
	const int max_line = 256;
	char line[max_line], *p, *logp, *phyp;
	
	if ((fp = fopen(fileName, "r")) == NULL)
		throw(EBadFile(fileName, "Unable to open"));
	
	nFont = 0;
	while (fgets(line, max_line, fp)) {
		for (p = line; ISSPACE(*p); p++) // skip spaces
			;
		if (*p == '\0' || *p == comment_char)
			continue;
		
		// get the logical font face name
		logp = p;
		while (!ISSPACE(*p))
			p = next_char_loc(p); // don't use p++ here
		*p++ = '\0';
		
		while (ISSPACE(*p)) p++; // skip spaces
		
		if (*p == '\0') {
			errCode = MISSING_PHYSICAL_FONT_NAME_ERR;
			continue;
		}
		
		// get the physical font face name
		phyp = p;
		while (!ISSPACE(*p))
			p = next_char_loc(p); // don't use p++ here
		*p = '\0';
		
		fontMapTbl[nFont].logFace = new char[strlen(logp)+1];
		strcpy(fontMapTbl[nFont].logFace, logp);
		fontMapTbl[nFont].phyFace = new char[strlen(phyp)+1];
		strcpy(fontMapTbl[nFont].phyFace, phyp);
		
		nFont++;
	}
	if (fp) fclose(fp);
	qsort(fontMapTbl, nFont, sizeof(FontPair), &qcmp);
}

char* CJKfontMap::errMsg () const
{
	return emsg[errCode];
}
