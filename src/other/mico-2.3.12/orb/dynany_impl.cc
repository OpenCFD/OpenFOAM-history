/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2005 by The Mico Team
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
 *  For more information, visit the MICO Home Page at
 *  http://www.mico.org/
 */

#ifdef FAST_PCH
#include "orb_pch.h"
#endif // FAST_PCH
#ifdef __COMO__
#pragma hdrstop
#endif // __COMO__

#ifndef FAST_PCH

#define MICO_CONF_IMR
#define MICO_CONF_DYNANY

#include <CORBA-SMALL.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <sstream>
#else
#include <strstream.h>
#endif
#include <mico/dynany_impl.h>
#include <mico/impl.h>
#include <mico/throw.h>
#include <mico/template_impl.h>
#include <mico/util.h>

#endif // FAST_PCH


using namespace std;

/************************** DynAny *********************************/


DynAny_impl::DynAny_impl ()
    : _index (0)
{
}

DynAny_impl::~DynAny_impl ()
{
}

DynamicAny::DynAnyFactory_ptr
DynAny_impl::_factory ()
{
    static DynamicAny::DynAnyFactory_ptr factory =
        DynamicAny::DynAnyFactory::_nil();

    if (CORBA::is_nil (factory)) {
        CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
	CORBA::Object_var obj =
            orb->resolve_initial_references ("DynAnyFactory");
        factory = DynamicAny::DynAnyFactory::_narrow (obj);
        assert (!CORBA::is_nil (factory));
    }
    return factory;
}

void
DynAny_impl::update_element (CORBA::Long idx)
{
}

void
DynAny_impl::assign (DynamicAny::DynAny_ptr dyn_any)
{
    CORBA::TypeCode_var tc = dyn_any->type();
    if (!tc->equivalent (_type.in()))
        mico_throw (TypeMismatch());

    CORBA::Any_var a = dyn_any->to_any ();
    from_any (a.in());
}

DynamicAny::DynAny_ptr
DynAny_impl::copy ()
{
    CORBA::Any_var a = to_any ();
    return _factory()->create_dyn_any (a.in());
}

CORBA::TypeCode_ptr
DynAny_impl::type ()
{
    return CORBA::TypeCode::_duplicate (_type);
}

void
DynAny_impl::destroy ()
{
  for (CORBA::ULong i = 0; i < _elements.size(); ++i) {
    if (!CORBA::is_nil(_elements[i])) {
      _elements[i]->destroy();
    }
  }
  _elements.erase (_elements.begin(), _elements.end());
  CORBA::release (this);
}

void
DynAny_impl::insert_boolean (CORBA::Boolean value)
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any a;
    // take care for aliases of base types
    CORBA::TypeCode_var tc = _elements[_index]->type();
    a.set_type (tc);
    a <<= CORBA::Any::from_boolean (value);
    _elements[_index]->from_any (a);
}

void
DynAny_impl::insert_octet (CORBA::Octet value)
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any a;
    // take care for aliases of base types
    CORBA::TypeCode_var tc = _elements[_index]->type();
    a.set_type (tc);
    a <<= CORBA::Any::from_octet (value);
    _elements[_index]->from_any (a);
}

void
DynAny_impl::insert_char (CORBA::Char value)
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any a;
    // take care for aliases of base types
    CORBA::TypeCode_var tc = _elements[_index]->type();
    a.set_type (tc);
    a <<= CORBA::Any::from_char (value);
    _elements[_index]->from_any (a);
}

void
DynAny_impl::insert_wchar (CORBA::WChar value)
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any a;
    // take care for aliases of base types
    CORBA::TypeCode_var tc = _elements[_index]->type();
    a.set_type (tc);
    a <<= CORBA::Any::from_wchar (value);
    _elements[_index]->from_any (a);
}

void
DynAny_impl::insert_short (CORBA::Short value)
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any a;
    // take care for aliases of base types
    CORBA::TypeCode_var tc = _elements[_index]->type();
    a.set_type (tc);
    a <<= value;
    _elements[_index]->from_any (a);
}

void
DynAny_impl::insert_ushort (CORBA::UShort value)
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any a;
    // take care for aliases of base types
    CORBA::TypeCode_var tc = _elements[_index]->type();
    a.set_type (tc);
    a <<= value;
    _elements[_index]->from_any (a);
}

void
DynAny_impl::insert_long (CORBA::Long value)
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any a;
    // take care for aliases of base types
    CORBA::TypeCode_var tc = _elements[_index]->type();
    a.set_type (tc);
    a <<= value;
    _elements[_index]->from_any (a);
}

void
DynAny_impl::insert_longlong (CORBA::LongLong value)
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any a;
    // take care for aliases of base types
    CORBA::TypeCode_var tc = _elements[_index]->type();
    a.set_type (tc);
    a <<= value;
    _elements[_index]->from_any (a);
}

void
DynAny_impl::insert_ulong (CORBA::ULong value)
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any a;
    // take care for aliases of base types
    CORBA::TypeCode_var tc = _elements[_index]->type();
    a.set_type (tc);
    a <<= value;
    _elements[_index]->from_any (a);
}

void
DynAny_impl::insert_ulonglong (CORBA::ULongLong value)
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any a;
    // take care for aliases of base types
    CORBA::TypeCode_var tc = _elements[_index]->type();
    a.set_type (tc);
    a <<= value;
    _elements[_index]->from_any (a);
}

void
DynAny_impl::insert_float (CORBA::Float value)
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any a;
    // take care for aliases of base types
    CORBA::TypeCode_var tc = _elements[_index]->type();
    a.set_type (tc);
    a <<= value;
    _elements[_index]->from_any (a);
}

void
DynAny_impl::insert_double (CORBA::Double value)
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any a;
    // take care for aliases of base types
    CORBA::TypeCode_var tc = _elements[_index]->type();
    a.set_type (tc);
    a <<= value;
    _elements[_index]->from_any (a);
}

void
DynAny_impl::insert_longdouble (CORBA::LongDouble value)
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any a;
    // take care for aliases of base types
    CORBA::TypeCode_var tc = _elements[_index]->type();
    a.set_type (tc);
    a <<= value;
    _elements[_index]->from_any (a);
}

void
DynAny_impl::insert_string (const char* value)
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any a;
    // take care for aliases of base types
    CORBA::TypeCode_var tc = _elements[_index]->type();
    CORBA::TypeCode_ptr utc = tc->unalias();

    if (utc->kind() == CORBA::tk_string &&
        utc->length() > 0 &&
        utc->length() < strlen (value))
        mico_throw (InvalidValue());

    a.set_type (tc);
    a <<= CORBA::Any::from_string ((char *)value, utc->length());
    _elements[_index]->from_any (a);
}

void
DynAny_impl::insert_wstring (const CORBA::WChar* value)
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any a;
    // take care for aliases of base types
    CORBA::TypeCode_var tc = _elements[_index]->type();
    CORBA::TypeCode_ptr utc = tc->unalias();

    if (utc->kind() == CORBA::tk_wstring &&
        utc->length() > 0 &&
        utc->length() < xwcslen (value))
        mico_throw (InvalidValue());

    a.set_type (tc);
    a <<= CORBA::Any::from_wstring ((CORBA::WChar *)value, utc->length());
    _elements[_index]->from_any (a);
}

void
DynAny_impl::insert_reference (CORBA::Object_ptr value)
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any a;
    // take care for aliases of base types
    CORBA::TypeCode_var tc = _elements[_index]->type();
    a.set_type (tc);
    if (_type->unalias()->kind() == CORBA::tk_objref)
        a <<= CORBA::Any::from_object (value, _type->name());
    else
        a <<= CORBA::Any::from_object (value, "");
    _elements[_index]->from_any (a);
}

void
DynAny_impl::insert_typecode (CORBA::TypeCode_ptr value)
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any a;
    // take care for aliases of base types
    CORBA::TypeCode_var tc = _elements[_index]->type();
    a.set_type (tc);
    a <<= value;
    _elements[_index]->from_any (a);
}

void
DynAny_impl::insert_val (CORBA::ValueBase *value)
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);

    CORBA::StaticAny sa (CORBA::_stc_ValueBase, &value);

    CORBA::Any a;
    // take care for aliases
    CORBA::TypeCode_var tc = _elements[_index]->type();

    if (!a.from_static_any (sa, tc))
	mico_throw (TypeMismatch());

    _elements[_index]->from_any (a);
}

void
DynAny_impl::insert_any (const CORBA::Any &value)
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any a;
    // take care for aliases of base types
    CORBA::TypeCode_var tc = _elements[_index]->type();
    a.set_type (tc);
    a <<= value;
    _elements[_index]->from_any (a);
}

void
DynAny_impl::insert_dyn_any (DynamicAny::DynAny_ptr value)
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    CORBA::Any_var a = value->to_any();
    insert_any (a.in());
}

void
DynAny_impl::insert_abstract (CORBA::AbstractBase_ptr value)
{
  CORBA::TypeCode_var tc = _elements[_index]->type ();
  CORBA::Any a;
  a.set_type (tc);
  a <<= value;
  _elements[_index]->from_any (a);
}

CORBA::Boolean
DynAny_impl::get_boolean ()
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any_var a = _elements[_index]->to_any();
    CORBA::Boolean value;
    if (!((CORBA::Any &)a >>= CORBA::Any::to_boolean (value)))
	mico_throw (TypeMismatch ());
    return value;
}

CORBA::Octet
DynAny_impl::get_octet ()
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any_var a = _elements[_index]->to_any();
    CORBA::Octet value;
    if (!((CORBA::Any &)a >>= CORBA::Any::to_octet (value)))
	mico_throw (TypeMismatch ());
    return value;
}

CORBA::Char
DynAny_impl::get_char ()
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any_var a = _elements[_index]->to_any();
    CORBA::Char value;
    if (!((CORBA::Any &)a >>= CORBA::Any::to_char (value)))
	mico_throw (TypeMismatch ());
    return value;
}

CORBA::WChar
DynAny_impl::get_wchar ()
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any_var a = _elements[_index]->to_any();
    CORBA::WChar value;
    if (!((CORBA::Any &)a >>= CORBA::Any::to_wchar (value)))
	mico_throw (TypeMismatch ());
    return value;
}

CORBA::Short
DynAny_impl::get_short ()
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any_var a = _elements[_index]->to_any();
    CORBA::Short value;
    if (!((CORBA::Any &)a >>= value))
	mico_throw (TypeMismatch ());
    return value;
}

CORBA::UShort
DynAny_impl::get_ushort ()
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any_var a = _elements[_index]->to_any();
    CORBA::UShort value;
    if (!((CORBA::Any &)a >>= value))
	mico_throw (TypeMismatch ());
    return value;
}

CORBA::Long
DynAny_impl::get_long ()
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any_var a = _elements[_index]->to_any();
    CORBA::Long value;
    if (!((CORBA::Any &)a >>= value))
	mico_throw (TypeMismatch ());
    return value;
}

CORBA::LongLong
DynAny_impl::get_longlong ()
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any_var a = _elements[_index]->to_any();
    CORBA::LongLong value;
    if (!((CORBA::Any &)a >>= value))
	mico_throw (TypeMismatch ());
    return value;
}

CORBA::ULong
DynAny_impl::get_ulong ()
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any_var a = _elements[_index]->to_any();
    CORBA::ULong value;
    if (!((CORBA::Any &)a >>= value))
	mico_throw (TypeMismatch ());
    return value;
}

CORBA::ULongLong
DynAny_impl::get_ulonglong ()
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any_var a = _elements[_index]->to_any();
    CORBA::ULongLong value;
    if (!((CORBA::Any &)a >>= value))
	mico_throw (TypeMismatch ());
    return value;
}

CORBA::Float
DynAny_impl::get_float ()
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any_var a = _elements[_index]->to_any();
    CORBA::Float value;
    if (!((CORBA::Any &)a >>= value))
	mico_throw (TypeMismatch ());
    return value;
}

CORBA::Double
DynAny_impl::get_double ()
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any_var a = _elements[_index]->to_any();
    CORBA::Double value;
    if (!((CORBA::Any &)a >>= value))
	mico_throw (TypeMismatch ());
    return value;
}

CORBA::LongDouble
DynAny_impl::get_longdouble ()
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any_var a = _elements[_index]->to_any();
    CORBA::LongDouble value;
    if (!((CORBA::Any &)a >>= value))
	mico_throw (TypeMismatch ());
    return value;
}

char*
DynAny_impl::get_string ()
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any_var a = _elements[_index]->to_any();
    const char *value;
    CORBA::TypeCode_var tc = a->type();
    if (!((CORBA::Any &)a >>= CORBA::Any::to_string (value, tc->unalias()->length())))
	mico_throw (TypeMismatch ());
    return CORBA::string_dup (value);
}

CORBA::WChar*
DynAny_impl::get_wstring ()
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any_var a = _elements[_index]->to_any();
    const CORBA::WChar *value;
    CORBA::TypeCode_var tc = a->type();
    if (!((CORBA::Any &)a >>= CORBA::Any::to_wstring (value, tc->unalias()->length())))
	mico_throw (TypeMismatch ());
    return CORBA::wstring_dup (value);
}

CORBA::Object_ptr
DynAny_impl::get_reference ()
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any_var a = _elements[_index]->to_any();
    CORBA::Object_var value;
    if (!((CORBA::Any &)a >>= CORBA::Any::to_object (value)))
	mico_throw (TypeMismatch ());
    return CORBA::Object::_duplicate (value);
}

CORBA::TypeCode_ptr
DynAny_impl::get_typecode ()
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any_var a = _elements[_index]->to_any();
    CORBA::TypeCode_ptr value;
    if (!((CORBA::Any &)a >>= value))
	mico_throw (TypeMismatch ());
    return CORBA::TypeCode::_duplicate (value);
}

CORBA::Any*
DynAny_impl::get_any ()
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any_var a = _elements[_index]->to_any();
    const CORBA::Any *value;
    if (!((CORBA::Any &)a >>= value))
	mico_throw (TypeMismatch ());
    CORBA::Any *retn = new CORBA::Any;
    *retn = *value;
    return retn;
}

DynamicAny::DynAny_ptr
DynAny_impl::get_dyn_any ()
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any_var a = _elements[_index]->to_any();
    const CORBA::Any* value;
    if (!((CORBA::Any &)a >>= value))
	mico_throw (TypeMismatch ());
    return _factory()->create_dyn_any (*value);
}

CORBA::ValueBase *
DynAny_impl::get_val ()
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any_var a = _elements[_index]->to_any();

    CORBA::ValueBase *value;
    CORBA::StaticAny sa (CORBA::_stc_ValueBase, &value);

    if (!a->to_static_any (sa))
	mico_throw (TypeMismatch());

    return value;
}

CORBA::AbstractBase_ptr
DynAny_impl::get_abstract ()
{
    if (_index < 0)
        mico_throw (TypeMismatch());

    update_element (_index);
    CORBA::Any_var a = _elements[_index]->to_any();
    CORBA::AbstractBase_ptr value;
    if (!((CORBA::Any &)a >>= CORBA::Any::to_abstract_base (value)))
	mico_throw (TypeMismatch ());
    return value;
}

DynamicAny::DynAny_ptr
DynAny_impl::current_component ()
{
    if (_index < 0) {
        if (component_count() == 0)
            mico_throw (TypeMismatch());
        return DynamicAny::DynAny::_nil();
    }
    update_element (_index);
    return DynamicAny::DynAny::_duplicate (_elements[_index]);
}

CORBA::ULong
DynAny_impl::component_count ()
{
    return _elements.size();
}

CORBA::Boolean
DynAny_impl::next ()
{
    if ((CORBA::ULong)_index+1 == _elements.size())
	return FALSE;
    ++_index;
    update_element (_index);
    return TRUE;
}

CORBA::Boolean
DynAny_impl::seek (CORBA::Long index)
{
    if (index < 0) {
        _index = -1;
        return FALSE;
    }
    update_element (index); // might cause element to disappear
    if ((CORBA::ULong)index >= _elements.size()) {
        _index = -1;
	return FALSE;
    }
    _index = index;
    return TRUE;
}

void
DynAny_impl::rewind ()
{
    seek (0);
}

CORBA::Boolean
DynAny_impl::equal (DynamicAny::DynAny_ptr da)
{
    CORBA::Any_var me = to_any();
    CORBA::Any_var he = da->to_any();
    return me.in() == he.in();
}


/************************* DynBasic *******************************/


DynBasic_impl::DynBasic_impl (CORBA::TypeCode_ptr tc)
{
    _type = CORBA::TypeCode::_duplicate (tc);
    // take care for aliases of base types
    _value.set_type (_type);
    tc = tc->unalias ();
    switch (tc->kind()) {
    case CORBA::tk_void:
    case CORBA::tk_null:
	break;
    case CORBA::tk_char:
	_value <<= CORBA::Any::from_char (0);
	break;
    case CORBA::tk_wchar:
	_value <<= CORBA::Any::from_wchar (0);
	break;
    case CORBA::tk_boolean:
	_value <<= CORBA::Any::from_boolean (FALSE);
	break;
    case CORBA::tk_octet:
	_value <<= CORBA::Any::from_octet (0);
	break;
    case CORBA::tk_short:
	_value <<= (CORBA::Short)0;
	break;
    case CORBA::tk_ushort:
	_value <<= (CORBA::UShort)0;
	break;
    case CORBA::tk_long:
	_value <<= (CORBA::Long)0;
	break;
    case CORBA::tk_longlong:
	_value <<= (CORBA::LongLong)0;
	break;
    case CORBA::tk_ulong:
	_value <<= (CORBA::ULong)0;
	break;
    case CORBA::tk_ulonglong:
	_value <<= (CORBA::ULongLong)0;
	break;
    case CORBA::tk_float:
	_value <<= (CORBA::Float)0;
	break;
    case CORBA::tk_double:
	_value <<= (CORBA::Double)0;
	break;
    case CORBA::tk_longdouble:
	_value <<= (CORBA::LongDouble)0;
	break;
    case CORBA::tk_any:
	_value <<= CORBA::Any();
	break;
    case CORBA::tk_TypeCode:
	_value <<= CORBA::_tc_null;
	break;
    case CORBA::tk_objref:
	_value <<= CORBA::Any::from_object (CORBA::Object::_nil(), "");
	break;
    case CORBA::tk_string:
        _value <<= "";
        break;
    case CORBA::tk_wstring:
        _value <<= L"";
        break;
    default:
	mico_throw (DynamicAny::DynAnyFactory::InconsistentTypeCode ());
    }
    _elements.push_back (_duplicate (this));
}

DynBasic_impl::DynBasic_impl (const CORBA::Any &a)
{
    _value = a;
    _type = _value.type();

    switch (_type->unalias()->kind()) {
    case CORBA::tk_void:
    case CORBA::tk_null:
    case CORBA::tk_char:
    case CORBA::tk_wchar:
    case CORBA::tk_boolean:
    case CORBA::tk_octet:
    case CORBA::tk_short:
    case CORBA::tk_ushort:
    case CORBA::tk_long:
    case CORBA::tk_longlong:
    case CORBA::tk_ulong:
    case CORBA::tk_ulonglong:
    case CORBA::tk_float:
    case CORBA::tk_double:
    case CORBA::tk_longdouble:
    case CORBA::tk_any:
    case CORBA::tk_TypeCode:
    case CORBA::tk_objref:
    case CORBA::tk_string:
    case CORBA::tk_wstring:
	break;
    default:
	mico_throw (DynamicAny::DynAnyFactory::InconsistentTypeCode ());
    }
    _elements.push_back (_duplicate (this));
}

DynBasic_impl::~DynBasic_impl ()
{
}

void
DynBasic_impl::destroy ()
{
    _elements.pop_back ();
    CORBA::release (this);
}

void
DynBasic_impl::from_any (const CORBA::Any& value)
{
    CORBA::TypeCode_var tc = value.type();
    if (_type->unalias()->kind() != CORBA::tk_objref ||
	tc->unalias()->kind() != CORBA::tk_objref) {
        if (!_type->equaltype (tc))
            mico_throw (TypeMismatch ());
    }
    _value = value;
}

CORBA::Any*
DynBasic_impl::to_any ()
{
    return new CORBA::Any (_value);
}


/************************** DynFixed ********************************/


DynFixed_impl::DynFixed_impl (CORBA::TypeCode_ptr tc)
{
    if (tc->unalias()->kind() != CORBA::tk_fixed)
	mico_throw (DynamicAny::DynAnyFactory::InconsistentTypeCode ());

    _type = CORBA::TypeCode::_duplicate (tc);

    CORBA::UShort digits = _type->unalias()->fixed_digits();
    CORBA::Short scale = _type->unalias()->fixed_scale();

    Fixed f (digits, scale);
    
    _value <<= CORBA::Any::from_fixed (f, digits, scale);

    _elements.push_back (_duplicate (this));

    _index = -1;
}

DynFixed_impl::DynFixed_impl (const CORBA::Any &a)
{
    _value = a;
    _type = a.type();
    if (_type->unalias()->kind() != CORBA::tk_fixed)
	mico_throw (DynamicAny::DynAnyFactory::InconsistentTypeCode ());

    _elements.push_back (_duplicate (this));

    _index = -1;
}

DynFixed_impl::~DynFixed_impl ()
{
}

void
DynFixed_impl::destroy ()
{
    _elements.pop_back ();
    CORBA::release (this);
}

void
DynFixed_impl::from_any (const CORBA::Any& value)
{
    CORBA::TypeCode_var tc = value.type();
    if (!_type->equaltype (tc))
	mico_throw (TypeMismatch ());
    _value = value;
}

CORBA::Any*
DynFixed_impl::to_any ()
{
    return new CORBA::Any (_value);
}

char *
DynFixed_impl::get_value ()
{
    CORBA::UShort digits = _type->unalias()->fixed_digits();
    CORBA::Short scale = _type->unalias()->fixed_scale();

    Fixed f (digits, scale);

    CORBA::Boolean r = (_value >>= CORBA::Any::to_fixed (f, digits, scale));
    assert (r);

#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
    ostringstream ostr;
    ostr << f;
    char *out = CORBA::string_dup (ostr.str().c_str());
#else
    ostrstream ostr;
    ostr << f << ends;
    char *out = CORBA::string_dup (ostr.str());
    ostr.rdbuf()->freeze (0);
#endif

    return out;
}

void
DynFixed_impl::set_value (const char *in)
{
    CORBA::UShort digits = _type->unalias()->fixed_digits();
    CORBA::Short scale = _type->unalias()->fixed_scale();

    Fixed f (digits, scale);

#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
    string instr (in);
    istringstream istr (instr);
#else
    istrstream istr (in);
#endif
    // XXX check for bad input string...
    istr >> f;

    _value <<= CORBA::Any::from_fixed (f, digits, scale);
}


/************************** DynEnum ********************************/


DynEnum_impl::DynEnum_impl (CORBA::TypeCode_ptr tc)
{
    if (tc->unalias()->kind() != CORBA::tk_enum)
	mico_throw (DynamicAny::DynAnyFactory::InconsistentTypeCode ());

    _type = CORBA::TypeCode::_duplicate (tc);
    _value.set_type (tc);
    _value.enum_put (0);

    _elements.push_back (_duplicate (this));

    _index = -1;
}

DynEnum_impl::DynEnum_impl (const CORBA::Any &a)
{
    _value = a;
    _type = a.type();
    if (_type->unalias()->kind() != CORBA::tk_enum)
	mico_throw (DynamicAny::DynAnyFactory::InconsistentTypeCode ());

    _elements.push_back (_duplicate (this));

    _index = -1;
}

DynEnum_impl::~DynEnum_impl ()
{
}

void
DynEnum_impl::destroy ()
{
    _elements.pop_back ();
    CORBA::release (this);
}

void
DynEnum_impl::from_any (const CORBA::Any& value)
{
    CORBA::TypeCode_var tc = value.type();
    if (!_type->equaltype (tc))
	mico_throw (TypeMismatch ());
    _value = value;
}

CORBA::Any*
DynEnum_impl::to_any ()
{
    return new CORBA::Any (_value);
}

char*
DynEnum_impl::get_as_string ()
{
    return CORBA::string_dup (
	_type->unalias()->member_name (get_as_ulong()));
}

void
DynEnum_impl::set_as_string (const char* value)
{
    CORBA::TypeCode_ptr tc = _type->unalias();
    CORBA::Long idx = tc->member_index (value);
    if (idx < 0)
        mico_throw (InvalidValue());
    CORBA::Boolean r = _value.enum_put (idx);
    assert (r);
}

CORBA::ULong
DynEnum_impl::get_as_ulong ()
{
    CORBA::ULong l;
    CORBA::Boolean r = _value.enum_get (l);
    assert (r);
    return l;
}

void
DynEnum_impl::set_as_ulong (CORBA::ULong value)
{
    if (value >= _type->unalias()->member_count())
        mico_throw (InvalidValue());
    CORBA::Boolean r = _value.enum_put (value);
    assert (r);
}


/************************* DynStruct *******************************/


DynStruct_impl::DynStruct_impl (CORBA::TypeCode_ptr tc)
{
    _type = CORBA::TypeCode::_duplicate (tc);
    tc = tc->unalias();
    if (tc->kind() != CORBA::tk_struct && tc->kind() != CORBA::tk_except)
	mico_throw (DynamicAny::DynAnyFactory::InconsistentTypeCode ());
    _isexcept = (tc->kind() == CORBA::tk_except);
    for (CORBA::ULong i = 0; i < tc->member_count(); ++i) {
	CORBA::TypeCode_var memtc = tc->member_type(i);
	_elements.push_back(_factory()->create_dyn_any_from_type_code (memtc));
    }
    if (_elements.size() == 0)
        _index = -1;
}

DynStruct_impl::DynStruct_impl (const CORBA::Any &a)
{
    _type = a.type();
    CORBA::TypeCode_ptr tc = _type->unalias();
    if (tc->kind() != CORBA::tk_struct && tc->kind() != CORBA::tk_except)
	mico_throw (DynamicAny::DynAnyFactory::InconsistentTypeCode ());
    _isexcept = (tc->kind() == CORBA::tk_except);

    if (_isexcept) {
	CORBA::String_var repoid;
	CORBA::Boolean r = a.except_get_begin (repoid.out());
	assert (r);
    } else {
	CORBA::Boolean r = a.struct_get_begin ();
	assert (r);
    }
    for (CORBA::ULong i = 0; i < tc->member_count(); ++i) {
	CORBA::Any el;
	CORBA::Boolean r = a.any_get (el);
	assert (r);
// Here it is needed to set the Any type after calling any_get()
// else there is no deal with aliased typecodes.
// Note that calling el.set_type() before calling any_get() doesn't resolve the alias problem.
         CORBA::TypeCode_var member_tc = tc->member_type(i);
         el.type(member_tc);
//
	_elements.push_back (_factory()->create_dyn_any (el));
    }
    if (_isexcept) {
	CORBA::Boolean r = a.except_get_end ();
	assert (r);
    } else {
	CORBA::Boolean r = a.struct_get_end ();
	assert (r);
    }
    if (_elements.size() == 0)
        _index = -1;
}

DynStruct_impl::~DynStruct_impl ()
{
}

void
DynStruct_impl::from_any (const CORBA::Any& value)
{
    CORBA::TypeCode_var tc = value.type();
    if (!_type->equaltype (tc))
	mico_throw (TypeMismatch ());

    if (_isexcept) {
	CORBA::String_var repoid;
	CORBA::Boolean r = value.except_get_begin (repoid.out());
	assert (r);
    } else {
	CORBA::Boolean r = value.struct_get_begin ();
	assert (r);
    }
    CORBA::TypeCode_ptr tc2 = tc->unalias();
    for (CORBA::ULong i = 0; i < tc2->member_count(); ++i) {
	CORBA::Any el;
	CORBA::Boolean r = value.any_get (el);
	assert (r);
// Here it is needed to set the Any type after calling any_get()
// else there is no deal with aliased typecodes.
// Note that calling el.set_type() before calling any_get() doesn't resolve the alias problem.
	CORBA::TypeCode_var member_tc = tc2->member_type(i);
	el.type(member_tc);
//
	_elements[i]->from_any (el);
    }
    if (_isexcept) {
	CORBA::Boolean r = value.except_get_end ();
	assert (r);
    } else {
	CORBA::Boolean r = value.struct_get_end ();
	assert (r);
    }
}

CORBA::Any*
DynStruct_impl::to_any ()
{
    CORBA::Any *a = new CORBA::Any;

    a->set_type (_type);

    if (_isexcept) {
	CORBA::TypeCode_ptr tc = _type->unalias();
	CORBA::Boolean r = a->except_put_begin (tc->id());
	assert (r);
    } else {
	CORBA::Boolean r = a->struct_put_begin ();
	assert (r);
    }
    for (CORBA::ULong i = 0; i < _elements.size(); ++i) {
	CORBA::Any_var el = _elements[i]->to_any ();
	CORBA::Boolean r = a->any_put (el.inout());
	assert (r);
    }
    if (_isexcept) {
	CORBA::Boolean r = a->except_put_end ();
	assert (r);
    } else {
	CORBA::Boolean r = a->struct_put_end ();
	assert (r);
    }
    return a;
}

DynamicAny::FieldName
DynStruct_impl::current_member_name ()
{
    if (_index < 0)
        mico_throw (TypeMismatch());
    return CORBA::string_dup (_type->unalias()->member_name (_index));
}

CORBA::TCKind
DynStruct_impl::current_member_kind ()
{
    if (_index < 0)
        mico_throw (TypeMismatch());
    // XXX unlias ???
    CORBA::TypeCode_var tc = _type->unalias()->member_type (_index);
    return tc->kind();
}

DynamicAny::NameValuePairSeq*
DynStruct_impl::get_members ()
{
    DynamicAny::NameValuePairSeq *seq = new DynamicAny::NameValuePairSeq;
    seq->length (_elements.size());
    CORBA::TypeCode_ptr tc = _type->unalias ();
    for (CORBA::ULong i = 0; i < _elements.size(); ++i) {
	(*seq)[i].id = tc->member_name (i);
	CORBA::Any_var a = _elements[i]->to_any ();
	(*seq)[i].value = a.in();
    }
    return seq;
}

void
DynStruct_impl::set_members (const DynamicAny::NameValuePairSeq& value)
{
    CORBA::TypeCode_ptr tc = _type->unalias ();
    if (value.length() != tc->member_count())
	mico_throw (InvalidValue ());

    for (CORBA::ULong i = 0; i < value.length(); ++i) {
	if (strcmp (tc->member_name(i), value[i].id))
	    mico_throw (TypeMismatch ());
	_elements[i]->from_any (value[i].value);
    }
    _index = _elements.size() > 0 ? 0 : -1;
}

DynamicAny::NameDynAnyPairSeq*
DynStruct_impl::get_members_as_dyn_any ()
{
    DynamicAny::NameDynAnyPairSeq *seq = new DynamicAny::NameDynAnyPairSeq;
    seq->length (_elements.size());
    CORBA::TypeCode_ptr tc = _type->unalias ();
    for (CORBA::ULong i = 0; i < _elements.size(); ++i) {
	(*seq)[i].id = tc->member_name (i);
	(*seq)[i].value = _elements[i]->copy();
    }
    return seq;
}

void
DynStruct_impl::set_members_as_dyn_any (
    const DynamicAny::NameDynAnyPairSeq& value)
{
    CORBA::TypeCode_ptr tc = _type->unalias ();
    if (value.length() != tc->member_count())
	mico_throw (InvalidValue ());

    for (CORBA::ULong i = 0; i < value.length(); ++i) {
	if (strlen (value[i].id) > 0 &&
            strcmp (tc->member_name(i), value[i].id))
	    mico_throw (TypeMismatch ());
	_elements[i]->assign (value[i].value.in());
    }
    _index = _elements.size() > 0 ? 0 : -1;
}


/************************* DynUnion *******************************/


DynUnion_impl::DynUnion_impl (CORBA::TypeCode_ptr tc)
{
    _type = CORBA::TypeCode::_duplicate (tc);
    tc = tc->unalias ();
    if (tc->kind() != CORBA::tk_union)
	mico_throw (DynamicAny::DynAnyFactory::InconsistentTypeCode ());

    CORBA::TypeCode_var disctc = tc->discriminator_type();
    _elements.push_back (_factory()->create_dyn_any_from_type_code (disctc));
    CORBA::Any_var disc = _elements[0]->to_any ();
    _member_idx = tc->member_index (disc.in());
    if (_member_idx >= 0) {
	CORBA::TypeCode_var memtc = tc->member_type (_member_idx);
	_elements.push_back(_factory()->create_dyn_any_from_type_code (memtc));
    } else {
	// XXX what to do for implicit default case ???
    }
}

DynUnion_impl::DynUnion_impl (const CORBA::Any &a)
{
    _type = a.type();
    CORBA::TypeCode_ptr tc = _type->unalias ();
    if (tc->kind() != CORBA::tk_union)
	mico_throw (DynamicAny::DynAnyFactory::InconsistentTypeCode ());

    CORBA::Boolean r = a.union_get_begin ();
    assert (r);
    CORBA::Any disc;
    r = a.any_get (disc);
    assert (r);
// Here it is needed to set the Any type after calling any_get()
// else there is no deal with aliased typecodes.
// Note that calling el.set_type() before calling any_get() doesn't resolve the alias problem.
    CORBA::TypeCode_var discr_tc = tc->discriminator_type();
    disc.type(discr_tc);
//
    _elements.push_back (_factory()->create_dyn_any (disc));
    _member_idx = tc->member_index (disc);
    if (_member_idx >= 0) {
	r = a.union_get_selection (_member_idx);
	assert (r);
	CORBA::Any el;
	r = a.any_get (el);
	assert (r);
// Here it is needed to set the Any type after calling any_get()
// else there is no deal with aliased typecodes.
// Note that calling el.set_type() before calling any_get() doesn't resolve the alias problem.
	CORBA::TypeCode_var member_tc = tc->member_type(_member_idx);
	el.type(member_tc);
//
	_elements.push_back (_factory()->create_dyn_any (el));
    } else {
	// XXX what to do for implicit default case ???
    }
    r = a.union_get_end ();
    assert (r);
}

DynUnion_impl::~DynUnion_impl ()
{
}

void
DynUnion_impl::update_element (CORBA::Long idx)
{
    if (idx != 1)
	return;

    // XXX slow 
    CORBA::Any_var disc = _elements[0]->to_any ();
    CORBA::Long nidx = _type->unalias()->member_index (disc.in());
    if (nidx == _member_idx)
	return;

    if (_elements.size() == 2) {
        // kcg: we need to destroy poped dynany first!
        _elements[_elements.size() - 1]->destroy();
	_elements.pop_back ();
    }
    if (nidx >= 0) {
	CORBA::TypeCode_var tc = _type->unalias()->member_type (nidx);
	_elements.push_back (_factory()->create_dyn_any_from_type_code (tc));
    } else {
	// XXX what to do for implicit default case ???
    }
    _member_idx = nidx;

    if (_index >= (CORBA::Long)_elements.size())
	_index = _elements.size()-1;
}

CORBA::ULong
DynUnion_impl::component_count ()
{
    update_element (1);
    return _elements.size();
}

void
DynUnion_impl::from_any (const CORBA::Any& value)
{
    CORBA::TypeCode_var tc = value.type();
    if (!_type->equaltype (tc))
	mico_throw (TypeMismatch ());

    CORBA::Boolean r = value.union_get_begin ();
    assert (r);
    CORBA::Any disc;
    r = value.any_get (disc);
    assert (r);
// Here it is needed to set the Any type after calling any_get()
// else there is no deal with aliased typecodes.
// Note that calling el.set_type() before calling any_get() doesn't resolve the alias problem.
    CORBA::TypeCode_var discr_tc = tc->discriminator_type();
    disc.type(discr_tc);
//
    _elements[0]->from_any (disc);
    update_element (1);
    if (_member_idx >= 0) {
	r = value.union_get_selection (_member_idx);
	assert (r);
	CORBA::Any el;
	r = value.any_get (el);
	assert (r);
// Here it is needed to set the Any type after calling any_get()
// else there is no deal with aliased typecodes.
// Note that calling el.set_type() before calling any_get() doesn't resolve the alias problem.
	CORBA::TypeCode_var member_tc = tc->member_type(_member_idx);
	el.type(member_tc);
//
	_elements[1]->from_any (el);
    } else {
	// XXX what to do for implicit default case ???
    }
    r = value.union_get_end ();
    assert (r);
}

CORBA::Any*
DynUnion_impl::to_any ()
{
    if (has_default_member())
        set_to_default_member ();

    CORBA::Any *a = new CORBA::Any;

    a->set_type (_type);
    CORBA::Boolean r = a->union_put_begin ();
    assert (r);
    CORBA::Any_var disc = _elements[0]->to_any ();
    r = a->any_put (disc.inout());
    assert (r);
    CORBA::Long idx = _type->unalias()->member_index (disc.in());
    update_element (1);
    if (idx >= 0) {
	r = a->union_put_selection (idx);
	assert (r);
	CORBA::Any_var val = _elements[1]->to_any ();
	r = a->any_put (val.inout());
	assert (r);
    }
    r = a->union_put_end ();
    assert (r);
    return a;
}

DynamicAny::DynAny_ptr
DynUnion_impl::get_discriminator ()
{
    return DynamicAny::DynAny::_duplicate (_elements[0]);
}

void
DynUnion_impl::set_discriminator (DynamicAny::DynAny_ptr d)
{
    _elements[0]->assign (d);
}

CORBA::TCKind
DynUnion_impl::discriminator_kind ()
{
    // XXX unalias ???
    CORBA::TypeCode_var tc = _elements[0]->type();
    return tc->kind();
}

CORBA::Boolean
DynUnion_impl::has_default_member ()
{
    CORBA::Any_var disc = _elements[0]->to_any ();
    CORBA::Long idx = _type->unalias()->member_index (disc.in());
    return idx < 0 || idx == _type->unalias()->default_index();
}

void
DynUnion_impl::set_to_default_member ()
{
    CORBA::TypeCode_ptr tc = _type->unalias();
    CORBA::Long defidx = tc->default_index();

    CORBA::TypeCode_var disc = tc->discriminator_type();
    disc = CORBA::TypeCode::_duplicate (disc->unalias());

    if (disc->kind() == CORBA::tk_enum) {
        CORBA::Any any;
        any.set_type (disc);
        for (CORBA::ULong i = 0; i < disc->member_count(); ++i) {
            CORBA::Boolean r = any.enum_put (i);
            assert (r);
            if (tc->member_index (any) == defidx) {
                _elements[0]->from_any (any);
                _index = 0;
                return;
            }
        }
        mico_throw (TypeMismatch());
    }
    if (disc->kind() == CORBA::tk_long) {
        CORBA::Long l = 0;
        CORBA::Any any;
        do {
            any <<= l;
            if (tc->member_index (any) == defidx) {
                _elements[0]->from_any (any);
                _index = 0;
                return;
            }
        } while (++l);
        mico_throw (TypeMismatch());
    }
    if (disc->kind() == CORBA::tk_longlong) {
        CORBA::LongLong l = 0;
        CORBA::Any any;
        do {
            any <<= l;
            if (tc->member_index (any) == defidx) {
                _elements[0]->from_any (any);
                _index = 0;
                return;
            }
        } while (++l);
        mico_throw (TypeMismatch());
    }
    if (disc->kind() == CORBA::tk_ulong) {
        CORBA::ULong l = 0;
        CORBA::Any any;
        do {
            any <<= l;
            if (tc->member_index (any) == defidx) {
                _elements[0]->from_any (any);
                _index = 0;
                return;
            }
        } while (++l);
        mico_throw (TypeMismatch());
    }
    if (disc->kind() == CORBA::tk_ulonglong) {
        CORBA::ULongLong l = 0;
        CORBA::Any any;
        do {
            any <<= l;
            if (tc->member_index (any) == defidx) {
                _elements[0]->from_any (any);
                _index = 0;
                return;
            }
        } while (++l);
        mico_throw (TypeMismatch());
    }
    if (disc->kind() == CORBA::tk_char) {
        CORBA::Char l = 0;
        CORBA::Any any;
        do {
            any <<= CORBA::Any::from_char (l);
            if (tc->member_index (any) == defidx) {
                _elements[0]->from_any (any);
                _index = 0;
                return;
            }
        } while (++l);
        mico_throw (TypeMismatch());
    }
    if (disc->kind() == CORBA::tk_wchar) {
        CORBA::WChar l = 0;
        CORBA::Any any;
        do {
            any <<= CORBA::Any::from_wchar (l);
            if (tc->member_index (any) == defidx) {
                _elements[0]->from_any (any);
                _index = 0;
                return;
            }
        } while (++l);
        mico_throw (TypeMismatch());
    }
    if (disc->kind() == CORBA::tk_short) {
        CORBA::Short l = 0;
        CORBA::Any any;
        do {
            any <<= l;
            if (tc->member_index (any) == defidx) {
                _elements[0]->from_any (any);
                _index = 0;
                return;
            }
        } while (++l);
        mico_throw (TypeMismatch());
    }
    if (disc->kind() == CORBA::tk_ushort) {
#if !defined(__GNUG__) && defined(__sgi)
        volatile
#endif
	    CORBA::UShort l = 0;
        CORBA::Any any;
        do {
            any <<= l;
            if (tc->member_index (any) == defidx) {
                _elements[0]->from_any (any);
                _index = 0;
                return;
            }
        } while (++l);
        mico_throw (TypeMismatch());
    }
    if (disc->kind() == CORBA::tk_boolean) {
        CORBA::Any any;
        any <<= CORBA::Any::from_boolean (TRUE);
        if (tc->member_index (any) == defidx) {
            _elements[0]->from_any (any);
            _index = 0;
            return;
        }
        any <<= CORBA::Any::from_boolean (FALSE);
        if (tc->member_index (any) == defidx) {
            _elements[0]->from_any (any);
            _index = 0;
            return;
        }
        mico_throw (TypeMismatch());
    }
}

void
DynUnion_impl::set_to_no_active_member ()
{
    CORBA::TypeCode_ptr tc = _type->unalias();
    CORBA::Long defidx = tc->default_index();

    CORBA::TypeCode_var disc = tc->discriminator_type();
    disc = CORBA::TypeCode::_duplicate (disc->unalias());

    if (defidx >= 0)
        mico_throw (TypeMismatch());

    if (disc->kind() == CORBA::tk_enum) {
        CORBA::Any any;
        any.set_type (disc);
        for (CORBA::ULong i = 0; i < disc->member_count(); ++i) {
            CORBA::Boolean r = any.enum_put (i);
            assert (r);
            if (tc->member_index (any) < 0) {
                _elements[0]->from_any (any);
                _index = 0;
                return;
            }
        }
        mico_throw (TypeMismatch());
    }
    if (disc->kind() == CORBA::tk_long) {
        CORBA::Long l = 0;
        CORBA::Any any;
        do {
            any <<= l;
            if (tc->member_index (any) < 0) {
                _elements[0]->from_any (any);
                _index = 0;
                return;
            }
        } while (++l);
        mico_throw (TypeMismatch());
    }
    if (disc->kind() == CORBA::tk_longlong) {
        CORBA::LongLong l = 0;
        CORBA::Any any;
        do {
            any <<= l;
            if (tc->member_index (any) < 0) {
                _elements[0]->from_any (any);
                _index = 0;
                return;
            }
        } while (++l);
        mico_throw (TypeMismatch());
    }
    if (disc->kind() == CORBA::tk_ulong) {
        CORBA::ULong l = 0;
        CORBA::Any any;
        do {
            any <<= l;
            if (tc->member_index (any) < 0) {
                _elements[0]->from_any (any);
                _index = 0;
                return;
            }
        } while (++l);
        mico_throw (TypeMismatch());
    }
    if (disc->kind() == CORBA::tk_ulonglong) {
        CORBA::ULongLong l = 0;
        CORBA::Any any;
        do {
            any <<= l;
            if (tc->member_index (any) < 0) {
                _elements[0]->from_any (any);
                _index = 0;
                return;
            }
        } while (++l);
        mico_throw (TypeMismatch());
    }
    if (disc->kind() == CORBA::tk_char) {
        CORBA::Char l = 0;
        CORBA::Any any;
        do {
            any <<= CORBA::Any::from_char (l);
            if (tc->member_index (any) < 0) {
                _elements[0]->from_any (any);
                _index = 0;
                return;
            }
        } while (++l);
        mico_throw (TypeMismatch());
    }
    if (disc->kind() == CORBA::tk_wchar) {
        CORBA::WChar l = 0;
        CORBA::Any any;
        do {
            any <<= CORBA::Any::from_wchar (l);
            if (tc->member_index (any) < 0) {
                _elements[0]->from_any (any);
                _index = 0;
                return;
            }
        } while (++l);
        mico_throw (TypeMismatch());
    }
    if (disc->kind() == CORBA::tk_short) {
        CORBA::Short l = 0;
        CORBA::Any any;
        do {
            any <<= l;
            if (tc->member_index (any) < 0) {
                _elements[0]->from_any (any);
                _index = 0;
                return;
            }
        } while (++l);
        mico_throw (TypeMismatch());
    }
    if (disc->kind() == CORBA::tk_ushort) {
#if !defined(__GNUG__) && defined(__sgi)
        volatile
#endif
	    CORBA::UShort l = 0;
        CORBA::Any any;
        do {
            any <<= l;
            if (tc->member_index (any) < 0) {
                _elements[0]->from_any (any);
                _index = 0;
                return;
            }
        } while (++l);
        mico_throw (TypeMismatch());
    }
    if (disc->kind() == CORBA::tk_boolean) {
        CORBA::Any any;
        any <<= CORBA::Any::from_boolean (TRUE);
        if (tc->member_index (any) < 0) {
            _elements[0]->from_any (any);
            _index = 0;
            return;
        }
        any <<= CORBA::Any::from_boolean (FALSE);
        if (tc->member_index (any) < 0) {
            _elements[0]->from_any (any);
            _index = 0;
            return;
        }
        mico_throw (TypeMismatch());
    }
}

CORBA::Boolean
DynUnion_impl::has_no_active_member ()
{
    CORBA::TypeCode_ptr tc = _type->unalias();
    CORBA::Long defidx = tc->default_index();

    if (defidx >= 0)
        return FALSE;

    update_element (1);
    return _elements.size() == 1;
}

DynamicAny::FieldName
DynUnion_impl::member_name ()
{
    if (component_count() == 1)
	mico_throw (InvalidValue());
    CORBA::Any_var disc = _elements[0]->to_any ();
    CORBA::Long idx = _type->unalias()->member_index (disc.in());
    assert (idx >= 0);
    return CORBA::string_dup (_type->unalias()->member_name (idx));
}

DynamicAny::DynAny_ptr
DynUnion_impl::member ()
{
    update_element (1);
    if (_elements.size() == 1)
	mico_throw (InvalidValue());
    return DynamicAny::DynAny::_duplicate (_elements[1]);
}

CORBA::TCKind
DynUnion_impl::member_kind ()
{
    // XXX unalias ???
    update_element (1);
    if (_elements.size() == 1)
	mico_throw (InvalidValue());
    CORBA::TypeCode_var tc = _elements[1]->type();
    return tc->kind();
}


/************************ DynSequence *****************************/


DynSequence_impl::DynSequence_impl (CORBA::TypeCode_ptr tc)
{
    _type = CORBA::TypeCode::_duplicate (tc);
    tc = tc->unalias ();
    if (tc->kind() != CORBA::tk_sequence)
	mico_throw (DynamicAny::DynAnyFactory::InconsistentTypeCode ());
    _length = 0;
    _index = -1;
}

DynSequence_impl::DynSequence_impl (const CORBA::Any &a)
{
    _type = a.type ();
    CORBA::TypeCode_ptr tc = _type->unalias ();
    if (tc->kind() != CORBA::tk_sequence)
	mico_throw (DynamicAny::DynAnyFactory::InconsistentTypeCode ());

    CORBA::Boolean r = a.seq_get_begin (_length);
    assert (r);
    for (CORBA::ULong i = 0; i < _length; ++i) {
	CORBA::Any el;
	r = a.any_get (el);
	assert (r);
// Here it is needed to set the Any type after calling any_get()
// else there is no deal with aliased typecodes.
// Note that calling el.set_type() before calling any_get() doesn't resolve the alias problem.
	CORBA::TypeCode_var member_tc = tc->content_type();
	el.type(member_tc);
//
	_elements.push_back (_factory()->create_dyn_any (el));
    }
    r = a.seq_get_end ();
    assert (r);

    if (_elements.size() == 0)
        _index = -1;
}

DynSequence_impl::~DynSequence_impl ()
{
}

void
DynSequence_impl::from_any (const CORBA::Any& value)
{
    CORBA::TypeCode_var tc = value.type();
    if (!_type->equaltype (tc))
	mico_throw (TypeMismatch ());

    CORBA::ULong len;
    CORBA::Boolean r = value.seq_get_begin (len);
    assert (r);
    if (len != _length)
	set_length (len);
    for (CORBA::ULong i = 0; i < _length; ++i) {
	CORBA::Any el;
	r = value.any_get (el);
	assert (r);
// Here it is needed to set the Any type after calling any_get()
// else there is no deal with aliased typecodes.
// Note that calling el.set_type() before calling any_get() doesn't resolve the alias problem.
	CORBA::TypeCode_var member_tc = tc->unalias()->content_type();
	el.type(member_tc);
//
	_elements[i]->from_any (el);
    }
    r = value.seq_get_end ();
    assert (r);
}

CORBA::Any*
DynSequence_impl::to_any ()
{
    CORBA::Any *a = new CORBA::Any;

    a->set_type (_type);
    CORBA::Boolean r = a->seq_put_begin (_length);
    assert (r);
    for (CORBA::ULong i = 0; i < _elements.size(); ++i) {
	CORBA::Any_var el = _elements[i]->to_any();
	r = a->any_put (el.inout());
	assert (r);
    }
    r = a->seq_put_end ();
    assert (r);
    return a;
}

CORBA::ULong
DynSequence_impl::get_length ()
{
    return _length;
}

void
DynSequence_impl::set_length (CORBA::ULong value)
{
    CORBA::TypeCode_ptr utc = _type->unalias();

    if (utc->length() > 0 && value > utc->length())
        mico_throw (InvalidValue());

    if (value < _elements.size ()) {
	_elements.erase (_elements.begin() + value, _elements.end());
	if (value == 0)
	    _index = -1;
	else if (_index >= (CORBA::Long)value)
	    _index = -1;
    } else if (value > _elements.size()) {
	CORBA::TypeCode_var tc = _type->unalias()->content_type ();
	for (CORBA::ULong i = 0; i < value - _length; ++i)
	    _elements.push_back (
                _factory()->create_dyn_any_from_type_code (tc.in()));
        if (_index < 0)
            _index = _length;
    }
    _length = value;
}

DynamicAny::AnySeq*
DynSequence_impl::get_elements ()
{
    DynamicAny::AnySeq *seq = new DynamicAny::AnySeq;
    seq->length (_length);

    for (CORBA::ULong i = 0; i < _length; ++i) {
	CORBA::Any_var el = _elements[i]->to_any ();
	(*seq)[i] = el.in();
    }
    return seq;
}

void
DynSequence_impl::set_elements (const DynamicAny::AnySeq& value)
{
    CORBA::TypeCode_ptr utc = _type->unalias();

    if (utc->length() > 0 && value.length() > utc->length())
        mico_throw (InvalidValue());

    _elements.erase (_elements.begin(), _elements.end());
    for (CORBA::ULong i = 0; i < value.length(); ++i)
	_elements.push_back (_factory()->create_dyn_any (value[i]));
    _length = value.length();
    _index = _length > 0 ? 0 : -1;
}

DynamicAny::DynAnySeq*
DynSequence_impl::get_elements_as_dyn_any ()
{
    DynamicAny::DynAnySeq *seq = new DynamicAny::DynAnySeq;
    seq->length (_length);

    for (CORBA::ULong i = 0; i < _length; ++i)
	(*seq)[i] = _elements[i]->copy();
    return seq;
}

void
DynSequence_impl::set_elements_as_dyn_any (const DynamicAny::DynAnySeq& value)
{
    CORBA::TypeCode_ptr utc = _type->unalias();

    if (utc->length() > 0 && value.length() > utc->length())
        mico_throw (InvalidValue());

    _elements.erase (_elements.begin(), _elements.end());
    for (CORBA::ULong i = 0; i < value.length(); ++i)
	_elements.push_back (value[i]->copy());
    _length = value.length();
    _index = _length > 0 ? 0 : -1;
}


/************************** DynArray *******************************/


DynArray_impl::DynArray_impl (CORBA::TypeCode_ptr tc)
{
    _type = CORBA::TypeCode::_duplicate (tc);
    tc = tc->unalias();
    if (tc->kind() != CORBA::tk_array)
	mico_throw (DynamicAny::DynAnyFactory::InconsistentTypeCode ());

    CORBA::ULong len = tc->length();
    CORBA::TypeCode_var eltc = tc->content_type();
    for (CORBA::ULong i = 0; i < len; ++i)
	_elements.push_back (
            _factory()->create_dyn_any_from_type_code (eltc.in()));
}

DynArray_impl::DynArray_impl (const CORBA::Any &a)
{
    _type = a.type ();
    CORBA::TypeCode_ptr tc = _type->unalias();
    if (tc->kind() != CORBA::tk_array)
	mico_throw (DynamicAny::DynAnyFactory::InconsistentTypeCode ());

    CORBA::ULong len = tc->length();
    CORBA::Boolean r = a.array_get_begin ();
    assert (r);
    for (CORBA::ULong i = 0; i < len; ++i) {
	CORBA::Any el;
	r = a.any_get (el);
	assert (r);
// Here it is needed to set the Any type after calling any_get()
// else there is no deal with aliased typecodes.
// Note that calling el.set_type() before calling any_get() doesn't resolve the alias problem.
	CORBA::TypeCode_var member_tc = tc->content_type();
	el.type(member_tc);
//
	_elements.push_back (_factory()->create_dyn_any (el));
    }
    r = a.array_get_end ();
    assert (r);
}

DynArray_impl::~DynArray_impl ()
{
}

void
DynArray_impl::from_any (const CORBA::Any& value)
{
    CORBA::TypeCode_var tc = value.type();
    if (!_type->equaltype (tc))
	mico_throw (TypeMismatch ());

    CORBA::ULong len = tc->unalias()->length();
    CORBA::Boolean r = value.array_get_begin ();
    assert (r);
    for (CORBA::ULong i = 0; i < len; ++i) {
	CORBA::Any el;
	r = value.any_get (el);
	assert (r);
// Here it is needed to set the Any type after calling any_get()
// else there is no deal with aliased typecodes.
// Note that calling el.set_type() before calling any_get() doesn't resolve the alias problem.
	CORBA::TypeCode_var member_tc = tc->unalias()->content_type();
	el.type(member_tc);
//
	_elements[i]->from_any (el);
    }
    r = value.array_get_end ();
    assert (r);
}

CORBA::Any*
DynArray_impl::to_any ()
{
    CORBA::Any *a = new CORBA::Any;

    a->set_type (_type);
    CORBA::Boolean r = a->array_put_begin ();
    assert (r);
    for (CORBA::ULong i = 0; i < _elements.size(); ++i) {
	CORBA::Any_var el = _elements[i]->to_any ();
	r = a->any_put (el.inout());
	assert (r);
    }
    r = a->array_put_end ();
    assert (r);
    return a;
}

DynamicAny::AnySeq*
DynArray_impl::get_elements ()
{
    DynamicAny::AnySeq *seq = new DynamicAny::AnySeq;
    seq->length (_elements.size());

    for (CORBA::ULong i = 0; i < _elements.size(); ++i) {
	CORBA::Any_var el = _elements[i]->to_any ();
	(*seq)[i] = el.in();
    }
    return seq;
}

void
DynArray_impl::set_elements (const DynamicAny::AnySeq& value)
{
    if (value.length() != _elements.size())
	mico_throw (TypeMismatch ());

    for (CORBA::ULong i = 0; i < _elements.size(); ++i)
	_elements[i]->from_any (value[i]);
}

DynamicAny::DynAnySeq*
DynArray_impl::get_elements_as_dyn_any ()
{
    DynamicAny::DynAnySeq *seq = new DynamicAny::DynAnySeq;
    seq->length (_elements.size());

    for (CORBA::ULong i = 0; i < _elements.size(); ++i)
	(*seq)[i] = _elements[i]->copy();
    return seq;
}

void
DynArray_impl::set_elements_as_dyn_any (const DynamicAny::DynAnySeq& value)
{
    if (value.length() != _elements.size())
	mico_throw (TypeMismatch ());

    for (CORBA::ULong i = 0; i < _elements.size(); ++i)
	_elements[i]->assign (value[i].in());
}

/************************** DynValueCommon *************************/

DynValueCommon_impl::DynValueCommon_impl ()
{
  _is_null = 1;
  _index = -1;
}

CORBA::Boolean
DynValueCommon_impl::is_null ()
{
  return _is_null;
}

void
DynValueCommon_impl::set_to_null ()
{
  _is_null = 1;
  _index = -1;
}

void
DynValueCommon_impl::set_to_value ()
{
  _is_null = 0;
  _index = 0;
}

/************************** DynValue *******************************/

/*
 * XXX Shared valuetypes end up in separate copies when using
 *     DynValue. This is due to the way CORBA::Any is implemented.
 */

DynValue_impl::DynValue_impl (CORBA::TypeCode_ptr tc)
{
    _type = CORBA::TypeCode::_duplicate (tc);
    tc = tc->unalias();
    if (tc->kind() != CORBA::tk_value)
	mico_throw (DynamicAny::DynAnyFactory::InconsistentTypeCode ());
    for (CORBA::ULong i = 0; i < tc->member_count_inherited(); ++i) {
	_elements.push_back (DynamicAny::DynAny::_nil());
    }
    _index = -1;
}

DynValue_impl::DynValue_impl (const CORBA::Any &a)
{
    _type = a.type();
    CORBA::TypeCode_ptr tc = _type->unalias();
    if (tc->kind() != CORBA::tk_value)
	mico_throw (DynamicAny::DynAnyFactory::InconsistentTypeCode ());

    CORBA::Long value_id;
    CORBA::Boolean is_ref;

    CORBA::Boolean r = a.value_get_begin (value_id, is_ref);
    assert (r);

    if (is_ref) {

      for (CORBA::ULong i = 0; i < tc->member_count_inherited(); ++i) {
	_elements.push_back (DynamicAny::DynValue::_nil ());
      }

      if (!value_id) {
	_is_null = 1;
	_index = -1;
      }
      else {
	_is_null = 0;
	// XXX shared value
	assert (0);
      }
    }
    else {
      _is_null = 0;

      for (CORBA::ULong i = 0; i < tc->member_count_inherited(); ++i) {
	CORBA::Any el;
	r = a.any_get (el);
	assert (r);
// Here it is needed to set the Any type after calling any_get()
// else there is no deal with aliased typecodes.
// Note that calling el.set_type() before calling any_get() doesn't resolve the alias problem.
	CORBA::TypeCode_var member_tc = tc->member_type_inherited(i);
	el.type(member_tc);
//
	_elements.push_back (_factory()->create_dyn_any (el));
      }

      r = a.value_get_end (value_id, is_ref);
      assert (r);
    }

    if (_elements.size() == 0)
        _index = -1;
}

DynValue_impl::~DynValue_impl ()
{
}

void
DynValue_impl::update_element (CORBA::Long idx)
{
  assert(idx >= 0);
  if ((CORBA::ULong)idx < _type->member_count_inherited() &&
      CORBA::is_nil (_elements[idx])) {
    CORBA::TypeCode_var member_tc = _type->member_type_inherited (idx);
    _elements[idx] = _factory()->create_dyn_any_from_type_code (member_tc);
  }
}

void
DynValue_impl::from_any (const CORBA::Any& a)
{
    CORBA::TypeCode_var tc = a.type();
    if (!_type->equaltype (tc))
	mico_throw (TypeMismatch ());

    CORBA::Long value_id;
    CORBA::Boolean is_ref;

    CORBA::Boolean r = a.value_get_begin (value_id, is_ref);
    assert (r);

    if (is_ref) {
      if (!value_id) {
	_is_null = 1;
	return;
      }

      // XXX shared value
      assert (0);
    }

    _is_null = 0;

    CORBA::TypeCode_ptr tc2 = tc->unalias();
    for (CORBA::ULong i = 0; i < tc2->member_count_inherited(); ++i) {
	CORBA::Any el;
	r = a.any_get (el);
	assert (r);
	CORBA::TypeCode_var member_tc = tc2->member_type_inherited(i);
	el.type(member_tc);
	_elements[i]->from_any (el);
    }

    r = a.value_get_end (value_id, is_ref);
    assert (r);
}

CORBA::Any*
DynValue_impl::to_any ()
{
    CORBA::Any *a = new CORBA::Any;

    a->set_type (_type);

    CORBA::Long value_id;

    if (_is_null) {
      a->value_put_ref (0);
      return a;
    }

    CORBA::Boolean r = a->value_put_begin (value_id);
    assert (r);

    for (CORBA::ULong i = 0; i < _elements.size(); ++i) {
        update_element (i);
	CORBA::Any_var el = _elements[i]->to_any ();
	r = a->any_put (el.inout());
	assert (r);
    }

    r = a->value_put_end (value_id);
    assert (r);

    return a;
}

DynamicAny::FieldName
DynValue_impl::current_member_name ()
{
    if (_is_null || _index < 0)
        mico_throw (TypeMismatch());
    return CORBA::string_dup (_type->unalias()->member_name_inherited(_index));
}

CORBA::TCKind
DynValue_impl::current_member_kind ()
{
    if (_is_null || _index < 0)
        mico_throw (TypeMismatch());
    // XXX unlias ???
    CORBA::TypeCode_var tc = _type->unalias()->member_type_inherited (_index);
    return tc->kind();
}

DynamicAny::NameValuePairSeq*
DynValue_impl::get_members ()
{
  if (_is_null) {
    mico_throw (InvalidValue());
  }

    DynamicAny::NameValuePairSeq *seq = new DynamicAny::NameValuePairSeq;
    seq->length (_elements.size());
    CORBA::TypeCode_ptr tc = _type->unalias ();
    for (CORBA::ULong i = 0; i < _elements.size(); ++i) {
        update_element (i);
	(*seq)[i].id = tc->member_name_inherited (i);
	CORBA::Any_var a = _elements[i]->to_any ();
	(*seq)[i].value = a.in();
    }
    return seq;
}

void
DynValue_impl::set_members (const DynamicAny::NameValuePairSeq& value)
{
    CORBA::TypeCode_ptr tc = _type->unalias ();
    if (value.length() != tc->member_count_inherited())
	mico_throw (TypeMismatch ());

    for (CORBA::ULong i = 0; i < value.length(); ++i) {
	if (strlen (value[i].id) > 0 &&
            strcmp (tc->member_name_inherited(i), value[i].id))
	    mico_throw (TypeMismatch ());
	_elements[i]->from_any (value[i].value);
    }
    _index = _elements.size() > 0 ? 0 : -1;
    _is_null = 0;
}

DynamicAny::NameDynAnyPairSeq*
DynValue_impl::get_members_as_dyn_any ()
{
  if (_is_null) {
    mico_throw (InvalidValue());
  }

    DynamicAny::NameDynAnyPairSeq *seq = new DynamicAny::NameDynAnyPairSeq;
    seq->length (_elements.size());
    CORBA::TypeCode_ptr tc = _type->unalias ();
    for (CORBA::ULong i = 0; i < _elements.size(); ++i) {
        update_element (i);
	(*seq)[i].id = tc->member_name_inherited (i);
	(*seq)[i].value = _elements[i]->copy();
    }
    return seq;
}

void
DynValue_impl::set_members_as_dyn_any (
    const DynamicAny::NameDynAnyPairSeq& value)
{
    CORBA::TypeCode_ptr tc = _type->unalias ();
    if (value.length() != tc->member_count())
	mico_throw (TypeMismatch ());

    for (CORBA::ULong i = 0; i < value.length(); ++i) {
	if (strlen (value[i].id) > 0 &&
            strcmp (tc->member_name_inherited(i), value[i].id))
	    mico_throw (TypeMismatch ());
        this->update_element(i);
	_elements[i]->assign (value[i].value.in());
    }
    _index = _elements.size() > 0 ? 0 : -1;
    _is_null = 0;
}


/************************** DynValue *******************************/

/*
 * XXX Shared valuetypes end up in separate copies when using
 *     DynValue. This is due to the way CORBA::Any is implemented.
 */

DynValueBox_impl::DynValueBox_impl (CORBA::TypeCode_ptr tc)
{
    _type = CORBA::TypeCode::_duplicate (tc);
    tc = tc->unalias();
    if (tc->kind() != CORBA::tk_value_box)
	mico_throw (DynamicAny::DynAnyFactory::InconsistentTypeCode ());
    CORBA::TypeCode_var memtc = tc->content_type ();
    _elements.push_back(_factory()->create_dyn_any_from_type_code (memtc));
    _index = -1;
}

DynValueBox_impl::DynValueBox_impl (const CORBA::Any &a)
{
    _type = a.type();
    CORBA::TypeCode_ptr tc = _type->unalias();
    if (tc->kind() != CORBA::tk_value_box)
	mico_throw (DynamicAny::DynAnyFactory::InconsistentTypeCode ());

    CORBA::Long value_id;
    CORBA::Boolean is_ref;

    CORBA::Boolean r = a.valuebox_get_begin (value_id, is_ref);
    assert (r);

    if (is_ref) {
      CORBA::TypeCode_var memtc = tc->content_type ();
      _elements.push_back(_factory()->create_dyn_any_from_type_code (memtc));

      if (!value_id) {
        _index = -1;
	_is_null = 1;
      }
      else {
	_is_null = 0;
	// XXX shared value
	assert (0);
      }
    }
    else {
      _is_null = 0;

      CORBA::TypeCode_var memtc = tc->content_type ();
      CORBA::Any el;
      r = a.any_get (el);
      assert (r);
      el.type (memtc);
      
      _elements.push_back (_factory()->create_dyn_any (el));
      
      r = a.valuebox_get_end (value_id, is_ref);
      assert (r);
    }
}

DynValueBox_impl::~DynValueBox_impl ()
{
}

void
DynValueBox_impl::from_any (const CORBA::Any& a)
{
    CORBA::TypeCode_var tc = a.type();
    if (!_type->equaltype (tc))
	mico_throw (TypeMismatch ());

    CORBA::Long value_id;
    CORBA::Boolean is_ref;
    CORBA::Boolean r = a.valuebox_get_begin (value_id, is_ref);
    assert (r);

    if (is_ref) {
      if (!value_id) {
	_is_null = 1;
	return;
      }

      // XXX shared value
      assert (0);
    }

    _is_null = 0;

    CORBA::TypeCode_ptr tc2 = tc->unalias();
    CORBA::TypeCode_var member_tc = tc2->content_type();
    CORBA::Any el;
    r = a.any_get (el);
    assert (r);
    el.type (member_tc);
    _elements[0]->from_any (el);

    r = a.valuebox_get_end (value_id, is_ref);
    assert (r);
}

CORBA::Any*
DynValueBox_impl::to_any ()
{
    CORBA::Any *a = new CORBA::Any;

    a->set_type (_type);

    CORBA::Long value_id;

    if (_is_null) {
      a->value_put_ref (0);
      return a;
    }

    CORBA::Boolean r = a->valuebox_put_begin (value_id);
    assert (r);

    CORBA::Any_var el = _elements[0]->to_any ();
    r = a->any_put (el.inout());
    assert (r);

    r = a->valuebox_put_end (value_id);
    assert (r);

    return a;
}

CORBA::Any *
DynValueBox_impl::get_boxed_value ()
{
  if (_is_null) {
    mico_throw (InvalidValue());
  }

  return _elements[0]->to_any ();
}

void
DynValueBox_impl::set_boxed_value (const CORBA::Any & boxed)
{
  _elements[0]->from_any (boxed);
  _is_null = 0;
  _index = 0;
}

DynamicAny::DynAny_ptr
DynValueBox_impl::get_boxed_value_as_dyn_any ()
{
  if (_is_null) {
    mico_throw (InvalidValue());
  }

  return _elements[0]->copy ();
}

void
DynValueBox_impl::set_boxed_value_as_dyn_any (DynamicAny::DynAny_ptr boxed)
{
  _elements[0]->assign (boxed);
  _is_null = 0;
  _index = 0;
}

/************************* DynAnyFactory ***************************/


DynAnyFactory_impl::DynAnyFactory_impl ()
{
}

DynAnyFactory_impl::~DynAnyFactory_impl ()
{
}

DynamicAny::DynAny_ptr
DynAnyFactory_impl::create_dyn_any (const CORBA::Any& value)
{
    CORBA::TypeCode_var tc = value.type();
    switch (tc->unalias()->kind()) {
    case CORBA::tk_void:
    case CORBA::tk_null:
    case CORBA::tk_char:
    case CORBA::tk_wchar:
    case CORBA::tk_boolean:
    case CORBA::tk_octet:
    case CORBA::tk_short:
    case CORBA::tk_ushort:
    case CORBA::tk_long:
    case CORBA::tk_longlong:
    case CORBA::tk_ulong:
    case CORBA::tk_ulonglong:
    case CORBA::tk_float:
    case CORBA::tk_double:
    case CORBA::tk_longdouble:
    case CORBA::tk_any:
    case CORBA::tk_TypeCode:
    case CORBA::tk_objref:
    case CORBA::tk_string:
    case CORBA::tk_wstring:
	return DynamicAny::DynBasic::_duplicate (new DynBasic_impl (value));
    case CORBA::tk_enum:
	return DynamicAny::DynEnum::_duplicate (new DynEnum_impl (value));
    case CORBA::tk_struct:
    case CORBA::tk_except:
	return DynamicAny::DynStruct::_duplicate (new DynStruct_impl (value));
    case CORBA::tk_union:
	return DynamicAny::DynUnion::_duplicate (new DynUnion_impl (value));
    case CORBA::tk_sequence:
	return DynamicAny::DynSequence::_duplicate (
	    new DynSequence_impl (value));
    case CORBA::tk_array:
	return DynamicAny::DynArray::_duplicate (new DynArray_impl (value));
    case CORBA::tk_fixed:
	return DynamicAny::DynFixed::_duplicate (new DynFixed_impl (value));
    case CORBA::tk_value:
	return DynamicAny::DynValue::_duplicate (new DynValue_impl (value));
    case CORBA::tk_value_box:
	return DynamicAny::DynValueBox::_duplicate (new DynValueBox_impl (value));
    default:
	mico_throw (InconsistentTypeCode());
    }
    return 0;
}

DynamicAny::DynAny_ptr
DynAnyFactory_impl::create_dyn_any_from_type_code (CORBA::TypeCode_ptr type)
{
    switch (type->unalias()->kind()) {
    case CORBA::tk_void:
    case CORBA::tk_null:
    case CORBA::tk_char:
    case CORBA::tk_wchar:
    case CORBA::tk_boolean:
    case CORBA::tk_octet:
    case CORBA::tk_short:
    case CORBA::tk_ushort:
    case CORBA::tk_long:
    case CORBA::tk_longlong:
    case CORBA::tk_ulong:
    case CORBA::tk_ulonglong:
    case CORBA::tk_float:
    case CORBA::tk_double:
    case CORBA::tk_longdouble:
    case CORBA::tk_any:
    case CORBA::tk_TypeCode:
    case CORBA::tk_objref:
    case CORBA::tk_string:
    case CORBA::tk_wstring:
	return DynamicAny::DynBasic::_duplicate (new DynBasic_impl (type));
    case CORBA::tk_enum:
	return DynamicAny::DynEnum::_duplicate (new DynEnum_impl (type));
    case CORBA::tk_struct:
    case CORBA::tk_except:
	return DynamicAny::DynStruct::_duplicate (new DynStruct_impl (type));
    case CORBA::tk_union:
	return DynamicAny::DynUnion::_duplicate (new DynUnion_impl (type));
    case CORBA::tk_sequence:
	return DynamicAny::DynSequence::_duplicate (
	    new DynSequence_impl (type));
    case CORBA::tk_array:
	return DynamicAny::DynArray::_duplicate (new DynArray_impl (type));
    case CORBA::tk_fixed:
	return DynamicAny::DynFixed::_duplicate (new DynFixed_impl (type));
    case CORBA::tk_value:
	return DynamicAny::DynValue::_duplicate (new DynValue_impl (type));
    case CORBA::tk_value_box:
      return DynamicAny::DynValueBox::_duplicate (new DynValueBox_impl (type));
    default:
	mico_throw (InconsistentTypeCode());
    }
    return 0;
}
