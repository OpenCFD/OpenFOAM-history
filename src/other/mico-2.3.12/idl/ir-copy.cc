/*
 *  Mico --- an Open Source CORBA implementation
 *  Copyright Frank Pilhofer
 *
 *  This file is part of MicoCCM, written by Frank Pilhofer.
 *  The MICO CORBA Component Project was sponsored by Alcatel.
 */

#ifdef FAST_PCH
#include "idl_pch.h"
#endif // FAST_PCH
#ifdef __COMO__
#pragma hdrstop
#endif // __COMO__

#ifndef FAST_PCH

#include <CORBA.h>
#ifdef USE_CCM
#include <mico/ir3.h>
#else
#include <mico/ir.h>
#endif
#include <mico/ir_creator.h>
#include <mico/util.h>
#include <mico/os-misc.h>
#include "db.h"
#include "parser.h"
#include "params.h"
#include "idlparser.h"

#endif // FAST_PCH


using namespace std;

/*
 * IDL 3 to IDL 2 Transformer
 */

class IRCopy {
public:
  IRCopy (DB & db, CORBA::Repository_ptr targetrepo);

  bool copy (CORBA::Container_ptr src,
	     bool feed_included_defs);

  char * get_error ();

private:
  CORBA::Contained_ptr      copy_Contained   (CORBA::Contained_ptr src);
  CORBA::Container_ptr      copy_Container   (CORBA::Container_ptr src);
  CORBA::IDLType_ptr        copy_IDLType     (CORBA::IDLType_ptr src);

  CORBA::ModuleDef_ptr      copy_Module      (CORBA::ModuleDef_ptr src);
  CORBA::ConstantDef_ptr    copy_Constant    (CORBA::ConstantDef_ptr src);
  CORBA::StructDef_ptr      copy_Struct      (CORBA::StructDef_ptr src);
  CORBA::UnionDef_ptr       copy_Union       (CORBA::UnionDef_ptr src);
  CORBA::EnumDef_ptr        copy_Enum        (CORBA::EnumDef_ptr src);
  CORBA::AliasDef_ptr       copy_Alias       (CORBA::AliasDef_ptr src);
  CORBA::ExceptionDef_ptr   copy_Exception   (CORBA::ExceptionDef_ptr src);
  CORBA::AttributeDef_ptr   copy_Attribute   (CORBA::AttributeDef_ptr src);
  CORBA::OperationDef_ptr   copy_Operation   (CORBA::OperationDef_ptr src);
  CORBA::InterfaceDef_ptr   copy_Interface   (CORBA::InterfaceDef_ptr src);
  CORBA::ValueMemberDef_ptr copy_ValueMember (CORBA::ValueMemberDef_ptr src);
  CORBA::ValueDef_ptr       copy_Value       (CORBA::ValueDef_ptr src);
  CORBA::ValueBoxDef_ptr    copy_ValueBox    (CORBA::ValueBoxDef_ptr src);
  CORBA::NativeDef_ptr      copy_Native      (CORBA::NativeDef_ptr src);

#ifdef USE_CCM
  CORBA::ComponentIR::ProvidesDef_ptr
  copy_Provides (CORBA::ComponentIR::ProvidesDef_ptr src);

  CORBA::ComponentIR::UsesDef_ptr
  copy_Uses (CORBA::ComponentIR::UsesDef_ptr src);

  CORBA::ComponentIR::EmitsDef_ptr
  copy_Emits (CORBA::ComponentIR::EmitsDef_ptr src);

  CORBA::ComponentIR::PublishesDef_ptr
  copy_Publishes (CORBA::ComponentIR::PublishesDef_ptr src);

  CORBA::ComponentIR::ConsumesDef_ptr
  copy_Consumes (CORBA::ComponentIR::ConsumesDef_ptr src);

  CORBA::ComponentIR::ComponentDef_ptr
  copy_Component (CORBA::ComponentIR::ComponentDef_ptr src);

  CORBA::ComponentIR::FactoryDef_ptr
  copy_Factory (CORBA::ComponentIR::FactoryDef_ptr src);

  CORBA::ComponentIR::FinderDef_ptr
  copy_Finder (CORBA::ComponentIR::FinderDef_ptr src);

  CORBA::ComponentIR::HomeDef_ptr
  copy_Home (CORBA::ComponentIR::HomeDef_ptr src);
#endif

  bool is_included_def (CORBA::Contained_ptr src);

  DB & _db;
  bool _feed_included_defs;
  CORBA::Repository_var _repo;
  set<string, less<string> > visited;
  string error;
};

IRCopy::IRCopy (DB & db, CORBA::Repository_ptr targetrepo)
  : _db (db)
{
  _repo = CORBA::Repository::_duplicate (targetrepo);
}

bool
IRCopy::is_included_def (CORBA::Contained_ptr src)
{
  string toplev = _db.get_toplevel_fname();

  if (toplev.length() == 0) {
    return false;
  }

  CORBA::String_var id = src->id ();
  string strid = id.in();

  if (!_db.is_repoid_defined (strid)) {
    return true;
  }

  string fname = _db.get_fname_for_repoid (strid);
  return (fname == toplev) ? false : true;
}

char *
IRCopy::get_error ()
{
  return CORBA::string_dup (error.c_str());
}

/*
 * Traverse the Interface Repository in Pre-Order
 */

bool
IRCopy::copy (CORBA::Container_ptr src,
	      bool feed_included_defs)
{
  _feed_included_defs = feed_included_defs;
  
  CORBA::Contained_var cs = CORBA::Contained::_narrow (src);

  if (!CORBA::is_nil (cs) &&
      (!is_included_def (cs) || _feed_included_defs ||
       cs->def_kind() == CORBA::dk_Module)) {
    CORBA::Contained_var tc = copy_Contained (cs);
    if (CORBA::is_nil (tc)) {
      return false;
    }
  }

  CORBA::ContainedSeq_var contents = src->contents (CORBA::dk_all, 1);
  CORBA::Contained_var dummy1;
  CORBA::Container_var dummy2;

  for (CORBA::ULong i=0; i<contents->length(); i++) {
    CORBA::String_var absname = contents[i]->absolute_name ();

    if (_db.is_implicit (absname)) {
      continue;
    }

    if (!is_included_def (contents[i]) || _feed_included_defs ||
	contents[i]->def_kind() == CORBA::dk_Module) {
      dummy1 = copy_Contained (contents[i]);
      if (CORBA::is_nil (dummy1)) {
	return false;
      }
    }
    dummy2 = CORBA::Container::_narrow (contents[i]);
    if (!CORBA::is_nil (dummy2)) {
      if (!copy (dummy2, _feed_included_defs)) {
	return false;
      }
    }
  }

  return true;
}

/*
 * Meta-Transformers
 */

CORBA::Contained_ptr
IRCopy::copy_Contained (CORBA::Contained_ptr src)
{
  CORBA::Contained_ptr res;

  CORBA::String_var id = src->id ();
  CORBA::Contained_var existing = _repo->lookup_id (id);

  if (!_feed_included_defs && is_included_def (src) &&
      src->def_kind() != CORBA::dk_Module) {
    if (CORBA::is_nil (existing)) {
      error  = id.in();
      error += " does not exist in target";
      return CORBA::Contained::_nil ();
    }
    return CORBA::Contained::_duplicate (existing);
  }

  if (!CORBA::is_nil (existing) && src->def_kind() != existing->def_kind()) {
    error  = id.in();
    error += " exists in target, but has the wrong type";
    return CORBA::ModuleDef::_nil ();
  }

  if (!CORBA::is_nil (existing) && visited.find (id.in()) != visited.end()) {
    return CORBA::Contained::_duplicate (existing);
  }

  visited.insert (id.in());

  switch (src->def_kind()) {
  case CORBA::dk_Module: {
    CORBA::ModuleDef_var el = CORBA::ModuleDef::_narrow (src);
    res = copy_Module (el);
    break;
  }

  case CORBA::dk_Constant: {
    CORBA::ConstantDef_var el = CORBA::ConstantDef::_narrow (src);
    res = copy_Constant (el);
    break;
  }

  case CORBA::dk_Struct: {
    CORBA::StructDef_var el = CORBA::StructDef::_narrow (src);
    res = copy_Struct (el);
    break;
  }

  case CORBA::dk_Union: {
    CORBA::UnionDef_var el = CORBA::UnionDef::_narrow (src);
    res = copy_Union (el);
    break;
  }

  case CORBA::dk_Enum: {
    CORBA::EnumDef_var el = CORBA::EnumDef::_narrow (src);
    res = copy_Enum (el);
    break;
  }

  case CORBA::dk_Alias: {
    CORBA::AliasDef_var el = CORBA::AliasDef::_narrow (src);
    res = copy_Alias (el);
    break;
  }

  case CORBA::dk_Exception: {
    CORBA::ExceptionDef_var el = CORBA::ExceptionDef::_narrow (src);
    res = copy_Exception (el);
    break;
  }

  case CORBA::dk_Attribute: {
    CORBA::AttributeDef_var el = CORBA::AttributeDef::_narrow (src);
    res = copy_Attribute (el);
    break;
  }

  case CORBA::dk_Operation: {
    CORBA::OperationDef_var el = CORBA::OperationDef::_narrow (src);
    res = copy_Operation (el);
    break;
  }

  case CORBA::dk_Interface: 
  case CORBA::dk_AbstractInterface: 
  case CORBA::dk_LocalInterface: {
    CORBA::InterfaceDef_var el = CORBA::InterfaceDef::_narrow (src);
    res = copy_Interface (el);
    break;
  }

  case CORBA::dk_ValueMember: {
    CORBA::ValueMemberDef_var el = CORBA::ValueMemberDef::_narrow (src);
    res = copy_ValueMember (el);
    break;
  }

  case CORBA::dk_Value: 
  case CORBA::dk_Event: {
    CORBA::ValueDef_var el = CORBA::ValueDef::_narrow (src);
    res = copy_Value (el);
    break;
  }

  case CORBA::dk_ValueBox: {
    CORBA::ValueBoxDef_var el = CORBA::ValueBoxDef::_narrow (src);
    res = copy_ValueBox (el);
    break;
  }

  case CORBA::dk_Native: {
    CORBA::NativeDef_var el = CORBA::NativeDef::_narrow (src);
    res = copy_Native (el);
    break;
  }

#ifdef USE_CCM
  case CORBA::dk_Provides: {
    CORBA::ComponentIR::ProvidesDef_var el =
      CORBA::ComponentIR::ProvidesDef::_narrow (src);
    res = copy_Provides (el);
    break;
  }

  case CORBA::dk_Uses: {
    CORBA::ComponentIR::UsesDef_var el =
      CORBA::ComponentIR::UsesDef::_narrow (src);
    res = copy_Uses (el);
    break;
  }

  case CORBA::dk_Emits: {
    CORBA::ComponentIR::EmitsDef_var el =
      CORBA::ComponentIR::EmitsDef::_narrow (src);
    res = copy_Emits (el);
    break;
  }

  case CORBA::dk_Publishes: {
    CORBA::ComponentIR::PublishesDef_var el =
      CORBA::ComponentIR::PublishesDef::_narrow (src);
    res = copy_Publishes (el);
    break;
  }

  case CORBA::dk_Consumes: {
    CORBA::ComponentIR::ConsumesDef_var el =
      CORBA::ComponentIR::ConsumesDef::_narrow (src);
    res = copy_Consumes (el);
    break;
  }

  case CORBA::dk_Component: {
    CORBA::ComponentIR::ComponentDef_var el =
      CORBA::ComponentIR::ComponentDef::_narrow (src);
    res = copy_Component (el);
    break;
  }

  case CORBA::dk_Factory: {
    CORBA::ComponentIR::FactoryDef_var el =
      CORBA::ComponentIR::FactoryDef::_narrow (src);
    res = copy_Factory (el);
    break;
  }

  case CORBA::dk_Finder: {
    CORBA::ComponentIR::FinderDef_var el =
      CORBA::ComponentIR::FinderDef::_narrow (src);
    res = copy_Finder (el);
    break;
  }

  case CORBA::dk_Home: {
    CORBA::ComponentIR::HomeDef_var el =
      CORBA::ComponentIR::HomeDef::_narrow (src);
    res = copy_Home (el);
    break;
  }
#endif

  default:
    cout << src->def_kind() << endl;
    assert (0);
    break;
  }

  return res;
}

CORBA::Container_ptr
IRCopy::copy_Container (CORBA::Container_ptr src)
{
  CORBA::Contained_var cs = CORBA::Contained::_narrow (src);
  CORBA::Container_ptr target;

  if (!CORBA::is_nil (cs)) {
    CORBA::Contained_var ct = copy_Contained (cs);
    target = CORBA::Container::_narrow (ct);
  }
  else {
    target = CORBA::Container::_duplicate (_repo);
  }

  return target;
}

CORBA::IDLType_ptr
IRCopy::copy_IDLType (CORBA::IDLType_ptr src)
{
  CORBA::Contained_var cs = CORBA::Contained::_narrow (src);
  CORBA::IDLType_ptr target;

  if (!CORBA::is_nil (cs)) {
    CORBA::Contained_var ct = copy_Contained (cs);
    target = CORBA::IDLType::_narrow (ct);
  }
  else {
    /*
     * Handle anonymous types
     */

    switch (src->def_kind()) {
    case CORBA::dk_Primitive: {
      CORBA::PrimitiveDef_var el = CORBA::PrimitiveDef::_narrow (src);
      target = _repo->get_primitive (el->kind ());
      break;
    }

    case CORBA::dk_String: {
      CORBA::StringDef_var el = CORBA::StringDef::_narrow (src);
      target = _repo->create_string (el->bound());
      break;
    }

    case CORBA::dk_Wstring: {
      CORBA::WstringDef_var el = CORBA::WstringDef::_narrow (src);
      target = _repo->create_wstring (el->bound());
      break;
    }

    case CORBA::dk_Fixed: {
      CORBA::FixedDef_var el = CORBA::FixedDef::_narrow (src);
      target = _repo->create_fixed (el->digits(), el->scale());
      break;
    }

    case CORBA::dk_Sequence: {
      CORBA::SequenceDef_var el = CORBA::SequenceDef::_narrow (src);
      CORBA::IDLType_var st = el->element_type_def ();
      CORBA::IDLType_var tt = copy_IDLType (st);

      if (CORBA::is_nil (tt)) {
	return CORBA::IDLType::_nil ();
      }

      target = _repo->create_sequence (el->bound(), tt);
      break;
    }

    case CORBA::dk_Array: {
      CORBA::ArrayDef_var el = CORBA::ArrayDef::_narrow (src);
      CORBA::IDLType_var st = el->element_type_def ();
      CORBA::IDLType_var tt = copy_IDLType (st);

      if (CORBA::is_nil (tt)) {
	return CORBA::IDLType::_nil ();
      }

      target = _repo->create_array (el->length(), tt);
      break;
    }

    default:
      assert (0);
    }
  }

  return target;
}

/*
 * Transformers for Interface Repository nodes
 */

CORBA::ModuleDef_ptr
IRCopy::copy_Module (CORBA::ModuleDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);
  CORBA::ModuleDef_ptr res = CORBA::ModuleDef::_narrow (existing);

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = copy_Container (defin);

  if (CORBA::is_nil (dest)) {
    return CORBA::ModuleDef::_nil ();
  }

  if (CORBA::is_nil (res)) {
    res = dest->create_module (id, name, version);
  }

  return res;
}

CORBA::ConstantDef_ptr
IRCopy::copy_Constant (CORBA::ConstantDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);
  CORBA::ConstantDef_ptr res = CORBA::ConstantDef::_narrow (existing);

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = copy_Container (defin);

  if (CORBA::is_nil (dest)) {
    return CORBA::ConstantDef::_nil ();
  }

  CORBA::IDLType_var st = src->type_def ();
  CORBA::IDLType_var tt = copy_IDLType (st);

  if (CORBA::is_nil (tt)) {
    return CORBA::ConstantDef::_nil ();
  }

  CORBA::Any_var value = src->value ();

  if (CORBA::is_nil (res)) {
    res = dest->create_constant (id, name, version, tt, value);
  }
  else {
    res->type_def (tt);
    res->value (value);
  }

  return res;
}

CORBA::StructDef_ptr
IRCopy::copy_Struct (CORBA::StructDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);
  CORBA::StructDef_ptr res = CORBA::StructDef::_narrow (existing);

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = copy_Container (defin);

  if (CORBA::is_nil (dest)) {
    return CORBA::StructDef::_nil ();
  }

  CORBA::StructMemberSeq_var sm = src->members ();
  CORBA::StructMemberSeq tm;

  if (CORBA::is_nil (res)) {
    res = dest->create_struct (id, name, version, tm);
  }

  tm.length (sm->length ());

  for (CORBA::ULong i=0; i<sm->length(); i++) {
    tm[i].name = sm[i].name;
    tm[i].type = CORBA::_tc_void;
    tm[i].type_def = copy_IDLType (sm[i].type_def);

    if (CORBA::is_nil (tm[i].type_def)) {
      return CORBA::StructDef::_nil ();
    }
  }

  res->members (tm);
  return res;
}

CORBA::UnionDef_ptr
IRCopy::copy_Union (CORBA::UnionDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);
  CORBA::UnionDef_ptr res = CORBA::UnionDef::_narrow (existing);

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = copy_Container (defin);

  if (CORBA::is_nil (dest)) {
    return CORBA::UnionDef::_nil ();
  }

  CORBA::IDLType_var sd = src->discriminator_type_def ();
  CORBA::IDLType_var td = copy_IDLType (sd);

  if (CORBA::is_nil (td)) {
    return CORBA::UnionDef::_nil ();
  }

  CORBA::UnionMemberSeq_var sm = src->members ();
  CORBA::UnionMemberSeq tm;

  if (CORBA::is_nil (res)) {
    res = dest->create_union (id, name, version, td, tm);
  }

  tm.length (sm->length ());

  for (CORBA::ULong i=0; i<sm->length(); i++) {
    tm[i].name  = sm[i].name;
    tm[i].label = sm[i].label;
    tm[i].type  = CORBA::_tc_void;
    tm[i].type_def = copy_IDLType (sm[i].type_def);

    if (CORBA::is_nil (tm[i].type_def)) {
      return CORBA::UnionDef::_nil ();
    }
  }

  res->discriminator_type_def (td);
  res->members (tm);
  return res;
}

CORBA::EnumDef_ptr
IRCopy::copy_Enum (CORBA::EnumDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);
  CORBA::EnumDef_ptr res = CORBA::EnumDef::_narrow (existing);

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = copy_Container (defin);

  if (CORBA::is_nil (dest)) {
    return CORBA::EnumDef::_nil ();
  }

  CORBA::EnumMemberSeq_var em = src->members ();

  if (CORBA::is_nil (res)) {
    res = dest->create_enum (id, name, version, em);
  }
  else {
    res->members (em);
  }
  
  return res;
}

CORBA::AliasDef_ptr
IRCopy::copy_Alias (CORBA::AliasDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = copy_Container (defin);

  if (CORBA::is_nil (dest)) {
    return CORBA::AliasDef::_nil ();
  }

  CORBA::IDLType_var st = src->original_type_def ();
  CORBA::IDLType_var tt = copy_IDLType (st);

  if (CORBA::is_nil (tt)) {
    return CORBA::AliasDef::_nil ();
  }

  CORBA::Contained_var existing = _repo->lookup_id (id);
  CORBA::AliasDef_ptr res = CORBA::AliasDef::_narrow (existing);

  if (CORBA::is_nil (res)) {
    res = dest->create_alias (id, name, version, tt);
  }
  else {
    res->original_type_def (tt);
  }

  return res;
}

CORBA::NativeDef_ptr
IRCopy::copy_Native (CORBA::NativeDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);
  CORBA::NativeDef_ptr res = CORBA::NativeDef::_narrow (existing);

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = copy_Container (defin);

  if (CORBA::is_nil (dest)) {
    return CORBA::NativeDef::_nil ();
  }

  if (CORBA::is_nil (res)) {
    res = dest->create_native (id, name, version);
  }

  return res;
}

CORBA::ExceptionDef_ptr
IRCopy::copy_Exception (CORBA::ExceptionDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);
  CORBA::ExceptionDef_ptr res = CORBA::ExceptionDef::_narrow (existing);

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = copy_Container (defin);

  if (CORBA::is_nil (dest)) {
    return CORBA::ExceptionDef::_nil ();
  }

  CORBA::StructMemberSeq_var sm = src->members ();
  CORBA::StructMemberSeq tm;

  if (CORBA::is_nil (res)) {
    res = dest->create_exception (id, name, version, tm);
  }

  tm.length (sm->length ());

  for (CORBA::ULong i=0; i<sm->length(); i++) {
    tm[i].name = sm[i].name;
    tm[i].type = CORBA::_tc_void;
    tm[i].type_def = copy_IDLType (sm[i].type_def);

    if (CORBA::is_nil (tm[i].type_def)) {
      return CORBA::ExceptionDef::_nil ();
    }
  }

  res->members (tm);

  return res;
}

CORBA::AttributeDef_ptr
IRCopy::copy_Attribute (CORBA::AttributeDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);
  CORBA::AttributeDef_ptr res = CORBA::AttributeDef::_narrow (existing);

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = copy_Container (defin);
  CORBA::InterfaceDef_var destiface = CORBA::InterfaceDef::_narrow (dest);
  CORBA::ValueDef_var destval = CORBA::ValueDef::_narrow (dest);

  if (CORBA::is_nil (dest)) {
    return CORBA::AttributeDef::_nil ();
  }

  assert (!CORBA::is_nil (destiface) || !CORBA::is_nil (destval));

  CORBA::IDLType_var st = src->type_def ();
  CORBA::IDLType_var tt = copy_IDLType (st);

  if (CORBA::is_nil (tt)) {
    return CORBA::AttributeDef::_nil ();
  }

  if (CORBA::is_nil (res)) {
    if (!CORBA::is_nil (destiface)) {
      res = destiface->create_attribute (id, name, version, tt, src->mode());
    }
    else {
      res = destval->create_attribute (id, name, version, tt, src->mode());
    }
  }
  else {
    res->type_def (tt);
    res->mode (src->mode());
  }

  CORBA::ExtAttributeDef_var ead = CORBA::ExtAttributeDef::_narrow (src);
  CORBA::ExtAttributeDef_var tead = CORBA::ExtAttributeDef::_narrow (res);

  if (!CORBA::is_nil (ead) && !CORBA::is_nil (tead)) {
    CORBA::ExceptionDefSeq_var sge = ead->get_exceptions ();
    CORBA::ExceptionDefSeq_var sse = ead->set_exceptions ();
    CORBA::ExceptionDefSeq tge, tse;
    tge.length (sge->length());
    tse.length (sse->length());

    for (CORBA::ULong i1=0; i1<sge->length(); i1++) {
      tge[i1] = copy_Exception (sge[i1]);
    }

    for (CORBA::ULong i2=0; i2<sse->length(); i2++) {
      tse[i2] = copy_Exception (sse[i2]);
    }

    tead->get_exceptions (tge);
    tead->set_exceptions (tse);
  }

  return res;
}

CORBA::OperationDef_ptr
IRCopy::copy_Operation (CORBA::OperationDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);
  CORBA::OperationDef_ptr res = CORBA::OperationDef::_narrow (existing);

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = copy_Container (defin);
  CORBA::InterfaceDef_var destiface = CORBA::InterfaceDef::_narrow (dest);
  CORBA::ValueDef_var destval = CORBA::ValueDef::_narrow (dest);

  if (CORBA::is_nil (dest)) {
    return CORBA::OperationDef::_nil ();
  }

  assert (!CORBA::is_nil (destiface) || !CORBA::is_nil (destval));

  CORBA::IDLType_var st = src->result_def ();
  CORBA::IDLType_var tt = copy_IDLType (st);

  if (CORBA::is_nil (tt)) {
    return CORBA::OperationDef::_nil ();
  }

  CORBA::ParDescriptionSeq_var sp = src->params ();
  CORBA::ParDescriptionSeq tp;
  tp.length (sp->length());

  for (CORBA::ULong i1=0; i1<sp->length(); i1++) {
    tp[i1].name = sp[i1].name;
    tp[i1].type = CORBA::_tc_void;
    tp[i1].mode = sp[i1].mode;
    tp[i1].type_def = copy_IDLType (sp[i1].type_def);

    if (CORBA::is_nil (tp[i1].type_def)) {
      return CORBA::OperationDef::_nil ();
    }
  }

  CORBA::OperationMode sm = src->mode ();
  CORBA::ContextIdSeq_var sc = src->contexts ();

  CORBA::ExceptionDefSeq_var se = src->exceptions ();
  CORBA::ExceptionDefSeq te;
  te.length (se->length());

  for (CORBA::ULong i2=0; i2<se->length(); i2++) {
    te[i2] = copy_Exception (se[i2]);
    if (CORBA::is_nil (te[i2])) {
      return CORBA::OperationDef::_nil ();
    }
  }

  if (CORBA::is_nil (res)) {
    if (!CORBA::is_nil (destiface)) {
      res = destiface->create_operation (id, name, version,
					 tt, sm, tp, te, sc);
    }
    else {
      res = destval->create_operation (id, name, version,
				       tt, sm, tp, te, sc);
    }
  }
  else {
    res->result_def (tt);
    res->params (tp);
    res->mode (sm);
    res->exceptions (te);
    res->contexts (sc);
  }

  return res;
}

CORBA::InterfaceDef_ptr
IRCopy::copy_Interface (CORBA::InterfaceDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);
  CORBA::InterfaceDef_ptr res = CORBA::InterfaceDef::_narrow (existing);

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = copy_Container (defin);

  if (CORBA::is_nil (dest)) {
    return CORBA::InterfaceDef::_nil ();
  }

  CORBA::InterfaceDefSeq_var sb = src->base_interfaces ();
  CORBA::InterfaceDefSeq tb;
  tb.length (sb->length());

  for (CORBA::ULong i=0; i<sb->length(); i++) {
    CORBA::IDLType_var ttmp = copy_IDLType (sb[i]);
    tb[i] = CORBA::InterfaceDef::_narrow (ttmp);

    if (CORBA::is_nil (tb[i])) {
      return CORBA::InterfaceDef::_nil ();
    }
  }

  if (CORBA::is_nil (res)) {
    switch (src->def_kind()) {
    case CORBA::dk_Interface: {
      res = dest->create_interface (id, name, version, tb);
      break;
    }

    case CORBA::dk_AbstractInterface: {
      CORBA::AbstractInterfaceDefSeq ab;
      res = dest->create_abstract_interface (id, name, version, ab);
      res->base_interfaces (tb);
      break;
    }

    case CORBA::dk_LocalInterface: {
      res = dest->create_local_interface (id, name, version, tb);
      break;
    }
      
    default:
      assert (0);
      break;
    }
  }
  else {
    res->base_interfaces (tb);
  }

  return res;
}

CORBA::ValueMemberDef_ptr
IRCopy::copy_ValueMember (CORBA::ValueMemberDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);
  CORBA::ValueMemberDef_ptr res = CORBA::ValueMemberDef::_narrow (existing);

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = copy_Container (defin);
  CORBA::ValueDef_var destval = CORBA::ValueDef::_narrow (dest);

  if (CORBA::is_nil (dest)) {
    return CORBA::ValueMemberDef::_nil ();
  }

  assert (!CORBA::is_nil (destval));

  CORBA::IDLType_var st = src->type_def ();
  CORBA::IDLType_var tt = copy_IDLType (st);

  if (CORBA::is_nil (tt)) {
    return CORBA::ValueMemberDef::_nil ();
  }

  if (CORBA::is_nil (res)) {
    res = destval->create_value_member (id, name, version, tt, src->access());
  }
  else {
    res->type_def (tt);
    res->access (src->access());
  }

  return res;
}
  
CORBA::ValueDef_ptr
IRCopy::copy_Value (CORBA::ValueDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);
  CORBA::ValueDef_ptr res = CORBA::ValueDef::_narrow (existing);

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = copy_Container (defin);

  if (CORBA::is_nil (dest)) {
    return CORBA::ValueDef::_nil ();
  }

  CORBA::InterfaceDefSeq_var ss = src->supported_interfaces ();
  CORBA::InterfaceDefSeq ts;
  ts.length (ss->length());

  for (CORBA::ULong i1=0; i1<ss->length(); i1++) {
    CORBA::IDLType_var ttmp = copy_IDLType (ss[i1]);
    ts[i1] = CORBA::InterfaceDef::_narrow (ttmp);

    if (CORBA::is_nil (ts[i1])) {
      return CORBA::ValueDef::_nil ();
    }
  }

  CORBA::ValueDef_var sb = src->base_value ();
  CORBA::ValueDef_var tb = CORBA::ValueDef::_nil ();

  if (!CORBA::is_nil (sb)) {
    CORBA::IDLType_var ttmp = copy_IDLType (sb);
    tb = CORBA::ValueDef::_narrow (ttmp);

    if (CORBA::is_nil (tb)) {
      return CORBA::ValueDef::_nil ();
    }
  }

  CORBA::ValueDefSeq_var sab = src->abstract_base_values ();
  CORBA::ValueDefSeq tab;
  tab.length (sab->length ());

  for (CORBA::ULong i3=0; i3<sab->length(); i3++) {
    CORBA::IDLType_var ttmp = copy_IDLType (sab[i3]);
    tab[i3] = CORBA::ValueDef::_narrow (ttmp);

    if (CORBA::is_nil (tab[i3])) {
      return CORBA::ValueDef::_nil ();
    }
  }

  /*
   * Defer setting the initializers; they might refer to the value,
   * for example in a tree node, where an initializer takes pointers
   * to the child nodes.
   */

  CORBA::InitializerSeq ti;

  if (CORBA::is_nil (res)) {
#ifdef USE_CCM
    if (src->def_kind() == CORBA::dk_Event) {
      CORBA::ComponentIR::Container_var cdest =
	CORBA::ComponentIR::Container::_narrow (dest);
     
      if (CORBA::is_nil (cdest)) {
	return CORBA::ValueDef::_nil ();
      }

      CORBA::ExtInitializerSeq eti;

      res = cdest->create_event (id, name, version,
				 src->is_custom (),
				 src->is_abstract (),
				 tb,
				 src->is_truncatable (),
				 tab,
				 ts,
				 eti);
    }
    else {
#endif
      res = dest->create_value (id, name, version,
				src->is_custom (),
				src->is_abstract (),
				tb,
				src->is_truncatable (),
				tab,
				ts,
				ti);
#ifdef USE_CCM
    }
#endif
  }
  else {
    res->is_custom (src->is_custom());
    res->is_abstract (src->is_abstract());
    res->base_value (tb);
    res->is_truncatable (src->is_truncatable());
    res->abstract_base_values (tab);
    res->supported_interfaces (ts);
  }

  CORBA::InitializerSeq_var si = src->initializers ();
  ti.length (si->length());

  for (CORBA::ULong i2=0; i2<si->length(); i2++) {
    ti[i2].name = si[i2].name;
    ti[i2].members.length (si[i2].members.length());

    for (CORBA::ULong j=0; j<si[i2].members.length(); j++) {
      ti[i2].members[j].name = si[i2].members[j].name;
      ti[i2].members[j].type = CORBA::_tc_void;
      ti[i2].members[j].type_def = copy_IDLType (si[i2].members[j].type_def);

      if (CORBA::is_nil (ti[i2].members[j].type_def)) {
	return CORBA::ValueDef::_nil ();
      }
    }
  }

  res->initializers (ti);
  return res;
}

CORBA::ValueBoxDef_ptr
IRCopy::copy_ValueBox (CORBA::ValueBoxDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);
  CORBA::ValueBoxDef_ptr res = CORBA::ValueBoxDef::_narrow (existing);

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = copy_Container (defin);

  if (CORBA::is_nil (dest)) {
    return CORBA::ValueBoxDef::_nil ();
  }

  CORBA::IDLType_var st = src->original_type_def ();
  CORBA::IDLType_var tt = copy_IDLType (st);

  if (CORBA::is_nil (tt)) {
    return CORBA::ValueBoxDef::_nil ();
  }

  if (CORBA::is_nil (res)) {
    res = dest->create_value_box (id, name, version, tt);
  }
  else {
    res->original_type_def (tt);
  }

  return res;
}

#ifdef USE_CCM
CORBA::ComponentIR::ProvidesDef_ptr
IRCopy::copy_Provides (CORBA::ComponentIR::ProvidesDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);
  CORBA::ComponentIR::ProvidesDef_ptr res =
    CORBA::ComponentIR::ProvidesDef::_narrow (existing);

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = copy_Container (defin);
  CORBA::ComponentIR::ComponentDef_var cdest =
    CORBA::ComponentIR::ComponentDef::_narrow (dest);

  if (CORBA::is_nil (cdest)) {
    return CORBA::ComponentIR::ProvidesDef::_nil ();
  }

  CORBA::IDLType_var st = src->interface_type ();
  CORBA::IDLType_var ttmp = copy_IDLType (st);
  CORBA::InterfaceDef_var tt = CORBA::InterfaceDef::_narrow (ttmp);

  if (CORBA::is_nil (tt)) {
    return CORBA::ComponentIR::ProvidesDef::_nil ();
  }

  if (CORBA::is_nil (res)) {
    res = cdest->create_provides (id, name, version, tt);
  }
  else {
    res->interface_type (tt);
  }

  return res;
}

CORBA::ComponentIR::UsesDef_ptr
IRCopy::copy_Uses (CORBA::ComponentIR::UsesDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);
  CORBA::ComponentIR::UsesDef_ptr res =
    CORBA::ComponentIR::UsesDef::_narrow (existing);

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = copy_Container (defin);
  CORBA::ComponentIR::ComponentDef_var cdest =
    CORBA::ComponentIR::ComponentDef::_narrow (dest);

  if (CORBA::is_nil (cdest)) {
    return CORBA::ComponentIR::UsesDef::_nil ();
  }

  CORBA::IDLType_var st = src->interface_type ();
  CORBA::IDLType_var ttmp = copy_IDLType (st);
  CORBA::InterfaceDef_var tt = CORBA::InterfaceDef::_narrow (ttmp);

  if (CORBA::is_nil (tt)) {
    return CORBA::ComponentIR::UsesDef::_nil ();
  }

  if (CORBA::is_nil (res)) {
    res = cdest->create_uses (id, name, version, tt, src->is_multiple());
  }
  else {
    res->interface_type (tt);
    res->is_multiple (src->is_multiple());
  }

  return res;
}

CORBA::ComponentIR::EmitsDef_ptr
IRCopy::copy_Emits (CORBA::ComponentIR::EmitsDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);
  CORBA::ComponentIR::EmitsDef_ptr res =
    CORBA::ComponentIR::EmitsDef::_narrow (existing);

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = copy_Container (defin);
  CORBA::ComponentIR::ComponentDef_var cdest =
    CORBA::ComponentIR::ComponentDef::_narrow (dest);

  if (CORBA::is_nil (cdest)) {
    return CORBA::ComponentIR::EmitsDef::_nil ();
  }

  CORBA::IDLType_var st = src->event ();
  CORBA::IDLType_var ttmp = copy_IDLType (st);
  CORBA::ComponentIR::EventDef_var tt =
    CORBA::ComponentIR::EventDef::_narrow (ttmp);

  if (CORBA::is_nil (tt)) {
    return CORBA::ComponentIR::EmitsDef::_nil ();
  }

  if (CORBA::is_nil (res)) {
    res = cdest->create_emits (id, name, version, tt);
  }
  else {
    res->event (tt);
  }

  return res;
}

CORBA::ComponentIR::PublishesDef_ptr
IRCopy::copy_Publishes (CORBA::ComponentIR::PublishesDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);
  CORBA::ComponentIR::PublishesDef_ptr res =
    CORBA::ComponentIR::PublishesDef::_narrow (existing);

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = copy_Container (defin);
  CORBA::ComponentIR::ComponentDef_var cdest =
    CORBA::ComponentIR::ComponentDef::_narrow (dest);

  if (CORBA::is_nil (cdest)) {
    return CORBA::ComponentIR::PublishesDef::_nil ();
  }

  CORBA::IDLType_var st = src->event ();
  CORBA::IDLType_var ttmp = copy_IDLType (st);
  CORBA::ComponentIR::EventDef_var tt =
    CORBA::ComponentIR::EventDef::_narrow (ttmp);

  if (CORBA::is_nil (tt)) {
    return CORBA::ComponentIR::PublishesDef::_nil ();
  }

  if (CORBA::is_nil (res)) {
    res = cdest->create_publishes (id, name, version, tt);
  }
  else {
    res->event (tt);
  }

  return res;
}

CORBA::ComponentIR::ConsumesDef_ptr
IRCopy::copy_Consumes (CORBA::ComponentIR::ConsumesDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);
  CORBA::ComponentIR::ConsumesDef_ptr res =
    CORBA::ComponentIR::ConsumesDef::_narrow (existing);

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = copy_Container (defin);
  CORBA::ComponentIR::ComponentDef_var cdest =
    CORBA::ComponentIR::ComponentDef::_narrow (dest);

  if (CORBA::is_nil (cdest)) {
    return CORBA::ComponentIR::ConsumesDef::_nil ();
  }

  CORBA::IDLType_var st = src->event ();
  CORBA::IDLType_var ttmp = copy_IDLType (st);
  CORBA::ComponentIR::EventDef_var tt =
    CORBA::ComponentIR::EventDef::_narrow (ttmp);

  if (CORBA::is_nil (tt)) {
    return CORBA::ComponentIR::ConsumesDef::_nil ();
  }

  if (CORBA::is_nil (res)) {
    res = cdest->create_consumes (id, name, version, tt);
  }
  else {
    res->event (tt);
  }

  return res;
}

CORBA::ComponentIR::ComponentDef_ptr
IRCopy::copy_Component (CORBA::ComponentIR::ComponentDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);
  CORBA::ComponentIR::ComponentDef_ptr res =
    CORBA::ComponentIR::ComponentDef::_narrow (existing);

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = copy_Container (defin);
  CORBA::ComponentIR::Container_var cdest =
    CORBA::ComponentIR::Container::_narrow (dest);

  if (CORBA::is_nil (cdest)) {
    return CORBA::ComponentIR::ComponentDef::_nil ();
  }

  CORBA::IDLType_var sb = src->base_component ();
  CORBA::ComponentIR::ComponentDef_var tb =
    CORBA::ComponentIR::ComponentDef::_nil ();

  if (!CORBA::is_nil (sb)) {
    CORBA::IDLType_var ttmp = copy_IDLType (sb);
    tb = CORBA::ComponentIR::ComponentDef::_narrow (ttmp);

    if (CORBA::is_nil (tb)) {
      return CORBA::ComponentIR::ComponentDef::_nil ();
    }
  }

  CORBA::InterfaceDefSeq_var ss = src->supported_interfaces ();
  CORBA::InterfaceDefSeq ts;
  ts.length (ss->length());

  for (CORBA::ULong i=0; i<ss->length(); i++) {
    CORBA::IDLType_var ttmp = copy_IDLType (ss[i]);
    ts[i] = CORBA::InterfaceDef::_narrow (ttmp);

    if (CORBA::is_nil (ts[i])) {
      return CORBA::ComponentIR::ComponentDef::_nil ();
    }
  }

  if (CORBA::is_nil (res)) {
    res = cdest->create_component (id, name, version, tb, ts);
  }
  else {
    res->base_component (tb);
    res->supported_interfaces (ts);
  }

  return res;
}

CORBA::ComponentIR::FactoryDef_ptr
IRCopy::copy_Factory (CORBA::ComponentIR::FactoryDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);
  CORBA::ComponentIR::FactoryDef_ptr res =
    CORBA::ComponentIR::FactoryDef::_narrow (existing);

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = copy_Container (defin);
  CORBA::ComponentIR::HomeDef_ptr cdest =
    CORBA::ComponentIR::HomeDef::_narrow (dest);

  if (CORBA::is_nil (cdest)) {
    return CORBA::ComponentIR::FactoryDef::_nil ();
  }

  CORBA::ParDescriptionSeq_var sp = src->params ();
  CORBA::ParDescriptionSeq tp;
  tp.length (sp->length());

  for (CORBA::ULong i1=0; i1<sp->length(); i1++) {
    tp[i1].name = sp[i1].name;
    tp[i1].type = CORBA::_tc_void;
    tp[i1].mode = sp[i1].mode;
    tp[i1].type_def = copy_IDLType (sp[i1].type_def);

    if (CORBA::is_nil (tp[i1].type_def)) {
      return CORBA::ComponentIR::FactoryDef::_nil ();
    }
  }

  CORBA::ExceptionDefSeq_var se = src->exceptions ();
  CORBA::ExceptionDefSeq te;
  te.length (se->length());

  for (CORBA::ULong i2=0; i2<se->length(); i2++) {
    te[i2] = copy_Exception (se[i2]);
    if (CORBA::is_nil (te[i2])) {
      return CORBA::ComponentIR::FactoryDef::_nil ();
    }
  }

  if (CORBA::is_nil (res)) {
    res = cdest->create_factory (id, name, version, tp, te);
  }
  else {
    res->params (tp);
    res->exceptions (se);
  }

  return res;
}

CORBA::ComponentIR::FinderDef_ptr
IRCopy::copy_Finder (CORBA::ComponentIR::FinderDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);
  CORBA::ComponentIR::FinderDef_ptr res =
    CORBA::ComponentIR::FinderDef::_narrow (existing);

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = copy_Container (defin);
  CORBA::ComponentIR::HomeDef_ptr cdest =
    CORBA::ComponentIR::HomeDef::_narrow (dest);

  if (CORBA::is_nil (cdest)) {
    return CORBA::ComponentIR::FinderDef::_nil ();
  }

  CORBA::ParDescriptionSeq_var sp = src->params ();
  CORBA::ParDescriptionSeq tp;
  tp.length (sp->length());

  for (CORBA::ULong i1=0; i1<sp->length(); i1++) {
    tp[i1].name = sp[i1].name;
    tp[i1].type = CORBA::_tc_void;
    tp[i1].mode = sp[i1].mode;
    tp[i1].type_def = copy_IDLType (sp[i1].type_def);

    if (CORBA::is_nil (tp[i1].type_def)) {
      return CORBA::ComponentIR::FinderDef::_nil ();
    }
  }

  CORBA::ExceptionDefSeq_var se = src->exceptions ();
  CORBA::ExceptionDefSeq te;
  te.length (se->length());

  for (CORBA::ULong i2=0; i2<se->length(); i2++) {
    te[i2] = copy_Exception (se[i2]);
    if (CORBA::is_nil (te[i2])) {
      return CORBA::ComponentIR::FinderDef::_nil ();
    }
  }

  if (CORBA::is_nil (res)) {
    res = cdest->create_finder (id, name, version, tp, te);
  }
  else {
    res->params (tp);
    res->exceptions (se);
  }

  return res;
}

CORBA::ComponentIR::HomeDef_ptr
IRCopy::copy_Home (CORBA::ComponentIR::HomeDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);
  CORBA::ComponentIR::HomeDef_ptr res =
    CORBA::ComponentIR::HomeDef::_narrow (existing);

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = copy_Container (defin);
  CORBA::ComponentIR::Container_var cdest =
    CORBA::ComponentIR::Container::_narrow (dest);

  if (CORBA::is_nil (cdest)) {
    return CORBA::ComponentIR::HomeDef::_nil ();
  }

  CORBA::IDLType_var sb = src->base_home ();
  CORBA::ComponentIR::HomeDef_var tb =
    CORBA::ComponentIR::HomeDef::_nil ();

  if (!CORBA::is_nil (sb)) {
    CORBA::IDLType_var ttmp = copy_IDLType (sb);
    tb = CORBA::ComponentIR::HomeDef::_narrow (ttmp);

    if (CORBA::is_nil (tb)) {
      return CORBA::ComponentIR::HomeDef::_nil ();
    }
  }

  CORBA::InterfaceDefSeq_var ss = src->supported_interfaces ();
  CORBA::InterfaceDefSeq ts;
  ts.length (ss->length());

  for (CORBA::ULong i=0; i<ss->length(); i++) {
    CORBA::IDLType_var ttmp = copy_IDLType (ss[i]);
    ts[i] = CORBA::InterfaceDef::_narrow (ttmp);

    if (CORBA::is_nil (ts[i])) {
      return CORBA::ComponentIR::HomeDef::_nil ();
    }
  }

  CORBA::IDLType_var sm = src->managed_component ();
  CORBA::ComponentIR::ComponentDef_var tm =
    CORBA::ComponentIR::ComponentDef::_nil ();

  if (!CORBA::is_nil (sm)) {
    CORBA::IDLType_var ttmp = copy_IDLType (sm);
    tm = CORBA::ComponentIR::ComponentDef::_narrow (ttmp);

    if (CORBA::is_nil (tm)) {
      return CORBA::ComponentIR::HomeDef::_nil ();
    }
  }

  CORBA::IDLType_var sk = src->primary_key ();
  CORBA::ValueDef_var tk = CORBA::ValueDef::_nil ();

  if (!CORBA::is_nil (sk)) {
    CORBA::IDLType_var ttmp = copy_IDLType (sk);
    tk = CORBA::ValueDef::_narrow (ttmp);

    if (CORBA::is_nil (tk)) {
      return CORBA::ComponentIR::HomeDef::_nil ();
    }
  }

  if (CORBA::is_nil (res)) {
    res = cdest->create_home (id, name, version, tb, tm, ts, tk);
  }
  else {
    res->base_home (tb);
    res->managed_component (tm);
    res->primary_key (tk);
  }

  return res;
}
#endif

/*
 * ----------------------------------------------------------------------
 * Main
 * ----------------------------------------------------------------------
 */

CORBA::Container_ptr
IRCopier (DB & db, IDLParam & params,
	  CORBA::Repository_ptr repo,
	  CORBA::Container_ptr cont)
{
  IRCopy cc (db, repo);

  if (!cc.copy (cont, params.feed_included_defs)) {
    CORBA::String_var err = cc.get_error ();
    cerr << "error in IR copy: " << err << endl;
    exit (1);
  }

  /*
   * Find the input container in the new repository and return it
   */

  CORBA::Container_var res;
  CORBA::Contained_var ci = CORBA::Contained::_narrow (cont);

  if (!CORBA::is_nil (ci)) {
    CORBA::String_var id = ci->id ();
    ci  = repo->lookup_id (id);
    res = CORBA::Container::_narrow (ci);
    assert (!CORBA::is_nil (res));
  }
  else {
    res = CORBA::Container::_duplicate (repo);
  }

  return res._retn();
}


