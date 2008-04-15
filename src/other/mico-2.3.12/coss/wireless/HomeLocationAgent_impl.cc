/*
 *  MIWCO --- Wireless CORBA support for MICO (OMG document formal/2004-04-02)
 *            Home Location Agent implementation
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

#include "HomeLocationAgent_impl.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else
#include <fstream.h>
#endif
#include <stdlib.h>
#include <time.h>

using namespace std;

// Implementation for interface HomeLocationAgent

HomeLocationAgent_impl::HomeLocationAgent_impl (CORBA::ORB_ptr orb,
						PortableServer::POA_ptr poa,
						CosNaming::NamingContext_ptr root,
						string prefix,
						string conffile)
{
    my_orb = orb;
    unsigned long length = prefix.length();

    CORBA::Octet *addr = mico_url_decode(prefix.c_str(), length);

    if (conffile[0] == '~') {
	char *s = getenv("HOME");
	string home = s ? s : "/tmp";
	conffile.replace(0, 1, home);
    }
    ifstream cnf(conffile.c_str());
    string line;
    while (getline(cnf, line)) {
	if (line.size() == 0 || line[0] == '#')
	    continue;
	size_t i = line.find('=');
	group_data data;
	CORBA::ULong len;
        CORBA::Octet *tmp = mico_url_decode(line.substr(0, i).c_str(), len);
	if (len >= 256)
	    len = 255;
	data.id.length(2 + length + len);
	if (length == 4) {
	    data.id[0] = 0x04;
	} else if (length == 16) {
	    data.id[0] = 0x06;
	} else {
	    data.id[0] = 0x01; // XXX: Should we throw an exception here?
	}
	CORBA::ULong l;
	for (l = 0; l < length; ++l) {
	    data.id[1 + l] = addr[l];
	}
	data.id[1 + length] = (CORBA::Octet)len;
	for (l = 0; l < len; ++l) {
	    data.id[2 + length + l] = tmp[l];
	}
	if (MICO::Logger::IsLogged(MICO::Logger::Info)) {
	    MICO::Logger::Stream(MICO::Logger::Info)
		<< "Adding terminal " << data.id << endl;
	}

	CORBA::string_free((char *)tmp);
	if (i != string::npos) {
	    try {
		CORBA::Object_var obj =
		    my_orb->string_to_object(line.substr(i+1).c_str());
		data.current_wab = MobileTerminal::AccessBridge::_narrow(obj);
	    } catch (const CORBA::Exception &) {
		data.current_wab = MobileTerminal::AccessBridge::_nil();
	    }
	} else {
	    data.current_wab = MobileTerminal::AccessBridge::_nil();
	}
	grid_map[data.id] = data;
    }

    root_context = CosNaming::NamingContext::_duplicate(root);
}

HomeLocationAgent_impl::~HomeLocationAgent_impl ()
{
    for (grid_data::iterator i = grid_map.begin(); i != grid_map.end(); ++i)
	CORBA::release(i->second.current_wab);
    CORBA::release(root_context);
}

void
HomeLocationAgent_impl::update_location
(const MobileTerminal::TerminalId& terminal_id,
 MobileTerminal::AccessBridge_ptr new_access_bridge)
    throw(CORBA::SystemException, MobileTerminal::UnknownTerminalId,
	  MobileTerminal::IllegalTargetBridge)
{
    grid_data::iterator ogd = grid_map.find(terminal_id);
    if (ogd == grid_map.end()) {
	throw MobileTerminal::UnknownTerminalId();
    }

    group_data data = ogd->second;

    if (MICO::Logger::IsLogged(MICO::Logger::Info)) {
	CORBA::String_var ab_str = my_orb->object_to_string(new_access_bridge);
	MICO::Logger::Stream(MICO::Logger::Info)
	    << "Terminal " << terminal_id << " moved to Access Bridge "
	    << ab_str << endl;
    }

    CORBA::release(ogd->second.current_wab);
    ogd->second.current_wab =
	MobileTerminal::AccessBridge::_duplicate(new_access_bridge);
}

void
HomeLocationAgent_impl::query_location
(const MobileTerminal::TerminalId& terminal_id,
 MobileTerminal::AccessBridge_out current_access_bridge)
    throw(CORBA::SystemException, MobileTerminal::UnknownTerminalId,
	  MobileTerminal::UnknownTerminalLocation)
{
    if (MICO::Logger::IsLogged(MICO::Logger::Info)) {
	MICO::Logger::Stream(MICO::Logger::Info)
	    << "Received query for location of terminal "
	    << terminal_id << endl;
    }

    grid_data::const_iterator og = grid_map.find(terminal_id);
    if (og == grid_map.end()) {
	throw MobileTerminal::UnknownTerminalId();
    }

    group_data data = og->second;
    if (CORBA::is_nil(data.current_wab)) {
	throw MobileTerminal::UnknownTerminalLocation();
    }

    current_access_bridge =
	MobileTerminal::AccessBridge::_duplicate(data.current_wab);
}


CORBA::Boolean
HomeLocationAgent_impl::deregister_terminal
(const MobileTerminal::TerminalId& terminal_id,
 MobileTerminal::AccessBridge_ptr old_access_bridge)
    throw(CORBA::SystemException, MobileTerminal::UnknownTerminalId)
{
    if (MICO::Logger::IsLogged(MICO::Logger::Info)) {
	MICO::Logger::Stream(MICO::Logger::Info)
	    << "Received deregistration of terminal "
	    << terminal_id << endl;
    }

    grid_data::iterator og = grid_map.find(terminal_id);
    if (og == grid_map.end()) {
	throw MobileTerminal::UnknownTerminalId();
    }

    if (old_access_bridge->_is_equivalent(og->second.current_wab)) {
	CORBA::release(og->second.current_wab);
	og->second.current_wab = MobileTerminal::AccessBridge::_nil();
	return TRUE;
    }
    return FALSE;
}


MobileTerminal::ObjectIdList*
HomeLocationAgent_impl::list_initial_services ()
    throw(CORBA::SystemException)
{
    MobileTerminal::ObjectIdList* retval = new CORBA::ORB::ObjectIdList;
    if (!CORBA::is_nil(root_context)) {
	retval->length(1);
	(*retval)[0] = CORBA::string_dup("NameService");
    }
    return retval;
}


CORBA::Object_ptr
HomeLocationAgent_impl::resolve_initial_references (const char* identifier)
    throw(CORBA::SystemException, MobileTerminal::InvalidName)
{
    if (MICO::Logger::IsLogged(MICO::Logger::Info)) {
	MICO::Logger::Stream(MICO::Logger::Info)
	    << "Resolving initial reference " << identifier << endl;
    }
    CORBA::Object_ptr retval;

    if (CORBA::is_nil(root_context)
	|| strcmp(identifier, "NameService") != 0) {
	throw MobileTerminal::InvalidName();
    } else {
	retval = CORBA::Object::_duplicate(root_context);
    }
    return retval; 
}
