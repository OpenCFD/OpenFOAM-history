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

#ifndef _NamingClient_h_
#define _NamingClient_h_

#include <coss/CosNaming.h>

class NamingClient {
public:
  NamingClient (CosNaming::NamingContext_ptr nc);
  ~NamingClient ();

  void change_context (const char* s);
  void list_context (const char* s = NULL);
  void create_context (const char* s);
  void print_current_context ();
  void remove_name (const char* s);
  void bind (const char* s, CORBA::Object_ptr obj);
  void bind_context (const char *s, CosNaming::NamingContext_ptr nc);
  void show_url (const char *);
  void show_ior (const char *);

  CORBA::Object_ptr resolve_name (const char* s);

private:
  void create_name_from_string (const char* s, CosNaming::Name& name);
  void list_context (CosNaming::NamingContext_ptr nc);

  CosNaming::NamingContext_var root_nc, current_nc;
  CosNaming::NamingContextExt_var root_nce;
  CosNaming::Name current_path;
};
void printname(std::ostream &,const CosNaming::Name& name);
#endif
