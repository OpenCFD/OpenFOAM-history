#include <include.h>
#include "util.h"

using namespace std;

const string error_string()
{
	return  error_string(GetLastError());
}

const string error_string(int error)
{

	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
		);
	// Process any inserts in lpMsgBuf.
	// ...
	// Display the string.
	trace(( "Error:%s", (LPCTSTR)lpMsgBuf));
	// Free the buffer.
	
	char buf[1025]="";
	strncpy(buf,(LPCSTR)lpMsgBuf,1024);
	
	LocalFree( lpMsgBuf );
	
	return string(buf);
}
