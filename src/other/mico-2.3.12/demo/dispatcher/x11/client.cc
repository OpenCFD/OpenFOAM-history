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

#include <CORBA-SMALL.h>
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
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/Form.h>


using namespace std;

Widget top, form, entry, quit;

HelloWorld_var hello_client;

void
return_cb (Widget, XEvent *, String *, Cardinal *)
{
    String arg;
    XtVaGetValues (entry,
		   XtNstring, &arg,
		   NULL);

    hello_client->hello (arg);
}

void
quit_cb (Widget, XtPointer, XtPointer)
{
    exit (1);
}

int
main (int argc, char *argv[])
{
    /*
     * MICO initialization
     */
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

    /*
     * X11 initialization
     */
    static String fallback_res[] = {
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
    assert (argc == 2);
    CORBA::Object_var obj = orb->bind ("IDL:HelloWorld:1.0", argv[1]);
    if (CORBA::is_nil (obj)) {
	cout << "cannot bind to " << argv[1] << endl;
	return 1;
    }
    hello_client = HelloWorld::_narrow (obj);


    /*
     * X11 stuff
     */
    form = XtVaCreateManagedWidget ("form",
				   formWidgetClass,
				   top,
				   NULL);

    // entry field
    entry = XtVaCreateManagedWidget ("argument",
				     asciiTextWidgetClass,
				     form,
				     XtNwidth, 150,
				     XtNeditType, XawtextEdit,
				     XtNresizable, True,
				     XtNresize, XawtextResizeWidth,
				     NULL);
    XtTranslations trans
	= XtParseTranslationTable ("<Key>Return: ReturnAction()\n");
    XtOverrideTranslations (entry, trans);

    static XtActionsRec actions[] = {
	{ "ReturnAction", return_cb }
    };
    XtAppAddActions (appctx, actions, XtNumber (actions));

    // quit button
    quit = XtVaCreateManagedWidget ("quit",
				    commandWidgetClass,
				    form,
				    XtNfromVert, entry,
				    NULL);
    XtAddCallback (quit, XtNcallback, quit_cb, 0);


    XtRealizeWidget (top);
    XtAppMainLoop (appctx);

    return 0;
}


