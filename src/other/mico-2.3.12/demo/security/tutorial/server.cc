/*
 * A Bank factory that creates Account objects
 */

#include <fstream.h>
#include "account.h"


using namespace std;

CORBA::ORB_var orb;
CORBA::Object_var securitymanager;
SecurityLevel2::SecurityManager_var secman;

/*
 * Implementation of the Account
 */

class Account_impl : virtual public POA_Account
{
public:
  Account_impl ();

  void deposit (CORBA::ULong);
  void withdraw (CORBA::ULong);
  CORBA::Long balance ();

private:
  CORBA::Long bal;
};

Account_impl::Account_impl ()
{
  bal = 0;
}

void
Account_impl::deposit (CORBA::ULong amount)
{
  bal += amount;
}

void
Account_impl::withdraw (CORBA::ULong amount)
{
	
  // Get SecurityCurrent
  CORBA::Object_var securitycurrent;
  SecurityLevel2::Current_var seccur;
  securitycurrent = orb->resolve_initial_references ("SecurityCurrent");
  assert (!CORBA::is_nil (securitycurrent));

  seccur = SecurityLevel2::Current::_narrow(securitycurrent);
  assert (!CORBA::is_nil (seccur));

  SecurityLevel2::ReceivedCredentials_var rc = seccur->received_credentials();

  Security::ExtensibleFamily fam;
  fam.family_definer = 0;
  fam.family = 1;
  Security::AttributeType at;
  at.attribute_family = fam;
  at.attribute_type = Security::AccessId;
  Security::AttributeTypeList atl;
  atl.length(1);
  atl[0]=at; 

  Security::AttributeList_var al =  rc->get_attributes( atl );
  cout << "Received  credentials" 
       << (*al).length() << " attributes\n";
  for (CORBA::ULong ctr = 0; ctr < (*al).length(); ctr++) {
    cout  
      << (*al)[ctr].attribute_type.attribute_family.family << " "
      << (*al)[ctr].attribute_type.attribute_type << " "
      <<  &(*al)[ctr].value[0] << " "
      << &(*al)[ctr].defining_authority[0]
      << endl;
  }
  bal -= amount;
}

CORBA::Long
Account_impl::balance ()
{
  return bal;
}

/*
 * Implementation of the Bank
 */

class Bank_impl : virtual public POA_Bank
{
public:
  Account_ptr create ();
};

Account_ptr
Bank_impl::create ()
{
  /*
   * Create a new account (which is never deleted)
   */

  Account_impl * ai = new Account_impl;

  /*
   * Obtain a reference using _this. This implicitely activates the
   * account servant (the RootPOA, which is the object's _default_POA,
   * has the IMPLICIT_ACTIVATION policy)
   */

  Account_ptr aref = ai->_this ();
  assert (!CORBA::is_nil (aref));

  /*
   * Return the reference
   */

  return aref;
}

int
main (int argc, char *argv[])
{
  /*
   * Initialize the ORB
   */
  cout << "Start Bank server\n";
  orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

  /*
   * Obtain a reference to the RootPOA and its Manager
   */
  PortableServer::POA_var poa;
  CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
  poa = PortableServer::POA::_narrow (poaobj);
  PortableServer::POAManager_var mgr = poa->the_POAManager();

  // Get SecurityManager
  securitymanager = orb->resolve_initial_references ("SecurityManager");
  assert (!CORBA::is_nil (securitymanager));

  secman = SecurityLevel2::SecurityManager::_narrow(securitymanager);
  assert (!CORBA::is_nil (secman));

  Security::AuthenticationMethod our_method = (Security::AuthenticationMethod)SecurityLevel2::KeyCertCAPass;
  
  Security::SSLKeyCertCAPass method_struct;
  
  CORBA::Any* any_struct = secman -> get_method_data(our_method);
  *any_struct >>= method_struct;

  method_struct.key = "ServerKey.pem";
  method_struct.cert = "ServerCert.pem";
  method_struct.CAfile = "list.pem";
  method_struct.CAdir = "";
  method_struct.pass = "";

  CORBA::Any* out_any_struct;
  out_any_struct = new CORBA::Any;
  *out_any_struct <<= method_struct;	
  SecurityLevel2::PrincipalAuthenticator_ptr pa = secman -> principal_authenticator();
  
  //	 const char* mechanism = "IDEA-CBC-SHA"; 
  const char* mechanism = ""; 
  const char* security_name = "ssl"; 
  Security::AttributeList privileges; 
  SecurityLevel2::Credentials_ptr creds; 
  CORBA::Any* continuation_data; 
  CORBA::Any* auth_specific_data;

  try
	{
		pa -> authenticate( our_method, mechanism, security_name, *out_any_struct,privileges, creds,continuation_data,auth_specific_data);	
	}
	catch (...)
	{
		cout << "authentication failed" <<endl;
		delete out_any_struct;
		return 0;
	}


  // Test  pa->get_supported_authen_methods

  //getting of own credentials (first from own credentials list)	
 
  SecurityLevel2::Credentials_ptr own_cred;
  own_cred = (*(secman -> own_credentials()))[0];

//we can request own credentials attributes 
  Security::ExtensibleFamily fam1;
  fam1.family_definer = 0;
  fam1.family = 1;
  Security::AttributeType at1;
  at1.attribute_family = fam1;
  at1.attribute_type = Security::AccessId;
  Security::AttributeTypeList atl1;  
  atl1.length(1);
  atl1[0]=at1;


  //getting  the attributes from the Credentials
  Security::AttributeList_var al1 = own_cred->get_attributes(atl1);
  cout << "Own credentials" 
       << (*al1).length() << " attributes\n";
  for (CORBA::ULong ctr = 0; ctr < (*al1).length(); ctr++) {
    cout << "family = "
	 << (*al1)[ctr].attribute_type.attribute_family.family << " "
	 << "type = "
	 << (*al1)[ctr].attribute_type.attribute_type << " "
	 <<  &(*al1)[ctr].value[0] << " "
	 << &(*al1)[ctr].defining_authority[0]
	 << endl;
  }

  /*
   * Create a Bank
   */
	Bank_impl * micocash = new Bank_impl;

  /*
   * Activate the Bank
   */

  PortableServer::ObjectId_var oid = poa->activate_object (micocash);

  /*
   * Write reference to file
   */

  ofstream of ("Bank.ref");
  CORBA::Object_var ref = poa->id_to_reference (oid.in());
  CORBA::String_var str = orb->object_to_string (ref.in());
  of << str.in() << endl;
  of.close ();


  /*
   * Activate the POA and start serving requests
   */
  
  cout << "Running." << endl;
  
  mgr->activate ();
  orb->run();

  /*
   * Shutdown (never reached)
   */

  poa->destroy (TRUE, TRUE);
  delete micocash;

  return 0;
}
