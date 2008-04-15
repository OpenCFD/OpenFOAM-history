/*
 *  Demo of COSS Relationship Service
 *  Copyright (C) 1998 Karel Gardas
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
 *  or to my private e-mail:
 *                 gardask@alpha.inf.upol.cz
 */


#include "Documents_impl.h"
#include <string.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif

//#define DEBUG 1


AbstractDocument_impl::AbstractDocument_impl () 
{
  doc_name = CORBA::string_dup ("");
  file_name = CORBA::string_dup ("");
}


// AbstractDocument_impl::AbstractDocument_impl (CORBA::Object_ptr obj)
//   : Documents::AbstractDocument_skel (obj)
// {
//   char s[1000];
//   ifstream in (obj->_ident ());
//   assert (in);

//   in >> s; // --abstract--
//   in >> s;
//   doc_name = CORBA::string_dup (s);
//   in >> s;
//   file_name = CORBA::string_dup (s);
// }


AbstractDocument_impl::AbstractDocument_impl (const char* name)
{
  doc_name = CORBA::string_dup (name);
  file_name = CORBA::string_dup ("");
}


AbstractDocument_impl::AbstractDocument_impl (const char* value, 
					      const char* file)
{
  doc_name = CORBA::string_dup (value);
  file_name = CORBA::string_dup (file);
}


// CORBA::Boolean
// AbstractDocument_impl::_save_object ()
// {
//   ofstream out (_ident ());
//   assert (out);
  
//   out << "--abstract--\n";
//   out << doc_name << "\n";
//   out << file_name << "\n";
  
//   return TRUE;
// }


char*
AbstractDocument_impl::name ()
{
  return CORBA::string_dup (doc_name);
}


void
AbstractDocument_impl::name (const char* value)
{
  doc_name = CORBA::string_dup (value);
}



char*
AbstractDocument_impl::file ()
{
  return CORBA::string_dup (file_name);
}


void
AbstractDocument_impl::file (const char* value)
{
  file_name = CORBA::string_dup (value);
}


void
AbstractDocument_impl::destroy ()
{
//   CORBA::BOA_var boa = _boa ();
//   CORBA::ORB_var orb = _orb ();
//   boa->deactivate_obj (this);
//   orb->shutdown (TRUE);
}


Figure_impl::Figure_impl ()
  : AbstractDocument_impl ()
{
  pict = new char [1048576];
}


Figure_impl::Figure_impl (const char* name)
  : AbstractDocument_impl (name)
{
  pict = new char [1048576];
}


Figure_impl::Figure_impl (const char* name, const char* file)
  : AbstractDocument_impl (name, file)
{
}


// Figure_impl::Figure_impl (CORBA::Object_ptr obj)
//   :  AbstractDocument_impl (obj)
// {
// }


void
Figure_impl::destroy ()
{
  //  PortableServer::POA_var poa = _default_POA ();
  
}

Text_impl::Text_impl ()
  : AbstractDocument_impl ()
{
}


Text_impl::Text_impl (const char* name)
  : AbstractDocument_impl (name)
{
}


Text_impl::Text_impl (const char* name, const char* file)
  : AbstractDocument_impl (name, file)
{
}


// Text_impl::Text_impl (CORBA::Object_ptr obj)
//   :  AbstractDocument_impl (obj), Documents::Text_skel (obj)
// {
// }


// CORBA::Boolean
// FigureLoader::restore (CORBA::Object_ptr obj)
// {
//   if (strcmp (obj->_repoid (), "IDL:Documents/Figure:1.0") == 0) {
// #if DEBUG
//     cout << "  <Documents_impl> restoring text\n";
// #endif
//     new Figure_impl (obj);
//     return TRUE;
//   }
// #if DEBUG
//   cout << "  <Documents_impl> can't restore " << obj->_repoid () << "\n";
// #endif
//   return FALSE;

// }


// CORBA::Boolean
// TextLoader::restore (CORBA::Object_ptr obj)
// {
//   if (strcmp (obj->_repoid (), "IDL:Documents/Text:1.0") == 0) {
// #if DEBUG
//     cout << "  <Documents_impl> restoring text\n";
// #endif
//     new Text_impl (obj);
//     return TRUE;
//   }
// #if DEBUG
//   cout << "  <Documents_impl> can't restore " << obj->_repoid () << "\n";
// #endif
//   return FALSE;

// }

Documents::Figure_ptr
FigureFactory_impl::create (const char* name)
{
  //CORBA::BOA_var boa = _boa ();
#if DEBUG
  cout << "  <Documents_impl> create figure\n";
#endif
  Figure_impl* fig = new Figure_impl (name);
  POA_Documents::Figure_tie<Figure_impl>* tie 
    = new POA_Documents::Figure_tie<Figure_impl> (*fig);
  // boa->deactivate_obj (fig);

  //return Documents::Figure::_duplicate (fig);
  return tie->_this ();
}

Documents::Figure_ptr
FigureFactory_impl::create_with_file (const char* name, const char* file)
{
  //CORBA::BOA_var boa = _boa ();
#if DEBUG
  cout << "  <Documents_impl> create figure\n";
#endif
  Figure_impl* fig = new Figure_impl (name, file);
  POA_Documents::Figure_tie<Figure_impl>* tie 
    = new POA_Documents::Figure_tie<Figure_impl> (*fig);
  //boa->deactivate_obj (fig);

  //return Documents::Figure::_duplicate (fig);
  return tie->_this ();
}



Documents::Text_ptr
TextFactory_impl::create (const char* name)
{
  //CORBA::BOA_var boa = _boa ();
#if DEBUG
  cout << "  <Documents_impl> create Text\n";
#endif
  Text_impl* text = new Text_impl (name);
  POA_Documents::Text_tie<Text_impl>* tie 
    = new POA_Documents::Text_tie<Text_impl> (*text);
  //boa->deactivate_obj (fig);

  //return Documents::Text::_duplicate (fig);
  return tie->_this ();
  
}

Documents::Text_ptr
TextFactory_impl::create_with_file (const char* name, const char* file)
{
  //CORBA::BOA_var boa = _boa ();
#if DEBUG
  cout << "  <Documents_impl> create Text\n";
#endif
  Text_impl* text = new Text_impl (name, file);
  POA_Documents::Text_tie<Text_impl>* tie 
    = new POA_Documents::Text_tie<Text_impl> (*text);
  //boa->deactivate_obj (fig);

  //return Documents::Text::_duplicate (fig);
  return tie->_this ();
}


