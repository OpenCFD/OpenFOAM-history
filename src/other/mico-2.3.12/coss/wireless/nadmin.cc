/*
 *  MIWCO --- Wireless CORBA support for MICO (OMG document formal/2004-04-02)
 *            Access Bridge administration program
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
#include <mico/util.h>
#include <coss/MobileTerminal.h>
#include <coss/CosNaming.h>

using namespace std;

typedef void (*CommFun_ptr)(ostream &, MobileTerminal::AccessBridge_ptr);
typedef map<char, pair<string,CommFun_ptr> > MapCommand;

void
admin_services (ostream &s, MobileTerminal::AccessBridge_ptr ab)
{
    MobileTerminal::ObjectIdList *list = ab->list_initial_services();
    s << "Services available at Access Bridge:" << endl;
    for (CORBA::ULong i = 0; i < list->length(); ++i) {
	s << '\t' << (*list)[i] << endl;
    }
}

void
admin_attached (ostream &s, MobileTerminal::AccessBridge_ptr ab)
{
    string sid;
    cout << "Enter Terminal Id (encoded): ";
    getline(cin, sid);
    CORBA::ULong len = sid.length();
    CORBA::Octet *id = mico_url_decode(sid.c_str(), len);
    MobileTerminal::TerminalId terminal_id;
    terminal_id.length(len);
    memcpy(terminal_id.get_buffer(), id, len);
    CORBA::string_free((char *)id);
    CORBA::Boolean is_att = ab->terminal_attached(terminal_id);
    if (is_att)
	s << "Terminal " << sid << " is attached here" << endl;
    else
	s << "Terminal " << sid << " is not attached here" << endl;
}

const char *
proto (CORBA::Short id)
{
    if (id ==  MobileTerminal::TCP_TUNNELING)
	return "TCP";
    else if (id == MobileTerminal::UDP_TUNNELING)
	return "UDP";
    else if (id ==  MobileTerminal::WAP_TUNNELING)
	return "WAP";
    else
	return "Unknown";
}

void
admin_addresses (ostream &s, MobileTerminal::AccessBridge_ptr ab)
{
    MobileTerminal::AccessBridgeTransportAddressList *list;
    ab->get_address_info(list);
    for (CORBA::ULong i = 0; i < list->length(); ++i) {
	s << "Address " << i + 1 << endl;
	s << '\t' << "Protocol: "
	  << proto((*list)[i].tunneling_protocol.protocol_id) << endl;
	s << '\t' << "Version: "
	  << (CORBA::Long) (*list)[i].tunneling_protocol.gtp_version.major
	  << '.'
	  << (CORBA::Long) (*list)[i].tunneling_protocol.gtp_version.minor
	  << endl;
	s << '\t' << "Transport endpoint: "
	  << mico_url_encode((*list)[i].transport_address.get_buffer(),
			     (*list)[i].transport_address.length()) << endl;
	s << endl;
    }
}

void
admin_handoff (ostream &s, MobileTerminal::AccessBridge_ptr ab)
{
    string sid;
    cout << "Enter Terminal Id (encoded): ";
    getline(cin, sid);
    CORBA::ULong len = sid.length();
    CORBA::Octet *id = mico_url_decode(sid.c_str(), len);
    MobileTerminal::TerminalId terminal_id;
    terminal_id.length(len);
    memcpy(terminal_id.get_buffer(), id, len);
    CORBA::string_free((char *)id);
    cout << "Enter Access Bridge: ";
    getline(cin, sid);
    CORBA::ORB_var orb = CORBA::ORB_instance("mico-local-orb");
    CORBA::Object_var obj;
    MobileTerminal::AccessBridge_var new_ab;
    try {
	obj = orb->string_to_object(sid.c_str());
	new_ab = MobileTerminal::AccessBridge::_narrow(obj);
    } catch (...) {
	cerr << "Bad Access Bridge name: " << sid << endl;
	return;
    }
    MobileTerminal::HandoffCallback_var hoff;
    try {
	char name[256];
	sprintf(name, "file://%s/hoff.ref", getenv("PWD"));
	obj = orb->string_to_object(name);
	hoff = MobileTerminal::HandoffCallback::_narrow(obj);
    } catch (...) {
	cerr << "Could not find handoff callback, using NIL" << endl;
	hoff = MobileTerminal::HandoffCallback::_nil();
    }
    ab->start_handoff(terminal_id, new_ab, hoff);
}

int
main (int argc, char *argv[])
{
    MapCommand commands;

    commands['1'] = make_pair<string,CommFun_ptr>("Get initial services",
						  &admin_services);
    commands['2'] = make_pair<string,CommFun_ptr>("Query attached terminal",
						  &admin_attached);
    commands['3'] = make_pair<string,CommFun_ptr>("Get transport addresses",
						  &admin_addresses);
    commands['4'] = make_pair<string,CommFun_ptr>("Force handoff",
						  &admin_handoff);
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    CORBA::Object_var obj = orb->resolve_initial_references("NameService");
    CosNaming::NamingContext_var ns = CosNaming::NamingContext::_narrow(obj);

    char *ab_name;
    if (argc > 1)
	ab_name = argv[1];
    else
	ab_name = "AccessBridge";
    CosNaming::Name n;
    n.length(2);
    n[0].id = CORBA::string_dup("MobilitySupport");
    n[1].id = CORBA::string_dup(ab_name);
    try {
	obj = ns->resolve(n);
    } catch (CORBA::Exception &) {
	cerr << "Could not resolve Access Bridge " << ab_name << endl;
	exit(EXIT_FAILURE);
    }
    MobileTerminal::AccessBridge_var ab =
	MobileTerminal::AccessBridge::_narrow(obj);

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
	itr->second.second(cout, ab);
    }
    return 0;
}
