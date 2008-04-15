
#include <CORBA.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <string>
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <string.h>
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <mico/impl.h>
#include <mico/util.h>
#include "types.h"


using namespace std;

/*
 * general rule for what is passed to StaticAny constructor,
 * StaticTypeInfo::marshal, StaticTypeInfo::demarshal as the
 * second argument:
 *
 * - base types: pointer to the base type;
 *   e.g. CORBA::Long *
 * - string, wstring: pointer to pointer to first char;
 *   e.g. char **
 * - interfaces (including Object, Context, Principal, TypeCode):
 *   pointer to *_ptr;
 *   e.g. Object_ptr *
 * - any: pointer to any; i.e. CORBA::Any *
 * - enum: pointer to enum;
 *   e.g. for enum foo { ... }: foo *
 * - struct: pointer to struct;
 *   e.g. for struct foo { ... }: foo *
 * - exception: pointer to exception;
 *   e.g. for exception foo { ... }: foo *
 * - union: pointer to union;
 *   e.g. for union foo switch (...) { ... }: foo *
 * - fixed: pointer to fixed;
 *   e.g. for typedef fixed<4,2> foo: foo *
 * - array: pointer to first array element;
 *   e.g. for typedef long foo[5]: long *
 * - sequence: pointer to sequence;
 *   e.g. for typedef sequence<long> foo: foo *
 *
 * take care when elements of constructed types are _var types. since
 * the second arg of StaticAny is of type void * the compiler
 * will *not* convert the _var to the containing type. instead you
 * have to explicitely convert the _var type to its containig type.
 * E.g., the IDL struct
 *   struct Foo {
 *       string s;
 *   };
 * will result in the C++ struct
 *   struct Foo {
 *       CORBA::String_var s;
 *   };
 * when passing Foo::s to one of the above mentioned methods, you have to
 * explicitely convert the String_var into a char* and take the address of
 * it to get the required char **:
 *   Foo f;
 *   StaticAny (_stc_Foo, &(char *&)f.s);
 * An better way would be to use the inout() method, because this works
 * for all kinds of _var types, whereas in the above example the cast
 * depends on the exact type of the _var:
 *   Foo f;
 *   StaticAny (_stc_Foo, &f.s.inout());
 */

class TCEnum : public CORBA::StaticTypeInfo {
    typedef Enum T;
public:
    StaticValueType create () const
    { return (StaticValueType)new T; }
    StaticValueType copy (const StaticValueType v) const
    { return (StaticValueType)new T (*(T *)v); }
    void assign (StaticValueType d, const StaticValueType s) const
    { *(T *)d = *(T *)s; }
    void free (StaticValueType v) const
    { delete (T *)v; }
    CORBA::Boolean demarshal (CORBA::DataDecoder &dc, StaticValueType v) const
    {
	CORBA::ULong ul;
	if (!dc.enumeration (ul))
	    return FALSE;
	*(T *)v = (T)ul;
	return TRUE;
    }
    void marshal (CORBA::DataEncoder &ec, StaticValueType v) const
    {
	ec.enumeration ((CORBA::ULong)*(T *)v);
    }
};

CORBA::StaticTypeInfo *_stc_Enum = new TCEnum;


//--------------------


class TCStruct : public CORBA::StaticTypeInfo {
    typedef Struct T;
public:
    StaticValueType create () const
    { return (StaticValueType)new T; }
    StaticValueType copy (const StaticValueType v) const
    { return (StaticValueType)new T (*(T *)v); }
    void assign (StaticValueType d, const StaticValueType s) const
    { *(T *)d = *(T *)s; }
    void free (StaticValueType v) const
    { delete (T *)v; }
    CORBA::Boolean demarshal (CORBA::DataDecoder &dc, StaticValueType v) const
    {
	return
	    dc.struct_begin() &&
	    CORBA::_stc_long->demarshal (dc, &((T *)v)->l) &&
	    CORBA::_stc_string->demarshal (dc, &((T *)v)->s.inout()) &&
	    dc.struct_end();
    }
    void marshal (CORBA::DataEncoder &ec, StaticValueType v) const
    {
	ec.struct_begin();
	CORBA::_stc_long->marshal (ec, &((T *)v)->l);
	CORBA::_stc_string->marshal (ec, &((T *)v)->s.inout());
	ec.struct_end();
    }
};

CORBA::StaticTypeInfo *_stc_Struct = new TCStruct;


//--------------------


/*
 * the _encode() method of the generated exception should look like this:
 *
 * void Except::_encode (CORBA::DataEncoder &ec)
 * {
 *     _stc_Except->marshal (ec, this);
 * }
 */
class TCExcept : public CORBA::StaticTypeInfo {
    typedef Except T;
public:
    StaticValueType create () const
    { return (StaticValueType)new T; }
    StaticValueType copy (const StaticValueType v) const
    { return (StaticValueType)new T (*(T *)v); }
    void assign (StaticValueType d, const StaticValueType s) const
    { *(T *)d = *(T *)s; }
    void free (StaticValueType v) const
    { delete (T *)v; }
    CORBA::Boolean demarshal (CORBA::DataDecoder &dc, StaticValueType v) const
    {
	string repoid;
	return
	    dc.except_begin (repoid) &&
	    CORBA::_stc_long->demarshal (dc, &((T *)v)->l) &&
	    CORBA::_stc_string->demarshal (dc, &((T *)v)->s.inout()) &&
	    dc.except_end();
    }
    void marshal (CORBA::DataEncoder &ec, StaticValueType v) const
    {
	ec.except_begin ("IDL:Except:1.0");
	CORBA::_stc_long->marshal (ec, &((T *)v)->l);
	CORBA::_stc_string->marshal (ec, &((T *)v)->s.inout());
	ec.except_end();
    }
};

CORBA::StaticTypeInfo *_stc_Except = new TCExcept;


//--------------------


class TCImplicitUnion : public CORBA::StaticTypeInfo {
    typedef ImplicitUnion T;
public:
    StaticValueType create () const
    { return (StaticValueType)new T; }
    StaticValueType copy (const StaticValueType v) const
    { return (StaticValueType)new T (*(T *)v); }
    void assign (StaticValueType d, const StaticValueType s) const
    { *(T *)d = *(T *)s; }
    void free (StaticValueType v) const
    { delete (T *)v; }
    CORBA::Boolean demarshal (CORBA::DataDecoder &dc, StaticValueType v) const
    {
	if (!dc.union_begin())
	    return FALSE;
	if (!CORBA::_stc_long->demarshal (dc, &((T *)v)->_discriminator))
	    return FALSE;
	switch (((T *)v)->_discriminator) {
	case 1:
	    if (!CORBA::_stc_long->demarshal (dc, &((T *)v)->_m.l))
		return FALSE;
	    break;
	case 2:
	    if (!CORBA::_stc_string->demarshal (dc, &((T *)v)->_m.s.inout()))
		return FALSE;
	    break;
	}
	return dc.union_end();
    }
    void marshal (CORBA::DataEncoder &ec, StaticValueType v) const
    {
	ec.union_begin();
	CORBA::_stc_long->marshal (ec, &((T *)v)->_discriminator);
	switch (((T *)v)->_discriminator) {
	case 1:
	    CORBA::_stc_long->marshal (ec, &((T *)v)->_m.l);
	    break;
	case 2:
	    CORBA::_stc_string->marshal (ec, &((T *)v)->_m.s.inout());
	    break;
	}
	ec.union_end();
    }
};

CORBA::StaticTypeInfo *_stc_ImplicitUnion = new TCImplicitUnion;


//--------------------


class TCExplicitUnion : public CORBA::StaticTypeInfo {
    typedef ExplicitUnion T;
public:
    StaticValueType create () const
    { return (StaticValueType)new T; }
    StaticValueType copy (const StaticValueType v) const
    { return (StaticValueType)new T (*(T *)v); }
    void assign (StaticValueType d, const StaticValueType s) const
    { *(T *)d = *(T *)s; }
    void free (StaticValueType v) const
    { delete (T *)v; }
    CORBA::Boolean demarshal (CORBA::DataDecoder &dc, StaticValueType v) const
    {
	if (!dc.union_begin())
	    return FALSE;
	if (!CORBA::_stc_long->demarshal (dc, &((T *)v)->_discriminator))
	    return FALSE;
	switch (((T *)v)->_discriminator) {
	case 1:
	    if (!CORBA::_stc_long->demarshal (dc, &((T *)v)->_m.l))
		return FALSE;
	    break;
	case 2:
	    if (!CORBA::_stc_string->demarshal (dc, &((T *)v)->_m.s.inout()))
		return FALSE;
	    break;
	default:
	    CORBA::Boolean b;
	    if (!CORBA::_stc_boolean->demarshal (dc, &((T *)v)->_m.b))
		return FALSE;
	    break;
	}
	return dc.union_end();
    }
    void marshal (CORBA::DataEncoder &ec, StaticValueType v) const
    {
	ec.union_begin();
	CORBA::_stc_long->marshal (ec, &((T *)v)->_discriminator);
	switch (((T *)v)->_discriminator) {
	case 1:
	    CORBA::_stc_long->marshal (ec, &((T *)v)->_m.l);
	    break;
	case 2:
	    CORBA::_stc_string->marshal (ec, &((T *)v)->_m.s.inout());
	    break;
	default:
	    CORBA::_stc_boolean->marshal (ec, &((T *)v)->_m.b);
	    break;
	}
	ec.union_end();
    }
};

CORBA::StaticTypeInfo *_stc_ExplicitUnion = new TCExplicitUnion;


//--------------------


class TCInterface : public CORBA::StaticTypeInfo {
    typedef Interface *T;
public:
    StaticValueType create () const
    { return (StaticValueType)new T (0); }
    StaticValueType copy (const StaticValueType v) const
    { return (StaticValueType)new T (Interface::_duplicate (*(T *)v)); }
    void assign (StaticValueType d, const StaticValueType s) const
    { *(T *)d = Interface::_duplicate (*(T *)s); }
    void free (StaticValueType v) const
    { CORBA::release (*(T *)v); delete (T *)v; }
    CORBA::Boolean demarshal (CORBA::DataDecoder &dc, StaticValueType v) const
    {
	CORBA::Object_ptr obj;
	if (!CORBA::_stc_Object->demarshal (dc, &obj))
	    return FALSE;
	*(T *)v = Interface::_narrow (obj);
	CORBA::release (obj);
	return TRUE;
    }
    void marshal (CORBA::DataEncoder &ec, StaticValueType v) const
    {
	CORBA::Object_ptr obj = *(T *)v;
	CORBA::_stc_Object->marshal (ec, &obj); 
    }
};

CORBA::StaticTypeInfo *_stc_Interface = new TCInterface;


//--------------------


class TCFixedExample : public CORBA::StaticTypeInfo {
    typedef FixedExample T;
public:
    StaticValueType create () const
    { return (StaticValueType)new T; }
    StaticValueType copy (const StaticValueType v) const
    { return (StaticValueType)new T (*(T *)v); }
    void assign (StaticValueType d, const StaticValueType s) const
    { *(T *)d = *(T *)s; }
    void free (StaticValueType v) const
    { delete (T *)v; }
    CORBA::Boolean demarshal (CORBA::DataDecoder &dc, StaticValueType v) const
    {
	FixedBase::FixedValue val;
	if (!dc.get_fixed (val, 4, 2))
	    return FALSE;
	((T *)v)->from_digits (val);
	return TRUE;
    }
    void marshal (CORBA::DataEncoder &ec, StaticValueType v) const
    {
	FixedBase::FixedValue_var val = ((T *)v)->to_digits();
	ec.put_fixed (val, 4, 2);
    }
};

CORBA::StaticTypeInfo *_stc_FixedExample = new TCFixedExample;


//--------------------


class TCBaseArray : public CORBA::StaticTypeInfo {
    typedef BaseArray_slice T;
public:
    StaticValueType create () const
    { return (StaticValueType)BaseArray_alloc(); }
    StaticValueType copy (const StaticValueType v) const
    { return (StaticValueType)BaseArray_dup ((T *)v); }
    // need BaseArray_copy() ...
    void assign (StaticValueType d, const StaticValueType s) const
    {
	for (int i = 0; i < 5; ++i)
	    ((T *)d)[i] = ((T *)s)[i];
    }
    void free (StaticValueType v) const
    { BaseArray_free ((T *)v); }
    CORBA::Boolean demarshal (CORBA::DataDecoder &dc, StaticValueType v) const
    {
	// this is much faster than iterating over the elements, but
	// is only supported for basic types
	return dc.get_longs ((T *)v, 5);
    }
    void marshal (CORBA::DataEncoder &ec, StaticValueType v) const
    {
	// this is much faster than iterating over the elements, but
	// is only supported for basic types
	ec.put_longs ((T *)v, 5);
    }
};

CORBA::StaticTypeInfo *_stc_BaseArray = new TCBaseArray;


//--------------------


class TCConstrArray : public CORBA::StaticTypeInfo {
    typedef ConstrArray_slice T;
public:
    StaticValueType create () const
    { return (StaticValueType)ConstrArray_alloc(); }
    StaticValueType copy (const StaticValueType v) const
    { return (StaticValueType)ConstrArray_dup ((T *)v); }
    // need ConstrArray_copy() ...
    void assign (StaticValueType d, const StaticValueType s) const
    {
	for (int i = 0; i < 5; ++i)
	    ((T *)d)[i] = ((T *)s)[i];
    }
    void free (StaticValueType v) const
    { ConstrArray_free ((T *)v); }
    CORBA::Boolean demarshal (CORBA::DataDecoder &dc, StaticValueType v) const
    {
	if (!dc.arr_begin())
	    return FALSE;
	for (CORBA::ULong i = 0; i < 5; ++i) {
	    if (!_stc_Struct->demarshal (dc, &((T *)v)[i]))
		return FALSE;
	}
	return dc.arr_end();
    }
    void marshal (CORBA::DataEncoder &ec, StaticValueType v) const
    {
	ec.arr_begin();
	for (CORBA::ULong i = 0; i < 5; ++i) {
	    _stc_Struct->marshal (ec, &((T *)v)[i]);
	}
	ec.arr_end();
    }
};

CORBA::StaticTypeInfo *_stc_ConstrArray = new TCConstrArray;


//--------------------


class TCConstrSeq : public CORBA::StaticTypeInfo {
    typedef ConstrSeq T;
public:
    StaticValueType create () const
    { return (StaticValueType)new T; }
    StaticValueType copy (const StaticValueType v) const
    { return (StaticValueType)new T (*(T *)v); }
    void assign (StaticValueType d, const StaticValueType s) const
    { *(T *)d = *(T *)s; }
    void free (StaticValueType v) const
    { delete (T *)v; }
    CORBA::Boolean demarshal (CORBA::DataDecoder &dc, StaticValueType v) const
    {
	CORBA::ULong len;
	if (!dc.seq_begin (len))
	    return FALSE;
	((T *)v)->length (len);
	for (CORBA::ULong i = 0; i < len; ++i) {
	    if (!_stc_Struct->demarshal (dc, &(*(T *)v)[i]))
		return FALSE;
	}
	return dc.seq_end();
    }
    void marshal (CORBA::DataEncoder &ec, StaticValueType v) const
    {
	CORBA::ULong len = ((T *)v)->length();
	ec.seq_begin (len);
	for (CORBA::ULong i = 0; i < len; ++i) {
	    _stc_Struct->marshal (ec, &(*(T *)v)[i]);
	}
	ec.seq_end();
    }
};

CORBA::StaticTypeInfo *_stc_ConstrSeq = new TCConstrSeq;


//--------------------


/*
 * valuetype Node {
 * public Node next;
 * };
 *
 * valuetype LongNode truncatable Node {
 * public long l;
 * };
 */

extern CORBA::StaticTypeInfo *_stc_Node;
class Node;
typedef ValueVar<Node> Node_var;

class Node : virtual public CORBA::ValueBase {
public:
    virtual Node *next () const = 0;
    virtual void next (Node *n) = 0;

    static Node *_downcast (CORBA::ValueBase *v)
    { return (Node *)v->_narrow_helper("Node"); }

    void *_narrow_helper (const char *id)
    {
	if (!strcmp (id, "Node"))
	    return (void *)this;
	return 0;
    }

    CORBA::ValueDef * get_value_def()
    { return 0; }

    void _get_marshal_info (vector<string> &repoids, CORBA::Boolean &chunked)
    {
	/*
	 * repoids Node can be truncated to (including itself).
	 */
	repoids.push_back ("IDL:Node:1.0");
	/*
	 * we have to use chunked encoding only if we have a truncateable
	 * base type or have custom marshalling ...
	 */
	chunked = FALSE;
    }
    void _marshal_members (CORBA::DataEncoder &ec)
    {
	Node *_next = next();
	_stc_Node->marshal (ec, &_next);
    }
    CORBA::Boolean _demarshal_members (CORBA::DataDecoder &ec)
    {
	Node_var _next;
	if (!_stc_Node->demarshal (ec, &_next.inout()))
	    return FALSE;
	next (_next);

	return TRUE;
    }
};


class Node_impl : virtual public Node,
		  virtual public CORBA::DefaultValueRefCountBase {
    Node *_next;
public:
    Node_impl () { _next = 0; }
    ~Node_impl () { CORBA::remove_ref (_next); }

    virtual Node *next () const { return _next; }
    virtual void next (Node *n)
    {
	CORBA::remove_ref (_next);
	_next = n;
	CORBA::add_ref (_next);
    }

    CORBA::ValueBase *_copy_value ()
    { return new Node_impl (*this); }
};

class Node_init : virtual public CORBA::ValueFactoryBase {
public:
    CORBA::ValueBase *create_for_unmarshal ()
    { return new Node_impl; }
    CORBA::TypeCode_ptr type ()
    { return CORBA::TypeCode::_nil(); /*XXX*/ }
};


class TCNode : public CORBA::StaticTypeInfo {
    typedef Node *T;
public:
    StaticValueType create () const
    { return (StaticValueType)new T (0); }
    StaticValueType copy (const StaticValueType v) const
    { CORBA::add_ref (*(T *)v); return (StaticValueType)new T (*(T *)v); }
    void assign (StaticValueType d, const StaticValueType s) const
    { CORBA::add_ref (*(T *)s); *(T *)d = *(T *)s; }
    void free (StaticValueType v) const
    { CORBA::remove_ref (*(T *)v); delete (T *)v; }
    CORBA::Boolean demarshal (CORBA::DataDecoder &dc, StaticValueType v) const
    {
	CORBA::ValueBase *vb;
	if (!CORBA::ValueBase::_demarshal (dc, vb, "IDL:Node:1.0"))
	    return FALSE;
	*(T *)v = Node::_downcast (vb);
	if (!*(T *)v) {
	    CORBA::remove_ref (vb);
	    return FALSE;
	}
	return TRUE;
    }
    void marshal (CORBA::DataEncoder &ec, StaticValueType v) const
    {
	CORBA::ValueBase::_marshal (ec, *(T *)v);
    }
};

CORBA::StaticTypeInfo *_stc_Node = new TCNode;





extern CORBA::StaticTypeInfo *_stc_LongNode;
class LongNode;
typedef ValueVar<LongNode> LongNode_var;

class LongNode : virtual public Node {
public:
    virtual CORBA::Long l () const = 0;
    virtual void l (CORBA::Long l) = 0;

    static LongNode *_downcast (CORBA::ValueBase *v)
    { return (LongNode *)v->_narrow_helper("LongNode"); }

    void *_narrow_helper (const char *id)
    {
	if (!strcmp (id, "Node"))
	    return (void *)(Node *)this;
	if (!strcmp (id, "LongNode"))
	    return (void *)this;
	return 0;
    }

    CORBA::ValueDef * get_value_def()
    { return 0; }
    
    void _get_marshal_info (vector<string> &repoids, CORBA::Boolean &chunked)
    {
	/*
	 * repoids LongNode can be truncated to (including itself).
	 */
	repoids.push_back ("IDL:LongNode:1.0");
	repoids.push_back ("IDL:Node:1.0");
	/*
	 * we have to use chunked encoding only if we have a truncateable
	 * base type or have custom marshalling ...
	 */
	chunked = TRUE;
    }
    void _marshal_members (CORBA::DataEncoder &ec)
    {
	Node::_marshal_members (ec);
	CORBA::Long _l = l();
	CORBA::_stc_long->marshal (ec, &_l);
    }
    CORBA::Boolean _demarshal_members (CORBA::DataDecoder &ec)
    {
	CORBA::Long _l;
	if (!Node::_demarshal_members (ec) ||
	    !CORBA::_stc_long->demarshal (ec, &_l))
	    return FALSE;

	l (_l);

	return TRUE;
    }
};

class LongNode_impl : virtual public LongNode,
		      virtual public CORBA::DefaultValueRefCountBase {
    Node *_next;
    CORBA::Long _l;
public:
    LongNode_impl () { _next = 0; }
    ~LongNode_impl () { CORBA::remove_ref (_next); }

    virtual Node *next () const { return _next; }
    virtual void next (Node *n)
    {
	CORBA::remove_ref (_next);
	_next = n;
	CORBA::add_ref (_next);
    }

    virtual CORBA::Long l () const { return _l; }
    virtual void l (CORBA::Long n) { _l = n; }

    CORBA::ValueBase *_copy_value ()
    { return new LongNode_impl (*this); }
};

class LongNode_init : virtual public CORBA::ValueFactoryBase {
public:
    CORBA::ValueBase *create_for_unmarshal ()
    { return new LongNode_impl; }
    CORBA::TypeCode_ptr type ()
    { return CORBA::TypeCode::_nil(); /*XXX*/ }
};

class TCLongNode : public CORBA::StaticTypeInfo {
    typedef LongNode *T;
public:
    StaticValueType create () const
    { return (StaticValueType)new T (0); }
    StaticValueType copy (const StaticValueType v) const
    { CORBA::add_ref (*(T *)v); return (StaticValueType)new T (*(T *)v); }
    void assign (StaticValueType d, const StaticValueType s) const
    { CORBA::add_ref (*(T *)s); *(T *)d = *(T *)s; }
    void free (StaticValueType v) const
    { CORBA::remove_ref (*(T *)v); delete (T *)v; }
    CORBA::Boolean demarshal (CORBA::DataDecoder &dc, StaticValueType v) const
    {
	CORBA::ValueBase *vb;
	if (!CORBA::ValueBase::_demarshal (dc, vb, "IDL:LongNode:1.0"))
	    return FALSE;
	*(T *)v = LongNode::_downcast (vb);
	if (!*(T *)v) {
	    CORBA::remove_ref (vb);
	    return FALSE;
	}
	return TRUE;
    }
    void marshal (CORBA::DataEncoder &ec, StaticValueType v) const
    {
	CORBA::ValueBase::_marshal (ec, *(T *)v);
    }
};

CORBA::StaticTypeInfo *_stc_LongNode = new TCLongNode;


//--------------------


class Interface_impl : virtual public POA_Interface {
};


inline int
operator== (const Struct &s1, const Struct &s2)
{
    if (s1.l != s2.l)
	return 0;
    if (!!s1.s.in() ^ !!s2.s.in())
	return 0;
    return !s1.s.in() || !strcmp (s1.s, s2.s);
}

inline int
operator== (const Except &s1, const Except &s2)
{
    if (s1.l != s2.l)
	return 0;
    if (!!s1.s.in() ^ !!s2.s.in())
	return 0;
    return !s1.s.in() || !strcmp (s1.s, s2.s);
}

inline int
operator== (const ExplicitUnion &s1, const ExplicitUnion &s2)
{
    return s1._d() == s2._d();
}

inline int
operator== (const ImplicitUnion &s1, const ImplicitUnion &s2)
{
    return s1._d() == s2._d();
}

template<class T>
struct StdCmp {
    int compare (const T &t1, const T &t2)
    {
	return t1 == t2;
    }
};

template<class T, int N>
struct ArrCmp {
    int compare (const T &a1, const T &a2)
    {
	for (int i = 0; i < N; ++i)
	    if (!(a1[i] == a2[i]))
		return 0;
	return 1;
    }
};

struct StrCmp {
    int compare (const char *t1, const char *t2)
    {
	return !strcmp (t1, t2);
    }
};

struct WStrCmp {
    int compare (const wchar_t *t1, const wchar_t *t2)
    {
	return !xwcscmp (t1, t2);
    }
};

struct ObjCmp {
    int compare (CORBA::Object_ptr obj1, CORBA::Object_ptr obj2)
    {
	return obj1->_is_equivalent (obj2);
    }
};

struct TCCmp {
    int compare (CORBA::TypeCode_ptr tc1, CORBA::TypeCode_ptr tc2)
    {
	return tc1->equal (tc2);
    }
};

struct ValueTypeCmp {
    int compare (Node *n1, Node *n2)
    {
	return n2->next()->next() == n2 &&
	    LongNode::_downcast(n2)->l() == 1 &&
	    LongNode::_downcast(n2->next())->l() == 2;
    }
};

template<class C, class T>
struct Checker {
    Checker (CORBA::StaticTypeInfo *ti, const T &val1)
    {
	MICO::CDREncoder ec;
	MICO::CDRDecoder dc (ec.buffer(), FALSE);

	CORBA::DataEncoder::ValueState evstate;
	CORBA::DataDecoder::ValueState dvstate;

	ec.valuestate (&evstate, FALSE);
	dc.valuestate (&dvstate, FALSE);
	
	CORBA::Boolean ret;

	T val2;

	CORBA::StaticAny any1 (ti, (void *)&val1);
	CORBA::StaticAny any2 (ti, (void *)&val2);

	any1.marshal (ec);
	ret = any2.demarshal (dc);
	assert (ret);

	C cmp;
	assert (cmp.compare (val1, val2));
    }
};

void
do_tests ()
{
    // basic types
    Checker<StdCmp<CORBA::Short>, CORBA::Short>
	short_c (CORBA::_stc_short, 42);

    Checker<StdCmp<CORBA::UShort>, CORBA::UShort>
	ushort_c (CORBA::_stc_ushort, 42);

    Checker<StdCmp<CORBA::Long>, CORBA::Long>
	long_c (CORBA::_stc_long, 42);

    Checker<StdCmp<CORBA::ULong>, CORBA::ULong>
	ulong_c (CORBA::_stc_ulong, 42);

    Checker<StdCmp<CORBA::LongLong>, CORBA::LongLong>
	longlong_c (CORBA::_stc_longlong, 42);

    Checker<StdCmp<CORBA::ULongLong>, CORBA::ULongLong>
	ulonglong_c (CORBA::_stc_ulonglong, 42);

    Checker<StdCmp<CORBA::Float>, CORBA::Float>
	float_c (CORBA::_stc_float, 42);

    Checker<StdCmp<CORBA::Double>, CORBA::Double>
	double_c (CORBA::_stc_double, 42);

    Checker<StdCmp<CORBA::LongDouble>, CORBA::LongDouble>
	ldouble_c (CORBA::_stc_longdouble, 42);

    Checker<StdCmp<CORBA::Octet>, CORBA::Octet>
	octet_c (CORBA::_stc_octet, 42);

    Checker<StdCmp<CORBA::Boolean>, CORBA::Boolean>
	bool_c (CORBA::_stc_boolean, TRUE);

    Checker<StdCmp<CORBA::Char>, CORBA::Char>
	char_c (CORBA::_stc_char, 42);

    Checker<StdCmp<CORBA::WChar>, CORBA::WChar>
	wchar_c (CORBA::_stc_wchar, 42);

    // string, wstring
    char *string_i = "foo";
    Checker<StrCmp, char *> string_c (CORBA::_stc_string, string_i);

    wchar_t *wstring_i = L"foo";
    Checker<WStrCmp, wchar_t *> wstring_c (CORBA::_stc_wstring, wstring_i);

    // any
    CORBA::Any any_i;
    any_i <<= (CORBA::Long)42;
    Checker<StdCmp<CORBA::Any>, CORBA::Any> any_c (CORBA::_stc_any, any_i);

    // typecode
    Checker<TCCmp, CORBA::TypeCode_ptr> tc_c
	(CORBA::_stc_TypeCode, _tc_Struct);

    // context, princiapl

    // interface
    Interface_impl* iimpl = new Interface_impl;
    Interface_ptr iface_i = iimpl->_this();
    Checker<ObjCmp, Interface_ptr> iface_s (_stc_Interface, iface_i);

    // sequences of basic types
    SequenceTmpl<CORBA::Short,MICO_TID_DEF> short_i;
    short_i.length (2);
    short_i[0] = 1;
    short_i[1] = 2;
    Checker<StdCmp<SequenceTmpl<CORBA::Short,MICO_TID_DEF> >,
	SequenceTmpl<CORBA::Short,MICO_TID_DEF> >
	short_s (CORBA::_stcseq_short, short_i);

    SequenceTmpl<CORBA::UShort,MICO_TID_DEF> ushort_i;
    ushort_i.length (2);
    ushort_i[0] = 1;
    ushort_i[1] = 2;
    Checker<StdCmp<SequenceTmpl<CORBA::UShort,MICO_TID_DEF> >,
	SequenceTmpl<CORBA::UShort,MICO_TID_DEF> >
	ushort_s (CORBA::_stcseq_ushort, ushort_i);

    SequenceTmpl<CORBA::Long,MICO_TID_DEF> long_i;
    long_i.length (2);
    long_i[0] = 1;
    long_i[1] = 2;
    Checker<StdCmp<SequenceTmpl<CORBA::Long,MICO_TID_DEF> >,
	SequenceTmpl<CORBA::Long,MICO_TID_DEF> >
	long_s (CORBA::_stcseq_long, long_i);

    SequenceTmpl<CORBA::ULong,MICO_TID_DEF> ulong_i;
    ulong_i.length (2);
    ulong_i[0] = 1;
    ulong_i[1] = 2;
    Checker<StdCmp<SequenceTmpl<CORBA::ULong,MICO_TID_DEF> >,
	SequenceTmpl<CORBA::ULong,MICO_TID_DEF> >
	ulong_s (CORBA::_stcseq_ulong, ulong_i);

    SequenceTmpl<CORBA::LongLong,MICO_TID_DEF> longlong_i;
    longlong_i.length (2);
    longlong_i[0] = 1;
    longlong_i[1] = 2;
    Checker<StdCmp<SequenceTmpl<CORBA::LongLong,MICO_TID_DEF> >,
	SequenceTmpl<CORBA::LongLong,MICO_TID_DEF> >
	longlong_s (CORBA::_stcseq_longlong, longlong_i);

    SequenceTmpl<CORBA::ULongLong,MICO_TID_DEF> ulonglong_i;
    ulonglong_i.length (2);
    ulonglong_i[0] = 1;
    ulonglong_i[1] = 2;
    Checker<StdCmp<SequenceTmpl<CORBA::ULongLong,MICO_TID_DEF> >,
	SequenceTmpl<CORBA::ULongLong,MICO_TID_DEF> >
	ulonglong_s (CORBA::_stcseq_ulonglong, ulonglong_i);

    SequenceTmpl<CORBA::Float,MICO_TID_DEF> float_i;
    float_i.length (2);
    float_i[0] = 1;
    float_i[1] = 2;
    Checker<StdCmp<SequenceTmpl<CORBA::Float,MICO_TID_DEF> >,
	SequenceTmpl<CORBA::Float,MICO_TID_DEF> >
	float_s (CORBA::_stcseq_float, float_i);

    SequenceTmpl<CORBA::Double,MICO_TID_DEF> double_i;
    double_i.length (2);
    double_i[0] = 1;
    double_i[1] = 2;
    Checker<StdCmp<SequenceTmpl<CORBA::Double,MICO_TID_DEF> >,
	SequenceTmpl<CORBA::Double,MICO_TID_DEF> >
	double_s (CORBA::_stcseq_double, double_i);

    SequenceTmpl<CORBA::LongDouble,MICO_TID_DEF> longdouble_i;
    longdouble_i.length (2);
    longdouble_i[0] = 1;
    longdouble_i[1] = 2;
    Checker<StdCmp<SequenceTmpl<CORBA::LongDouble,MICO_TID_DEF> >,
	SequenceTmpl<CORBA::LongDouble,MICO_TID_DEF> >
	ldouble_s (CORBA::_stcseq_longdouble, longdouble_i);

    SequenceTmpl<CORBA::Octet,MICO_TID_OCTET> octet_i;
    octet_i.length (2);
    octet_i[0] = 1;
    octet_i[1] = 2;
    Checker<StdCmp<SequenceTmpl<CORBA::Octet,MICO_TID_OCTET> >,
	SequenceTmpl<CORBA::Octet,MICO_TID_OCTET> >
	octet_s (CORBA::_stcseq_octet, octet_i);

    SequenceTmpl<CORBA::Boolean,MICO_TID_BOOL> bool_i;
    bool_i.length (2);
    bool_i[0] = TRUE;
    bool_i[1] = FALSE;
    Checker<StdCmp<SequenceTmpl<CORBA::Boolean,MICO_TID_BOOL> >,
	SequenceTmpl<CORBA::Boolean,MICO_TID_BOOL> >
	bool_s (CORBA::_stcseq_boolean, bool_i);

    SequenceTmpl<CORBA::Char,MICO_TID_CHAR> char_i;
    char_i.length (2);
    char_i[0] = 1;
    char_i[1] = 2;
    Checker<StdCmp<SequenceTmpl<CORBA::Char,MICO_TID_CHAR> >,
	SequenceTmpl<CORBA::Char,MICO_TID_CHAR> >
	char_s (CORBA::_stcseq_char, char_i);

    SequenceTmpl<CORBA::WChar,MICO_TID_WCHAR> wchar_i;
    wchar_i.length (2);
    wchar_i[0] = 1;
    wchar_i[1] = 2;
    Checker<StdCmp<SequenceTmpl<CORBA::WChar,MICO_TID_WCHAR> >,
	SequenceTmpl<CORBA::WChar,MICO_TID_WCHAR> >
	wchar_s (CORBA::_stcseq_wchar, wchar_i);

    // enum
    Checker<StdCmp<Enum>, Enum> Enum_c (_stc_Enum, enum2);

    // struct
    Struct Struct_i;
    Struct_i.l = 42;
    Struct_i.s = (const char *)"foo";
    Checker<StdCmp<Struct>, Struct> Struct_c (_stc_Struct, Struct_i);

    // exception
    Except Except_i;
    Except_i.l = 42;
    Except_i.s = (const char *)"foo";
    Checker<StdCmp<Except>, Except> Except_c (_stc_Except, Except_i);

    // union with explicit default case
    ExplicitUnion ExUn_i;
    ExUn_i.b (TRUE);
    Checker<StdCmp<ExplicitUnion>, ExplicitUnion>
	ExUn_c (_stc_ExplicitUnion, ExUn_i);

    // union with implicit default case
    ImplicitUnion ImUn_i;
    ImUn_i.l (42);
    Checker<StdCmp<ImplicitUnion>, ImplicitUnion>
	ImUn_c (_stc_ImplicitUnion, ImUn_i);

    // fixed
    FixedExample Fixed_i ((CORBA::LongDouble)42.34);
    Checker<StdCmp<FixedExample>, FixedExample>
	Fixed_c (_stc_FixedExample, Fixed_i);

    // sequence of constructed type
    ConstrSeq CoSeq_i;
    CoSeq_i.length (5);
    CoSeq_i[0].l = 1;
    CoSeq_i[1].l = 2;
    CoSeq_i[2].l = 3;
    CoSeq_i[3].l = 4;
    CoSeq_i[4].l = 5;
    CoSeq_i[0].s = (const char *)"1";
    CoSeq_i[1].s = (const char *)"2";
    CoSeq_i[2].s = (const char *)"3";
    CoSeq_i[3].s = (const char *)"4";
    CoSeq_i[4].s = (const char *)"5";
    Checker<StdCmp<ConstrSeq>, ConstrSeq> CoSeq_c (_stc_ConstrSeq, CoSeq_i);

    // array of base type
    BaseArray BaseArr_i = { 1, 2, 3, 4, 5 };
    Checker<ArrCmp<BaseArray,5>, BaseArray>
	BaseArr_c (_stc_BaseArray, BaseArr_i);

    // array of constructed type
    ConstrArray CoArr_i;
    CoArr_i[0].l = 1;
    CoArr_i[1].l = 2;
    CoArr_i[2].l = 3;
    CoArr_i[3].l = 4;
    CoArr_i[4].l = 5;
    CoArr_i[0].s = (const char *)"1";
    CoArr_i[1].s = (const char *)"2";
    CoArr_i[2].s = (const char *)"3";
    CoArr_i[3].s = (const char *)"4";
    CoArr_i[4].s = (const char *)"5";
    Checker<ArrCmp<ConstrArray,5>, ConstrArray>
	CoArr_c (_stc_ConstrArray, CoArr_i);

    // valuetype
    CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
    orb->register_value_factory ("IDL:Node:1.0", new Node_init);
    orb->register_value_factory ("IDL:LongNode:1.0", new LongNode_init);

    LongNode *n1 = new LongNode_impl;
    LongNode *n2 = new LongNode_impl;

    n1->next (n2);
    n1->l (1);

    n2->next (n1);
    n2->l (2);

    Checker<ValueTypeCmp, Node*> ValueType_c (_stc_Node, n1);
}

int
main (int argc, char *argv)
{
    do_tests();
    return 0;
}
