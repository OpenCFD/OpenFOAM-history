/*
 * Demonstrates the various possibilities of inheriting interfaces
 */

#include "inherit.h"


using namespace std;

/*
 * Implementation for the base interface
 */

class Base_impl : virtual public POA_Base
{
public:
  void BaseOp () { cout << "BaseOp (inheritance based)" << endl; }
};

/*
 * Implementation for the base interface, tie based
 */

class Base_Tie_impl
{
public:
  void BaseOp (void) { cout << "BaseOp (tie based)" << endl; }
};

/*
 * Implementation for the derived interface which inherits the
 * base implementation
 */

class Derived_impl_1 :
  virtual public Base_impl,
  virtual public POA_Derived
{
public:
  void DerivedOp () {
    cout << "DerivedOp (inheritance based, impl inheritance)" << endl;
  }
};

/*
 * Implementation for the derived interface which does not inherit
 * the base implementation but provides its own
 */

class Derived_impl_2 :
  virtual public POA_Base,
  virtual public POA_Derived
{
public:
  void BaseOp () {
    cout << "BaseOp in derived interface (inheritance based)" << endl;
  }
  void DerivedOp () {
    cout << "DerivedOp (inheritance based)" << endl;
  }
};

/*
 * Tie-based implementation of the derived interface which inherits
 * the base implementation
 */

class Derived_Tie_impl_1 :
  virtual public Base_Tie_impl
{
public:
  void DerivedOp () {
    cout << "DerivedOp (tie based, impl inheritance)" << endl;
  }
};

/*
 * Tie-based implementation of the derived interface which does not
 * inherit the base implementation but provides its own
 */

class Derived_Tie_impl_2
{
public:
  void BaseOp () {
    cout << "BaseOp in derived interface (tie based)" << endl;
  }
  void DerivedOp () {
    cout << "DerivedOp (tie based)" << endl;
  }
};

int
main (int argc, char *argv[])
{
  /*
   * Initialize the ORB
   */

  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

  /*
   * Obtain a reference to the RootPOA and its Manager
   */

  CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
  PortableServer::POAManager_var mgr = poa->the_POAManager();

  /*
   * The RootPOA has the SYSTEM_ID policy, but we want to assign our
   * own IDs, so create a new POA with the USER_ID policy
   */

  CORBA::PolicyList pl;
  pl.length(1);
  pl[0] = poa->create_id_assignment_policy (PortableServer::USER_ID);
  PortableServer::POA_var mypoa = poa->create_POA ("MyPOA", mgr, pl);

  /*
   * Create inheritance based Servants
   */

  Base_impl bi;
  Derived_impl_1 di1;
  Derived_impl_2 di2;

  /*
   * Create tie based Servants
   */

  Base_Tie_impl _bti;
  Derived_Tie_impl_1 _dti1;
  Derived_Tie_impl_2 _dti2;

  POA_Base_tie<Base_Tie_impl> bti (_bti);
  POA_Derived_tie<Derived_Tie_impl_1> dti1 (_dti1);
  POA_Derived_tie<Derived_Tie_impl_2> dti2 (_dti2);

  /*
   * Activate 'em
   */

  PortableServer::ObjectId_var bi_id, di1_id, di2_id;
  PortableServer::ObjectId_var bti_id, dti1_id, dti2_id;

  bi_id   = PortableServer::string_to_ObjectId ("Base_impl");
  bti_id  = PortableServer::string_to_ObjectId ("Base_Tie_impl");
  di1_id  = PortableServer::string_to_ObjectId ("Derived_impl_1");
  di2_id  = PortableServer::string_to_ObjectId ("Derived_impl_2");
  dti1_id = PortableServer::string_to_ObjectId ("Derived_Tie_impl_1");
  dti2_id = PortableServer::string_to_ObjectId ("Derived_Tie_impl_2");

  mypoa->activate_object_with_id (*bi_id, &bi);
  mypoa->activate_object_with_id (*bti_id, &bti);
  mypoa->activate_object_with_id (*di1_id, &di1);
  mypoa->activate_object_with_id (*di2_id, &di2);
  mypoa->activate_object_with_id (*dti1_id, &dti1);
  mypoa->activate_object_with_id (*dti2_id, &dti2);

  cout << "Running." << endl;

  mgr->activate ();
  orb->run();

  /*
   * Shutdown (never reached)
   */
  
  poa->destroy (TRUE, TRUE);

  return 0;
}
