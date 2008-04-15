//
//  MICOsec --- a free CORBA Security implementation
//  Copyright (C) 2000 ObjectSecurity Ltd.
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Library General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Library General Public License for more details.
//
//  You should have received a copy of the GNU Library General Public
//  License along with this library; if not, write to the Free
//  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

//  Send comments and/or bug reports to:
//                 micosec@objectsecurity.com
//

#include <mico/security/AuditOptions.h>


using namespace std;

CORBA::Boolean
AuditOptions::parse(CORBA::ORB_ptr orb, int& argc, char* argv[])
{
    MICOGetOpt::OptMap opts;
    opts["-AccessControl"]  = "arg-expected"; 		// access control off/on

    MICOGetOpt opt_parser (opts);
    CORBA::Boolean r = opt_parser.parse (orb->rcfile(), TRUE);
    if (!r)
	return FALSE;
    r = opt_parser.parse (argc, argv, TRUE);
    if (!r)
	return FALSE;
    const MICOGetOpt::OptVec& o = opt_parser.opts();
  
    for (MICOGetOpt::OptVec::const_iterator i = o.begin(); i != o.end(); ++i) {
	string arg = (*i).first;
	string val = (*i).second;
	options_[arg] = val;
    }
    return TRUE;
}

const char *
AuditOptions::operator[](const char* opt)
{
    map<string,string,less<string> >::iterator it = options_.find (opt);
    if (it == options_.end()) {
	return NULL;
    }
    return (*it).second.c_str();
}
