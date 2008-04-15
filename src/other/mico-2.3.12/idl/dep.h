/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2005 by The Mico Team
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
 *  For more information, visit the MICO Home Page at
 *  http://www.mico.org/
 */

#ifndef __DEP_H__
#define __DEP_H__


#include <CORBA.h>
#ifdef USE_CCM
#include <mico/ir3.h>
#else
#include <mico/ir.h>
#endif
#include <mico/util.h>
#include <mico/template_impl.h>

struct Dep {
  Dep()
  {
    obj = CORBA::IRObject::_nil();
    iface_as_forward = false;
    visited = false;
  }
  CORBA::IRObject_var obj;
  bool                iface_as_forward;
  std::vector<Dep*>        deps;
  bool                visited;
};

typedef TVarVar<Dep> Dep_var;


struct IRObj {
  IRObj()
  {
    obj = CORBA::IRObject::_nil();
    iface_as_forward = false;
  }
  
  CORBA::IRObject_var obj;
  std::string              scope;
  bool                iface_as_forward;
};

typedef IRObj *IRObj_ptr;
typedef TVarVar<IRObj> IRObj_var;

typedef SequenceTmpl<IRObj_var,MICO_TID_DEF> IRObjSeq;


class IDLDep {
private:
  struct objcomp : std::binary_function<CORBA::Object *, CORBA::Object *, bool> {
    bool operator() (CORBA::Object *o1, CORBA::Object *o2) const
    {
      return *o1->_ior() < *o2->_ior();
    }
  };
  typedef std::map<CORBA::Object *, Dep *, objcomp> MapObjDep;

  Dep_var                 _top;
  Dep_var                 _bottom;
  MapObjDep               _all_deps;
  MapObjDep               _fwd_iface_deps;
  IRObjSeq                _ordered_objs;
  
  CORBA::Container_ptr    _container;
  CORBA::Container_var    _current_iface;

  void traverse_container( CORBA::Container_ptr con );
  void set_current_iface( CORBA::Container_ptr in );
  CORBA::Container_ptr get_current_iface();
  bool is_circular_dep( Dep* from, Dep* to );
  void create_dep( Dep* from, Dep* to, bool parent_dependency = false);
  void create_iface_dep( Dep* from );
  Dep* locate_dep_for_forward_iface( CORBA::IRObject_ptr obj );
  Dep* locate_dep( CORBA::IRObject_ptr obj, 
		   bool create_type = false,
		   bool create_iface = false );
  void dep_Struct( CORBA::StructDef_ptr s );
  void dep_Union( CORBA::UnionDef_ptr u );
  void dep_Constant( CORBA::ConstantDef_ptr u );
  void dep_Exception( CORBA::ExceptionDef_ptr u );
  void dep_Enum( CORBA::EnumDef_ptr u );
  void dep_Alias( CORBA::AliasDef_ptr a );
  void dep_Interface( CORBA::InterfaceDef_ptr in );
  void dep_Operation( CORBA::OperationDef_ptr o );
  void dep_Attribute( CORBA::AttributeDef_ptr a );
  void dep_ValueBox( CORBA::ValueBoxDef_ptr vb );
  void dep_Value( CORBA::ValueDef_ptr vb );
  void dep_ValueMember( CORBA::ValueMemberDef_ptr vmd );
  void dep_Native( CORBA::NativeDef_ptr nd );
#ifdef USE_CCM
  void dep_Component (CORBA::ComponentIR::ComponentDef_ptr);
  void dep_Provides (CORBA::ComponentIR::ProvidesDef_ptr);
  void dep_Uses (CORBA::ComponentIR::UsesDef_ptr);
  void dep_EventPort (CORBA::ComponentIR::EventPortDef_ptr);
  void dep_Home (CORBA::ComponentIR::HomeDef_ptr);
#endif
  
  void compute_dependencies();
  void traverse_dep( Dep* dep );
  void set_visited_flag( Dep* dep, bool flag );
  void add_anonymous_types( Dep* dep );
  
public:
  IDLDep( CORBA::Container_ptr con );
  IRObjSeq& build_dependencies();
  void print (Dep * = 0);
};

#endif
