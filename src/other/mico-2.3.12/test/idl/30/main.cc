#include <CORBA.h>
#include "struct-fwd.h"

int main( int argc, char *argv[] )
{
  // the following should be legal types
  Link l;
  uLink u;

  // this code doesn't make a lot of sense
  l.descendants.length (1);
  l.descendants[0] = l;
  l.udescendants.length (1);
  l.udescendants[0] = u;

  u.descendants().length (1);
  u.descendants()[0] = l;
  u.udescendants().length (1);
  u.udescendants()[0] = u;

  return 0;
}
