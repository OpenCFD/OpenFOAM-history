/*
 * This file is part of the load-balancing example
 *
 * In this example, the client obtains the reference to a printer server
 * object and then asks the latter to print a message.  If the naming service
 * from where the client obtains the reference does load-balancing, the
 * client might obtain the reference to a potentially different server object
 * each time it does nc->resolve() or nc->resolve_str().
 */

#include <stdlib.h>
#include <stdio.h>
#include <coss/CosNaming.h>
#include "Printer.h"


int main (int argc, char *argv[])
{
  CORBA::ORB_var orb;
  CORBA::Object_var obj;
  int count;
  int total;
  char *printerName;
  CosNaming::NamingContextExt_var nc;
  Printer_var printer;


  // The name of the target printer
  printerName = argv[1];

  // The number of times looking for a printer and printing something
  total = atoi (argv[2]);

  // Initialize the ORB
  orb = CORBA::ORB_init (argc, argv);

  // Obtain the reference to the naming service
  obj = orb->resolve_initial_references ("NameService");

  nc = CosNaming::NamingContextExt::_narrow (obj);

  // Look for a printer and print something.  Do these a number of times.
  for (count = 1; count <= total; ++count)
  {
    // Obtain from the naming service the reference to a printer
    obj = nc->resolve_str (printerName);

    printer = Printer::_narrow (obj);

    // Ask the printer to print a message
    printer->print ("this is a message");
  }

  return (EXIT_SUCCESS);
}
