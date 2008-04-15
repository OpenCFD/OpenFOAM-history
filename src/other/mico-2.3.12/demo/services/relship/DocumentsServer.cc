/*
 *  Demo of COSS Relationship Service
 *  Copyright (C) 1998 Karel Gardas
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  Send comments and/or bug reports to:
 *                 mico@informatik.uni-frankfurt.de
 *  or to my private e-mail:
 *                 gardask@alpha.inf.upol.cz
 */


#include "Documents.h"
#include "Documents_impl.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif
#include <unistd.h>

#define DEBUG 0

int main( int argc, char *argv[] )
{
  CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );
 
  CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
  PortableServer::POAManager_var mgr = poa->the_POAManager();
 
  FigureFactory_impl* ff = new FigureFactory_impl;
  POA_Documents::FigureFactory_tie<FigureFactory_impl>* fig_tie 
    = new POA_Documents::FigureFactory_tie<FigureFactory_impl> (*ff);

  TextFactory_impl* tf = new TextFactory_impl;
  POA_Documents::TextFactory_tie<TextFactory_impl>* text_tie
    = new POA_Documents::TextFactory_tie<TextFactory_impl> (*tf);
 
  poa->activate_object (fig_tie);
  poa->activate_object (text_tie);

  mgr->activate ();

  orb->run ();
#if DEBUG
  cout << "  <FigureServer> END!\n";
#endif
  return 0;
}





