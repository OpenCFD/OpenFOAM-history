#include "account.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif


using namespace std;

CORBA::ORB_var orb;
CORBA::Object_var securitymanager;
SecurityLevel2::SecurityManager_var secman;

int
main (int argc, char *argv[]) {
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

  // Get SecurityManager

  securitymanager = orb->resolve_initial_references ("SecurityManager");
  secman = SecurityLevel2::SecurityManager::_narrow(securitymanager);
  assert (!CORBA::is_nil (secman));
  
  //setting of method (it is const) 
  Security::AuthenticationMethod our_method = (Security::AuthenticationMethod)SecurityLevel2::KeyCertCAPass;
  
  //loading data for our method  
  Security::SSLKeyCertCAPass *method_struct;
  method_struct = new Security::SSLKeyCertCAPass;
  CORBA::Any* any_struct = secman -> get_method_data(our_method);
  *any_struct >>= *method_struct;
  method_struct -> key = "ClientKey.pem";
  method_struct -> cert = "ClientCert.pem";
  method_struct -> CAfile = "";
  method_struct -> CAdir = "";
  method_struct -> pass = "";
  
  CORBA::Any* out_any_struct;
  out_any_struct = new CORBA::Any;
  *out_any_struct <<= *method_struct;	
  
  //authentiction process - client authenticates itself  by 
  // principal authenticator and sets own_credentials
  
  SecurityLevel2::PrincipalAuthenticator_ptr pa = secman -> principal_authenticator();
  //const char* mechanism = ""; 
  const char* security_name = "ssl"; 
  Security::AttributeList privileges; 
  SecurityLevel2::Credentials_ptr creds; 
  CORBA::Any* continuation_data; 
  CORBA::Any* auth_specific_data;
  
  try
    {
      pa -> authenticate( our_method, "", security_name, *out_any_struct,privileges, creds,continuation_data,auth_specific_data);	
    }
  catch (...)
    {
      cout << "authentication failed" <<endl;
      delete out_any_struct;
      return 0;
    }
    
  delete out_any_struct;
  
  //getting of own credentials (first from own credentials list)	
  
  SecurityLevel2::Credentials_var own_cred;
  own_cred = (*(secman -> own_credentials()))[0];
	
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
    cout  
      << "family = "
      << (*al1)[ctr].attribute_type.attribute_family.family << " "
      << "type = "
      << (*al1)[ctr].attribute_type.attribute_type << " "
      <<  &(*al1)[ctr].value[0] << " "
      << &(*al1)[ctr].defining_authority[0]
      << endl;
  }
  // Connect to the Bank

  /*
   * IOR is in Bank.ref in the local directory
   */
  
  char pwd[256], uri[300];
  sprintf (uri, "file://%s/Bank.ref", getcwd(pwd, 256));

  /*
   * Connect to the Bank
   */

  CORBA::Object_var obj = orb->string_to_object (uri);
  Bank_var bank = Bank::_narrow (obj);
  assert (!CORBA::is_nil (bank));

  // Get the Credentials
  // The Credentials contains the security attributes of the server, but not
  // directly. Normally they are just a reference to a object specific to 
  // the security mechanism.

  //getting target credentials
  //Get and print attributes of server
  Security::ExtensibleFamily fam2;
  fam2.family_definer = 0;
  fam2.family = 1;
  Security::AttributeType at;
  at.attribute_family = fam2;
  at.attribute_type = Security::AccessId;
  Security::AttributeTypeList atl;
  atl.length(2);
  atl[0]=at;
  Security::AttributeType at2;
  fam2.family = 11;
  at2.attribute_family = fam2;
  at2.attribute_type = Security::PeerAddress;
  atl[1]=at2;

  SecurityLevel2::TargetCredentials_var tc = secman->get_target_credentials(bank);
  // get the server attributes
  Security::AttributeList_var al = tc -> get_attributes(atl);

  cout << "Server credentials " 
       << (*al).length() << " attributes\n";
  
  for (CORBA::ULong ctr = 0; ctr < (*al).length(); ctr++) {
    cout  
      << "family = "
      << (*al)[ctr].attribute_type.attribute_family.family << " "
      << "type = "
      << (*al)[ctr].attribute_type.attribute_type << " "
      <<  &(*al)[ctr].value[0] << " "
      << &(*al)[ctr].defining_authority[0]
      << endl;
  }

  //
  // Open an account
  //

  Account_var account = bank->create ();
  if (CORBA::is_nil (account)) {
    cout << "oops: account is nil" << endl;
    exit (1);
  }
  
  //
  // Deposit and withdraw some money
  //

  cout << "deposit - 700\n";
  account->deposit (700);
  cout << "withdraw - 450\n";
  account->withdraw (450);
  cout << "Balance is " << account->balance () << "." << endl;
  return 0;
}
