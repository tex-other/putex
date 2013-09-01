#ifndef PKFILE_H_
#define PKFILE_H_

#include "texofstream.h"
#include "VCFontDrv.h"

typedef unsigned char pk_byte;
typedef pk_byte pk_cmd;
typedef unsigned int pk_quad;

// PK commands
const pk_cmd pk_xxx1 =  240;
const pk_cmd pk_xxx2 =  241;
const pk_cmd pk_xxx3 =  242;
const pk_cmd pk_xxx4 =  243;
const pk_cmd pk_yyy =   244;
const pk_cmd pk_post =  245;
const pk_cmd pk_no_op = 246;
const pk_cmd pk_pre =   247;

// PK file identification number
const pk_byte pk_id =    89;		

class PKFile : public texofstream {
public:
	void open (dev_info& dev, cfont_def &fd, string& font_name);
	virtual void output (dev_info& dev, cfont_def &fd, dbcode cfb[]);
protected:
	inline void pk_put_nyb (int n);
	inline void pk_put_long (int n);
	inline void pk_put_count (int n);
	inline void tallyup (int n);
	inline void write_preamble (int dpi_x, int dpi_y, int dsize, string& comment);
	inline void write_postamble ();
	inline void write_char_descriptor ();
	void find_bbox (bitmap_info& bi);
	bool pack_char_bitmap (bitmap_info& bi);
	void copy_char_bitmap (bitmap_info& bi);
	inline bool fit_long_form ()
	{
		return (pl > 196594) || (dx > 65535) || (dy > 65535) || (width > 65535) ||
			   (height > 65535) || (hoff > 32767) || (voff > 32767) ||
			   (hoff < -32768) || (voff < -32768);
	}
	inline bool fit_extended_form ()
	{
		return (pl > 1015) || (dx > 255) || (width > 255) || (height > 255) ||
			   (hoff > 127) || (voff > 127) || (hoff < -128) ||(voff < -128);
	}

	int dpi;
	char_metric cm;
	pk_byte *bitmap, *bitmap_end;
	int skip;
	bool odd;
	pk_byte part;
	pk_byte flag;
	int big_dyn_f;
	int pk_len;
	int base;          /* cost of this character if pk_dyn_f = 0 */
	int deltas[13];    /* cost of increasing pk_dyn_f from i to i+1 */

	// current character descripter
	int dyn_f;
	pk_quad pl;				// packet length
	pk_quad cc;				// character code
	pk_quad tfm_width;		// tfm width
	pk_quad dx, dy;			// horizontal and vertical escpements
	pk_quad width, height;	// width and height of bounding box (in pixels)
	int hoff, voff;			// horizontal and vertical offset

};

inline void PKFile::pk_put_nyb (int n)
{
  if (odd)
  {
    *bitmap_end++ = (part << 4) | n;
    odd = false;
  }
  else
  {
    part = n;
    odd = true;
  }
}


inline void PKFile::pk_put_long (int n)
{
  if (n >= 16)
  {
    pk_put_nyb(0);
    pk_put_long(n / 16);
  }
  pk_put_nyb(n % 16);
}


inline void PKFile::pk_put_count(int n)
{
  if (n > dyn_f)
  {
    if (n > big_dyn_f)
      pk_put_long(n - big_dyn_f + 15);
    else
    {
      pk_put_nyb(dyn_f + (n - dyn_f + 15) / 16);
      pk_put_nyb((n - dyn_f - 1) % 16);
    }
  }
  else
    pk_put_nyb(n);
}

inline void PKFile::tallyup (int n)
{
  int m;


  if (n > 208)
  {
    ++base;
    n -= 192;
    for (m = 0x100; m != 0 && m < n; m <<= 4)
      base += 2;
    if (m != 0 && (m = (m - n) / 15) < 13)
      deltas[m] += 2;
  }
  else if (n > 13)
    ++deltas[(208 - n) / 15];
  else
    --deltas[n - 1];
}

inline void PKFile::write_preamble (int dpi_x, int dpi_y, int dsize, string& comment)
{
	pk_quad hppp = (dpi_x << 16) / 72.27;		// horizontal pixel per point
	pk_quad vppp = (dpi_y << 16) / 72.27;		// vertical pixel per point
	
	putByte(pk_pre) ;			// PK preamble command
	putByte(pk_id) ;			// identification
	putByte(comment.size()) ;   // comment string length
	putString(comment);			// comment string
	putQuad(dsize<<4) ;			// design size
	putQuad(0) ;				// checksum
	putQuad(hppp) ;				// horizontal resolution
	putQuad(vppp) ;				// vertical resolution
}

inline void PKFile::write_postamble (void)  // write PK file postamble
{
	long pos;
	
	putByte(pk_post);       // write PK postamble command
	pos = tellp();     // get current file point location
	while (pos % 4 != 0) 
	{
		putByte(pk_no_op) ;    // write PK command -> no operation
		pos++;
	}
}

inline void PKFile::write_char_descriptor ()
{
  flag |= (dyn_f << 4);

  if(fit_long_form()) {
    flag |= 7;				// set flag last 3 bit -> 111
    putByte(flag);
    pl += 28;
    putQuad(pl);
    putQuad(cc);
    putQuad(tfm_width); 
    putQuad((dx<<16));
    putQuad((dy<<16));
    putQuad(width);
    putQuad(height);
    putQuad(hoff);
    putQuad(voff);
  }
  else if (fit_extended_form()) {
    pl += 13;
    flag += (pl/65536) + 4;	// set flag last 3 bit -> 100 or 101 or 110 
    putByte(flag);
    putPair((pl%65536));
    putByte(cc);
    putTrio(tfm_width);
    putPair(dx);
    putPair(width);
    putPair(height);
    putPair(hoff);
    putPair(voff);
  }
  else {
    pl += 8;
    flag += (pl/256);	// set flag last 3 bit -> 000 or 001 or 010 or 011 
    putByte(flag);
    putByte(pl%256);
    putByte(cc);
    putTrio(tfm_width);
    putByte(dx);
    putByte(width);
    putByte(height);
    putByte(hoff);
    putByte(voff);
  }
}

#endif

