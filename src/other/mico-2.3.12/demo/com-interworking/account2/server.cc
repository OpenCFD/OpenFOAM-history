#define MICO_CONF_IMR
#include <CORBA-SMALL.h>
#include <iostream.h>
#include <fstream.h>
#include <unistd.h>
#include "account.h"

// create objects on demand when requested by a bind()
#define OBJECT_CREATION_ON_DEMAND

/*
 * Account implementation
 */

class Account_impl : virtual public Account_skel {
    CORBA::Long _current_balance;
    CORBA::Long _calls;
public:
    Account_impl (const CORBA::BOA::ReferenceData &refdata)
	: Account_skel (refdata)
    {
        _calls = 0;
        _current_balance = 0;
    }

    Account_impl (CORBA::Object_ptr obj)
        : Account_skel (obj)
    {
        cout << "restoring id " << obj->_ident() << endl;
        _calls = 0;
        ifstream inp (obj->_ident());
        assert (inp);
        inp >> _current_balance;

	CORBA::BOA_var boa = _boa();
	CORBA::BOA::ReferenceData_var id = boa->get_id (this);
	CORBA::String_var str = CORBA::ORB::tag_to_string (id);
	cout << "ReferenceData = " << str.in() << endl;
    }

    CORBA::Boolean _save_object ()
    {
        cout << "saving id " << _ident() << endl;
        ofstream out (_ident());
        assert (out);
        out << _current_balance;
	return TRUE;
    }

    void check_exit ()
    {
        if (++_calls == 5) {
            CORBA::BOA_var boa = _boa();
            CORBA::ORB_var orb = _orb();
	    /*
	     * beware: while calling deactivate_impl() we can get
	     * recursive invocations to the Account methods ...
	     */
            boa->deactivate_impl (CORBA::ImplementationDef::_nil());
	    orb->shutdown (TRUE);
        }
    }
  
    void deposit( CORBA::ULong amount )
    {
        _current_balance += amount;
        check_exit ();
    }

    void withdraw( CORBA::ULong amount )
    {
        _current_balance -= amount;
        check_exit ();
    }
    
    CORBA::Long balance()
    {
        check_exit ();
        return _current_balance;
    }
};

/*
 * Account object restorer
 */

class AccountLoader : public CORBA::BOAObjectRestorer {
public:
    CORBA::Boolean restore (CORBA::Object_ptr obj)
    {
        if (!strcmp (obj->_repoid(), "IDL:Account:1.0")) {
            (void)new Account_impl (obj);
            return TRUE;
        }
        cout << "cannot restore " << obj->_repoid() << " objects" << endl;
        return FALSE;
    }
#ifdef OBJECT_CREATION_ON_DEMAND
    CORBA::Boolean bind (const char *repoid, const CORBA::ORB::ObjectTag &tag)
    {
        if (!strcmp (repoid, "IDL:Account:1.0")) {
	    cout << "creating ... " << endl;
	    (void)new Account_impl (tag);
	    return TRUE;
	}
	return FALSE;
    }
#endif
};


int main( int argc, char *argv[] )
{
    cout << "server init" << endl;

    AccountLoader loader;
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );
    CORBA::BOA_var boa = orb->BOA_init (argc, argv, "mico-local-boa");

    if (!boa->restoring()) {
#ifndef OBJECT_CREATION_ON_DEMAND
	cout << "creating ... " << endl;
	CORBA::ORB::ObjectTag_var tag = CORBA::ORB::string_to_tag ("foobar");
	/*
	 * create an account object whose ReferenceData equals "foobar".
	 */
        Account_ptr acc = new Account_impl (tag);
#endif
    }
    boa->impl_is_ready (CORBA::ImplementationDef::_nil());
    orb->run ();

    return 0;
}
