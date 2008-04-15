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

#ifndef __mico_typecode_h__
#define __mico_typecode_h__

namespace CORBA {

class TypeCode : public ServerlessObject {
    TCKind tckind;
    std::string repoid;
    std::string tcname;
    std::vector<std::string> namevec;
    std::vector<TypeCode_ptr> tcvec;
    std::vector<Any *> labelvec;
    std::vector<Visibility> visvec;
    TypeCode_ptr content;
    TypeCode_ptr discriminator;
    TypeCode_ptr recurse_tc;
    Long defidx;
    Long len;
    Long recurse_depth;
    Boolean isconst;
    UShort digits;
    Short scale;
    ValueModifier value_mod;

    void init ();
    void copy (const TypeCode &);
    void free ();

    typedef std::pair<TypeCode_ptr, TypeCode_ptr> PairTC;
    typedef std::set<PairTC, std::less<PairTC> > SetTC;
    typedef std::map<ULong, std::pair<ULong, TypeCode_ptr>, std::less<ULong> >
    MapPosTC;
    typedef std::map<const TypeCode *, ULong, std::less<const TypeCode *> >
    MapTCPos;

    TypeCode_ptr resolve_recursion() const;
    void strip();
    void connect (TypeCode_ptr, Long depth = 0);
    CORBA::ULong get_recurse_depth();
    static CORBA::Any *convert_case_label (CORBA::TypeCode_ptr disc,
					   const CORBA::Any &);
public:
    // begin-mico-extension
    void disconnect (TypeCode_ptr);
    TypeCode (const std::string &);
    TypeCode (); // for vector<TypeCode>
    ~TypeCode ();
    TypeCode (TCKind);
    TypeCode (const TypeCode &);
    TypeCode (DataDecoder &);
    TypeCode &operator= (const TypeCode &);
    Boolean from_string (const char *);

    static TypeCode_ptr create_basic_tc (TCKind);
    static TypeCode_ptr create_struct_tc (const char *rep_id, const char *name,
					  const StructMemberSeq &members);
    static TypeCode_ptr create_exception_tc (const char *rep_id,
					     const char *name,
					     const StructMemberSeq &members);
    static TypeCode_ptr create_union_tc (const char *rep_id, const char *name,
					 TypeCode_ptr disc_type,
					 const UnionMemberSeq &members);
    static TypeCode_ptr create_enum_tc (const char *rep_id, const char *name,
					const EnumMemberSeq &members);
    static TypeCode_ptr create_alias_tc (const char *rep_id, const char *name,
					 TypeCode_ptr orig_type);
    static TypeCode_ptr create_interface_tc (const char *rep_id,
					     const char *name);
    static TypeCode_ptr create_string_tc (ULong bound);
    static TypeCode_ptr create_wstring_tc (ULong bound);
    static TypeCode_ptr create_sequence_tc (ULong bound, TypeCode_ptr el_type);
    static TypeCode_ptr create_array_tc (ULong bound, TypeCode_ptr el_type);
    static TypeCode_ptr create_recursive_sequence_tc (ULong bound,
						      ULong offset);
    static TypeCode_ptr create_fixed_tc (UShort digits, Short scale);
    static TypeCode_ptr create_value_tc (const char *rep_id, const char *name,
					 ValueModifier mod,
					 TypeCode_ptr concrete_base,
					 const ValueMemberSeq &members);
    static TypeCode_ptr create_value_box_tc (const char *rep_id,
					     const char *name,
					     TypeCode_ptr boxed_type);
    static TypeCode_ptr create_native_tc (const char *rep_id,
					  const char *name);
    static TypeCode_ptr create_recursive_tc (const char *rep_id);
    static TypeCode_ptr create_abstract_interface_tc (const char *rep_id,
						      const char *name);
    static TypeCode_ptr create_local_interface_tc (const char *rep_id,
						   const char *name);
    // end-mico-extension

    typedef CORBA::TypeCode_Bounds Bounds;
    typedef CORBA::TypeCode_Bounds_catch Bounds_catch;

    typedef CORBA::TypeCode_BadKind BadKind;
    typedef CORBA::TypeCode_BadKind_catch BadKind_catch;

    TCKind kind () const;
    Boolean equal (TypeCode_ptr, Boolean remove_aliases = FALSE,
		                 Boolean ignore_string_bounds = FALSE) const;
    Boolean equaltype (TypeCode_ptr, SetTC * = 0);
    Boolean equivalent (TypeCode_ptr tc);

    const char *id () const;
    const char *name () const;

    Long member_index (const Any &);
    Long member_index (const char *);
    ULong member_count () const;
    const char *member_name (ULong idx) const;
    TypeCode_ptr member_type (ULong idx) const;
    Any *member_label (ULong idx) const;
    TypeCode_ptr discriminator_type () const;
    Long default_index () const;

    ULong length () const;
    
    TypeCode_ptr content_type () const;

    UShort fixed_digits () const;
    Short fixed_scale () const;

    Visibility member_visibility (ULong idx) const;
    ValueModifier type_modifier() const;
    TypeCode_ptr concrete_base_type() const;
    TypeCode_ptr get_compact_typecode() const;

    ULong member_count_inherited () const;
    const char *member_name_inherited (ULong idx) const;
    TypeCode_ptr member_type_inherited (ULong idx) const;
    Visibility member_visibility_inherited (ULong idx) const;

    Long param_count () const;
    Any *parameter (Long) const;

    // begin-mico-extension
    ULong array_size ();
    TypeCode_ptr array_type ();
    Boolean is_constant () const;
    TypeCode_ptr mk_constant ();
    TypeCode_ptr unalias ();

    void encode (DataEncoder &, MapTCPos * = NULL) const;
    Boolean decode (DataDecoder &, MapPosTC * = NULL, ULong level = 0);

    std::string stringify () const;

    Boolean is_recursive_seq ();
    Boolean is_freestanding();
    Boolean is_freestanding( ULong depth, TypeCode_ptr tc );


    Boolean is_base_type();
    Boolean is_char();
    Boolean is_wchar();
    Boolean is_octet();
    Boolean is_bool();
    Boolean is_enum();
    Boolean is_string();
    Boolean is_wstring();
    Boolean is_any();
    Boolean is_type_code();
    Boolean is_alias();
    Boolean is_variable();
    Boolean is_objref();
    Boolean is_object();
    Boolean is_sequence();
    Boolean is_array();
    Boolean is_valuetype();
    Boolean is_native();
    Boolean is_abstract_interface();
    // end-mico-extension

    static TypeCode_ptr _duplicate (TypeCode_ptr);
    static TypeCode_ptr _nil ()
    {
	return 0;
    }
};
extern void release (TypeCode_ptr);

//typedef ObjVar<TypeCode> TypeCode_var;


// begin-mico-extension
class LevelRecord {
public:
    enum Level {
	LNone, LArray, LSequence, LStruct, LExcept, LUnion, LValue, LValueBox
    };
private:
    TypeCode_ptr _tc;
    Level _level;
public:
    Long i, n, x, last_i;

    LevelRecord (Level, TypeCode_ptr, Long n = 0, Long i = 0, Long x = 0);
    LevelRecord (const LevelRecord &);
    LevelRecord ();
    ~LevelRecord ();
    LevelRecord &operator= (const LevelRecord &);
    Level level () const;
    TypeCode_ptr tc ();
};

class TypeCodeChecker {
    TypeCode_ptr _top;
    TypeCode_ptr _tc;
    Boolean done;
    std::vector<LevelRecord> levelvec;

    LevelRecord *level ();
    Boolean advance ();
    Boolean nexttc ();
    Boolean leave (LevelRecord::Level, Boolean sloppy = FALSE);
    void enter (const LevelRecord &);
    void tc (TypeCode_ptr);
    void override_tc (TypeCode_ptr);
public:
    TypeCodeChecker ();
    TypeCodeChecker (TypeCode_ptr);
    TypeCodeChecker (const TypeCodeChecker &);
    TypeCodeChecker &operator= (const TypeCodeChecker &);
    ~TypeCodeChecker ();

    TypeCode_ptr tc ();
    Boolean inside (TCKind);
    Boolean level_finished ();

    Boolean basic (TypeCode_ptr);
    Boolean enumeration (Long val);
    Boolean abstract_interface (TypeCode_ptr override);

    Boolean union_begin ();
    Boolean union_selection (Long idx = -1L);
    Boolean union_end (Boolean sloppy = FALSE);

    Boolean struct_begin ();
    Boolean struct_end (Boolean sloppy = FALSE);

    Boolean except_begin ();
    Boolean except_end (Boolean sloppy = FALSE);

    Boolean seq_begin (ULong len);
    Boolean seq_end (Boolean sloppy = FALSE);

    Boolean arr_begin ();
    Boolean arr_end (Boolean sloppy = FALSE);

    Boolean value_begin (TypeCode_ptr override = 0);
    Boolean value_end ();

    Boolean valuebox_begin ();
    Boolean valuebox_end ();

    void finish ();
    Boolean completed () const;
    void restart (TypeCode_ptr);
    void restart ();
    ULong level_count () const;
};
// end-mico-extension


extern MICO_EXPORT TypeCode_ptr _tc_null;
extern MICO_EXPORT TypeCode_ptr _tc_void;
extern MICO_EXPORT TypeCode_ptr _tc_short;
extern MICO_EXPORT TypeCode_ptr _tc_long;
extern MICO_EXPORT TypeCode_ptr _tc_longlong;
extern MICO_EXPORT TypeCode_ptr _tc_ushort;
extern MICO_EXPORT TypeCode_ptr _tc_ulong;
extern MICO_EXPORT TypeCode_ptr _tc_ulonglong;
extern MICO_EXPORT TypeCode_ptr _tc_float;
extern MICO_EXPORT TypeCode_ptr _tc_double;
extern MICO_EXPORT TypeCode_ptr _tc_longdouble;
extern MICO_EXPORT TypeCode_ptr _tc_boolean;
extern MICO_EXPORT TypeCode_ptr _tc_char;
extern MICO_EXPORT TypeCode_ptr _tc_wchar;
extern MICO_EXPORT TypeCode_ptr _tc_octet;
extern MICO_EXPORT TypeCode_ptr _tc_any;
extern MICO_EXPORT TypeCode_ptr _tc_TypeCode;
extern MICO_EXPORT TypeCode_ptr _tc_Principal;
extern MICO_EXPORT TypeCode_ptr _tc_Context; // extension
extern MICO_EXPORT TypeCode_ptr _tc_NamedValue; // extension
extern MICO_EXPORT TypeCode_ptr _tc_Object;
extern MICO_EXPORT TypeCode_ptr _tc_ValueBase;
extern MICO_EXPORT TypeCode_ptr _tc_AbstractBase; // extension
extern MICO_EXPORT TypeCode_ptr _tc_string;
extern MICO_EXPORT TypeCode_ptr _tc_wstring;

}

#endif // __mico_typecode_h__
