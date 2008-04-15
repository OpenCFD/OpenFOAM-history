#include <iostream.h>
#include <fstream.h>
#include <unistd.h>
#include <string.h>
#include "Bank.h"
#include "Bank_impl.h"

Account_impl::Account_impl()
{
#if DEBUG
  cout << "  <Account_impl> constructor: " << _current_balance << "\n";
#endif
  _calls = 0;
  _current_balance = 0;
  account_name = CORBA::string_dup ("");
#if DEBUG
  cout << "  <Account_impl> constructor end: " << _current_balance << "\n";
#endif
}

Account_impl::Account_impl (CORBA::Object_ptr obj)
  : Account_skel (obj)
{
  //  cout << "restoring id " << obj->_ident() << endl;
  _calls = 0;
  account_name = new char[100];
  ifstream inp (obj->_ident());
  assert (inp);
  inp >> account_name;
  inp >> _current_balance;
}

CORBA::Boolean 
Account_impl::_save_object ()
{
  //        cout << "saving id " << _ident() << endl;
  ofstream out (_ident());
  assert (out);
  out << account_name << "\n";
  out << _current_balance << "\n";
  return TRUE;
}

char*
Account_impl::name ()
{
  return CORBA::string_dup (account_name);
}

void 
Account_impl::name (const char* s)
{
  account_name = CORBA::string_dup (s);
}

void 
Account_impl::deposit( CORBA::ULong amount )
{
  _current_balance += amount;
}

void 
Account_impl::withdraw( CORBA::ULong amount )
{
  _current_balance -= amount;
}

void 
Account_impl::set (CORBA::ULong amount)
{
  _current_balance = amount;
}
    
CORBA::Long 
Account_impl::balance()
{
  return _current_balance;
}

void
Account_impl::destroy()
{
  CORBA::BOA_var boa = _boa();
  CORBA::ORB_var orb = _orb();

  boa->deactivate_obj (this);
  orb->shutdown (TRUE);
}


Bank_impl::Bank_impl ()
{
  bank_name = CORBA::string_dup ("NoName Bank");
}


Account_ptr 
Bank_impl::create ()
{
  CORBA::BOA_var boa = _boa();
  CORBA::ORB_ptr orb = _orb();
  
  /*
   * create new account and deactivate to move it to a separate
   * server.
   */
  //Account_ptr acc = new Account_impl;
  Account_impl* tmp_acc = new Account_impl;
  // some my tests...
  //tmp_acc->_current_balance = 0;
  
  // end of my tests
  Account_ptr acc = tmp_acc;

  boa->deactivate_obj (acc);
  
  /*
   * now create a stub for the object so we can still access
   * it from within this process (if you do not need access
   * to the newly created account from within this process
   * you could just return Account::_duplicate (acc) here).
   */
  CORBA::String_var ref = orb->object_to_string (acc);
  CORBA::release (acc);
  CORBA::Object_var obj = orb->string_to_object (ref);
  acc = Account::_narrow (obj);
  /*
   * now acc is a stub for the account object in the other server.
   * save the stub so we can access it later.
   */
  accounts.length (accounts.length()+1);
  accounts[accounts.length()-1] = acc;
  
  return Account::_duplicate (acc);
}

void 
Bank_impl::deposit_all_accounts (CORBA::ULong amount)
{
  for (CORBA::ULong i = 0; i < accounts.length(); ++i)
    accounts[i]->deposit (amount);
}

void 
Bank_impl::destroy()
{
  CORBA::BOA_var boa = _boa();
  CORBA::ORB_var orb = _orb();
  
  boa->deactivate_obj (this);
  orb->shutdown (TRUE);
}

char*
Bank_impl::name ()
{
  return CORBA::string_dup (bank_name);
}


void
Bank_impl::name (const char* value)
{
  bank_name = CORBA::string_dup (value);
}

CORBA::Boolean 
AccountLoader::restore (CORBA::Object_ptr obj)
{
  if (!strcmp (obj->_repoid(), "IDL:Account:1.0")) {
    new Account_impl (obj);
    return TRUE;
  }
  /*
   * Bank objects are transient because _save_object() is not
   * implementent for them. so we won't be asked to restore them
   */
  cout << "cannot restore " << obj->_repoid() << " objects" << endl;
  return FALSE;
}

