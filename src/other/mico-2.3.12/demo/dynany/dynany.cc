#define MICO_CONF_DYNANY
#include <CORBA-SMALL.h>
#include "test.h"


CORBA::ORB_var orb;
DynamicAny::DynAnyFactory_var fac;

void dyn_union ()
{
    DynamicAny::DynAny_var tmp =
        fac->create_dyn_any_from_type_code (_tc_Union);
    DynamicAny::DynUnion_var da =
        DynamicAny::DynUnion::_narrow (tmp);

    DynamicAny::DynEnum_var disc = DynamicAny::DynEnum::_narrow (
        da->get_discriminator());
    assert (!CORBA::is_nil (disc));

    da->set_to_default_member ();
    assert (disc->get_as_ulong() == 2);

    CORBA::Boolean r = da->seek (1);
    assert (r);
    DynamicAny::DynAny_var value = da->current_component();
    value->insert_short (10);

    r = da->seek (1);
    assert (r);
    disc->set_as_string ("el2");
    da->insert_boolean (TRUE);

    da->destroy ();
}

void dyn_seq ()
{
    DynamicAny::DynAny_var tmp =
        fac->create_dyn_any_from_type_code (_tc_Sequence);
    DynamicAny::DynSequence_var da =
        DynamicAny::DynSequence::_narrow (tmp);

    da->set_length (10);

    for (int i = 0; i < 10; ++i) {
        da->insert_short (i);
        da->next();
    }

    CORBA::AnySeq_var seq = da->get_elements();
    { // sander
        for (int i = 0; i < 10; ++i) {
            CORBA::Short s;
            CORBA::Boolean r = (seq[MICO_ULong(i)] >>= s);
            assert (r);
            assert (s == i);
        }
    } // sander

    da->destroy ();
}

void dyn_enum ()
{
    DynamicAny::DynAny_var tmp =
        fac->create_dyn_any_from_type_code (_tc_Enum);
    DynamicAny::DynEnum_var da =
        DynamicAny::DynEnum::_narrow (tmp);

    da->set_as_string ("el1");
    assert (da->get_as_ulong() == 0);

    da->set_as_ulong (1);
    CORBA::String_var str = da->get_as_string ();
    assert (!strcmp (str, "el2"));

    da->destroy ();
}

void dyn_struct ()
{
    CORBA::Any a;
    Struct s;
    s.l = 10;
    s.s = (const char *)"foobar";
    a <<= s;
    
    DynamicAny::DynAny_var da = fac->create_dyn_any (a);
    CORBA::Long l = da->get_long ();
    assert (l == 10);
    da->next();
    CORBA::String_var str = da->get_string ();
    assert (!strcmp (str, "foobar"));

    CORBA::Any_var av = da->to_any ();
    assert (a == av);

    da->destroy ();
}

void dyn_basic ()
{
    DynamicAny::DynAny_var da =
        fac->create_dyn_any_from_type_code (CORBA::_tc_long);
    da->insert_long (10);

    CORBA::Long l = da->get_long();
    assert (l == 10);

    da->destroy ();
}

int main (int argc, char *argv[])
{
    // ORB initialization
    orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );

    CORBA::Object_var obj =
        orb->resolve_initial_references ("DynAnyFactory");
    fac = DynamicAny::DynAnyFactory::_narrow (obj);

    dyn_basic ();
    dyn_struct ();
    dyn_enum ();
    dyn_seq ();
    dyn_union ();
    return 0;
}
