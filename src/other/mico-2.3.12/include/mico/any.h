// -*- c++ -*-
/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
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

#ifndef __mico_any_h__
#define __mico_any_h__

namespace CORBA {

class Any {
    typedef std::map<Long, Long, std::less<Long> > MapLL;

    TypeCodeChecker *checker;
    DataEncoder *ec;
    DataDecoder *dc;
    TypeCode_ptr thetc;
    DataEncoder::ValueState value_estate;
    DataDecoder::ValueState value_dstate;
    StaticAny *extracted_value;

    TypeCode_ptr tc () const;
    void tc (TypeCode_ptr);
    void tc_if_changed (TypeCode_ptr);

    void free ();
    void copy (const Any &);
    void reset_extracted_value ();

    void prepare_write ();
    void prepare_read ();
    Boolean copy_any (Any &, MapLL &value_id_map, Boolean recurse = TRUE);
    Boolean copy_any (Any &, Boolean recurse = TRUE);
    Boolean compare_any (Any &,	MapLL &value_id_map);
    Boolean compare_any (Any &);

    // dummy argument to distinguish from public Any ctor
    Any (TypeCode_ptr tc, Buffer *, Boolean release, int dummy);
    Any (TypeCode_ptr tc, DataDecoder *, DataEncoder *);

    Boolean get_short (Short &);
    Boolean get_ushort (UShort &);
    Boolean get_long (Long &);
    Boolean get_longlong (LongLong &);
    Boolean get_ulong (ULong &);
    Boolean get_ulonglong (ULongLong &);
    Boolean get_float (Float &);
    Boolean get_double (Double &);
    Boolean get_longdouble (LongDouble &);
public:
    Any ();
    Any (const Any &);
    Any (TypeCode_ptr tc, void *value, Boolean release = FALSE);
    ~Any ();

    Any &operator= (const Any &);
    // begin-mico-extension
    Boolean operator== (const Any &) const;
    Boolean equivalent (const Any &) const;
    // end-mico-extension

    struct from_boolean {
	from_boolean (Boolean b) : val (b) {}
	Boolean val;
    };
    struct from_octet {
	from_octet (Octet o) : val (o) {}
	Octet val;
    };
    struct from_char {
	from_char (Char c) : val (c) {}
	Char val;
    };
    struct from_wchar {
	from_wchar (WChar c) : val (c) {}
	WChar val;
    };
    struct from_string {
	from_string (char *s, ULong b, Boolean nc = FALSE)
	    : val (s), bound (b), nocopy (nc) {}
	from_string (const char *s, ULong b)
	    : val ((char *)s), bound (b), nocopy (FALSE) {}
	char *val;
	ULong bound;
	Boolean nocopy;
    };
    struct from_wstring {
	from_wstring (wchar_t *s, ULong b, Boolean nc = FALSE)
	    : val (s), bound (b), nocopy (nc) {}
	from_wstring (const wchar_t *s, ULong b)
	    : val ((wchar_t *)s), bound (b), nocopy (FALSE) {}
	wchar_t *val;
	ULong bound;
	Boolean nocopy;
    };
    struct from_fixed {
	from_fixed (const Fixed &f, UShort d, Short s)
            : fixed (f), digits (d), scale (s) {}
	const Fixed &fixed;
        UShort digits;
        Short scale;
    };
    // begin-mico-extension
    struct from_object {
	from_object (Object_ptr o, const char *n) : name (n), val (o) {}
	const char *name;
	Object_ptr val;
    };
    // end-mico-extension

    void operator<<= (Short);
    void operator<<= (Long);
    void operator<<= (LongLong);
    void operator<<= (UShort);
    void operator<<= (ULong);
    void operator<<= (ULongLong);
    void operator<<= (Float);
    void operator<<= (Double);
    void operator<<= (LongDouble);
    void operator<<= (const Any &);
    void operator<<= (Any *);
    void operator<<= (const Exception &);
    void operator<<= (Exception *);
    void operator<<= (const char *);
    void operator<<= (const wchar_t *);
    void operator<<= (from_boolean);
    void operator<<= (from_octet);
    void operator<<= (from_char);
    void operator<<= (from_wchar);
    void operator<<= (from_string);
    void operator<<= (from_wstring);
    void operator<<= (AbstractBase*);
    void operator<<= (AbstractBase**);
    // begin-mico-extension
    void operator<<= (from_fixed);
    void operator<<= (from_object);
    void operator<<= (Object_ptr);
    void operator<<= (Object_ptr*);
    void operator<<= (TypeCode_ptr);
    void operator<<= (TypeCode_ptr*);
    void operator<<= (Context_ptr);
    void operator<<= (Context_ptr*);
    void operator<<= (Principal_ptr);
    void operator<<= (Principal_ptr*);

    Boolean insert (Short);
    Boolean insert (Long);
    Boolean insert (LongLong);
    Boolean insert (UShort);
    Boolean insert (ULong);
    Boolean insert (ULongLong);
    Boolean insert (Float);
    Boolean insert (Double);
    Boolean insert (LongDouble);
    Boolean insert (const Any &);
    Boolean insert (Any *);
    Boolean insert (const Exception &);
    Boolean insert (Exception *);
    Boolean insert (const char *);
    Boolean insert (const wchar_t *);
    Boolean insert (from_boolean);
    Boolean insert (from_octet);
    Boolean insert (from_char);
    Boolean insert (from_wchar);
    Boolean insert (from_string);
    Boolean insert (from_wstring);
    Boolean insert (from_fixed);
    Boolean insert (from_object);
    Boolean insert (Object_ptr);
    Boolean insert (Object_ptr*);
    Boolean insert (TypeCode_ptr);
    Boolean insert (TypeCode_ptr*);
    Boolean insert (Context_ptr);
    Boolean insert (Context_ptr*);
    Boolean insert (Principal_ptr);
    Boolean insert (Principal_ptr*);
    Boolean insert (AbstractBase*);
    Boolean insert (AbstractBase**);
    // end-mico-extension

    struct to_boolean {
	to_boolean (Boolean &b) : ref (b) {}
	Boolean &ref;
    };
    struct to_octet {
	to_octet (Octet &o) : ref (o) {}
	Octet &ref;
    };
    struct to_char {
	to_char (Char &c) : ref (c) {}
	Char &ref;
    };
    struct to_wchar {
	to_wchar (WChar &c) : ref (c) {}
	WChar &ref;
    };
    struct to_string {
	to_string (const char * &s, ULong b) : ref (s), bound (b) {}
	const char * &ref;
	ULong bound;
    };
    struct to_wstring {
	to_wstring (const wchar_t * &s, ULong b) : ref (s), bound (b) {}
	const wchar_t * &ref;
	ULong bound;
    };
    struct to_object {
	to_object (Object_out o)
	    : ref (o.ptr()), name (0) {}
	to_object (Object_out o, String_out n)
	    : ref (o.ptr()), name (&n.ptr()) {}
	Object_ptr &ref;
	char **name;
    };
    struct to_fixed {
	to_fixed (Fixed &f, UShort d, Short s)
            : fixed (f), digits (d), scale (s) {}
	Fixed &fixed;
        UShort digits;
        Short scale;
    };
    struct to_abstract_base {
        to_abstract_base (AbstractBase *& base)
	  : ref (base) {}
        AbstractBase *& ref;
    };

    Boolean operator>>= (Short &) const;
    Boolean operator>>= (Long &) const;
    Boolean operator>>= (LongLong &) const;
    Boolean operator>>= (UShort &) const;
    Boolean operator>>= (ULong &) const;
    Boolean operator>>= (ULongLong &) const;
    Boolean operator>>= (Float &) const;
    Boolean operator>>= (Double &) const;
    Boolean operator>>= (LongDouble &) const;
    Boolean operator>>= (const Any *&) const;
    Boolean operator>>= (const char * &) const;
    Boolean operator>>= (const wchar_t * &) const;
    Boolean operator>>= (to_boolean) const;
    Boolean operator>>= (to_octet) const;
    Boolean operator>>= (to_char) const;
    Boolean operator>>= (to_wchar) const;
    Boolean operator>>= (to_string) const;
    Boolean operator>>= (to_wstring) const;
    Boolean operator>>= (to_object) const;
    Boolean operator>>= (to_abstract_base) const;
    // begin-mico-extension
    Boolean operator>>= (to_fixed) const;
    Boolean operator>>= (Object_ptr &) const;
    Boolean operator>>= (TypeCode_ptr &) const;
    Boolean operator>>= (Context_ptr &) const;
    Boolean operator>>= (Principal_ptr &) const;
    // end-mico-extension

    void replace (TypeCode_ptr tc, void *value, Boolean release = FALSE);

    // mico-extension
    void replace (TypeCode_ptr tc);

    const void *value () const;
    TypeCode_ptr type () const;
    void type (TypeCode_ptr);
    // begin-mico-extension
    ULong length () const;
    void set_type (TypeCode_ptr);
    void reset ();
    void rewind ();

    void encode (DataEncoder &) const;
    Boolean decode (DataDecoder &);

    Boolean marshal (DataEncoder &);
    Boolean demarshal (TypeCode_ptr, DataDecoder &);

    Boolean from_static_any (const StaticAny &);
    Boolean from_static_any (const StaticAny &, TypeCode_ptr);
    Boolean to_static_any (StaticAny &) const;
    Boolean to_static_any (StaticTypeInfo *ti, void *&value) const;
    Boolean to_static_any (StaticTypeInfo *ti, CORBA::TypeCode_ptr,
			   void *&value) const;
    Boolean to_static_any (StaticAny &, TypeCode_ptr) const;

    Boolean coerce (CORBA::Any &dst) const;
    // end-mico-extension

    // begin-mico-extension
    // typesafe interface for constructed types
    Boolean struct_put_begin ();
    Boolean struct_put_end ();

    Boolean union_put_begin ();
    Boolean union_put_selection (Long idx);
    Boolean union_put_end ();

    Boolean enum_put (ULong val);
    Boolean any_put (Any &, Boolean recurse = TRUE);

    Boolean seq_put_begin (ULong len);
    Boolean seq_put_end ();

    Boolean array_put_begin ();
    Boolean array_put_end ();

    Boolean except_put_begin (const char *repoid);
    Boolean except_put_end ();

    Boolean value_put_begin (Long &value_id);
    Boolean value_put_end (Long value_id);
    Boolean value_put_ref (Long value_id);

    Boolean valuebox_put_begin (Long &value_id);
    Boolean valuebox_put_end (Long value_id);
    Boolean valuebox_put_ref (Long value_id);


    Boolean struct_get_begin () const;
    Boolean struct_get_end () const;

    Boolean union_get_begin () const;
    Boolean union_get_selection (Long idx) const;
    Boolean union_get_end () const;

    Boolean enum_get (ULong &val) const;
    Boolean any_get (Any &, Boolean recurse = TRUE) const;

    Boolean seq_get_begin (ULong &len) const;
    Boolean seq_get_end () const;

    Boolean array_get_begin () const;
    Boolean array_get_end () const;

    Boolean except_get_begin (String_out repoid) const;
    Boolean except_get_end () const;

    Boolean value_get_begin (Long &value_id, Boolean &is_ref) const;
    Boolean value_get_end (Long value_id, Boolean is_ref) const;

    Boolean valuebox_get_begin (Long &value_id, Boolean &is_ref) const;
    Boolean valuebox_get_end (Long value_id, Boolean is_ref) const;
    // end-mico-extension

private:
    void operator<<= (unsigned char) {}
    void operator>>= (unsigned char &) const {}
};

typedef TVarVar<Any> Any_var;
typedef TVarOut<Any> Any_out;

}

#endif // __mico_any_h__
