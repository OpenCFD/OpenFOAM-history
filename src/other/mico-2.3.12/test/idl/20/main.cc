#include <CORBA-SMALL.h>
#include "const.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS


using namespace std;

int main( int argc, char *argv[] )
{
  cout << c1 << endl;
  cout << c2 << endl;
  cout << c3 << endl;
  cout << c4 << endl;
  cout << c5 << endl;
  cout << c6 << endl;
  cout << c7 << endl;
  if( c8 == TRUE )
    cout << "TRUE" << endl;
  else
    cout << "FALSE" << endl;
  cout << c9 << endl;
  cout << c10 << endl;
  cout << foo::c11 << endl;
  cout << foo::c12 << endl;
}
