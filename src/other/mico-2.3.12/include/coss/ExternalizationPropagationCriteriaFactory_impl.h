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

#ifndef __ExternalizationPropagationCriteriaFactory_impl_h__
#define __ExternalizationPropagationCriteriaFactory_impl_h__

#include <coss/CosExternalization.h>
#include <coss/ExternalizationTraversalCriteria_impl.h>

namespace CosCompoundExternalization_impl {
    class PropagationCriteriaFactory_impl : virtual public POA_CosCompoundExternalization::PropagationCriteriaFactory
    {
    public:
	PropagationCriteriaFactory_impl();
	virtual CosGraphs::TraversalCriteria_ptr create_for_externalize();
    };

};
#endif  __ExternalizationPropagationCriteriaFactory_impl_h__
