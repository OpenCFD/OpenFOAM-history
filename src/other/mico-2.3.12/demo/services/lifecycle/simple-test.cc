/*
 *  Test of COSS Relationship Service for MICO
 *  Copyright (C) 1998-99 Karel Gardas
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  Send comments and/or bug reports to:
 *                 mico@informatik.uni-frankfurt.de
 *  or to my private e-mail:
 *                 gardask@alpha.inf.upol.cz
 */

#include <CORBA.h>
#include <time.h>
#include <string.h>

#include <coss/CosRelationships.h>
#include <coss/CosCompoundLifeCycle.h>
#include <coss/CosLifeCycleContainment.h>
#include <coss/CosLifeCycleReference.h>
// #include <coss/CosGraphsExtension.h>
#include "Documents.h"
#include "Test.h"

int main( int argc, char *argv[] )
{
  //ORB initialization
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

  cout << "Runing all tests\n" << flush;

  CORBA::Boolean exception = FALSE;
  CORBA::Boolean exception2 = FALSE;
  CORBA::Boolean exception3 = FALSE;
  CORBA::Boolean state_good = TRUE;
  state_good = TRUE;

  // containment
  cout << "Binding to LifeCycleContainsRoleFactory ..." << flush;
  CORBA::ORB::ObjectTag_var 
    tag2 = CORBA::ORB::string_to_tag ("LifeCycleContainsRole_impl");
  CORBA::Object_var 
  obj10 = orb->bind ("IDL:omg.org/CosRelationships/RoleFactory:1.0", 
		     tag2.in());
  assert (!CORBA::is_nil (obj10));
  CosRelationships::RoleFactory_var contains_factory; 
  if (contains_factory = CosRelationships::RoleFactory::_narrow (obj10))
    cout << "done.\n" << flush;
  else
    cout << "ERROR!\n" << flush;

  cout << "Binding to LifeCycleContainedInRoleFactory ..." << flush;
  CORBA::ORB::ObjectTag_var 
    tag3 = CORBA::ORB::string_to_tag ("LifeCycleContainedInRole_impl");
  CORBA::Object_var 
    obj11 = orb->bind ("IDL:omg.org/CosRelationships/RoleFactory:1.0", 
                       tag3.in());
  assert (!CORBA::is_nil (obj11));
  CosRelationships::RoleFactory_var containedin_factory; 
  if (containedin_factory = CosRelationships::RoleFactory::_narrow (obj11))
    cout << "done.\n" << flush;
  else
    cout << "ERROR!\n" << flush;
  

  cout << "Binding to LifeCycleContainmentFactory ..." << flush;
  CORBA::ORB::ObjectTag_var 
    tag11 = CORBA::ORB::string_to_tag ("LifeCycleContainment_impl");
  CORBA::Object_var 
    obj30 = orb->bind ("IDL:omg.org/CosRelationships/RelationshipFactory:1.0",
                       tag11.in());
  assert (!CORBA::is_nil (obj30));
  CosRelationships::RelationshipFactory_var containment_factory;
  if (containment_factory = CosRelationships::RelationshipFactory::_narrow 
      (obj30))
    cout << "done.\n" << flush;
  else
    cout << "ERROR!\n" << flush;
   

  // reference
  cout << "Binding to LifeCycleReferencesRoleFactory ..." << flush;
  //CORBA::ORB::ObjectTag_var 
  tag2 = CORBA::ORB::string_to_tag ("LifeCycleReferencesRole_impl");
  //CORBA::Object_var
  obj10 = orb->bind ("IDL:omg.org/CosRelationships/RoleFactory:1.0", 
		     tag2.in());
  assert (!CORBA::is_nil (obj10));
  CosRelationships::RoleFactory_var references_factory; 
  if (references_factory = CosRelationships::RoleFactory::_narrow (obj10))
    cout << "done.\n" << flush;
  else
    cout << "ERROR!\n" << flush;

  cout << "Binding to LifeCycleReferencedByRoleFactory ..." << flush;
  //CORBA::ORB::ObjectTag_var 
  tag3 = CORBA::ORB::string_to_tag ("LifeCycleReferencedByRole_impl");
  //CORBA::Object_var 
  obj11 = orb->bind ("IDL:omg.org/CosRelationships/RoleFactory:1.0", 
		     tag3.in());
  assert (!CORBA::is_nil (obj11));
  CosRelationships::RoleFactory_var referencedby_factory; 
  if (referencedby_factory = CosRelationships::RoleFactory::_narrow (obj11))
    cout << "done.\n" << flush;
  else
    cout << "ERROR!\n" << flush;
  

  cout << "Binding to LifeCycleReferenceFactory ..." << flush;
  //CORBA::ORB::ObjectTag_var 
  tag11 = CORBA::ORB::string_to_tag ("LifeCycleReference_impl");
  //CORBA::Object_var 
  obj30 = orb->bind ("IDL:omg.org/CosRelationships/RelationshipFactory:1.0", 
		     tag11.in());
  assert (!CORBA::is_nil (obj30));
  CosRelationships::RelationshipFactory_var reference_factory;
  if (reference_factory = CosRelationships::RelationshipFactory::_narrow
      (obj30))
    cout << "done.\n" << flush;
  else
    cout << "ERROR!\n" << flush;

  
  // node factory
  cout << "Binding to CosCompoundLifeCycleNodeFactory ..." << flush;
  //CORBA::ORB::ObjectTag_var 
  tag11 = CORBA::ORB::string_to_tag ("CosCompoundLifeCycle_Node_impl");
  //CORBA::Object_var 
  obj30 = orb->bind ("IDL:omg.org/CosGraphs/NodeFactory:1.0", 
		     tag11.in());
  assert (!CORBA::is_nil (obj30));
  CosGraphs::NodeFactory_var node_factory;
  if (node_factory = CosGraphs::NodeFactory::_narrow (obj30))
    cout << "done.\n" << flush;
  else
    cout << "ERROR!\n" << flush;

  // figure and text factory
  cout << "Binding to TextFactory ..." << flush;
  //CORBA::Object_var 
  obj30 = orb->bind ("IDL:Documents/TextFactory:1.0");
  assert (!CORBA::is_nil (obj30));
  Documents::TextFactory_var text_factory;
  if (text_factory = Documents::TextFactory::_narrow (obj30))
    cout << "done.\n" << flush;
  else
    cout << "ERROR!\n" << flush;

  cout << "Binding to FigureFactory ..." << flush;
  obj30 = orb->bind ("IDL:Documents/FigureFactory:1.0");
  assert (!CORBA::is_nil (obj30));
  Documents::FigureFactory_var figure_factory;
  if (figure_factory = Documents::FigureFactory::_narrow (obj30))
    cout << "done.\n" << flush;
  else
    cout << "ERROR!\n" << flush;
  

  // binding to factories which are need for tests

  cout << "Binding to BaseRoleFactory ..." << flush;
  //CORBA::ORB::ObjectTag_var 
  tag2 = CORBA::ORB::string_to_tag ("BaseRole_impl");
  //CORBA::Object_var 
  obj10 = orb->bind ("IDL:omg.org/CosRelationships/RoleFactory:1.0", 
		     tag2.in());
  assert (!CORBA::is_nil (obj10));
  CosRelationships::RoleFactory_var base_factory; 
  if (base_factory = CosRelationships::RoleFactory::_narrow (obj10))
    cout << "done.\n" << flush;
  else
    cout << "ERROR!\n" << flush;

  cout << "Binding to DerivedRoleFactory 1..." << flush;
  //CORBA::ORB::ObjectTag_var 
  tag3 = CORBA::ORB::string_to_tag ("DerivedRole1_impl");
  //CORBA::Object_var 
  obj11 = orb->bind ("IDL:omg.org/CosRelationships/RoleFactory:1.0", 
		     tag3.in());
  assert (!CORBA::is_nil (obj11));
  CosRelationships::RoleFactory_var derived_factory1; 
  if (derived_factory1 = CosRelationships::RoleFactory::_narrow (obj11))
    cout << "done.\n" << flush;
  else
    cout << "ERROR!\n" << flush;
  
  cout << "Binding to DerivedRoleFactory 2..." << flush;
  //CORBA::ORB::ObjectTag_var 
  tag3 = CORBA::ORB::string_to_tag ("DerivedRole2_impl");
  //CORBA::Object_var 
  obj11 = orb->bind ("IDL:omg.org/CosRelationships/RoleFactory:1.0", 
		     tag3.in());
  assert (!CORBA::is_nil (obj11));
  CosRelationships::RoleFactory_var derived_factory2; 
  if (derived_factory2 = CosRelationships::RoleFactory::_narrow (obj11))
    cout << "done.\n" << flush;
  else
    cout << "ERROR!\n" << flush;
  

  cout << "Binding to TestRelationshipFactory ..." << flush;
  //CORBA::ORB::ObjectTag_var 
  tag11 = CORBA::ORB::string_to_tag ("TestRelationship_impl");
  //CORBA::Object_var 
  obj30 = orb->bind ("IDL:omg.org/CosRelationships/RelationshipFactory:1.0",
		     tag11.in());
  assert (!CORBA::is_nil (obj30));
  CosRelationships::RelationshipFactory_var test_relationship_factory;
  if (test_relationship_factory = CosRelationships::RelationshipFactory::
      _narrow (obj30))
    cout << "done.\n" << flush;
  else
    cout << "ERROR!\n" << flush;   


  cout << "Binding to TraversalFactory ..." << flush;
  CORBA::Object_var obj3 = orb->bind ("IDL:omg.org/CosGraphs/TraversalFactory:1.0");
  assert (!CORBA::is_nil (obj3));
  CosGraphs::TraversalFactory_var traversal_factory;
  if (traversal_factory = CosGraphs::TraversalFactory::_narrow (obj3))
    cout << "done.\n" << flush;
  else
    cout << "ERROR!\n" << flush;


  cout << "Binding to GenericCriteriaFactory ..." << flush;
  obj3 = orb->bind 
    ("IDL:omg.org/CosCompoundLifeCycle/PropagationCriteriaFactory:1.0");
  assert (!CORBA::is_nil (obj3));
  CosCompoundLifeCycle::PropagationCriteriaFactory_var criteria_factory;
  if (criteria_factory = CosCompoundLifeCycle::PropagationCriteriaFactory::_narrow 
      (obj3))
    cout << "done.\n" << flush;
  else
    cout << "ERROR!\n" << flush;


  cout << "Checking first level\n";

  cout << "Checking RoleFactory::role_type ()  ..." << flush;
  //CORBA::InterfaceDef_ptr interface = owner_factory->role_type ();
  CORBA::InterfaceDef_ptr interface = contains_factory->role_type ();
  if (!CORBA::is_nil (interface)) {
    //interface->destroy ();
    cout << "ok.\n" << flush;
  }
  else
    cout << "error.\n" << flush;

  cout << "Checking RelationshipFactory::relationship_type ()  ..." << flush;
  //CORBA::InterfaceDef_ptr interface2 = ownership_factory->relationship_type ();
  CORBA::InterfaceDef_ptr interface2 = containment_factory->relationship_type ();
  if (!CORBA::is_nil (interface2)) {
    //interface2->destroy ();
    cout << "ok.\n" << flush;
  }
  else
    cout << "error.\n" << flush;

  //sleep (1);

  cout << "Checking RoleFactory::create_role ()  ..." << flush;
  CORBA::Boolean right_role = TRUE;

  CosLifeCycleContainment::ContainsRole_var role1;
  CosLifeCycleContainment::ContainedInRole_var role2;
  CosLifeCycleContainment::Relationship_var relship1;
  Documents::Figure_var fig1;
  Documents::Figure_var fig2;
  CosCompoundLifeCycle::Node_var node1;
  CosCompoundLifeCycle::Node_var node2;
  try {
    fig1 = Documents::Figure::_narrow (figure_factory->create ("fig1.png"));
    CosGraphs::Node_var obj = CosGraphs::Node::_duplicate(node_factory->create_node (fig1));
    node1 = CosCompoundLifeCycle::Node::_narrow(obj);
    role1 = CosLifeCycleContainment::ContainsRole::_narrow 
      (contains_factory->create_role (node1));
  }  
  catch (CosRelationships::RoleFactory::RelatedObjectTypeError &ex1)
    {
      cout << "RelatedObjectTypeError!\n" << flush;
      right_role = FALSE;
    }

  if (!CORBA::is_nil (role1)) {
    cout << "ok.\n" << flush;
  }
  else
    cout << "failed.\n";


  cout << "Checking Role::related_object ()  ..." << flush;
  CosCompoundLifeCycle::Node_var tmp_node = CosCompoundLifeCycle::Node::_narrow (role1->related_object ());
  if (!CORBA::is_nil (tmp_node))
    cout << "ok.\n" << flush;
  else {
    cout << "failed.\n" << flush;
  }


  exception = FALSE;
  cout << "Checking Role::destroy ()  ..." << flush;
  role1->destroy ();
  try {
    CORBA::Object_var x = role1->related_object ();
  }
  catch (CORBA::SystemException &ex) {
    exception = TRUE;
  }
 
  if (exception)
    cout << "ok.\n" << flush;
  else {
    cout << "failed.\n" << flush;
  }

  exception = FALSE;
  cout << "Checking RoleFactory::RelatedObjectTypeError exception  ..." 
       << flush;
  
  try {
    role1 = CosLifeCycleContainment::ContainsRole::_narrow 
      (contains_factory->create_role 
       (Documents::Figure::_duplicate (fig1)));
  } catch (CosRelationships::RoleFactory::RelatedObjectTypeError& ex1) {
    exception = TRUE;
  }

  if (exception)
    cout << "ok.\n" << flush;
  else {
    cout << "failed.\n" << flush;
  }

  cout << "Checking RoleFactory::NilRelatedObject exception  ..." << flush;
  exception = FALSE;

  try {
    role1 = CosLifeCycleContainment::ContainsRole::_narrow 
      (contains_factory->create_role 
       (CosCompoundLifeCycle::Node::_nil ()));
  } catch (CosRelationships::RoleFactory::NilRelatedObject &ex1) {
    exception = TRUE;
  }

  if (exception)
    cout << "ok.\n" << flush;
  else {
    cout << "failed.\n" << flush;
  }
  


  try {
    fig2 = Documents::Figure::_narrow (figure_factory->create ("fig2.jpg"));
    node2 = CosCompoundLifeCycle::Node::_narrow 
      (node_factory->create_node (fig2));
    role2 = CosLifeCycleContainment::ContainedInRole::_narrow 
      (containedin_factory->create_role 
       (node2));
    
    role1 = CosLifeCycleContainment::ContainsRole::_narrow
      (contains_factory->create_role
       (node1));

  } catch (CosRelationships::RoleFactory::RelatedObjectTypeError &ex1) {
    cout << "RelatedObjectTypeError!\n" << flush;
  }

  assert (!CORBA::is_nil (role1));
  assert (!CORBA::is_nil (role2));
  exception = FALSE;
  
  cout << "Checking RelationshipFactory::create () ..." << flush;
  CosRelationships::NamedRoles nr;
  nr.length (2);
  nr[0].name = CORBA::string_dup ("LifeCycleContainsRole");
  nr[0].aRole = CosRelationships::Role::_duplicate (role1);
  nr[1].name = CORBA::string_dup ("LifeCycleContainedInRole");
  nr[1].aRole = CosRelationships::Role::_duplicate (role2);
  
  try {
    relship1 = CosLifeCycleContainment::Relationship::_narrow
      (containment_factory->create (nr));
  } 
  catch (CosRelationships::RelationshipFactory::DegreeError &ex) {
    cout << "DegreeError exception!!!\n" << flush;
    cout << "Required degree is " << ex->required_degree << "\n" << flush;
    relship1 = CosLifeCycleContainment::Relationship::_nil ();
  } 
  catch (CosRelationships::RelationshipFactory::DuplicateRoleName &ex) {
    cout << "DuplicateRoleName exception!!!\n" << flush;
    cout << "ex->culprits.length () = " << (ex->culprits).length () << "\n" << flush;
    for (unsigned int i=0; i<(ex->culprits).length () ; i++) {
      cout << i << ". name: " << (ex->culprits[i]).name << "\n" << flush;
    }
    relship1 = CosLifeCycleContainment::Relationship::_nil ();
  } 
  catch (CosRelationships::RelationshipFactory::UnknownRoleName &ex) {
    cout << "UnknownRoleName exception!!!\n" << flush;
    cout << "ex->culprits.length () = " << (ex->culprits).length () << "\n" << flush;
    for (unsigned int i=0; i< (ex->culprits).length (); i++) {
      cout << i << ". name: " << (ex->culprits[i]).name << "\n" << flush;
    }
    relship1 = CosLifeCycleContainment::Relationship::_nil ();
    cout << "End of catch statement!\n" << flush;
  }
  
  if (!CORBA::is_nil (relship1)) {
    cout << "ok.\n" << flush;
  }
  else {
    cout << "failed.\n";
  }

  exception = FALSE;
  state_good = FALSE;
  cout << "Checking RelationshipFactory::UnknownRoleName exception  ..." 
       << flush;
  nr.length (2);
  nr[0].name = strdup ("Ha");
  nr[0].aRole = CosRelationships::Role::_duplicate (role1);
  nr[1].name = strdup ("LifeCycleConatinedInRole");
  nr[1].aRole = CosRelationships::Role::_duplicate (role2);
 
  try {
    relship1 = CosLifeCycleContainment::Relationship::_narrow 
      (containment_factory->create (nr));
  } 
  catch (CosRelationships::RelationshipFactory::DegreeError &ex) {
    cout << "DegreeError exception!!!\n" << flush;
    cout << "Required degree is " << ex->required_degree << "\n" << flush;
    exception = TRUE;
  } 
  catch (CosRelationships::RelationshipFactory::DuplicateRoleName &ex) {
    cout << "DuplicateRoleName exception!!!\n" << flush;
    cout << "ex->culprits.length () = " << (ex->culprits).length () << "\n" << flush;
    for (unsigned int i=0; i<(ex->culprits).length () ; i++) {
      cout << i << ". name: " << (ex->culprits[i]).name << "\n" << flush;
    }
    exception = TRUE;
  } 
  catch (CosRelationships::RelationshipFactory::UnknownRoleName &ex) {
    //       cout << "UnknownRoleName exception!!!\n" << flush;
    //       cout << "ex->culprits.length () = " << (ex->culprits).length () << "\n" 
    //       << flush;
    //       for (int i=0; i< (ex->culprits).length (); i++) {
    //         cout << i << ". name: " << (ex->culprits[i]).name << "\n" << flush;
    //       }
    state_good = TRUE;
  }

  if (state_good) {
    cout << "ok.\n" << flush;
  }
  else 
    cout << "failed.\n" << flush;


  exception = FALSE;
  state_good = FALSE;
  cout << "Checking RelationshipFactory::DuplicateRoleName exception  ..." 
       << flush;
  nr.length (2);
  nr[0].name = CORBA::string_dup ("LifeCycleContainsRole");
  nr[0].aRole = CosRelationships::Role::_duplicate (role1);
  nr[1].name = CORBA::string_dup ("LifeCycleContainsRole");
  nr[1].aRole = CosRelationships::Role::_duplicate (role1);
 
  try {
    relship1 = CosLifeCycleContainment::Relationship::_narrow
      (containment_factory->create (nr));
  } 
  catch (CosRelationships::RelationshipFactory::DegreeError &ex) {
    cout << "DegreeError exception!!!\n" << flush;
    cout << "Required degree is " << ex->required_degree << "\n" << flush;
    exception = TRUE;
  } 
  catch (CosRelationships::RelationshipFactory::DuplicateRoleName &ex) {
    //       cout << "DuplicateRoleName exception!!!\n" << flush;
    //       cout << "ex->culprits.length () = " << (ex->culprits).length () << "\n"
    //            << flush;
    //       for (int i=0; i<(ex->culprits).length () ; i++) {
    //         cout << i << ". name: " << (ex->culprits[i]).name << "\n" << flush;
    //       }
    state_good = TRUE;
  } 
  catch (CosRelationships::RelationshipFactory::UnknownRoleName &ex) {
    cout << "UnknownRoleName exception!!!\n" << flush;
    cout << "ex->culprits.length () = " << (ex->culprits).length () << "\n" << flush;
    for (unsigned int i=0; i< (ex->culprits).length (); i++) {
      cout << i << ". name: " << (ex->culprits[i]).name << "\n" << flush;
    }
    exception = TRUE;
    cout << "End of catch statement!\n" << flush;
  }

  if (state_good) 
    cout << "ok.\n" << flush;
  else
    cout << "failed.\n" << flush;


  exception = FALSE;
  state_good = FALSE;
  cout << "Checking RelationshipFactory::DegreeError exception  ..." << flush;
  nr.length (3);
  nr[0].name = CORBA::string_dup ("LifeCycleContainsRole");
  nr[0].aRole = CosRelationships::Role::_duplicate (role1);
  nr[1].name = CORBA::string_dup ("LifeCycleContainedInRole");
  nr[1].aRole = CosRelationships::Role::_duplicate (role2);
  nr[2].name = CORBA::string_dup ("LifeCycleContainedInRole");
  nr[2].aRole = CosRelationships::Role::_duplicate (role2);

 
  try {
    relship1 = CosLifeCycleContainment::Relationship::_narrow 
      (containment_factory->create (nr));
  } 
  catch (CosRelationships::RelationshipFactory::DegreeError &ex) {
    //       cout << "DegreeError exception!!!\n" << flush;
    //       cout << "Required degree is " << ex->required_degree << "\n" << flush;
    state_good = TRUE;
  } 
  catch (CosRelationships::RelationshipFactory::DuplicateRoleName &ex) {
    cout << "DuplicateRoleName exception!!!\n" << flush;
    cout << "ex->culprits.length () = " << (ex->culprits).length () << "\n" << flush;
    for (unsigned int i=0; i<(ex->culprits).length () ; i++) {
      cout << i << ". name: " << (ex->culprits[i]).name << "\n" << flush;
    }
    exception = TRUE;
  } 
  catch (CosRelationships::RelationshipFactory::UnknownRoleName &ex) {
    cout << "UnknownRoleName exception!!!\n" << flush;
    cout << "ex->culprits.length () = " << (ex->culprits).length () << "\n" << flush;
    for (unsigned int i=0; i< (ex->culprits).length (); i++) {
      cout << i << ". name: " << (ex->culprits[i]).name << "\n" << flush;
    }
    exception = TRUE;
    cout << "End of catch statement!\n" << flush;
  }
  if (state_good) 
    cout << "ok.\n" << flush;
  else
    cout << "failed\n" << flush;


  exception = FALSE;
  state_good = FALSE;
  cout << "Checking RelationshipFactory::RoleTypeError exception  ..." 
       << flush;
  nr.length (2);
  nr[0].name = CORBA::string_dup ("LifeCycleContainsRole");
  nr[0].aRole = CosRelationships::Role::_duplicate (role1);
  nr[1].name = CORBA::string_dup ("LifeCycleContainedInRole");
  nr[1].aRole = CosRelationships::Role::_duplicate (role1);  // ContainedInRole
 
  try {
    relship1 = CosLifeCycleContainment::Relationship::_narrow
      (containment_factory->create (nr));
  } 
  catch (CosRelationships::RelationshipFactory::DegreeError &ex) {
    cout << "DegreeError exception!!!\n" << flush;
    cout << "Required degree is " << ex->required_degree << "\n" << flush;
    exception = TRUE;
  } 
  catch (CosRelationships::RelationshipFactory::DuplicateRoleName &ex) {
    cout << "DuplicateRoleName exception!!!\n" << flush;
    cout << "ex->culprits.length () = " << (ex->culprits).length () << "\n" << flush;
    for (unsigned int i=0; i<(ex->culprits).length () ; i++) {
      cout << i << ". name: " << (ex->culprits[i]).name << "\n" << flush;
    }
    exception = TRUE;
  } 
  catch (CosRelationships::RelationshipFactory::UnknownRoleName &ex) {
    cout << "UnknownRoleName exception!!!\n" << flush;
    cout << "ex->culprits.length () = " << (ex->culprits).length () << "\n" << flush;
    for (unsigned int i=0; i< (ex->culprits).length (); i++) {
      cout << i << ". name: " << (ex->culprits[i]).name << "\n" << flush;
    }
    exception = TRUE;
    cout << "End of catch statement!\n" << flush;
  }
  catch (CosRelationships::RelationshipFactory::RoleTypeError &ex) {
    //      cout << "RoleTypeError exception!!!\n" << flush;
    //      cout << "ex->culprits.length () = " << (ex->culprits).length () << "\n" << flush;
    //      for (int i=0; i< (ex->culprits).length (); i++) {
    //        cout << i << ". name: " << (ex->culprits[i]).name << "\n" << flush;
    //      }
    state_good = TRUE;
  }

  if (state_good)
    cout << "ok.\n" << flush;
  else
    cout << "failed.\n" << flush;


  exception = FALSE;
  state_good = FALSE;
  cout << "Checking RelationshipFactory::MaxCardinalityExceeded exception  ..."
       << flush;
  nr.length (2);
  nr[0].name = CORBA::string_dup ("LifeCycleContainsRole");
  nr[0].aRole = CosRelationships::Role::_duplicate (role1);
  nr[1].name = CORBA::string_dup ("LifeCycleContainedInRole");  // will be twice
  nr[1].aRole = CosRelationships::Role::_duplicate (role2);

  try {
    relship1 = CosLifeCycleContainment::Relationship::_narrow 
      (containment_factory->create (nr));
  } 
  catch (CosRelationships::RelationshipFactory::DegreeError &ex) {
    cout << "DegreeError exception!!!\n" << flush;
    cout << "Required degree is " << ex->required_degree << "\n" << flush;
    exception = TRUE;
  } 
  catch (CosRelationships::RelationshipFactory::DuplicateRoleName &ex) {
    cout << "DuplicateRoleName exception!!!\n" << flush;
    cout << "ex->culprits.length () = " << (ex->culprits).length () << "\n" << flush;
    for (unsigned int i=0; i<(ex->culprits).length () ; i++) {
      cout << i << ". name: " << (ex->culprits[i]).name << "\n" << flush;
    }
    exception = TRUE;
  } 
  catch (CosRelationships::RelationshipFactory::UnknownRoleName &ex) {
    cout << "UnknownRoleName exception!!!\n" << flush;
    cout << "ex->culprits.length () = " << (ex->culprits).length () << "\n" << flush;
    for (unsigned int i=0; i< (ex->culprits).length (); i++) {
      cout << i << ". name: " << (ex->culprits[i]).name << "\n" << flush;
    }
    exception = TRUE;
    cout << "End of catch statement!\n" << flush;
  }
  catch (CosRelationships::RelationshipFactory::MaxCardinalityExceeded &ex) {
    //       cout << "MaxCardinalityExceeded exception!!!\n" << flush;
    //       cout << "ex->culprits.length () = " << (ex->culprits).length () << "\n" << flush;
    //       for (int i=0; i< (ex->culprits).length (); i++) {
    //         cout << i << ". name: " << (ex->culprits[i]).name << "\n" << flush;
    //       }
    state_good = TRUE;
  }


  if (state_good)
    cout << "ok.\n" << flush;
  else
    cout << "failed.\n" << flush;


  cout << "Checking Role::get_relationships ()  ..." << flush;
  CosRelationships::RelationshipIterator_var 
    iterator = CosRelationships::RelationshipIterator::_nil ();
  CosRelationships::RelationshipHandles* rel_handles;
  rel_handles = NULL;
  role1->get_relationships (3, rel_handles, iterator);
  if ((rel_handles != NULL) && (CORBA::is_nil (iterator))) {
    rel_handles = NULL;
    iterator = CosRelationships::RelationshipIterator::_nil ();
    role1->get_relationships (0, rel_handles, iterator);
    if (rel_handles->length () == 0) 
      if (!CORBA::is_nil (iterator))
	cout << "ok.\n" << flush;
      else
	cout << "error3!\n" << flush;
    else
      cout << "error2!\n" << flush;
  }
  else
    cout << "error!\n" << flush;

  cout << "Checking RelationshipIterator::next_n ()  ..." << flush;
  iterator->next_n (10, rel_handles);
  if (rel_handles->length () == 1 && 
      ( (*rel_handles)[0].the_relationship->is_identical (relship1)))
    cout << "ok.\n" << flush;
  else
    cout << "error!\n" << flush;
  iterator->destroy ();

  cout << "Checking RelationshipIterator::next_one ()  ..." << flush;
  CosRelationships::RelationshipHandle* rel_handle;
  role1->get_relationships (0, rel_handles, iterator);
  iterator->next_one (rel_handle);
  if (rel_handle->the_relationship->is_identical (relship1))
    cout << "ok.\n" << flush;
  else
    cout << "error!\n" << flush;

  // testing second level of MICO RS
  cout << "Checking second level\n";

  assert (!CORBA::is_nil (text_factory));
  assert (!CORBA::is_nil (figure_factory));

  Documents::Text_var text = text_factory->create_with_file 
    (CORBA::string_dup ("MICO Relationship Service."), 
     CORBA::string_dup ("thesis.tex"));

  Documents::Figure_var obr1 = figure_factory->create_with_file 
    (CORBA::string_dup ("obr1"), CORBA::string_dup ("obr1.eps"));
  Documents::Figure_var obr2 = figure_factory->create_with_file 
    (CORBA::string_dup ("obr2"), CORBA::string_dup ("obr2.eps")); 
  Documents::Figure_var obr3 = figure_factory->create_with_file 
    (CORBA::string_dup ("obr3"), CORBA::string_dup ("obr3.eps"));

  Documents::Figure_var obr4 = figure_factory->create_with_file 
    (CORBA::string_dup ("obr4"), CORBA::string_dup ("obr4.eps"));
  Documents::Figure_var obr5 = figure_factory->create_with_file 
    (CORBA::string_dup ("obr5"), CORBA::string_dup ("obr5.eps"));
  

  assert (!CORBA::is_nil (text));
  assert (!CORBA::is_nil (obr1));
  assert (!CORBA::is_nil (obr2));
  assert (!CORBA::is_nil (obr3));
  assert (strcmp (text->file (), "thesis.tex") == 0);
  cout << "Checking NodeFactory::create_node () ..." << flush;
  CosCompoundLifeCycle::Node_var node_text = CosCompoundLifeCycle::Node::_narrow(node_factory->create_node (text));
  if (!CORBA::is_nil (node_text))
    cout << "ok.\n";
  else
    cout << "error.\n";

  cout << "Checking Node::related_object () ..." << flush;
  exception = FALSE;
  try {
    node_text->related_object ();
    node_text->related_object ();
    if (!CORBA::is_nil (node_text->related_object ()))
      if (Documents::Text::_narrow (node_text->related_object ()))
	if ((Documents::Text::_narrow (node_text->related_object ()))
	    ->name () != NULL)
	  cout << "ok.\n";
  } catch (CORBA::SystemException& e) {
    cout << "failed.\n";
  } catch (CORBA::UserException& e) {
    cout << "error.\n";
  }
  
  cout << "Checking Node::add_role () ..." << flush;
  CosCompoundLifeCycle::Node_var node_obr1;   
  CosCompoundLifeCycle::Node_var node_obr2;
  CosCompoundLifeCycle::Node_var node_obr3;
  CosCompoundLifeCycle::Node_var node_obr4;
  CosCompoundLifeCycle::Node_var node_obr5;

  try {
    node_obr1 = CosCompoundLifeCycle::Node::_narrow(node_factory->create_node (obr1));
    node_obr2 = CosCompoundLifeCycle::Node::_narrow(node_factory->create_node (obr2));
    node_obr3 = CosCompoundLifeCycle::Node::_narrow(node_factory->create_node (obr3));
    node_obr4 = CosCompoundLifeCycle::Node::_narrow(node_factory->create_node (obr4));
    node_obr5 = CosCompoundLifeCycle::Node::_narrow(node_factory->create_node (obr5));
  } catch (::CORBA::Exception& e) {
    cout << "can't create nodes!!! " << flush;
  }
  CosCompoundLifeCycle::Role_var contains;
  CosCompoundLifeCycle::Role_var containedin1;
  CosCompoundLifeCycle::Role_var containedin2;
  CosCompoundLifeCycle::Role_var containedin3;
  CosCompoundLifeCycle::Role_var contains_obr1;
  CosCompoundLifeCycle::Role_var containedin4;
  CosCompoundLifeCycle::Role_var containedin5;
  
  try {
    contains = CosCompoundLifeCycle::Role::_narrow 
      (contains_factory->create_role (node_text));
    containedin1 = CosCompoundLifeCycle::Role::_narrow
      (containedin_factory->create_role (node_obr1));
    containedin2 = CosCompoundLifeCycle::Role::_narrow
      (containedin_factory->create_role (node_obr2));
    containedin3 = CosCompoundLifeCycle::Role::_narrow
      (containedin_factory->create_role (node_obr3));
    contains_obr1 = CosCompoundLifeCycle::Role::_narrow 
      (contains_factory->create_role (node_obr1));
    containedin4 = CosCompoundLifeCycle::Role::_narrow
      (containedin_factory->create_role (node_obr4));
    containedin5 = CosCompoundLifeCycle::Role::_narrow
      (containedin_factory->create_role (node_obr5));


  } catch (CosRelationships::RoleFactory::NilRelatedObject& e) {
    cout << "can't create roles - nil related object\n";
  } catch (CosRelationships::RoleFactory::RelatedObjectTypeError& e) {
    cout << "can't create roles - related object type error\n";
  } catch (::CORBA::SystemException& e) {
    cout << "can't create roles!!! system exception\n" << flush;
  }
    

  assert (!CORBA::is_nil (contains));
  exception = FALSE;
  try {
    node_text->add_role (contains);
  } catch (::CORBA::SystemException& e) {
    exception = TRUE;
  }
  if (node_text->roles_of_node ()->length () == 1 && !exception)
    cout << "ok.\n";
  else
    cout << "error.\n";


  cout << "Checking Node::roles_of_node () ..." << flush;
  exception = FALSE;
  CosGraphs::Node::Roles_var node_roles;
  try {
    node_roles = node_text->roles_of_node ();
    node_roles = node_text->roles_of_node ();
    node_roles = node_text->roles_of_node ();
  } catch (CORBA::SystemException& e) {
    exception = TRUE;
  }
  if (node_roles->length () == 1 && !exception)
    cout << "ok.\n";
  else
    cout << "error.\n"; 


  cout << "Checking Node::remove_role () ..." << flush;
  try {
    ::CORBA::InterfaceDef_var type = contains->_get_interface ();
    node_text->remove_role (type);
  } catch (CORBA::SystemException& e) {
    cout << "system exception\n";
  } catch (CORBA::UserException& e) {
    exception = TRUE;
  }
  if (node_text->roles_of_node ()->length () == 0 && !exception)
    cout << "ok.\n";
  else
    cout << "error.\n";

  assert (!CORBA::is_nil (node_text));

  cout << "Checking Node::roles_of_type () ..." << flush;

  // in this time is role_of_types buggy

  exception = FALSE;
  Test::BaseRole_ptr base_role = Test::BaseRole::_narrow 
    (base_factory->create_role (node_text));
  CORBA::InterfaceDef_var base_role_type = base_role->_get_interface ();

  Test::DerivedRole1_ptr derived1_role = Test::DerivedRole1::_narrow 
    (derived_factory1->create_role (node_text));
  CORBA::InterfaceDef_var derived1_role_type 
    = derived1_role->_get_interface ();

  Test::DerivedRole2_ptr derived2_role = Test::DerivedRole2::_narrow 
    (derived_factory2->create_role (node_text));
  CORBA::InterfaceDef_var derived2_role_type 
    = derived2_role->_get_interface ();

  state_good = TRUE;
  exception = FALSE;
  exception2 = FALSE;
  try {
    node_text->add_role (derived1_role);
    node_text->add_role (derived2_role);
    node_roles = node_text->roles_of_type (derived1_role_type);
    if (node_roles->length () == 1 
	&& (!CORBA::is_nil (Test::DerivedRole1::_narrow ((*node_roles)[0]))))
      state_good = TRUE;
    else
      state_good = FALSE;
    
    if (state_good) {
      node_roles = node_text->roles_of_type (derived2_role_type);
      if (node_roles->length () == 1 
	  && (Test::DerivedRole2::_narrow ((*node_roles)[0])))
	state_good = TRUE;
      else
	state_good = FALSE;
    }
    if (state_good) {
      node_roles = node_text->roles_of_type (base_role_type);
      if (node_roles->length () == 2)
	state_good = TRUE;
      else
	state_good = FALSE;
    }
    // cleaning up
    node_text->remove_role (base_role_type);

  } catch (CORBA::SystemException& e) {
    exception = TRUE;
  } catch (CORBA::UserException& e) {
    exception2 = TRUE;
  }
  
  if (state_good && !exception2 && !exception)
    cout << "ok.\n";
  if (!state_good)
    cout << "failed.\n";
  if (exception)
    cout << "failed2.\n";
  if (exception2)
    cout << "error.\n";
  

  cout << "Checking Node::DuplicateRoleType exception ..." << flush;
  exception = FALSE;
  try {
    node_text->add_role (contains);
    node_text->add_role (contains);
  } catch (CosGraphs::Node::DuplicateRoleType& e) {
    exception = TRUE;
  }
  if (exception)
    cout << "ok.\n";
  else
    cout << "error.\n";

  
  cout << "Checking Node::NoSuchRole exception ..." << flush;
  exception = FALSE;
  exception2 = FALSE;
  exception3 = FALSE;
  try {
    CORBA::InterfaceDef_var type = containedin1->_get_interface ();
    node_text->remove_role (type);
  } catch (CORBA::SystemException& e) {
    cout << "system exception\n";
  } catch (CosGraphs::Node::NoSuchRole& e) {
    exception = TRUE;
  }
  try {
    CORBA::InterfaceDef_var type = contains->_get_interface ();
    node_text->remove_role (type);
  } catch (CORBA::SystemException& e) {
    cout << "system exception\n";
  } catch (CosGraphs::Node::NoSuchRole& e) {
    exception2 = TRUE;
  }
  try {
    CORBA::InterfaceDef_var type = contains->_get_interface ();
    node_text->remove_role (type);
  } catch (CORBA::SystemException& e) {
    cout << "system exception\n";
  } catch (CosGraphs::Node::NoSuchRole& e) {
    exception3 = TRUE;
  }
    
  if (exception && !exception2 && exception3) 
    cout << "ok.\n";
  else
    cout << "error.\n";


  cout << "Checking Role::get_edges () " << flush;
  assert (node_text->roles_of_node ()->length () == 0);
  node_text->add_role (contains);
  node_obr1->add_role (containedin1);
  node_obr1->add_role (contains_obr1);
  node_obr2->add_role (containedin2);
  node_obr3->add_role (containedin3);
  node_obr4->add_role (containedin4);
  node_obr5->add_role (containedin5);
  
  cout << "." << flush;
  nr.length (2);
  nr[0].name = CORBA::string_dup ("LifeCycleContainsRole");
  nr[1].name = CORBA::string_dup ("LifeCycleContainedInRole");

  nr[0].aRole = CosCompoundLifeCycle::Role::_duplicate (contains);
  nr[1].aRole = CosCompoundLifeCycle::Role::_duplicate (containedin3);
  CosLifeCycleContainment::Relationship_ptr containment3 
    = CosLifeCycleContainment::Relationship::_narrow (containment_factory->create (nr));
  cout << "." << flush;

  nr[0].aRole = CosCompoundLifeCycle::Role::_duplicate (contains);
  nr[1].aRole = CosCompoundLifeCycle::Role::_duplicate (containedin1);
  CosLifeCycleContainment::Relationship_ptr containment1 
    = CosLifeCycleContainment::Relationship::_narrow (containment_factory->create (nr));
  cout << "." << flush;

  nr[0].aRole = CosCompoundLifeCycle::Role::_duplicate (contains);
  nr[1].aRole = CosCompoundLifeCycle::Role::_duplicate (containedin2);
  CosLifeCycleContainment::Relationship_ptr containment2 
    = CosLifeCycleContainment::Relationship::_narrow (containment_factory->create (nr));
  cout << "." << flush;


  nr[0].aRole = CosCompoundLifeCycle::Role::_duplicate (contains_obr1);
  nr[1].aRole = CosCompoundLifeCycle::Role::_duplicate (containedin4);
  CosLifeCycleContainment::Relationship_ptr containment4 
    = CosLifeCycleContainment::Relationship::_narrow (containment_factory->create (nr));
  cout << "." << flush;

  nr[0].aRole = CosCompoundLifeCycle::Role::_duplicate (contains_obr1);
  nr[1].aRole = CosCompoundLifeCycle::Role::_duplicate (containedin5);
  CosLifeCycleContainment::Relationship_ptr containment5 
    = CosLifeCycleContainment::Relationship::_narrow (containment_factory->create (nr));
  cout << "." << flush;

  state_good = true;
  CosGraphs::Edges* edges;
  CosGraphs::EdgeIterator_ptr edge_iterator;
  contains->get_edges (1, edges, edge_iterator);

  Documents::Text_ptr tmp_text 
    = Documents::Text::_narrow 
    (CosCompoundLifeCycle::Node::_narrow ((*edges)[0].from.the_role.the_role
			       ->related_object ())->related_object ()); 
  if (strcmp (tmp_text->name (), text->name ()) != 0)
    state_good = false;
  Documents::Figure_ptr tmp_obr 
    = Documents::Figure::_narrow 
    (CosCompoundLifeCycle::Node::_narrow ((*edges)[0].relatives[0].the_role.the_role
			       ->related_object ())->related_object ()); 
  if (strcmp (tmp_obr->name (), obr3->name ()) != 0)
    state_good = false;

  if (CORBA::is_nil (edge_iterator))
    state_good = false;
  else 
    edge_iterator->destroy ();
  if (state_good)
    cout << "ok.\n";
  else
    cout <<"failed.\n";

  cout << "Checking EdgeIterator::next_one () ..." << flush;
  contains->get_edges (1, edges, edge_iterator);
  CosGraphs::Edge* edge;
  if (!CORBA::is_nil (edge_iterator)) {
    edge_iterator->next_one (edge);
    tmp_obr = Documents::Figure::_narrow 
      (CosCompoundLifeCycle::Node::_narrow (edge->relatives[0].the_role.the_role
				 ->related_object ())->related_object ()); 
    if (strcmp (tmp_obr->name (), obr1->name ()) != 0)
      state_good = false;
    edge_iterator->next_one (edge);
    if (edge_iterator->next_one (edge))
      state_good = false;
  }
  else
    state_good = false;

  edge_iterator->destroy ();

  // for get_edges (0, edges, edge_iterator);
  contains->get_edges (0, edges, edge_iterator);
  if (!CORBA::is_nil (edge_iterator)) {
    edge_iterator->next_one (edge);
    tmp_obr = Documents::Figure::_narrow 
      (CosCompoundLifeCycle::Node::_narrow (edge->relatives[0].the_role.the_role
			       ->related_object ())->related_object ()); 
    if (strcmp (tmp_obr->name (), obr3->name ()) != 0)
      state_good = false;
    edge_iterator->next_one (edge);
    tmp_obr = Documents::Figure::_narrow 
      (CosCompoundLifeCycle::Node::_narrow (edge->relatives[0].the_role.the_role
				 ->related_object ())->related_object ()); 
    if (strcmp (tmp_obr->name (), obr1->name ()) != 0)
      state_good = false;
    edge_iterator->next_one (edge);
    if (edge_iterator->next_one (edge))
      state_good = false;

    edge_iterator->destroy ();
  }
  else
    state_good = false;
  
  contains->get_edges (3, edges, edge_iterator);
  if (!CORBA::is_nil (edge_iterator)) {
    if (edge_iterator->next_one (edge))
      state_good = false;

    edge_iterator->destroy ();
  }
  else
    state_good = false;

  contains->get_edges (30, edges, edge_iterator);
  if (!CORBA::is_nil (edge_iterator)) {
    if (edge_iterator->next_one (edge))
      state_good = false;

    edge_iterator->destroy ();
  }
  else
    state_good = false;
  
  if (state_good)
    cout << "ok.\n";
  else
    cout <<"failed.\n";
  

  cout << "Checking EdgeIterator::next_n () ..." << flush;
  contains->get_edges (0, edges, edge_iterator);
  if (!CORBA::is_nil (edge_iterator)) {
    edge_iterator->next_n (1, edges);
    tmp_obr = Documents::Figure::_narrow 
      (CosCompoundLifeCycle::Node::_narrow ((*edges)[0].relatives[0].the_role.the_role
				 ->related_object ())->related_object ()); 
    if (strcmp (tmp_obr->name (), obr3->name ()) != 0)
      state_good = false;
    edge_iterator->next_one (edge);
    edge_iterator->next_one (edge);
    if (edge_iterator->next_one (edge))
      state_good = false;

    edge_iterator->destroy ();
  }
  else
    state_good = false;

  contains->get_edges (0, edges, edge_iterator);
  if (!CORBA::is_nil (edge_iterator)) {
    edge_iterator->next_n (3, edges);
    tmp_obr = Documents::Figure::_narrow 
      (CosCompoundLifeCycle::Node::_narrow ((*edges)[0].relatives[0].the_role.the_role
				 ->related_object ())->related_object ()); 
    if (strcmp (tmp_obr->name (), obr3->name ()) != 0)
      state_good = false;

    tmp_obr = Documents::Figure::_narrow 
      (CosCompoundLifeCycle::Node::_narrow ((*edges)[1].relatives[0].the_role.the_role
				 ->related_object ())->related_object ()); 
    if (strcmp (tmp_obr->name (), obr1->name ()) != 0)
      state_good = false;

    tmp_obr = Documents::Figure::_narrow 
      (CosCompoundLifeCycle::Node::_narrow ((*edges)[2].relatives[0].the_role.the_role
				 ->related_object ())->related_object ()); 
    if (strcmp (tmp_obr->name (), obr2->name ()) != 0)
      state_good = false;

    if (edge_iterator->next_one (edge))
      state_good = false;

    edge_iterator->destroy ();
  }
  else
    state_good = false;

  contains->get_edges (0, edges, edge_iterator);
  if (!CORBA::is_nil (edge_iterator)) {
    edge_iterator->next_n (30, edges);
    tmp_obr = Documents::Figure::_narrow 
      (CosCompoundLifeCycle::Node::_narrow ((*edges)[0].relatives[0].the_role.the_role
				 ->related_object ())->related_object ()); 
    if (strcmp (tmp_obr->name (), obr3->name ()) != 0)
      state_good = false;

    tmp_obr = Documents::Figure::_narrow 
      (CosCompoundLifeCycle::Node::_narrow ((*edges)[1].relatives[0].the_role.the_role
				 ->related_object ())->related_object ()); 
    if (strcmp (tmp_obr->name (), obr1->name ()) != 0)
      state_good = false;

    tmp_obr = Documents::Figure::_narrow 
      (CosCompoundLifeCycle::Node::_narrow ((*edges)[2].relatives[0].the_role.the_role
				 ->related_object ())->related_object ()); 
    if (strcmp (tmp_obr->name (), obr2->name ()) != 0)
      state_good = false;

    if (edge_iterator->next_one (edge))
      state_good = false;

    edge_iterator->destroy ();
  }
  else
    state_good = false;

  contains->get_edges (0, edges, edge_iterator);
  if (!CORBA::is_nil (edge_iterator)) {
    edge_iterator->next_n (0, edges);
    if (!edge_iterator->next_one (edge))
      state_good = false;
    if (!edge_iterator->next_one (edge))
      state_good = false;
    if (!edge_iterator->next_one (edge))
      state_good = false;
    if (edge_iterator->next_one (edge))
      state_good = false;

    edge_iterator->destroy ();
  }
  else
    state_good = false;

  if (state_good)
    cout << "ok.\n";
  else
    cout <<"failed.\n";
  
  
  state_good = true;
  cout << "Checking TraversalFactory_impl::create_traversal_on () ..." 
       << flush;

  CosGraphs::TraversalCriteria_ptr criteria = criteria_factory->create 
    (CosCompoundLifeCycle::copy);
  if (CORBA::is_nil (criteria))
    state_good = false;

  CosGraphs::NodeHandle node_text_handle;
  CosGraphs::Traversal::ScopedEdge* scoped_edge;

  node_text_handle.the_node = CosCompoundLifeCycle::Node::_duplicate (node_text);
  node_text_handle.constant_random_id = node_text->constant_random_id ();

  CosGraphs::Traversal_ptr traversal = traversal_factory->create_traversal_on 
    //(node_text_handle, criteria, CosGraphs::breadthFirst);
    (node_text_handle, criteria, CosGraphs::depthFirst);

  if (CORBA::is_nil (traversal))
    state_good = false;
  
  if (state_good)
    cout << "ok.\n";
  else
    cout <<"failed.\n";

  cout << "Checking traversing graphs of related objects - depthFirst\n";
  
  assert (!CORBA::is_nil (traversal));
  
  while (traversal->next_one (scoped_edge)) {
    cout << "from: " << Documents::AbstractDocument::_narrow (scoped_edge->from.point.the_node.the_node->related_object ())->name () << "\n";
    cout << "role: " << scoped_edge->from.point.the_role.the_name << "\n";
    cout << "to: " << Documents::AbstractDocument::_narrow (scoped_edge->relatives[0].point.the_node.the_node->related_object ())->name () << "\n";
    cout << "role: " << scoped_edge->relatives[0].point.the_role.the_name << "\n";
    cout << "\n";
  }
  
  state_good = true;
  traversal->destroy ();

  CosGraphs::TraversalCriteria_ptr criteria2 = criteria_factory->create 
    (CosCompoundLifeCycle::copy);
  assert (!CORBA::is_nil (criteria2));

  
  CosGraphs::NodeHandle node_obr1_handle;
  node_obr1_handle.the_node = CosCompoundLifeCycle::Node::_duplicate (node_obr1);
  node_obr1_handle.constant_random_id = node_obr1->constant_random_id ();
  
  CosGraphs::Traversal_ptr traversal2 = traversal_factory->create_traversal_on 
    (node_text_handle, criteria2, CosGraphs::breadthFirst);

  
  assert (!CORBA::is_nil (traversal2));
  
  cout << "Checking traversing graphs of related objects - breadthFirst\n";

  
  CosGraphs::Traversal::ScopedEdge* scoped_edge2;
  while (traversal2->next_one (scoped_edge2)) {
    assert (scoped_edge2 != NULL);
    assert (!CORBA::is_nil (scoped_edge2->from.point.the_node.the_node));
    cout << "from: " << Documents::AbstractDocument::_narrow (scoped_edge2->from.point.the_node.the_node->related_object ())->name () << "\n";
    cout << "role: " << scoped_edge2->from.point.the_role.the_name << "\n";
    cout << "to: " << Documents::AbstractDocument::_narrow (scoped_edge2->relatives[0].point.the_node.the_node->related_object ())->name () << "\n";
    cout << "role: " << scoped_edge2->relatives[0].point.the_role.the_name << "\n";
    cout << "\n";

  }

  cout << "Binding to FactoryFinder ..." << flush;
  CORBA::ORB::ObjectTag_var 
    tagff = CORBA::ORB::string_to_tag ("FactoryFinder_impl");
  CORBA::Object_var 
  objff = orb->bind ("IDL:omg.org/CosLifeCycle/FactoryFinder:1.0", 
		     tagff.in());
  assert (!CORBA::is_nil (objff));
  CosLifeCycle::FactoryFinder_var factory_finder; 
  if (factory_finder = CosLifeCycle::FactoryFinder::_narrow (objff))
    cout << "done.\n" << flush;
  else
    cout << "ERROR!\n" << flush;

  cout << "Binding to OperationsFactory ..." << flush;
  CORBA::ORB::ObjectTag_var 
    tagof = CORBA::ORB::string_to_tag ("Operations_impl");
  CORBA::Object_var
  objof = orb->bind ("IDL:omg.org/CosCompoundLifeCycle/OperationsFactory:1.0",
		     tagof.in());
  assert (!CORBA::is_nil (objof));
  CosCompoundLifeCycle::OperationsFactory_var operations_factory; 
  if (operations_factory = CosCompoundLifeCycle::OperationsFactory::_narrow (objof))
    cout << "done.\n" << flush;
  else
    cout << "ERROR!\n" << flush;

    
  CosCompoundLifeCycle::Operations_ptr oper = operations_factory->create_compound_operations();

  CosLifeCycle::Criteria the_criteria;
  the_criteria.length(0);

  cout << "Checking copy operations for graphs of related objects\n";
  exception=FALSE;
  CosCompoundLifeCycle::Node_var new_node;
  try {
    CORBA::Object_ptr __obj = oper->copy(node_text,CosLifeCycle::FactoryFinder::_duplicate(factory_finder),the_criteria);
    new_node = CosCompoundLifeCycle::Node::_narrow(__obj);
  } catch (CORBA::SystemException& e) {
    cout << "system exception\n";
    exception = TRUE;
  } catch (CosLifeCycle::NoFactory& e) {
    cout << "NoFactory exception\n";
    exception = TRUE;
  } catch (CosLifeCycle::NotCopyable& e) {
    cout << "NotCopyable exception\n";
    exception = TRUE;
  } catch (CosLifeCycle::InvalidCriteria& e) {
    cout << "InvalidCriteria exception\n";
    exception = TRUE;
  } catch (CosLifeCycle::CannotMeetCriteria& e) {
    cout << "CannotMeetCriteria exception\n";
    exception = TRUE;
  }
  
  if (exception)
      cout << "error\n";
  else
      cout << "ok\n";


  CosGraphs::TraversalCriteria_ptr criteria3 = criteria_factory->create 
    (CosCompoundLifeCycle::copy);
  assert (!CORBA::is_nil (criteria3));

  
  CosGraphs::NodeHandle new_node_handle;
  new_node_handle.the_node = CosCompoundLifeCycle::Node::_duplicate (new_node);
  new_node_handle.constant_random_id = new_node->constant_random_id ();
  
  CosGraphs::Traversal_ptr traversal3 = traversal_factory->create_traversal_on 
    (new_node_handle, criteria3, CosGraphs::breadthFirst);

  
  assert (!CORBA::is_nil (traversal3));
  
  cout << "Checking traversing graphs of coped related objects - breadthFirst\n";

  
  CosGraphs::Traversal::ScopedEdge* scoped_edge3;
  while (traversal3->next_one (scoped_edge3)) {
    assert (scoped_edge3 != NULL);
    assert (!CORBA::is_nil (scoped_edge3->from.point.the_node.the_node));
    cout << "from: " << Documents::AbstractDocument::_narrow (scoped_edge3->from.point.the_node.the_node->related_object ())->name () << "\n";
    cout << "role: " << scoped_edge3->from.point.the_role.the_name << "\n";
    cout << "to: " << Documents::AbstractDocument::_narrow (scoped_edge3->relatives[0].point.the_node.the_node->related_object ())->name () << "\n";
    cout << "role: " << scoped_edge3->relatives[0].point.the_role.the_name << "\n";
    cout << "\n";

  }

  cout << "Checking LifeCycle remove operation ...";
  exception=FALSE;
  try {
    oper->remove(new_node.inout());
  } catch (CORBA::SystemException& e) {
    cout << "system exception\n";
    exception = TRUE;
  } catch (CosLifeCycle::NoFactory& e) {
    cout << "NoFactory exception\n";
    exception = TRUE;
  } catch (CosLifeCycle::NotRemovable& e) {
    cout << "NotCopyable exception\n";
    exception = TRUE;
  } catch (CosLifeCycle::InvalidCriteria& e) {
    cout << "InvalidCriteria exception\n";
    exception = TRUE;
  } catch (CosLifeCycle::CannotMeetCriteria& e) {
    cout << "CannotMeetCriteria exception\n";
    exception = TRUE;
  }
  
  if (exception)
      cout << "error\n";
  else
      cout << "ok\n";



  state_good = true;

  cout << "Destroing objects" << flush;
  // containment relationships..
  containment1->destroy ();
  cout << "." << flush;
  containment2->destroy ();
  cout << "." << flush;
  containment3->destroy ();
  cout << "." << flush;
  containment4->destroy ();
  cout << "." << flush;
  containment5->destroy ();
  cout << "." << flush;
  // containment roles
  contains->destroy ();
  cout << "." << flush;
  containedin1->destroy ();
  cout << "." << flush;
  containedin2->destroy ();
  cout << "." << flush;
  containedin3->destroy ();
  cout << "." << flush;
  
  // traversals
  //traversal->destroy ();
  traversal2->destroy ();
  cout << "." << flush;
  traversal3->destroy ();
  cout << "." << flush;
  
  // criterias
  criteria->destroy ();
  criteria2->destroy ();
  criteria3->destroy ();
  cout << "." << flush;
  //operations
  oper->destroy();
  cout << "." << flush;

  if (!CORBA::is_nil (iterator)) {
    iterator->destroy ();
    cout << "." << flush;  
  }
  if (!CORBA::is_nil (relship1)) {
    relship1->destroy ();
    cout << "." << flush;  
  }
  if (!CORBA::is_nil (role1)) {
    role1->destroy ();
    cout << "." << flush;  
  }
  if (!CORBA::is_nil (role2)) {
    role2->destroy ();
    cout << "." << flush;  
  }
  
  cout << "done.\n" << flush;
  return 0;
}


