/*
 * This file is part of the load-balancing example
 *
 * In this example, a server process houses a single printer server object.
 * The server registers its printer server object with the naming service.
 * To demonstrate the load-balancing function of the naming service, this
 * example requires the launching of several server processes, each of which
 * houses a printer server object with a unique name.  Names have id and kind
 * components.  Unique names in this case refers to names with unique id values
 * but identical kind values.  This example uses stringified names that have
 * the format "<id>.<kind>".
 */

#include <stdlib.h>
#include <stdio.h>
#include <coss/CosNaming.h>
#include "Printer.h"
#include "Printer_impl.h"


int main (int argc, char *argv[])
{
  CORBA::ORB_var orb;
  CORBA::Object_var obj;
  CosNaming::Name_var name;
  CosNaming::NamingContextExt_var nc;
  PortableServer::POA_var poa;
  PortableServer::POAManager_var poaManager;
  Printer_impl *serverImpl;
  char *printerName;


  // The printer name
  printerName = argv[1];

  // Initialize the ORB
  orb = CORBA::ORB_init (argc, argv);

  // Obtain the reference to the naming service
  obj = orb->resolve_initial_references ("NameService");

  nc = CosNaming::NamingContextExt::_narrow (obj);

  // Obtain the reference to the root POA
  obj = orb->resolve_initial_references ("RootPOA");

  poa = PortableServer::POA::_narrow (obj);

  // Obtain the reference to the POA manager
  poaManager = poa->the_POAManager ();
  
  // Prepare a printer server object
  serverImpl = new Printer_impl (printerName);

  // Activate the printer server object
  poa->activate_object (serverImpl);

  // Activate the POA manager
  poaManager->activate ();

  // Register the printer server object with the naming service
  name = nc->to_name (printerName);
  nc->bind (name, serverImpl->_this ());

  // Run the ORB
  orb->run ();

  // Shutdown
  poa->destroy (TRUE, TRUE);
  delete serverImpl;

  return (EXIT_SUCCESS);
}

