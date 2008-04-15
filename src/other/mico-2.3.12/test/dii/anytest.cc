#include <CORBA.h>
#include <ctype.h>
#if !defined(__sgi) || !defined(__GNUG__)
// conflict for initstate()
#include <math.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <string>
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <string.h>
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <mico/util.h>
#include <mico/template_impl.h>
//#include "subtype.h"


using namespace std;

inline int
check (int expr, const char *msg)
{
    if (!expr) {
	cout << msg << ": failed" << endl;
	return 1;
    }
    return 0;
}

const char *
extract (CORBA::Any &a, char *types, int cond = 1, const char *val = "")
{
    for ( ; *types; ++types) switch (*types) {
    case 's': {
	CORBA::Short o;
	if ((a >>= o) != cond || (cond && o != (CORBA::Short)atol (val)))
	    return "Short";
	break;
    }
    case 'S': {
	CORBA::UShort o;
	if ((a >>= o) != cond || (cond && o != (CORBA::UShort)atol (val)))
	    return "UShort";
	break;
    }
    case 'l': {
	CORBA::Long o;
	if ((a >>= o) != cond || (cond && o != (CORBA::Long)atol (val)))
	    return "Long";
	break;
    }
    case 'L': {
	CORBA::ULong o;
	if ((a >>= o) != cond || (cond && o != (CORBA::ULong)atol (val)))
	    return "ULong";
	break;
    }
    case 'x': {
	CORBA::LongLong o;
	if ((a >>= o) != cond || (cond && o != (CORBA::LongLong)atol (val)))
	    return "LongLong";
	break;
    }
    case 'X': {
	CORBA::ULongLong o;
	if ((a >>= o) != cond || (cond && o != (CORBA::ULongLong)atol (val)))
	    return "ULongLong";
	break;
    }
    case 'f': {
	CORBA::Float o;
	CORBA::Float exp = atof (val);
	
	if ((a >>= o) != cond || (cond && o != exp))
	    return "Float";
	break;
    }
    case 'd': {
	CORBA::Double o;
	CORBA::Double exp = atof (val);
	if ((a >>= o) != cond || (cond && o != exp))
	    return "Double";
	break;
    }
    case 'D': {
	CORBA::LongDouble o;
	CORBA::LongDouble exp = atof (val);
	if ((a >>= o) != cond || (cond && o != exp))
	    return "LongDouble";
	break;
    }
    case 'o': {
	CORBA::Octet o;
	if ((a >>= CORBA::Any::to_octet (o)) != cond ||
	    (cond && o != (CORBA::Octet)atol (val)))
	    return "Octet";
	break;
    }
    case 'b': {
	CORBA::Boolean o;
	if ((a >>= CORBA::Any::to_boolean (o)) != cond ||
	    (cond && o != (CORBA::Boolean)atol (val)))
	    return "Boolean";
	break;
    }
    case 'c': {
	CORBA::Char o;
	if ((a >>= CORBA::Any::to_char (o)) != cond ||
	    (cond && o != (CORBA::Char)atol (val)))
	    return "Char";
	break;
    }
    case 'w': {
	CORBA::WChar o;
	if ((a >>= CORBA::Any::to_wchar (o)) != cond ||
	    (cond && o != (CORBA::WChar)atol (val)))
	    return "WChar";
	break;
    }
    case 't': {
	const char *o = 0;
	int ret = ((a >>= o) != cond || (cond && strcmp (o, val)));
	if (ret)
	    return "String";
	break;
    }
    case 'T': {
	assert (isdigit (types[1]));
	CORBA::ULong len = strtol (types+1, &types, 10);
	--types;
	const char *o = 0;
	int ret = ((a >>= CORBA::Any::to_string (o, len)) != cond ||
		   (cond && strcmp (o, val)));
	if (ret)
	    return "BoundedString";
	break;
    }
    case 'u': {
        const CORBA::WChar *o = 0;
	int ret = ((a >>= o) != cond ||
		   (cond && xwcscmp (o, (CORBA::WChar *)val)));
	if (ret)
	    return "WString";
	break;
    }
    case 'U': {
	assert (isdigit (types[1]));
	CORBA::ULong len = strtol (types+1, &types, 10);
	--types;
	const CORBA::WChar *o = 0;
	int ret = ((a >>= CORBA::Any::to_wstring (o, len)) != cond ||
		   (cond && xwcscmp (o, (CORBA::WChar *)val)));
	if (ret)
	    return "BoundedWString";
	break;
    }
    default:
	assert (0);
    }
    return 0;
}

/****************************** Basic Types *******************************/

int
test_basic ()
{
    int res = 0;

    CORBA::Any a;

    CORBA::Short s = 1234;
    a <<= s;
    res += check (!extract (a, "s", 1, "1234") &&
		  !extract (a, "s", 1, "1234") &&
		  !extract (a, "SLXlxfdDcwobtu", 0),
		  "Short");

    CORBA::UShort us = 4321;
    a <<= us;
    res += check (!extract (a, "S", 1, "4321") &&
		  !extract (a, "S", 1, "4321") &&
		  !extract (a, "scwobtulLxXfdD", 0),
		  "UShort");

    CORBA::Long l = 12345678;
    a <<= l;
    res += check (!extract (a, "l", 1, "12345678") &&
		  !extract (a, "l", 1, "12345678") &&
		  !extract (a, "sSLXfcwobtuxdD", 0),
		  "Long");

    CORBA::LongLong ll = 12345678;
    a <<= ll;
    res += check (!extract (a, "x", 1, "12345678") &&
		  !extract (a, "x", 1, "12345678") &&
		  !extract (a, "sSlLXfcwobtuD", 0),
		  "LongLong");

    CORBA::ULong ul = 87654321;
    a <<= ul;
    res += check (!extract (a, "L", 1, "87654321") &&
		  !extract (a, "L", 1, "87654321") &&
		  !extract (a, "sSlfcwobtuxXdD", 0),
		  "ULong");

    CORBA::ULongLong ull = 87654321;
    a <<= ull;
    res += check (!extract (a, "X", 1, "87654321") &&
		  !extract (a, "X", 1, "87654321") &&
		  !extract (a, "sSlLfdcwobtuD", 0),
		  "ULongLong");

    CORBA::Float f = 8.125;
    a <<= f;
    res += check (!extract (a, "f", 1, "8.125") &&
		  !extract (a, "f", 1, "8.125") &&
		  !extract (a, "sSlLxXcwobtudD", 0),
		  "Float");

    CORBA::Double d = 16.0625;
    a <<= d;
    res += check (!extract (a, "d", 1, "16.0625") &&
		  !extract (a, "d", 1, "16.0625") &&
		  !extract (a, "sSxXlLfcwobtuD", 0),
		  "Double");

    CORBA::LongDouble dd = 16.0625;
    a <<= dd;
    res += check (!extract (a, "D", 1, "16.0625") &&
		  !extract (a, "D", 1, "16.0625") &&
		  !extract (a, "sSxXlLfdcwobtu", 0),
		  "LongDouble");

    CORBA::Char c = 21;
    a <<= CORBA::Any::from_char(c);
    res += check (!extract (a, "c", 1, "21") &&
		  !extract (a, "c", 1, "21") &&
		  !extract (a, "sSlLfdobt", 0),
		  "Char");

    CORBA::WChar w = 21;
    a <<= CORBA::Any::from_wchar(w);
    res += check (!extract (a, "w", 1, "21") &&
		  !extract (a, "w", 1, "21") &&
		  !extract (a, "sSlLxXfdDocbtu", 0),
		  "WChar");

    CORBA::Octet o = 42;
    a <<= CORBA::Any::from_octet(o);
    res += check (!extract (a, "o", 1, "42") &&
		  !extract (a, "o", 1, "42") &&
		  !extract (a, "sSlLfdcbt", 0),
		  "Octet");

    CORBA::Boolean b = 1;
    a <<= CORBA::Any::from_boolean(b);
    res += check (!extract (a, "b", 1, "1") &&
		  !extract (a, "b", 1, "1") &&
		  !extract (a, "sSlLfdcot", 0),
		  "Boolean");
    return res;
}

/******************************** Fixed ********************************/

void
operator<<= (CORBA::Any &a, const FixedTmpl<4,2> &f)
{
    a <<= CORBA::Any::from_fixed (f, 4, 2);
}

CORBA::Boolean
operator>>= (const CORBA::Any &a, FixedTmpl<4,2> &f)
{
    if (!(a >>= CORBA::Any::to_fixed (f, 4, 2)))
	return FALSE;
    return TRUE;
}

int
test_fixed ()
{
    FixedTmpl<4,2> f1, f2 = (CORBA::LongDouble)42.42;
    CORBA::Any a;

    a <<= f2;
    if (!(a >>= f1))
	return 1;
    return (!(f1 == f2));
}

/******************************** String ********************************/

int
test_string ()
{
    int res = 0;
    CORBA::Any a;

    char *i = "foobarbaz";
    a <<= i;
    res += check (!extract (a, "t", 1, i) &&
		  !extract (a, "T0", 1, i) &&
		  !extract (a, "sSlLfdcob", 0),
		  "unbounded string 1");

    i = "foobazbar";
    a <<= CORBA::Any::from_string(i,0);
    res += check (!extract (a, "T0", 1, i) &&
		  !extract (a, "t", 1, i) &&
		  !extract (a, "sSlLfdcob", 0),
		  "unbounded string 2");

    i = "barfoobaz";
    a <<= CORBA::Any::from_string(i,20);
    res += check (!extract (a, "T20", 1, i) &&
		  !extract (a, "T20", 1, i) &&
		  !extract (a, "sSlLfdcobtT10", 0),
		  "bounded string");
    return res;
}

int
test_wstring ()
{
    int res = 0;
    CORBA::Any a;

    CORBA::WChar *i = L"AB";

    a <<= i;
    res += check (!extract (a, "u", 1, (char *)i) &&
		  !extract (a, "U0", 1, (char *)i) &&
		  !extract (a, "sSlLxXfdDcwobt", 0),
		  "unbounded wstring 1");

    a <<= CORBA::Any::from_wstring(i,0);
    res += check (!extract (a, "U0", 1, (char *)i) &&
		  !extract (a, "u", 1, (char *)i) &&
		  !extract (a, "sSlLxXfdDcwobt", 0),
		  "unbounded wstring 2");

    a <<= CORBA::Any::from_wstring(i,20);
    res += check (!extract (a, "U20", 1, (char *)i) &&
		  !extract (a, "U20", 1, (char *)i) &&
		  !extract (a, "sSlLxXfdDcowbtuU10", 0),
		  "bounded wstring");
    return res;
}

/******************************* Sequence ********************************/

typedef SequenceTmpl<CORBA::Long,MICO_TID_DEF> LongSeq;

CORBA::TypeCode_ptr _tc_LongSeq;

#if 0
void
operator<<= (CORBA::Any &a, const LongSeq &seq)
{
    a.set_type (_tc_LongSeq);
    a.seq_put_begin (seq.length());
    for (int i = 0; i < seq.length(); ++i)
        a <<= seq[i];
    a.seq_put_end();
}

CORBA::Boolean
operator>>= (CORBA::Any &a, LongSeq &seq)
{
    CORBA::ULong len;
    if (!a.seq_get_begin (len))
        return FALSE;
    seq.length (len);
    for (int i = 0; i < len; ++i)
        if (!(a >>= seq[i]))
            return FALSE;
    return a.seq_get_end();
}
#else
void operator<<= (CORBA::Any &a, const LongSeq &);
CORBA::Boolean operator>>= (const CORBA::Any &a, LongSeq &);
#endif

CORBA::Boolean
operator== (const LongSeq &s1, const LongSeq &s2)
{
    CORBA::ULong l = s1.length();
    if (l != s2.length())
        return FALSE;
    for (CORBA::ULong i = 0; i < l; ++i)
        if (s1[i] != s2[i])
            return FALSE;
    return TRUE;
}

int
test_seq ()
{
    _tc_LongSeq = CORBA::TypeCode::create_sequence_tc (
	0, CORBA::_tc_long)->mk_constant();

    CORBA::Any a;
    LongSeq s1, s2, s3;

    s1.length (10);
    for (int i = 0; i < 10; ++i)
        s1[i] = i;

    a <<= s1;
    return check ((a >>= s2) && s1 == s2 &&
                  (a >>= s3) && s1 == s3,
                  "sequence");
}

/******************************* Array ********************************/

CORBA::TypeCode_ptr _tc_LongArray;
CORBA::TypeCode_ptr _tc_LongArrayArray;

void operator<<= (CORBA::Any &a, const CORBA::Long arr[2][3])
{
    a.set_type (_tc_LongArrayArray);
    a.array_put_begin();
    for (int i = 0; i < 2; ++i) {
        a.array_put_begin();
        for (int j = 0; j < 3; ++j) {
            a <<= arr[i][j];
        }
        a.array_put_end();
    }
    a.array_put_end();
}

CORBA::Boolean operator>>= (CORBA::Any &a, CORBA::Long arr[2][3])
{
    if (!a.array_get_begin())
	return FALSE;
    for (int i = 0; i < 2; ++i) {
        if (!a.array_get_begin())
	    return FALSE;
        for (int j = 0; j < 3; ++j) {
            if (!(a >>= arr[i][j]))
		return FALSE;
        }
        if (!a.array_get_end())
	    return FALSE;
    }
    return a.array_get_end();
}

int test_array ()
{
    _tc_LongArray = CORBA::TypeCode::create_array_tc (
	3, CORBA::_tc_long)->mk_constant();
    _tc_LongArrayArray = CORBA::TypeCode::create_array_tc (
	2, _tc_LongArray)->mk_constant();

    CORBA::Long arr1[2][3], arr2[2][3];

    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 3; ++j)
            arr1[i][j] = i*3+j;

    CORBA::Any a1, a2;
    a1 <<= arr1;
    a2.replace (_tc_LongArrayArray, (void *)a1.value());
    if (!(a2 >>= arr2))
	return 1;

    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 3; ++j)
            if (!(arr1[i][j] == arr2[i][j]))
		return FALSE;

    string s = _tc_LongArrayArray->stringify();
    CORBA::TypeCode t (s.c_str());
    return 0;
}

/******************************* Struct ********************************/

struct SomeStruct {
    CORBA::Long l;
    LongSeq s;
};

CORBA::TypeCode_ptr _tc_SomeStruct;

struct __mk_somestruct_typecode {
    __mk_somestruct_typecode ()
    {
        CORBA::StructMember sm1;
        sm1.name = (const char *)"l";
        sm1.type = CORBA::_tc_long;
        sm1.type_def = 0;

        CORBA::StructMember sm2;
        sm2.name = (const char *)"s";
        sm2.type = _tc_LongSeq;
        sm2.type_def = 0;
        
        CORBA::StructMemberSeq ss;
        ss.length (2);
        ss[0] = sm1;
        ss[1] = sm2;

        _tc_SomeStruct = CORBA::TypeCode::create_struct_tc ("repoid",
							    "SomeStruct",
							    ss)->mk_constant();
    }
};

void
operator<<= (CORBA::Any &a, const SomeStruct &s)
{
    a.set_type (_tc_SomeStruct);
    a.struct_put_begin ();
    a <<= s.l;
    a <<= s.s;
    a.struct_put_end ();
}

CORBA::Boolean
operator>>= (const CORBA::Any &a, SomeStruct &s)
{
    return (a.struct_get_begin () &&
            (a >>= s.l) &&
            (a >>= s.s) &&
            a.struct_get_end ());
}

CORBA::Boolean
operator== (const SomeStruct &s1, const SomeStruct &s2)
{
    return s1.l == s2.l && s1.s == s2.s;
}

int
test_struct ()
{
    __mk_somestruct_typecode _mk_somestruct_typecode;
    (void)_mk_somestruct_typecode;

    CORBA::Any a;
    SomeStruct s1, s2, s3, s4;

    s1.l = 1234;
    s1.s.length (10);
    for (int i = 0; i < 10; ++i)
        s1.s[i] = i;

    a <<= s1;
    if (check ((a >>= s2) && s1 == s2 &&
               (a >>= s3) && s1 == s3,
               "struct 1"))
        return 1;

    CORBA::Any a2 (a.type(), (void *)a.value());
    int res = check (a == a2 && (a2 >>= s4) && s1 == s4, "struct 2");

    CORBA::Any a3;
    const CORBA::Any *a4;
    a3 <<= a;
    res += check ((a3 >>= a4) && (a == *a4), "struct 3");
    return res;
}

/********************************* Union *********************************/

/*
 * #define EXPLICIT
 * 
 * union SomeUnion switch (long) {
 * case 1:  long l;
 * case 2:  short s;
 * default: boolean b;
 * };
 *
 * #undef EXPLICIT
 * 
 * union SomeUnion switch (long) {
 * case 1:  long l;
 * case 2:  short s;
 * };
 */

#define EXPLICIT

class SomeUnion {
    CORBA::Long _l;
    CORBA::Short _s;
#ifdef EXPLICIT
    CORBA::Boolean _b;
#endif
    CORBA::Long _disc;
public:
    void _d (CORBA::Long d)
    {
        _disc = d;
    }
    CORBA::Long _d () const
    {
        return _disc;
    }

#ifndef EXPLICIT
    void _default ()
    {
	_disc = 3;
    }
#endif

    void l (CORBA::Long l)
    {
        _d (1);
        _l = l;
    }
    CORBA::Long l () const
    {
        assert (_d() == 1);
        return _l;
    }

    void s (CORBA::Long s)
    {
        _d (2);
        _s = s;
    }
    CORBA::Short s () const
    {
        assert (_d() == 2);
        return _s;
    }

#ifdef EXPLICIT
    void b (CORBA::Boolean b)
    {
        _d (3);
        _b = b;
    }
    CORBA::Boolean b () const
    {
	assert (_d() > 2);
        return _b;
    }
#endif
};

CORBA::TypeCode_ptr _tc_SomeUnion;

struct __mk_someunion_typecode {
    __mk_someunion_typecode ()
    {
        CORBA::UnionMember um1;
        um1.name = (const char *)"l";
        um1.type = CORBA::_tc_long;
        um1.label <<= (CORBA::Long)1;
        um1.type_def = 0;

        CORBA::UnionMember um2;
        um2.name = (const char *)"s";
        um2.type = CORBA::_tc_short;
        um2.label <<= (CORBA::Long)2;
        um2.type_def = 0;

#ifdef EXPLICIT
        CORBA::UnionMember um3;
        um3.name = (const char *)"b";
        um3.type = CORBA::_tc_boolean;
        um3.label <<= CORBA::Any::from_octet (0);
        um3.type_def = 0;
#endif

        CORBA::UnionMemberSeq us;
        us.length (2);
        us[0] = um1;
        us[1] = um2;
#ifdef EXPLICIT
	us.length (3);
        us[2] = um3;
#endif

        _tc_SomeUnion
            = CORBA::TypeCode::create_union_tc ("repoid", "SomeUnion",
						CORBA::_tc_long,
						us)->mk_constant();
    }
};

void
operator<<= (CORBA::Any &a, const SomeUnion &u)
{
    a.set_type (_tc_SomeUnion);
    a.union_put_begin();
    a <<= u._d();
    switch (u._d()) {
    case 1:
        /*
         * the argument to union_*_selection() is the index of the
         * corresponding entry in the TypeCode. do not call for
	 * implicit default case.
         */
        a.union_put_selection (0);
        a <<= u.l();
        break;

    case 2:
        a.union_put_selection (1);
        a <<= u.s();
        break;
#ifdef EXPLICIT
    default:
	// explicit default
        a.union_put_selection (2);
        a <<= CORBA::Any::from_boolean (u.b());
        break;
#else
    default:
	// implicit default
	break;
#endif
    }
    a.union_put_end();
}

CORBA::Boolean
operator>>= (const CORBA::Any &a, SomeUnion &u)
{
    if (!a.union_get_begin())
	return FALSE;
    CORBA::Long d;
    if (!(a >>= d))
	return FALSE;
    switch (d) {
    case 1: {
        if (!a.union_get_selection (0))
	    return FALSE;
        CORBA::Long l;
        if (!(a >>= l))
	    return FALSE;
        u.l (l);
        u._d (1); // in case multiple labels for one value
        break;
    }
    case 2: {
        if (!a.union_get_selection (1))
	    return FALSE;
        CORBA::Short s;
        if (!(a >>= s))
	    return FALSE;
        u.s (s);
        u._d (2); // in case multiple labels for one value
        break;
    }
#ifdef EXPLICIT
    default: {
	// explicit default
        if (!(a.union_get_selection (2)))
	    return FALSE;
        CORBA::Boolean b;
        if (!(a >>= CORBA::Any::to_boolean (b)))
	    return FALSE;
        u.b (b);
        u._d (d);
        break;
    }
#else
    default:
	// implicit default
	u._default ();
	u._d (d);
	break;
#endif
    }
    return a.union_get_end();
}

int
test_union ()
{
    __mk_someunion_typecode _mk_someunion_typecode;
    (void)_mk_someunion_typecode;
     
    CORBA::Any a1, a2;
    SomeUnion u1, u2;

    u1.l (10);
    a1 <<= u1;
    a2.replace (_tc_SomeUnion, (void *)a1.value());
    if (!(a2 >>= u2))
	return 1;

    if (!(u1._d() == u2._d()))
	return 1;
    if (!(u1.l() == u2.l()))
	return 1;

    
    u1.s (5);
    a1 <<= u1;
    a2.replace (_tc_SomeUnion, (void *)a1.value());
    if (!(a2 >>= u2))
	return 1;

    if (!(u1._d() == u2._d()))
	return 1;
    if (!(u1.s() == u2.s()))
	return 1;


#ifdef EXPLICIT
    u1.b (TRUE);
    a1 <<= u1;
    a2.replace (_tc_SomeUnion, (void *)a1.value());
    if (!(a2 >>= u2))
	return 1;

    if (!(u1._d() == u2._d()))
	return 1;
    if (!(u1.b() == u2.b()))
	return 1;
#else
    // implicit default
    u1._default();
    a1 <<= u1;
    a2.replace (_tc_SomeUnion, (void *)a1.value());
    if (!(a2 >>= u2))
	return 1;

    if (!(u1._d() == u2._d()))
	return 1;
#endif
    
    string s = _tc_SomeUnion->stringify();
    CORBA::TypeCode t (s.c_str());

    return 0;
}

/*************************** RecursiveSequence ***************************/

/*
 * struct SomeRecStruct {
 *     long l;
 *     sequence<SomeRecStruct> childs;
 * };
 */

struct SomeRecStruct {
    CORBA::Long l;
    SequenceIndTmpl<SomeRecStruct, SomeRecStruct, 1> childs;

    SomeRecStruct ()
    {
    }

    SomeRecStruct (const SomeRecStruct &s)
      : l (s.l), childs (s.childs)
    {
    }

    ~SomeRecStruct ()
    {
    }

    SomeRecStruct &operator= (const SomeRecStruct &s)
    {
      l = s.l;
      childs = s.childs;
      return *this;
    }
};

typedef SequenceIndTmpl<SomeRecStruct, SomeRecStruct, 1> SomeRecStructSeq;


CORBA::TypeCode_ptr _tc_SomeRecStruct;

struct __mk_somerecstruct_typecode {
    __mk_somerecstruct_typecode ()
    {
        CORBA::StructMember sm1;
        sm1.name = (const char *)"l";
        sm1.type = CORBA::_tc_long;
        sm1.type_def = 0;

        CORBA::StructMember sm2;
        sm2.name = (const char *)"childs";
	/*
	 * first argument is the sequence bound.
	 * second arg is the recursion depth: a value of 0 would result
	 * in a sequence of itself, 1 results in a sequence of its
	 * enclosing type and so on.
	 */
        sm2.type = CORBA::TypeCode::create_recursive_sequence_tc (0, 1);
        sm2.type_def = 0;
        
        CORBA::StructMemberSeq ss;
        ss.length (2);
        ss[0] = sm1;
        ss[1] = sm2;

        _tc_SomeRecStruct
	    = CORBA::TypeCode::create_struct_tc ("repoid",
						 "SomeRecStruct",
						 ss)->mk_constant();
    }
} __mk_somerecstruct_tc;

void operator<<= (CORBA::Any &a, const SomeRecStruct &s);
CORBA::Boolean operator>>= (const CORBA::Any &a, SomeRecStruct &s);

void
operator<<= (CORBA::Any &a, const SomeRecStructSeq &s)
{
    static CORBA::TypeCode_ptr _tc =
	CORBA::TypeCode::create_sequence_tc (0,
					     _tc_SomeRecStruct)->mk_constant();

    a.set_type (_tc);
    a.seq_put_begin (s.length());
    for (CORBA::ULong i = 0; i < s.length(); ++i) {
	a <<= s[i];
    }
    a.seq_put_end ();
}

CORBA::Boolean
operator>>= (const CORBA::Any &a, SomeRecStructSeq &s)
{
    CORBA::ULong len;
    if (!a.seq_get_begin (len))
	return FALSE;
    s.length (len);
    for (CORBA::ULong i = 0; i < len; ++i) {
        if (!(a >>= s[i]))
	    return FALSE;
    }
    return a.seq_get_end();
}

void
operator<<= (CORBA::Any &a, const SomeRecStruct &s)
{
    a.set_type (_tc_SomeRecStruct);
    a.struct_put_begin ();
    a <<= s.l;
    a <<= s.childs;
    a.struct_put_end ();
}

CORBA::Boolean
operator>>= (const CORBA::Any &a, SomeRecStruct &s)
{
    return (a.struct_get_begin () &&
	    (a >>= s.l) &&
	    (a >>= s.childs) &&
	    a.struct_get_end ());
}

CORBA::Boolean
operator== (const SomeRecStruct &s1, const SomeRecStruct &s2)
{
    if (s1.l != s2.l || s1.childs.length() != s2.childs.length())
        return FALSE;
    for (CORBA::ULong i = 0; i < s1.childs.length(); ++i) {
        if (!(s1.childs[i] == s2.childs[i]))
            return FALSE;
    }
    return TRUE;
}

int
test_recursive ()
{
    __mk_somerecstruct_typecode _mk_somerecstruct_typecode;
    (void)_mk_somerecstruct_typecode;

    CORBA::Any a;
    CORBA::TypeCode_ptr tc;

    a <<= _tc_SomeRecStruct;
    if (!(a >>= tc))
	return 1;
    if (!(tc->equal (_tc_SomeRecStruct)))
	return 1;

    SomeRecStruct s1, s11, s12, s13, s111, s112;
    SomeRecStruct s2;

    s1.l = 1;
    s11.l = 11;
    s12.l = 12;
    s13.l = 13;
    s111.l = 111;
    s112.l = 112;

    s1.childs.length (3);
    s1.childs[0] = s11;
    s1.childs[1] = s12;
    s1.childs[2] = s13;

    s11.childs.length (2);
    s11.childs[0] = s111;
    s11.childs[1] = s112;

    a <<= s1;
    if (!(a >>= s2))
	return 1;

    if (!(s1 == s2))
	return 1;

    return 0;
}

/*************************** Subtyping ***********************************/

#if 0
int
test_subtyping()
{
  CORBA::Any a;
  T1 t1;
  T2 t2;
  
  t1.x = 1;
  t1.z.y = 2;
  t1.z.next.length( 1 );
  t1.z.next[ 0 ].x = 3;
  t1.z.next[ 0 ].z.y = 4;

  a <<= t1;
  if (!( a >>= t2 ))
      return 1;

  if (!( t2.x == 1 ))
      return 1;
  if (!( t2.z.y == 2 ))
      return 1;
  if (!( t2.z.next[ 0 ].x == 3 ))
      return 1;
  if (!( t2.z.next[ 0 ].z.y == 4 ))
      return 1;
  if (!( t2.z.next[ 0 ].z.next.length() == 0 ))
      return 1;

  return 0;
}
#endif


/******************************* TypeCode ********************************/

int
test_typecode ()
{
    CORBA::Any a;

    CORBA::TypeCode_ptr i = _tc_SomeStruct;
    CORBA::TypeCode_ptr o = CORBA::_tc_null;

    a <<= i;
    int ret = check ((a >>= o) && i->equal (o),
                     "typecode 1");

    string strtc = i->stringify();
    CORBA::TypeCode tmp (strtc);
    check (i->equal (&tmp), "typecode 2");
    return ret;
}

/********************************* Enum **********************************/

enum SomeEnum {
    Red, Green, Blue
};

CORBA::TypeCode_ptr _tc_SomeEnum;

struct __mk_someenum_typecode {
    __mk_someenum_typecode ()
    {
        CORBA::EnumMemberSeq es;
        es.length (3);
        es[0] = (const char *)"Red";
        es[1] = (const char *)"Green";
        es[2] = (const char *)"Blue";

        _tc_SomeEnum =
            CORBA::TypeCode::create_enum_tc ("repoid",
					     "SomeEnum", es)->mk_constant();
    }
};

void
operator<<= (CORBA::Any &a, const SomeEnum &en)
{
    a.set_type (_tc_SomeEnum);
    a.enum_put ((CORBA::ULong)en);
}

CORBA::Boolean
operator>>= (const CORBA::Any &a, SomeEnum &en)
{
    CORBA::ULong ul;
    if (!a.enum_get (ul))
        return FALSE;
    en = (SomeEnum)ul;
    return TRUE;
}

int
test_enum()
{
    __mk_someenum_typecode _mk_someenum_typecode;
    (void)_mk_someenum_typecode;

    CORBA::Any a;
    SomeEnum e1, e2, e3;

    e1 = Green;
    a <<= e1;
    return (check ((a >>= e2) && e1 == e2 &&
                   (a >>= e3) && e1 == e3,
                   "enum"));
}

/******************************* Object **********************************/

CORBA::TypeCode_ptr _tc_SomeInterface;


class SomeInterface;
typedef SomeInterface *SomeInterface_ptr;

class SomeInterface : public CORBA::Object {
public:
    static SomeInterface_ptr narrow (CORBA::Object_ptr obj)
    {
        SomeInterface_ptr o = new SomeInterface;
        o->CORBA::Object::operator= (*obj);
        return o;
    }

    void foo () {}
};

void
operator<<= (CORBA::Any &a, SomeInterface_ptr obj)
{
    a <<= CORBA::Any::from_object (obj, "SomeInterface");
}

CORBA::Boolean
operator>>= (const CORBA::Any &a, SomeInterface_ptr &obj)
{
    // XXX must *not* return a copy
    CORBA::Object_var o;
    if (!(a >>= CORBA::Any::to_object (o)))
        return FALSE;
    obj = SomeInterface::narrow (o);
    return TRUE;
}

int
test_interface ()
{
    _tc_SomeInterface = CORBA::TypeCode::create_interface_tc (
	"SomeInterface", "repo-id")->mk_constant();

    // XXX have no object to do test with ...
    return 0;
}

/***************************** ValueType *********************************/

/*
 * valuetype Node {
 * public Long l;
 * };
 *
 * valuetype LongList truncatable Node {
 * public LongList next;
 * };
 */

CORBA::TypeCode_ptr _tc_Node;
CORBA::TypeCode_ptr _tc_LongList;

void __mk_valuetype_typecodes ()
{
    CORBA::ValueMemberSeq vms;
    vms.length (1);
    vms[0].name = (const char *)"next";
    vms[0].id = (const char *)"";
    vms[0].type = CORBA::TypeCode::_duplicate (CORBA::_tc_long);
    vms[0].access = CORBA::PUBLIC_MEMBER;

    _tc_Node = CORBA::TypeCode::create_value_tc ("IDL:Node:1.0",
						 "Node",
						 CORBA::VM_NONE,
						 CORBA::TypeCode::_nil(),
						 vms);

    CORBA::TypeCode_var next_tc =
	CORBA::TypeCode::create_recursive_tc ("IDL:LongList:1.0");

    vms[0].name = (const char *)"l";
    vms[0].id = (const char *)"";
    vms[0].type = next_tc;
    vms[0].access = CORBA::PUBLIC_MEMBER;

    _tc_LongList = CORBA::TypeCode::create_value_tc ("IDL:LongList:1.0",
						     "LongList",
						     CORBA::VM_TRUNCATABLE,
						     _tc_Node,
						     vms);
}


typedef map<CORBA::ValueBase*, CORBA::Long, less<CORBA::ValueBase*> > MapVBId;
typedef map<CORBA::Long, CORBA::ValueBase*, less<CORBA::Long> > MapIdVB;


class Node : virtual public CORBA::ValueBase {
public:
    virtual CORBA::Long l () const = 0;
    virtual void l (CORBA::Long n) = 0;

    static Node *_downcast (CORBA::ValueBase *v)
    { return (Node *)v->_narrow_helper(""); }

    virtual void *_narrow_helper (const char *)
    { return (void *)this; }

    CORBA::ValueDef * get_value_def()
    { return 0; }
};

class Node_impl : virtual public Node,
		  virtual public CORBA::DefaultValueRefCountBase {
    CORBA::Long _l;
public:
    virtual CORBA::Long l () const { return _l; }
    virtual void l (CORBA::Long n) { _l = n; }

    CORBA::ValueBase *_copy_value ()
    { return new Node_impl (*this); }
};

typedef ValueVar<Node> Node_var;

class Node_init : virtual public CORBA::ValueFactoryBase {
public:
    CORBA::ValueBase *create_for_unmarshal ()
    { return new Node_impl; }
    CORBA::TypeCode_ptr type ()
    { return CORBA::TypeCode::_duplicate (_tc_Node); }
};


void
operator<<= (CORBA::Any &_a, Node *_v)
{
    CORBA::Long _id;

    _a.set_type (_tc_Node);
    _a.value_put_begin (_id);
    _a <<= _v->l();
    _a.value_put_end (_id);
}


CORBA::Boolean
__any_extract (const CORBA::Any &_a, Node *&_v, MapIdVB &_m)
{
}

CORBA::Boolean
operator>>= (const CORBA::Any &_a, Node *&_v)
{
    // XXX must *not* return a copy ...
    CORBA::Long _id;
    CORBA::Boolean _is_ref;

    if (!_a.value_get_begin (_id, _is_ref))
	return FALSE;
    assert (!_is_ref);

    CORBA::ORB_var _orb = CORBA::ORB_instance ("mico-local-orb");
    CORBA::ValueFactoryBase_var _vf =
	_orb->lookup_value_factory ("IDL:Node:1.0");
    assert (_vf.in());

    CORBA::ValueBase *_vb = _vf->create_for_unmarshal();
    _v = Node::_downcast (_vb);

    CORBA::Long _l;
    if (!(_a >>= _l) || !_a.value_get_end (_id, _is_ref))
	return FALSE;
    _v->l (_l);

    return TRUE;
}




class LongList : virtual public Node {
public:
    virtual LongList *next () const = 0;
    virtual void next (LongList *n) = 0;

    static LongList *_downcast (CORBA::ValueBase *v)
    { return (LongList *)v->_narrow_helper(""); }

    virtual void *_narrow_helper (const char *)
    { return (void *)this; }

    CORBA::ValueDef * get_value_def()
    { return 0; }
};

typedef ValueVar<LongList> LongList_var;

class LongList_impl : virtual public LongList,
		      virtual public CORBA::DefaultValueRefCountBase {
    LongList *_next;
    CORBA::Long _l;
public:
    virtual LongList *next () const { return _next; }
    virtual void next (LongList *n) { _next = n; }

    virtual CORBA::Long l () const { return _l; }
    virtual void l (CORBA::Long n) { _l = n; }

    CORBA::ValueBase *_copy_value ()
    { return new LongList_impl (*this); }
};

class LongList_init : virtual public CORBA::ValueFactoryBase {
public:
    CORBA::ValueBase *create_for_unmarshal ()
    { return new LongList_impl; }
    CORBA::TypeCode_ptr type ()
    { return CORBA::TypeCode::_duplicate (_tc_LongList); }
};



void
__any_insert (CORBA::Any &_a, LongList *_v, MapVBId &_m)
{
    CORBA::Long _id;

    _a.set_type (_tc_LongList);

    if (!_v || _m.count (_v)) {
	_a.value_put_ref (_m[_v]);
	return;
    }

    _a.value_put_begin (_id);
    _m[_v] = _id;
    _a <<= _v->l();
    __any_insert (_a, _v->next(), _m);
    _a.value_put_end (_id);
}

void
operator<<= (CORBA::Any &_a, LongList *_v)
{
    MapVBId _m;
    __any_insert (_a, _v, _m);
}


CORBA::Boolean
__any_extract (const CORBA::Any &_a, LongList *&_v, MapIdVB &_m)
{
    // XXX must *not* return a copy ...
    CORBA::Long _id;
    CORBA::Boolean _is_ref;

    if (!_a.value_get_begin (_id, _is_ref))
	return FALSE;
    if (_is_ref) {
	if (!_a.value_get_end (_id, _is_ref))
	    return FALSE;
	_v = _id ? LongList::_downcast (_m[_id]) : 0;
	return TRUE;
    }

    CORBA::ORB_var _orb = CORBA::ORB_instance ("mico-local-orb");
    CORBA::ValueFactoryBase_var _vf =
	_orb->lookup_value_factory ("IDL:LongList:1.0");
    assert (_vf.in());

    CORBA::ValueBase *_vb = _vf->create_for_unmarshal();
    _v = LongList::_downcast (_vb);

    _m[_id] = _vb;

    LongList *_next;
    CORBA::Long _l;
    if (!(_a >>= _l) ||
	!__any_extract (_a, _next, _m) ||
	!_a.value_get_end (_id, _is_ref))
	return FALSE;
    _v->next (_next);
    _v->l (_l);

    return TRUE;
}

CORBA::Boolean
operator>>= (const CORBA::Any &_a, LongList *&_v)
{
    MapIdVB _m;
    return __any_extract (_a, _v, _m);
}


int
test_value ()
{
    __mk_valuetype_typecodes();
    CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
    orb->register_value_factory ("IDL:Node:1.0", new Node_init);
    orb->register_value_factory ("IDL:LongList:1.0", new LongList_init);

    LongList *n1 = new LongList_impl;
    LongList *n2 = new LongList_impl;

    n1->l (1);
    n1->next (n2);
    n2->l (2);
    n2->next (n1);

    LongList *n3;
    Node *n4;

    CORBA::Any a;
    a <<= n1;
    assert (a >>= n3);
    assert (a >>= n4);

    return 0;
}

/*************************************************************************/

int
main (int argc, char *argv[])
{
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

    int res = 0;
    res += test_basic ();
    res += test_string ();
    res += test_wstring ();
    res += test_fixed ();
    res += test_seq ();
    res += test_array ();
    res += test_struct ();
    res += test_union ();
    res += test_recursive ();
//    res += test_subtyping();
    res += test_typecode ();
    res += test_enum ();
    res += test_interface ();
    res += test_value ();
    
    if (res > 0)
	cout << res << " errors" << endl;
    return !!res;
}
