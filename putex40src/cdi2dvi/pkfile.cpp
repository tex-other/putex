#include <string.h>
#include <direct.h>
#include "exception.h"
#include "pkfile.h"
#include "dpi.h"
#include "cttf2bmp_w32.h"

static string comment="Created by cdi2dvi v3.0";
// static pk_byte mask[8] = {0x00, 0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F};
static pk_byte mask1[8] = {0x00, 0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE};
static pk_byte mask2[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

void PKFile::open (dev_info& dev, cfont_def &fd, string& font_name)
{
	extern string pk_root_dir;

    // dpi = (double(mag) * fd.size * dev.dpi_x) / (fd.dsize * 1000.0) + 0.5;
    dpi = dev.dpi_mag * fd.size / fd.dsize + 0.5;
    dpi = dpiTbl->dpicheck (dpi, dev.dpi_x);

	char buf[_MAX_PATH];
	sprintf(buf, "%sdpi%d\\", pk_root_dir.c_str(), dpi);
	_mkdir(buf);
	strcat(buf, "\\");
	strcat(buf, font_name.c_str());
	strcat(buf, ".pk");
	texofstream::open(buf);
	if(bad())
		throw (EBadFile("Fail to open", buf));
}

// This function computes the bounding box of bitmap. It is adapted from ttf2pk.
void PKFile::find_bbox (bitmap_info& bi)
{
	pk_byte msk, *p, *row_end;;
	bitmap = bi.bits;
	bitmap_end = bi.bitsEnd - bi.byteWidth + bi.logByteWidth;

	// clean up the garbage bits of bitmap
	int tail_bits = bi.width % 8;
	if (tail_bits) {
		msk = mask1[tail_bits];
		for (p = bi.bits + bi.logByteWidth - 1; p < bi.bitsEnd; p += bi.byteWidth)
			*p &= msk;
	}

	height = bi.height, width = bi.width;
	skip = 0;

	// trim top

	for (;;)
	{
		if (bitmap >= bi.bitsEnd) {  /* bitmap is totally blank */
			height = bi.height;
			width = bi.width;;
			pl = (width*height + 7)/8;
			bitmap = bi.bits;
			bitmap_end = bi.bitsEnd;
			return;
		}
		
		p = bitmap;
		row_end = bitmap + bi.logByteWidth;
		while (p < row_end && *p == 0)
			++p;
		if (p < row_end)
			break;
		++skip;
		bitmap += bi.byteWidth;
	}
	
	height -= skip;
	voff -= skip;

	/* trim bottom */
	skip = 0;
	
	for (;;)
	{
		p = bitmap_end - bi.byteWidth;
		row_end = p + bi.logByteWidth;
		while (p < row_end && *p == 0)
			++p;
		if (p < row_end)
			break;
		++skip;
		bitmap_end -= bi.byteWidth;
	}
	
	height -= skip;

	/* trim right */

	--width;
	
	for (;;)
	{
		msk = mask2[width % 8];
		for (p = bitmap + width / 8; p < bitmap_end; p += bi.byteWidth)
			if (msk & *p) break;
		if (p < bitmap_end)
			break;
		
		--width;
	}
	
	++width;
	
    /* trim left */
	
	skip = 0;
	
	for (;;)
	{
		msk = mask2[skip % 8];;
		for (p = bitmap + skip / 8; p < bitmap_end; p += bi.byteWidth)
			if (msk & *p) break;
		if (p < bitmap_end)
			break;
		
		++skip;
	}
	bitmap += skip / 8;
	width -= skip;
	hoff -= skip;
	skip = skip % 8;

	pk_len = (height * width + 7) / 8;
}


// Convert raw bitmap data to PK bitmap data (adapted from ttf2pk).

void PKFile::copy_char_bitmap (bitmap_info& bi)
{	
	pk_byte *rowptr;
	pk_byte *p;
	int  blib1;          /* bits left in byte */
	int  bits_left;      /* bits left in row */
	pk_byte *q;
	int  blib2;
	pk_byte nextbyte;
	
	
	q = bitmap;
	blib2 = 8;
	nextbyte = 0;
	dyn_f = 14;
	
	for (rowptr = bitmap; rowptr < bitmap_end; rowptr += bi.byteWidth)
	{
		p = rowptr;
		blib1 = 8 - skip;
		bits_left = width;
		
		if (blib2 != 8)
		{
			int n;
			
			
			if (blib1 < blib2)
			{
				nextbyte |= *p << (blib2 - blib1);
				n = blib1;
			}
			else
			{
				nextbyte |= *p >> (blib1 - blib2);
				n = blib2;
			}
			blib2 -= n;
			if ((bits_left -= n) < 0)
			{
				blib2 -= bits_left;
				continue;
			}
			if ((blib1 -= n) == 0)
			{
				blib1 = 8;
				++p;
				if (blib2 > 0)
				{
					nextbyte |= *p >> (8 - blib2);
					blib1 -= blib2;
					bits_left -= blib2;
					if (bits_left < 0)
					{
						blib2 = -bits_left;
						continue;
					}
				}
			}
			*q++ = nextbyte;
		}
		
		/* fill up whole (destination) bytes */
		
		while (bits_left >= 8)
		{
			nextbyte = *p++ << (8 - blib1);
			*q++ = nextbyte | (*p >> blib1);
			bits_left -= 8;
		}
		
		/* now do the remainder */
		
		nextbyte = *p << (8 - blib1);
		if (bits_left > blib1)
			nextbyte |= p[1] >> blib1;
		blib2 = 8 - bits_left;
	}
	
	if (blib2 != 8)
		*q++ = nextbyte;
	
	// write((char *) bitmap, q - bitmap);
	pk_len = q - bitmap;
}




//
//   Pack the bitmap using the rll method.  (Return false if it's better
//   to just pack the bits.)
//
// This code is adapted from ttf2pk.

 bool PKFile::pack_char_bitmap (bitmap_info& bi)
 {
	 static int   *counts   = NULL; /* area for saving bit counts */
	 static int   maxcounts = 0;    /* size of this area */
	 unsigned int ncounts;          /* max to allow this time */
	 int          *nextcount;       /* next count value */
	 
	 int  *counts_end;              /* pointer to end */
	 pk_byte *rowptr;
	 pk_byte *p;
	 pk_byte mask;
	 pk_byte *rowdup;                  /* last row checked for dup */
	 pk_byte paint_switch;             /* 0 or 0xff */
	 int  bits_left;                /* bits left in row */
	 int  cost;
	 int  i;
	 odd = false;
	 
	 /* Allocate space for bit counts. */
	 
	 ncounts = (width * height + 3) / 4;
	 if (ncounts > maxcounts)
	 {
		 delete [] counts;
		 counts = new int[ncounts + 2];
		 maxcounts = ncounts;
	 }
	 counts_end = counts + ncounts;
	 
	 /* Form bit counts and collect statistics */
	 
	 base = 0;
	 memset(deltas, 0, sizeof (deltas));
	 rowdup = NULL;                    /* last row checked for duplicates */
	 p = rowptr = bitmap;
	 mask = 0x80 >> skip;
	 flag = 0;
	 paint_switch = 0;
	 
	 if (*p & mask)
	 {
		 flag = 8;
		 paint_switch = 0xff;
	 }
	 
	 bits_left = width;
	 nextcount = counts;
	 
	 while (rowptr < bitmap_end)       /* loop over shift counts */
	 {
		 int shift_count = bits_left;
		 
		 
		 for (;;)
		 {
			 if (bits_left == 0)
			 {
				 if ((p = rowptr += bi.byteWidth) >= bitmap_end)
					 break;
				 mask = 0x80 >> skip;
				 bits_left = width;
				 shift_count += width;
			 }
			 if (((*p ^ paint_switch) & mask) != 0)
				 break;
			 --bits_left;
			 mask >>= 1;
			 if (mask == 0)
			 {
				 ++p;
				 while (*p == paint_switch && bits_left >= 8)
				 {
					 ++p;
					 bits_left -= 8;
				 }
				 mask = 0x80;
			 }
		 }
		 
		 if (nextcount >= counts_end)
			 return false;
		 shift_count -= bits_left;
		 *nextcount++ = shift_count;
		 tallyup(shift_count);
		 
		 /* check for duplicate rows */
		 if (rowptr != rowdup && bits_left != width)
		 {
			 byte *p1 = rowptr;
			 byte *q  = rowptr + bi.byteWidth;
			 int repeat_count;
			 
			 
			 while (q < bitmap_end && *p1 == *q)
			 {
				 ++p1;
				 ++q;
			 }
			 repeat_count = (p1 - rowptr) / bi.byteWidth;
			 if (repeat_count > 0)
			 {
				 *nextcount++ = -repeat_count;
				 if (repeat_count == 1)
					 --base;
				 else
				 {
					 ++base;
					 tallyup(repeat_count);
				 }
				 rowptr += repeat_count * bi.byteWidth;
			 }
			 rowdup = rowptr;
		 }
		 paint_switch = ~paint_switch;
	 }
	 
	 /* Determine the best dyn_f */
	 
	 dyn_f = 0;
	 cost = base += 2 * (nextcount - counts);
	 
	 for (i = 1; i < 14; ++i)
	 {
		 base += deltas[i - 1];
		 if (base < cost)
		 {
			 dyn_f = i;
			 cost = base;
		 }
	 }
	 
	 /* last chance to bail out */
	 
	 if (cost * 4 > width * height)
		 return false;
	 
	 /* Pack the bit counts */
	 
	 big_dyn_f = 208 - 15 * dyn_f;
	 bitmap_end = bitmap;
	 *nextcount = 0;
	 nextcount = counts;
	 
	 while (*nextcount != 0)
	 {
		 if (*nextcount > 0)
			 pk_put_count(*nextcount);
		 else
			 if (*nextcount == -1)
				 pk_put_nyb(15);
			 else
			 {
				 pk_put_nyb(14);
				 pk_put_count(-*nextcount);
			 }
			 ++nextcount;
	 }
	 
	 if (odd)
	 {
		 pk_put_nyb(0);
		 ++cost;
	 }
	 
	 if (cost != 2 * (bitmap_end - bitmap))
		 printf("Cost miscalculation:  expected %d, got %ld\n",
		 cost, (long)(2 * (bitmap_end - bitmap)));
	 pk_len = bitmap_end - bitmap;
	 return true;
}


void PKFile::output (dev_info& dev, cfont_def &fd, dbcode cfb[])
{
	extern VCFontDrv *font_drv;

	cm.width  = fd.width * dev.scale_x + 0.5;
	cm.height = fd.height * dev.scale_y + 0.5;
	cm.depth  = fd.depth * dev.scale_y + 0.5;
	cm.accent = cm.height - cm.depth;
	
	font_drv->choose_cfont(fd, cm);
	
	write_preamble(dpi, dpi, fd.dsize, comment);
	
	// char_descriptor cd;
	tfm_width = fd.fw_width;
	dyn_f = 14;
	int i=0;
	while (!cfb[i].is_null())   
	{
		bitmap_info *bip = font_drv->getBitmap(cfb[i]);
		cc = i;
		dx = bip->width;
		dy = bip->height;
		hoff = 0;
		voff = cm.accent;
		find_bbox (*bip);
		if (!pack_char_bitmap(*bip))
			copy_char_bitmap (*bip);
		pl = pk_len;
		write_char_descriptor ();
		write((char *) bitmap, pk_len);
		i++;
	} 
	
	font_drv->release_cfont();
	
	write_postamble();
}


