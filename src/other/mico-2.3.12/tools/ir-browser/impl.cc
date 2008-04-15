/*
 *  MICO --- a free CORBA implementation
 *  Copyright (C) 1997 Kay Roemer & Arno Puder
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
 */

#include <iostream.h>
#include <CORBA.h>
#include "impl.h"
#include <mico/template_impl.h>


CalcImpl::CalcImpl ()
    : CORBA::DynamicImplementation (CORBA::BOA::ReferenceData (),
				    new CORBA::InterfaceDef (NULL /* XXX */),
				    new CORBA::ImplementationDef ("Calc"))
{
}

void
CalcImpl::invoke (CORBA::ServerRequest_ptr svreq, CORBA::Environment &env)
{
    if (!strcmp (svreq->op_name(), "add")) {
	// create argument list
	CORBA::NVList_ptr args;
	// number of in/inout params
	CORBA::ULong nargs = svreq->param_count();
	_orb()->create_list (nargs+1, args);

	// fill in NamedValue's
	// XXX must set TypeCode's, too ...
	for (int i = 0; i < nargs; ++i)
	    args->add (CORBA::ARG_IN);
	args->add (CORBA::ARG_OUT);

	// get IN param values
	svreq->params (args);

	CORBA::Long result = 0;
	for (int i = 0; i < nargs; ++i) {
	    CORBA::Long x;
	    *args->item(i)->value() >>= x;
	    result += x;
	}

	// set result
	*args->item(nargs)->value() <<= result;
    } else
	assert (0);
}
