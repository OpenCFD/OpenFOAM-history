/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2005 by The Mico Team
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
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <sstream>
#else
#include <strstream.h>
#endif
#include <mico/util.h>
#include "codegen.h"

#endif // FAST_PCH


using namespace std;

//-- OutputStream -------------------------------------------------------

OutputStream::OutputStream( int indent )
{
  _indent = indent;
  _current_indent = 0;
  _bofl = true;
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
  _ostr = new ostringstream;
#else
  _ostr = new ostrstream;
#endif
}


OutputStream::~OutputStream ()
{
  delete _ostr;
}


void OutputStream::enterLevel()
{
  _current_indent += _indent;
}


void OutputStream::leaveLevel()
{
  assert( _current_indent >= 0 );
  _current_indent -= _indent;
}


OutputStream &OutputStream::operator<<( const char *s )
{
  checkBofL();
  *_ostr << s;
  return *this;
}


OutputStream &OutputStream::operator<<( const string &str )
{
  checkBofL();
  *_ostr << str;
  return *this;
}

OutputStream &OutputStream::operator<<( const CORBA::WChar *s )
{
  checkBofL();
  /*
   * here we assume that the string has not been interpreted, i.e.
   * escape sequences like \\ have not been evaluated into \ etc.
   * therefore we dont have to escape special charaters, we just print
   * them out.
   * we cannot just print every character as a hex code, because this
   * would "destroy" the uninterpreted escape sequences in the original
   * string.
   */
  for( size_t i = 0; i < xwcslen( s ); i++ ) {
      CORBA::WChar wc = s[i];
      if (wc >= L' ' && wc <= L'z')
	  *this << (CORBA::Char)wc;
      else {
	  char buf[ 10 ];
	  /*
	   * print in octal format \ooo to make sure the following
	   * character gets not interpreted as another octal digit
	   * (the number of octal digits is limited to 3).
	   * \xhh does not work, because the number of hex digits
	   * is not limited as per C++ spec.
	   */
	  sprintf( buf, "\\03%o", (int) wc & 0xff);
	  *this << buf;
      }
  }
  return *this;
}

#if SIZEOF_LONG == 4
OutputStream &OutputStream::operator<<( int i )
{
  checkBofL();
  *_ostr << i;
  return *this;
}
#endif


OutputStream &OutputStream::operator<<( CORBA::LongDouble ld )
{
  checkBofL();
#if (defined(__hpux) && !defined(__GNUG__)) || (defined(__linux__) && defined(__COMO__) && defined(_STLPORT_VERSION))
  // XXX
  *_ostr << double(ld);
#else
  *_ostr << ld;
#endif
  return *this;
}



OutputStream &OutputStream::operator<<( CORBA::Float f )
{
  checkBofL();
  *_ostr << f;
  return *this;
}


OutputStream &OutputStream::operator<<( CORBA::Double d )
{
  checkBofL();
  *_ostr << d;
  return *this;
}


OutputStream &OutputStream::operator<<( CORBA::LongLong ll )
{
  checkBofL();
  *_ostr << ll;
  return *this;
}


OutputStream &OutputStream::operator<<( CORBA::ULongLong ull )
{
  checkBofL();
  *_ostr << ull;
  return *this;
}


OutputStream &OutputStream::operator<<( CORBA::Long l )
{
  checkBofL();
  *_ostr << l;
  return *this;
}


OutputStream &OutputStream::operator<<( CORBA::ULong ul )
{
  checkBofL();
  *_ostr << ul;
  return *this;
}


OutputStream &OutputStream::operator<<( CORBA::Short s )
{
  checkBofL();
  *_ostr << s;
  return *this;
}


OutputStream &OutputStream::operator<<( CORBA::UShort us )
{
  checkBofL();
  *_ostr << us;
  return *this;
}


OutputStream &OutputStream::operator<<( CORBA::Char c )
{
  checkBofL();
  *_ostr << c;
  return *this;
}


#if defined(_WIN32) && defined(_MSC_VER)
OutputStream &OutputStream::operator<<( MICO_WCharWrapper c )
#else
OutputStream &OutputStream::operator<<( CORBA::WChar c )
#endif
{
  checkBofL();
  *_ostr << (CORBA::WChar) c;
  return *this;
}


OutputStream &OutputStream::operator<<( const FixedBase &fb )
{
  checkBofL();
  fb.write( *_ostr );
  return *this;
}


OutputStream &OutputStream::operator<<( const CORBA::Any *a )
{
  CORBA::TypeCode_var tc = a->type();

  while (tc->kind() == CORBA::tk_alias) {
    tc = tc->content_type ();
  }

  switch( tc->kind() ) {
  case CORBA::tk_longdouble:
    {
      CORBA::LongDouble ld;
      *a >>= ld;
      *this << ld;
      break;
    }
  case CORBA::tk_float: 
    {
      CORBA::Float f;
      *a >>= f;
      *this << f;
      break;
    }
  case CORBA::tk_double:
    {
      CORBA::Double d;
      *a >>= d;
      *this << d;
      break;
    }
  case CORBA::tk_longlong:
    {
      CORBA::LongLong ll;
      *a >>= ll;
      *this << ll;
      break;
    }
  case CORBA::tk_ulonglong:
    {
      CORBA::ULongLong ull;
      *a >>= ull;
      *this << ull;
      break;
    }
  case CORBA::tk_long:
    {
      CORBA::Long l;
      *a >>= l;
      *this << l;
      break;
    }
  case CORBA::tk_short:
    {
      CORBA::Short s;
      *a >>= s;
      *this << s;
      break;
    }
  case CORBA::tk_ulong:
    {
      CORBA::ULong ul;
      *a >>= ul;
      *this << ul;
      break;
    }
  case CORBA::tk_ushort:
    {
      CORBA::UShort us;
      *a >>= us;
      *this << us;
      break;
    }
  case CORBA::tk_octet:
    {
      CORBA::Octet us;
      *a >>= CORBA::Any::to_octet (us);
      *this << (CORBA::UShort) us;
      break;
    }
  case CORBA::tk_boolean:
    {
      CORBA::Boolean b;
      *a >>= CORBA::Any::to_boolean( b );
      if( b )
	*this << "TRUE";
      else
	*this << "FALSE";
      break;
    }
  case CORBA::tk_string:
    {
      const char *s;
      *a >>= CORBA::Any::to_string( s, tc->length() );
      *this << "\"";
      while (*s) {
	unsigned char c = *s;
	if (c == '"' || c == '\\') {
	  *this << "\\" << *s;
	}
	else if ((c >= 0040 && c <= 0176) ||
		 (c >= 0220 && c != 0231 && c != 0234)) {
	  *this << *s;
	}
	else if (c == '\r') {
	  *this << "\\r";
	}
	else if (c == '\n') {
	  *this << "\\n";
	}
	else {
	  char buf[ 10 ];
	  sprintf( buf, "\\%03o", (int) c );
	  *this << buf;
	}
	s++;
      }
      *this << "\"";
      break;
    }
  case CORBA::tk_wstring:
    {
      const CORBA::WChar *s;
      bool hex=false;
      *a >>= CORBA::Any::to_wstring( s, tc->length() );
      *this << "\"";
      while (*s) {
	if (*s == (CORBA::WChar) '"' || *s == (CORBA::WChar) '\\') {
	  *this << "\\" << (char) *s;
	  hex=false;
	}
	else if (((*s >= (CORBA::WChar) '0' && *s <= (CORBA::WChar) '9') ||
		  (*s >= (CORBA::WChar) 'a' && *s <= (CORBA::WChar) 'f') ||
		  (*s >= (CORBA::WChar) 'A' && *s <= (CORBA::WChar) 'F')) &&
		 hex) {
	  /*
	   * must not print hex character immediately after a hex sequence,
	   * else it will be scanned part of the hex sequence
	   */
	  char buf[ 10 ];
	  sprintf( buf, "\\x%04x", (unsigned int) *s );
	  *this << buf;
	  hex=true;
	}
	else if ((*s >= 0040 && *s <= 0176) ||
		 (*s >= 0220 && *s <= 0377 && *s != 0231 && *s != 0234)) {
	  *this << (char) *s;
	  hex=false;
	}
	else if (*s == (CORBA::WChar) '\r') {
	  *this << "\\r";
	  hex=false;
	}
	else if (*s == (CORBA::WChar) '\n') {
	  *this << "\\n";
	  hex=false;
	}
	else {
	  char buf[ 10 ];
	  sprintf( buf, "\\x%04x", (unsigned int) *s );
	  *this << buf;
	  hex=true;
	}
	s++;
      }
      *this << "\"";
      
      break;
    }
  case CORBA::tk_char:
    {
      CORBA::Char c;
      *a >>= CORBA::Any::to_char( c );
      if( c == '\'' || c == '\\')
	*this << "'\\" << c << "'";
      else if( c < ' ' || c > 'z' ) {
	char buf[ 10 ];
	sprintf( buf, "'\\x%02x'", (int) (unsigned char) c );
	*this << buf;
      }	else
	*this << "'" << c << "'";
      break;
    }
  case CORBA::tk_wchar:
    {
      CORBA::WChar c;
      *a >>= CORBA::Any::to_wchar( c );
      if( c == L'\'' || c == L'\\')
	*this << "'\\" << (CORBA::Char)c << "'";
      else if( c < L' ' || c > L'z' ) {
	char buf[ 10 ];
	sprintf( buf, "'\\u%04x'", (unsigned int) c);
	*this << buf;
      }	else
	*this << "'" << (CORBA::Char)c << "'";
      break;
    }
  case CORBA::tk_fixed:
    {
      FixedBase::FixedValue_var fv;
      FixedBase f( tc->fixed_digits(), tc->fixed_scale() );
      *a >>= CORBA::Any::to_fixed( f, tc->fixed_digits(),
				   tc->fixed_scale() );
      *this << f;
      break;
    }
  case CORBA::tk_enum:
    {
      CORBA::ULong i;
      a->enum_get (i);
      *this << tc->member_name (i);
      break;
    }
  default:
    cout << tc->kind() << endl;
    assert( 0 );
  }
  return *this;
}



OutputStream &OutputStream::operator<<( endlnfp e )
{
#if defined(__SUNPRO_CC) || defined(_STLPORT_VERSION) || _CPPLIB_VER >= 400 || defined(__DECCXX) || (!defined(__GNUC__) && defined(__sgi)) || defined(CRAY)
  // XXX
  // kcg: this is just hack which needs to be investigated. I've added
  // stlport and cpplib defines. sunpro is original.
  if (1) {
#else
  if( e == (endlnfp) endl ) {
#endif
    *_ostr << endl;
    _bofl = true;
  }
  return *this;
}


OutputStream &OutputStream::operator<<( OutputFormatting f )
{
  if( f == indent )
    enterLevel();
  if( f == exdent )
    leaveLevel();
  return *this;
}


void OutputStream::checkBofL()
{
  if( _bofl ) {
    for( int i = 0; i < _current_indent; i++ )
      *_ostr << " ";
  }
  _bofl = false;
}


//-- Output -------------------------------------------------------------

Output::Output()
{
  _ostr = NULL;
  for( int i = 0; i < MaxStreams; i++ )
    _streams[ i ] = NULL;
}

Output::~Output()
{
  free_streams();
}

void Output::free_streams()
{
  for( int i = 0; i < MaxStreams; i++ ) {
    if( _streams[ i ] == NULL )
      continue;
    if( _ostr != NULL ) {
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
      *_ostr << _streams[ i ]->_ostr->str();
#else
      *_streams[ i ]->_ostr << ends;
      *_ostr << _streams[ i ]->_ostr->str();
      _streams[ i ]->_ostr->rdbuf()->freeze( 0 );
#endif
    }
    delete _streams[ i ];
    _streams[ i ] = NULL;
  }
}

void Output::start_output( ostream &o, int indent )
{
  free_streams();
  _ostr = &o;
  for( int i = 0; i < MaxStreams; i++ )
    _streams[ i ] = new OutputStream( indent );
  _current_stream = 0;
}

void Output::stop_output()
{
  free_streams();
}

void Output::save_streams( Output &o )
{
  o.free_streams();
  o._ostr = _ostr;
  o._current_stream = _current_stream;
  for( int i = 0; i < MaxStreams; i++ ) {
    o._streams[ i ] = _streams[ i ];
    _streams[ i ] = NULL;
  }
}

void Output::restore_streams( Output &o )
{
  free_streams();
  _ostr = o._ostr;
  _current_stream = o._current_stream;
  for( int i = 0; i < MaxStreams; i++ ) {
    _streams[ i ] = o._streams[ i ];
    o._streams[ i ] = NULL;
  }
}

void Output::switchStream( int stream )
{
  assert( stream >=0 && stream < MaxStreams );
  _current_stream = stream;
}

void Output::enterLevel()
{
  _streams[ _current_stream ]->enterLevel();
}


void Output::leaveLevel()
{
  _streams[ _current_stream ]->leaveLevel();
}


Output &Output::operator<<( const char *s )
{
  (*_streams[ _current_stream ]) << s;
  return *this;
}


Output &Output::operator<<( const string &str )
{
  (*_streams[ _current_stream ]) << str;
  return *this;
}

Output &Output::operator<<( const CORBA::WChar *s )
{
  (*_streams[ _current_stream ]) << s;
  return *this;
}

#if SIZEOF_LONG == 4
Output &Output::operator<<( int i )
{
  (*_streams[ _current_stream ]) << i;
  return *this;
}
#endif

Output &Output::operator<<( CORBA::LongDouble ld )
{
  (*_streams[ _current_stream ]) << ld;
  return *this;
}


Output &Output::operator<<( CORBA::Float f )
{
  (*_streams[ _current_stream ]) << f;
  return *this;
}


Output &Output::operator<<( CORBA::Double d )
{
  (*_streams[ _current_stream ]) << d;
  return *this;
}


Output &Output::operator<<( CORBA::LongLong ll )
{
  (*_streams[ _current_stream ]) << ll;
  return *this;
}


Output &Output::operator<<( CORBA::ULongLong ull )
{
  (*_streams[ _current_stream ]) << ull;
  return *this;
}


Output &Output::operator<<( CORBA::Long l )
{
  (*_streams[ _current_stream ]) << l;
  return *this;
}


Output &Output::operator<<( CORBA::ULong ul )
{
  (*_streams[ _current_stream ]) << ul;
  return *this;
}


Output &Output::operator<<( CORBA::Short s )
{
  (*_streams[ _current_stream ]) << s;
  return *this;
}


Output &Output::operator<<( CORBA::UShort us )
{
  (*_streams[ _current_stream ]) << us;
  return *this;
}


Output &Output::operator<<( CORBA::Char c )
{
  (*_streams[ _current_stream ]) << c;
  return *this;
}


#if defined(_WIN32) && defined(_MSC_VER)
Output &Output::operator<<( MICO_WCharWrapper c )
#else
Output &Output::operator<<( CORBA::WChar c )
#endif
{
  (*_streams[ _current_stream ]) << c;
  return *this;
}


Output &Output::operator<<( const FixedBase &fb )
{
  (*_streams[ _current_stream ]) << fb;
  return *this;
}


Output &Output::operator<<( const CORBA::Any *a )
{
  (*_streams[ _current_stream ]) << a;
  return *this;
}


Output &Output::operator<<( endlnfp e )
{
  (*_streams[ _current_stream ]) << e;
  return *this;
}


Output &Output::operator<<( OutputFormatting f )
{
  (*_streams[ _current_stream ]) << f;
  return *this;
}


//-- CodeGen ------------------------------------------------------------

CodeGen::CodeGen (CORBA::Container_ptr con)
{
  _container = con;

  if (con->def_kind() == CORBA::dk_Repository) {
    _repo = CORBA::Repository::_narrow( con );
  }
  else {
    CORBA::Contained_var contained = CORBA::Contained::_narrow( con );
    assert( !CORBA::is_nil( contained ) );
    _repo = contained->containing_repository();
  }

  // Build dependencies
  IDLDep dep( _container );
  _idl_objs = dep.build_dependencies();
  // dep.print();
}

CodeGen::~CodeGen ()
{
}

CORBA::IDLType_ptr
CodeGen::lookup_result_by_id( const char *id )
{
  CORBA::Contained_var con = _repo->lookup_id( id );
  assert( !CORBA::is_nil( con ) );
  CORBA::OperationDef_var op = CORBA::OperationDef::_narrow( con );
  assert( !CORBA::is_nil( op ) );
  return op->result_def();
}

CORBA::IDLType_ptr
CodeGen::lookup_attribute_by_id( const char *id )
{
  CORBA::Contained_var con = _repo->lookup_id( id );
  assert( !CORBA::is_nil( con ) );
  CORBA::AttributeDef_var a = CORBA::AttributeDef::_narrow( con );
  assert( !CORBA::is_nil( a ) );
  return a->type_def();
}
