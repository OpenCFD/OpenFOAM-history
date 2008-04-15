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

#ifndef __CosCompoundExternalization_impl_h__
#define __CosCompoundExternalization_impl_h__

#include <coss/CosExternalization.h>
#include <coss/Node_impl.h>
#include <coss/GraphsRole_impl.h>
#include <coss/Relationship_impl.h>

namespace CosCompoundExternalization_impl {
    class Node_impl : virtual public POA_CosCompoundExternalization::Node,
		      virtual public POA_CosStream::Streamable,
		      virtual public ::Node_impl
    {
	CosLifeCycle::Key external_from_id_;

    public:
	Node_impl ( );
	Node_impl ( CosRelationships::RelatedObject_ptr obj);

	virtual CosLifeCycle::Key* external_form_id();

	virtual void externalize_to_stream(CosStream::StreamIO_ptr targetStreamIO);

	virtual void internalize_from_stream(CosStream::StreamIO_ptr sourceStreamIO,
                            		     CosLifeCycle::FactoryFinder_ptr there);

	virtual void externalize_node(CosStream::StreamIO_ptr sio);

	virtual void internalize_node(CosStream::StreamIO_ptr sio,
                    		      CosLifeCycle::FactoryFinder_ptr there,
                		      CosGraphs::Node::Roles_out rolesOfNode);
    };


    class Role_impl : virtual public POA_CosCompoundExternalization::Role,
		      virtual public CosGraphs_impl::Role_impl
    {
    protected:
	CosLifeCycle::Key external_from_id_;
//	typedef SequenceTmpl<CosGraphs::Traversal::TraversalScopedId,MICO_TID_DEF> IdSeq;
//	typedef TSeqVar<SequenceTmpl<CosGraphs::Traversal::TraversalScopedId,MICO_TID_DEF> > IdSeq_var;
//	IdSeq relationships_id;
//	void append_id(CosGraphs::Traversal::TraversalScopedId id) { relationships_id.append(id); };
//	CORBA::Boolean id_in_relationships(CosGraphs::Traversal::TraversalScopedId id);

    public:
	Role_impl();
	Role_impl(CosRelationships::RelatedObject_ptr obj);
    
	friend void CosCompoundExternalization_impl::Node_impl::internalize_node(CosStream::StreamIO_ptr sio,
                        							 CosLifeCycle::FactoryFinder_ptr there,
										 CosGraphs::Node::Roles_out rolesOfNode);
	virtual void externalize_role(CosStream::StreamIO_ptr sio);

	virtual void internalize_role(CosStream::StreamIO_ptr sio);
    };

    class Relationship_impl : virtual public POA_CosCompoundExternalization::Relationship,
			      virtual public ::Relationship_impl
    {
    protected:
	CosLifeCycle::Key external_from_id_;

    public:
	Relationship_impl ();
	Relationship_impl (CosRelationships::NamedRoles nr);

	friend void CosStream::StreamIO::read_graph(CosCompoundExternalization::Node_ptr starting_node,
            					    CosLifeCycle::FactoryFinder_ptr there);

	virtual void externalize_relationship(CosStream::StreamIO_ptr sio);

	virtual void internalize_relationship(CosStream::StreamIO_ptr sio,
                            		      const CosGraphs::NamedRoles& newRoles);
	virtual CosGraphs::PropagationValue externalize_propagation(const char* from_role_name,
                                            			    const char* to_role_name,
                                            			    CORBA::Boolean& same_for_all);
    };

};

#endif /*__CosCompoundExternalization_impl_h__*/
