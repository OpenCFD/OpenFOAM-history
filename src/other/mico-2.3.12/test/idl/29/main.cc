#include <CORBA-SMALL.h>
#include "container.h"

int main( int argc, char *argv[] )
{
  // the following should be legal types
  A a;
  A::B b;
  A::B::C c;
  A::E e;
  A::E::F f;
}
