/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2003 by The Mico Team
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


#ifdef FAST_PCH
#include "idl_pch.h"
#endif // FAST_PCH
#ifdef __COMO__
#pragma hdrstop
#endif // __COMO__

#ifndef FAST_PCH

#include <stdio.h>
#include <stdarg.h>
#include "error.h"

#endif // FAST_PCH

static const char *ErrorMsg[] = {
  /*   0 */ "No Error",
	    /*   1 */ "Not all forward declarations have been defined",
	    /*   2 */ "scoped name '%s' not defined",
	    /*   3 */ "scoped name '%s' is not a typedef",
	    /*   4 */ "constant definition '%s' not of proper type",
	    /*   5 */ "union switch type spec not of proper type",
	    /*   6 */ "scoped name '%s' is not an exception",
	    /*   7 */ "scoped name '%s' is not an IDLType",
	    /*   8 */ "name '%s' is not a base interface",
	    /*   9 */ "base interface '%s' is not yet defined",
	    /*  10 */ "can not coerce '%s' to '%s'",
	    /*  11 */ "'%s' is not member of enum '%s'",
	    /*  12 */ "identifier '%s' defined twice in this scope",
	    /*  13 */ "repository id for identifier '%s' already used",
	    /*  14 */ "union '%s' has duplicate labels",
	    /*  15 */ "reference to identifier '%s' is ambiguous",
	    /*  16 */ "can not overload operation or attribute '%s'",
	    /*  17 */ "bad kind of recursion in constructed type '%s'",
	    /*  18 */ "oneway operation '%s' can not have result, "
                      "out-parameters or raises-expressions",
	    /*  19 */ "type '%s' has identical members",
	    /*  20 */ "case label type is not an enum",
	    /*  21 */ "no value for default case from discriminator type "
	              "available",
	    /*  22 */ "bad #pragma directive",
	    /*  23 */ "identifier '%s' is not an interface",
	    /*  24 */ "scoped name '%s' is not a constant",
	    /*  25 */ "interface '%s' can not inherit from itself",
	    /*  26 */ "custom value '%s' cannot be truncatable",
	    /*  27 */ "abstract value '%s' cannot be truncatable",
	    /*  28 */ "abstract value '%s' cannot inherit from "
	              "concrete value '%s'",
	    /*  29 */ "cannot truncate '%s' to abstract value '%s'",
            /*  30 */ "illegal interface inheritance: '%s' cannot inherit"
	              "from '%s'",
            /*  31 */ "internal error",
            /*  32 */ "unknown preprocessor directive '%s'",
            /*  33 */ "interface defined with different repository id '%s'",
	    /*  34 */ "identifier '%s' is not a value",
            /*  35 */ "value defined with different repository id '%s'",
	    /*  36 */ "cannot import '%s': no Interface Repository",
	    /*  37 */ "cannot import '%s': not found in Interface Repository",
	    /*  38 */ "cannot import '%s': no entry in Repository and no file",
	    /*  39 */ "cannot import '%s': non-container object",
	    /*  40 */ "import statement not at toplevel",
            /*  41 */ "identifier '%s' collides with IDL keyword '%s'",
            /*     */ ""
};


IDLError::IDLError()
{
  _err_num = 0;
  _msg = ErrorMsg[ 0 ];
}

IDLError::IDLError( int err_num, ... )
{
  char    buf[ 500 ];
  va_list p;
  
  va_start( p, err_num );
  _err_num = err_num;
  vsprintf( buf, ErrorMsg[ err_num ], p );
  _msg = "Error: ";
  _msg += buf;
  va_end( p );
}

IDLError::IDLError( int err_num, ParseNode *n, ... )
{
  char    buf[ 500 ];
  char    buf_line_no[ 20 ];
  va_list p;
  
  va_start( p, n );
  _err_num = err_num;
  vsprintf( buf, ErrorMsg[ err_num ], p );
  _msg = n->getFname();
  sprintf( buf_line_no, ":%ld: ", n->getLineNo() );
  _msg += buf_line_no;
  _msg += buf;
  va_end( p );
}

IDLError::IDLError( const IDLError &err )
{
  _err_num = err._err_num;
  _msg = err._msg;
}

IDLError& IDLError::operator=( const IDLError &err )
{
  _err_num = err._err_num;
  _msg = err._msg;
  return *this;
}
