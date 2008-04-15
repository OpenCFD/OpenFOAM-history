
#include <winbase.h>
#include <afx.h>
#include <mico/config-ppc.h>
#include <iostream.h>
#include <strstream.h>
#include <errno.h>


// From iostream.h
ostream cout;
ostream cerr;
IOSTREAM_ENDS ends;

// From strstream.h
string ostrstream::_ostr;


char* getenv (const char*)
{
	//NOT_IMPLEMENTED;
	return NULL;
}

// errno.h
int errno;


extern "C" {
    char *sys_errlist[] = { "" };
    int sys_nerr = 0;
}

void mico_assert (const char* file, int line)
{
	CString msg;

	msg.Format (_T("File: %s, line: %d"), CString(file), line);
	HWND w = ::GetActiveWindow();
	::MessageBox (w, msg, CString ("Failed Assertion"), MB_OK | MB_ICONWARNING);
	::SendMessage (w, WM_CLOSE, 0, 0);
}

