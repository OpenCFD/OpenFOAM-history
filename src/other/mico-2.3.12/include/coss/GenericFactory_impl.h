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


#ifndef __GenericFactory_impl_h__
#define __GenericFactory_impl_h__

#include <coss/CosLifeCycle.h>
#include <coss/RegisterHelper.h>


class GenericFactory_impl : virtual public POA_CosLifeCycle::GenericFactory
{
	CORBA::ORB_var orb_;
	CosTrading::Lookup_var trader_;
	CosNaming::NamingContext_var naming_;
	CORBA::Boolean nsupport (const CosLifeCycle::Key &factory_key);
	CORBA::Boolean tsupport (const CosLifeCycle::Key &factory_key);
	CORBA::Object_ptr ncreate_object (const CosLifeCycle::Key &factory_key,
									  const CosLifeCycle::Criteria &the_criteria);
	CORBA::Object_ptr tcreate_object (const CosLifeCycle::Key &factory_key,
									  const CosLifeCycle::Criteria &the_criteria);
public:
	GenericFactory_impl ();
	GenericFactory_impl (CORBA::ORB_ptr orb, RegisterHelper::FinderType);
	GenericFactory_impl (CORBA::ORB_ptr orb, CosTrading::Lookup_ptr trader);
	GenericFactory_impl (CORBA::ORB_ptr orb, CosNaming::NamingContext_ptr naming);
	~GenericFactory_impl ();

	CORBA::Boolean support (const CosLifeCycle::Key &factory_key);
  // Returns true if the Generic Factory is able to forward a request
  // for creating an object described by the <factory_key>.

	CORBA::Object_ptr create_object (const CosLifeCycle::Key &factory_key,
									 const CosLifeCycle::Criteria &the_criteria);
  // Returns an object reference to a newly created object, though the
  // Generic Factory itself cannot create objects, it will forward the
  // request to a more concrete Factory.
};

#endif /* __GenericFactory_impl_h__ */
