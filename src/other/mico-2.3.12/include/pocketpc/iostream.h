
// there is no iostream.h under PocketPC!

#ifndef __IOSTREAM_H__
#define __IOSTREAM_H__

#include <mico/assert.h>

class IOSTREAM_ENDS {};
extern IOSTREAM_ENDS ends;


class ostream {
public:
    virtual ostream& operator<< (char&)
    {
	MICO_NOT_IMPLEMENTED;
	return *this;
    }
    virtual ostream& operator<< (const char*)
    {
	MICO_NOT_IMPLEMENTED;
	return *this;
    }
    virtual ostream& operator<< (int)
    {
	MICO_NOT_IMPLEMENTED;
	return *this;
    }
    virtual ostream& operator<< (const IOSTREAM_ENDS&)
    {
        // do nothing
	return *this;
    }

};

class istream {
public:
	void get (char&)
	{
		MICO_NOT_IMPLEMENTED;
	}
	operator void* ()
	{
		MICO_NOT_IMPLEMENTED;
		return this;
	}
	virtual istream& operator>> (double)
	{
		MICO_NOT_IMPLEMENTED;
		return *this;
	}
};

extern ostream cout;
extern ostream cerr;
#define endl "\n"
#define hex  "HEX"

#endif
