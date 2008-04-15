/*
 *  Externalization Service for MICO
 *  Copyright (C) 1999 Dmitry V. Sedov
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  Send comments and/or bug reports to:
 *                 mico@informatik.uni-frankfurt.de
 *  or to my private e-mail:
 *                 sedov@postman.simcb.ru
 */


#include <CORBA.h>
#include <coss/CosStream_impl.h>
#include <coss/ExternalizationTraversalCriteria_impl.h>
#include <coss/Traversal_impl.h>
#include <coss/RegisterHelper.h>
#include <map>
#include <list>
#include <algorithm>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif

using std::string;

typedef std::map<RoleNameKey,CosGraphs::Traversal::TraversalScopedId,std::less<RoleNameKey> > WorkedRoles;
typedef std::map<RoleNameKey,CosGraphs::Role_ptr,std::less<RoleNameKey> > NewRoles;
typedef IfaceSequenceTmpl<CosCompoundExternalization::Node_var,CosCompoundExternalization::Node_ptr> NewNodes;

const char tag_object = 0xF0;
const char tag_repeat_object  = 0x04;
const char tag_nil    = 0x05;
const char tag_char   = 0xF1;
const char tag_octet  = 0xF2;
const char tag_ulong  = 0xF3;
const char tag_ushort = 0xF4;
const char tag_long   = 0xF5;
const char tag_short  = 0xF6;
const char tag_float  = 0xF7;
const char tag_double = 0xF8;
const char tag_bool   = 0xF9;
const char tag_string = 0xFA;
const char tag_longlong = 0xFB;
const char tag_ulonglong = 0xFC;
const char tag_longdouble = 0xFD;


void StreamIO_impl::write_string(const char* aString)
{
    if (iotype == out)
	(*ostream_) << tag_string << aString << '\0';
}

void StreamIO_impl::write_char(CORBA::Char aChar)
{
    if (iotype == out)
	(*ostream_) << tag_char << aChar;
}

void StreamIO_impl::write_octet(CORBA::Octet anOctet)
{
    if (iotype == out)
	(*ostream_) << tag_octet << anOctet;
}

void StreamIO_impl::write_unsigned_long(CORBA::ULong anUnsignedLong)
{
    if (iotype == out)
	(*ostream_) << tag_ulong << anUnsignedLong;
}

void StreamIO_impl::write_unsigned_short(CORBA::UShort anUnsignedShort)
{
    if (iotype == out)
	(*ostream_) << tag_ushort << anUnsignedShort;
}

void StreamIO_impl::write_long(CORBA::Long aLong)
{
    if (iotype == out)
	(*ostream_) << tag_long << aLong;
}

void StreamIO_impl::write_short(CORBA::Short aShort)
{
    if (iotype == out)
	(*ostream_) << tag_short << aShort;
}

void StreamIO_impl::write_float(CORBA::Float aFloat)
{
    if (iotype == out)
	(*ostream_) << tag_float << aFloat;
}

void StreamIO_impl::write_double(CORBA::Double aDouble)
{
    if (iotype == out)
	(*ostream_) << tag_double << aDouble;
}

void StreamIO_impl::write_boolean(CORBA::Boolean aBoolean)
{
    if (iotype == out)
	(*ostream_) << tag_bool << aBoolean;
}

void StreamIO_impl::write_object(CosStream::Streamable_ptr aStreamable)
{
    if (iotype != out)
	return;
    if (CORBA::is_nil(aStreamable)) {
	(*ostream_) << tag_nil;
	return;
    }
    CosLifeCycle::Key_var object_key_var((*aStreamable).external_form_id());
    (*ostream_) << tag_object << (char)object_key_var->length();
    for(CORBA::ULong i = 0;i < object_key_var->length();++i)
	(*ostream_) << object_key_var[i].id << '\0';
    CosStream::StreamIO_ptr _sio = this->_this();
    (*aStreamable).externalize_to_stream(_sio);
}

CORBA::Boolean StreamIO_impl::was_extern(std::vector<CosObjectIdentity::ObjectIdentifier>* vec_,
					 CosObjectIdentity::ObjectIdentifier id)
{
    std::vector<CosObjectIdentity::ObjectIdentifier>::iterator j;
    for(j = vec_->begin();j != vec_->end();++j)
	if (*j == id)
	    return TRUE;
    return FALSE;
}


void StreamIO_impl::write_graph(CosCompoundExternalization::Node_ptr starting_node)
{
    if (iotype != out)
	return;
    CosStream::StreamIO_ptr _sio = this->_this();
    std::vector<CosObjectIdentity::ObjectIdentifier>* done_nodes =
	new std::vector<CosObjectIdentity::ObjectIdentifier>;
    std::vector<CosObjectIdentity::ObjectIdentifier>* done_rel =
	new std::vector<CosObjectIdentity::ObjectIdentifier>;
// Externalize Root Node
    CosObjectIdentity::ObjectIdentifier starting_node_id = starting_node->constant_random_id();
// End
    CosCompoundExternalization_impl::TraversalCriteria_impl* trav_crit_impl =
		new CosCompoundExternalization_impl::TraversalCriteria_impl();
    CosGraphs::TraversalCriteria_ptr trav_criteria = trav_crit_impl->_this();
    CosGraphs::NodeHandle root_node;
    root_node.the_node = CosCompoundExternalization::Node::_narrow(starting_node);
    root_node.constant_random_id = starting_node->constant_random_id();
    CosGraphs::Mode mode = CosGraphs::depthFirst;
    Traversal_impl* trav_impl = new Traversal_impl(root_node,trav_criteria,mode);
    CosGraphs::Traversal_ptr traversal = trav_impl->_this();
    CosGraphs::Traversal::ScopedEdges* OriginalEdgeList =
	new CosGraphs::Traversal::ScopedEdges;
    (*OriginalEdgeList).length(0);
    CosGraphs::Traversal::ScopedEdge* tmp_edge = NULL;
    while (traversal->next_one(tmp_edge))
    {
	(*OriginalEdgeList).length((*OriginalEdgeList).length()+1);
	(*OriginalEdgeList)[(*OriginalEdgeList).length()-1] = *tmp_edge;
	delete tmp_edge;
    }
    traversal->destroy();
    trav_criteria->destroy();
    for(CORBA::ULong i = 0;i < (*OriginalEdgeList).length();++i)
    {
	if (!was_extern(done_nodes, (*OriginalEdgeList)[i].from.point.the_node.constant_random_id))
	{
//externalize node
	    CosCompoundExternalization::Node_ptr cur_node_ptr =
		CosCompoundExternalization::Node::_narrow((*OriginalEdgeList)[i].from.point.the_node.the_node);
	    if (!CORBA::is_nil(cur_node_ptr))
	    {
		if (starting_node_id != cur_node_ptr->constant_random_id())
		{
		    CosLifeCycle::Key_var object_key_var((*cur_node_ptr).external_form_id());
		    (*ostream_) << tag_object << (char)object_key_var->length();
		    for(CORBA::ULong j = 0;j < object_key_var->length();++j)
			(*ostream_) << object_key_var[j].id << '\0';
		}
		cur_node_ptr -> externalize_node(_sio);
		done_nodes->push_back(cur_node_ptr->constant_random_id());
	    }
//done
	}
	for(CORBA::ULong j = 0;j < (*OriginalEdgeList)[i].relatives.length();++j)
	{
	    if (was_extern(done_nodes, (*OriginalEdgeList)[i].relatives[j].point.the_node.constant_random_id))
		continue;
//externalize node
	    CosCompoundExternalization::Node_ptr cur_node_ptr =
		CosCompoundExternalization::Node::_narrow(
			(*OriginalEdgeList)[i].relatives[j].point.the_node.the_node);
	    if (CORBA::is_nil(cur_node_ptr))
		continue;
	    CosLifeCycle::Key_var object_key_var((*cur_node_ptr).external_form_id());
	    (*ostream_) << tag_object << (char)object_key_var->length();
	    for(CORBA::ULong k = 0;k < object_key_var->length();++k)
		(*ostream_) << object_key_var[k].id << '\0';
	    cur_node_ptr -> externalize_node(_sio);
	    done_nodes->push_back(cur_node_ptr->constant_random_id());
//done
	}
    }
    if (!was_extern(done_nodes,starting_node_id))
	starting_node->externalize_node(_sio);
    for(CORBA::ULong i = 0;i < (*OriginalEdgeList).length();++i)
    {
	WorkedRoles role_rel_;
	if (was_extern(done_rel,(*OriginalEdgeList)[i].the_relationship.scoped_relationship.constant_random_id))
	    continue;
	CosCompoundExternalization::Relationship_ptr cur_rel_ptr =
		CosCompoundExternalization::Relationship::_narrow(
		    (*OriginalEdgeList)[i].the_relationship.scoped_relationship.the_relationship);
	if (CORBA::is_nil(cur_rel_ptr))
	    continue;
//externalize key of roles linked with current relationship
	this -> write_unsigned_long((*OriginalEdgeList)[i].the_relationship.scoped_relationship.constant_random_id);
	CORBA::InterfaceDef_ptr tmp_intdef =
	    (*OriginalEdgeList)[i].from.point.the_role.the_role->_get_interface();
	CORBA::InterfaceDef::FullInterfaceDescription_var full_ =
    		tmp_intdef -> describe_interface();
//	char* interface_id = CORBA::string_dup(full_->id);
  	RoleNameKey new_rec((*OriginalEdgeList)[i].from.point.the_node.constant_random_id,
  			full_->id.in(),
  			(*OriginalEdgeList)[i].from.point.the_role.the_name.in());
  	role_rel_[new_rec] = (*OriginalEdgeList)[i].the_relationship.scoped_relationship.constant_random_id;
  	CORBA::release(tmp_intdef);
	for(CORBA::ULong j = 0;j < (*OriginalEdgeList)[i].relatives.length();++j)
	{
	    CORBA::InterfaceDef_ptr tmp_intdef =
		(*OriginalEdgeList)[i].relatives[j].point.the_role.the_role->_get_interface();
	    CORBA::InterfaceDef::FullInterfaceDescription_var full_ =
    		tmp_intdef -> describe_interface();
//	    char * interface_id = CORBA::string_dup(full_->id);
	    RoleNameKey new_rec((*OriginalEdgeList)[i].relatives[j].point.the_node.constant_random_id,
			    full_->id.in(),
			    (*OriginalEdgeList)[i].relatives[j].point.the_role.the_name.in());
	    role_rel_[new_rec] =
		(*OriginalEdgeList)[i].the_relationship.scoped_relationship.constant_random_id;
	    CORBA::release(tmp_intdef);
	}
	CORBA::ULong count = role_rel_.size();
	this -> write_unsigned_long(count);
        if (1)	
        for(WorkedRoles::iterator j = role_rel_.begin();j != role_rel_.end();++j)
	{
	    this -> write_unsigned_long((*j).first.id);
  	    this -> write_string((*j).first.IDef);
  	    this -> write_string((*j).first.role_name);
      }
// done
	cur_rel_ptr -> externalize_relationship(_sio);
	done_rel->push_back((*OriginalEdgeList)[i].the_relationship.scoped_relationship.constant_random_id);
    }
    delete done_nodes;
    delete done_rel;
    delete OriginalEdgeList;
}

void StreamIO_impl::write_long_long( CORBA::LongLong val )
{
    if (iotype == out)
	(*ostream_) << tag_longlong << val;
}

void StreamIO_impl::write_unsigned_long_long( CORBA::ULongLong val )
{
    if (iotype == out)
	(*ostream_) << tag_longlong << val;
}

void StreamIO_impl::write_long_double( CORBA::LongDouble val )
{
    if (iotype == out)
	(*ostream_) << tag_longdouble << val;
}

void StreamIO_impl::write_fixed(const CORBA::Any& val,
                             CORBA::Short s)
{
}

char* StreamIO_impl::read_string()
{
    if (iotype == in)
    {
        string buf_;
	char tag_;
	tag_ = (char)(*istream_).peek();
	if (tag_ != tag_string)
	{
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	(*istream_) >> tag_;
	(*istream_) >> tag_;
	while (tag_ && !(*istream_).eof()) {
	    buf_ += tag_;
	    (*istream_) >> tag_;
	}
	if ((*istream_).eof())
	{
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	return CORBA::string_dup(buf_.c_str());
    }
    return NULL;
}

CORBA::Char StreamIO_impl::read_char()
{
    if (iotype == in)
    {
        CORBA::Char buf_;
	char tag_;
	tag_ = (char)(*istream_).peek();
	if (tag_ != tag_char)
	{
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	(*istream_) >> tag_ >> buf_;
	if ((*istream_).eof())
	{
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	return buf_;
    }
    return (CORBA::Char)0x00;
}

CORBA::Octet StreamIO_impl::read_octet()
{
    if (iotype == in)
    {
        CORBA::Octet buf_;
	char tag_;
	tag_ = (char)(*istream_).peek();
	if (tag_ != tag_octet)
	{
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	(*istream_) >> tag_ >> buf_;
	if ((*istream_).eof())
	{
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	return buf_;
    }
    return (CORBA::Octet)0x00;
}

CORBA::ULong StreamIO_impl::read_unsigned_long()
{
    if (iotype == in)
    {
        CORBA::ULong buf_;
	char tag_;
	tag_ = (char)(*istream_).peek();
	if (tag_ != tag_ulong)
	{
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	(*istream_) >> tag_ >> buf_;
	if ((*istream_).eof())
	{
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	return buf_;
    }
    return (CORBA::ULong)0x00;
}

CORBA::UShort StreamIO_impl::read_unsigned_short()
{
    if (iotype == in)
    {
	CORBA::UShort buf_;
	char tag_;
	tag_ = (char)(*istream_).peek();
	if (tag_ != tag_ushort)
	{
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	(*istream_) >> tag_ >> buf_;
	if ((*istream_).eof())
	{
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	return buf_;
    }
    return (CORBA::UShort)0x00;
}

CORBA::Long StreamIO_impl::read_long()
{
    if (iotype == in)
    {
	CORBA::Long buf_;
	char tag_;
	tag_ = (char)(*istream_).peek();
	if (tag_ != tag_long)
	{
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	(*istream_) >> tag_ >> buf_;
	if ((*istream_).eof())
	{
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	return buf_;
    }
    return (CORBA::Long)0x00;
}

CORBA::Short StreamIO_impl::read_short()
{
    if (iotype == in)
    {
	CORBA::Short buf_;
	char tag_;
	tag_ = (char)(*istream_).peek();
	if (tag_ != tag_short)
	{
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	(*istream_) >> tag_ >> buf_;
	if ((*istream_).eof())
	{
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	return buf_;
    }
    return (CORBA::Short)0x00;
}

CORBA::Float StreamIO_impl::read_float()
{
    if (iotype == in)
    {
	CORBA::Float buf_;
	char tag_;
	tag_ = (char)(*istream_).peek();
	if (tag_ != tag_float)
	{
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	(*istream_) >> tag_ >> buf_;
	if ((*istream_).eof())
	{
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	return buf_;
    }
    return (CORBA::Float)0x00;
}

CORBA::Double StreamIO_impl::read_double()
{
    if (iotype == in)
    {
	CORBA::Double buf_;
	char tag_;
	tag_ = (char)(*istream_).peek();
	if (tag_ != tag_double)
	{
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	(*istream_) >> tag_ >> buf_;
	if ((*istream_).eof())
	{
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	return buf_;
    }
    return (CORBA::Double)0x00;
}

CORBA::Boolean StreamIO_impl::read_boolean()
{
    if (iotype == in)
    {
	CORBA::Boolean buf_;
	char tag_;
	tag_ = (char)(*istream_).peek();
	if (tag_ != tag_bool)
	{
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	(*istream_) >> tag_ >> buf_;
	if ((*istream_).eof())
	{
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	return buf_;
    }
    return (CORBA::Boolean)0x00;
}

CosStream::Streamable_ptr StreamIO_impl::read_object(CosLifeCycle::FactoryFinder_ptr there,
                                		     CosStream::Streamable_ptr aStreamable)
{
    if (iotype != in)
	return (CosStream::Streamable_ptr)NULL;

    char tag_;
    char key_count;
    CosLifeCycle::Key factory_key;
    tag_ = (char)(*istream_).peek();
    if (tag_ == tag_nil)
    {
	(*istream_) >> tag_;
	return (CosStream::Streamable_ptr)NULL;
    }
    if (tag_ != tag_object)
    {
	CosStream::StreamDataFormatError ex;
	mico_throw (ex);
    }
    (*istream_) >> tag_;
    (*istream_) >> key_count;
    factory_key.length((CORBA::UShort)key_count);
    for(CORBA::UShort i = 0;i < (CORBA::UShort)key_count;++i)
    {
	string key;
	(*istream_) >> tag_;
	while(tag_ && !(*istream_).eof()) {
	    key += tag_;
	    (*istream_) >> tag_;
	}
	if ((*istream_).eof())
	{
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	factory_key[i].id = CORBA::string_dup(key.c_str());
	switch(i) {
	    case 0:
		factory_key[i].kind = CORBA::string_dup(_lc_ks_object_interface);
		break;
	    case 1:
		factory_key[i].kind = CORBA::string_dup(_lc_ks_impl_equiv_class);
		break;
	    case 2:
		factory_key[i].kind = CORBA::string_dup(_lc_ks_object_implementation);
		break;
	    case 3:
		factory_key[i].kind = CORBA::string_dup(_lc_ks_factory_interface);
		break;
	    default:
		factory_key[i].kind = CORBA::string_dup("Custom kind");
	}
    }

    CosStream::Streamable_ptr new_object_ptr;
    if (CORBA::is_nil(aStreamable))
    {
	CosLifeCycle::Factories* object_factories_ptr = there -> find_factories(factory_key);
	if (!(*object_factories_ptr).length())
	{
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	CORBA::ULong i;
	CosStream::StreamableFactory_ptr objectfactory_ptr = NULL;
	for (i = 0;i < (*object_factories_ptr).length();++i)
	{
	    objectfactory_ptr = CosStream::StreamableFactory::_narrow((*object_factories_ptr)[i]);
	    if (CORBA::is_nil(objectfactory_ptr))
		continue;
#ifdef HAVE_EXCEPTIONS
	    try {
#endif
		new_object_ptr = (*objectfactory_ptr).create_uninitialized();
#ifdef HAVE_EXCEPTIONS
	    } catch(...) {
		continue;
	    }
#endif
	    break;
	}
	if (i == (*object_factories_ptr).length())
	{
	    delete object_factories_ptr;
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	delete object_factories_ptr;
    }
    else
	new_object_ptr = aStreamable;
    CosStream::StreamIO_ptr _sio = this->_this();
    (*new_object_ptr).internalize_from_stream(_sio,there);
    return new_object_ptr;
}

void StreamIO_impl::read_graph(CosCompoundExternalization::Node_ptr starting_node,
                    	       CosLifeCycle::FactoryFinder_ptr there)
{
    CosGraphs::Node::Roles* rolesOfNode = NULL;
    NewRoles newallroles;
    CosStream::StreamIO_ptr _sio = this->_this();
    starting_node -> internalize_node(_sio,there,rolesOfNode);
    for(CORBA::ULong i = 0;i < (*rolesOfNode).length();++i)
    {
	CORBA::InterfaceDef_ptr tmp_intdef =
		(*rolesOfNode)[i]->_get_interface();
	CORBA::InterfaceDef::FullInterfaceDescription_var full_ =
    			tmp_intdef -> describe_interface();
//	char* interface_id = CORBA::string_dup (full_->id);
	CosObjectIdentity::ObjectIdentifier id_ = (*starting_node).constant_random_id();
	RoleNameKey new_rec(id_,full_->id.in(),"");
	if (newallroles.find(new_rec) == newallroles.end())
	    newallroles[new_rec] = (CosGraphs::Role_ptr)(*rolesOfNode)[i];
	CORBA::release(tmp_intdef);
    }
//    if (rolesOfNode) delete rolesOfNode;
// inrternalize nodes
    while (TRUE)
    {
	char tag_;
	char key_count;
	CosLifeCycle::Key factory_key;
	tag_ = (char)(*istream_).peek();
	if (tag_ != tag_object)
	    break;
	(*istream_) >> tag_;
	(*istream_) >> key_count;
	factory_key.length((CORBA::UShort)key_count);
	if (1)
            for(CORBA::UShort i = 0;i < (CORBA::UShort)key_count;++i)
	{
	    string key;
	    (*istream_) >> tag_;
	    while(tag_ && !(*istream_).eof()) {
		key += tag_;
		(*istream_) >> tag_;
	    }
	    if ((*istream_).eof())
	    {
		CosStream::StreamDataFormatError ex;
		mico_throw (ex);
	    }
	    factory_key[i].id = CORBA::string_dup(key.c_str());
	    switch(i) {
			case 0:
				factory_key[i].kind = CORBA::string_dup(_lc_ks_object_interface);
				break;
	    	case 1:
				factory_key[i].kind = CORBA::string_dup(_lc_ks_impl_equiv_class);
				break;
	    	case 2:
				factory_key[i].kind = CORBA::string_dup(_lc_ks_object_implementation);
				break;
	    	case 3:
				factory_key[i].kind = CORBA::string_dup(_lc_ks_factory_interface);
				break;
			default:
		    	factory_key[i].kind = CORBA::string_dup("Custom kind");
	    }
	}

	CosStream::Streamable_ptr new_object_ptr;
	CosLifeCycle::Factories* object_factories_ptr = there -> find_factories(factory_key);
	if (!(*object_factories_ptr).length())
	{
	    delete object_factories_ptr;
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	CORBA::ULong i;
	CosStream::StreamableFactory_ptr objectfactory_ptr = NULL;
	CosCompoundExternalization::Node_ptr new_node_ptr = NULL;
	for (i = 0;i < (*object_factories_ptr).length();++i)
	{
	    objectfactory_ptr = CosStream::StreamableFactory::_narrow((*object_factories_ptr)[i]);
	    if (CORBA::is_nil(objectfactory_ptr))
		continue;
#ifdef HAVE_EXCEPTIONS
	    try {
#endif
		new_object_ptr = (*objectfactory_ptr).create_uninitialized();
#ifdef HAVE_EXCEPTIONS
	    } catch(...) {
		continue;
	    }
#endif
	    new_node_ptr = CosCompoundExternalization::Node::_narrow(new_object_ptr);
	    if (CORBA::is_nil(new_node_ptr))
		continue;
	    break;
	}
	if (i == (*object_factories_ptr).length())
	{
	    delete object_factories_ptr;
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	delete object_factories_ptr;
//	CORBA::release(objectfactory_ptr);
	new_node_ptr -> internalize_node(_sio,there,rolesOfNode);
	for(CORBA::ULong i = 0;i < (*rolesOfNode).length();++i)
	{
	    CORBA::InterfaceDef_ptr tmp_intdef =
		(*rolesOfNode)[i]->_get_interface();
	    CORBA::InterfaceDef::FullInterfaceDescription_var full_ =
    			tmp_intdef -> describe_interface();
//	    char* interface_id = CORBA::string_dup (full_->id);
	    CosObjectIdentity::ObjectIdentifier id_ = (*new_node_ptr).constant_random_id();
	    RoleNameKey new_rec(id_,full_->id.in(),"");
	    if (newallroles.find(new_rec) == newallroles.end())
		newallroles[new_rec] = (CosGraphs::Role_ptr)(*rolesOfNode)[i];
	    CORBA::release(tmp_intdef);
	}
//	if (rolesOfNode) delete rolesOfNode;
    }
// done
// internalize relationships
    while (TRUE)
    {
	char tag_;
	tag_ = (char)(*istream_).peek();
	if (tag_ != tag_ulong)
	    break;
	CORBA::ULong rel_id = this -> read_unsigned_long();
// read key of roles linked to relationship
	CORBA::ULong count = this -> read_unsigned_long();
	CosRelationships::NamedRoles newroles;
	for(CORBA::ULong i = 0;i < count;++i)
	{
	    CORBA::ULong id = this -> read_unsigned_long();
	    string idef = this -> read_string();
	    string role_name = this -> read_string();
	    RoleNameKey rolekey(id,idef.c_str(),role_name.c_str());
	    NewRoles::iterator it = newallroles.find(rolekey);
	    if (it != newallroles.end())
	    {
		newroles.length(newroles.length()+1);
		newroles[newroles.length()-1].aRole = CosRelationships::Role::_duplicate((*it).second);
		newroles[newroles.length()-1].name = CORBA::string_dup(role_name.c_str());
	    }
	}
// done
	CORBA::ULong key_count = this -> read_unsigned_long();
	CosLifeCycle::Key factory_key;
	factory_key.length(key_count);
	for(CORBA::ULong i = 0;i < key_count;++i)
	{
	    string id;
	    id += this -> read_string();
	    factory_key[i].id = CORBA::string_dup(id.c_str());
	    switch(i) {
			case 0:
				factory_key[i].kind = CORBA::string_dup(_lc_ks_object_interface);
				break;
	    	case 1:
				factory_key[i].kind = CORBA::string_dup(_lc_ks_impl_equiv_class);
				break;
	    	case 2:
				factory_key[i].kind = CORBA::string_dup(_lc_ks_object_implementation);
				break;
	    	case 3:
				factory_key[i].kind = CORBA::string_dup(_lc_ks_factory_interface);
				break;
			default:
				factory_key[i].kind = CORBA::string_dup("Custom kind");
	    }
	}
	CosRelationships::Relationship_ptr new_object_ptr;
	CosLifeCycle::Factories* object_factories_ptr = there -> find_factories(factory_key);
	if (!(*object_factories_ptr).length())
	{
	    delete object_factories_ptr;
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	CORBA::ULong j;
	CosRelationships::RelationshipFactory_ptr relfactory_ptr = NULL;
	CosCompoundExternalization::Relationship_ptr new_rel_ptr = NULL;
	for (j = 0;j < (*object_factories_ptr).length();++j)
	{
	    relfactory_ptr = CosRelationships::RelationshipFactory::_narrow((*object_factories_ptr)[j]);
	    if (CORBA::is_nil(relfactory_ptr))
		continue;
#ifdef HAVE_EXCEPTIONS
	    try {
#endif
		new_object_ptr = (*relfactory_ptr).create(newroles);
#ifdef HAVE_EXCEPTIONS
	    } catch(...) {
		continue;
	    }
#endif
	    new_rel_ptr = CosCompoundExternalization::Relationship::_narrow(new_object_ptr);
	    if (CORBA::is_nil(new_rel_ptr))
		continue;
	    break;
	}
	if (j == (*object_factories_ptr).length())
	{
	    delete object_factories_ptr;
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	delete object_factories_ptr;
//	CORBA::release(relfactory_ptr);
	CosGraphs::NamedRoles tmp;
	tmp.length(newroles.length());
	for(j = 0;j < newroles.length();++j)
	{
	    tmp[j].the_role = CosGraphs::Role::_narrow(newroles[j].aRole);
	    tmp[j].the_name = CORBA::string_dup(newroles[j].name);
	}
	
	new_rel_ptr -> internalize_relationship(_sio,tmp);
    }
}

CORBA::LongLong
StreamIO_impl::read_long_long()
{
    if (iotype == in)
    {
        CORBA::LongLong buf_;
	char tag_;
	tag_ = (char)(*istream_).peek();
	if (tag_ != tag_longlong)
	{
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	(*istream_) >> tag_ >> buf_;
	if ((*istream_).eof())
	{
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	return buf_;
    }
    return (CORBA::LongLong)0x00;
}

CORBA::ULongLong
StreamIO_impl::read_unsigned_long_long()
{
    if (iotype == in)
    {
        CORBA::ULongLong buf_;
	char tag_;
	tag_ = (char)(*istream_).peek();
	if (tag_ != tag_ulonglong)
	{
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	(*istream_) >> tag_ >> buf_;
	if ((*istream_).eof())
	{
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	return buf_;
    }
    return (CORBA::ULongLong)0x00;
}

CORBA::LongDouble
StreamIO_impl::read_long_double()
{
    if (iotype == in)
    {
        CORBA::LongDouble buf_;
	char tag_;
	tag_ = (char)(*istream_).peek();
	if (tag_ != tag_longdouble)
	{
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	(*istream_) >> tag_ >> buf_;
	if ((*istream_).eof())
	{
	    CosStream::StreamDataFormatError ex;
	    mico_throw (ex);
	}
	return buf_;
    }
    return (CORBA::LongDouble)0x00;
}

CORBA::Any* StreamIO_impl::read_fixed()
{
}
