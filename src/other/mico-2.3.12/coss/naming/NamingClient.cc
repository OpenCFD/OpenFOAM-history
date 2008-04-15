/*
 *  COSS Naming Service administration tool for MICO
 *  Copyright (C) 1997 Kai-Uwe Sattler
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
#include <mico/impl.h>
#include "NamingClient.h"


using namespace std;

#if 0
#define TRY try {
#define CATCH(__name) \
  }								\
  catch (CosNaming::NamingContext::NotFound_catch &exc) {	\
    cerr << "NotFound: " << __name << endl;			\
    switch(exc.why)                                  	\
    {                                               	\
    case CosNaming::NamingContext::missing_node:	\
	cerr << "Missing Node";                       	\
	break;                                          \
    case CosNaming::NamingContext::not_context:      	\
	cerr << "Not Context" ;                    	\
        break;                                  	\
    case CosNaming::NamingContext::not_object:		\
        cerr << "Not Object";				\
        break;						\
    }							\
    cerr << " (Rest of name: ";				\
    printname(cerr,exc.rest_of_name);			\
    cerr << ')' << endl;				\
    cerr << endl;                                               \
  }								\
  catch (CosNaming::NamingContext::CannotProceed_catch &exc) {	\
    cerr << "CannotProceed: " << __name << endl;	       	\
  }								\
  catch (CosNaming::NamingContext::InvalidName_catch &exc) {	\
    cerr << "InvalidName: " << __name << endl;			\
  }								\
  catch (CosNaming::NamingContext::AlreadyBound_catch &exc) {	\
    cerr << "AlreadyBound: " << __name << endl;			\
  }
#else
#define TRY {
#define CATCH(__name) }
#endif

NamingClient::NamingClient (CosNaming::NamingContext_ptr nc)
{
  root_nc = CosNaming::NamingContext::_duplicate (nc);
  root_nce = CosNaming::NamingContextExt::_narrow (nc);
  current_nc = CosNaming::NamingContext::_duplicate (nc);
  current_path.length (0);
}

NamingClient::~NamingClient ()
{
}

void NamingClient::change_context (const char* s)
{
  CosNaming::Name name;
  CosNaming::NamingContext_var nctx = CosNaming::NamingContext::_nil();
  CORBA::Object_var obj;

  TRY;
  obj = resolve_name (s);
  nctx = CosNaming::NamingContext::_narrow (obj);

  if (!CORBA::is_nil (nctx)) {
    create_name_from_string (s, name);
    current_path = name;
    current_nc = nctx;
  }
  CATCH(s);
}

void NamingClient::list_context (const char* s)
{
  if (s == NULL) {
    assert (! CORBA::is_nil (current_nc));
    list_context (current_nc);
  }
  else {
    CosNaming::NamingContext_var nctx = CosNaming::NamingContext::_nil();
    CORBA::Object_var obj;

    TRY;
    obj = resolve_name (s);
    nctx = CosNaming::NamingContext::_narrow (obj);

    if (!CORBA::is_nil (nctx))
      list_context (nctx);
    else {
      CORBA::ORB_var orb = CORBA::ORB_instance("mico-local-orb");
      cout << orb->object_to_string (obj) << endl;
    }
    CATCH(s);
  }
}

void NamingClient::create_context (const char* s)
{
  CosNaming::NamingContext_var nc;
  CosNaming::Name name;

  TRY;
  create_name_from_string (s, name);
  nc = root_nc->bind_new_context (name);
  CATCH(s);
}

void NamingClient::print_current_context ()
{
  if (!CORBA::is_nil (root_nce)) {
    CORBA::String_var path = root_nce->to_string (current_path);
    cout << "/" << path.in() << endl;
  }
  else {
    string path ("/");
    for (CORBA::ULong i = 0; i < current_path.length (); i++) {
      path += current_path[i].id;
      if (i < current_path.length () - 1)
	path += "/";
    }
    cout << path.c_str () << endl;
  }
}

void NamingClient::remove_name (const char* s)
{
  CosNaming::Name name;
  TRY;
  create_name_from_string (s, name);
  root_nc->unbind (name);
  CATCH(s);
}

void NamingClient::bind (const char* s, CORBA::Object_ptr obj)
{
  CosNaming::Name name;
  TRY;
  create_name_from_string (s, name);
  root_nc->bind (name, obj);
  CATCH(s);
}

void NamingClient::bind_context (const char *s,CosNaming::NamingContext_ptr nc)
{
  CosNaming::Name name;
  TRY;
  create_name_from_string (s, name);

  root_nc->bind_context(name,nc);
  CATCH(s);
}

void NamingClient::create_name_from_string (const char* s, 
					    CosNaming::Name& name)
{
  string str (s);

  if (str == ".") {
      name = current_path;
      return;
  } else if (str == "..") {
    if (current_path.length () <= 1) {
      name.length (0);
      return;
    }
    name.length (current_path.length () - 1);
    for (CORBA::ULong i = 0; i < current_path.length () - 1; i++)
      name[i] = current_path[i];
  }
  else if (str == "/") {
    name.length (0);
  }
  else if (!CORBA::is_nil (root_nce)) {
    if (str[0] == '/') {
      CosNaming::Name_var temp;
      temp = root_nce->to_name(s + 1); 
      name = *temp;
    }
    else {
      CosNaming::Name_var temp;
      temp = root_nce->to_name(s);
      name.length(current_path.length() + temp->length());
      for (CORBA::ULong i = 0; i < current_path.length (); i++)
	name[i] = current_path[i];
      for (CORBA::ULong j = current_path.length ();
	   j < name.length ();
	   j++)
	name[j] = (*temp)[j - current_path.length ()];
    }
  }
  else {
    // not talking to an INS
    size_t pos = 0, p = 0;
    CORBA::ULong num = 0;
    if (str[0] == '/') {
      pos = 1;
    } else {
      name = current_path;
      num = name.length();
    }
    do {
      p = str.find ('/', pos);
      num++;

      string sub;
      if ((CORBA::Long)p < 0)
        sub = str.substr (pos);
      else
        sub = str.substr (pos, p - pos);
      pos = p + 1;
      name.length (num);
      name[num - 1].id = CORBA::string_dup (sub.c_str ());
      name[num - 1].kind = CORBA::string_dup ("");
    } while ((CORBA::Long)p >= 0);
  }
}



void NamingClient::list_context (CosNaming::NamingContext_ptr nc)
{
  CosNaming::BindingList_var blist;
  CosNaming::BindingIterator_var biter;
  CosNaming::Binding_var b;
  
  nc->list (0, blist, biter);

  if (CORBA::is_nil (biter)) {
    return;
  }

  while (biter->next_one (b.out())) {
    cout << "\t";
    if (!CORBA::is_nil (root_nce)) {
      CORBA::String_var name = root_nce->to_string (b->binding_name);
      cout << name.in();
    }
    else {
      for (CORBA::ULong k = 0; k < b->binding_name.length (); k++)
	cout << b->binding_name[k].id.in() << " ";
    }
    cout << endl;
  }
}

CORBA::Object_ptr NamingClient::resolve_name (const char* s)
{
  CosNaming::Name name;
  CORBA::Object_ptr result;

  create_name_from_string (s, name);

  if (name.length() == 0)
    result = CosNaming::NamingContext::_duplicate (root_nc);
  else
    result = root_nc->resolve (name);

  return result;
}

void
NamingClient::show_url (const char * s)
{
  if (CORBA::is_nil (root_nce)) {
    cerr << "we aren't talking to an Interoperable Naming Service" << endl;
    return;
  }

  /*
   * Determine address of name service
   */

  const CORBA::Address * addr =
    root_nce->_ior()->addr (CORBA::IORProfile::TAG_INTERNET_IOP);

  if (!addr) {
    cerr << "we aren't talking to the INS using IIOP" << endl;
    return;
  }

  char tmp[64];
  const MICO::InetAddress * ina = (const MICO::InetAddress *) addr;
  sprintf (tmp, "%lu", (unsigned long) ina->port());

  string straddr = ":";
  straddr += ina->host();
  straddr += ':';
  straddr += tmp;

  CosNaming::Name name;
  CORBA::String_var r1, r2;

  TRY;
  create_name_from_string (s, name);
  r1 = root_nce->to_string (name);
  r2 = root_nce->to_url (straddr.c_str(), r1.in());
  cout << r2.in() << endl;
  CATCH(s);
}

void 
NamingClient::show_ior (const char* s)
{
  CORBA::ORB_var orb = CORBA::ORB_instance("mico-local-orb");
  if (s == NULL) {
    assert (! CORBA::is_nil (current_nc));
    cout << orb->object_to_string (current_nc) << endl;
  }
  else {
    TRY;
    CORBA::Object_var obj = resolve_name(s);
    cout << orb->object_to_string (obj) << endl;
    CATCH(s);
  }
}

void printname(ostream & str,const CosNaming::Name& name)
{
  unsigned int i;
  for(i=0;i<name.length();i++)
  {
      if(i<0) str << '/';
      str << name[i].id.in();
      if(strlen(name[i].kind.in())>0) str << '.' << name[i].kind.in();
  }
}









