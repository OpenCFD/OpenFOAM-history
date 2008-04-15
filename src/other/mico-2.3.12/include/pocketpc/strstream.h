
#ifndef __STRSTREAM_H__
#define __STRSTREAM_H__

#include <iostream.h>
#include <ministl/string>


class RdBuf {
public:
    void freeze (int)
    {
	// do nothing
    }
};

static RdBuf __rdbuf;


class ostrstream : public virtual ostream {
private:
    static string _ostr;
    
public:
    ostrstream ()
    {
	_ostr = "";
    }
    const char *str()
    {
	return _ostr.c_str();
    }
    RdBuf* rdbuf ()
    {
	return &__rdbuf;
    }

    // needed for xdec() in util.cc
    virtual ostream& operator<< (int d)
    {
	static char* digits = "0123456789";
	string num = "";

	do {
	    num = string(digits [d % 10]) + num;
	    d = d / 10;
	} while (d != 0);
	
	_ostr += num;
	return *this;
    }
};

class istrstream : public virtual istream {
public:
	istrstream (const char*)
	{
		MICO_NOT_IMPLEMENTED;
	}
};



#endif
