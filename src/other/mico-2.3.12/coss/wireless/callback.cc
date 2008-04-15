/*
 *  MIWCO --- Wireless CORBA support for MICO (OMG document formal/2004-04-02)
 *            A simple Handoff Callback object
 *  Copyright (C) 2001 Jaakko Kangasharju
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
 */

#include <CORBA.h>
#include <coss/MobileTerminal.h>
#include <fstream>
#include <ctime>

using namespace std;

class HandoffCallback_impl : public virtual POA_MobileTerminal::HandoffCallback
{
public:
    void report_handoff_status (MobileTerminal::HandoffStatus);
    HandoffCallback_impl (string filename);
private:
    ofstream out;
};

HandoffCallback_impl::HandoffCallback_impl (string filename)
    : out(filename.c_str())
{
    if (!out) {
	cerr << "Opening logfile failed, aborting" << endl;
	exit(EXIT_FAILURE);
    }
}

void
HandoffCallback_impl::report_handoff_status (MobileTerminal::HandoffStatus st)
{
    char tstr[256];
    time_t t;
    time(&t);
    struct tm *trec = localtime(&t);
    strftime(tstr, sizeof tstr, "%Y.%m.%d.%H:%M:%S", trec);
    out << tstr << ": ";
    if (st == MobileTerminal::HANDOFF_SUCCESS)
	out << "Handoff succeeded" << endl;
    else if (st == MobileTerminal::HANDOFF_FAILURE)
	out << "Handoff failed" << endl;
    else
	out << "Odd handoff status" << endl;
}

int
main (int argc, char *argv[])
{
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var root_poa = PortableServer::POA::_narrow(obj);
    PortableServer::POAManager_var root_mgr = root_poa->the_POAManager();

    HandoffCallback_impl *servant =
	new HandoffCallback_impl("/tmp/mico.hoff.log");
    MobileTerminal::HandoffCallback_var hoff = servant->_this();

    ofstream of("hoff.ref");
    of << orb->object_to_string(hoff) << endl;
    of.close();

    root_mgr->activate();
    orb->run();

    root_poa->destroy(TRUE, TRUE);
    delete servant;

    return 0;
}
