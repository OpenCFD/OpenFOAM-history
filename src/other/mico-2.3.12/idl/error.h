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

#ifndef __ERROR_H__
#define __ERROR_H__

#include <CORBA.h>
#include <mico/template_impl.h>
#include "parsenode.h"

#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif


class IDLError
{
private:
  int    _err_num;
  std::string _msg;
public:
  IDLError();
  IDLError( int err_num, ... );
  IDLError( int err_num, ParseNode *n, ... );
  IDLError( const IDLError &err );
  IDLError& operator=( const IDLError &err );

  std::ostream& print (std::ostream &o) const
  {
    o << _msg;
    return o;
  }
};

static inline std::ostream& operator<<( std::ostream& o, const IDLError& err )
{
  return err.print (o);
}


#endif
