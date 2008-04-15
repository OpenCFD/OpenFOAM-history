/*
 *  Mico --- an Open Source CORBA implementation
 *  Copyright Frank Pilhofer
 *
 *  This file is part of MicoCCM, written by Frank Pilhofer.
 *  The MICO CORBA Component Project was sponsored by Alcatel.
 */

#include <CORBA.h>
#include <mico/ir3.h>
#include <mico/ir_creator.h>
#include <mico/util.h>
#include <mico/os-misc.h>
#include "db.h"
#include "parser.h"
#include "params.h"
#include "idlparser.h"


using namespace std;

class RepoId {
public:
  RepoId (const char *, CORBA::IRObject_ptr = CORBA::IRObject::_nil());
  const char * get ();
  string & sget ();

  string prefix;
  vector<string> container;
  string name;
  string version;

private:
  string complete;
};

RepoId::RepoId (const char * id, CORBA::IRObject_ptr obj)
{
  string repoid (id);

  assert (id[0] == 'I' && id[1] == 'D' &&
	  id[2] == 'L' && id[3] == ':');
  assert (id[strlen(id)-4] == ':');

  version = repoid.substr (repoid.length() - 3);

  repoid = repoid.substr (4);
  repoid = repoid.substr (0, repoid.length() - 4);

  int pos = repoid.find_last_of ("/");

  if (pos == -1) {
    name = repoid;
    return;
  }

  name = repoid.substr (pos+1);
  repoid = repoid.substr (0, pos);

  /*
   * If obj is non-null, it can be used to deduce the name of the outer-
   * most scope, and we can use that knowledge to split off the prefix.
   * Otherwise, the heuristic that the prefix includes a "." (as in
   * omg.org), but that a scope does not.
   */

  CORBA::Contained_var c1 = CORBA::Contained::_narrow (obj);
  CORBA::Container_var c2;
  string outermost_scope;

  if (!CORBA::is_nil (c1)) {
    c2 = c1->defined_in ();

    while (c2->def_kind() != CORBA::dk_Repository) {
      c1 = CORBA::Contained::_narrow (c2);
      c2 = c1->defined_in ();
    }

    CORBA::String_var tmp = c1->name ();
    outermost_scope = tmp.in();
  }

  if (outermost_scope.length() && outermost_scope == name) {
    prefix = repoid;
    return;
  }

  while (repoid.length()) {
    string scope_name;

    if ((pos = repoid.find_last_of ("/")) == -1) {
      scope_name = repoid;
      repoid = "";
    }
    else {
      scope_name = repoid.substr (pos+1);
      repoid = repoid.substr (0, pos);
    }

    if (CORBA::is_nil (c1) && scope_name.find (".") != string::npos) {
      prefix = scope_name;
      break;
    }

    container.insert (container.begin(), scope_name);

    if (outermost_scope.length() && scope_name == outermost_scope) {
      prefix = repoid;
      break;
    }
  }
}

const char *
RepoId::get ()
{
  return sget().c_str();
}

string &
RepoId::sget ()
{
  complete = "IDL:";

  if (prefix.length()) {
    complete += prefix;
    complete += "/";
  }

  for (unsigned int i=0; i<container.size(); i++) {
    complete += container[i];
    complete += "/";
  }

  complete += name;
  complete += ":";
  complete += version;

  return complete;
}

/*
 * IDL 3 to IDL 2 Transformer
 */

class CCMTransformer {
public:
  CCMTransformer (DB & db, CORBA::Repository_ptr targetrepo);

  bool transform (CORBA::Container_ptr src);

private:
  CORBA::Contained_ptr   transform_Contained (CORBA::Contained_ptr src);
  CORBA::Container_ptr   transform_Container (CORBA::Container_ptr src);
  CORBA::IDLType_ptr     transform_IDLType   (CORBA::IDLType_ptr src);

  CORBA::ModuleDef_ptr   transform_Module    (CORBA::ModuleDef_ptr src);
  CORBA::ConstantDef_ptr transform_Constant  (CORBA::ConstantDef_ptr src);
  CORBA::StructDef_ptr   transform_Struct    (CORBA::StructDef_ptr src);
  CORBA::UnionDef_ptr    transform_Union     (CORBA::UnionDef_ptr src);
  CORBA::EnumDef_ptr     transform_Enum      (CORBA::EnumDef_ptr src);
  CORBA::AliasDef_ptr    transform_Alias     (CORBA::AliasDef_ptr src);
  CORBA::ExceptionDef_ptr transform_Exception (CORBA::ExceptionDef_ptr src);
  CORBA::AttributeDef_ptr transform_Attribute (CORBA::AttributeDef_ptr src);
  CORBA::OperationDef_ptr transform_Operation (CORBA::OperationDef_ptr src);
  CORBA::InterfaceDef_ptr transform_Interface (CORBA::InterfaceDef_ptr src);
  CORBA::ValueMemberDef_ptr transform_ValueMember (CORBA::ValueMemberDef_ptr src);
  CORBA::ValueDef_ptr     transform_Value     (CORBA::ValueDef_ptr src);
  CORBA::ValueBoxDef_ptr  transform_ValueBox  (CORBA::ValueBoxDef_ptr src);

  CORBA::OperationDef_ptr
  transform_Provides (CORBA::ComponentIR::ProvidesDef_ptr src);

  CORBA::OperationDef_ptr
  transform_Uses (CORBA::ComponentIR::UsesDef_ptr src);

  CORBA::ValueDef_ptr
  transform_Event (CORBA::ComponentIR::EventDef_ptr src);

  CORBA::OperationDef_ptr
  transform_Emits (CORBA::ComponentIR::EmitsDef_ptr src);

  CORBA::OperationDef_ptr
  transform_Publishes (CORBA::ComponentIR::PublishesDef_ptr src);

  CORBA::OperationDef_ptr
  transform_Consumes (CORBA::ComponentIR::ConsumesDef_ptr src);

  CORBA::InterfaceDef_ptr
  transform_Component (CORBA::ComponentIR::ComponentDef_ptr src);

  CORBA::OperationDef_ptr
  transform_Factory (CORBA::ComponentIR::FactoryDef_ptr src);

  CORBA::OperationDef_ptr
  transform_Finder (CORBA::ComponentIR::FinderDef_ptr src);

  CORBA::InterfaceDef_ptr
  transform_Home (CORBA::ComponentIR::HomeDef_ptr src);

  DB & _db;
  CORBA::Repository_var _repo;
};

CCMTransformer::CCMTransformer (DB & db, CORBA::Repository_ptr targetrepo)
  : _db (db)
{
  _repo = CORBA::Repository::_duplicate (targetrepo);
}

/*
 * Traverse the Interface Repository in Pre-Order
 */

bool
CCMTransformer::transform (CORBA::Container_ptr src)
{
  CORBA::Contained_var cs = CORBA::Contained::_narrow (src);

  if (!CORBA::is_nil (cs)) {
    CORBA::Contained_var tc = transform_Contained (cs);
  }

  CORBA::ContainedSeq_var contents = src->contents (CORBA::dk_all, 1);
  CORBA::Contained_var dummy1;
  CORBA::Container_var dummy2;

  for (CORBA::ULong i=0; i<contents->length(); i++) {
    CORBA::String_var absname = contents[i]->absolute_name ();

    if (_db.is_implicit (absname)) {
      continue;
    }

    dummy1 = transform_Contained (contents[i]);
    dummy2 = CORBA::Container::_narrow (contents[i]);
    if (!CORBA::is_nil (dummy2)) {
      if (!transform (dummy2)) {
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
CCMTransformer::transform_Contained (CORBA::Contained_ptr src)
{
  CORBA::String_var absname = src->absolute_name ();
  CORBA::Contained_ptr res;

  switch (src->def_kind()) {
  case CORBA::dk_Module: {
    CORBA::ModuleDef_var el = CORBA::ModuleDef::_narrow (src);
    res = transform_Module (el);
    break;
  }

  case CORBA::dk_Constant: {
    CORBA::ConstantDef_var el = CORBA::ConstantDef::_narrow (src);
    res = transform_Constant (el);
    break;
  }

  case CORBA::dk_Struct: {
    CORBA::StructDef_var el = CORBA::StructDef::_narrow (src);
    res = transform_Struct (el);
    break;
  }

  case CORBA::dk_Union: {
    CORBA::UnionDef_var el = CORBA::UnionDef::_narrow (src);
    res = transform_Union (el);
    break;
  }

  case CORBA::dk_Enum: {
    CORBA::EnumDef_var el = CORBA::EnumDef::_narrow (src);
    res = transform_Enum (el);
    break;
  }

  case CORBA::dk_Alias: {
    CORBA::AliasDef_var el = CORBA::AliasDef::_narrow (src);
    res = transform_Alias (el);
    break;
  }

  case CORBA::dk_Exception: {
    CORBA::ExceptionDef_var el = CORBA::ExceptionDef::_narrow (src);
    res = transform_Exception (el);
    break;
  }

  case CORBA::dk_Attribute: {
    CORBA::AttributeDef_var el = CORBA::AttributeDef::_narrow (src);
    res = transform_Attribute (el);
    break;
  }

  case CORBA::dk_Operation: {
    CORBA::OperationDef_var el = CORBA::OperationDef::_narrow (src);
    res = transform_Operation (el);
    break;
  }

  case CORBA::dk_Interface: 
  case CORBA::dk_AbstractInterface: 
  case CORBA::dk_LocalInterface: {
    CORBA::InterfaceDef_var el = CORBA::InterfaceDef::_narrow (src);
    res = transform_Interface (el);
    break;
  }

  case CORBA::dk_ValueMember: {
    CORBA::ValueMemberDef_var el = CORBA::ValueMemberDef::_narrow (src);
    res = transform_ValueMember (el);
    break;
  }

  case CORBA::dk_Value: {
    CORBA::ValueDef_var el = CORBA::ValueDef::_narrow (src);
    res = transform_Value (el);
    break;
  }

  case CORBA::dk_ValueBox: {
    CORBA::ValueBoxDef_var el = CORBA::ValueBoxDef::_narrow (src);
    res = transform_ValueBox (el);
    break;
  }

  case CORBA::dk_Provides: {
    CORBA::ComponentIR::ProvidesDef_var el =
      CORBA::ComponentIR::ProvidesDef::_narrow (src);
    res = transform_Provides (el);
    break;
  }

  case CORBA::dk_Uses: {
    CORBA::ComponentIR::UsesDef_var el =
      CORBA::ComponentIR::UsesDef::_narrow (src);
    res = transform_Uses (el);
    break;
  }

  case CORBA::dk_Event: {
    CORBA::ComponentIR::EventDef_var el =
      CORBA::ComponentIR::EventDef::_narrow (src);
    res = transform_Event (el);
    break;
  }

  case CORBA::dk_Emits: {
    CORBA::ComponentIR::EmitsDef_var el =
      CORBA::ComponentIR::EmitsDef::_narrow (src);
    res = transform_Emits (el);
    break;
  }

  case CORBA::dk_Publishes: {
    CORBA::ComponentIR::PublishesDef_var el =
      CORBA::ComponentIR::PublishesDef::_narrow (src);
    res = transform_Publishes (el);
    break;
  }

  case CORBA::dk_Consumes: {
    CORBA::ComponentIR::ConsumesDef_var el =
      CORBA::ComponentIR::ConsumesDef::_narrow (src);
    res = transform_Consumes (el);
    break;
  }

  case CORBA::dk_Component: {
    CORBA::ComponentIR::ComponentDef_var el =
      CORBA::ComponentIR::ComponentDef::_narrow (src);
    res = transform_Component (el);
    break;
  }

  case CORBA::dk_Factory: {
    CORBA::ComponentIR::FactoryDef_var el =
      CORBA::ComponentIR::FactoryDef::_narrow (src);
    res = transform_Factory (el);
    break;
  }

  case CORBA::dk_Finder: {
    CORBA::ComponentIR::FinderDef_var el =
      CORBA::ComponentIR::FinderDef::_narrow (src);
    res = transform_Finder (el);
    break;
  }

  case CORBA::dk_Home: {
    CORBA::ComponentIR::HomeDef_var el =
      CORBA::ComponentIR::HomeDef::_narrow (src);
    res = transform_Home (el);
    break;
  }

  default:
    assert (0);
    break;
  }

  return res;
}

CORBA::Container_ptr
CCMTransformer::transform_Container (CORBA::Container_ptr src)
{
  CORBA::Contained_var cs = CORBA::Contained::_narrow (src);
  CORBA::Container_ptr target;

  if (!CORBA::is_nil (cs)) {
    CORBA::Contained_var ct = transform_Contained (cs);
    target = CORBA::Container::_narrow (ct);
  }
  else {
    target = CORBA::Container::_duplicate (_repo);
  }

  assert (!CORBA::is_nil (target));
  return target;
}

CORBA::IDLType_ptr
CCMTransformer::transform_IDLType (CORBA::IDLType_ptr src)
{
  CORBA::Contained_var cs = CORBA::Contained::_narrow (src);
  CORBA::IDLType_ptr target;

  if (!CORBA::is_nil (cs)) {
    CORBA::Contained_var ct = transform_Contained (cs);
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
      CORBA::IDLType_var tt = transform_IDLType (st);
      target = _repo->create_sequence (el->bound(), tt);
      break;
    }

    case CORBA::dk_Array: {
      CORBA::ArrayDef_var el = CORBA::ArrayDef::_narrow (src);
      CORBA::IDLType_var st = el->element_type_def ();
      CORBA::IDLType_var tt = transform_IDLType (st);
      target = _repo->create_array (el->length(), tt);
      break;
    }

    default:
      assert (0);
    }
  }

  assert (!CORBA::is_nil (target));
  return target;
}

/*
 * Transformers for Interface Repository nodes
 */

CORBA::ModuleDef_ptr
CCMTransformer::transform_Module (CORBA::ModuleDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = transform_Container (defin);

  CORBA::Contained_var existing = _repo->lookup_id (id);

  if (!CORBA::is_nil (existing)) {
    return CORBA::ModuleDef::_narrow (existing);
  }

  return dest->create_module (id, name, version);
}

CORBA::ConstantDef_ptr
CCMTransformer::transform_Constant (CORBA::ConstantDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);

  if (!CORBA::is_nil (existing)) {
    return CORBA::ConstantDef::_narrow (existing);
  }

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = transform_Container (defin);

  CORBA::IDLType_var st = src->type_def ();
  CORBA::IDLType_var tt = transform_IDLType (st);

  CORBA::Any_var value = src->value ();

  return dest->create_constant (id, name, version, tt, value);
}

CORBA::StructDef_ptr
CCMTransformer::transform_Struct (CORBA::StructDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();
  CORBA::StructDef_ptr res;

  CORBA::Contained_var existing = _repo->lookup_id (id);

  if (!CORBA::is_nil (existing)) {
    return CORBA::StructDef::_narrow (existing);
  }

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = transform_Container (defin);

  CORBA::StructMemberSeq_var sm = src->members ();
  CORBA::StructMemberSeq tm;

  res = dest->create_struct (id, name, version, tm);

  tm.length (sm->length ());

  for (CORBA::ULong i=0; i<sm->length(); i++) {
    tm[i].name = sm[i].name;
    tm[i].type = CORBA::_tc_void;
    tm[i].type_def = transform_IDLType (sm[i].type_def);
  }

  res->members (tm);
  return res;
}

CORBA::UnionDef_ptr
CCMTransformer::transform_Union (CORBA::UnionDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);
  CORBA::UnionDef_ptr res;

  if (!CORBA::is_nil (existing)) {
    return CORBA::UnionDef::_narrow (existing);
  }

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = transform_Container (defin);

  CORBA::IDLType_var sd = src->discriminator_type_def ();
  CORBA::IDLType_var td = transform_IDLType (sd);

  CORBA::UnionMemberSeq_var sm = src->members ();
  CORBA::UnionMemberSeq tm;

  res = dest->create_union (id, name, version, td, tm);

  tm.length (sm->length ());

  for (CORBA::ULong i=0; i<sm->length(); i++) {
    tm[i].name  = sm[i].name;
    tm[i].label = sm[i].label;
    tm[i].type  = CORBA::_tc_void;
    tm[i].type_def = transform_IDLType (sm[i].type_def);
  }

  res->members (tm);
  return res;
}

CORBA::EnumDef_ptr
CCMTransformer::transform_Enum (CORBA::EnumDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);

  if (!CORBA::is_nil (existing)) {
    return CORBA::EnumDef::_narrow (existing);
  }

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = transform_Container (defin);

  CORBA::EnumMemberSeq_var em = src->members ();
  
  return dest->create_enum (id, name, version, em);
}

CORBA::AliasDef_ptr
CCMTransformer::transform_Alias (CORBA::AliasDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);

  if (!CORBA::is_nil (existing)) {
    return CORBA::AliasDef::_narrow (existing);
  }

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = transform_Container (defin);

  CORBA::IDLType_var st = src->original_type_def ();
  CORBA::IDLType_var tt = transform_IDLType (st);

  return dest->create_alias (id, name, version, tt);
}

CORBA::ExceptionDef_ptr
CCMTransformer::transform_Exception (CORBA::ExceptionDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);

  if (!CORBA::is_nil (existing)) {
    return CORBA::ExceptionDef::_narrow (existing);
  }

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = transform_Container (defin);

  CORBA::StructMemberSeq_var sm = src->members ();
  CORBA::StructMemberSeq tm;
  tm.length (sm->length ());

  for (CORBA::ULong i=0; i<sm->length(); i++) {
    tm[i].name = sm[i].name;
    tm[i].type = CORBA::_tc_void;
    tm[i].type_def = transform_IDLType (sm[i].type_def);
  }

  return dest->create_exception (id, name, version, tm);
}

CORBA::AttributeDef_ptr
CCMTransformer::transform_Attribute (CORBA::AttributeDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);

  if (!CORBA::is_nil (existing)) {
    return CORBA::AttributeDef::_narrow (existing);
  }

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = transform_Container (defin);
  CORBA::InterfaceDef_var destiface = CORBA::InterfaceDef::_narrow (dest);
  CORBA::ValueDef_var destval = CORBA::ValueDef::_narrow (dest);

  CORBA::IDLType_var st = src->type_def ();
  CORBA::IDLType_var tt = transform_IDLType (st);

  CORBA::AttributeDef_var res;

  if (!CORBA::is_nil (destiface)) {
    res = destiface->create_attribute (id, name, version, tt, src->mode());
  }
  else {
    res = destval->create_attribute (id, name, version, tt, src->mode());
  }

  CORBA::ExtAttributeDef_var ead = CORBA::ExtAttributeDef::_narrow (src);
  CORBA::ExtAttributeDef_var tead = CORBA::ExtAttributeDef::_narrow (res);
  CORBA::ExceptionDefSeq_var sge, sse;
  CORBA::ExceptionDefSeq tge, tse;

  if (!CORBA::is_nil (ead) && !CORBA::is_nil (tead)) {
    sge = ead->get_exceptions ();
    sse = ead->set_exceptions ();
    tge.length (sge->length());
    tse.length (sse->length());

    for (CORBA::ULong i1=0; i1<sge->length(); i1++) {
      tge[i1] = transform_Exception (sge[i1]);
    }

    for (CORBA::ULong i2=0; i2<sse->length(); i2++) {
      tse[i2] = transform_Exception (sse[i2]);
    }

    tead->get_exceptions (tge);
    tead->set_exceptions (tse);
  }

  /*
   * If we are contained in a component or home, then also add this
   * attribute to the local interface
   */

  if (defin->def_kind() == CORBA::dk_Component ||
      defin->def_kind() == CORBA::dk_Home) {
    RepoId origid (id.in(), src);
    RepoId lid (origid);
    lid.container.back().insert (0, "CCM_");

    CORBA::InterfaceDef_var lif;

    if (defin->def_kind() == CORBA::dk_Component) {
      RepoId coifid (lid);
      coifid.name = coifid.container.back ();
      coifid.container.pop_back ();
      CORBA::Contained_var lexifco = _repo->lookup_id (coifid.get());
      lif = CORBA::InterfaceDef::_narrow (lexifco);
    }
    else {
      lid.container.back() += "Explicit";
      RepoId exifid (lid);
      exifid.name = exifid.container.back ();
      exifid.container.pop_back ();
      CORBA::Contained_var lexifco = _repo->lookup_id (exifid.get());
      lif = CORBA::InterfaceDef::_narrow (lexifco);
    }

    assert (!CORBA::is_nil (lif));
    
    CORBA::AttributeDef_var lattr =
      lif->create_attribute (lid.get(), lid.name.c_str(),
			     lid.version.c_str(),
			     tt, src->mode());

    string fname = _db.get_fname_for_repoid (origid.sget());
    _db.register_repoid (lid.sget(), fname);

    CORBA::ExtAttributeDef_var lead = CORBA::ExtAttributeDef::_narrow (lattr);

    if (!CORBA::is_nil (ead) && !CORBA::is_nil (tead)) {
      lead->get_exceptions (tge);
      lead->set_exceptions (tse);
    }
  }

  /*
   * If we are contained in a component or home, then also add this
   * attribute to the local main executor interface
   */

  if (defin->def_kind() == CORBA::dk_Component) {
    RepoId origid (id.in(), src);
    RepoId lid (origid);
    lid.container.back().insert (0, "CCM_");
    lid.container.back() += "_Executor";

    RepoId coifid (lid);
    coifid.name = coifid.container.back ();
    coifid.container.pop_back ();
    CORBA::Contained_var lexifco = _repo->lookup_id (coifid.get());
    CORBA::InterfaceDef_var lif = CORBA::InterfaceDef::_narrow (lexifco);
    assert (!CORBA::is_nil (lif));
    
    CORBA::AttributeDef_var lattr =
      lif->create_attribute (lid.get(), lid.name.c_str(),
			     lid.version.c_str(),
			     tt, src->mode());

    string fname = _db.get_fname_for_repoid (origid.sget());
    _db.register_repoid (lid.sget(), fname);

    CORBA::ExtAttributeDef_var lead = CORBA::ExtAttributeDef::_narrow (lattr);

    if (!CORBA::is_nil (ead) && !CORBA::is_nil (tead)) {
      lead->get_exceptions (tge);
      lead->set_exceptions (tse);
    }
  }

  return res._retn ();
}

CORBA::OperationDef_ptr
CCMTransformer::transform_Operation (CORBA::OperationDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);

  if (!CORBA::is_nil (existing)) {
    return CORBA::OperationDef::_narrow (existing);
  }

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = transform_Container (defin);
  CORBA::InterfaceDef_var destiface = CORBA::InterfaceDef::_narrow (dest);
  CORBA::ValueDef_var destval = CORBA::ValueDef::_narrow (dest);

  CORBA::IDLType_var st = src->result_def ();
  CORBA::IDLType_var tt = transform_IDLType (st);

  CORBA::ParDescriptionSeq_var sp = src->params ();
  CORBA::ParDescriptionSeq tp;
  tp.length (sp->length());

  for (CORBA::ULong i1=0; i1<sp->length(); i1++) {
    tp[i1].name = sp[i1].name;
    tp[i1].type = CORBA::_tc_void;
    tp[i1].mode = sp[i1].mode;
    tp[i1].type_def = transform_IDLType (sp[i1].type_def);
  }

  CORBA::OperationMode sm = src->mode ();
  CORBA::ContextIdSeq_var sc = src->contexts ();

  CORBA::ExceptionDefSeq_var se = src->exceptions ();
  CORBA::ExceptionDefSeq te;
  te.length (se->length());

  for (CORBA::ULong i2=0; i2<se->length(); i2++) {
    te[i2] = transform_Exception (se[i2]);
  }

  CORBA::OperationDef_var res;

  if (!CORBA::is_nil (destiface)) {
    res = destiface->create_operation (id, name, version, tt, sm, tp, te, sc);
  }
  else {
    res = destval->create_operation (id, name, version, tt, sm, tp, te, sc);
  }

  /*
   * If we are contained in a home, then also add this
   * operation to the local interface
   */

  if (defin->def_kind() == CORBA::dk_Home) {
    RepoId origid (id.in(), src);
    RepoId lid (origid);
    lid.container.back().insert (0, "CCM_");
    lid.container.back() += "Explicit";

    RepoId exifid (lid);
    exifid.name = exifid.container.back ();
    exifid.container.pop_back ();
    
    CORBA::Contained_var lco = _repo->lookup_id (exifid.get());
    CORBA::InterfaceDef_var lif = CORBA::InterfaceDef::_narrow (lco);

    assert (!CORBA::is_nil (lif));

    CORBA::OperationDef_var lop =
      lif->create_operation (lid.get(), lid.name.c_str(),
			     lid.version.c_str(),
			     tt, sm, tp, te, sc);

    string fname = _db.get_fname_for_repoid (origid.sget());
    _db.register_repoid (lid.sget(), fname);
  }

  return res._retn ();
}

CORBA::InterfaceDef_ptr
CCMTransformer::transform_Interface (CORBA::InterfaceDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();
  CORBA::String_var absname = src->absolute_name ();

  CORBA::Contained_var existing = _repo->lookup_id (id);

  if (!CORBA::is_nil (existing)) {
    return CORBA::InterfaceDef::_narrow (existing);
  }

  /*
   * If this is an implicit interface, find the "real" interface and
   * transform it instead.
   */

  if (_db.is_implicit (absname)) {
    string an (absname.in());

    CORBA::ULong i = an.find ("CCM_");
    assert (i != (CORBA::ULong) -1);
    an.replace (i, 4, "");

    CORBA::Repository_var myrepo = src->containing_repository ();
    CORBA::Contained_var orig = myrepo->lookup (an.c_str());
    assert (!CORBA::is_nil (orig));
    CORBA::Contained_var torig = transform_Contained (orig);

    /*
     * Now we should exist
     */

    CORBA::Contained_var realme = _repo->lookup (absname);
    assert (!CORBA::is_nil (realme));

    return CORBA::InterfaceDef::_narrow (realme);
  }

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = transform_Container (defin);

  CORBA::InterfaceDefSeq_var sb = src->base_interfaces ();
  CORBA::InterfaceDefSeq tb;
  tb.length (sb->length());

  for (CORBA::ULong i=0; i<sb->length(); i++) {
    CORBA::IDLType_var ttemp = transform_IDLType (sb[i]);
    tb[i] = CORBA::InterfaceDef::_narrow (ttemp);
  }

  CORBA::InterfaceDef_var res;

  switch (src->def_kind()) {
  case CORBA::dk_Interface:
    res = dest->create_interface (id, name, version, tb);
    break;

  case CORBA::dk_AbstractInterface: {
    CORBA::AbstractInterfaceDefSeq ab;
    res = dest->create_abstract_interface (id, name, version, ab);
    res->base_interfaces (tb);
    break;
  }

  case CORBA::dk_LocalInterface:
    res = dest->create_local_interface (id, name, version, tb);
    break;

  default:
    assert (0);
    break;
  }

  /*
   * For all non-abstract and non-local interface, a local CCM_ interface
   * is generated.
   */

  if (src->def_kind() == CORBA::dk_Interface) {
    RepoId origid (id.in());
    RepoId lid (origid);
    lid.name.insert (0, "CCM_");

    CORBA::InterfaceDefSeq ltb;
    ltb.length (1);
    ltb[0] = CORBA::InterfaceDef::_narrow (res);

    CORBA::InterfaceDef_var lres =
      dest->create_local_interface (lid.get(), lid.name.c_str(),
				    lid.version.c_str(), ltb);

    string fname = _db.get_fname_for_repoid (origid.sget());
    _db.register_repoid (lid.sget(), fname);
  }

  assert (!CORBA::is_nil (res));
  return res._retn();
}

CORBA::ValueMemberDef_ptr
CCMTransformer::transform_ValueMember (CORBA::ValueMemberDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);

  if (!CORBA::is_nil (existing)) {
    return CORBA::ValueMemberDef::_narrow (existing);
  }

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = transform_Container (defin);
  CORBA::ValueDef_var destval = CORBA::ValueDef::_narrow (dest);

  CORBA::IDLType_var st = src->type_def ();
  CORBA::IDLType_var tt = transform_IDLType (st);

  return destval->create_value_member (id, name, version, tt, src->access());
}
  
CORBA::ValueDef_ptr
CCMTransformer::transform_Value (CORBA::ValueDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);

  if (!CORBA::is_nil (existing)) {
    return CORBA::ValueDef::_narrow (existing);
  }

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = transform_Container (defin);

  CORBA::ValueDef_var sb = src->base_value ();
  CORBA::ValueDef_var tb = CORBA::ValueDef::_nil ();

  if (!CORBA::is_nil (sb)) {
    CORBA::IDLType_var ttemp = transform_IDLType (sb);
    tb = CORBA::ValueDef::_narrow (ttemp);
  }

  CORBA::ValueDefSeq_var sab = src->abstract_base_values ();
  CORBA::ValueDefSeq tab;
  tab.length (sab->length ());

  for (CORBA::ULong i3=0; i3<sab->length(); i3++) {
    CORBA::IDLType_var ttemp = transform_IDLType (sab[i3]);
    tab[i3] = CORBA::ValueDef::_narrow (ttemp);
  }

  CORBA::InterfaceDefSeq_var ss = src->supported_interfaces ();
  CORBA::InterfaceDefSeq ts;
  ts.length (ss->length());

  for (CORBA::ULong i1=0; i1<ss->length(); i1++) {
    CORBA::IDLType_var ttemp = transform_IDLType (ss[i1]);
    ts[i1] = CORBA::InterfaceDef::_narrow (ttemp);
  }

  CORBA::InitializerSeq ti;
  
  CORBA::ValueDef_var res =
    dest->create_value (id, name, version,
			src->is_custom (),
			src->is_abstract (),
			tb,
			src->is_truncatable (),
			tab, ts, ti);

  CORBA::ExtValueDef_var evd = CORBA::ExtValueDef::_narrow (src);
  CORBA::ExtValueDef_var eres = CORBA::ExtValueDef::_narrow (res);

  if (!CORBA::is_nil (evd) && !CORBA::is_nil (eres)) {
    CORBA::ExtInitializerSeq_var esi = evd->ext_initializers ();
    CORBA::ExtInitializerSeq eti;
    eti.length (esi->length());

    for (CORBA::ULong i2=0; i2<esi->length(); i2++) {
      eti[i2].name = esi[i2].name;
      eti[i2].members.length (esi[i2].members.length());
      
      for (CORBA::ULong j1=0; j1<esi[i2].members.length(); j1++) {
	eti[i2].members[j1].name = esi[i2].members[j1].name;
	eti[i2].members[j1].type = CORBA::_tc_void;
	eti[i2].members[j1].type_def =
	  transform_IDLType (esi[i2].members[j1].type_def);
      }

      eti[i2].exceptions_def.length (esi[i2].exceptions_def.length());

      for (CORBA::ULong j2=0; j2<esi[i2].exceptions_def.length(); j2++) {
	eti[i2].exceptions_def[j2] =
	  transform_Exception (esi[i2].exceptions_def[j2]);
      }
    }

    eres->ext_initializers (eti);
  }
  else {
    CORBA::InitializerSeq_var si = src->initializers ();
    ti.length (si->length());

    for (CORBA::ULong i2=0; i2<si->length(); i2++) {
      ti[i2].name = si[i2].name;
      ti[i2].members.length (si[i2].members.length());
      
      for (CORBA::ULong j=0; j<si[i2].members.length(); j++) {
	ti[i2].members[j].name = si[i2].members[j].name;
	ti[i2].members[j].type = CORBA::_tc_void;
	ti[i2].members[j].type_def =
	  transform_IDLType (si[i2].members[j].type_def);
      }
    }

    res->initializers (ti);
  }

  return res._retn();
}

CORBA::ValueBoxDef_ptr
CCMTransformer::transform_ValueBox (CORBA::ValueBoxDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);

  if (!CORBA::is_nil (existing)) {
    return CORBA::ValueBoxDef::_narrow (existing);
  }

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = transform_Container (defin);

  CORBA::IDLType_var st = src->original_type_def ();
  CORBA::IDLType_var tt = transform_IDLType (st);

  return dest->create_value_box (id, name, version, tt);
}

/*
 * Here it finally gets more interesting
 */

CORBA::OperationDef_ptr
CCMTransformer::transform_Provides (CORBA::ComponentIR::ProvidesDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  /*
   * Original:
   *                  provides <interface_type> <name>;
   *
   * Equivalent IDL:
   *                  <interface_type> provide_<name> ();
   */

  /*
   * Compute new name and id for the resulting operation
   */

  RepoId origid (id.in(), src);
  string fname = _db.get_fname_for_repoid (origid.sget());

  RepoId pid (origid);
  pid.name.insert (0, "provide_");

  CORBA::Contained_var existing = _repo->lookup_id (pid.get());

  if (!CORBA::is_nil (existing)) {
    return CORBA::OperationDef::_narrow (existing);
  }

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = transform_Container (defin);
  CORBA::InterfaceDef_var destiface = CORBA::InterfaceDef::_narrow (dest);

  CORBA::InterfaceDef_var st = src->interface_type ();
  CORBA::IDLType_var tt = transform_IDLType (st);

  CORBA::ParDescriptionSeq tp;
  CORBA::ContextIdSeq tc;
  CORBA::ExceptionDefSeq te;

  CORBA::OperationDef_var res =
    destiface->create_operation (pid.get(), pid.name.c_str(),
				 pid.version.c_str(), tt,
				 CORBA::OP_NORMAL,
				 tp, te, tc);

  _db.register_repoid (pid.sget(), fname);

  /*
   * Original:        provides <interface_type> <name>;
   * Equivalent IDL:  CCM_<interface_type> get_<name> ();
   */

  RepoId lid (origid);
  lid.name.insert (0, "get_");
  lid.container.back().insert (0, "CCM_");

  CORBA::String_var ifid = st->id ();
  RepoId stid (ifid.in());
  stid.name.insert (0, "CCM_");

  CORBA::Contained_var tempco = _repo->lookup_id (stid.get());
  CORBA::InterfaceDef_var lifex = CORBA::InterfaceDef::_narrow (tempco);

  RepoId lifid (lid);
  lifid.name = lifid.container.back();
  lifid.container.pop_back ();
  CORBA::Contained_var lco = _repo->lookup_id (lifid.get());
  CORBA::InterfaceDef_var lif = CORBA::InterfaceDef::_narrow (lco);
  CORBA::OperationDef_var lres =
    lif->create_operation (lid.get(), lid.name.c_str(),
			   lid.version.c_str(),
			   lifex, CORBA::OP_NORMAL,
			   tp, te, tc);

  _db.register_repoid (lid.sget(), fname);

  return res._retn();
}

CORBA::OperationDef_ptr
CCMTransformer::transform_Uses (CORBA::ComponentIR::UsesDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::OperationDef_var tempop;

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = transform_Container (defin);
  CORBA::InterfaceDef_var destiface = CORBA::InterfaceDef::_narrow (dest);

  RepoId origid (id.in(), src);
  string fname = _db.get_fname_for_repoid (origid.sget());

  RepoId alreadythere (origid);
  alreadythere.name.insert (0, "connect_");
  CORBA::Contained_var existing = _repo->lookup_id (alreadythere.get());
      
  if (!CORBA::is_nil (existing)) {
    return CORBA::OperationDef::_narrow (existing);
  }

  /*
   * Original:
   *                  uses <interface_type> <name>;
   *
   * Equivalent IDL:
   *                  void connect_<name> (in <interface_type> conxn)
   *                    raises (Components::AlreadyConnected,
   *                            Components::InvalidConnection);
   *
   *                  <interface_type> disconnect_<name> ()
   *                    raises (Components::NoConnection);
   *
   *                  <interface_type> get_connection_<name> ()
   */

  if (!src->is_multiple ()) {
    /*
     * connect_<name> operation
     */

    {
      RepoId cid (origid);
      cid.name.insert (0, "connect_");
      CORBA::IDLType_var tt = _repo->get_primitive (CORBA::pk_void);
      CORBA::IDLType_var spt = src->interface_type ();
      
      CORBA::ParDescriptionSeq tp;
      tp.length (1);
      tp[0].name = CORBA::string_dup ("conxn");
      tp[0].type = CORBA::_tc_void;
      tp[0].type_def = transform_IDLType (spt);
      tp[0].mode = CORBA::PARAM_IN;

      CORBA::ExceptionDefSeq te;
      CORBA::Contained_var tempco;
      te.length (2);

      tempco = _repo->lookup ("Components::AlreadyConnected");
      te[0] = CORBA::ExceptionDef::_narrow (tempco);

      tempco = _repo->lookup ("Components::InvalidConnection");
      te[1] = CORBA::ExceptionDef::_narrow (tempco);

      CORBA::ContextIdSeq tc;

      tempop = destiface->create_operation (cid.get(),
					    cid.name.c_str(),
					    cid.version.c_str(),
					    tt, CORBA::OP_NORMAL,
					    tp, te, tc);

      _db.register_repoid (cid.sget(), fname);
    }

    /*
     * disconnect_<name> operation
     */

    {
      RepoId did (origid);
      did.name.insert (0, "disconnect_");
      
      CORBA::IDLType_var st = src->interface_type ();
      CORBA::IDLType_var tt = transform_IDLType (st);
      
      CORBA::ExceptionDefSeq te;
      CORBA::Contained_var tempco;
      te.length (1);

      tempco = _repo->lookup ("Components::NoConnection");
      te[0] = CORBA::ExceptionDef::_narrow (tempco);

      CORBA::ParDescriptionSeq tp;
      CORBA::ContextIdSeq tc;

      tempop = destiface->create_operation (did.get(),
					    did.name.c_str(),
					    did.version.c_str(),
					    tt, CORBA::OP_NORMAL,
					    tp, te, tc);

      _db.register_repoid (did.sget(), fname);
    }

    /*
     * get_connection_<name> operation
     */

    {
      RepoId gid (origid);
      gid.name.insert (0, "get_connection_");
      
      CORBA::IDLType_var st = src->interface_type ();
      CORBA::IDLType_var tt = transform_IDLType (st);
      
      CORBA::ParDescriptionSeq tp;
      CORBA::ContextIdSeq tc;
      CORBA::ExceptionDefSeq te;

      tempop = destiface->create_operation (gid.get(),
					    gid.name.c_str(),
					    gid.version.c_str(),
					    tt, CORBA::OP_NORMAL,
					    tp, te, tc);

      _db.register_repoid (gid.sget(), fname);
    }
  }

  /*
   * Original:
   *                  uses multiple <interface_type> <name>;
   *
   * Equivalent IDL:
   *                  struct <name>Connection {
   *                    <interface_type> objref;
   *                    Components::Cookie ck;
   *                  };
   *
   *                  typedef sequence<<name>Connection> <name>Connections;
   *
   *                  Components::Cookie
   *                    connect_<name> (in <interface_type> connection)
   *                      raises (Components::ExceededConnectionLimit,
   *                              Components::InvalidConnection);
   *
   *                  <interface_type>
   *                    disconnect_<name> (in Components::Cookie ck)
   *                      raises (Components::InvalidConnection);
   *
   *                  <name>Connections get_connections_<name> ();
   */

  else {
    CORBA::IDLType_var conn;
    CORBA::IDLType_var conns;

    /*
     * struct <name>Connection
     */

    {
      RepoId structid (origid);
      structid.name += "Connection";
      
      CORBA::IDLType_var st = src->interface_type ();

      CORBA::StructMemberSeq tms;
      CORBA::Contained_var tempco;
      tms.length (2);

      tms[0].name = CORBA::string_dup ("objref");
      tms[0].type = CORBA::_tc_void;
      tms[0].type_def = transform_IDLType (st);

      tempco = _repo->lookup ("Components::Cookie");

      tms[1].name = CORBA::string_dup ("ck");
      tms[1].type = CORBA::_tc_void;
      tms[1].type_def = CORBA::IDLType::_narrow (tempco);

      conn = dest->create_struct (structid.get(),
				  structid.name.c_str(),
				  structid.version.c_str(),
				  tms);

      _db.register_repoid (structid.sget(), fname);
    }

    /*
     * typedef sequence <<name>Connection> <name>Connections
     */

    {
      RepoId seqid (origid);
      seqid.name += "Connections";
      
      CORBA::SequenceDef_var dummy = _repo->create_sequence (0, conn);

      string pseudoid = _db.gen_pseudo_repoid (dummy);
      _db.register_repoid (pseudoid, fname);

      conns = dest->create_alias (seqid.get(),
				  seqid.name.c_str(),
				  seqid.version.c_str(),
				  dummy);

      _db.register_repoid (seqid.sget(), fname);
    }

    /*
     * connect_<name> operation
     */

    {
      RepoId cid (origid);
      cid.name.insert (0, "connect_");
      
      CORBA::Contained_var tempco;
      tempco = _repo->lookup ("Components::Cookie");
      CORBA::IDLType_var tt = CORBA::IDLType::_narrow (tempco);

      CORBA::IDLType_var spt = src->interface_type ();
      
      CORBA::ParDescriptionSeq tp;
      tp.length (1);
      tp[0].name = CORBA::string_dup ("connection");
      tp[0].type = CORBA::_tc_void;
      tp[0].type_def = transform_IDLType (spt);
      tp[0].mode = CORBA::PARAM_IN;

      CORBA::ExceptionDefSeq te;
      te.length (2);

      tempco = _repo->lookup ("Components::ExceededConnectionLimit");
      te[0] = CORBA::ExceptionDef::_narrow (tempco);

      tempco = _repo->lookup ("Components::InvalidConnection");
      te[1] = CORBA::ExceptionDef::_narrow (tempco);

      CORBA::ContextIdSeq tc;

      tempop = destiface->create_operation (cid.get(),
					    cid.name.c_str(),
					    cid.version.c_str(),
					    tt, CORBA::OP_NORMAL,
					    tp, te, tc);

      _db.register_repoid (cid.sget(), fname);
    }

    /*
     * disconnect_<name> operation
     */

    {
      RepoId did (origid);
      did.name.insert (0, "disconnect_");

      CORBA::IDLType_var st = src->interface_type ();
      CORBA::IDLType_var tt = transform_IDLType (st);
      
      CORBA::Contained_var tempco;
      tempco = _repo->lookup ("Components::Cookie");

      CORBA::ParDescriptionSeq tp;
      tp.length (1);
      tp[0].name = CORBA::string_dup ("ck");
      tp[0].type = CORBA::_tc_void;
      tp[0].type_def = CORBA::IDLType::_narrow (tempco);
      tp[0].mode = CORBA::PARAM_IN;

      CORBA::ExceptionDefSeq te;
      te.length (1);

      tempco = _repo->lookup ("Components::InvalidConnection");
      te[0] = CORBA::ExceptionDef::_narrow (tempco);

      CORBA::ContextIdSeq tc;

      tempop = destiface->create_operation (did.get(),
					    did.name.c_str(),
					    did.version.c_str(),
					    tt, CORBA::OP_NORMAL,
					    tp, te, tc);

      _db.register_repoid (did.sget(), fname);
    }

    /*
     * get_connections_<name> operation
     */

    {
      RepoId gid (origid);
      gid.name.insert (0, "get_connections_");

      CORBA::ParDescriptionSeq tp;
      CORBA::ExceptionDefSeq te;
      CORBA::ContextIdSeq tc;

      tempop = destiface->create_operation (gid.get(),
					    gid.name.c_str(),
					    gid.version.c_str(),
					    conns, CORBA::OP_NORMAL,
					    tp, te, tc);

      _db.register_repoid (gid.sget(), fname);
    }
  }

  /*
   * Equivalent operation on context interface
   */

  RepoId sdefinid (origid);
  sdefinid.name = sdefinid.container.back ();
  sdefinid.container.pop_back ();
  sdefinid.name.insert (0, "CCM_");
  sdefinid.name += "_Context";

  CORBA::Contained_var lco = _repo->lookup_id (sdefinid.get());
  CORBA::InterfaceDef_var ctxif = CORBA::InterfaceDef::_narrow (lco);

  /*
   * Original:
   *                  uses <interface_type> <name>;
   *
   * Equivalent IDL:
   *                  <interface_type> get_connection_<name> ();
   */

  if (!src->is_multiple ()) {
    RepoId gid (origid);
    gid.container.back().insert (0, "CCM_");
    gid.container.back() += "_Context";
    gid.name.insert (0, "get_connection_");

    CORBA::IDLType_var spt = src->interface_type ();
    CORBA::IDLType_var tt = transform_IDLType (spt);

    CORBA::ParDescriptionSeq tp;
    CORBA::ExceptionDefSeq te;
    CORBA::ContextIdSeq tc;

    CORBA::OperationDef_var lop =
      ctxif->create_operation (gid.get(),
			       gid.name.c_str(),
			       gid.version.c_str(),
			       tt, CORBA::OP_NORMAL,
			       tp, te, tc);

    _db.register_repoid (gid.sget(), fname);
  }

  /*
   * Original:
   *                  uses multiple <interface_type> <name>;
   *
   * Equivalent IDL:
   *                  <name>Connections get_connections_<name> ();
   */

  else {
    RepoId gid (origid);
    gid.container.back().insert (0, "CCM_");
    gid.container.back() += "_Context";
    gid.name.insert (0, "get_connections_");

    RepoId seqid (origid);
    seqid.name += "Connections";

    CORBA::Contained_var tempco = _repo->lookup_id (seqid.get());
    CORBA::IDLType_var tt = CORBA::IDLType::_narrow (tempco);

    CORBA::ParDescriptionSeq tp;
    CORBA::ExceptionDefSeq te;
    CORBA::ContextIdSeq tc;

    CORBA::OperationDef_var lop =
      ctxif->create_operation (gid.get(),
			       gid.name.c_str(),
			       gid.version.c_str(),
			       tt, CORBA::OP_NORMAL,
			       tp, te, tc);

    _db.register_repoid (gid.sget(), fname);
  }

  return tempop._retn();
}

CORBA::ValueDef_ptr
CCMTransformer::transform_Event (CORBA::ComponentIR::EventDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = transform_Container (defin);
  CORBA::Contained_var tempco;

  /*
   * Original:
   *                  eventtype <name> { ... };
   *
   * Equivalent IDL:
   *                  valuetype <name> : Components::EventBase { ... };
   */

  RepoId origid (id.in(), src);
  string fname = _db.get_fname_for_repoid (origid.sget());

  RepoId eid (origid);

  CORBA::Contained_var existing = _repo->lookup_id (eid.get());

  if (!CORBA::is_nil (existing)) {
    return CORBA::ValueDef::_narrow (existing);
  }

  CORBA::ValueDef_var res = transform_Value (src);
  CORBA::ValueDef_var base = res->base_value ();

  if (CORBA::is_nil (base)) {
    CORBA::ValueDefSeq_var tab = res->abstract_base_values ();
    CORBA::ULong nab = tab->length ();
    tab->length (nab + 1);

    tempco = _repo->lookup ("Components::EventBase");
    tab[nab] = CORBA::ValueDef::_narrow (tempco);

    res->abstract_base_values (tab.in());
  }

  /*
   * Original:
   *                  eventtype <name> { ... };
   *
   * Equivalent IDL:
   *                  interface <name>Consumer : Components::EventConsumerBase
   *                  {
   *                    void push_<name> (in <name> event);
   *                  };
   */

  RepoId iid (origid);
  iid.name += "Consumer";

  CORBA::InterfaceDefSeq tib;
  tib.length (1);

  tempco = _repo->lookup ("Components::EventConsumerBase");
  tib[0] = CORBA::InterfaceDef::_narrow (tempco);

  CORBA::InterfaceDef_var cif =
    dest->create_interface (iid.get(),
			    iid.name.c_str(),
			    iid.version.c_str(),
			    tib);

  _db.register_repoid (iid.sget(), fname);

  RepoId pid (origid);
  pid.name = "push_";
  pid.name += name.in();
  pid.container.push_back (iid.name);

  CORBA::IDLType_var ptt = _repo->get_primitive (CORBA::pk_void);

  CORBA::ParDescriptionSeq ptp;
  ptp.length (1);
  ptp[0].name = CORBA::string_dup ("event");
  ptp[0].type = CORBA::_tc_void;
  ptp[0].type_def = CORBA::IDLType::_narrow (res);
  ptp[0].mode = CORBA::PARAM_IN;

  CORBA::ContextIdSeq ptc;
  CORBA::ExceptionDefSeq pte;

  CORBA::OperationDef_var pop =
    cif->create_operation (pid.get(),
			   pid.name.c_str(),
			   pid.version.c_str(),
			   ptt, CORBA::OP_NORMAL,
			   ptp, pte, ptc);

  _db.register_repoid (pid.sget(), fname);

  /*
   * Original:
   *                  eventtype <name> { ... };
   *
   * Equivalent IDL:
   *                  local interface CCM_<name>Consumer
   *                  {
   *                    void push (in <name> event);
   *                  };
   */

  RepoId lcid (origid);
  lcid.name.insert (0, "CCM_");
  lcid.name += "Consumer";

  CORBA::InterfaceDefSeq ltib;

  CORBA::InterfaceDef_var lcif =
    dest->create_local_interface (lcid.get(),
				  lcid.name.c_str(),
				  lcid.version.c_str(),
				  ltib);

  _db.register_repoid (lcid.sget(), fname);

  RepoId lpid (lcid);
  lpid.container.push_back (lpid.name);
  lpid.name = "push";

  CORBA::OperationDef_var lpop =
    lcif->create_operation (lpid.get(),
			    lpid.name.c_str(),
			    lpid.version.c_str(),
			    ptt, CORBA::OP_NORMAL,
			    ptp, pte, ptc);

  _db.register_repoid (lpid.sget(), fname);

  return res._retn();
}

CORBA::OperationDef_ptr
CCMTransformer::transform_Emits (CORBA::ComponentIR::EmitsDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  /*
   * Original:
   *                  emits <type> <name>;
   *
   * Equivalent IDL:
   *                  void connect_<name> (in <type>Consumer consumer)
   *                    raises (Components::AlreadyConnected);
   *                  <type>Consumer disconnect_<name> ()
   *                    raises (Components::NoConnection);
   */

  /*
   * Compute new name and id for the resulting operation
   */

  RepoId origid (id.in(), src);
  string fname = _db.get_fname_for_repoid (origid.sget());

  RepoId conid (origid);
  conid.name.insert (0, "connect_");

  CORBA::Contained_var existing = _repo->lookup_id (conid.get());

  if (!CORBA::is_nil (existing)) {
    return CORBA::OperationDef::_narrow (existing);
  }

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = transform_Container (defin);
  CORBA::InterfaceDef_var destiface = CORBA::InterfaceDef::_narrow (dest);

  CORBA::ValueDef_var ev = src->event ();
  CORBA::Contained_var tev = transform_Contained (ev);
  CORBA::String_var aen = tev->absolute_name ();
  string aens (aen.in());
  aens += "Consumer";
  CORBA::Contained_var tempco = _repo->lookup (aens.c_str());
  CORBA::InterfaceDef_var consumer = CORBA::InterfaceDef::_narrow (tempco);

  /*
   * connect_ operation
   */

  CORBA::ContextIdSeq tc;
  CORBA::ExceptionDefSeq te;

  CORBA::ParDescriptionSeq tp;
  tp.length (1);
  tp[0].name = CORBA::string_dup ("event");
  tp[0].type = CORBA::_tc_void;
  tp[0].type_def = CORBA::IDLType::_narrow (consumer);
  tp[0].mode = CORBA::PARAM_IN;

  CORBA::IDLType_var tt = _repo->get_primitive (CORBA::pk_void);

  CORBA::OperationDef_var res =
    destiface->create_operation (conid.get(), conid.name.c_str(),
				 conid.version.c_str(), tt,
				 CORBA::OP_NORMAL,
				 tp, te, tc);

  _db.register_repoid (conid.sget(), fname);

  /*
   * disconnect_ operation
   */

  RepoId disconid (origid);
  disconid.name.insert (0, "disconnect_");

  tp.length (0);

  res = destiface->create_operation (disconid.get(), disconid.name.c_str(),
				     disconid.version.c_str(), consumer,
				     CORBA::OP_NORMAL,
				     tp, te, tc);

  _db.register_repoid (disconid.sget(), fname);

  /*
   * Equivalent operations on context interface
   */

  RepoId sdefinid (origid);
  sdefinid.name = sdefinid.container.back ();
  sdefinid.container.pop_back ();
  sdefinid.name.insert (0, "CCM_");
  sdefinid.name += "_Context";

  CORBA::Contained_var tempcont = _repo->lookup_id (sdefinid.get());
  CORBA::InterfaceDef_var ctxif = CORBA::InterfaceDef::_narrow (tempcont);

  RepoId pushid (origid);
  pushid.container.back().insert (0, "CCM_");
  pushid.container.back() += "_Context";
  pushid.name.insert (0, "push_");

  tp.length (1);
  tp[0].name = CORBA::string_dup ("event");
  tp[0].type = CORBA::_tc_void;
  tp[0].type_def = CORBA::IDLType::_narrow (tev);
  tp[0].mode = CORBA::PARAM_IN;

  CORBA::OperationDef_var lop =
    ctxif->create_operation (pushid.get(), pushid.name.c_str(),
			     pushid.version.c_str(), tt,
			     CORBA::OP_NORMAL,
			     tp, te, tc);

  _db.register_repoid (pushid.sget(), fname);

  return res._retn ();
}

CORBA::OperationDef_ptr
CCMTransformer::transform_Publishes (CORBA::ComponentIR::PublishesDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  /*
   * Original:
   *                  publishes <type> <name>;
   *
   * Equivalent IDL:
   *                  Components::Cookie
   *                    subscribe_<name> (in <type>Consumer consumer)
   *                      raises (Components::ExceededConnectionLimit);
   *                  <type>Consumer
   *                    unsubscribe_<name> (in Components::Cookie ck)
   *                      raises (Components::InvalidConnection);
   */

  /*
   * Compute new name and id for the resulting operation
   */

  RepoId origid (id.in(), src);
  string fname = _db.get_fname_for_repoid (origid.sget());

  RepoId subid (origid);
  subid.name.insert (0, "subscribe_");

  CORBA::Contained_var existing = _repo->lookup_id (subid.get());

  if (!CORBA::is_nil (existing)) {
    return CORBA::OperationDef::_narrow (existing);
  }

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = transform_Container (defin);
  CORBA::InterfaceDef_var destiface = CORBA::InterfaceDef::_narrow (dest);

  CORBA::ValueDef_var ev = src->event ();
  CORBA::Contained_var tev = transform_Contained (ev);
  CORBA::String_var aen = tev->absolute_name ();
  string aens (aen.in());
  aens += "Consumer";
  CORBA::Contained_var tempco = _repo->lookup (aens.c_str());
  CORBA::InterfaceDef_var consumer = CORBA::InterfaceDef::_narrow (tempco);

  tempco = _repo->lookup ("Components::Cookie");
  CORBA::IDLType_var cookie = CORBA::IDLType::_narrow (tempco);

  /*
   * subscribe_ operation
   */

  CORBA::ContextIdSeq tc;
  CORBA::ExceptionDefSeq te;

  CORBA::ParDescriptionSeq tp;
  tp.length (1);
  tp[0].name = CORBA::string_dup ("consumer");
  tp[0].type = CORBA::_tc_void;
  tp[0].type_def = CORBA::IDLType::_narrow (consumer);
  tp[0].mode = CORBA::PARAM_IN;

  CORBA::OperationDef_var res =
    destiface->create_operation (subid.get(), subid.name.c_str(),
				 subid.version.c_str(), cookie,
				 CORBA::OP_NORMAL,
				 tp, te, tc);

  _db.register_repoid (subid.sget(), fname);

  /*
   * unsubscribe_ operation
   */

  RepoId unsubid (origid);
  unsubid.name.insert (0, "unsubscribe_");

  tp[0].name = CORBA::string_dup ("ck");
  tp[0].type = CORBA::_tc_void;
  tp[0].type_def = CORBA::IDLType::_narrow (cookie);
  tp[0].mode = CORBA::PARAM_IN;

  res = destiface->create_operation (unsubid.get(), unsubid.name.c_str(),
				     unsubid.version.c_str(), consumer,
				     CORBA::OP_NORMAL,
				     tp, te, tc);

  _db.register_repoid (unsubid.sget(), fname);

  /*
   * Equivalent operations on context interface
   */

  RepoId sdefinid (origid);
  sdefinid.name = sdefinid.container.back ();
  sdefinid.container.pop_back ();
  sdefinid.name.insert (0, "CCM_");
  sdefinid.name += "_Context";

  CORBA::Contained_var tempcont = _repo->lookup_id (sdefinid.get());
  CORBA::InterfaceDef_var ctxif = CORBA::InterfaceDef::_narrow (tempcont);

  RepoId pushid (origid);
  pushid.container.back().insert (0, "CCM_");
  pushid.container.back() += "_Context";
  pushid.name.insert (0, "push_");

  tp.length (1);
  tp[0].name = CORBA::string_dup ("event");
  tp[0].type = CORBA::_tc_void;
  tp[0].type_def = CORBA::IDLType::_narrow (tev);
  tp[0].mode = CORBA::PARAM_IN;

  CORBA::IDLType_var tt = _repo->get_primitive (CORBA::pk_void);

  CORBA::OperationDef_var lop =
    ctxif->create_operation (pushid.get(), pushid.name.c_str(),
			     pushid.version.c_str(), tt,
			     CORBA::OP_NORMAL,
			     tp, te, tc);

  _db.register_repoid (pushid.sget(), fname);

  return res._retn ();
}

CORBA::OperationDef_ptr
CCMTransformer::transform_Consumes (CORBA::ComponentIR::ConsumesDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  /*
   * Original:
   *                  consumes <type> <name>;
   *
   * Equivalent IDL:
   *                  <type>Consumer get_consumer_<name> ();
   */

  /*
   * Compute new name and id for the resulting operation
   */

  RepoId origid (id.in(), src);
  string fname = _db.get_fname_for_repoid (origid.sget());

  RepoId getid (origid);
  getid.name.insert (0, "get_consumer_");

  CORBA::Contained_var existing = _repo->lookup_id (getid.get());

  if (!CORBA::is_nil (existing)) {
    return CORBA::OperationDef::_narrow (existing);
  }

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = transform_Container (defin);
  CORBA::InterfaceDef_var destiface = CORBA::InterfaceDef::_narrow (dest);

  CORBA::ValueDef_var ev = src->event ();
  CORBA::Contained_var tev = transform_Contained (ev);
  CORBA::String_var aen = tev->absolute_name ();
  string aens (aen.in());
  aens += "Consumer";
  CORBA::Contained_var tempco = _repo->lookup (aens.c_str());
  CORBA::InterfaceDef_var consumer = CORBA::InterfaceDef::_narrow (tempco);

  CORBA::ContextIdSeq tc;
  CORBA::ExceptionDefSeq te;
  CORBA::ParDescriptionSeq tp;

  CORBA::OperationDef_var res =
    destiface->create_operation (getid.get(), getid.name.c_str(),
				 getid.version.c_str(), consumer,
				 CORBA::OP_NORMAL,
				 tp, te, tc);

  _db.register_repoid (getid.sget(), fname);

  /*
   * Equivalent operation on local interface
   */

  /*
   * Original:
   *                  consumes <type> <name>;
   *
   * Equivalent IDL:
   *                  void push_<name> (in <type> ev);
   */

  RepoId pushid (origid);
  pushid.container.back().insert (0, "CCM_");
  pushid.name.insert (0, "push_");

  CORBA::IDLType_var tt = _repo->get_primitive (CORBA::pk_void);

  tp.length (1);
  tp[0].name = CORBA::string_dup ("conxn");
  tp[0].type = CORBA::_tc_void;
  tp[0].type_def = CORBA::IDLType::_narrow (tev);
  tp[0].mode = CORBA::PARAM_IN;

  RepoId lifid (pushid);
  lifid.name = lifid.container.back();
  lifid.container.pop_back ();

  CORBA::Contained_var lco = _repo->lookup_id (lifid.get());
  CORBA::InterfaceDef_var lif = CORBA::InterfaceDef::_narrow (lco);

  CORBA::OperationDef_var lop =
    lif->create_operation (pushid.get(), pushid.name.c_str(),
			   pushid.version.c_str(), tt,
			   CORBA::OP_NORMAL,
			   tp, te, tc);

  _db.register_repoid (pushid.sget(), fname);

  return res._retn ();
}

CORBA::InterfaceDef_ptr
CCMTransformer::transform_Component (CORBA::ComponentIR::ComponentDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  RepoId origid (id.in(), src);
  string fname = _db.get_fname_for_repoid (origid.sget());

  CORBA::Contained_var existing = _repo->lookup_id (id);

  if (!CORBA::is_nil (existing)) {
    return CORBA::InterfaceDef::_narrow (existing);
  }

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = transform_Container (defin);

  /*
   * Original:
   *                  component <name> { ... };
   * Equivalent IDL:
   *                  interface <name> : Components::CCMObject { ... };
   *
   * Original:
   *                  component <name> supports <if1>, <if2> { ... };
   * Equivalent IDL:
   *                  interface <name> : Components::CCMObject, <if1>, <if2>
   *                  { ... };
   *
   * Original:
   *                  component <name> : <base> { ... };
   * Equivalent IDL:
   *                  interface <name> : <base> { ... };
   *
   * Original:
   *                  component <name> : <base> supports <if1>, <if2>
   *                  { ... };
   * Equivalent IDL:
   *                  interface <name> : <base>, <if1>, <if2>
   *                  { ... };
   *
   * In other words, the component is represented by an interface that
   * inherits all supported interfaces, plus the equivalent interface
   * for the base component, if it exists, or CCMObject otherwise.
   */

  CORBA::ComponentIR::ComponentDef_var base = src->base_component ();
  CORBA::InterfaceDefSeq_var supported = src->supported_interfaces ();
  CORBA::InterfaceDefSeq tb;

  tb.length (supported->length() + 1);

  if (CORBA::is_nil (base)) {
    CORBA::Contained_var tempco = _repo->lookup ("Components::CCMObject");
    tb[0] = CORBA::InterfaceDef::_narrow (tempco);
  }
  else {
    CORBA::IDLType_var ttemp = transform_IDLType (base);
    tb[0] = CORBA::InterfaceDef::_narrow (ttemp);
  }

  for (CORBA::ULong i=0; i<supported->length(); i++) {
    CORBA::IDLType_var ttemp = transform_IDLType (supported[i]);
    tb[i+1] = CORBA::InterfaceDef::_narrow (ttemp);
  }

  CORBA::InterfaceDef_var res =
    dest->create_interface (origid.get(), origid.name.c_str(),
			    origid.version.c_str(), tb);

  /*
   * Original:        component <name> { ... };
   * Equivalent IDL:  local interface CCM_<name>
   */

  RepoId lid (origid);
  lid.name.insert (0, "CCM_");

  if (CORBA::is_nil (base)) {
    CORBA::Contained_var tempco =
      _repo->lookup ("Components::EnterpriseComponent");
    tb[0] = CORBA::InterfaceDef::_narrow (tempco);
  }
  else {
    CORBA::String_var bid = base->id ();
    RepoId sbid (bid.in());
    sbid.name.insert (0, "CCM_");
    CORBA::Contained_var tempco = _repo->lookup_id (sbid.get());
    tb[0] = CORBA::InterfaceDef::_narrow (tempco);
  }

  CORBA::InterfaceDef_var lres =
    dest->create_local_interface (lid.get(), lid.name.c_str(),
				  lid.version.c_str(), tb);

  _db.register_repoid (lid.sget(), fname);

  /*
   * Original:        component <name> { ... };
   * Equivalent IDL:  local interface CCM_<name>_Excecutor
   */

  RepoId meid (origid);
  meid.name.insert (0, "CCM_");
  meid.name += "_Executor";

  if (CORBA::is_nil (base)) {
    CORBA::Contained_var tempco =
      _repo->lookup ("Components::EnterpriseComponent");
    tb[0] = CORBA::InterfaceDef::_narrow (tempco);
  }
  else {
    CORBA::String_var bid = base->id ();
    RepoId sbid (bid.in());
    sbid.name.insert (0, "CCM_");
    sbid.name += "_Executor";
    CORBA::Contained_var tempco = _repo->lookup_id (sbid.get());
    tb[0] = CORBA::InterfaceDef::_narrow (tempco);
  }

  CORBA::InterfaceDef_var meres =
    dest->create_local_interface (meid.get(), meid.name.c_str(),
				  meid.version.c_str(), tb);

  _db.register_repoid (meid.sget(), fname);

  /*
   * Original:        component <name> { ... };
   * Equivalent IDL:  local interface CCM_<name>_Context
   */

  lid.name += "_Context";
  tb.length(1);

  if (CORBA::is_nil (base)) {
    CORBA::Contained_var tempco =
      _repo->lookup ("Components::SessionContext");
    tb[0] = CORBA::InterfaceDef::_narrow (tempco);
  }
  else {
    CORBA::String_var bid = base->id ();
    RepoId sbid (bid.in());
    sbid.name.insert (0, "CCM_");
    sbid.name += "_Context";
    CORBA::Contained_var tempco = _repo->lookup_id (sbid.get());
    tb[0] = CORBA::InterfaceDef::_narrow (tempco);
  }

  lres = dest->create_local_interface (lid.get(), lid.name.c_str(),
				       lid.version.c_str(), tb);

  _db.register_repoid (lid.sget(), fname);

  return res._retn();
}

CORBA::OperationDef_ptr
CCMTransformer::transform_Factory (CORBA::ComponentIR::FactoryDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  RepoId origid (id.in(), src);
  string fname = _db.get_fname_for_repoid (origid.sget());
  
  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var tempcont = transform_Container (defin);

  CORBA::ComponentIR::HomeDef_var home =
    CORBA::ComponentIR::HomeDef::_narrow (defin);
  CORBA::ComponentIR::ComponentDef_var managed = home->managed_component ();
  CORBA::IDLType_var managedif = transform_IDLType (managed);
  assert (!CORBA::is_nil (managedif));

  /*
   * Original:
   *                  factory <name> (...);
   *
   * Equivalent IDL:
   *                  <component> <name> (...);
   *
   * The factory operation must be added to the home's explicit interface.
   */

  RepoId fid (origid);
  fid.container.back() += "Explicit";

  CORBA::Contained_var existing = _repo->lookup_id (fid.get());

  if (!CORBA::is_nil (existing)) {
    return CORBA::OperationDef::_narrow (existing);
  }

  /*
   * Lookup explicit interface
   */

  RepoId exid (fid);
  exid.name = exid.container.back();
  exid.container.pop_back ();

  CORBA::Contained_var tempco = _repo->lookup_id (exid.get());
  CORBA::InterfaceDef_var destiface = CORBA::InterfaceDef::_narrow (tempco);

  CORBA::ParDescriptionSeq_var sp = src->params ();
  CORBA::ParDescriptionSeq tp;
  tp.length (sp->length());

  for (CORBA::ULong i1=0; i1<sp->length(); i1++) {
    tp[i1].name = sp[i1].name;
    tp[i1].type = CORBA::_tc_void;
    tp[i1].mode = sp[i1].mode;
    tp[i1].type_def = transform_IDLType (sp[i1].type_def);
  }

  CORBA::ContextIdSeq_var sc = src->contexts ();

  CORBA::ExceptionDefSeq_var se = src->exceptions ();
  CORBA::ExceptionDefSeq te;
  te.length (se->length());

  for (CORBA::ULong i2=0; i2<se->length(); i2++) {
    te[i2] = transform_Exception (se[i2]);
  }

  CORBA::OperationDef_var res =
    destiface->create_operation (fid.get(), name, version,
				 managedif, CORBA::OP_NORMAL,
				 tp, te, sc);

  _db.register_repoid (fid.sget(), fname);

  /*
   * Also add this operation to the home's local explicit interface
   */

  RepoId exifid (origid);
  exifid.container.back().insert (0, "CCM_");
  exifid.container.back() += "Explicit";
  exifid.name = exifid.container.back();
  exifid.container.pop_back ();

  CORBA::Contained_var lexifco = _repo->lookup_id (exifid.get());
  CORBA::InterfaceDef_var lexif = CORBA::InterfaceDef::_narrow (lexifco);
  assert (!CORBA::is_nil (lexifco));

  RepoId lid (origid);
  lid.container.back().insert (0, "CCM_");
  lid.container.back() += "Explicit";

  tempco = _repo->lookup ("Components::EnterpriseComponent");
  CORBA::IDLType_var tt = CORBA::IDLType::_narrow (tempco);
  assert (!CORBA::is_nil (tt));

  CORBA::OperationDef_var lres =
    lexif->create_operation (lid.get(),
			     lid.name.c_str(),
			     lid.version.c_str(),
			     tt, CORBA::OP_NORMAL,
			     tp, te, sc);

  _db.register_repoid (lid.sget(), fname);

  return res._retn ();
}

CORBA::OperationDef_ptr
CCMTransformer::transform_Finder (CORBA::ComponentIR::FinderDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  RepoId origid (id.in(), src);
  string fname = _db.get_fname_for_repoid (origid.sget());
  
  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var tempcont = transform_Container (defin);

  CORBA::ComponentIR::HomeDef_var home =
    CORBA::ComponentIR::HomeDef::_narrow (defin);
  CORBA::ComponentIR::ComponentDef_var managed = home->managed_component ();
  CORBA::IDLType_var managedif = transform_Component (managed);

  /*
   * Original:
   *                  finder <name> (...);
   *
   * Equivalent IDL:
   *                  <component> <name> (...);
   *
   * The finder operation must be added to the home's explicit interface.
   */
  
  RepoId fid (origid);
  fid.container.back() += "Explicit";

  CORBA::Contained_var existing = _repo->lookup_id (fid.get());

  if (!CORBA::is_nil (existing)) {
    return CORBA::OperationDef::_narrow (existing);
  }

  /*
   * Look up explicit interface
   */

  RepoId exid (fid);
  exid.name = exid.container.back();
  exid.container.pop_back ();

  CORBA::Contained_var tempco = _repo->lookup_id (exid.get());
  CORBA::InterfaceDef_var destiface = CORBA::InterfaceDef::_narrow (tempco);

  CORBA::ParDescriptionSeq_var sp = src->params ();
  CORBA::ParDescriptionSeq tp;
  tp.length (sp->length());

  for (CORBA::ULong i1=0; i1<sp->length(); i1++) {
    tp[i1].name = sp[i1].name;
    tp[i1].type = CORBA::_tc_void;
    tp[i1].mode = sp[i1].mode;
    tp[i1].type_def = transform_IDLType (sp[i1].type_def);
  }

  CORBA::ContextIdSeq_var sc = src->contexts ();

  CORBA::ExceptionDefSeq_var se = src->exceptions ();
  CORBA::ExceptionDefSeq te;
  te.length (se->length());

  for (CORBA::ULong i2=0; i2<se->length(); i2++) {
    te[i2] = transform_Exception (se[i2]);
  }

  CORBA::OperationDef_var res =
    destiface->create_operation (fid.get(), fid.name.c_str(),
				 fid.version.c_str(), managedif,
				 CORBA::OP_NORMAL,
				 tp, te, sc);

  _db.register_repoid (fid.sget(), fname);

  /*
   * Also add this operation to the home's local interface
   */

  RepoId exifid (origid);
  exifid.container.back().insert (0, "CCM_");
  exifid.container.back() += "Explicit";
  exifid.name = exifid.container.back();
  exifid.container.pop_back ();

  CORBA::Contained_var lexifco = _repo->lookup_id (exifid.get());
  CORBA::InterfaceDef_var lexif = CORBA::InterfaceDef::_narrow (lexifco);
  assert (!CORBA::is_nil (lexifco));

  RepoId lid (origid);
  lid.container.back().insert (0, "CCM_");
  lid.container.back() += "Explicit";

  tempco = _repo->lookup ("Components::EnterpriseComponent");
  CORBA::IDLType_var tt = CORBA::IDLType::_narrow (tempco);

  CORBA::OperationDef_var lres =
    lexif->create_operation (lid.get(),
			     lid.name.c_str(),
			     lid.version.c_str(),
			     tt, CORBA::OP_NORMAL,
			     tp, te, sc);

  _db.register_repoid (lid.sget(), fname);

  return res._retn ();
}

CORBA::InterfaceDef_ptr
CCMTransformer::transform_Home (CORBA::ComponentIR::HomeDef_ptr src)
{
  CORBA::String_var id = src->id ();
  CORBA::String_var name = src->name ();
  CORBA::String_var version = src->version ();

  CORBA::Contained_var existing = _repo->lookup_id (id);

  if (!CORBA::is_nil (existing)) {
    return CORBA::InterfaceDef::_narrow (existing);
  }

  CORBA::Container_var defin = src->defined_in ();
  CORBA::Container_var dest = transform_Container (defin);

  CORBA::ComponentIR::HomeDef_var base = src->base_home ();
  CORBA::InterfaceDefSeq_var supported = src->supported_interfaces ();
  CORBA::ComponentIR::ComponentDef_var managed = src->managed_component ();
  CORBA::ValueDef_var key = src->primary_key ();

  RepoId origid (id.in(), src);
  string fname = _db.get_fname_for_repoid (origid.sget());
  CORBA::InterfaceDef_var explif, implif, res;

  CORBA::Contained_var tempco = _repo->lookup_id (origid.get());

  if (!CORBA::is_nil (tempco)) {
    return CORBA::InterfaceDef::_narrow (tempco);
  }

  /*
   * Original:
   *                  home <name> manages <component> { ... };
   * Equivalent IDL:
   *                  interface <name>Explicit : Components::CCMHome
   *                  { ... };
   *
   *                  interface <name>Implicit : Components::KeylessCCMHome {
   *                  {
   *                    <component> create ();
   *                  };
   *
   *                  interface <name> : <name>Explicit, <name>Implicit {};
   */

  /*
   * Explicit Interface
   */

  {
    RepoId explicitid (origid);
    explicitid.name += "Explicit";

    CORBA::InterfaceDefSeq tb;
    tb.length (supported->length() + 1);

    if (CORBA::is_nil (base)) {
      tempco = _repo->lookup ("Components::CCMHome");
      tb[0] = CORBA::InterfaceDef::_narrow (tempco);
    }
    else {
      tempco = transform_Contained (base);
      tb[0] = CORBA::InterfaceDef::_narrow (tempco);
    }

    for (CORBA::ULong i=0; i<supported->length(); i++) {
      tempco = transform_Contained (supported[i]);
      tb[i+1] = CORBA::InterfaceDef::_narrow (tempco);
    }

    explif = dest->create_interface (explicitid.get(),
				     explicitid.name.c_str(),
				     explicitid.version.c_str(),
				     tb);

    _db.register_repoid (explicitid.sget(), fname);
  }

  /*
   * Implicit Interface
   */

  {
    RepoId implicitid (origid);
    implicitid.name += "Implicit";

    CORBA::InterfaceDefSeq tb;

    if (CORBA::is_nil (key)) {
      CORBA::Contained_var tempco =
	_repo->lookup ("Components::KeylessCCMHome");
      tb.length (1);
      tb[0] = CORBA::InterfaceDef::_narrow (tempco);
    }

    implif = dest->create_interface (implicitid.get(),
				     implicitid.name.c_str(),
				     implicitid.version.c_str(),
				     tb);

    _db.register_repoid (implicitid.sget(), fname);

    /*
     * Operations on implicit interface differ for keyless and keyful homes
     */

    assert (CORBA::is_nil (key));

    /*
     * <component> create ();
     */

    RepoId createid (implicitid);
    createid.container.push_back (createid.name);
    createid.name = "create";

    CORBA::ParDescriptionSeq tp;
    CORBA::ContextIdSeq tc;
    CORBA::ExceptionDefSeq te;
    
    CORBA::IDLType_var result = transform_IDLType (managed);
    
    CORBA::OperationDef_var tempop =
      implif->create_operation (createid.get(), createid.name.c_str(),
				createid.version.c_str(), result,
				CORBA::OP_NORMAL,
				tp, te, tc);

    _db.register_repoid (createid.sget(), fname);
  }

  /*
   * Home equivalent interface
   */

  {
    CORBA::InterfaceDefSeq tb;
    tb.length (2);

    tb[0] = CORBA::InterfaceDef::_duplicate (explif);
    tb[1] = CORBA::InterfaceDef::_duplicate (implif);

    res = dest->create_interface (origid.get(), origid.name.c_str(),
				  origid.version.c_str(), tb);
  }

  /*
   * Original:        home <name> { ... };
   * Equivalent IDL:  local interface CCM_<name>Explicit { ... };
   *                  local interface CCM_<name>Implicit { ... };
   *                  local interface CCM_<name> :
   *                    CCM_<name>Explicit, CCM_<name>Implicit
   *                  {};
   */

  RepoId lid (origid);
  lid.name.insert (0, "CCM_");

  CORBA::InterfaceDef_var lexif;
  CORBA::InterfaceDef_var limif;

  /*
   * Home explicit executor interface
   */

  {
    RepoId exifid (lid);
    exifid.name += "Explicit";

    CORBA::InterfaceDefSeq tb;
    tb.length (supported->length() + 1);

    if (CORBA::is_nil (base)) {
      CORBA::Contained_var tempco =
	_repo->lookup ("Components::HomeExecutorBase");
      tb[0] = CORBA::InterfaceDef::_narrow (tempco);
    }
    else {
      CORBA::String_var bid = base->id ();
      RepoId sbid (bid.in());
      sbid.name.insert (0, "CCM_");
      sbid.name += "Explicit";
      CORBA::Contained_var tempco = _repo->lookup_id (sbid.get());
      tb[0] = CORBA::InterfaceDef::_narrow (tempco);
    }

    for (CORBA::ULong i=0; i<supported->length(); i++) {
      CORBA::IDLType_var ttemp = transform_IDLType (supported[i]);
      tb[i+1] = CORBA::InterfaceDef::_narrow (ttemp);
    }

    lexif = dest->create_local_interface (exifid.get(), exifid.name.c_str(),
					  exifid.version.c_str(), tb);

    _db.register_repoid (exifid.sget(), fname);
  }

  /*
   * Home implicit executor interface
   */

  {
    RepoId imifid (lid);
    imifid.name += "Implicit";

    CORBA::InterfaceDefSeq tb;
    limif = dest->create_local_interface (imifid.get(), imifid.name.c_str(),
					  imifid.version.c_str(), tb);

    _db.register_repoid (imifid.sget(), fname);

    /*
     * Add implicit operations
     */

    assert (CORBA::is_nil (key));

    /*
     * Components::EnterpriseComponent create ();
     */

    RepoId createid (imifid);
    createid.container.push_back (createid.name);
    createid.name = "create";

    CORBA::ParDescriptionSeq tp;
    CORBA::ContextIdSeq tc;
    CORBA::ExceptionDefSeq te;
    
    CORBA::Contained_var tempco =
      _repo->lookup ("Components::EnterpriseComponent");
    CORBA::IDLType_var result = CORBA::IDLType::_narrow (tempco);
    
    CORBA::OperationDef_var tempop =
      limif->create_operation (createid.get(), createid.name.c_str(),
			       createid.version.c_str(), result,
			       CORBA::OP_NORMAL,
			       tp, te, tc);

    _db.register_repoid (createid.sget(), fname);
  }

  /*
   * Home main executor interface
   */

  {
    CORBA::InterfaceDefSeq tb;
    tb.length (2);
    tb[0] = CORBA::InterfaceDef::_narrow (lexif);
    tb[1] = CORBA::InterfaceDef::_narrow (limif);

    CORBA::InterfaceDef_var lmif =
      dest->create_local_interface (lid.get(), lid.name.c_str(),
				    lid.version.c_str(), tb);

    _db.register_repoid (lid.sget(), fname);
  }
  
  return res._retn();
}

/*
 * ----------------------------------------------------------------------
 * Main CCM Transformer
 * ----------------------------------------------------------------------
 */

CORBA::Container_ptr
CCMTransform (DB & db, IDLParam & params,
	      CORBA::Repository_ptr repo,
	      CORBA::Container_ptr cont)
{
  /*
   * Preload CCM IDL
   */

  /*
   * Step 0: Locate CCM.idl
   */

  string path_to_ccm_idl;
  if (params.base_dir.empty()) {
    path_to_ccm_idl = ABSEXECDIR;
    if (!path_to_ccm_idl.empty() &&
      path_to_ccm_idl[path_to_ccm_idl.length() - 1] != '/') {
        path_to_ccm_idl += '/';
      }
  } else {
    path_to_ccm_idl = params.base_dir;
  }
  path_to_ccm_idl += "include/mico/CCM.idl";

  if (OSMisc::access (path_to_ccm_idl.c_str(), OSMisc::ACCESS_READ) != 0) {
    cerr << "error: CCM.idl not found" << endl;
    exit (1);
  }

  /*
   * Step 1: Invoke the Preprocessor
   */

  string cmd = params.cpp;
  cmd += " -D __MICO_IDL__ ";
  cmd += params.cpp_options;
  cmd += ' ';

  string includepath;
  if (params.base_dir.empty()) {
    includepath = ABSEXECDIR;
    if (!includepath.empty() && includepath[includepath.length() - 1] != '/')
      includepath += '/';
  } else {
    includepath = params.base_dir;
  }
  includepath += "include/";
  string micopath = includepath + "mico/";

  cmd += "-isystem ";
  cmd += includepath;
  cmd += ' ';
  cmd += " -isystem ";
  cmd += micopath;
  cmd += ' ';

  cmd += path_to_ccm_idl;

  FILE * inp_file = OSMisc::popen (cmd.c_str(), "r");

  if (inp_file == NULL) {
    cerr << "error: cannot open " << path_to_ccm_idl << endl;
    exit (1);
  }

  int dummy_char = fgetc (inp_file);

  if (dummy_char == EOF) {
    cerr << "error: cannot execute mico-cpp: " << cmd << endl;
    exit( 1 );
  }

  ungetc (dummy_char, inp_file);
  
  /*
   * Step 2: Invoke the Parser (bison)
   */
    
  Parser parser (inp_file, "components.idl");
  parser.parse();
  
  OSMisc::pclose (inp_file);

  /*
   * Step 3: Traverse the Parse Tree and load Interface Repository
   */
  
  IDLParser idlParser (db, params);
  idlParser.collect (repo, parser.getRootNode());
  db.set_repoids (repo);

  /*
   * Traverse the CORBA 3 Interface Repository and transform to CORBA 2
   */
  
  CCMTransformer ct (db, repo);

  if (!ct.transform (cont)) {
    cerr << "error: transformation failed" << endl;
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

  return res;
}

