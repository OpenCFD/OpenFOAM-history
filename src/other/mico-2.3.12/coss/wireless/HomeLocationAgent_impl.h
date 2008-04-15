// -*- c++ -*-
/*
 *  MIWCO --- Wireless CORBA support for MICO (OMG document formal/2004-04-02)
 *            Home Location Agent interface
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

#ifndef __HOME_LOCATION_AGENT_IMPL_H__
#define __HOME_LOCATION_AGENT_IMPL_H__

#include <coss/MobileTerminal.h>
#include <coss/CosNaming.h>
//#include <mico/intercept.h>
#include <mico/impl.h>

// Implementation for interface HomeLocationAgent
class HomeLocationAgent_impl
    : virtual public POA_MobileTerminal::HomeLocationAgent
{
public:

    void update_location (const MobileTerminal::TerminalId& terminal_id,
			  MobileTerminal::AccessBridge_ptr new_access_bridge)
	throw(CORBA::SystemException, MobileTerminal::UnknownTerminalId,
	      MobileTerminal::IllegalTargetBridge);

    void
    query_location (const MobileTerminal::TerminalId& terminal_id,
		    MobileTerminal::AccessBridge_out current_access_bridge)
	throw(CORBA::SystemException, MobileTerminal::UnknownTerminalId,
	      MobileTerminal::UnknownTerminalLocation);

    CORBA::Boolean
    deregister_terminal (const MobileTerminal::TerminalId& terminal_id,
			 MobileTerminal::AccessBridge_ptr old_access_bridge)
	throw(CORBA::SystemException, MobileTerminal::UnknownTerminalId);

    MobileTerminal::ObjectIdList *list_initial_services ()
	throw(CORBA::SystemException);

    CORBA::Object_ptr resolve_initial_references (const char* identifier)
	throw(CORBA::SystemException, MobileTerminal::InvalidName);

    void debug_dump ()
	throw(CORBA::SystemException);

    HomeLocationAgent_impl (CORBA::ORB_ptr orb, PortableServer::POA_ptr poa,
			    CosNaming::NamingContext_ptr root,
			    std::string prefix = "", std::string conffile = "~/.hlarc");

    ~HomeLocationAgent_impl ();

    struct group_data {
	MobileTerminal::TerminalId id;
	MobileTerminal::AccessBridge_ptr current_wab;
    };

    struct ic :
	std::binary_function<const MobileTerminal::TerminalId,
	                     const MobileTerminal::TerminalId,
	                     bool>
    {
	bool operator() (const MobileTerminal::TerminalId& id1,
			 const MobileTerminal::TerminalId& id2) const
	    {
		if (id1.length() != id2.length())
		    return id1.length() < id2.length();
		return mico_key_compare(id1.get_buffer(), id2.get_buffer(),
					id2.length()) < 0;
	    }
    };
    typedef std::map<MobileTerminal::TerminalId, group_data, ic> grid_data;

    grid_data grid_map;

  private:

    CORBA::ORB_ptr my_orb;

    CosNaming::NamingContext_ptr root_context;

    HomeLocationAgent_impl (const HomeLocationAgent_impl &);
    void operator= (const HomeLocationAgent_impl &);
};

#include <mico/util.h>

namespace {

    std::ostream&
	operator<< (std::ostream& s, const std::vector<CORBA::Octet>& v)
	{
	    for (unsigned int i = 0; i < v.size(); ++i) {
		// US-ASCII alphanumeric characters are not escaped
		if (isalnum((unsigned char)v[i])) {
		    s << v[i];
		    continue;
		}
		// Also not escaped
		switch (v[i]) {
		case ';':
		case '/':
		case ':':
		case '?':
		case '@':
		case '&':
		case '=':
		case '+':
		case '$':
		case ',':
		case '-':
		case '.':
		case '!':
		case '~':
		case '*':
		case '\'':
		case '(':
		case ')':
		    s << v[i];
		    break;

		default: {
			CORBA::Octet x = v[i];
			s << '%';
			s << (char) mico_to_xdigit (x>>4);
			s << (char) mico_to_xdigit (x&15);
		    }
		}
	    }
	    return s;
	}

    std::ostream&
	operator<< (std::ostream& s, const MobileTerminal::TerminalId& c)
	{
	    char *str = mico_url_encode(c.get_buffer(), c.length());
	    s << str;
	    CORBA::string_free(str);
	    return s;
	}

}

#endif
