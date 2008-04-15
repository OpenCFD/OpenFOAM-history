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

#include "../../idl/codegen-idl.h"
#include "browserI.h"
#include <strstream.h>

IRBrowser_impl::IRBrowser_impl()
{
    orb = CORBA::ORB_instance ("mico-local-orb");
    CORBA::Object_var irobj =
	orb->resolve_initial_references ("InterfaceRepository");
    assert (!CORBA::is_nil (irobj));

    ir = CORBA::Repository::_narrow (irobj);
    assert (!CORBA::is_nil (ir));

    current = CORBA::Container::_duplicate( ir );
    current_wd = "/";
}

char*
IRBrowser_impl::getwd()
{
    return CORBA::string_dup (current_wd.c_str());
}

CORBA::Boolean
IRBrowser_impl::back()
{
    CORBA::Contained_var contained = CORBA::Contained::_narrow (current);
    if (CORBA::is_nil (contained))
        // at top
        return FALSE;

    current = contained->defined_in();
    assert (!CORBA::is_nil (current));

    assert (current_wd.length() > 0);
    int pos = current_wd.rfind ("/", current_wd.length()-2);
    assert (pos >= 0);
    current_wd = current_wd.substr (0, pos+1);
    return TRUE;
}

CORBA::Boolean
IRBrowser_impl::cd( const char* el )
{
    if (string(el) == "..")
	return back();

    CORBA::Contained_var contained = current->lookup ((char *)el);
    if (CORBA::is_nil (contained))
	return FALSE;

    CORBA::Container_var container = CORBA::Container::_narrow (contained);
    if (CORBA::is_nil (container))
	return FALSE;
    current = container;

    current_wd += el;
    current_wd += "/";

    return TRUE;
}

IRBrowser::NameList *
IRBrowser_impl::list()
{
    CORBA::ContainedSeq_var contents
        = current->contents (CORBA::dk_all, TRUE);

    NameList *nl = new NameList;
    CORBA::ULong j;
    if (current_wd == "/") {
	nl->length (contents->length());
	j = 0;
    } else {
	nl->length (contents->length()+1);
	(*nl)[0] = CORBA::string_dup ("..");
	j = 1;
    }

    for (CORBA::ULong i = 0; i < contents->length(); ++i, ++j) {
        (*nl)[j] = contents[i]->name();
    }
    return nl;
}

IRBrowser::NameList *
IRBrowser_impl::types()
{
    CORBA::ContainedSeq_var contents
        = current->contents (CORBA::dk_all, TRUE);

    NameList *nl = new NameList;
    CORBA::ULong j;
    if (current_wd == "/") {
	nl->length (contents->length());
	j = 0;
    } else {
	nl->length (contents->length()+1);
	(*nl)[0] = type ("..");
	j = 1;
    }

    for (CORBA::ULong i = 0; i < contents->length(); ++i, ++j) {
	CORBA::String_var name = contents[i]->name();
        (*nl)[j] = type (name);
    }
    return nl;
}

char *
IRBrowser_impl::type( const char* el )
{
    if (string(el) == "..")
        return CORBA::string_dup ("back");

    CORBA::Contained_var contained = current->lookup ((char *)el);
    assert (!CORBA::is_nil (contained));

    string type;

    switch (contained->def_kind()) {
    case CORBA::dk_Attribute:
        type = "attribute";
        break;
    case CORBA::dk_Constant:
        type = "constant";
        break;
    case CORBA::dk_Exception:
        type = "exception";
        break;
    case CORBA::dk_Interface:
        type = "interface";
        break;
    case CORBA::dk_Module:
        type = "module";
        break;
    case CORBA::dk_Operation:
        type = "operation";
        break;
    case CORBA::dk_Typedef:
        type = "typedef";
        break;
    case CORBA::dk_Alias:
        type = "alias";
        break;
    case CORBA::dk_Struct:
        type = "struct";
        break;
    case CORBA::dk_Union:
        type = "union";
        break;
    case CORBA::dk_Enum:
        type = "enum";
        break;
    case CORBA::dk_Primitive:
        type = "primitive";
        break;
    case CORBA::dk_String:
        type = "string";
        break;
    case CORBA::dk_Sequence:
        type = "sequence";
        break;
    case CORBA::dk_Array:
        type = "array";
        break;
    default:
        assert (0);
    }
    return CORBA::string_dup (type.c_str());
}

char *
IRBrowser_impl::describe( const char* el )
{
  if( string( el ) == ".." )
    return CORBA::string_dup( "" );
  
  CORBA::Contained_var contained = current->lookup( (char *) el );
  assert( !CORBA::is_nil( contained ) );
  
  DB db;
  IDLParam params;
  CodeGenIDL gen( db, params, ir );
  string descr;
  descr = gen.emit( contained );
  return CORBA::string_dup( descr.c_str() );
}

ConceptualGraph *
IRBrowser_impl::mkConNode (const string &name, const string &inst,
			   ConceptualGraph *wd,
			   ConceptualGraph *ht)
{
    SetOfString v;
    v.push_back (inst);
    return new ConceptualGraph (new ConceptNode (name, v), wd, ht);
}

ConceptualGraph *
IRBrowser_impl::mkConNode (const string &name,
			   ConceptualGraph *wd,
			   ConceptualGraph *ht)
{
    return new ConceptualGraph (new ConceptNode (name), wd, ht);
}

ConceptualGraph *
IRBrowser_impl::mkRelNode (const string &name,
			   ConceptualGraph *wd,
			   ConceptualGraph *ht)
{
    return new ConceptualGraph (new RelationNode (name), wd, ht);
}

ConceptualGraph *
IRBrowser_impl::tcToCg (CORBA::TypeCode_ptr _tc)
{
    ConceptualGraph *cg = 0;

    CORBA::TypeCode_ptr tc = _tc->unalias();
    switch (tc->kind()) {
    case CORBA::tk_short:
	return mkRelNode ("BASIC",
			  mkConNode ("SHORT", "0", 0, 0),
			  0);

    case CORBA::tk_ushort:
	return mkRelNode ("BASIC",
			  mkConNode ("USHORT", "0", 0, 0),
			  0);

    case CORBA::tk_long:
	return mkRelNode ("BASIC",
			  mkConNode ("LONG", "0", 0, 0),
			  0);

    case CORBA::tk_ulong:
	return mkRelNode ("BASIC",
			  mkConNode ("ULONG", "0", 0, 0),
			  0);

    case CORBA::tk_string:
	return mkRelNode ("BASIC",
			  mkConNode ("STRING", "foobar", 0, 0),
			  0);

    case CORBA::tk_objref:
	return mkRelNode ("BASIC",
			  mkConNode ("OBJREF", "key0", 0, 0),
			  0);

    case CORBA::tk_boolean:
	return mkRelNode ("BASIC",
			  mkConNode ("BOOL", "FALSE", 0, 0),
			  0);

    case CORBA::tk_struct:
      {
	for (int i = tc->member_count(); --i >= 0; ) {
	    cg = mkRelNode ("MEMBER",
			    mkConNode ("VALUE",
				       tcToCg (tc->member_type(i)),
				       mkConNode ("NAME", tc->member_name(i),
						  0, 0)),
			    cg);
	}
	return mkRelNode ("COMPLEX",
			  mkConNode ("STRUCT", cg, 0),
			  0);
      }
    case CORBA::tk_sequence:
	return mkRelNode
	    ("COMPLEX",
	     mkConNode ("SEQUENCE",
			mkRelNode ("DUMMY",
				   mkConNode ("VALUE",
					      tcToCg (tc->content_type()),
					      0),
				   0),
			0),
	     0);

    default:
	assert (0);
	return 0;
    }
}

char *
IRBrowser_impl::optemplate (const char *el)
{
    CORBA::Contained_var contained = current->lookup ((char *)el);
    if (CORBA::is_nil (contained))
	return FALSE;

    CORBA::OperationDef_var opdef = CORBA::OperationDef::_narrow (contained);
    if (CORBA::is_nil (opdef))
	return CORBA::string_dup ("");

    CORBA::ParDescriptionSeq_var params = opdef->params();

    ConceptualGraph *cg = 0;
    for (CORBA::Long i = params->length(); --i >= 0; ) {
	string dir;
	switch (params[ (CORBA::ULong) i].mode) {
	case CORBA::PARAM_IN:
	    dir = "IN_PARAM";
	    break;
	case CORBA::PARAM_OUT:
	    dir = "OUT_PARAM";
	    break;
	case CORBA::PARAM_INOUT:
	    dir = "INOUT_PARAM";
	    break;
	}
	CORBA::TypeCode_var tc = params[ (CORBA::ULong) i].type_def->type();
	cg = mkRelNode (dir,
			mkConNode ("VALUE",
				   tcToCg (tc),
				   mkConNode ("NAME",
				   (const char *)params[(CORBA::ULong)i].name,
					      0, 0)),
			cg);
    }
    CORBA::TypeCode_var res = opdef->result();
    if (!res->equaltype (CORBA::_tc_void))
	cg = mkRelNode ("RESULT",
			mkConNode ("VALUE",
				   tcToCg (res),
				   0),
			cg);

    CORBA::String_var name = opdef->name();
    cg = mkConNode ("OPERATION", (const char *)name, cg, 0);

    ostrstream ostr;
    cg->print (ostr);
    ostr << ends;
    delete cg;

    char *retval = CORBA::string_dup (ostr.str());
    ostr.rdbuf()->freeze(0);
    return retval;
}

char *
IRBrowser_impl::repoid ()
{
  CORBA::Contained_var contained = CORBA::Contained::_narrow (current);
  if (CORBA::is_nil (contained))
	return CORBA::string_dup ("");
  return contained->id();
}
