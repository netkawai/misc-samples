/***
*istream.h - definitions/declarations for the istream class
*
*	Copyright (c) 1990-1993, Microsoft Corporation.  All rights reserved.
*
*Purpose:
*	This file defines the classes, values, macros, and functions
*	used by the istream class.
*	[AT&T C++]
*
****/

#ifndef _INC_ISTREAM
#define _INC_ISTREAM

#include <ios.h>

// C4069: "long double != double"
#pragma warning(disable:4069)	// disable C4069 warning
// #pragma warning(default:4069)	// use this to reenable, if desired

// C4505: "unreferenced local function has been removed"
#pragma warning(disable:4505) // disable C4505 warning
// #pragma warning(default:4505)	// use this to reenable, if desired

// Force word packing to avoid possible -Zp override
#pragma pack(push,4)


typedef long streamoff, streampos;

class _CRTIMP istream : virtual public ios {

public:
    istream(streambuf*);
    virtual ~istream();

    int  ipfx(int =0);
    void isfx() { unlockbuf(); unlock(); }

    inline istream& operator>>(istream& (*_f)(istream&));
    inline istream& operator>>(ios& (*_f)(ios&));
    istream& operator>>(char *);
    inline istream& operator>>(unsigned char *);
    inline istream& operator>>(signed char *);
    istream& operator>>(char &);
    inline istream& operator>>(unsigned char &);
    inline istream& operator>>(signed char &);
    istream& operator>>(short &);
    istream& operator>>(unsigned short &);
    istream& operator>>(int &);
    istream& operator>>(unsigned int &);
    istream& operator>>(long &);
    istream& operator>>(unsigned long &);
    istream& operator>>(float &);
    istream& operator>>(double &);
    istream& operator>>(long double &);
    istream& operator>>(streambuf*);

    int get();
    istream& get(char *,int,char ='\n');
    inline istream& get(unsigned char *,int,char ='\n');
    inline istream& get(signed char *,int,char ='\n');
    istream& get(char &);
    inline istream& get(unsigned char &);
    inline istream& get(signed char &);
    istream& get(streambuf&,char ='\n');
    inline istream& getline(char *,int,char ='\n');
    inline istream& getline(unsigned char *,int,char ='\n');
    inline istream& getline(signed char *,int,char ='\n');

    inline istream& ignore(int =1,int =EOF);
    istream& read(char *,int);
    inline istream& read(unsigned char *,int);
    inline istream& read(signed char *,int);

    int gcount() const { return x_gcount; }
    int peek();
    istream& putback(char);
    int sync();

    istream& seekg(streampos);
    istream& seekg(streamoff,ios::seek_dir);
    streampos tellg();

    void eatwhite();	// consider: protect and friend with manipulator ws
protected:
    istream();
    istream(const istream&);	// treat as private
    istream& operator=(streambuf* _isb); // treat as private
    istream& operator=(const istream& _is) { return operator=(_is.rdbuf()); }
    int do_ipfx(int);

private:
    istream(ios&);
    int getint(char *);
    int getdouble(char *, int);
    int _fGline;
    int x_gcount;
};

    inline istream& istream::operator>>(istream& (*_f)(istream&)) { (*_f)(*this); return *this; }
    inline istream& istream::operator>>(ios& (*_f)(ios&)) { (*_f)(*this); return *this; }

    inline istream& istream::operator>>(unsigned char * _s) { return operator>>((char *)_s); }
    inline istream& istream::operator>>(signed char * _s) { return operator>>((char *)_s); }

    inline istream& istream::operator>>(unsigned char & _c) { return operator>>((char &) _c); }
    inline istream& istream::operator>>(signed char & _c) { return operator>>((char &) _c); }

    inline istream& istream::get(unsigned char * b, int lim ,char delim) { return get((char *)b, lim, delim); }
    inline istream& istream::get(signed char * b, int lim, char delim) { return get((char *)b, lim, delim); }

    inline istream& istream::get(unsigned char & _c) { return get((char &)_c); }
    inline istream& istream::get(signed char & _c) { return get((char &)_c); }

    inline istream& istream::getline(char * _b,int _lim,char _delim) { lock(); _fGline++; get(_b, _lim, _delim); unlock(); return *this; }
    inline istream& istream::getline(unsigned char * _b,int _lim,char _delim) { lock(); _fGline++; get((char *)_b, _lim, _delim); unlock(); return *this; }
    inline istream& istream::getline(signed char * _b,int _lim,char _delim) { lock(); _fGline++; get((char *)_b, _lim, _delim); unlock(); return *this; }

    inline istream& istream::ignore(int _n,int delim) { lock(); _fGline++; return get((char *)0, _n+1, (char)delim); unlock(); return *this; }

    inline istream& istream::read(unsigned char * _ptr, int _n) { return read((char *) _ptr, _n); }
    inline istream& istream::read(signed char * _ptr, int _n) { return read((char *) _ptr, _n); }

class _CRTIMP istream_withassign : public istream {
	public:
		istream_withassign();
		istream_withassign(streambuf*);
		~istream_withassign();
    istream& operator=(const istream& _is) { return istream::operator=(_is); }
    istream& operator=(streambuf* _isb) { return istream::operator=(_isb); }
};

#ifndef _WINDLL	   // Warning!  Not available under Windows without QuickWin:
extern _CRTIMP istream_withassign cin;
#endif

inline _CRTIMP istream& ws(istream& _ins) { _ins.eatwhite(); return _ins; }

_CRTIMP ios&		dec(ios&);
_CRTIMP ios&		hex(ios&);
_CRTIMP ios&		oct(ios&);

// Restore default packing
#pragma pack(pop)

#endif 		// !_INC_ISTREAM
