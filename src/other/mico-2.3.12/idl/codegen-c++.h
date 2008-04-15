/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
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
 *  For more information, visit the MICO Home Page at
 *  http://www.mico.org/
 */

#ifndef __CODEGENCPP_H__
#define __CODEGENCPP_H__

#include "codegen-c++-common.h"
#include "codegen-c++-stub.h"
#include "codegen-c++-skel.h"
#include "codegen-c++-impl.h"


class CodeGenCPP : virtual public CodeGenCPPCommon,
                   virtual public CodeGenCPPStub,
                   virtual public CodeGenCPPSkel,
		   virtual public CodeGenCPPImpl
{
public:
  CodeGenCPP( DB &db, IDLParam &params, CORBA::Container_ptr con );
  void emit( std::string &fnbase );
};


#endif
