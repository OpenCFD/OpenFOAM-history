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

#ifndef __CosStream_StreamIO_impl_h__
#define __CosStream_StreamIO_impl_h__

#include <coss/CosExternalization.h>

struct RoleNameKey
{
    CosObjectIdentity::ObjectIdentifier id;
    CORBA::String_var IDef;
    CORBA::String_var role_name;
    RoleNameKey(CosObjectIdentity::ObjectIdentifier& id_, const char* IDef_, const char* role_name_)
		{ id = id_; IDef = CORBA::string_dup(IDef_); role_name = CORBA::string_dup(role_name_); };
    RoleNameKey(const RoleNameKey& key)
		{ id = key.id; IDef = CORBA::string_dup(key.IDef); role_name = CORBA::string_dup(key.role_name); };
    ~RoleNameKey()
		{};
    CORBA::Boolean operator<(RoleNameKey& key) const
		{
		    if (id<key.id)
			return TRUE;
		    if (id>key.id)
			return FALSE;
		    return (strcmp(IDef,key.IDef) < 0);
		};
    CORBA::Boolean operator<(const RoleNameKey& key) const
		{
		    if (id<key.id)
			return TRUE;
		    if (id>key.id)
			return FALSE;
		    return (strcmp(IDef,key.IDef) < 0);
		};
};

class StreamIO_impl : virtual public POA_CosStream::StreamIO
{
    std::istream* istream_;
    std::ostream* ostream_;
    enum in_out
    {
	in,
	out
    } iotype;

protected:
    CORBA::Boolean was_extern(std::vector<CosObjectIdentity::ObjectIdentifier>* vec_,
			      CosObjectIdentity::ObjectIdentifier id);
public:
    StreamIO_impl(std::ostream* ostream_ptr = &std::cout)
	: ostream_(ostream_ptr), iotype(out) { };
    StreamIO_impl(std::istream* istream_ptr = &std::cin)
	: istream_(istream_ptr), iotype(in) { };

    virtual void write_string(const char* aString);

    virtual void write_char(CORBA::Char aChar);

    virtual void write_octet(CORBA::Octet anOctet);

    virtual void write_unsigned_long(CORBA::ULong anUnsignedLong);

    virtual void write_unsigned_short(CORBA::UShort anUnsignedShort);

    virtual void write_long(CORBA::Long aLong);

    virtual void write_short(CORBA::Short aShort);

    virtual void write_float(CORBA::Float aFloat);

    virtual void write_double(CORBA::Double aDouble);

    virtual void write_boolean(CORBA::Boolean aBoolean);

    virtual void write_object(CosStream::Streamable_ptr aStreamable);

    virtual void write_graph(CosCompoundExternalization::Node_ptr starting_node);

    virtual void StreamIO_impl::write_long_long( CORBA::LongLong val );

    virtual void StreamIO_impl::write_unsigned_long_long( CORBA::ULongLong val );

    virtual void StreamIO_impl::write_long_double( CORBA::LongDouble val );

    virtual void write_fixed(const CORBA::Any& val, CORBA::Short s);
    
    virtual char* read_string();

    virtual CORBA::Char read_char();

    virtual CORBA::Octet read_octet();

    virtual CORBA::ULong read_unsigned_long();

    virtual CORBA::UShort read_unsigned_short();

    virtual CORBA::Long read_long();

    virtual CORBA::Short read_short();

    virtual CORBA::Float read_float();

    virtual CORBA::Double read_double();

    virtual CORBA::Boolean read_boolean();

    virtual CosStream::Streamable_ptr read_object(CosLifeCycle::FactoryFinder_ptr there,
                                                  CosStream::Streamable_ptr aStreamable);

    virtual void read_graph(CosCompoundExternalization::Node_ptr starting_node,
                            CosLifeCycle::FactoryFinder_ptr there);

    virtual CORBA::LongLong read_long_long();

    virtual CORBA::ULongLong read_unsigned_long_long();

    virtual CORBA::LongDouble read_long_double();

    virtual CORBA::Any* read_fixed();
};

template <class T>
class StreamableFactory_impl : virtual public POA_CosStream::StreamableFactory

{
public:
    StreamableFactory_impl();
    virtual CosStream::Streamable_ptr create_uninitialized ();
};

template <class T>
StreamableFactory_impl<T>::StreamableFactory_impl()
    : POA_CosStream::StreamableFactory()
{
};

// ----------------------------------------------------------------------
// CosCompoundExternalization_NodeFactory_impl public member implementation
// ----------------------------------------------------------------------

template <class T>
CosStream::Streamable_ptr
StreamableFactory_impl<T>::create_uninitialized()
{
  T* tmp_streamable = new T ();
  CosStream::Streamable_ptr streamable = tmp_streamable->_this ();

  return streamable;
};

extern const char tag_object;
extern const char tag_repeat_object;
extern const char tag_nil;
extern const char tag_char;
extern const char tag_octet;
extern const char tag_ulong;
extern const char tag_ushort;
extern const char tag_long;
extern const char tag_short;
extern const char tag_float;
extern const char tag_double;
extern const char tag_bool;
extern const char tag_string;
extern const char tag_longlong;
extern const char tag_ulonglong;
extern const char tag_longdouble;


#endif /* __CosStream_Stream_impl_h__ */
