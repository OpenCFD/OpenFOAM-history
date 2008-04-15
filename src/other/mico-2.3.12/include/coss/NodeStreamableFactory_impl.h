/*
 *  Externaization Service for MICO
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


#ifndef __NodeStreamableFactory_impl_h__
#define __NodeStreamableFactory_impl_h__

#include <coss/NodeFactory_impl.h>
#include <coss/CosExternalization.h>

// Implementation for interface NodeFactory
template <class N>
class NodeStreamableFactory_impl : virtual public NodeFactory_impl<N>,
				   virtual public POA_CosCompoundExternalization::NodeStreamableFactory
{
public:
    NodeStreamableFactory_impl (CORBA::ORB_ptr _orb);
    virtual CosStream::Streamable_ptr create_uninitialized();
};


template <class N>
NodeStreamableFactory_impl<N>::NodeStreamableFactory_impl (CORBA::ORB_ptr _orb)
    : NodeFactory_impl<N>(_orb)
    , POA_CosCompoundExternalization::NodeStreamableFactory()
{
}

template <class N>
CosStream::Streamable_ptr
NodeStreamableFactory_impl<N>::create_uninitialized ()
{
#ifdef DEBUG
    cout << "NodeFactory_impl::create_uninitialized ()\n";
#endif
    N* tmp_node = new N();
    CosCompoundExternalization::Node_ptr node = tmp_node->_this ();

    return node;
}

#endif

