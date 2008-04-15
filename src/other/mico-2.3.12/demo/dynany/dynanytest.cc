// **********************************************************************
//
// This file contains a set of tests about CORBA::DynAny API.
//
// Author: Philippe Merle
// from the Laboratoire d'Informatique Fondamentale de Lille in France 
// (it is a computer science laboratory of the Lille french university)
//
// All Rights Reserved
//
// **********************************************************************

#define MICO_CONF_DYNANY
#define MICO_CONF_IR
#include <CORBA-SMALL.h>                          // here is for MICO
#include <mico/template_impl.h>

#include <stdlib.h>
#include <stdio.h>

using namespace std;

// ----------------------------------------------------------------------
// CORBA2 macro
// ----------------------------------------------------------------------

// this macro is used to access CORBA module definitions
#define CORBA2(IDENT) CORBA::IDENT

#define CORBA2_DynAny DynamicAny::DynAny
#define CORBA2_DynAny_ptr DynamicAny::DynAny_ptr
#define CORBA2_DynAny_var DynamicAny::DynAny_var

#define CORBA2_DynEnum DynamicAny::DynEnum
#define CORBA2_DynEnum_var DynamicAny::DynEnum_var

#define CORBA2_DynArray DynamicAny::DynArray
#define CORBA2_DynArray_ptr DynamicAny::DynArray_ptr
#define CORBA2_DynArray_var DynamicAny::DynArray_var

#define CORBA2_DynSequence DynamicAny::DynSequence
#define CORBA2_DynSequence_ptr DynamicAny::DynSequence_ptr
#define CORBA2_DynSequence_var DynamicAny::DynSequence_var

#define CORBA2_DynStruct DynamicAny::DynStruct
#define CORBA2_DynStruct_ptr DynamicAny::DynStruct_ptr
#define CORBA2_DynStruct_var DynamicAny::DynStruct_var

#define CORBA2_DynUnion DynamicAny::DynUnion
#define CORBA2_DynUnion_var DynamicAny::DynUnion_var

#define CORBA2_AnySeq  DynamicAny::AnySeq
#define CORBA2_AnySeq_var  DynamicAny::AnySeq_var

#define CORBA2_NameValuePairSeq_var DynamicAny::NameValuePairSeq_var

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// a class to display CORBA::TypeCode and CORBA::DynAny instances
//
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

class CORBA_OUT
{
protected: // internal state

   int level_;
   ostream& ostr_;
   CORBA2(ORB_ptr) orb_;
   DynamicAny::DynAnyFactory_ptr fac_;

public: // constructor and destructor

   CORBA_OUT (ostream& ostr)
     : level_(0), ostr_ (ostr), orb_(0), fac_(0) {};
   ~CORBA_OUT () {};

public: // manage level

   void set_orb (CORBA2(ORB_ptr) orb) { orb_ = orb; };
   void set_fac (DynamicAny::DynAnyFactory_ptr fac) { fac_ = fac; };
   void down () { level_ ++; };
   void up () { level_ --; };

   ostream& skip () {
       for(int i=0; i<level_; i++) ostr_ << "   ";
       return ostr_;
   };

public:

   void display_IDL_TypeCode (CORBA2(TypeCode_ptr));
   void show_name_TypeCode (CORBA2(TypeCode_ptr));

   void display_DynAny (CORBA2_DynAny_ptr);
   void show_DynAny_without_type (CORBA2_DynAny_ptr);
   void show_DynAny (CORBA2_DynAny_ptr);
   void display_Any (const CORBA2(Any)&);
   void show_Any (const CORBA2(Any)&);

   void display_AnySeq (CORBA2_AnySeq* any_seq);
};

// ----------------------------------------------------------------------
// Get original type, i.e. remove aliases
// ----------------------------------------------------------------------

CORBA2(TypeCode_ptr)
GetOrigType(CORBA2(TypeCode_ptr) p)
{
/*
    CORBA2(TypeCode_var) tc = CORBA2(TypeCode)::_duplicate(p);

    while(tc -> kind() == CORBA2(tk_alias))
	tc = tc -> content_type();

    return tc._retn();
*/
  return CORBA2(TypeCode)::_duplicate ( p->unalias() );
}

// ----------------------------------------------------------------------
// display CORBA::TypeCode
// ----------------------------------------------------------------------

static char* NameTypeIDL [] = { "null", "void", "short", "long",
  "unsigned short", "unsigned long", "float", "double", "boolean", "char",
  "octet", "any", "TypeCode", "Principal", "objref", "struct", "union",
  "enum", "string", "sequence", "array", "alias", "exception"
};

void CORBA_OUT::display_IDL_TypeCode (CORBA2(TypeCode_ptr) tc)
{
    CORBA2(ULong) i;
    CORBA2(TCKind) kind = tc->kind();

    switch ( kind ) {
    case CORBA2(tk_objref):
        skip() << "interface " << tc->name() << ';';
        break;

    case CORBA2(tk_struct):
    case CORBA2(tk_except): {
        skip() << NameTypeIDL[kind] << ' ' << tc->name() << " {" << endl;

        down ();
        for (i=0; i<tc->member_count(); i++) {
            skip();
            CORBA2(TypeCode_var) member_type = tc->member_type(i);
            show_name_TypeCode (member_type);
            ostr_ << ' ' << tc->member_name(i) << ';' << endl;
        }
        up ();
        skip() << "};";
      } break;
 
    case CORBA2(tk_union): {
        skip () << "union " << tc->name() << " switch(";
        CORBA2(TypeCode_var) discriminator_type = tc->discriminator_type();
        show_name_TypeCode (discriminator_type);
        ostr_ << ") {" << endl;

        down ();
        for (i=0 ; i<tc->member_count(); i++) {
            CORBA2(Any_var) any_member_label = tc->member_label(i);
            CORBA2(TypeCode_var) type_label = any_member_label->type();
            if ( type_label->kind() == CORBA2(tk_octet) ) {
	        skip() << "default";
            } else {
	        skip() << "case ";
                CORBA2_DynAny_var dyn_any_member_label =
		    fac_->create_dyn_any (any_member_label);
               show_DynAny_without_type (dyn_any_member_label);
            }
            ostr_ << ": ";
            CORBA2(TypeCode_var) member_type = tc->member_type(i);
            show_name_TypeCode (member_type);
            ostr_ << ' ' << tc->member_name(i) << ';' << endl;
        }
        up();
        skip() << "};"; 
      } break;

    case CORBA2(tk_enum):
        skip()  << "enum " << tc->name() << " {";
        for (i=0; i<tc->member_count(); i++) {
            if ( i != 0 )
 	       ostr_ << ", ";
            ostr_ << tc->member_name(i);
        }
        ostr_ << "};";
        break;

    case CORBA2(tk_string): {
        skip() << "string";
        CORBA2(ULong) length = tc->length();
        if (length != 0)
	  ostr_ << '<' << length << '>';
      } break;

    case CORBA2(tk_sequence): {
        skip() << "sequence<";
        CORBA2(TypeCode_var) content_type = tc->content_type();
        show_name_TypeCode (content_type);
        CORBA2(ULong) length = tc->length();
        if (length != 0)
          ostr_ << ", " << length;
        ostr_ << '>';
      } break;

    case CORBA2(tk_array): {
        skip();
        CORBA2(TypeCode_var) content_type = tc->content_type();
        show_name_TypeCode (content_type);
        ostr_ << '[' << tc->length() << ']';
      } break;

    case CORBA2(tk_alias): {
        skip() << "typedef ";
        CORBA2(TypeCode_var) content_type = tc->content_type();
        show_name_TypeCode (content_type);
        ostr_ << ' ' << tc->name() << ';';
      } break;

    default:
        skip() << NameTypeIDL [kind];
            break;
    }
    ostr_ << endl;
}

void CORBA_OUT::show_name_TypeCode (CORBA2(TypeCode_ptr) tc)
{
    CORBA2(TCKind) kind = tc->kind();

    switch ( kind ) {
    case CORBA2(tk_objref):
    case CORBA2(tk_enum):
    case CORBA2(tk_union):
    case CORBA2(tk_struct):
    case CORBA2(tk_except):
    case CORBA2(tk_alias):
        ostr_ << tc->name();
        break;

    case CORBA2(tk_string): {
        ostr_ << "string";
        CORBA2(ULong) length = tc->length();
        if (length != 0)
	  ostr_ << '<' << length << '>';
      } break;

    case CORBA2(tk_sequence): {
        ostr_ << "sequence<";
        CORBA2(TypeCode_var) content_type = tc->content_type();
        show_name_TypeCode (content_type);
        CORBA2(ULong) length = tc->length();
        if (length != 0)
          ostr_ << ", " << length;
        ostr_ << '>';
      } break;

    case CORBA2(tk_array): {
        CORBA2(TypeCode_var) content_type = tc->content_type();
        show_name_TypeCode (content_type);
        ostr_ << '[' << tc->length() << ']';
      } break;

    default:
        ostr_ << NameTypeIDL [kind];
        break;
    }
}

// ----------------------------------------------------------------------
// show CORBA::DynAny
// ----------------------------------------------------------------------

void CORBA_OUT::display_DynAny (CORBA2_DynAny_ptr dyn_any)
{
    skip();
    show_DynAny(dyn_any);
    ostr_ << endl;
}

void CORBA_OUT::show_DynAny (CORBA2_DynAny_ptr dyn_any)
{
    CORBA2(TypeCode_var) tc = dyn_any->type();

    show_name_TypeCode (tc);
    ostr_ << '(';
    show_DynAny_without_type(dyn_any);
    ostr_ << ')';
}

void CORBA_OUT::show_DynAny_without_type (CORBA2_DynAny_ptr dyn_any)
{
    CORBA2(ULong) i;

    dyn_any->rewind ();

    CORBA2(TypeCode_var) tc = dyn_any->type();
    tc = GetOrigType(tc);

    CORBA2(TCKind) kind = tc->kind();

    switch ( kind ) {
    case CORBA2(tk_void):
	break;

    case CORBA2(tk_short): {
        CORBA2(Short) v_short = dyn_any->get_short();
        ostr_ << v_short;
      } break;

    case CORBA2(tk_ushort): {
        CORBA2(UShort) v_ushort = dyn_any->get_ushort();
        ostr_ << v_ushort;
      } break;

    case CORBA2(tk_long): {
        CORBA2(Long) v_long = dyn_any->get_long();
        ostr_ << v_long;
      } break;

    case CORBA2(tk_ulong): {
        CORBA2(ULong) v_ulong = dyn_any->get_ulong();
        ostr_ << v_ulong;
      } break;

    case CORBA2(tk_float): {
        CORBA2(Float) v_float = dyn_any->get_float();
        ostr_ << v_float;
      } break;

    case CORBA2(tk_double): {
        CORBA2(Float) v_double = dyn_any->get_double();
        ostr_ << v_double;
      } break;

    case CORBA2(tk_boolean): {
        CORBA2(Boolean) v_boolean = dyn_any->get_boolean();
//        if ( v_boolean == CORBA2(TRUE) ) ostr_ << "TRUE";
//        else if ( v_boolean == CORBA2(FALSE) ) ostr_ << "FALSE";
//        else ostr_ << "UNKNOWN_BOOLEAN";
        if ( v_boolean ) ostr_ << "TRUE";
        else  ostr_ << "FALSE";
      } break;

    case CORBA2(tk_octet): {
        CORBA2(Octet) v_octet = dyn_any->get_octet();
        ostr_ << int(v_octet);
      } break;

    case CORBA2(tk_char): {
        CORBA2(Char) v_char = dyn_any->get_char();
        ostr_ << "'" << (char)v_char << "'";
      } break;

    case CORBA2(tk_string): {
        CORBA2(String_var) v_string = dyn_any->get_string();
        ostr_ << '"' << v_string.in() << '"';
      } break;

    case CORBA2(tk_TypeCode): {
        CORBA2(TypeCode_var) v_tc = dyn_any->get_typecode();
        show_name_TypeCode (v_tc);
      } break;

    case CORBA2(tk_any): {
        CORBA2(Any_var) v_any = dyn_any->get_any();
        show_Any(v_any);
      } break; 

    case CORBA2(tk_array): {
        CORBA2_DynArray_var dyn_array = CORBA2_DynArray::_narrow (dyn_any);
        for (i=0; i<tc->length(); i++) {
            if ( i != 0 ) ostr_ << ", ";
            CORBA2_DynAny_var component = dyn_array->current_component();
            show_DynAny (component);
            dyn_array->next ();
	}
      } break;   

    case CORBA2(tk_sequence): {
        CORBA2_DynSequence_var dyn_sequence =
	    CORBA2_DynSequence::_narrow (dyn_any);
        for (i=0; i<dyn_sequence->get_length(); i++) {
            if ( i != 0 ) ostr_ << ", ";
            CORBA2_DynAny_var component = dyn_sequence->current_component();
            show_DynAny (component);
            dyn_sequence->next ();
	}
      } break;   

    case CORBA2(tk_enum): {
         CORBA2_DynEnum_var dyn_enum = CORBA2_DynEnum::_narrow (dyn_any);
         CORBA2(String_var) v_string = dyn_enum->get_as_string();
         ostr_ << v_string.in();
       } break;   

   case CORBA2(tk_struct): {
        CORBA2_DynStruct_var dyn_struct =
	    CORBA2_DynStruct::_narrow (dyn_any);

        ostr_ << endl;
        down ();

        CORBA2(ULong) member_count = tc->member_count();
        for (i=0; i<member_count; i++) {
            CORBA2(String_var) member_name = dyn_struct->current_member_name();
            skip() << member_name.in() << " = ";
            CORBA2_DynAny_var component = dyn_struct->current_component();
            show_DynAny (component);
            ostr_ << endl;
            dyn_struct->next();  
        }

        up ();
        skip();
      } break;

   case CORBA2(tk_union): {
        CORBA2_DynUnion_var dyn_union = CORBA2_DynUnion::_narrow(dyn_any);

        ostr_ << endl;
        down ();

        CORBA2_DynAny_var component = dyn_union->current_component();

        skip() << "discriminator = ";
        show_DynAny (component);
        ostr_ << endl;

        dyn_union->next();

        component = dyn_union->current_component();

        CORBA2(String_var) member_name = dyn_union->member_name ();
        skip() << member_name.in() << " = ";

        if ( !CORBA2(is_nil) (component) )
            show_DynAny (component);
        else
            ostr_ << "not initialised" << endl;

        ostr_ << endl;

        up ();
        skip();
      } break;

    default:
        assert (false);
        break;
    }
    dyn_any->rewind();
}

// ----------------------------------------------------------------------
// show CORBA::Any
// ----------------------------------------------------------------------

void CORBA_OUT::display_Any (const CORBA2(Any)& any)
{
    CORBA2_DynAny_var dyn_any = fac_->create_dyn_any (any);
    display_DynAny (dyn_any);
}

void CORBA_OUT::show_Any (const CORBA2(Any)& any)
{
    CORBA2_DynAny_var dyn_any = fac_->create_dyn_any (any);
    show_DynAny (dyn_any);
}

// ----------------------------------------------------------------------
// show CORBA::AnySeq
// ----------------------------------------------------------------------

void
CORBA_OUT::display_AnySeq (CORBA2_AnySeq* any_seq)
{
  for (CORBA2(ULong) i=0; i<any_seq->length(); i++) {
      ostr_ << "  ";
      show_Any((*any_seq)[i]);
  }
  ostr_ << endl;
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// global variable
//
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

CORBA2(ORB_var) orb;
DynamicAny::DynAnyFactory_var fac;

CORBA_OUT out (cout);

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// When error
//
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

void
TraceError (const char* expression,
            const char* reason,
            const char* file, int line)
{
  cout << "There is a problem encounted in the test of the DynAny API" << endl;
  cout << "  Expression = " << expression << endl;
  cout << "  Reason     = " << reason << endl;
  cout << "  File       = " << file << endl;
  cout << "  Line       = " << line << endl;
  exit (1);
}

#define DYNANY_DEMO_ERR(EXPRESSION,REASON) \
TraceError(EXPRESSION, REASON, __FILE__, __LINE__)

#define DYNANY_DEMO_TEST(EXPRESSION) \
if ( ! (EXPRESSION) ) DYNANY_DEMO_ERR(#EXPRESSION, "The expression is false");

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// Various tests on a DynAny value
//
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

CORBA2_DynAny_ptr
create_a_void_DynAny ()
{
  return fac->create_dyn_any_from_type_code (CORBA2(_tc_void));
}

// ----------------------------------------------------------------------

CORBA2(Any)*
create_a_void_Any ()
{
  return new CORBA2(Any) (CORBA2(_tc_void), (void*)0, 0); // CORBA2(FALSE));
}

// ----------------------------------------------------------------------

void
Tests_on_DynAny (CORBA2_DynAny_ptr dyn_any, CORBA2(ULong) nb_components)
{
  cout << endl << "----------------------" << endl << endl;

  cout << "Call dyn_any->to_any()" << endl;
  CORBA2(Any_var) any_any = dyn_any->to_any();
  out.display_Any(any_any);  
  
  cout << "Display dyn_any->type()" << endl;
  CORBA2(TypeCode_var) tc_any = dyn_any->type();
  out.display_IDL_TypeCode(tc_any);

  cout << "Display dyn_any->copy()" << endl;
  CORBA2_DynAny_var dyn_copy = dyn_any->copy();
  out.display_DynAny(dyn_copy);

  cout << "Display dyn_copy->type()" << endl;
  CORBA2(TypeCode_var) tc = dyn_copy->type();
  out.display_IDL_TypeCode(tc);

  cout << "Call dyn_copy->to_any()" << endl;
  CORBA2(Any_var) any = dyn_copy->to_any();
  out.display_Any(any);  
  
  cout << "Test dyn_copy->assign(dyn_any)" << endl;
  try {
    dyn_copy->assign(dyn_any);
  } catch (CORBA2_DynAny::TypeMismatch_catch &) {
    DYNANY_DEMO_ERR("dyn_copy->assign(dyn_any)",
		    "A CORBA::DynAny::Invalid exception must not be thrown");
  } catch (...) {
    DYNANY_DEMO_ERR("dyn_copy->assign(dyn_any)",
		    "An unexpected exception has been thrown");
  }
  out.display_DynAny (dyn_copy);

  cout << "Test dyn_copy->assign(dyn_void)" << endl;
  CORBA2_DynAny_var dyn_void = create_a_void_DynAny();
  try {
    dyn_copy->assign(dyn_void);
    DYNANY_DEMO_ERR("dyn_copy->assign(dyn_void)",
		    "Must throw a CORBA::DynAny::Invalid exception");
  } catch (CORBA2_DynAny::TypeMismatch_catch &) {
    cout << "  OK, a CORBA::DynAny::Invalid has been thrown" << endl;
    out.display_DynAny (dyn_copy);
  } catch (...) {
    DYNANY_DEMO_ERR("dyn_copy->assign(dyn_any)",
		    "An unexpected exception has been thrown");
  }

  cout << "Test dyn_copy->from_any(any)" << endl;
  try {
    dyn_copy->from_any(any);
  } catch (CORBA2_DynAny::TypeMismatch_catch &) {
    DYNANY_DEMO_ERR("dyn_copy->from_any(any)",
		    "A CORBA::DynAny::Invalid exception must not be thrown");
  } catch (...) {
    DYNANY_DEMO_ERR("dyn_copy->from_any(any)",
		    "An unexpected exception has been thrown");
  }
  out.display_DynAny (dyn_copy);

  cout << "Test dyn_copy->from_any(any_void)" << endl;
  CORBA2(Any_var) any_void = create_a_void_Any();
  try {
    dyn_copy->from_any(any_void);
    DYNANY_DEMO_ERR("dyn_copy->from_any(any_void)",
		    "Must throw a CORBA::DynAny::Invalid exception");
  } catch (CORBA2_DynAny::TypeMismatch_catch &) {
    cout << "  OK, a CORBA::DynAny::Invalid has been thrown" << endl;
    out.display_DynAny (dyn_copy);
  } catch (...) {
    DYNANY_DEMO_ERR("dyn_copy->from_any(any_void)",
		    "An unexpected exception has been thrown");
  }

  cout << "Count component members = ";
  CORBA2(ULong) component_count = 0;
  do {
    component_count ++;
  } while ( dyn_copy->next() );
  cout << component_count << endl;

  DYNANY_DEMO_TEST(component_count == nb_components);

  cout << "Test dyn_copy->seek(-1)" << endl;
  DYNANY_DEMO_TEST(dyn_copy->seek(-1) == 0 ); // CORBA2(FALSE));

  cout << "Test dyn_copy->seek(index) index is between 0 and "
       << component_count-1 << endl;
  for (CORBA2(Long) i=0; i<CORBA2(Long)(component_count); i++)
    DYNANY_DEMO_TEST( dyn_copy->seek(i) == 1 ); // CORBA2(TRUE) );

  cout << "Test dyn_copy->seek(" << component_count << ")" << endl;
  DYNANY_DEMO_TEST(dyn_copy->seek(component_count) == 0 ); // CORBA2(FALSE));

  cout << "Test dyn_copy->destroy()" << endl;
  dyn_copy->destroy();

}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// Test on Color IDL definition
//
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

CORBA2(TypeCode_ptr)
create_Color_TypeCode ()
{
  CORBA2(EnumMemberSeq) mems;
  mems.length (5);

  mems[0] = CORBA2(string_dup) ("black");
  mems[1] = CORBA2(string_dup) ("white");
  mems[2] = CORBA2(string_dup) ("green");
  mems[3] = CORBA2(string_dup) ("blue");
  mems[4] = CORBA2(string_dup) ("red");

  return orb->create_enum_tc ("IDL:Color:1.0", "Color", mems);
}

// ----------------------------------------------------------------------

void
Tests_on_DynEnum_Color ()
{
  cout << "Creation of an enum Color TypeCode" << endl;
  CORBA2(TypeCode_var) tc = create_Color_TypeCode ();

  cout << "Show the IDL definition of the Color TypeCode" << endl;
  out.display_IDL_TypeCode (tc);

  cout << "Creation of a DynEnum from the Color TypeCode" << endl;
  CORBA2_DynAny_var tmp = fac->create_dyn_any_from_type_code (tc);
  CORBA2_DynEnum_var dyn_enum = DynamicAny::DynEnum::_narrow (tmp);

  cout << "Call dyn_enum->value_as_string(\"green\")" << endl;
  dyn_enum->set_as_string("green");

  cout << "Display DynEnum" << endl;
  out.display_DynAny (dyn_enum);

  cout << "Display dyn_enum->value_as_string() = ";
  CORBA2(String_var) v_string = dyn_enum->get_as_string ();
  cout << v_string.in() << endl;
  DYNANY_DEMO_TEST ( strcmp(v_string,"green") == 0 );

  cout << "Call dyn_enum->value_as_ulong(3)" << endl;
  dyn_enum->set_as_ulong(3);

  cout << "Display DynEnum" << endl;
  out.display_DynAny (dyn_enum);

  cout << "Display dyn_enum->value_as_ulong() = ";
  CORBA2(ULong) v_ulong = dyn_enum->get_as_ulong();
  cout << v_ulong << endl;
  DYNANY_DEMO_TEST ( v_ulong == 3 );

  Tests_on_DynAny (dyn_enum,1);
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// Test on ArrayShort IDL definition
//
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#define ARRAY_SIZE 10

CORBA2(TypeCode_ptr)
create_ArrayShort_TypeCode ()
{
  CORBA2(TypeCode_var) tc = 
                   orb->create_array_tc (ARRAY_SIZE,CORBA2(TypeCode)::_duplicate(CORBA2(_tc_short)));
  return orb->create_alias_tc ("IDL:ArrayShort:1.0", "ArrayShort", tc);
}

// ----------------------------------------------------------------------

void
init_DynArray_ArrayShort (CORBA2_DynArray_ptr dyn_array)
{
  for (CORBA2(Short) i=0; i<ARRAY_SIZE; i++) {
    dyn_array->insert_short (i);
    dyn_array->next();
  }
}

// ----------------------------------------------------------------------

void
inverse_AnySeq (CORBA2_AnySeq* any_seq)
{
  for (CORBA2(ULong) i=0, m=any_seq->length()-1; i<CORBA2(ULong)(m/2); i++) {
      CORBA2(Any) tmp ((*any_seq)[i]);
      (*any_seq)[i] = (*any_seq)[m-i];
      (*any_seq)[m-i] = tmp;
  }
}

// ----------------------------------------------------------------------

void
Tests_on_DynArray_ArrayShort ()
{
  cout << "Creation of an ArrayShort TypeCode" << endl;
  CORBA2(TypeCode_var) tc = create_ArrayShort_TypeCode ();

  cout << "Show IDL definition of the ArrayShort TypeCode" << endl;
  out.display_IDL_TypeCode (tc);

  cout << "Creation of a DynAny from the ArrayShort TypeCode" << endl;
  CORBA2_DynAny_var tmp = fac->create_dyn_any_from_type_code (tc);
  CORBA2_DynArray_var dyn_array = DynamicAny::DynArray::_narrow (tmp);

  cout << "Initialisation of the DynArray containing ArrayShort" << endl;
  init_DynArray_ArrayShort (dyn_array);

  cout << "Show the initialised DynArray" << endl;
  out.display_DynAny(dyn_array);

  Tests_on_DynAny (dyn_array,ARRAY_SIZE);

  cout << "Call dyn_array->get_elements ()" << endl;
  CORBA2_AnySeq_var any_seq = dyn_array->get_elements ();
  out.display_AnySeq (any_seq);

  cout << "Inverse AnySeq" << endl;
  inverse_AnySeq(any_seq);

  cout << "Call dyn_array->set_element(any_seq)" << endl;
  try {
      dyn_array->set_elements (any_seq);
  } catch (CORBA2_DynAny::TypeMismatch_catch &) {
      DYNANY_DEMO_ERR("dyn_array->set_elements (any_seq)",
		      "A CORBA::DynAny::Invalid exception must not be thrown");
  } catch (...) {
      DYNANY_DEMO_ERR("dyn_array->set_elements (any_seq)",
		      "An unexpected exception has been thrown");
  }
 
  cout << "Show the DynArray" << endl;
  out.display_DynAny(dyn_array);

  cout << "Call dyn_array->set_element(any_seq_void)" << endl;
  CORBA2_AnySeq_var any_seq_void = new CORBA2_AnySeq;
  any_seq_void->length(0);
  try {
      dyn_array->set_elements (any_seq_void);
      DYNANY_DEMO_ERR("dyn_array->set_elements (any_seq_void)",
		      "A CORBA::DynAny::Invalid exception must be thrown");
  } catch (CORBA2_DynAny::TypeMismatch_catch &) {
     cout << "  OK, A CORBA::DynAny::InvalidSeq exception has been thrown"
          << endl;
  } catch (...) {
      DYNANY_DEMO_ERR("dyn_array->set_elements (any_seq_void)",
		      "An unexpected exception has been thrown");
  }
 
  cout << "Show the DynArray" << endl;
  out.display_DynAny(dyn_array);

  cout << "Show the  DynArray" << endl;
  out.display_DynAny(dyn_array);

  Tests_on_DynAny (dyn_array,ARRAY_SIZE);
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// Test on SeqShort IDL definition
//
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

CORBA2(TypeCode_ptr)
create_SeqShort_TypeCode ()
{
  CORBA2(TypeCode_var) tc = orb->create_sequence_tc (0,CORBA2(_tc_short));
  return orb->create_alias_tc ("IDL:SeqShort:1.0", "SeqShort", tc);
}

// ----------------------------------------------------------------------

void
init_DynSequence_SeqShort (CORBA2_DynSequence_ptr dyn_sequence)
{
  dyn_sequence->set_length (ARRAY_SIZE);
  for (CORBA2(Short) i=0; i<ARRAY_SIZE; i++) {
    dyn_sequence->insert_short (i);
    dyn_sequence->next();
  }
}

// ----------------------------------------------------------------------

void Tests_on_DynSequence_SeqShort ()
{
  cout << "Creation of a SeqShort TypeCode" << endl;
  CORBA2(TypeCode_var) tc = create_SeqShort_TypeCode ();

  cout << "Show IDL definition of the SeqShort TypeCode" << endl;
  out.display_IDL_TypeCode (tc);

  cout << "Creation of a DynAny from SeqShort TypeCode" << endl;
  CORBA2_DynAny_var tmp = fac->create_dyn_any_from_type_code (tc);
  CORBA2_DynSequence_var dyn_sequence = DynamicAny::DynSequence::_narrow (tmp);

  cout << "Initialisation of the DynSequence containing SeqShort" << endl;
  init_DynSequence_SeqShort (dyn_sequence);

  cout << "Show the initialised DynSequence" << endl;
  out.display_DynAny(dyn_sequence);

  cout << "Call dyn_sequence->get_elements ()" << endl;
  CORBA2_AnySeq_var any_seq = dyn_sequence->get_elements ();
  out.display_AnySeq (any_seq);

  cout << "Inverse the AnySeq" << endl;
  inverse_AnySeq (any_seq);

  cout << "Call dyn_sequence->set_element(any_seq)" << endl;
  try {
      dyn_sequence->set_elements (any_seq);
  } catch (CORBA2_DynAny::TypeMismatch_catch &) {
      DYNANY_DEMO_ERR("dyn_sequence->set_elements (any_seq)",
		      "A CORBA::DynAny::Invalid exception must not be thrown");
  } catch (...) {
      DYNANY_DEMO_ERR("dyn_sequence->set_elements (any_seq)",
		      "An unexpected exception has been thrown");
  }
 
  cout << "Show the DynSequence" << endl;
  out.display_DynAny(dyn_sequence);

#if 0
  // XXX set_elements() can now change the length ...
  cout << "Call dyn_sequence->set_element(any_seq_void)" << endl;
  CORBA2_AnySeq_var any_seq_void = new CORBA2_AnySeq;
  any_seq_void->length(0);
  try {
      dyn_sequence->set_elements (any_seq_void);
      DYNANY_DEMO_ERR("dyn_sequence->set_elements (any_seq_void)",
		      "A CORBA::DynAny::Invalid exception must be thrown");
  } catch (CORBA2_DynAny::InvalidValue_catch &) {
      cout << "  OK, A CORBA::DynAny::InvalidSeq exception has been thrown";
  } catch (...) {
      DYNANY_DEMO_ERR("dyn_sequence->set_elements (any_seq_void)",
		      "An unexpected exception has been thrown");
  }
 
  cout << "Show the DynArray" << endl;
  out.display_DynAny(dyn_sequence);
#endif

  Tests_on_DynAny (dyn_sequence,ARRAY_SIZE);
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// Test on BasicStruct IDL definition
//
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#define STRUCT_MEMBER_COUNT 13

CORBA2(TypeCode_ptr)
create_BasicStruct_TypeCode ()
{
  CORBA2(StructMemberSeq) mems;
  mems.length (STRUCT_MEMBER_COUNT);

  mems[0].name = CORBA2(string_dup) ("m_short");
  mems[0].type = CORBA2(TypeCode)::_duplicate (CORBA2(_tc_short));
  mems[1].name = CORBA2(string_dup) ("m_ushort");
  mems[1].type = CORBA2(TypeCode)::_duplicate (CORBA2(_tc_ushort));
  mems[2].name = CORBA2(string_dup) ("m_long");
  mems[2].type = CORBA2(TypeCode)::_duplicate (CORBA2(_tc_long));
  mems[3].name = CORBA2(string_dup) ("m_ulong");
  mems[3].type = CORBA2(TypeCode)::_duplicate (CORBA2(_tc_ulong));
  mems[4].name = CORBA2(string_dup) ("m_float");
  mems[4].type = CORBA2(TypeCode)::_duplicate (CORBA2(_tc_float));
  mems[5].name = CORBA2(string_dup) ("m_double");
  mems[5].type = CORBA2(TypeCode)::_duplicate (CORBA2(_tc_double));
  mems[6].name = CORBA2(string_dup) ("m_boolean");
  mems[6].type = CORBA2(TypeCode)::_duplicate (CORBA2(_tc_boolean));
  mems[7].name = CORBA2(string_dup) ("m_octet");
  mems[7].type = CORBA2(TypeCode)::_duplicate (CORBA2(_tc_octet));
  mems[8].name = CORBA2(string_dup) ("m_char");
  mems[8].type = CORBA2(TypeCode)::_duplicate (CORBA2(_tc_char));
  mems[9].name = CORBA2(string_dup) ("m_string");
  mems[9].type = CORBA2(TypeCode)::_duplicate (CORBA2(_tc_string));
  mems[10].name = CORBA2(string_dup) ("m_typecode");
  mems[10].type = CORBA2(TypeCode)::_duplicate (CORBA2(_tc_TypeCode));
  mems[11].name = CORBA2(string_dup) ("m_any");
  mems[11].type = CORBA2(TypeCode)::_duplicate (CORBA2(_tc_any));
  mems[12].name = CORBA2(string_dup) ("m_color");
  mems[12].type = create_Color_TypeCode ();

  return orb->create_struct_tc ("IDL:BasicStruct:1.0", "BasicStruct", mems);
}

// ----------------------------------------------------------------------

#define SHORT_VALUE  CORBA2(Short)   (-100)
#define USHORT_VALUE CORBA2(UShort)  (100)
#define LONG_VALUE   CORBA2(Long)    (-9999)
#define ULONG_VALUE  CORBA2(ULong)   (9999)
#define FLOAT_VALUE  CORBA2(Float)   (3.14159)
#define DOUBLE_VALUE CORBA2(Double)  (1.2345e-5)
#define OCTET_VALUE  CORBA2(Octet)   (127)
#define CHAR_VALUE   CORBA2(Char)    ('c')
#define STRING_VALUE (const char*)   "This is a string"
#define ANY_VALUE    (const char*)   "This is a string into an Any"

void
init_DynStruct_BasicStruct (CORBA2_DynAny_ptr dyn_any)
{
  dyn_any->insert_short (SHORT_VALUE);
  dyn_any->next();
  dyn_any->insert_ushort (USHORT_VALUE);
  dyn_any->next();
  dyn_any->insert_long (LONG_VALUE);
  dyn_any->next();
  dyn_any->insert_ulong (ULONG_VALUE);
  dyn_any->next();
  dyn_any->insert_float (FLOAT_VALUE);
  dyn_any->next();
  dyn_any->insert_double (DOUBLE_VALUE);
  dyn_any->next();
  dyn_any->insert_boolean (1) ; // CORBA2(TRUE));
  dyn_any->next();
  dyn_any->insert_octet (OCTET_VALUE);
  dyn_any->next();
  dyn_any->insert_char (CHAR_VALUE);
  dyn_any->next();
  dyn_any->insert_string (STRING_VALUE);
  dyn_any->next();
  dyn_any->insert_typecode (CORBA2(_tc_void));
  dyn_any->next();

  CORBA2(Any) any;
  any <<= ANY_VALUE;
  dyn_any->insert_any (any);
  dyn_any->next();

  CORBA2_DynAny_var current_component = dyn_any->current_component();
  CORBA2_DynEnum_var dyn_enum = CORBA2_DynEnum::_narrow (current_component);
  dyn_enum->set_as_string ("blue");

}

// ----------------------------------------------------------------------

void Tests_on_DynStruct_BasicStruct ()
{
  cout << "Creation of a BasicStruct TypeCode" << endl;
  CORBA2(TypeCode_var) tc = create_BasicStruct_TypeCode ();

  cout << "Show IDL definition of the BasicStruct TypeCode" << endl;
  out.display_IDL_TypeCode (tc);

  cout << "Creation of a DynStruct from the BasicStruct TypeCode" << endl;
  CORBA2_DynAny_var tmp = fac->create_dyn_any_from_type_code (tc);
  CORBA2_DynStruct_var dyn_struct = DynamicAny::DynStruct::_narrow (tmp);

  cout << "Initialisation of the DynStruct containing BasicStruct" << endl;
  init_DynStruct_BasicStruct (dyn_struct);

  cout << "Show the initialised DynStruct" << endl;
  out.display_DynAny(dyn_struct);


  cout << "Test dyn_struct->get_members ()" << endl;
  CORBA2_NameValuePairSeq_var seq = dyn_struct->get_members ();
  for(CORBA2(ULong) i=0; i<seq->length(); i++) {
    cout << "  " << seq[i].id.in() << " = ";
    out.show_Any(seq[i].value);
    cout << endl;
  }

  cout << "Test dyn_struct->set_members (seq)" << endl;
  dyn_struct->set_members (seq);
  out.display_DynAny (dyn_struct);

  Tests_on_DynAny (dyn_struct,STRUCT_MEMBER_COUNT);
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// Test on ComplexStruct IDL definition
//
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#define COMPLEX_STRUCT_MEMBER_COUNT 3

CORBA2(TypeCode_ptr)
create_ComplexStruct_TypeCode ()
{
  CORBA2(StructMemberSeq) mems;
  mems.length (COMPLEX_STRUCT_MEMBER_COUNT);

  mems[0].name = CORBA2(string_dup) ("member1");
  mems[0].type = create_BasicStruct_TypeCode();
  mems[1].name = CORBA2(string_dup) ("member2");
  mems[1].type = create_ArrayShort_TypeCode();
  mems[2].name = CORBA2(string_dup) ("member3");
  mems[2].type = create_SeqShort_TypeCode();

  return orb->create_struct_tc ("IDL:ComplexStruct:1.0", "ComplexStruct",
				mems);
}

// ----------------------------------------------------------------------

void
init_DynStruct_ComplexStruct (CORBA2_DynAny_ptr dyn_any)
{
  CORBA2_DynAny_var current_component;

  current_component = dyn_any->current_component ();
  CORBA2_DynStruct_var member1 =
      CORBA2_DynStruct::_narrow (current_component);
  init_DynStruct_BasicStruct (member1);
  dyn_any->next ();

  current_component = dyn_any->current_component ();
  CORBA2_DynArray_var member2 =
      CORBA2_DynArray::_narrow (current_component);
  init_DynArray_ArrayShort (member2);
  dyn_any->next ();

  current_component = dyn_any->current_component ();
  CORBA2_DynSequence_var member3 =
      CORBA2_DynSequence::_narrow (current_component);
  init_DynSequence_SeqShort (member3);
  dyn_any->next ();
}

// ----------------------------------------------------------------------

void Tests_on_DynStruct_ComplexStruct ()
{
  cout << "Creation of a ComplexStruct TypeCode" << endl;
  CORBA2(TypeCode_var) tc = create_ComplexStruct_TypeCode ();

  cout << "Show IDL definition of the ComplexStruct TypeCode" << endl;
  out.display_IDL_TypeCode (tc);

  cout << "Creation of DynStruct from the ComplexStruct TypeCode" << endl;
  CORBA2_DynAny_var tmp = fac->create_dyn_any_from_type_code (tc);
  CORBA2_DynStruct_var dyn_struct = DynamicAny::DynStruct::_narrow (tmp);

  cout << "Initialisation of the DynStruct containing ComplexStruct" << endl;
  init_DynStruct_ComplexStruct (dyn_struct);

  cout << "Show the initialised DynStruct" << endl;
  out.display_DynAny(dyn_struct);

  Tests_on_DynAny (dyn_struct,COMPLEX_STRUCT_MEMBER_COUNT);
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// Test on Union_UShort IDL definition
//
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#define UNION_MEMBER_COUNT 17

CORBA2(TypeCode_ptr)
create_Union_UShort_TypeCode ()
{
  CORBA2(UnionMemberSeq) mems;  
  mems.length (UNION_MEMBER_COUNT);

  mems[0].name = CORBA2(string_dup) ("m_short");
  mems[0].label <<= CORBA2(UShort) (0);
  mems[0].type = CORBA2(TypeCode)::_duplicate (CORBA2(_tc_short));
  mems[1].name = CORBA2(string_dup) ("m_ushort");
  mems[1].label <<= CORBA2(UShort) (1);
  mems[1].type = CORBA2(TypeCode)::_duplicate (CORBA2(_tc_ushort));
  mems[2].name = CORBA2(string_dup) ("m_long");
  mems[2].label <<= CORBA2(UShort) (2);
  mems[2].type = CORBA2(TypeCode)::_duplicate (CORBA2(_tc_long));
  mems[3].name = CORBA2(string_dup) ("m_ulong");
  mems[3].label <<= CORBA2(UShort) (3);
  mems[3].type = CORBA2(TypeCode)::_duplicate (CORBA2(_tc_ulong));
  mems[4].name = CORBA2(string_dup) ("m_float");
  mems[4].label <<= CORBA2(UShort) (4);
  mems[4].type = CORBA2(TypeCode)::_duplicate (CORBA2(_tc_float));
  mems[5].name = CORBA2(string_dup) ("m_double");
  mems[5].label <<= CORBA2(UShort) (5);
  mems[5].type = CORBA2(TypeCode)::_duplicate (CORBA2(_tc_double));
  mems[6].name = CORBA2(string_dup) ("m_boolean");
  mems[6].label <<= CORBA2(UShort) (6);
  mems[6].type = CORBA2(TypeCode)::_duplicate (CORBA2(_tc_boolean));
  mems[7].name = CORBA2(string_dup) ("m_octet");
  mems[7].label <<= CORBA2(UShort) (7);
  mems[7].type = CORBA2(TypeCode)::_duplicate (CORBA2(_tc_octet));
  mems[8].name = CORBA2(string_dup) ("m_char");
  mems[8].label <<= CORBA2(UShort) (8);
  mems[8].type = CORBA2(TypeCode)::_duplicate (CORBA2(_tc_char));
  mems[9].name = CORBA2(string_dup) ("m_string");
  mems[9].label <<= CORBA2(UShort) (9);
  mems[9].type = CORBA2(TypeCode)::_duplicate (CORBA2(_tc_string));
  mems[10].name = CORBA2(string_dup) ("m_typecode");
  mems[10].label <<= CORBA2(UShort) (10);
  mems[10].type = CORBA2(TypeCode)::_duplicate (CORBA2(_tc_TypeCode));
  mems[11].name = CORBA2(string_dup) ("m_any");
  mems[11].label <<= CORBA2(UShort) (11);
  mems[11].type = CORBA2(TypeCode)::_duplicate (CORBA2(_tc_any));
  mems[12].name = CORBA2(string_dup) ("m_color");
  mems[12].label <<= CORBA2(UShort) (12);
  mems[12].type = create_Color_TypeCode ();
  mems[13].name = CORBA2(string_dup) ("m_array");
  mems[13].label <<= CORBA2(UShort) (13);
  mems[13].type = create_ArrayShort_TypeCode ();
  mems[14].name = CORBA2(string_dup) ("m_sequence");
  mems[14].label <<= CORBA2(UShort) (14);
  mems[14].type = create_SeqShort_TypeCode ();
  mems[15].name = CORBA2(string_dup) ("m_basic_struct");
  mems[15].label <<= CORBA2(UShort) (15);
  mems[15].type = create_BasicStruct_TypeCode ();
  mems[16].name = CORBA2(string_dup) ("m_complex_struct");
  CORBA2(Any)::from_octet from_octet (CORBA2(Octet) (0));
  mems[16].label <<= from_octet;
  mems[16].type = create_ComplexStruct_TypeCode ();

  return orb->create_union_tc ("IDL:Union_UShort:1.0", "Union_UShort",
			       CORBA2(_tc_ushort), mems);
}

// ----------------------------------------------------------------------

void Tests_on_DynUnion ()
{
  cout << "Creation of Union_UShort TypeCode" << endl;
  CORBA2(TypeCode_var) tc = create_Union_UShort_TypeCode ();

  cout << "Show IDL definition of Union_UShort TypeCode" << endl;
  out.display_IDL_TypeCode (tc);

  cout << "Creation of a DynUnion from the Union_UShort TypeCode" << endl;
  CORBA2_DynAny_var tmp = fac->create_dyn_any_from_type_code (tc);
  CORBA2_DynUnion_var dyn_union = DynamicAny::DynUnion::_narrow (tmp);

  cout << "Initialisation of the DynUnion" << endl;
  dyn_union->insert_ushort (0);
  dyn_union->next();
  dyn_union->insert_short (1);
  out.display_DynAny (dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->rewind ();

  dyn_union->insert_ushort (1);
  dyn_union->next();
  dyn_union->insert_ushort (1);
  out.display_DynAny (dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->rewind ();

  dyn_union->insert_ushort (2);
  dyn_union->next();
  dyn_union->insert_long (1);
  out.display_DynAny (dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->rewind ();

  dyn_union->insert_ushort (3);
  dyn_union->next();
  dyn_union->insert_ulong (1);
  out.display_DynAny (dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->rewind ();

  dyn_union->insert_ushort (4);
  dyn_union->next();
  dyn_union->insert_float (1);
  out.display_DynAny (dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->rewind ();

  dyn_union->insert_ushort (5);
  dyn_union->next();
  dyn_union->insert_double (1);
  out.display_DynAny (dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->rewind ();

  dyn_union->insert_ushort (6);
  dyn_union->next();
  dyn_union->insert_boolean (1); // CORBA2(TRUE));
  out.display_DynAny (dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->rewind ();

  dyn_union->insert_ushort (7);
  dyn_union->next();
  dyn_union->insert_octet (1);
  out.display_DynAny (dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->rewind ();

  dyn_union->insert_ushort (8);
  dyn_union->next();
  dyn_union->insert_char ('c');
  out.display_DynAny (dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->rewind ();

  dyn_union->insert_ushort (9);
  dyn_union->next();
  dyn_union->insert_string ("Hello");
  out.display_DynAny (dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->rewind ();

  dyn_union->insert_ushort (10);
  dyn_union->next();
  dyn_union->insert_typecode (CORBA2(_tc_short));
  out.display_DynAny (dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->rewind ();

  dyn_union->insert_ushort (11);
  dyn_union->next();
  CORBA2(Any) any;
  any <<= ANY_VALUE;
  dyn_union->insert_any (any);
  out.display_DynAny (dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->rewind ();

  dyn_union->insert_ushort (12);
  dyn_union->next();
  CORBA2_DynAny_var current_component = dyn_union->current_component ();
  CORBA2_DynEnum_var dyn_enum = CORBA2_DynEnum::_narrow (current_component);
  dyn_enum->set_as_string ("white");
  out.display_DynAny (dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->rewind ();

  dyn_union->insert_ushort (13);
  dyn_union->next();
  current_component = dyn_union->current_component ();
  CORBA2_DynArray_var dyn_array =
      CORBA2_DynArray::_narrow (current_component);
  init_DynArray_ArrayShort (dyn_array);
  out.display_DynAny (dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->rewind ();

  dyn_union->insert_ushort (14);
  dyn_union->next();
  current_component = dyn_union->current_component ();
  CORBA2_DynSequence_var dyn_sequence =
      CORBA2_DynSequence::_narrow (current_component);
  init_DynSequence_SeqShort (dyn_sequence);
  out.display_DynAny (dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->rewind ();

  dyn_union->insert_ushort (15);
  dyn_union->next();
  current_component = dyn_union->current_component ();
  CORBA2_DynStruct_var dyn_struct =
      CORBA2_DynStruct::_narrow (current_component);
  init_DynStruct_BasicStruct (dyn_struct);

  out.display_DynAny (dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->rewind ();

  dyn_union->insert_ushort (100);
  dyn_union->next();
  current_component = dyn_union->current_component ();
  dyn_struct = CORBA2_DynStruct::_narrow (current_component);
  init_DynStruct_ComplexStruct (dyn_struct);
  out.display_DynAny (dyn_union);

  Tests_on_DynAny (dyn_union,2);

  CORBA2_DynAny_var member;

#if 0
  // XXX member_name(char *) does no longer exist
  dyn_union->rewind ();

  dyn_union->member_name ("m_short");
  member = dyn_union->member ();
  member->insert_short(100);
  out.display_DynAny(dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->member_name ("m_ushort");
  //  member = dyn_union->member ();
  //  member->insert_ushort(100);
  // previous lines is standard, next line is another way
  dyn_union->insert_ushort(100);
  out.display_DynAny(dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->member_name ("m_long");
  member = dyn_union->member ();
  member->insert_long(100);
  out.display_DynAny(dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->member_name ("m_ulong");
  member = dyn_union->member ();
  member->insert_ulong(100);
  out.display_DynAny(dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->member_name ("m_float");
  member = dyn_union->member ();
  member->insert_float(100);
  out.display_DynAny(dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->member_name ("m_double");
  member = dyn_union->member ();
  member->insert_double(100);
  out.display_DynAny(dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->member_name ("m_boolean");
  member = dyn_union->member ();
  member->insert_boolean(0); // CORBA2(FALSE));
  out.display_DynAny(dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->member_name ("m_octet");
  member = dyn_union->member ();
  member->insert_octet(100);
  out.display_DynAny(dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->member_name ("m_char");
  member = dyn_union->member ();
  member->insert_char(100);
  out.display_DynAny(dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->member_name ("m_string");
  member = dyn_union->member ();
  member->insert_string("Hello");
  out.display_DynAny(dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->member_name ("m_typecode");
  member = dyn_union->member ();
  member->insert_typecode(CORBA2(_tc_TypeCode));
  out.display_DynAny(dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->member_name ("m_any");
  member = dyn_union->member ();
  any <<= ANY_VALUE;
  member->insert_any(any);
  out.display_DynAny(dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->member_name ("m_color");
  member = dyn_union->member ();
  dyn_enum = CORBA2_DynEnum::_narrow(member);
  dyn_enum->value_as_string("red");
  out.display_DynAny(dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->member_name ("m_array");
  member = dyn_union->member ();
  dyn_array = CORBA2_DynArray::_narrow(member);
  init_DynArray_ArrayShort (dyn_array);
  out.display_DynAny(dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->member_name ("m_sequence");
  member = dyn_union->member ();
  dyn_sequence = CORBA2_DynSequence::_narrow(member);
  init_DynSequence_SeqShort (dyn_sequence);
  out.display_DynAny(dyn_union);

  Tests_on_DynAny (dyn_union,2);

  dyn_union->member_name ("m_basic_struct");
  member = dyn_union->member ();
  dyn_struct = CORBA2_DynStruct::_narrow(member);
  init_DynStruct_BasicStruct (dyn_struct);
  out.display_DynAny(dyn_union);

  dyn_union->member_name ("m_complex_struct");
  member = dyn_union->member ();
  dyn_struct = CORBA2_DynStruct::_narrow(member);
  init_DynStruct_ComplexStruct (dyn_struct);
  out.display_DynAny(dyn_union);
#endif
  
  dyn_union->set_to_default_member (); // CORBA2(TRUE);
  member = dyn_union->member ();
  dyn_struct = CORBA2_DynStruct::_narrow(member);
  init_DynStruct_ComplexStruct (dyn_struct);
  out.display_DynAny(dyn_union);

  Tests_on_DynAny (dyn_union,2);
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// Test on CORBA::InterfaceDef::FullInterfaceDescription IDL definition
//
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

void
Tests_on_FullInterfaceDescription ()
{
  cout << "Show the CORBA::InterfaceDef::FullInterfaceDescription TypeCode" << endl;
  out.display_IDL_TypeCode (CORBA2(InterfaceDef)::_tc_FullInterfaceDescription);

  cout << "Creation of a CORBA::InterfaceDef::FullInterfaceDescription" << endl;

  CORBA2(InterfaceDef)::FullInterfaceDescription description;

  description.name = CORBA2(string_dup) ("OneInterface");
  description.id = CORBA2(string_dup) ("IDL:OneModule/OneInterface:1.0");
  description.defined_in = CORBA2(string_dup) ("IDL:OneModule:1.0");
  description.version = CORBA2(string_dup) ("1.0");
  description.operations.length(0);
  description.attributes.length(0);
  description.base_interfaces.length(0);
  description.type = orb->create_interface_tc(
                        "IDL:OneModule/OneInterface:1.0","OneInterface");

  cout << "Creation of an Any containing it" << endl;
  CORBA2(Any) any;
  any <<= description;

  cout << "Show the Any" << endl;
  out.display_Any(any);

  cout << "Creation of a DynAny from the Any" << endl;
  CORBA2_DynAny_var dyn_any = fac->create_dyn_any (any);

  cout << "Show the DynAny" << endl;
  out.display_DynAny(dyn_any);

  Tests_on_DynAny (dyn_any,
    CORBA2(InterfaceDef)::_tc_FullInterfaceDescription->member_count());
}

void Tests_Alias ()
{
  CORBA2(TypeCode_var) tc = orb->create_alias_tc (
        "IDL:MyShort:1.0", "MyShort", CORBA2(_tc_short));

  cout << "Show IDL definition of Alias TypeCode" << endl;
  out.display_IDL_TypeCode (tc);

  cout << "Creation of a DynAny from the Alias TypeCode" << endl;
  CORBA2_DynAny_var dyn_any = fac->create_dyn_any_from_type_code (tc);

  cout << "Initialisation of the DynAny" << endl;
  dyn_any->insert_short (1);
  out.display_DynAny (dyn_any);

  cout << "Show dyn_any->to_any()" << endl;
  CORBA2(Any_var) any = dyn_any->to_any();
  out.display_Any (any);

  CORBA2(TypeCode_var) any_tc = any->type();
  out.display_IDL_TypeCode (any_tc);
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// OMG specification examples
//
// with some changes to be CORBA compliant :-)
//
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
//
// Page 4-15 of the "ORB Portability" document
//

void the_first_OMG_example ()
{
  CORBA2(StructMemberSeq) mems(2);
  // Next line needed for OmniBroker
  mems.length (2);

  // Here change
  // CORBA2(Any) result;
  CORBA2(Any_var) result;

  CORBA2(Long) value1 = 100;
  CORBA2(Boolean) value2 = 1; // CORBA2(TRUE);

  mems[0].name = CORBA2(string_dup) ("member1");
  mems[0].type = CORBA2(TypeCode)::_duplicate (CORBA2(_tc_long));
  mems[1].name = CORBA2(string_dup) ("member2");
  mems[1].type = CORBA2(TypeCode)::_duplicate (CORBA2(_tc_boolean));

  CORBA2(TypeCode_var) new_tc = orb->create_struct_tc (
				 "IDL:MyStruct:1.0",
				 "MyStruct",
				 mems
				);

  // for display the TypeCode
  out.display_IDL_TypeCode (new_tc);

  // construct the DynStruct object.
  // Values for members have read in the value1 and value2 variables.

  CORBA2_DynAny_var tmp = fac->create_dyn_any_from_type_code (new_tc);
  CORBA2_DynStruct_ptr dyn_struct = DynamicAny::DynStruct::_narrow (tmp);
  dyn_struct->insert_long (value1);
  dyn_struct->next();
  dyn_struct->insert_boolean(value2);

  // for display the DynStruct
  out.display_DynAny (dyn_struct);

  result = dyn_struct->to_any();
  dyn_struct->destroy ();
  CORBA2(release) (dyn_struct);

  // for display the Any
  out.display_Any(result);
}

// ----------------------------------------------------------------------
//
// Page 4-16 of the same document
//

CORBA2(Boolean) eval_filter (const CORBA2(Any) &event)
{
  CORBA2(Boolean) success = 0; // CORBA2(FALSE);

  // First, typecode is extracted from the event.
  // This is necessary to get struct member names:
  CORBA2(TypeCode_var) event_type = event.type();

  // Bad code in the OMG document
  // CORBA2(TypeCode_var) event_type = event->type();

  // The filter only returns true if the event is a struct:
  if (event_type->kind() == CORBA2(tk_struct) ) {
    CORBA2_DynAny_ptr dyn_any = fac->create_dyn_any(event);
    CORBA2_DynStruct_ptr dyn_struct = CORBA2_DynStruct::_narrow (dyn_any);
    CORBA2(release) (dyn_any);

    CORBA2(Boolean) found = 0; // CORBA2(FALSE);

    do {
      CORBA2(String_var) member_name = dyn_struct->current_member_name();
      found = ( strcmp(member_name, "is_urgent") == 0 );
    } while ( !found && !dyn_struct->next() );

    if (found) {
      CORBA2_DynAny_var dyn_member = dyn_struct->current_component ();
      success = dyn_member->get_boolean ();
    }

    dyn_struct->destroy ();
    CORBA2(release) (dyn_struct);
  }
  return success;
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// Test Program
//
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

void line ()
{
  cout << "------------------------------------------------------------"
       << endl;
}

// ----------------------------------------------------------------------

void Run_All_Tests ()
{
  line ();
  cout << "The OMG example" << endl; 
  the_first_OMG_example ();

  line ();
  Tests_on_DynEnum_Color ();

  line ();
  Tests_on_DynStruct_BasicStruct ();

  line ();
  Tests_on_FullInterfaceDescription ();

  line ();
  Tests_on_DynSequence_SeqShort ();

  line ();
  Tests_on_DynArray_ArrayShort ();

  line ();
  Tests_on_DynUnion ();

  line ();
  Tests_on_DynStruct_ComplexStruct ();

  line ();
  Tests_Alias ();

  line ();

  cout << "All tests completed successfully " << endl;
  cout << endl;
}

// ----------------------------------------------------------------------

int main (int argc, char** argv)
{
   // Initialisation
   orb = CORBA2(ORB_init) (argc,argv,"mico-local-orb");
   CORBA2(Object_var) obj = orb->resolve_initial_references ("DynAnyFactory");
   fac = DynamicAny::DynAnyFactory::_narrow(obj);

   out.set_orb (orb);
   out.set_fac (fac);
   
   out.down ();

   Run_All_Tests ();

   return 0;
}

// ----------------------------------------------------------------------
// end of file
// ----------------------------------------------------------------------


