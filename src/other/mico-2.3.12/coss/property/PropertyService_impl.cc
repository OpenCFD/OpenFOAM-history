/*
 *  Object Property Service for MICO
 *  Copyright (C) 1998, 1999, 2000 Leif Jakobsmeier
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

#define MICO_CONF_POA
#define MICO_CONF_INTERCEPT
#define MICO_CONF_IR
#include <CORBA-SMALL.h>
#include <coss/PropertyService_impl.h>

#ifdef HAVE_EXCEPTIONS
#define MULTIPLE_EXCEPTIONS_CATCH(__mul_exc, __fail_prop_name)                \
  catch (PropertyService::InvalidPropertyName_catch &exp) {                   \
    CORBA::ULong len = __mul_exc.exceptions.length ();                        \
    __mul_exc.exceptions.length (len + 1);                                    \
    __mul_exc.exceptions[len].reason = PropertyService::invalid_property_name;\
    __mul_exc.exceptions[len].failing_property_name =                         \
      CORBA::string_dup (__fail_prop_name);                                   \
  }                                                                           \
  catch (PropertyService::ConflictingProperty_catch &exp) {                   \
    CORBA::ULong len = __mul_exc.exceptions.length ();                        \
    __mul_exc.exceptions.length (len + 1);                                    \
    __mul_exc.exceptions[len].reason = PropertyService::conflicting_property; \
    __mul_exc.exceptions[len].failing_property_name =                         \
      CORBA::string_dup (__fail_prop_name);                                   \
  }                                                                           \
  catch (PropertyService::PropertyNotFound_catch &exp) {                      \
    CORBA::ULong len = __mul_exc.exceptions.length ();                        \
    __mul_exc.exceptions.length (len + 1);                                    \
    __mul_exc.exceptions[len].reason = PropertyService::property_not_found;   \
    __mul_exc.exceptions[len].failing_property_name =                         \
      CORBA::string_dup (__fail_prop_name);                                   \
  }                                                                           \
  catch (PropertyService::UnsupportedTypeCode_catch &exp) {                   \
    CORBA::ULong len = __mul_exc.exceptions.length ();                        \
    __mul_exc.exceptions.length (len + 1);                                    \
    __mul_exc.exceptions[len].reason = PropertyService::unsupported_type_code;\
    __mul_exc.exceptions[len].failing_property_name =                         \
      CORBA::string_dup (__fail_prop_name);                                   \
  }                                                                           \
  catch (PropertyService::UnsupportedProperty_catch &exp) {                   \
    CORBA::ULong len = __mul_exc.exceptions.length ();                        \
    __mul_exc.exceptions.length (len + 1);                                    \
    __mul_exc.exceptions[len].reason = PropertyService::unsupported_property; \
    __mul_exc.exceptions[len].failing_property_name =                         \
      CORBA::string_dup (__fail_prop_name);                                   \
  }                                                                           \
  catch (PropertyService::UnsupportedMode_catch &exp) {                       \
    CORBA::ULong len = __mul_exc.exceptions.length ();                        \
    __mul_exc.exceptions.length (len + 1);                                    \
    __mul_exc.exceptions[len].reason = PropertyService::unsupported_mode;     \
    __mul_exc.exceptions[len].failing_property_name =                         \
      CORBA::string_dup (__fail_prop_name);                                   \
  }                                                                           \
  catch (PropertyService::FixedProperty_catch &exp) {                         \
    CORBA::ULong len = __mul_exc.exceptions.length ();                        \
    __mul_exc.exceptions.length (len + 1);                                    \
    __mul_exc.exceptions[len].reason = PropertyService::fixed_property;       \
    __mul_exc.exceptions[len].failing_property_name =                         \
      CORBA::string_dup (__fail_prop_name);                                   \
  }                                                                           \
  catch (PropertyService::ReadOnlyProperty_catch &exp) {                      \
    CORBA::ULong len = __mul_exc.exceptions.length ();                        \
    __mul_exc.exceptions.length (len + 1);                                    \
    __mul_exc.exceptions[len].reason = PropertyService::read_only_property;   \
    __mul_exc.exceptions[len].failing_property_name =                         \
      CORBA::string_dup (__fail_prop_name);                                   \
  }
#else
#define MULTIPLE_EXCEPTIONS_CATCH(__mul_exc, __fail_prop_name)
#endif


/*****************************************************************************/
/*****************************************************************************/
/*                   The PropertySetFactory Interface                        */
/*****************************************************************************/
/*****************************************************************************/

PropertySetFactory_impl::PropertySetFactory_impl ()
{
}

PropertySetFactory_impl::~PropertySetFactory_impl ()
{
}

PropertyService::PropertySet_ptr
PropertySetFactory_impl::create_propertyset ()
{
  PropertySet_impl* pset = new PropertySet_impl ();
  PropertyService::PropertySet_ptr pset_ref = pset->_this();
  return pset_ref;
}

PropertyService::PropertySet_ptr
PropertySetFactory_impl::create_constrained_propertyset
                  (const PropertyService::PropertyTypes &allowed_property_types,
                   const PropertyService::Properties &allowed_properties)
{
  PropertySet_impl* new_ps;
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    new_ps = new PropertySet_impl (allowed_property_types, allowed_properties);
  #ifdef HAVE_EXCEPTIONS
  }
  catch (PropertyService::InvalidPropertyName_catch &exp) {
    mico_throw (PropertyService::ConstraintNotSupported ());
  }
  catch (PropertyService::UnsupportedTypeCode_catch &exp) {
    mico_throw (PropertyService::ConstraintNotSupported ());
  }
  #endif
  PropertyService::PropertySet_ptr ref = new_ps->_this();
  return ref;
}

PropertyService::PropertySet_ptr
PropertySetFactory_impl::create_initial_propertyset 
                   (const PropertyService::Properties &initial_properties)
{
  PropertySet_impl* new_ps = new PropertySet_impl (initial_properties);
  PropertyService::PropertySet_ptr ref = new_ps->_this();
  return ref;
}

/*****************************************************************************/
/*****************************************************************************/
/*                 The PropertySetDefFactory Interface                       */
/*****************************************************************************/
/*****************************************************************************/

PropertySetDefFactory_impl::PropertySetDefFactory_impl ()
{
}

PropertySetDefFactory_impl::~PropertySetDefFactory_impl ()
{
}

PropertyService::PropertySetDef_ptr
PropertySetDefFactory_impl::create_propertysetdef ()
{
  PropertySetDef_impl* psdef = new PropertySetDef_impl ();
  PropertyService::PropertySetDef_ptr ref = psdef->_this();
  return ref;
}

PropertyService::PropertySetDef_ptr
PropertySetDefFactory_impl::create_constrained_propertysetdef
                  (const PropertyService::PropertyTypes &allowed_property_types,
                   const PropertyService::PropertyDefs &allowed_property_defs)
{
  PropertySetDef_impl* new_psd = NULL;
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    new_psd = 
      new PropertySetDef_impl (allowed_property_types, allowed_property_defs);
  #ifdef HAVE_EXCEPTIONS
  }
  catch (PropertyService::InvalidPropertyName_catch &exp) {
    mico_throw (PropertyService::ConstraintNotSupported ());
  }
  catch (PropertyService::UnsupportedTypeCode_catch &exp) {
    mico_throw (PropertyService::ConstraintNotSupported ());
  }
  #endif
  PropertyService::PropertySetDef_ptr ref = new_psd->_this();
  return ref;
}

PropertyService::PropertySetDef_ptr
PropertySetDefFactory_impl::create_initial_propertysetdef 
                   (const PropertyService::PropertyDefs &initial_property_defs)
{
  PropertySetDef_impl* new_psd = new PropertySetDef_impl (initial_property_defs);
  PropertyService::PropertySetDef_ptr ref = new_psd->_this();
  return ref;
}

/*****************************************************************************/
/*****************************************************************************/
/*                      The PropertySet Interface                            */
/*****************************************************************************/
/*****************************************************************************/

PropertySet_impl::PropertySet_impl ()
    : ps_lock_(FALSE, MICOMT::Mutex::Recursive)
{
}

/* Used by PropertySetFactory::create_constrained_propertyset (...) */
PropertySet_impl::PropertySet_impl (const PropertyService::PropertyTypes
                                      &allowed_property_types,
                                    const PropertyService::Properties
                                      &allowed_properties)
    : ps_lock_(FALSE, MICOMT::Mutex::Recursive)
{
  CORBA::ULong len = allowed_property_types.length ();

  for (CORBA::ULong idx0 = 0; idx0 < len; idx0++)
    mv_allowed_property_types.push_back (allowed_property_types [idx0]);
 
  len = allowed_properties.length ();

  for (CORBA::ULong idx = 0; idx < len; idx++)
  {
    /* Check if PropertyName is valid */
    if (! is_property_name_valid (allowed_properties [idx].property_name)) {
      mico_throw (PropertyService::InvalidPropertyName ());
    }
    /* Check if there are any conflicts between the constraints */
    if (! is_property_type_allowed 
            (allowed_properties [idx].property_value.type ()))
    {
      mico_throw (PropertyService::UnsupportedTypeCode ());
    }
    PropertyService::PropertyDef_var new_property = 
      new PropertyService::PropertyDef ();

    new_property->property_name = 
      CORBA::string_dup (allowed_properties [idx].property_name);

    new_property->property_value = allowed_properties [idx].property_value;

    /* All PropertyModeTypes are allowed */
    new_property->property_mode  = PropertyService::undefined;

    mv_allowed_properties.push_back (new_property); 
  }
  
}

/* Used by PropertySetFactory::create_initial_propertyset (...) */
PropertySet_impl::PropertySet_impl (const PropertyService::Properties
                                      &initial_properties)
    : ps_lock_(FALSE, MICOMT::Mutex::Recursive)
{
  def_props (initial_properties, PropertyService::fixed_normal);
}

PropertySet_impl::~PropertySet_impl ()
{
}


/***********************/
/** Protected Methods **/
/***********************/

CORBA::Boolean
PropertySet_impl::get_index (const char *property_name,
                             CORBA::ULong *idx)
{
  MICOMT::AutoLock t_lock(ps_lock_);
  CORBA::UShort max = get_number_of_properties ();
  for(CORBA::UShort i = 0; i < max; i++)
  {
    if (strcmp(get_name(i), property_name) == 0) {
      /* property_name found */
      *idx = i;
      return TRUE;
    }
  }  
  /* property_name not found */
  return FALSE;
}

PropertyService::PropertyName
PropertySet_impl::get_name (CORBA::ULong idx)
{
  MICOMT::AutoLock t_lock(ps_lock_);
  assert (idx < get_number_of_properties ());
  return mv_properties[idx]->property_name;
}

CORBA::Any *
PropertySet_impl::get_value (CORBA::ULong idx)
{
  MICOMT::AutoLock t_lock(ps_lock_);
  assert (idx < get_number_of_properties ());
  return &mv_properties[idx]->property_value;
}

CORBA::Boolean
PropertySet_impl::is_property_name_valid (const char *name)
{
  if (strcmp (name, "") == 0)
    return FALSE;
  return TRUE;
}

CORBA::Boolean
PropertySet_impl::is_property_type_allowed (CORBA::TypeCode_ptr 
                                            property_type)
{
  MICOMT::AutoLock t_lock(ps_lock_);
  CORBA::ULong len = mv_allowed_property_types.size (); 
  
  if (len == 0)
    return TRUE;
  
  for (CORBA::ULong idx = 0; idx < len; idx++)
  {
    if (property_type->equal (mv_allowed_property_types [idx]))
      return TRUE;
  }
  return FALSE;
}

/*----------------------------------------------------------------------------*/
/* Checks if the Property fills with the constrains.                          */
/* For that an Item with the same PropertyName, PropertyType and PropertyMode */
/* must exist in the Constraints-Vector (mv_allowed_properties). If the       */
/* PropertyMode in the Constraints-Vector is set to 'undefined' all Property- */
/* Modes will be excepted.                                                    */
/*----------------------------------------------------------------------------*/
CORBA::Boolean
PropertySet_impl::is_property_allowed (const char *name,
                                       const CORBA::Any &value,
                                       const PropertyService::PropertyModeType 
                                         &mode_type)
{
  MICOMT::AutoLock t_lock(ps_lock_);
  CORBA::ULong len = mv_allowed_properties.size ();

  if (len == 0)
    return TRUE;

  for (CORBA::ULong idx = 0; idx < len; idx++)
  {
    /* Check PropertyName */
    if (strcmp (name, (mv_allowed_properties[idx])->property_name) == 0) {
      CORBA::TypeCode_ptr type1 = value.type();
      CORBA::TypeCode_ptr type2 = mv_allowed_properties[idx]->
                                    property_value.type();
      /* Check PropertyType */
      if (type1->equal (type2)) {
        /* Check PropertyMode */
        if (mv_allowed_properties[idx]->property_mode == mode_type ||
            mv_allowed_properties[idx]->property_mode == 
            PropertyService::undefined)
          return TRUE;
      }
    }
  }
  return FALSE;
}

CORBA::Boolean
PropertySet_impl::is_property_readonly (CORBA::ULong idx)
{
  MICOMT::AutoLock t_lock(ps_lock_);
  if (mv_properties[idx]->property_mode == PropertyService::read_only ||
      mv_properties[idx]->property_mode == PropertyService::fixed_readonly)
    return TRUE;
  return FALSE;
}

CORBA::Boolean
PropertySet_impl::is_property_fixed (CORBA::ULong idx)
{
  MICOMT::AutoLock t_lock(ps_lock_);
  if (mv_properties[idx]->property_mode == PropertyService::fixed_normal ||
      mv_properties[idx]->property_mode == PropertyService::fixed_readonly)
    return TRUE;
  return FALSE;
}

void 
PropertySet_impl::def_prop (const char *property_name,
                            const CORBA::Any &property_value,
                            const PropertyService::PropertyModeType &mode_type)
{
  MICOMT::AutoLock t_lock(ps_lock_);
  /* Check if PropertyName is valid */
  if (! is_property_name_valid (property_name)) {
    mico_throw (PropertyService::InvalidPropertyName ());
  }

  CORBA::TypeCode_var new_property_type = property_value.type();

  /* Check if TypeCode is allowed */
  if (! is_property_type_allowed (new_property_type)) {
    mico_throw (PropertyService::UnsupportedTypeCode ());
  }

  /* Check if Property is allowed */
  if (! is_property_allowed (property_name, property_value, mode_type)) {
    mico_throw (PropertyService::UnsupportedProperty ());
  }

  /* Check if item allready exist */

  CORBA::ULong idx;
  CORBA::Boolean success = get_index (property_name, &idx);

  if (success) {
    /* Property allready exists */

    /* Check if Property is ReadOnly */
    if (is_property_readonly (idx)) {
      mico_throw (PropertyService::ReadOnlyProperty ());
    }
 
    CORBA::TypeCode_var old_property_type = 
      mv_properties[idx]->property_value.type ();

    /* Check if the new TypeCode is equal to the old */
    if (! (old_property_type->equal (new_property_type)) ) {
      mico_throw (PropertyService::ConflictingProperty ());
    }
    /* Substitute old Value */
    mv_properties[idx]->property_value = property_value;
  } else {
    /* Insert new Property */
    PropertyService::PropertyDef_var new_property =
      new PropertyService::PropertyDef ();
    new_property->property_name  = CORBA::string_dup (property_name);
    new_property->property_value = property_value;
    new_property->property_mode  = mode_type;
    mv_properties.push_back (new_property);
  }
}

void 
PropertySet_impl::def_props (const PropertyService::Properties &nproperties,
                             const PropertyService::PropertyModeType &mode_type)
{
  MICOMT::AutoLock t_lock(ps_lock_);
  CORBA::ULong len = nproperties.length();
  assert (len > 0); 

  PropertyService::MultipleExceptions mul_exc;
  
  for (CORBA::ULong idx = 0; idx < len; idx++)
  {
    #ifdef HAVE_EXCEPTIONS
    try {
    #endif
      def_prop (nproperties[idx].property_name,
                nproperties[idx].property_value,
                mode_type);
    #ifdef HAVE_EXCEPTIONS
    }
    #endif
    MULTIPLE_EXCEPTIONS_CATCH(mul_exc, nproperties[idx].property_name);
  }

  if (mul_exc.exceptions.length () != 0) {
    mico_throw (mul_exc);
  }
}


/********************/
/** Public Methods **/
/********************/

  /*************************************/
  /* Defining and Modifying Properties */
  /*************************************/  

void 
PropertySet_impl::define_property (const char *property_name,
                                   const CORBA::Any &property_value)
{
  MICOMT::AutoLock t_lock(ps_lock_);
  def_prop (property_name, property_value, PropertyService::normal);
}

void 
PropertySet_impl::define_properties (const PropertyService::Properties 
                                     &nproperties)
{
  MICOMT::AutoLock t_lock(ps_lock_);
  def_props (nproperties, PropertyService::normal);
}

  /**********************************/
  /* Listing and Getting Properties */
  /**********************************/

CORBA::ULong
PropertySet_impl::get_number_of_properties ()
{
  MICOMT::AutoLock t_lock(ps_lock_);
  return mv_properties.size ();
} 

void
PropertySet_impl::get_all_property_names (CORBA::ULong how_many, 
                                PropertyService::PropertyNames_out property_names,
                                PropertyService::PropertyNamesIterator_out rest)
{
  MICOMT::AutoLock t_lock(ps_lock_);
  CORBA::ULong len = get_number_of_properties ();

  property_names = new PropertyService::PropertyNames ();

  if (how_many < len)
    property_names->length (how_many);
  else
    property_names->length (len);
  
  for (CORBA::ULong idx = 0; idx < how_many && idx < len; idx++) {
    /* Copy Property_Name */
    (*property_names) [idx] = CORBA::string_dup (get_name (idx));
  }
  
  if (how_many >= len) {
    /* All Properties have been copyed => return an empty PropertyNamesIter. */
    PropertyNamesIterator_impl* iter = new PropertyNamesIterator_impl;
    rest = iter->_this();
    return;
  }
  /* Initiate PropertyNamesIterator */
  PropertyNamesIterator_impl* iter
    = new PropertyNamesIterator_impl (this, how_many);
  rest = iter->_this();
  return;
}

CORBA::Any *
PropertySet_impl::get_property_value (const char *property_name) 
{
  MICOMT::AutoLock t_lock(ps_lock_);
  /* Check if PropertyName is valid */
  if (! is_property_name_valid (property_name)) {
    mico_throw (PropertyService::InvalidPropertyName ());
  }

  CORBA::ULong idx;
  CORBA::Boolean success = get_index (property_name, &idx);
  if (success)
  {
    /* Property found */
    CORBA::Any *res = new CORBA::Any (mv_properties[idx]->property_value);
 
    return res;
  } 
  /* Property not found */
  mico_throw (PropertyService::PropertyNotFound ());

  assert (0);
  return new CORBA::Any;
}


CORBA::Boolean
PropertySet_impl::get_properties (const PropertyService::PropertyNames 
                                    &property_names,
                                  PropertyService::Properties_out nproperties) 
{
  MICOMT::AutoLock t_lock(ps_lock_);
  CORBA::ULong len = property_names.length ();
  assert (len > 0);
  
  /* Create new Properties Sequence */
  nproperties = new PropertyService::Properties;
  nproperties->length (len);

  CORBA::Boolean return_stat = TRUE;

  for (CORBA::ULong idx = 0; idx < len; idx++) {
    /* Copy Property_Name */
    (*nproperties)[idx].property_name = CORBA::string_dup (property_names[idx]);

    CORBA::Any *property_value;

    #ifdef HAVE_EXCEPTIONS
    try {
    #endif
      /* Try to get the value */
      property_value = get_property_value (property_names [idx]);
    #ifdef HAVE_EXCEPTIONS
    }
    /* If an Exception raises set the Property_Value to tk_void */ 
    catch (PropertyService::InvalidPropertyName_catch &exp) {
      property_value = new CORBA::Any;
      return_stat = FALSE;
    }
    catch (PropertyService::PropertyNotFound_catch &exp) {
      property_value = new CORBA::Any;
      return_stat = FALSE;
    }
    #endif

    /* Copy Property_Value */ 
    (*nproperties)[idx].property_value = *property_value;
    delete property_value;
  }
  return return_stat;
}

void
PropertySet_impl::get_all_properties (CORBA::ULong how_many, 
                                     PropertyService::Properties_out nproperties,
                                     PropertyService::PropertiesIterator_out rest)
{
  MICOMT::AutoLock t_lock(ps_lock_);
  CORBA::ULong len = get_number_of_properties();

  /* Create new Properties Sequence */
  nproperties = new PropertyService::Properties;

  if (how_many < len)
    nproperties->length (how_many);
  else
    nproperties->length (len);

  for (CORBA::ULong idx = 0; idx < how_many && idx < len ; idx++) {
    /* Copy Property_Name */
    (*nproperties)[idx].property_name = CORBA::string_dup (get_name (idx));

    /* Copy Property_Value */
    (*nproperties)[idx].property_value = *get_value (idx); 
  }

  if (how_many >= len) {
    /* All Properties have been copyed => return an empty PropertiesIterator */
    PropertiesIterator_impl* iter = new PropertiesIterator_impl ();
    rest = iter->_this();
    return;
  }
  /* Initiate PropertiesIterator */
  PropertiesIterator_impl* iter = new PropertiesIterator_impl (this, how_many);
  rest = iter->_this();
  return;
}

  /***********************/
  /* deleting properties */
  /***********************/

void
PropertySet_impl::delete_property (const char *property_name)
{
  MICOMT::AutoLock t_lock(ps_lock_);
  /* Check if PropertyName is valid */
  if (! is_property_name_valid (property_name)) {
    mico_throw (PropertyService::InvalidPropertyName ());
  }

  CORBA::ULong idx;

  /* Check if Property exists */
  CORBA::Boolean success = get_index (property_name, &idx);

  if (success) {
    /* Property found */

    /* Check if Property is Fixed */
    if (is_property_fixed (idx)) {
      mico_throw (PropertyService::FixedProperty ());
    }
    mv_properties.erase (mv_properties.begin() + idx);
    return;
  }
  /* Property not found */
  mico_throw (PropertyService::PropertyNotFound ());
}

void
PropertySet_impl::delete_properties (const PropertyService::PropertyNames 
                                      &property_names)
{
  MICOMT::AutoLock t_lock(ps_lock_);
  CORBA::ULong len = property_names.length();
  assert (len > 0);   

  PropertyService::MultipleExceptions mul_exc;

  for (CORBA::ULong i = 0; i < len; i++)
  {
    #ifdef HAVE_EXCEPTIONS
    try {
    #endif
      delete_property (property_names [i]);
    #ifdef HAVE_EXCEPTIONS
    }
    #endif
    MULTIPLE_EXCEPTIONS_CATCH(mul_exc, property_names[i]);
  }
 
  if (mul_exc.exceptions.length () != 0) {
    mico_throw (mul_exc);
  }
}

CORBA::Boolean 
PropertySet_impl::delete_all_properties()
{
  MICOMT::AutoLock t_lock(ps_lock_);
  CORBA::ULong len = get_number_of_properties();

  CORBA::Boolean success = TRUE;
  for (CORBA::Long idx = len - 1; idx >= 0; idx--)
  {
    /* Check if Property is fixed */
    if (! is_property_fixed (idx)) {
      /* Delete Property */
      mv_properties.erase (mv_properties.begin() + idx);
    } else {
      /* Skip Property */
      success = FALSE;
    }
  }
 
  return success;
}

  /************************************************/
  /* Determining if a Property is already defined */
  /************************************************/  

CORBA::Boolean 
PropertySet_impl::is_property_defined (const char *property_name)
{
  MICOMT::AutoLock t_lock(ps_lock_);
  /* Check if PropertyName is valid */
  if (! is_property_name_valid (property_name)) {
    mico_throw (PropertyService::InvalidPropertyName ());
  }

  CORBA::ULong idx;
  CORBA::Boolean success = get_index (property_name, &idx);
  if (success)
    return TRUE;
  else
    return FALSE;
}


/*****************************************************************************/
/*****************************************************************************/
/*                    The PropertySetDef Interface                           */
/*****************************************************************************/
/*****************************************************************************/

PropertySetDef_impl::PropertySetDef_impl ()
{
}

/* Used by PropertySetDefFactory::create_constrained_propertysetdef (...) */
PropertySetDef_impl::PropertySetDef_impl (const PropertyService::PropertyTypes
                                            &allowed_property_types,
                                          const PropertyService::PropertyDefs
                                            &allowed_property_defs)
{
  CORBA::ULong len = allowed_property_types.length ();

  for (CORBA::ULong idx0 = 0; idx0 < len; idx0++)
    mv_allowed_property_types.push_back (allowed_property_types [idx0]);
 
  len = allowed_property_defs.length ();

  for (CORBA::ULong idx = 0; idx < len; idx++)
  {
    /* Check if PropertyName is valid */
    if (! is_property_name_valid (allowed_property_defs [idx].property_name)) {
      mico_throw (PropertyService::InvalidPropertyName ());
    }
    /* Check if there are any conflicts between the constraints */
    if (! is_property_type_allowed 
            (allowed_property_defs [idx].property_value.type ()))
    {
      mico_throw (PropertyService::UnsupportedTypeCode ());
    }
    PropertyService::PropertyDef_var new_property_def = 
      new PropertyService::PropertyDef ();

    new_property_def->property_name = 
      CORBA::string_dup (allowed_property_defs [idx].property_name);

    new_property_def->property_value=allowed_property_defs [idx].property_value;

    /* If PropertyModeType is set to 'undefined' all Types are allowed */
    new_property_def->property_mode = allowed_property_defs [idx].property_mode;

    mv_allowed_properties.push_back (new_property_def); 
  }
}

/* Used by PropertySetDefFactory::create_initial_propertysetdef (...) */
PropertySetDef_impl::PropertySetDef_impl (const PropertyService::PropertyDefs
                                            &initial_property_defs)
{
  define_properties_with_modes (initial_property_defs);
}

PropertySetDef_impl::~PropertySetDef_impl ()
{
}


  /****************************************************/
  /* Support for retrieval of PropertySet constraints */
  /****************************************************/

void
PropertySetDef_impl::get_allowed_property_types (PropertyService::PropertyTypes_out
                                                   property_types)
{
  MICOMT::AutoLock t_lock(ps_lock_);
  CORBA::ULong len = mv_allowed_property_types.size ();

  property_types = new PropertyService::PropertyTypes;
  property_types->length (len);

  for (CORBA::ULong idx = 0; idx < len; idx++)
    (*property_types) [idx] = mv_allowed_property_types [idx];
}

void
PropertySetDef_impl::get_allowed_properties (PropertyService::PropertyDefs_out
                                               property_defs)
{
  MICOMT::AutoLock t_lock(ps_lock_);
  CORBA::ULong len = mv_allowed_properties.size ();

  property_defs  = new PropertyService::PropertyDefs;
  property_defs->length (len);

  for (CORBA::ULong idx = 0; idx < len; idx++) {
    (*property_defs) [idx].property_name = 
      mv_allowed_properties [idx]->property_name;

    (*property_defs) [idx].property_value = 
      mv_allowed_properties [idx]->property_value;

    (*property_defs) [idx].property_mode = 
      mv_allowed_properties [idx]->property_mode;
  }
}

  /*************************************************/
  /* Support for defining and modifiying poperties */
  /*************************************************/

void
PropertySetDef_impl::define_property_with_mode 
                       (const char *property_name,
                        const CORBA::Any &property_value,
                        PropertyService::PropertyModeType property_mode)
{
  MICOMT::AutoLock t_lock(ps_lock_);
  /* Check PropertyModeType */
  if (property_mode == PropertyService::undefined)
  {
    mico_throw (PropertyService::UnsupportedMode ());
  }
  def_prop (property_name, property_value, property_mode);
}

void
PropertySetDef_impl::define_properties_with_modes 
                       (const PropertyService::PropertyDefs &property_defs)
{
  MICOMT::AutoLock t_lock(ps_lock_);
  CORBA::ULong len = property_defs.length();
  assert (len > 0);

  PropertyService::MultipleExceptions mul_exc;

  for (CORBA::ULong idx = 0; idx < len; idx++)
  {
    #ifdef HAVE_EXCEPTIONS
    try {
    #endif
      define_property_with_mode (property_defs[idx].property_name,
                                 property_defs[idx].property_value,
                                 property_defs[idx].property_mode);
    #ifdef HAVE_EXCEPTIONS
    }
    #endif
    MULTIPLE_EXCEPTIONS_CATCH(mul_exc, property_defs[idx].property_name);
  }

  if (mul_exc.exceptions.length () != 0) {
    mico_throw (mul_exc);
  }
}

  /**************************************************/
  /* Support for Getting and Setting Property Modes */
  /**************************************************/

PropertyService::PropertyModeType
PropertySetDef_impl::get_property_mode (const char *property_name)
{
  MICOMT::AutoLock t_lock(ps_lock_);
  /* Check if PropertyName is valid */
  if (! is_property_name_valid (property_name)) {
    mico_throw (PropertyService::InvalidPropertyName ());
  }

  CORBA::ULong idx;
  CORBA::Boolean success = get_index (property_name, &idx);

  if (success == FALSE) {
    /* Property not found */
    mico_throw (PropertyService::PropertyNotFound ());
  }
  return mv_properties[idx]->property_mode; 
}

CORBA::Boolean
PropertySetDef_impl::get_property_modes (const PropertyService::PropertyNames
                                           &property_names,
                                         PropertyService::PropertyModes_out 
                                           property_modes)
{
  MICOMT::AutoLock t_lock(ps_lock_);
  CORBA::ULong len = property_names.length ();
  assert (len > 0);
  
  /* Create new PropertyModes Sequence */
  property_modes = new PropertyService::PropertyModes;
  property_modes->length (len);

  CORBA::Boolean return_stat = TRUE;

  for (CORBA::ULong idx = 0; idx < len; idx++) {
    /* Copy Property_Name */
    (*property_modes) [idx].property_name = 
      CORBA::string_dup (property_names [idx]);

    PropertyService::PropertyModeType prop_mode;
    #ifdef HAVE_EXCEPTIONS
    try {
    #endif
      /* Try to get the value */
      prop_mode = get_property_mode (property_names [idx]);
    #ifdef HAVE_EXCEPTIONS
    }
    /* If an Exception raises set the Property_Value to tk_void */ 
    catch (PropertyService::InvalidPropertyName_catch &exp) {
      prop_mode = PropertyService::undefined;
      return_stat = FALSE;
    }
    catch (PropertyService::PropertyNotFound_catch &exp) {
      prop_mode = PropertyService::undefined;
      return_stat = FALSE;
    }
    #endif

    /* Copy PropertyMode */ 
    (*property_modes) [idx].property_mode = prop_mode;
  }
  return return_stat;
}

void
PropertySetDef_impl::set_property_mode (const char *property_name,
                                        PropertyService::PropertyModeType
                                          property_mode)
{
  MICOMT::AutoLock t_lock(ps_lock_);
  /* Check if PropertyName is valid */
  if (! is_property_name_valid (property_name)) {
    mico_throw (PropertyService::InvalidPropertyName ());
  }

  /* Check if PropertyModeType is valid */
  if (property_mode == PropertyService::undefined)
  {
    mico_throw (PropertyService::UnsupportedMode ());
  }

  CORBA::ULong idx;
  CORBA::Boolean success = get_index (property_name, &idx);

  if (success == FALSE) {
    /* Property not found */
    mico_throw (PropertyService::PropertyNotFound ());
  }

  /* Check if new Property is allowed */
  if (! is_property_allowed (property_name, *get_value (idx), property_mode))
  {
    mico_throw (PropertyService::UnsupportedMode ());
  }

  mv_properties [idx]->property_mode = property_mode;
}

void
PropertySetDef_impl::set_property_modes (const PropertyService::PropertyModes
                                           &property_modes)
{
  MICOMT::AutoLock t_lock(ps_lock_);
  CORBA::ULong len = property_modes.length();
  assert (len > 0);

  PropertyService::MultipleExceptions mul_exc;
  
  for (CORBA::ULong idx = 0; idx < len; idx++)
  {
    #ifdef HAVE_EXCEPTIONS
    try {
    #endif
      set_property_mode (property_modes[idx].property_name,
                         property_modes[idx].property_mode);
    #ifdef HAVE_EXCEPTIONS
    }
    #endif
    MULTIPLE_EXCEPTIONS_CATCH(mul_exc, property_modes[idx].property_name);
  }

  if (mul_exc.exceptions.length () != 0) {
    mico_throw (mul_exc);
  }
}

/*****************************************************************************/
/*****************************************************************************/
/*                  The PropertyNamesIterator Interface                      */
/*****************************************************************************/
/*****************************************************************************/

PropertyNamesIterator_impl::PropertyNamesIterator_impl ()
    : pni_lock_(FALSE, MICOMT::Mutex::Recursive)
{
  mp_property_set = 0;
  m_index = 0;
}

PropertyNamesIterator_impl::PropertyNamesIterator_impl (PropertySet_impl
                                                          *pro_set,
                                                        CORBA::ULong idx)
    : pni_lock_(FALSE, MICOMT::Mutex::Recursive)
{
  mp_property_set = pro_set;
  m_index = idx;
}

PropertyNamesIterator_impl::~PropertyNamesIterator_impl ()
{
}

void
PropertyNamesIterator_impl::reset ()
{
  MICOMT::AutoLock t_lock(pni_lock_);
  assert (mp_property_set != 0);

  m_index = 0; 
}

CORBA::Boolean
PropertyNamesIterator_impl::next_one (CORBA::String_out property_name)
{
  MICOMT::AutoLock t_lock(pni_lock_);
  assert (mp_property_set != 0);

  if (m_index >= mp_property_set->get_number_of_properties ())
  {
    property_name = CORBA::string_dup ("");
    return FALSE; 
  }

  /* Copy Property_Name */
  property_name = CORBA::string_dup(mp_property_set->get_name (m_index));

  ++m_index;

  return TRUE; 
}

CORBA::Boolean
PropertyNamesIterator_impl::next_n (CORBA::ULong how_many, 
                                PropertyService::PropertyNames_out property_names)
{
  MICOMT::AutoLock t_lock(pni_lock_);
  assert (mp_property_set != 0);

  CORBA::ULong len = mp_property_set->get_number_of_properties ();

  if (m_index >= len || how_many == 0)
  {
    /* No more Items! */
    property_names = new PropertyService::PropertyNames ();
    return FALSE;
  }

  /* Create new PropertyNames Sequence */
  property_names = new PropertyService::PropertyNames ();

  if (m_index + how_many <= len)
    property_names->length (how_many);
  else
    property_names->length (len-m_index);

  CORBA::ULong max = (m_index + how_many < len) ? m_index+how_many : len;

  CORBA::ULong start=m_index;
  for (; m_index < max; m_index++)
  {
    /* Copy Property_Name */
    (*property_names) [m_index - start] = 
      CORBA::string_dup(mp_property_set->get_name (m_index));
  }
  return TRUE; 
}

void
PropertyNamesIterator_impl::destroy ()
{
  MICOMT::AutoLock t_lock(pni_lock_);
  mp_property_set = 0;
  m_index = 0;

  PortableServer::ObjectId_var oid = _default_POA ()->servant_to_id (this);
  _default_POA ()->deactivate_object (*oid);
  this->_remove_ref();
}


/*****************************************************************************/
/*****************************************************************************/
/*                     The PropertiesIterator Interface                      */
/*****************************************************************************/
/*****************************************************************************/

PropertiesIterator_impl::PropertiesIterator_impl ()
    : pi_lock_(FALSE, MICOMT::Mutex::Recursive)
{
  mp_property_set = 0;
  m_index = 0;
}

PropertiesIterator_impl::PropertiesIterator_impl (PropertySet_impl *pro_set,
                                                  CORBA::ULong idx)
    : pi_lock_(FALSE, MICOMT::Mutex::Recursive)
{
  mp_property_set = pro_set;
  m_index = idx;
}

PropertiesIterator_impl::~PropertiesIterator_impl ()
{
}

void
PropertiesIterator_impl::reset()
{
  MICOMT::AutoLock t_lock(pi_lock_);
  assert (mp_property_set != 0);

  m_index = 0;
}

CORBA::Boolean
PropertiesIterator_impl::next_one (PropertyService::Property_out aproperty)
{
  MICOMT::AutoLock t_lock(pi_lock_);
  assert (mp_property_set != 0);

  if (m_index >= mp_property_set->get_number_of_properties ())
  {
    aproperty = new PropertyService::Property;
    return FALSE;
  }
  /* Create new Property Objekt */
  aproperty = new PropertyService::Property;

  /* Copy Property_Name */
  aproperty->property_name = 
    CORBA::string_dup (mp_property_set->get_name (m_index));

  /* Copy Property_Value */
  aproperty->property_value = *mp_property_set->get_value(m_index); 

  ++m_index;

  return TRUE;
}

CORBA::Boolean
PropertiesIterator_impl::next_n (CORBA::ULong how_many,
                                 PropertyService::Properties_out nproperties)
{
  MICOMT::AutoLock t_lock(pi_lock_);
  assert (mp_property_set != 0);

  CORBA::ULong len = mp_property_set->get_number_of_properties ();
  if (m_index >= len || how_many == 0)
  {
    /* No more Items! */
    nproperties = new PropertyService::Properties (); 
    return FALSE;
  }
   
  /* Create new Properties Sequence */
  nproperties = new PropertyService::Properties ();
 
  if (m_index + how_many <= len)
    nproperties->length (how_many);
  else
    nproperties->length (len - m_index);

  CORBA::ULong max = (m_index + how_many < len) ? m_index+how_many : len;

  CORBA::ULong start=m_index;
  for (; m_index < max; m_index++)
  {
    /* Copy Property_Name */
    (*nproperties) [m_index-start].property_name = 
      CORBA::string_dup(mp_property_set->get_name (m_index));
  
    /* Copy Property_Value */
    (*nproperties) [m_index-start].property_value =
      *mp_property_set->get_value (m_index);
  }
  return TRUE;  
}

void
PropertiesIterator_impl::destroy ()
{
  MICOMT::AutoLock t_lock(pi_lock_);
  mp_property_set = 0;  
  m_index = 0;

  PortableServer::ObjectId_var oid = _default_POA ()->servant_to_id (this);
  _default_POA ()->deactivate_object (*oid);
  this->_remove_ref();
}

