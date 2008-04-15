/*
 *  MIWCO --- Wireless CORBA support for MICO (OMG document formal/2004-04-02)
 *            Terminal Bridge administration program
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
#include <coss/CosNaming.h>

using namespace std;

typedef void (*CommFun_ptr)(ostream &, MobileTerminal::TerminalBridge_ptr);
typedef map<char, pair<string,CommFun_ptr> > MapCommand;

void
admin_release (ostream &s, MobileTerminal::TerminalBridge_ptr tb)
{
    tb->release(TRUE);
}

void
admin_break (ostream &s, MobileTerminal::TerminalBridge_ptr tb)
{
    tb->release(FALSE);
}

void
admin_connect (ostream &s, MobileTerminal::TerminalBridge_ptr tb)
{
    string addr;
    cout << "Enter Access Bridge address: ";
    getline(cin, addr);
    CORBA::Address *address = CORBA::Address::parse(addr.c_str());
    if (address == NULL) {
	cerr << "Bad Access Bridge address: " << addr << endl;
	return;
    }
    tb->connect(addr.c_str());
    delete address;
}

void
admin_services (ostream &s, MobileTerminal::TerminalBridge_ptr tb)
{
    MobileTerminal::ObjectIdList *list = tb->list_initial_services();
    s << "Services available at Terminal Bridge:" << endl;
    for (CORBA::ULong i = 0; i < list->length(); ++i) {
	s << '\t' << (*list)[i] << endl;
    }
}

void
admin_handoff (ostream &s, MobileTerminal::TerminalBridge_ptr tb)
{
    string addr;
    cout << "Enter new Access Bridge address: ";
    getline(cin, addr);
    CORBA::Address *address = CORBA::Address::parse(addr.c_str());
    if (address == NULL) {
	cerr << "Bad Access Bridge address: " << addr << endl;
	return;
    }
    tb->do_handoff(addr.c_str());
    delete address;
}

#define constrain(exp, mess) if (!(exp)) do { \
                                 cerr << "tadmin: " << mess << endl; \
                                 return EXIT_FAILURE; \
                             } while (0)

int
main (int argc, char *argv[])
{
    MapCommand commands;

    commands['1'] = make_pair<string,CommFun_ptr>("Get initial services",
						  &admin_services);
    commands['2'] = make_pair<string,CommFun_ptr>("Connect to Access Bridge",
						  &admin_connect);
    commands['3'] = make_pair<string,CommFun_ptr>("Break GTP connection",
						  &admin_break);
    commands['4'] = make_pair<string,CommFun_ptr>("Release GTP connection",
						  &admin_release);
    commands['5'] = make_pair<string,CommFun_ptr>("Do terminal handoff",
						  &admin_handoff);

    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    CORBA::Object_var obj;
    MobileTerminal::TerminalBridge_var tb;
    CosNaming::NamingContext_var ns;

    try {
	obj = orb->resolve_initial_references("MobileTerminalBridge");
	tb = MobileTerminal::TerminalBridge::_narrow(obj);
    } catch (const CORBA::ORB::InvalidName &) {
	obj = orb->resolve_initial_references("NameService");
	ns = CosNaming::NamingContext::_narrow(obj);
	char *tb_name;
	if (argc > 1)
	    tb_name = argv[1];
	else
	    tb_name = "TerminalBridge";
	CosNaming::Name n;
	n.length(2);
	n[0].id = CORBA::string_dup("MobilitySupport");
	n[1].id = CORBA::string_dup(tb_name);
	try {
	    obj = ns->resolve(n);
	    tb = MobileTerminal::TerminalBridge::_narrow(obj);
	} catch (CORBA::Exception &) {
	    cerr << "Could not resolve Terminal Bridge " << tb_name << endl;
	    exit(EXIT_FAILURE);
	}
    }

    for ( ; ; ) {
	cout << endl;
	for (MapCommand::iterator i = commands.begin();
	     i != commands.end(); ++i) {
	    cout << i->first << '\t' << i->second.first << endl;
	}
	cout << 'X' << '\t' << "Exit program" << endl << endl;
	cout << "Select action: ";
	string action;
	getline(cin, action);
	if (action[0] == 'X')
	    break;
	MapCommand::iterator itr = commands.find(action[0]);
	if (itr == commands.end()) {
	    cerr << "Invalid command: " << action[0] << endl;
	    continue;
	}
	try {
	    itr->second.second(cout, tb);
	} catch (const CORBA::Exception &e) {
	    cerr << "Received exception: " << e << endl;
	}
    }
    return 0;
}
