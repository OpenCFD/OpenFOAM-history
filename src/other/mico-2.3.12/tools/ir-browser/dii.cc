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

#include <iostream.h>
#include <strstream.h>
#include "dii.h"
#include <CORBA.h>
#include <mico/util.h>
#include <mico/template_impl.h>

// do some hardcoded consistency checks for the Account example.
#undef ACCOUNT_HACK
#include "account.h"


DII::DII()
    : thekey (0)
{
    orb = CORBA::ORB_instance ("mico-local-orb");
    CORBA::Object_var irobj =
	orb->resolve_initial_references ("InterfaceRepository");
    assert (!CORBA::is_nil (irobj));

    ir = CORBA::Repository::_narrow (irobj);
    assert (!CORBA::is_nil (ir));
}

DII::~DII()
{
}

const string &
DII::getInst (ConceptNode *n)
{
    assert (n->getInstances().size() == 1);
    SetOfString::const_iterator i;
    i = n->getInstances().begin();
    return *i;
}

void
DII::setInst (ConceptNode *n, const string &s)
{
    SetOfString inst;
    inst.push_back (s);
    n->setInstances (inst);
}

ConceptualGraph *
DII::findNode (ConceptualGraph *cg, const string &name)
{
    for (; cg; cg = cg->getDepth()) {
	if (cg->getNode()->getName() == name)
	    return cg;
    }
    return NULL;
}

bool
DII::getValue (CORBA::Any &a, ConceptualGraph *rootcg, int level)
{
    ConceptualGraph *cg = rootcg->getWidth();
    if (!cg)
	return false;
    cg = findNode (cg, "VALUE");
    if (!cg)
	return false;
    cg = cg->getWidth();
    if (!cg)
	return false;
    if (cg->getNode()->getName() == "BASIC") {
	cg = cg->getWidth();
	if (!cg)
	    return false;
	ConceptNode *cn = (ConceptNode *)cg->getNode();
	string type = cn->getName();
	if (type == "LONG") {
	    a <<= (CORBA::Long)cn->getValue();
	} else if (type == "ULONG") {
	    a <<= (CORBA::ULong)cn->getValue();
	} else if (type == "SHORT") {
	    a <<= (CORBA::Short)cn->getValue();
	} else if (type == "USHORT") {
	    a <<= (CORBA::UShort)cn->getValue();
	} else if (type == "STRING") {
	    a <<= getInst(cn).c_str();
	} else if (type == "BOOL") {
	    a <<= CORBA::Any::from_boolean (getInst(cn) == "TRUE"
					    ? TRUE : FALSE);
	} else if (type == "OBJREF") {
	    string key = getInst(cn);
	    if (key == "nil") {
		a <<= CORBA::Any::from_object (CORBA::Object::_nil(), "Foo");
	    } else {
		if (!haveObject (key))
		    return false;
		a <<= CORBA::Any::from_object (getObject (key), "Foo");
	    }
	} else
	    return false;
    } else if (cg->getNode()->getName() == "COMPLEX") {
	cg = cg->getWidth();
	if (!cg)
	    return false;
	string type = cg->getNode()->getName();
	if (type == "STRUCT") {
	    if (level == 0) {
		CORBA::TypeCode_var tc = mkTypeCode (rootcg);
		if (CORBA::is_nil (tc))
		    return false;
		a.reset ();
		a.set_type (tc);
	    }
	    if (!a.struct_put_begin())
		return false;
	    cg = cg->getWidth();
	    if (!cg)
		return false;
	    for ( ; cg; cg = cg->getDepth()) {
		if (!getValue (a, cg, level+1))
		    return false;
	    }
	    return a.struct_put_end();
	} else if (type == "SEQUENCE") {
	    if (level == 0) {
		CORBA::TypeCode_var tc = mkTypeCode (rootcg);
		if (CORBA::is_nil (tc))
		    return false;
		a.reset ();
		a.set_type (tc);
	    }
	    cg = cg->getWidth();
	    if (!cg)
		return false;
	    // count elements
	    CORBA::ULong nel = 0;
	    for (ConceptualGraph *cg2 = cg; cg; cg = cg->getDepth())
		++nel;
	    if (!a.seq_put_begin (nel))
		return false;
	    for ( ; cg; cg = cg->getDepth()) {
		if (cg->getNode()->getName() == "ELEMENT") {
		    if (!getValue (a, cg, level+1))
			return false;
		}
	    }
	    return a.seq_put_end();
	} else
	    return false;
    } else {
	return false;
    }
    return true;
}

bool
DII::setValue (CORBA::Any &a, ConceptualGraph *rootcg, int level)
{
    ConceptualGraph *cg = rootcg->getWidth();
    if (!cg)
	return false;
    cg = findNode (cg, "VALUE");
    if (!cg)
	return false;
    cg = cg->getWidth();
    if (!cg)
	return false;
    if (cg->getNode()->getName() == "BASIC") {
	cg = cg->getWidth();
	if (!cg)
	    return false;
	ConceptNode *cn = (ConceptNode *)cg->getNode();
	string type = cn->getName();
	if (type == "LONG") {
	    CORBA::Long l;
	    if (!(a >>= l))
		return false;
	    cn->setValue (l);
	} else if (type == "ULONG") {
	    CORBA::ULong ul;
	    if (!(a >>= ul))
		return false;
	    cn->setValue (ul);
	} else if (type == "SHORT") {
	    CORBA::Short s;
	    if (!(a >>= s))
		return false;
	    cn->setValue (s);
	} else if (type == "USHORT") {
	    CORBA::UShort us;
	    if (!(a >>= us))
		return false;
	    cn->setValue (us);
	} else if (type == "STRING") {
	    const char *s;
	    if (!(a >>= s))
		return false;
	    setInst (cn, (const char *)s);
	} else if (type == "BOOL") {
	    CORBA::Boolean b;
	    if (!(a >>= CORBA::Any::to_boolean (b)))
		return false;
	    setInst (cn, b ? "TRUE" : "FALSE");
	} else if (type == "OBJREF") {
	    CORBA::Object_var obj;
	    if (!(a >>= CORBA::Any::to_object (obj)))
		return false;
	    if (CORBA::is_nil (obj)) {
		setInst (cn, "nil");
	    } else {
		setInst (cn, getKey (obj));
	    }
	} else
	    return false;
    } else if (cg->getNode()->getName() == "COMPLEX") {
	cg = cg->getWidth();
	if (!cg)
	    return false;
	string type = cg->getNode()->getName();
	if (type == "STRUCT") {
	    if (!a.struct_get_begin())
		return false;
	    cg = cg->getWidth();
	    if (!cg)
		return false;
	    for ( ; cg; cg = cg->getDepth()) {
		if (!setValue (a, cg, level+1))
		    return false;
	    }
	    return a.struct_get_end();
	} else if (type == "SEQUENCE") {
	    CORBA::ULong nel;
	    if (!a.seq_get_begin (nel))
		return false;
	    cg = cg->getWidth();
	    if (!cg)
		return false;
	    cg->setDepth (0);
	    /*
	     * if no elements in list then create one dummy entry
	     * to represent the type of the list. Otherwise create
	     * nel-many successors.
	     */
	    if (nel == 0) {
		cg->getNode()->setName ("DUMMY");
	    } else {
		cg->getNode()->setName ("ELEMENT");
		ConceptualGraph *cg2 = cg;
		for (int i = 1; i < nel; ++i) {
		    cg2->setDepth (CopyGraph (cg2));
		    cg2 = cg2->getDepth();
		}
		for ( ; cg; cg = cg->getDepth()) {
		    if (!setValue (a, cg, level+1))
			return false;
		}
	    }
	    return a.seq_get_end();
	} else
	    return false;
    } else {
	return false;
    }
    return true;
}

CORBA::TypeCode_ptr
DII::mkTypeCode (ConceptualGraph *rootcg)
{
    CORBA::TypeCode_ptr NIL = CORBA::TypeCode::_nil();

    rootcg = rootcg->getWidth();
    if (!rootcg)
	return NIL;

    ConceptualGraph *cg = findNode (rootcg, "VALUE");
    if (!cg)
	return NIL;
    cg = cg->getWidth();
    if (!cg)
	return NIL;

    if (cg->getNode()->getName() == "BASIC") {
	cg = cg->getWidth();
	if (!cg)
	    return NIL;
	string type = cg->getNode()->getName();
	if (type == "LONG") {
	    return CORBA::_tc_long;
	} else if (type == "ULONG") {
	    return CORBA::_tc_ulong;
	} else if (type == "SHORT") {
	    return CORBA::_tc_short;
	} else if (type == "USHORT") {
	    return CORBA::_tc_ushort;
	} else if (type == "STRING") {
	    return CORBA::_tc_string;
	} else if (type == "BOOL") {
	    return CORBA::_tc_boolean;
	} else if (type == "OBJREF") {
	    return CORBA::_tc_Object;
	} else
	    return NIL;
    } else if (cg->getNode()->getName() == "COMPLEX") {
	cg = cg->getWidth();
	if (!cg)
	    return NIL;
	string type = cg->getNode()->getName();
	if (type == "STRUCT") {
	    cg = cg->getWidth();
	    if (!cg)
		return NIL;
	    CORBA::StructMemberSeq mems;
	    for (int i=0; cg; cg = cg->getDepth(), ++i) {
		CORBA::StructMember mem;
		ConceptualGraph *cg2 = cg->getWidth();
		if (!cg2)
		    return NIL;
		cg2 = findNode (cg2, "NAME");
		if (!cg2)
		    return NIL;
		ConceptNode *cn = (ConceptNode *)cg2->getNode();
		mem.name = CORBA::string_dup (getInst(cn).c_str());
		mem.type = mkTypeCode (cg);
		if (CORBA::is_nil (mem.type))
		    return NIL;
		mems.length (i+1);
		mems[i] = mem;
	    }
	    // XXX no repoid, no struct name ...
	    return CORBA::TypeCode::create_struct_tc ("", "", mems);
	} else if (type == "SEQUENCE") {
	    cg = cg->getWidth();
	    if (!cg)
		return NIL;
	    CORBA::TypeCode_var eltc = mkTypeCode (cg);
	    if (CORBA::is_nil (eltc))
		return NIL;
	    // XXX no repoid, no struct name ...
	    return CORBA::TypeCode::create_sequence_tc (0, eltc);
	} else
	    return NIL;
    } else {
	return NIL;
    }
    return NIL;
}

DIIError
DII::checkOp (CORBA::Object_ptr obj, CORBA::Request_ptr req)
{
    CORBA::InterfaceDef_var iface = obj->_get_interface();
    assert (!CORBA::is_nil (iface));

    CORBA::Contained_var contained = iface->lookup ((char *)req->operation());
    if (CORBA::is_nil (contained))
	return DIIError ("no such method");

    CORBA::OperationDef_var opdef = CORBA::OperationDef::_narrow (contained);
    assert (!CORBA::is_nil (opdef));

    CORBA::ParDescriptionSeq_var params = opdef->params();
    if (params->length() != req->arguments()->count())
	return DIIError ("wrong argument count");

    CORBA::TypeCode_var optc = req->result()->value()->type();
    CORBA::TypeCode_var irtc = opdef->result();
    if (!irtc->equivalent (optc))
	return DIIError ("wrong result type");

    for (CORBA::ULong i = 0; i < params->length(); ++i) {
	optc = req->arguments()->item(i)->value()->type();
	irtc = params[i].type_def->type();
	CORBA::Flags f = req->arguments()->item(i)->flags();

	string s = "for argument <";
	s += req->arguments()->item(i)->name();
	s += ">";

	switch (params[i].mode) {
	case CORBA::PARAM_IN:
	    if (f != CORBA::ARG_IN)
		return DIIError (string("wrong direction ") + s);
	    if (!optc->equivalent (irtc))
		return DIIError (string("wrong type ") + s);
	    break;

	case CORBA::PARAM_OUT:
	    if (f != CORBA::ARG_OUT)
		return DIIError (string("wrong direction ") + s);
	    if (!irtc->equivalent (optc))
		return DIIError (string("wrong type ") + s);
	    break;

	case CORBA::PARAM_INOUT:
	    if (f != CORBA::ARG_INOUT)
		return DIIError (string("wrong direction ") + s);
	    if (!optc->equaltype (irtc))
		return DIIError (string("wrong type ") + s);
	    break;
	}
    }
    return DIIError();
}

bool
DII::haveObject (CORBA::Object_ptr obj)
{
    for (ObjMap::iterator i = objmap.begin(); i != objmap.end(); ++i) {
	if (obj->_is_equivalent ((*i).second))
	    return true;
    }
    return false;
}

bool
DII::haveObject (const string &key)
{
    return objmap.count (key) == 1;
}

string
DII::insertObject (CORBA::Object_ptr obj)
{
    string key_ret = string("key") + xdec(thekey++);
    objmap[key_ret] = CORBA::Object::_duplicate (obj);
    return key_ret;
}

void
DII::removeObject (CORBA::Object_ptr obj)
{
    for (ObjMap::iterator i = objmap.begin(); i != objmap.end(); ++i) {
	if (obj->_is_equivalent ((*i).second)) {
	    objmap.erase (i);
	    break;
	}
    }
}

void
DII::removeObject (const string &key)
{
    objmap.erase (key);
}

CORBA::Object_ptr
DII::getObject (const string &key)
{
    ObjMap::iterator i = objmap.find (key);
    assert (i != objmap.end());
    return (*i).second;
}

string
DII::getKey (CORBA::Object_ptr obj)
{
    for (ObjMap::iterator i = objmap.begin(); i != objmap.end(); ++i) {
	if (obj->_is_equivalent ((*i).second)) {
	    return (*i).first;
	}
    }
    return insertObject (obj);
}

DIIError
DII::bind (const string &objspec, string &key_ret)
{
    CORBA::ORB_ptr orb = CORBA::ORB_instance ("mico-local-orb");
    CORBA::Object_var obj = orb->bind (objspec.c_str(), "local:");
    if (CORBA::is_nil (obj))
	return DIIError (string("no such repository id: ") + objspec);
    key_ret = insertObject (obj);
    return DIIError();
}

DIIError
DII::release (const string &key)
{
    if (!haveObject (key))
	return DIIError (string("no such key: ") + key);
    removeObject (key);
    return DIIError();
}

DIIError
DII::invoke (const string &key, ConceptualGraph *cg)
{
    if (!haveObject (key))
	return DIIError (string("no such key: ") + key);
    CORBA::Object_ptr obj = getObject (key);

    assert (cg);
    ConceptNode *root = (ConceptNode *) cg->getNode();
    if (root->getName() != string("OPERATION")) {
	return DIIError (string("bad root node: ") + root->getName());
    }
    string meth_name = getInst( root );
#ifdef ACCOUNT_HACK
    if (meth_name != "deposit" &&
	meth_name != "withdraw" &&
	meth_name != "balance" &&
	meth_name != "get_holder" &&
	meth_name != "set_holder") {
	return DIIError (string("no such method: ") + meth_name);
    }
#endif
    CORBA::Request_ptr req = obj->_request( meth_name.c_str() );

    ConceptualGraph *rels = cg->getWidth();

    int in_param_count = 0;
    int out_param_count = 0;
    int inout_param_count = 0;
    int result_count = 0;
    while( rels != NULL ) {
	string ptype = rels->getNode()->getName();
	ConceptualGraph *cgp, *args = rels->getWidth();
	string pname;
	CORBA::Any pvalue;

	if (ptype == "RESULT") {
	    if (++result_count > 1) {
		return DIIError ("more than one result branch");
	    }
            CORBA::TypeCode_ptr tc = mkTypeCode (rels);
            if (CORBA::is_nil (tc)) {
		return DIIError ("missing or bad value for result");
            }
            req->result()->value()->set_type (tc);
            CORBA::release (tc);
	} else {
	    cgp = findNode (args, "NAME");
	    if (!cgp) {
		return DIIError ("parameter without name");
	    }
	    pname = getInst ((ConceptNode *)cgp->getNode());
	    if (!getValue (pvalue, rels)) {
		return DIIError (string ("missing or bad value for "
					 "parameter ") + pname);
	    }
	    if (ptype == "IN_PARAM") {
		++in_param_count;
		req->add_in_arg (pname.c_str()) = pvalue;
	    } else if (ptype == "OUT_PARAM") {
		++out_param_count;
		req->add_out_arg (pname.c_str()) = pvalue;
	    } else if (ptype == "INOUT_PARAM") {
		++inout_param_count;
		req->add_inout_arg (pname.c_str()) = pvalue;
	    } else {
		return DIIError (string("bad type for parameter ") +
				 pname + ": " + ptype);
	    }
	}
	rels = rels->getDepth();
    }
#ifdef ACCOUNT_HACK
    CORBA::ULong ul_arg;
    Holder holder_arg;
    if (((meth_name == "deposit" || meth_name == "withdraw") &&
	 (in_param_count != 1 ||
	  out_param_count != 0 ||
	  inout_param_count != 0 ||
	  result_count != 0 ||
	  !(*req->arguments()->item(0)->value() >>= ul_arg))) ||
	((meth_name == "balance") &&
	 (in_param_count != 0 ||
	  out_param_count != 0 ||
	  inout_param_count != 0 ||
	  result_count != 1)) ||
	((meth_name == "set_holder") &&
	 (in_param_count != 1 ||
	  out_param_count != 0 ||
	  inout_param_count != 0 ||
	  result_count != 0 ||
	  !(*req->arguments()->item(0)->value() >>= holder_arg))) ||
	((meth_name == "get_holder") &&
	 (in_param_count != 0 ||
	  out_param_count != 0 ||
	  inout_param_count != 0 ||
	  result_count != 1))) {
	return DIIError (string("bad arguments for method ") + meth_name);
    }
#endif

    DIIError err = checkOp (obj, req);
    if (err.errorOccured())
	return err;

    req->invoke();
    if (CORBA::Exception *ex = req->env()->exception()) {
	CORBA::UnknownUserException *uuex =
	    CORBA::UnknownUserException::_downcast (ex);
	if (uuex)
	    return DIIError (string("user exception: ") +
			     uuex->_except_repoid());
	return DIIError (string("system exception: ") + ex->_repoid());
    }

    rels = cg->getWidth();
    for (int argidx=0; rels != NULL; ++argidx) {
	string ptype = rels->getNode()->getName();
	ConceptualGraph *cgp, *args = rels->getWidth();
	string pname;

	if (ptype == "RESULT") {
	    if (!setValue (*req->result()->value(), rels)) {
		CORBA::release (req);
		return DIIError ("cannot set result value");
	    }
	} else {
	    cgp = findNode (args, "NAME");
	    assert (cgp);
	    pname = getInst ((ConceptNode *)cgp->getNode());

	    if (ptype == "OUT_PARAM" || ptype == "INOUT_PARAM") {
		CORBA::Any *a = req->arguments()->item(argidx)->value();
		if (!setValue (*a, rels)) {
		    CORBA::release (req);
		    return DIIError (string ("cannot set out value "
					     "for parameter ") + pname);
		}
	    }
	}
	rels = rels->getDepth();
    }
    CORBA::release (req);
    return DIIError();
}

string
DII::getAnswerString () const
{
    return answer;
}

void
DII::setAnswerKey (const string &key)
{
    answer = "@answer-key=";
    answer += key;
}

void
DII::setAnswerGraph (ConceptualGraph *cg)
{
    answer = "@answer-graph=";
    ostrstream ostr;
    cg->print (ostr);
    ostr << ends;

    answer += ostr.str();
    ostr.rdbuf()->freeze (0);
}

void
DII::setAnswerOk ()
{
    answer = "@answer-ok";
}

void
DII::setError (int num, const string &err)
{
    answer = "@error ";
    answer += xdec(num);
    answer += " : '";
    answer += err;
    answer += "'";
}
