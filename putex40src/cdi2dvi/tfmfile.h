#ifndef TFMFILE_H_
#define TFMFILE_H_

#include <fstream>
#include "types.h"
#include "texofstream.h"

using namespace std;

class TfmFile : texofstream {
public:
	TfmFile () : texofstream() {}
	void open (const string& dir, const string& font_name);
	void makeit (int bc, int ec, int dsize, int w, int h, int d);
};

#endif
