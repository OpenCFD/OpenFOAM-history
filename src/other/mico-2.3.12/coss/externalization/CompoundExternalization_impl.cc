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
#include <coss/CompoundExternalization_impl.h>
#include <coss/RegisterHelper.h>

CosCompoundExternalization_impl::Node_impl::Node_impl ()
    : ::Node_impl(),
      POA_CosCompoundExternalization::Node(),
      POA_CosStream::Streamable()
{
    external_from_id_.length(4);
    external_from_id_[0].id = CORBA::string_dup("::CosCompoundExternalization::Node");
    external_from_id_[1].id = CORBA::string_dup("::CosStream::Streamable");
    external_from_id_[2].id = CORBA::string_dup("");
    external_from_id_[3].id = CORBA::string_dup("NodeStreamableFactory");
    external_from_id_[0].kind = CORBA::string_dup( _lc_ks_object_interface );
    external_from_id_[1].kind = CORBA::string_dup( _lc_ks_impl_equiv_class );
    external_from_id_[2].kind = CORBA::string_dup( _lc_ks_object_implementation );
    external_from_id_[3].kind = CORBA::string_dup( _lc_ks_factory_interface );
}

CosCompoundExternalization_impl::Node_impl::Node_impl ( CosRelationships::RelatedObject_ptr obj)
    : ::Node_impl(),
      POA_CosCompoundExternalization::Node(),
      POA_CosStream::Streamable()
{
    external_from_id_.length(4);
    external_from_id_[0].id = CORBA::string_dup("::CosCompoundExternalization::Node");
    external_from_id_[1].id = CORBA::string_dup("::CosStream::Streamable");
    external_from_id_[2].id = CORBA::string_dup("");
    external_from_id_[3].id = CORBA::string_dup("NodeStreamableFactory");
    external_from_id_[0].kind = CORBA::string_dup( _lc_ks_object_interface );
    external_from_id_[1].kind = CORBA::string_dup( _lc_ks_impl_equiv_class );
    external_from_id_[2].kind = CORBA::string_dup( _lc_ks_object_implementation );
    external_from_id_[3].kind = CORBA::string_dup( _lc_ks_factory_interface );
    _related_object = CosRelationships::RelatedObject::_duplicate(obj);
}

CosLifeCycle::Key*
CosCompoundExternalization_impl::Node_impl::external_form_id()
{
    return new CosLifeCycle::Key(external_from_id_);
}

void
CosCompoundExternalization_impl::Node_impl::externalize_to_stream(CosStream::StreamIO_ptr targetStreamIO)
{
    CosCompoundExternalization::Node_ptr _node = this->_this();
    targetStreamIO -> write_graph( _node );
}

void
CosCompoundExternalization_impl::Node_impl::internalize_from_stream(CosStream::StreamIO_ptr sourceStreamIO,
                        					    CosLifeCycle::FactoryFinder_ptr there)
{
    CosCompoundExternalization::Node_ptr _node = this->_this();
    sourceStreamIO -> read_graph( _node, there );
}

void
CosCompoundExternalization_impl::Node_impl::externalize_node(CosStream::StreamIO_ptr sio)
{
//  Write out it's non-object data using the primitive StreamIO::write_...(data) functions
    sio -> write_unsigned_long(this->random_id);
    sio -> write_unsigned_short(_roles->length());
//  Write out other objects using the StreamIO::write_object(...) function
    CosStream::Streamable_ptr object_ptr = CosStream::Streamable::_narrow(_related_object);
    sio -> write_object(object_ptr);

//  Node writes its role objects using the Role::externalize_role(...) operation
    for(CORBA::ULong i = 0;i < _roles->length();++i)
    {
	CosCompoundExternalization::Role_ptr tmp_role_ptr =
		    CosCompoundExternalization::Role::_narrow((*_roles)[i]);
	if (CORBA::is_nil(tmp_role_ptr))
	{
	    sio -> write_object((CosStream::Streamable_ptr)NULL);
	    continue;
	}
	tmp_role_ptr -> externalize_role(sio);
    }
}

void
CosCompoundExternalization_impl::Node_impl::internalize_node(CosStream::StreamIO_ptr sio,
                    					     CosLifeCycle::FactoryFinder_ptr there,
                    					     CosGraphs::Node::Roles_out rolesOfNode)
{
//  Node read its non-object data using the primitive StreamIO::read_...(data) function
    random_id = sio -> read_unsigned_long();
    CORBA::UShort role_count = sio -> read_unsigned_short();
//  Node read other objects using the StreamIO::read_object(...) function
    this -> _related_object = sio -> read_object(there,(CosStream::Streamable_ptr)NULL);
//  Node reads its role objects using the Role::internalize_role(...) operation
    for(CORBA::UShort i = 0;i < role_count;++i)
    {
	CORBA::ULong key_count = sio -> read_unsigned_long();
	CosLifeCycle::Key factory_key;
	factory_key.length(key_count);
	CORBA::ULong j;
	for(j = 0;j < key_count;++j)
	{
	    char * id;
	    id = sio -> read_string();
	    factory_key[j].id = CORBA::string_dup(id);
	    switch(j) {
		case 0:
		    factory_key[j].kind = CORBA::string_dup(_lc_ks_object_interface);
		    break;
		case 1:
		    factory_key[j].kind = CORBA::string_dup(_lc_ks_impl_equiv_class);
		    break;
		case 2:
		    factory_key[j].kind = CORBA::string_dup(_lc_ks_object_implementation );
		    break;
		case 3:
		    factory_key[j].kind = CORBA::string_dup(_lc_ks_factory_interface);
		    break;
		default:
		    factory_key[j].kind = CORBA::string_dup("Custom kind");
	    }
	    CORBA::string_free(id);
	}

	CosLifeCycle::Factories* role_factories_ptr = there -> find_factories(factory_key);
	if (!(*role_factories_ptr).length())
	{
	    delete role_factories_ptr;
	    CosLifeCycle::NoFactory ex;
	    ex.search_key = factory_key;
	    mico_throw (ex);
	}
	for (j = 0;j < (*role_factories_ptr).length();++j)
	{
	    CosRelationships::RoleFactory_ptr rolefactory_ptr =
		    CosRelationships::RoleFactory::_narrow((*role_factories_ptr)[j]);
	    if (CORBA::is_nil(rolefactory_ptr))
		continue;
	    CosRelationships::RelatedObject_ptr _obj = this->_this();
	    CosCompoundExternalization::Role_ptr new_role_ptr =
		CosCompoundExternalization::Role::_narrow((*rolefactory_ptr).create_role(_obj));
	    if (CORBA::is_nil(new_role_ptr))
		continue;
	    new_role_ptr->internalize_role(sio);
	    this->add_role(new_role_ptr);
	    CORBA::release(rolefactory_ptr);
	    break;
	}
	if (j == (*role_factories_ptr).length())
	{
	    delete role_factories_ptr;
	    CosLifeCycle::NoFactory ex;
	    ex.search_key = factory_key;
	    mico_throw (ex);
	}
	delete role_factories_ptr;
    }
    rolesOfNode = this->roles_of_node();
}

CosCompoundExternalization_impl::Role_impl::Role_impl()
    : ::Role_impl(),
      CosGraphs_impl::Role_impl(),
      POA_CosCompoundExternalization::Role()
{
    external_from_id_.length(4);
    external_from_id_[0].id = CORBA::string_dup("::CosCompoundExternalization::Role");
    external_from_id_[1].id = CORBA::string_dup("");
    external_from_id_[2].id = CORBA::string_dup("");
    external_from_id_[3].id = CORBA::string_dup("RoleFactory");
    external_from_id_[0].kind = CORBA::string_dup( _lc_ks_object_interface );
    external_from_id_[1].kind = CORBA::string_dup( _lc_ks_impl_equiv_class );
    external_from_id_[2].kind = CORBA::string_dup( _lc_ks_object_implementation );
    external_from_id_[3].kind = CORBA::string_dup( _lc_ks_factory_interface );
}

CosCompoundExternalization_impl::Role_impl::Role_impl(CosRelationships::RelatedObject_ptr obj)
    : ::Role_impl(),
      CosGraphs_impl::Role_impl(),
      POA_CosCompoundExternalization::Role()
{
    if (!CORBA::is_nil(obj) && CORBA::is_nil(CosCompoundExternalization::Node::_narrow(obj)))
    {
	CosRelationships::RoleFactory::RelatedObjectTypeError ex;
	mico_throw (ex);
    }
    rel_obj = obj;
    external_from_id_.length(4);
    external_from_id_[0].id = CORBA::string_dup("::CosCompoundExternalization::Role");
    external_from_id_[1].id = CORBA::string_dup("");
    external_from_id_[2].id = CORBA::string_dup("");
    external_from_id_[3].id = CORBA::string_dup("RoleFactory");
    external_from_id_[0].kind = CORBA::string_dup( _lc_ks_object_interface );
    external_from_id_[1].kind = CORBA::string_dup( _lc_ks_impl_equiv_class );
    external_from_id_[2].kind = CORBA::string_dup( _lc_ks_object_implementation );
    external_from_id_[3].kind = CORBA::string_dup( _lc_ks_factory_interface );
}

void
CosCompoundExternalization_impl::Role_impl::externalize_role(CosStream::StreamIO_ptr sio)
{
    sio -> write_unsigned_long(this -> external_from_id_.length());
    for(CORBA::ULong i = 0;i < external_from_id_.length();++i)
	sio -> write_string(external_from_id_[i].id);
//    sio -> write_long(this->cardinality);
//    sio -> write_long(this->min_cardinality);
//    sio -> write_long(this->max_cardinality);
}

void
CosCompoundExternalization_impl::Role_impl::internalize_role(CosStream::StreamIO_ptr sio)
{
//    this -> set_cardinality(sio->read_long());
//    this -> set_min_cardinality(sio->read_long());
//    this -> set_max_cardinality(sio->read_long());
}

CosCompoundExternalization_impl::Relationship_impl::Relationship_impl()
    : ::Relationship_impl(),
      POA_CosCompoundExternalization::Relationship()
{
    external_from_id_.length(4);
    external_from_id_[0].id = CORBA::string_dup("::CosCompoundExternalization::Relationship");
    external_from_id_[1].id = CORBA::string_dup("");
    external_from_id_[2].id = CORBA::string_dup("");
    external_from_id_[3].id = CORBA::string_dup("RelationshipFactory");
    external_from_id_[0].kind = CORBA::string_dup( _lc_ks_object_interface );
    external_from_id_[1].kind = CORBA::string_dup( _lc_ks_impl_equiv_class );
    external_from_id_[2].kind = CORBA::string_dup( _lc_ks_object_implementation );
    external_from_id_[3].kind = CORBA::string_dup( _lc_ks_factory_interface );
}

CosCompoundExternalization_impl::Relationship_impl::Relationship_impl(CosRelationships::NamedRoles nr)
    : ::Relationship_impl(),
      POA_CosCompoundExternalization::Relationship()
{
    external_from_id_.length(4);
    external_from_id_[0].id = CORBA::string_dup("::CosCompoundExternalization::Relationship");
    external_from_id_[1].id = CORBA::string_dup("");
    external_from_id_[2].id = CORBA::string_dup("");
    external_from_id_[3].id = CORBA::string_dup("RelationshipFactory");
    external_from_id_[0].kind = CORBA::string_dup( _lc_ks_object_interface );
    external_from_id_[1].kind = CORBA::string_dup( _lc_ks_impl_equiv_class );
    external_from_id_[2].kind = CORBA::string_dup( _lc_ks_object_implementation );
    external_from_id_[3].kind = CORBA::string_dup( _lc_ks_factory_interface );
    n_roles = nr;
}

void
CosCompoundExternalization_impl::Relationship_impl::externalize_relationship(CosStream::StreamIO_ptr sio)
{
    sio -> write_unsigned_long(this -> external_from_id_.length());
    for(CORBA::ULong i = 0;i < external_from_id_.length();++i)
	sio -> write_string(external_from_id_[i].id);
//    sio -> write_unsigned_long(this->random_id);
    sio -> write_unsigned_long(this->random_id);
}

void
CosCompoundExternalization_impl::Relationship_impl::internalize_relationship(CosStream::StreamIO_ptr sio,
                                    					     const CosGraphs::NamedRoles& newRoles)
{
    random_id = sio -> read_unsigned_long();
}

CosGraphs::PropagationValue
CosCompoundExternalization_impl::Relationship_impl::externalize_propagation(const char* from_role_name,
                                            				    const char* to_role_name,
                                            				    CORBA::Boolean& same_for_all)
{

    CosCompoundExternalization::RelationshipHandle tmp_rel_hand;
    tmp_rel_hand.constantRandomId = this -> constant_random_id();
    tmp_rel_hand.theRelationship = CosCompoundExternalization::Relationship::_duplicate(this->_this());
    CosRelationships::NamedRoles* relation_roles_ptr = this -> named_roles();
    CosCompoundExternalization::Role_var from_role_var;
    CORBA::Boolean from_ = false;
    for(CORBA::ULong i = 0;i < (*relation_roles_ptr).length() && !from_;++i)
	if (!strcmp((*relation_roles_ptr)[i].name,from_role_name))
	{
	    from_ = true;
	    from_role_var = CosCompoundExternalization::Role::_narrow((*relation_roles_ptr)[i].aRole);
	}
    delete relation_roles_ptr;
    if (!from_)
	return CosGraphs::none;
    return from_role_var -> externalize_propagation(tmp_rel_hand,to_role_name,same_for_all);
}
