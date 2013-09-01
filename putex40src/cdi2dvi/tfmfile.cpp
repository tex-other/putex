#include "tfmfile.h"

static string signature = "Created by cdi2dvi  "; // 20 bytes = 5 words

void TfmFile::open (const string& dir, const string& font_name)
{
//	string filePath = fd.area + fd.fName;
	string filePath = dir + font_name + ".tfm";
	texofstream::open(filePath);
}

void TfmFile::makeit (int bc, int ec, int dsize, int w, int h, int d)
{
	dvi_pair lf;

	// Output the length values
	lf = 6+7+(ec-bc+1)+7;
	putPair(lf);
	putPair(7);  // lh
	putPair(bc); // bc
	putPair(ec); // ec
	putPair(2);  // nw
	putPair(2);  // nh
	putPair(2);  // nd
	putPair(1);  // ni
	putPair(0);  // nl
	putPair(0);  // nk
	putPair(0);  // ne
	putPair(0);  // np

	// Output the headers
	putQuad(0);				// checksum
	putQuad(dsize<<4);		//design size
	putString(signature);	// output the signature string

	// output the char info word
	for (int i = bc; i <= ec; i++)
		putQuad(0X01110000);

	putQuad(0); putQuad(w); // width table
	putQuad(0); putQuad(h); // width table
	putQuad(0); putQuad(d); // width table
	putQuad(0);             // italic table

	close();
}

