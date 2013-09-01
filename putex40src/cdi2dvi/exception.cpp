#include <string>
#include <cstdlib>
#include "exception.h"

using namespace std;

EUnknown::EUnknown (int line, const char* file)
{
	char buf[10];
	_msg = string("Internal Error: ") + file + " (" + itoa(line, buf, 10) + ')';
}

EFatal::EFatal (const char* msg)
{
	_msg = msg;
}

EMemErr::EMemErr (const char* msg)
{
	_msg = string("Unable to allocate memory to variable: ") + msg;
}

EInternal::EInternal (const char* msg)
{
	_msg = string("Internal Error: ") + msg;
}

EBadFile::EBadFile (const char* fName, const char* msg)
{
	_msg = string(fName) + ": " + msg;
}

EBadCdiFormat::EBadCdiFormat (const char* msg)
{
	_msg = string("Bad CDI Format: ") + msg;
}

EEncoding::EEncoding ()
{
	_msg = "Invalid DBCS code";
}

EUnsupported::EUnsupported (const char* msg)
{
	_msg = string("Unsupported ") + msg;
}