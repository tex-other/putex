#ifndef TTF2BMP_W32_H_
#define TTF2BMP_W32_H_

#include <windows.h>

#include "VCFontDrv.h"

class TTF2Bmp_W32 : public VCFontDrv {
public:
	TTF2Bmp_W32 () : VCFontDrv() {}
	virtual void choose_cfont (cfont_def&, char_metric&);
	virtual void release_cfont ();
	virtual bitmap_info* getBitmap (dbcode c);
private:
	HDC hdcMem;
	HBITMAP  hBitmap, hBitmapOrig; 
	HFONT    hFont;
};


#endif