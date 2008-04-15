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
  // The SecurityManager contains among others the Credentials
  securitymanager = orb->resolve_initial_references ("SecurityManager");
  secman = SecurityLevel2::SecurityManager::_narrow(securitymanager);
  assert (!CORBA::is_nil (secman));
  ////////////////////////////////////////////////////////////////////////////////////////////
  //Mechanisms output
  //to display all supported mechanisms availiable for orb	
  Security::MechandOptionsList* p_mech_list = secman -> supported_mechanisms();
  Security::MechandOptions mech_opt;
  
  cout << "Mechanisms" << endl;
  for (CORBA::ULong i=0; i < p_mech_list -> length(); i++) {
    mech_opt = (*p_mech_list)[i];
    cout << (char *)mech_opt.mechanism_type << endl;
  }	
  
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
  cout << "///////////////////////////////////////////////////////////////////////////\n";
  
  ////////////////////////////////////////////////////////////////////////////////////////  
  //authentiction process - Security Manager authenticates itself	by principal authenticator
  //and sets own_credentials (it occurs within authenticate(...) method)	
  
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
///////////////////////////////////////////////////////////////////////////////////////// 
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

  //output	
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
  cout << "///////////////////////////////////////////////////////////////////////////\n";
  ///////////////////////////////////////////////////////////////////////////////////////// 
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

  ///////////////////////////////////////////////////////////////////////////////////////// 
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
  SecurityLevel2::CredentialsList* init_creds = tc -> initiating_credentials();
  SecurityLevel2::Credentials_var cred = (*init_creds)[0];
	
  // get the attributes from the initiating_credentials of tc
  Security::AttributeList_var al = cred -> get_attributes(atl);

  // and print them
  cout << "Initiating credentials " 
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
  cout << "///////////////////////////////////////////////////////////////////////////\n";
  Security::AssociationOptions opt_used = tc -> association_options_used();
  cout << "Association_options_used (target credentials)\n" ;
  
  
  if (Security::NoDelegation == (opt_used & Security::NoDelegation))
    cout << "NoDelegaion\n";
  if (Security::SimpleDelegation == (opt_used & Security::SimpleDelegation))
    cout << "SimpleDelegation\n";
  if (Security::CompositeDelegation == (opt_used & Security::CompositeDelegation))
    cout << "CompositeDelegation\n";
  if (Security::NoProtection == (opt_used & Security::NoProtection))
    cout << "NoProtection\n";
  if (Security::Integrity == (opt_used & Security::Integrity))
    cout << "Integrity\n";
  if (Security::Confidentiality == (opt_used & Security::Confidentiality))
    cout << "Confidentiality\n";
  if (Security::DetectReplay == (opt_used & Security::DetectReplay))
    cout << "DetectReplay\n";
  if (Security::DetectMisordering == (opt_used & Security::DetectMisordering))
    cout << "DetectMisordering\n";
  if (Security::EstablishTrustInTarget == (opt_used & Security::EstablishTrustInTarget))
    cout << "EstablishTrustInTarget\n";
  if (Security::EstablishTrustInClient == (opt_used & Security::EstablishTrustInClient))
    cout << "EstablishTrustInClient\n";	

  cout << "///////////////////////////////////////////////////////////////////////////\n";
  ///////////////////////////////////////////////////////////////////////////////////////// 


  //PolicyCurrent output
  cout << "PolicyCurrent" << endl;
  CORBA::Object_var policy_current_obj = orb->resolve_initial_references ("PolicyCurrent");
  SecurityLevel2::PolicyCurrent_var policy_current = SecurityLevel2::PolicyCurrent::_narrow(policy_current_obj);
  assert (!CORBA::is_nil (policy_current));
  
  CORBA::PolicyTypeSeq policy_types;
  policy_types.length(0); 
  CORBA::PolicyList_var  policies = policy_current -> get_policy_overrides(policy_types);
  
  for (CORBA::ULong i=0; i < policies -> length(); i++) {
    CORBA::Policy_ptr policy = (*policies)[i];
    CORBA::PolicyType type = policy -> policy_type();
    if (type == Security::SecMechanismPolicy) {
	cout << "MechanismPolicy" << endl;
	SecurityLevel2::MechanismPolicy_ptr mp  = SecurityLevel2::MechanismPolicy::_narrow(policy);
	Security::MechanismTypeList* mech = mp -> mechanisms();
	for (CORBA::ULong i=0; i < mech -> length(); i++) 
	    cout << (char *)((*mech)[i]) << endl;
	continue;
      } 
    if (type == Security::SecInvocationCredentialsPolicy){
	cout << "InvocationCredentialsPolicy" << endl;
	SecurityLevel2::InvocationCredentialsPolicy_ptr icp  = SecurityLevel2::InvocationCredentialsPolicy::_narrow(policy);
	SecurityLevel2::CredentialsList* cred =  icp -> creds();
	int len = cred -> length();
	for (int i = 0; i < len; i++) {
	  SecurityLevel2::Credentials_ptr own_cred;
	  own_cred = (*cred)[i];
	  
	  Security::ExtensibleFamily fam1;
	  fam1.family_definer = 0;
	  fam1.family = 1;
	  Security::AttributeType at1;
	  at1.attribute_family = fam1;
	  at1.attribute_type = Security::AccessId;
	  Security::AttributeTypeList atl1;
	  atl1.length(1);
	  atl1[0]=at1;
	  Security::AttributeList_var al1 = own_cred->get_attributes(atl1);
	  cout << "Own " 
	       << (*al1).length() << " attributes\n";
	  
	  for (CORBA::ULong ctr = 0; ctr < (*al1).length(); ctr++) {
	    cout  
	      << (*al1)[ctr].attribute_type.attribute_family.family << " "
	      << (*al1)[ctr].attribute_type.attribute_type << " "
	      <<  &(*al1)[ctr].value[0] << " "
	      << &(*al1)[ctr].defining_authority[0]
	      << endl;
	  }
	}
	continue;
      }	if (type == Security::SecQOPPolicy) {	
	  cout << "QOPPolicy" << endl;
	  continue;
      } 
    if (type == Security::SecDelegationDirectivePolicy) {
	cout << "DelegationDirectivePolicy" << endl;
	continue;
      } 
    if (type == Security::SecEstablishTrustPolicy) {
      SecurityLevel2::EstablishTrustPolicy_ptr etp  = SecurityLevel2::EstablishTrustPolicy::_narrow(policy);
      if (etp -> trust().trust_in_client)
	  cout << "EstablishTrustPolicy - trust in client" << endl;
      else 
	  cout << "EstablishTrustPolicy - trust in target" << endl;
      continue;
    } 
    cout << "Invalid type!!" << endl;
  }	
  cout << "///////////////////////////////////////////////////////////////////////////\n";

  policy_current -> set_policy_overrides(policies,CORBA::SET_OVERRIDE);  


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

  // SecurityPolicies on IOR are not supported yet.
  //  CORBA::Policy_var ior_policy = account->_get_policy(Security::SecTargetInvocationAccess);

  cout << "deposit - 700\n";
  account->deposit (700);
  cout << "withdraw - 450\n";
  account->withdraw (450);
  cout << "Balance is " << account->balance () << "." << endl;
  return 0;
}
