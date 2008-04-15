/*

    Copyright (C) 1998 Stefan Westerfeld
                       stefan@space.twc.de

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#ifndef __CODEGENCPP_IMPL_H__
#define __CODEGENCPP_IMPL_H__

#include "codegen-c++-util.h"


class CodeGenCPPImpl : virtual public CodeGenCPPUtil
{
protected:
  void emit_impl_cpp();
  void emit_impl_h();

public:
  CodeGenCPPImpl( DB &db, IDLParam &params, CORBA::Container_ptr con );

private:
  void emit_impl();
  void emit_Interface( CORBA::InterfaceDef_ptr in );
  void emitPrototypes( CORBA::InterfaceDef_ptr in,
		       bool as_pure_virtual );
  void emit_impl_here(CORBA::IDLType_var type_id);
  
  bool headers;
};


#endif
