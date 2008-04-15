
#ifndef __FSTREAM_H__
#define __FSTREAM_H__

// PocketPC doesn't have fstream.h

class strbuf {
public:
	int gcount ()
	{
		MICO_NOT_IMPLEMENTED;
		return 0;
	}
};

class ifstream : public virtual istream {
public:
	ifstream (const char*)
	{
		MICO_NOT_IMPLEMENTED;
	}
	int good ()
	{
		MICO_NOT_IMPLEMENTED;
		return 0;
	}
	int eof ()
	{
		MICO_NOT_IMPLEMENTED;
		return 0;
	}
	strbuf read (char*, int)
	{
		MICO_NOT_IMPLEMENTED;
		strbuf ret;
		return ret;
	}
	ifstream& getline (char*, int)
	{
		MICO_NOT_IMPLEMENTED;
		return *this;
	}
};


class ios {
public:
	enum mode { app };
};


class ofstream : public virtual ostream {
public:
	ofstream (const char*, ios::mode)
	{
		MICO_NOT_IMPLEMENTED;
	}
};


#endif
