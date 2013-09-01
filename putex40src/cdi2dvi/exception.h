#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <exception>
#include <string>

using namespace std;

class EUnknown : public exception {
public:
	EUnknown () {} 
	EUnknown (int line, const char* file); 
	virtual const char *what() { return _msg.c_str(); }
protected:
	string _msg;
};

class EFatal : public EUnknown {
public:
	EFatal () {} 
	EFatal (const char* msg);
};

class EMemErr : public EUnknown {
public:
	EMemErr () {} 
	EMemErr (const char* msg);
};


class EInternal : public EUnknown {
public:
	EInternal () {} 
	EInternal (const char* msg);
};

class EBadFile : public EUnknown {
public:
	EBadFile () {} 
	EBadFile (const char* fName, const char* msg);
};

class EBadCdiFormat : public EBadFile {
public:
	EBadCdiFormat () {} 
	EBadCdiFormat (const char* msg);
};

class EEncoding : public EUnknown {
public:
	EEncoding (); 
};

class EUnsupported : public EUnknown {
public:
	EUnsupported () {}
	EUnsupported (const char* msg);

};


#endif