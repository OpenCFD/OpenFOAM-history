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


#include "Documents.h"

#ifndef __Documents_impl_h__
#define __Documents_impl_h__


class AbstractDocument_impl
{
protected:
  char* doc_name;
  char* file_name;
public:
  AbstractDocument_impl ();
//  AbstractDocument_impl (CORBA::Object_ptr);
  AbstractDocument_impl (const char*);
  AbstractDocument_impl (const char*, const char*);
//  CORBA::Boolean _save_object ();
  virtual char* name ();
  virtual void name (const char*);
  virtual char* file ();
  virtual void file (const char*);
  virtual void destroy ();
};


class Figure_impl : virtual public AbstractDocument_impl
{
  char* pict;
protected:
public:
  Figure_impl ();
//  Figure_impl (CORBA::Object_ptr);
  Figure_impl (const char*);
  Figure_impl (const char*, const char*);
  virtual void destroy ();

};


class Text_impl : virtual public AbstractDocument_impl
{
protected:
public:
  Text_impl ();
//  Text_impl (CORBA::Object_ptr);
  Text_impl (const char*);
  Text_impl (const char*, const char*);
};


// class FigureLoader : virtual CORBA::BOAObjectRestorer
// {
// public:
//   CORBA::Boolean restore (CORBA::Object_ptr);
// };


// class TextLoader : virtual CORBA::BOAObjectRestorer
// {
// public:
//   CORBA::Boolean restore (CORBA::Object_ptr);
// };

class FigureFactory_impl
{
public:
  virtual Documents::Figure_ptr create (const char*);
  virtual Documents::Figure_ptr 
    create_with_file (const char*, const char*);
};


class TextFactory_impl
{
public:
  virtual Documents::Text_ptr create (const char*);
  virtual Documents::Text_ptr 
    create_with_file (const char*, const char*);
};

#endif





