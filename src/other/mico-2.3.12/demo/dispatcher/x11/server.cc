/*
 *  MICO --- a free CORBA implementation
 *  Copyright (C) 1997-98 Kay Roemer & Arno Puder
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

#include <CORBA.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif
#include "hello.h"
#include <mico/x11.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Form.h>

Widget top, form, label, quit;

class Hello_impl : virtual public POA_HelloWorld {
public:
    void hello (const char *s)
    {
	XtVaSetValues (label,
		       XtNlabel, s,
		       NULL);
    }
};

void
quit_cb (Widget, XtPointer, XtPointer)
{
    exit (0);
}

int
main (int argc, char *argv[])
{
    /*
     * MICO initialization
     */
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);
    PortableServer::POAManager_var mgr = poa->the_POAManager();

    /*
     * X11 initialization
     */
    static String fallback_res[] = {
	"*argument.label: ",
	"*quit.label: Quit",
	NULL
    };

    XtAppContext appctx;
    top = XtAppInitialize (&appctx, "Xhello",
			   0, 0,
			   &argc, argv,
			   fallback_res,
			   0, 0);

    /*
     * Use X11 dispatcher
     */
    orb->dispatcher (new X11Dispatcher (appctx));

    /*
     * MICO stuff
     */
    Hello_impl* hello = new Hello_impl;
    hello->_this();

    /*
     * X11 stuff
     */
    form = XtVaCreateManagedWidget ("form",
				   formWidgetClass,
				   top,
				   NULL);

    // label
    label = XtVaCreateManagedWidget ("argument",
				     labelWidgetClass,
				     form,
				     XtNwidth, 150,
				     XtNjustify, XtJustifyLeft,
				     NULL);

    // quit button
    quit = XtVaCreateManagedWidget ("quit",
				    commandWidgetClass,
				    form,
				    XtNfromVert, label,
				    NULL);
    XtAddCallback (quit, XtNcallback, quit_cb, 0);


    XtRealizeWidget (top);

    mgr->activate();
    orb->run ();

    return 0;
}
