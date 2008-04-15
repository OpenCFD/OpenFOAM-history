/*
 *  LifeCycle Service for MICO
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

#ifndef __CosCompoundLifeCycle_Node_impl_h__
#define __CosCompoundLifeCycle_Node_impl_h__

#include <coss/CosCompoundLifeCycle.h>
#include <coss/Node_impl.h>

namespace CosCompoundLifeCycle_impl {
    class Node_impl : virtual public ::Node_impl, 
                      virtual public POA_CosCompoundLifeCycle::Node
    {
    protected:
	CosLifeCycle::Key factory_id_;

    public:
	Node_impl ();
	Node_impl (CosRelationships::RelatedObject_ptr obj);

	virtual void copy_node(CosLifeCycle::FactoryFinder_ptr there,
                	       const CosLifeCycle::Criteria& the_criteria,
			       CosCompoundLifeCycle::Node_out new_node,
			       CosGraphs::Node::Roles_out roles_of_new_node);

	virtual void move_node(CosLifeCycle::FactoryFinder_ptr there,
    			       const CosLifeCycle::Criteria& the_criteria);

	virtual void remove_node();

	virtual CosLifeCycle::LifeCycleObject_ptr get_life_cycle_object();
    };
};

#endif  /* __CosCompoundLifeCycle_Node_impl_h__ */
