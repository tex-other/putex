#include "exception.h"
#include "cttf2bmp_w32.h"

const int rotate_flag = 0x01;
const int inverse_flag = 0x08;
const int strikeout_flag = 0x10;
const int underline_flag = 0x20;
const int italic_flag = 0x40;

void TTF2Bmp_W32::choose_cfont (cfont_def& cfd, char_metric& cm)
{
	LOGFONT    lf;
	TEXTMETRIC tm;
	BITMAP     bm;
		
	// creates a memory device context (DC) compatible with the display device.
	hdcMem = CreateCompatibleDC(NULL); 
	
	lf.lfHeight = -cm.height;
	lf.lfWidth  = (cm.width == cm.height) ? 0 : cm.width;
	lf.lfEscapement     = 0 ;
	lf.lfOrientation    = 0 ;
	lf.lfWeight         = cfd.weight ;
	lf.lfItalic         = ((cfd.style & italic_flag) == 0 ) ? 0 : 1 ;
	lf.lfUnderline      = ((cfd.style & underline_flag) == 0 ) ? 0 : 1 ;
	lf.lfStrikeOut      = ((cfd.style & strikeout_flag) == 0 ) ? 0 : 1 ;
	lf.lfCharSet        = cfd.win_charset;
	lf.lfOutPrecision   = OUT_DEFAULT_PRECIS  ;
	lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS ;
	lf.lfQuality        = PROOF_QUALITY       ;
	lf.lfPitchAndFamily = DEFAULT_PITCH       ;
	if (cfd.style & rotate_flag)  {       // rotation font 90 angle 
		strcpy(lf.lfFaceName,"@");
		strcat (lf.lfFaceName, cfd.face_name.c_str()) ;  // face_name = @face_name
	}
	else  
		strcpy (lf.lfFaceName, cfd.face_name.c_str()) ;  // not rotation font (Normal)
	if ( (hFont = CreateFontIndirect(&lf) ) == NULL )  // Create LogFont
		throw(EFatal("Unable to create font"));
	hFont = (HFONT) SelectObject(hdcMem,hFont);
	if (GetTextMetrics(hdcMem,&tm) == 0) 
		throw(EFatal("Unable to get text metric"));
	hBitmap = CreateBitmap (tm.tmMaxCharWidth, tm.tmHeight , 1, 1, NULL);  
	hBitmapOrig = (HBITMAP) SelectObject(hdcMem, hBitmap);
	GetObject(hBitmap,sizeof(BITMAP),(LPVOID)&bm);  // get bitmap infomation
	int dwCount=(int)bm.bmWidthBytes*bm.bmHeight*bm.bmPlanes;
	bi.width  = bm.bmWidth;
	bi.height = bm.bmHeight;
	bi.byteWidth = bm.bmWidthBytes;
	bi.logByteWidth = (bi.width + 7) / 8;
	bi.bits = new BYTE[dwCount];
	if (bi.bits == NULL) 
		throw(EMemErr("bi.bits (TTF2Bmp_W32::choose_cfont)"));
	bi.bitsEnd = bi.bits + dwCount;

	// PK fonts and Windows fonts use reversed bit values for black and white.
	if (cfd.style & inverse_flag) 
	{
		SetTextColor(hdcMem,RGB(0,0,0));        // text color is black
		SetBkColor(hdcMem,RGB(255,255,255));    // background is white
		bgColor = WHITENESS;
	}
	else
	{
		SetTextColor(hdcMem,RGB(255,255,255));  // text color is white
		SetBkColor(hdcMem,RGB(0,0,0));          // background is black   (Normal)
		bgColor = BLACKNESS;
	}	
}

void TTF2Bmp_W32::release_cfont ()
{
	VCFontDrv::release_cfont();
	DeleteDC (hdcMem) ;
	DeleteObject (SelectObject(hdcMem,hFont));  
	DeleteObject (SelectObject(hdcMem,hBitmapOrig)); 
}

bitmap_info* TTF2Bmp_W32::getBitmap (dbcode c)
{
	char word[3];
	
	word[0]=c.hibyte;   // set CJK Code
	word[1]=c.lobyte;
	word[2]='\0';
	PatBlt(hdcMem, 0, 0, bi.width, bi.height, bgColor); // clear bitmap
	TextOut(hdcMem, 0, 0, word, 2);
	// retrieve  bitmap's pixel data  in  bi.bits(array)
	if ( GetBitmapBits(hBitmap, bi.byteWidth*bi.height, bi.bits) == 0 )
		throw(EFatal("Unable to get character bitmap"));
	return &bi;
}
