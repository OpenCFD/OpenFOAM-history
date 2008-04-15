// -*- c++ -*-
/*
 *  MICO --- a free CORBA implementation
 *  Copyright (C) 1997 Kay Roemer & Arno Puder
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

#ifndef __DII_H__
#define __DII_H__

#include <CORBA.h>
#include "cg.h"
#include <map>

class DIIError {
    string errmsg;
public:
    DIIError (const string &s) : errmsg (s) {}
    DIIError () {}
    bool errorOccured () const
    {
	return errmsg.length() != 0;
    }
    string getErrorMsg () const
    {
	return errmsg;
    }
};


class DII {
private:
    typedef map<string, CORBA::Object_var, less<string> > ObjMap;
    ObjMap objmap;
    string answer;
    int thekey;
    CORBA::ORB_var orb;
    CORBA::Repository_var ir;

    const string &getInst (ConceptNode *n);
    void setInst (ConceptNode *n, const string &s);
    bool getValue (CORBA::Any &, ConceptualGraph *, int level = 0);
    bool setValue (CORBA::Any &, ConceptualGraph *, int level = 0);
    CORBA::TypeCode_ptr mkTypeCode (ConceptualGraph *);
    ConceptualGraph *findNode (ConceptualGraph *, const string &);

    bool haveObject (CORBA::Object_ptr);
    bool haveObject (const string &);
    string insertObject (CORBA::Object_ptr);
    void removeObject (CORBA::Object_ptr);
    void removeObject (const string &);
    CORBA::Object_ptr getObject (const string &);
    string getKey (CORBA::Object_ptr);

    DIIError checkOp (CORBA::Object_ptr obj, CORBA::Request_ptr req);
public:
    DII();
    ~DII();

    DIIError bind (const string &spec, string &key_return);
    DIIError release (const string &key);
    DIIError invoke (const string &key, ConceptualGraph *);

    string getAnswerString () const;
    void setAnswerKey (const string &key);
    void setAnswerGraph (ConceptualGraph *cg);
    void setAnswerOk ();
    void setError (int num, const string &err);
};

#endif
