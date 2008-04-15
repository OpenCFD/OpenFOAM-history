#include "inheritance.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS


using namespace std;

int main( int argc, char *argv[] )
{
    //kcg: the main test here is to test if "inheritance.h" really
    //compiles well. If we're here, then it probably compiles.
    cout << "...passed" << endl;
}
