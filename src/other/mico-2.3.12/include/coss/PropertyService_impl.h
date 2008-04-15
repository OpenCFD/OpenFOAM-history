/*
 *  Object Property Service for MICO
 *  Copyright (C) 1998 Leif Jakobsmeier
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
 */

#ifndef __PropertyService_impl_h__
#define __PropertyService_impl_h__

#include <coss/PropertyService.h>

/*****************************************************************************/
/*****************************************************************************/
/*                   The PropertySetFactory Interface                        */
/*****************************************************************************/
/*****************************************************************************/

class PropertySetFactory_impl
    : public virtual POA_PropertyService::PropertySetFactory
{
private:

public:
  /* construktor */
  PropertySetFactory_impl ();

  /* destructor */
  virtual ~PropertySetFactory_impl ();

  virtual PropertyService::PropertySet_ptr 
    create_propertyset ();

  virtual PropertyService::PropertySet_ptr
    create_constrained_propertyset (const PropertyService::PropertyTypes
                                      &allowed_property_types,
                                    const PropertyService::Properties
                                      &allowed_properties);

  virtual PropertyService::PropertySet_ptr
    create_initial_propertyset (const PropertyService::Properties
                                  &initial_properties);
};


/*****************************************************************************/
/*****************************************************************************/
/*                 The PropertySetDefFactory Interface                       */
/*****************************************************************************/
/*****************************************************************************/

class PropertySetDefFactory_impl
    : public virtual POA_PropertyService::PropertySetDefFactory
{
private:

public:
  /* construktor */
  PropertySetDefFactory_impl ();

  /* destructor */
  virtual ~PropertySetDefFactory_impl ();

  virtual PropertyService::PropertySetDef_ptr 
    create_propertysetdef ();

  virtual PropertyService::PropertySetDef_ptr
    create_constrained_propertysetdef (const PropertyService::PropertyTypes
                                         &allowed_property_types,
                                       const PropertyService::PropertyDefs
                                         &allowed_property_defs);

  virtual PropertyService::PropertySetDef_ptr
    create_initial_propertysetdef (const PropertyService::PropertyDefs
                                     &initial_property_defs);

};

/*****************************************************************************/
/*****************************************************************************/
/*                      The PropertySet Interface                            */
/*****************************************************************************/
/*****************************************************************************/

class PropertySet_impl
    : public virtual POA_PropertyService::PropertySet
{
friend class PropertyNamesIterator_impl;
friend class PropertiesIterator_impl;

private:

protected:
  std::vector<PropertyService::PropertyDef_var> mv_properties;
  std::vector<CORBA::TypeCode_var> mv_allowed_property_types;
  std::vector<PropertyService::PropertyDef_var> mv_allowed_properties;
  MICOMT::Mutex ps_lock_;

  CORBA::Boolean 
    get_index (const char *property_name,
               CORBA::ULong *idx);
 
  PropertyService::PropertyName
    get_name (CORBA::ULong idx);

  CORBA::Any*
    get_value (CORBA::ULong idx);
 
  CORBA::Boolean
    is_property_name_valid (const char *name);

  CORBA::Boolean
    is_property_type_allowed (CORBA::TypeCode_ptr property_type);

  CORBA::Boolean
    is_property_allowed (const char *name,
                         const CORBA::Any &value,
                         const PropertyService::PropertyModeType &mode_type);

  CORBA::Boolean
    is_property_readonly (CORBA::ULong idx);

  CORBA::Boolean
    is_property_fixed (CORBA::ULong idx);

  void
    def_prop (const char *property_name,
              const CORBA::Any &property_value,
              const PropertyService::PropertyModeType &mode_type); 

  void 
    def_props (const PropertyService::Properties &nproperties,
               const PropertyService::PropertyModeType &mode_type);

public:
  /* constructor */
  PropertySet_impl ();

  /* Constructor used from PropertySetFactory::create_constrained_propertyset */
  PropertySet_impl(const PropertyService::PropertyTypes &allowed_property_types,
                   const PropertyService::Properties &allowed_properties);

  /* Constructor used from PropertySetFactory::create_initial_propertyset */
  PropertySet_impl (const PropertyService::Properties &initial_properties);

  /* destructor */
  virtual ~PropertySet_impl ();
  
  /*************************************/
  /* Defining and Modifying Properties */
  /*************************************/
  virtual void
    define_property (const char *property_name,
                     const CORBA::Any &property_value);

  virtual void 
    define_properties (const PropertyService::Properties &nproperties);

  virtual CORBA::Boolean
    delete_all_properties();

  /**********************************/
  /* Listing and Getting Properties */
  /**********************************/
  virtual CORBA::ULong
    get_number_of_properties ();

  virtual void
    get_all_property_names (CORBA::ULong how_many,
                            PropertyService::PropertyNames_out property_names,
                            PropertyService::PropertyNamesIterator_out rest);

  virtual CORBA::Any *
    get_property_value (const char *property_name);

  virtual CORBA::Boolean
    get_properties (const PropertyService::PropertyNames &property_names,
                    PropertyService::Properties_out nproperties);

  virtual void
    get_all_properties (CORBA::ULong how_many,
                        PropertyService::Properties_out nproperties,
                        PropertyService::PropertiesIterator_out rest);

  /***********************/
  /* Deleting Properties */
  /***********************/
  virtual void 
    delete_property (const char *property_name);

  virtual void 
    delete_properties (const PropertyService::PropertyNames &property_names);  

  /************************************************/
  /* Determining if a Property is already defined */
  /************************************************/
  virtual CORBA::Boolean 
    is_property_defined (const char *property_name);
};


/*****************************************************************************/
/*****************************************************************************/
/*                    The PropertySetDef Interface                           */
/*****************************************************************************/
/*****************************************************************************/


class PropertySetDef_impl
    : virtual public PropertySet_impl,
      virtual public POA_PropertyService::PropertySetDef
{
private:

public:
  /* constructor */
  PropertySetDef_impl ();

  /* Constructor used from PropertySetFactory::create_constrained_propertyset */
  PropertySetDef_impl (const PropertyService::PropertyTypes 
                         &allowed_property_types,
                       const PropertyService::PropertyDefs 
                         &allowed_property_defs);

  /* Constructor used from PropertySetFactory::create_initial_propertyset */
  PropertySetDef_impl (const PropertyService::PropertyDefs 
                         &initial_property_defs);

  /* destructor */
  virtual ~PropertySetDef_impl ();

  /****************************************************/
  /* Support for retrieval of PropertySet constraints */
  /****************************************************/
 
  virtual void
    get_allowed_property_types(PropertyService::PropertyTypes_out property_types);

  virtual void
    get_allowed_properties (PropertyService::PropertyDefs_out property_defs);

  /*************************************************/
  /* Support for defining and modifiying poperties */
  /*************************************************/

  virtual void
    define_property_with_mode (const char *property_name,
                               const CORBA::Any &property_value,
                               PropertyService::PropertyModeType 
                                 property_mode);

  virtual void 
    define_properties_with_modes (const PropertyService::PropertyDefs 
                                    &property_defs);

  /**************************************************/
  /* Support for Getting and Setting Property Modes */
  /**************************************************/
 
  virtual PropertyService::PropertyModeType
    get_property_mode (const char *property_name);

  virtual CORBA::Boolean
    get_property_modes (const PropertyService::PropertyNames &property_names,
                        PropertyService::PropertyModes_out property_modes); 

  virtual void
    set_property_mode (const char *property_name,
                       PropertyService::PropertyModeType property_mode);

  virtual void
    set_property_modes (const PropertyService::PropertyModes &property_modes);
};


/*****************************************************************************/
/*****************************************************************************/
/*                  The PropertyNamesIterator Interface                      */
/*****************************************************************************/
/*****************************************************************************/

class PropertyNamesIterator_impl
    : public virtual POA_PropertyService::PropertyNamesIterator,
      public virtual PortableServer::RefCountServantBase
{
private:
  PropertySet_impl *mp_property_set;
  CORBA::ULong m_index;
  MICOMT::Mutex pni_lock_;

public:
  /* construktor */
  PropertyNamesIterator_impl ();

  PropertyNamesIterator_impl (PropertySet_impl *pro_set, CORBA::ULong idx);

  /* destructor */
  virtual ~PropertyNamesIterator_impl();

  virtual void 
    reset();

  virtual CORBA::Boolean
    next_one (CORBA::String_out property_name);

  virtual CORBA::Boolean
    next_n (CORBA::ULong how_many,
            PropertyService::PropertyNames_out property_names);

  virtual void
    destroy ();
};


/*****************************************************************************/
/*****************************************************************************/
/*                     The PropertiesIterator Interface                      */
/*****************************************************************************/
/*****************************************************************************/

class PropertiesIterator_impl
    : public virtual POA_PropertyService::PropertiesIterator,
      public virtual PortableServer::RefCountServantBase
{
private:
  PropertySet_impl *mp_property_set;
  CORBA::ULong m_index;
  MICOMT::Mutex pi_lock_;

public:
  /* construktor */
  PropertiesIterator_impl ();

  PropertiesIterator_impl (PropertySet_impl *pro_set, CORBA::ULong idx);

  /* destructor */
  virtual ~PropertiesIterator_impl();

  virtual void 
    reset();

  virtual CORBA::Boolean
    next_one (PropertyService::Property_out aproperty);

  virtual CORBA::Boolean
    next_n (CORBA::ULong how_many,
            PropertyService::Properties_out nproperties);

  virtual void
    destroy ();
};

#endif //__PropertyService_impl_h__
