/*
 * A Bank factory that creates Account objects
 */

#include <fstream.h>
#include "account.h"
#include <mico/security/security.h>
#include <mico/security/securitylevel1.h>
#include <mico/security/securitylevel2.h>


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

  Security::CredentialsType cred_type = rc->credentials_type();
  if ( cred_type == Security::SecOwnCredentials )
    cout << "SecOwnCredentials\n";
  if ( cred_type == Security::SecReceivedCredentials )
    cout << "SecReceivedCredentials\n";
  if ( cred_type ==  Security::SecTargetCredentials )
    cout << "SecTargetCredentials\n";

 // Get Security Features
  cout << "Security Features:\n";
  if (rc->get_security_feature(Security::SecDirectionBoth,Security::SecNoDelegation ))
    cout << "SecNoDelegation\n";
  if (rc->get_security_feature(Security::SecDirectionBoth,Security::SecSimpleDelegation ))
    cout << "SecSimpleDelegation\n";
  if (rc->get_security_feature(Security::SecDirectionBoth,Security::SecCompositeDelegation ))
    cout << "SecCompositeDelegation\n";
  if (rc->get_security_feature(Security::SecDirectionBoth,Security::SecNoProtection ))
    cout << "SecNoProtection\n";
  if (rc->get_security_feature(Security::SecDirectionBoth,Security::SecIntegrity))
    cout << "SecIntegrity\n";
  if (rc->get_security_feature(Security::SecDirectionBoth,Security::SecConfidentiality))
    cout << "SecConfidentiality\n";
  if (rc->get_security_feature(Security::SecDirectionBoth,Security::SecIntegrityAndConfidentiality))
    cout << "SecIntegrityAndConfidentiality\n";
  if (rc->get_security_feature(Security::SecDirectionBoth,Security::SecDetectReplay))
    cout << "SecDetectReplay\n";
  if (rc->get_security_feature(Security::SecDirectionBoth,Security::SecDetectMisordering))
    cout << "SecDetectMisordering\n";
  if (rc->get_security_feature(Security::SecDirectionBoth,Security::SecEstablishTrustInTarget))
    cout << "SecEstablishTrustInTarget\n";
  if (rc->get_security_feature(Security::SecDirectionBoth,Security::SecEstablishTrustInClient))
    cout << "SecEstablishTrustInClient\n";


  Security::ExtensibleFamily fam;
  fam.family_definer = 0;
  fam.family = 1;
  Security::AttributeType at;
  at.attribute_family = fam;
  at.attribute_type = Security::AccessId;
  Security::AttributeTypeList atl;
  atl.length(0);
  //  atl[0]=at; 

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
cout << "///////////////////////////////////////////////////////////////////////////\n";  


  SecurityLevel2::CredentialsList* accept_creds = rc -> accepting_credentials();
	SecurityLevel2::Credentials_var cred = (*accept_creds)[0];
	
// get the attributes from the initiating_credentials of tc
  al = cred -> get_attributes(atl);

// and print them
  cout << "Associating credentials " 
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
	
	Security::AssociationOptions opt_used = rc -> association_options_used();
	cout << "Association_options_used (received credentials)\n" ;
  
	
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

  // Check for security service
  // Are the values correct?
  CORBA::ServiceType service_type = CORBA::Security;
  CORBA::ServiceInformation_var service_information = new CORBA::ServiceInformation;
  
  if (orb->get_service_information(service_type, service_information)) {
    cout << "Security Service exists\n";
    CORBA::ULong opt_len = service_information->service_options.length();
    cout << opt_len << " Service Options:= ";
    for ( CORBA::ULong i = 0; i < opt_len; i++) {
      cout << service_information->service_options[i] << " ";
    }
    cout << endl;
    CORBA::ULong det_len = service_information->service_details.length();
    cout << det_len << " Service Details:\n";
    for ( CORBA::ULong i = 0; i < det_len; i++) {
      cout << "Type= " << 
	service_information->service_details[i].service_detail_type << " ";
      for ( CORBA::ULong j = 0; j < service_information->service_details[i].service_detail.length(); j++) {
	  cout << (unsigned char)service_information->service_details[i].service_detail[j];
      }
      cout << endl;
    }
  } else {
    cout << "No Security Service available\n";
    assert(0);
  }
  
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
	
  Security::AuthenticationStatus auth_state = own_cred -> authentication_state();  
  cout << "Authentication State= ";
  if ( auth_state == Security::SecAuthSuccess )
	 cout << "SecAuthSuccess\n";
  if ( auth_state == Security::SecAuthFailure )
	 cout << "SecAuthFailure\n";
  if ( auth_state == Security::SecAuthContinue )
	 cout << "SecAuthContinue\n";
  if ( auth_state == Security::SecAuthExpired )
	 cout << "SecAuthExpired\n";

  Security::CredentialsType cred_type = own_cred->credentials_type();

  if ( cred_type == Security::SecOwnCredentials )
    cout << "SecOwnCredentials\n";
  if ( cred_type == Security::SecReceivedCredentials )
    cout << "SecReceivedCredentials\n";
  if ( cred_type ==  Security::SecTargetCredentials )
    cout << "SecTargetCredentials\n";

  cout << "Mechanism Type= " << own_cred->mechanism() << endl;

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

  // Destroy Credential, OK
  //  own_cred->destroy(); 


  // Copy Credential, OK
  //  SecurityLevel2::Credentials_ptr copied_cred = own_cred->copy();
  //  Security::AttributeList_out al1 = copied_cred->get_attributes(atl1);

  // Get Security Features
  cout << "Security Features:\n";
  if (own_cred->get_security_feature(Security::SecDirectionBoth,Security::SecNoDelegation ))
    cout << "SecNoDelegation\n";
  if (own_cred->get_security_feature(Security::SecDirectionBoth,Security::SecSimpleDelegation ))
    cout << "SecSimpleDelegation\n";
  if (own_cred->get_security_feature(Security::SecDirectionBoth,Security::SecCompositeDelegation ))
    cout << "SecCompositeDelegation\n";
  if (own_cred->get_security_feature(Security::SecDirectionBoth,Security::SecNoProtection ))
    cout << "SecNoProtection\n";
  if (own_cred->get_security_feature(Security::SecDirectionBoth,Security::SecIntegrity))
    cout << "SecIntegrity\n";
  if (own_cred->get_security_feature(Security::SecDirectionBoth,Security::SecConfidentiality))
    cout << "SecConfidentiality\n";
  if (own_cred->get_security_feature(Security::SecDirectionBoth,Security::SecIntegrityAndConfidentiality))
    cout << "SecIntegrityAndConfidentiality\n";
  if (own_cred->get_security_feature(Security::SecDirectionBoth,Security::SecDetectReplay))
    cout << "SecDetectReplay\n";
  if (own_cred->get_security_feature(Security::SecDirectionBoth,Security::SecDetectMisordering))
    cout << "SecDetectMisordering\n";
  if (own_cred->get_security_feature(Security::SecDirectionBoth,Security::SecEstablishTrustInTarget))
    cout << "SecEstablishTrustInTarget\n";
  if (own_cred->get_security_feature(Security::SecDirectionBoth,Security::SecEstablishTrustInClient))
    cout << "SecEstablishTrustInClient\n";



  //getting  the attributes from the Credentials
  Security::AttributeList_var al1 = own_cred->get_attributes(atl1);

//output	
 cout << "Own credentials" 
       << (*al1).length() << " attributes\n";
  for (CORBA::ULong ctr = 0; ctr < (*al1).length(); ctr++) 
    {
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

  CORBA::Object_var policy_current_obj = orb->resolve_initial_references ("PolicyCurrent");
  SecurityLevel2::PolicyCurrent_var policy_current = SecurityLevel2::PolicyCurrent::_narrow(policy_current_obj);
  assert (!CORBA::is_nil (policy_current));

  //PolicyCurrent output
  
  CORBA::PolicyTypeSeq policy_types;
  policy_types.length(0); 
  CORBA::PolicyList * policies = policy_current -> get_policy_overrides(policy_types);
  
  for (CORBA::ULong i=0; i < policies -> length(); i++) {
    CORBA::Policy_ptr policy = (*policies)[i];
    CORBA::PolicyType type = policy -> policy_type();
    if (type == Security::SecMechanismPolicy) {
      cout << "MechanismPolicy" << endl;
      SecurityLevel2::MechanismPolicy_ptr mp  = SecurityLevel2::MechanismPolicy::_narrow(policy);
      Security::MechanismTypeList* mech = mp -> mechanisms();
      for (CORBA::ULong j=0; j < mech -> length(); j++) 
	  cout << (char *)((*mech)[j]) << endl;
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
