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

#ifdef FAST_PCH
#include "idl_pch.h"
#endif // FAST_PCH
#ifdef __COMO__
#pragma hdrstop
#endif // __COMO__

#ifndef FAST_PCH

#include <CORBA.h>
#include <string>
#include <stdlib.h>
#include "prepro.h"
#include <mico/template_impl.h>

#endif // FAST_PCH


using namespace std;

char  *toplevelFile = 0;
char  *idl_lexFile = 0;
int    idl_line_no = 0;


/*
 * The following function parses a preprocessor directive.
 */
void preprocessor_directive( const char *str )
{
  string s = str;
  string num;
  string name;
  int i, j;

  if( s[ 0 ] != '#' )
    // this is something we don't understand
    return;
 
  if (s.find ("#line ") == 0)
    s = s.substr (6);
  else if (s[1] == ' ') 
    s = s.substr( 2 );
  else
    return;

  // Get line number
  i = s.find_first_of( "0123456789" );
  assert( i >= 0 );
  j = s.find_first_not_of( "0123456789", i );
  assert( j > i );
  
  num = s.substr( i, j-i );
  idl_line_no = atoi( num.c_str() );

  // strip off line number
  s = s.substr( j );

  // Get file name
  i = s.find( '"' );
  if (i >= 0) {
    ++i;
    j = s.find( '"', i );
    assert( j >= i );

    name = s.substr( i, j-i );
    idl_lexFile = strdup( name.c_str() );
  }
}
