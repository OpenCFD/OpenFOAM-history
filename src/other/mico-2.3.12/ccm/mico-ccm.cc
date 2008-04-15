/*
 *  Mico --- a free CORBA implementation
 *  Copyright Frank Pilhofer
 *
 *  This file is part of MicoCCM, written by Frank Pilhofer.
 *  The MICO CORBA Component Project was sponsored by Alcatel.
 */

#include <CORBA.h>
#include <stdio.h>
#include <unistd.h>
#include <mico/ir_creator.h>
#include <mico/util.h>
#include <mico/os-misc.h>
#include "parser.h"
#include "idlparser.h"
#include "params.h"
#include "db.h"
#include "codegen.h"
#include "codegen-c++-util.h"


using namespace std;

static const char *copyright[] = {
  "/*",
  " *  MICO --- a free CORBA implementation",
  " *",
  " *  MicoCCM written by Frank Pilhofer, sponsored by Alcatel",
  " *",
  " *  This file was automatically generated. DO NOT EDIT!",
  " */"
};

static const int copyright_size = sizeof(copyright) / sizeof(char *);

/*
 * ----------------------------------------------------------------------
 */

class CCMCodeGenCpp : virtual public CodeGenCPPUtil
{
public:
  enum ComponentType {
    ServiceComponent,
    SessionComponent,
    ProcessComponent,
    EntityComponent
  };

  enum ExecutorType {
    MonolithicExecutor,
    ExecutorLocator
  };

  CCMCodeGenCpp (DB &db, IDLParam & params, CORBA::Container_ptr con,
		 ComponentType type, ExecutorType etype,
		 bool standalone);

  void emit (string &fnbase);

private:
  void emit_glue_decl ();
  void emit_glue_code ();

  void emitComponentGlueDecl (CORBA::ComponentIR::ComponentDef_ptr);
  void emitHomeGlueDecl      (CORBA::ComponentIR::HomeDef_ptr);
  void emitComponentGlueImpl (CORBA::ComponentIR::ComponentDef_ptr);
  void emitHomeGlueImpl      (CORBA::ComponentIR::HomeDef_ptr);

  void emitContextDecl       (CORBA::ComponentIR::ComponentDef_ptr);
  void emitContextImpl       (CORBA::ComponentIR::ComponentDef_ptr);

  void emitInterfaceGlueDecl (CORBA::InterfaceDef_ptr,
			      CORBA::ComponentIR::ComponentDef_ptr,
			      const char *);
  void emitInterfaceGlueImpl (CORBA::InterfaceDef_ptr,
			      CORBA::ComponentIR::ComponentDef_ptr,
			      const char *);

  void emitConsumerGlueDecl  (CORBA::ComponentIR::EventDef_ptr,
			      CORBA::ComponentIR::ComponentDef_ptr,
			      const char *);
  void emitConsumerGlueImpl  (CORBA::ComponentIR::EventDef_ptr,
			      CORBA::ComponentIR::ComponentDef_ptr,
			      const char *);

  bool ccm_check_for_included_defn (CORBA::IRObject_ptr obj,
				    bool emitinc = true);

private:
  bool _standalone;
  ComponentType _type;
  ExecutorType _etype;
};

/*
 * ----------------------------------------------------------------------
 */

CCMCodeGenCpp::CCMCodeGenCpp (DB & db, IDLParam & params,
			      CORBA::Container_ptr con,
			      ComponentType type,
			      ExecutorType etype,
			      bool standalone)
  : CodeGen (con), CodeGenCPPUtil (db, params, con)
{
  _type = type;
  _etype = etype;
  _standalone = standalone;
  assert (_type == ServiceComponent ||
	  _type == SessionComponent);
  assert (_etype == MonolithicExecutor ||
	  _etype == ExecutorLocator);
}

void
CCMCodeGenCpp::emit (string & fn)
{
  string fnbase = fn;

  for (string::size_type i0=0; i0<fnbase.length(); i0++) {
    fnbase[i0] = isalnum(fnbase[i0]) ? toupper(fnbase[i0]) : '_';
  }

  string fnHeader = fn + "_ccm." + _params.hh_suffix;
  string fnStub = fn + "_ccm." + _params.cpp_suffix;

  /*
   * Generate Header File
   */

  ofstream header (fnHeader.c_str());

  if (!header) {
    cerr << "error: cannot open file " << fnHeader << " for writing"
	 << endl;
    return;
  }

  for (int i1=0; i1<copyright_size; i1++)
    header << copyright[ i1 ] << endl;

  header << endl;
  header << "#include <CORBA.h>" << endl;
  header << "#include <mico/CCMContainer.h>" << endl;
  header << "#include ";
  header << ( _params.use_quotes ? "\"" : "<" );
  header << _params.hh_prefix << fn << "." << _params.hh_suffix;
  header << ( _params.use_quotes ? "\"" : ">" );
  header << endl;
  header << endl;

  header << "#ifndef __" << fnbase << "__CCM__H__" << endl;
  header << "#define __" << fnbase << "__CCM__H__" << endl;

  o.start_output (header);
  emit_glue_decl ();
  o.stop_output ();

  header << "#endif" << endl << endl;

  /*
   * Generate Glue Code
   */

  ofstream impl (fnStub.c_str());

  if (!impl) {
    cerr << "error: cannot open file " << fnStub << " for writing"
	 << endl;
    return;
  }

  for (int i2=0; i2<copyright_size; i2++)
    impl << copyright[i2] << endl;

  impl << endl;

  if (_standalone) {
    impl << "#include <coss/CosNaming.h>" << endl;
    impl << "#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS" << endl;
    impl << "#include <iostream>" << endl;
    impl << "#include <fstream>" << endl;
    impl << "#else // HAVE_ANSI_CPLUSPLUS_HEADERS" << endl;
    impl << "#include <iostream.h>" << endl;
    impl << "#include <fstream.h>" << endl;
    impl << "#endif // HAVE_ANSI_CPLUSPLUS_HEADERS" << endl;
  }

  impl << "#include ";
  impl << ( _params.use_quotes ? "\"" : "<" );
  impl << _params.hh_prefix << fn << "_ccm." << _params.hh_suffix;
  impl << ( _params.use_quotes ? "\"" : ">" );
  impl << endl;
  impl << endl;
  impl << "using namespace std;" << endl;
  impl << endl;

  o.start_output (impl);
  emit_glue_code ();
  o.stop_output ();
}

/*
 * ----------------------------------------------------------------------
 */

void
CCMCodeGenCpp::emit_glue_decl ()
{
  /*
   * First pass: emit includes
   */

  {
    for (CORBA::ULong i=0; i<_idl_objs.length(); i++) {
      ccm_check_for_included_defn (_idl_objs[i]->obj);
    }

    o << endl;
  }

  /*
   * Second pass: emit decls for glue code classes
   */

  o << "namespace MICO_CCM_impls " << BL_OPEN << endl;

  for (CORBA::ULong i=0; i<_idl_objs.length(); i++) {
    if (_idl_objs[i]->iface_as_forward) {
      continue;
    }
    
    if (ccm_check_for_included_defn (_idl_objs[i]->obj)) {
      continue;
    }

    CORBA::IRObject_ptr obj = _idl_objs[i]->obj;

    switch (obj->def_kind()) {
    case CORBA::dk_Component:
      {
	CORBA::ComponentIR::ComponentDef_var v =
	  CORBA::ComponentIR::ComponentDef::_narrow (obj);
	emitComponentGlueDecl (v);
	emitContextDecl (v);
	break;
      }

    case CORBA::dk_Home:
      {
	CORBA::ComponentIR::HomeDef_var v =
	  CORBA::ComponentIR::HomeDef::_narrow (obj);
	emitHomeGlueDecl (v);
	break;
      }
    default:
      break;
    }
  }

  o << "// end of namespace MICO_CCM_impls" << endl;
  o << BL_CLOSE << endl;
}

void
CCMCodeGenCpp::emit_glue_code ()
{
  /*
   * First pass: emit glue code implementations
   */

  for (CORBA::ULong i=0; i<_idl_objs.length(); i++) {
    if (_idl_objs[i]->iface_as_forward) {
      continue;
    }
    
    if (ccm_check_for_included_defn (_idl_objs[i]->obj)) {
      continue;
    }

    CORBA::IRObject_ptr obj = _idl_objs[i]->obj;

    switch (obj->def_kind()) {
    case CORBA::dk_Component:
      {
	CORBA::ComponentIR::ComponentDef_var v =
	  CORBA::ComponentIR::ComponentDef::_narrow (obj);
	emitComponentGlueImpl (v);
	emitContextImpl (v);
	break;
      }

    case CORBA::dk_Home:
      {
	CORBA::ComponentIR::HomeDef_var v =
	  CORBA::ComponentIR::HomeDef::_narrow (obj);
	emitHomeGlueImpl (v);
	break;
      }
    default:
      break;
    }
  }

  /*
   * Second pass. Emit entry points
   */

  o << endl
    << "/*" << endl
    << " * Glue entry points for the container" << endl
    << " */" << endl
    << endl;

  for (CORBA::ULong i1=0; i1<_idl_objs.length(); i1++) {
    if (_idl_objs[i1]->iface_as_forward) {
      continue;
    }
    
    if (ccm_check_for_included_defn (_idl_objs[i1]->obj)) {
      continue;
    }

    CORBA::IRObject_ptr obj = _idl_objs[i1]->obj;

    switch (obj->def_kind()) {
    case CORBA::dk_Home:
      {
	CORBA::ComponentIR::HomeDef_var v =
	  CORBA::ComponentIR::HomeDef::_narrow (obj);
	CORBA::String_var an = v->absolute_name ();
	string abs_name = an.in() + 2;
	CORBA::ULong idx;

	string ccm_abs_name = abs_name;

	if ((idx = ccm_abs_name.rfind ("::")) != (CORBA::ULong) -1) {
	  ccm_abs_name.insert (idx+2, "CCM_");
	}
	else {
	  ccm_abs_name.insert (0, "CCM_");
	}

	string flat_name = abs_name;

	while ((idx = flat_name.find ("::")) != (CORBA::ULong) -1) {
	  flat_name.replace (idx, 2, "_");
	}

	CORBA::ComponentIR::ComponentDef_var comp = v->managed_component ();

	an = comp->absolute_name ();
	string comp_abs_name = an.in() + 2;

	string comp_ccm_abs_name = comp_abs_name;

	if ((idx = comp_ccm_abs_name.rfind ("::")) != (CORBA::ULong) -1) {
	  comp_ccm_abs_name.insert (idx+2, "CCM_");
	}
	else {
	  comp_ccm_abs_name.insert (0, "CCM_");
	}

	string comp_flat_name = comp_abs_name;

	while ((idx = comp_flat_name.find ("::")) != (CORBA::ULong) -1) {
	  comp_flat_name.replace (idx, 2, "_");
	}

	CORBA::String_var home_short_name = v->name ();
	CORBA::String_var home_absolute_name = v->absolute_name ();
	CORBA::String_var home_id = v->id ();

	CORBA::String_var comp_short_name = comp->name ();
	CORBA::String_var comp_absolute_name = comp->absolute_name ();
	CORBA::String_var comp_id = comp->id ();

	o << "extern \"C\" " << BL_OPEN;
	o << "MICO::CCM::ContainerBase *" << endl;
	o << "deploy_" << flat_name << " (CORBA::ORB_ptr _orb, "
	  << "Components::HomeExecutorBase_ptr _he)" << endl
	  << BL_OPEN;
	if (_type == ServiceComponent) {
	  o << "MICO::CCM::ServiceContainer * _container =" << endl
	    << "  new MICO::CCM::ServiceContainer (_orb);" << endl << endl;
	  o << "MICO::CCM::ServiceContainer::ComponentInfo info;" << endl;
	}
	else if (_type == SessionComponent) {
	  o << "MICO::CCM::SessionContainer * _container =" << endl
	    << "  new MICO::CCM::SessionContainer (_orb);" << endl << endl;
	  o << "MICO::CCM::SessionContainer::ComponentInfo info;" << endl;
	}

	o << endl;
	o << "info.home_short_name = \"" << home_short_name << "\";" << endl;
	o << "info.home_absolute_name = \"" << home_absolute_name << "\";" << endl;
	o << "info.home_id = \"" << home_id << "\";" << endl;
	o << "info.component_short_name = \"" << comp_short_name << "\";" << endl;
	o << "info.component_absolute_name = \"" << comp_absolute_name << "\";" << endl;
	o << "info.component_id = \"" << comp_id << "\";" << endl;
	o << endl;

	o << ccm_abs_name << "_ptr _hi = "
	  << ccm_abs_name << "::_narrow (_he);" << endl;
	o << "info.home_instance = CORBA::LocalObject::_duplicate (_hi);"
	  << endl;
	o << "info.home_glue = new MICO_CCM_impls::"
	  << flat_name << "_impl (_hi, _container);" << endl;

	if (_type == ServiceComponent) {
	  o << "Components::EnterpriseComponent_var _ci = _hi->create ();"
	    << endl;

	  if (_etype == MonolithicExecutor) {
	    o << comp_ccm_abs_name << "_ptr _ti =" << endl
	      << "  " << comp_ccm_abs_name << "::_narrow (_ci);" << endl;
	  }
	  else {
	    o << "Components::ExecutorLocator_ptr _ti =" << endl
	      << "  " << "Components::ExecutorLocator::_narrow (_ci);"
	      << endl;
	  }

	  o << "info.comp_instance = CORBA::LocalObject::_duplicate (_ci);"
	    << endl;
	  o << "info.comp_glue = new MICO_CCM_impls::"
	    << comp_flat_name << "_impl (_ti, _container);" << endl;
	}

	o << endl;
	o << "_container->load (info);" << endl;
	o << "return _container;" << endl;
	o << BL_CLOSE << BL_CLOSE << endl;
	break;
      }
    default:
      break;
    }
  }

  /*
   * Third pass. Emit main method if standalone
   */

  if (_standalone) {
    o << endl
      << "/*" << endl
      << " * Default main for standalone program" << endl
      << " */" << endl
      << endl;

    /*
     * Emit prototypes for user-written home factories
     */

    o << "extern \"C\" " << BL_OPEN;

    for (CORBA::ULong i2=0; i2<_idl_objs.length(); i2++) {
      if (_idl_objs[i2]->iface_as_forward) {
	continue;
      }
      
      if (ccm_check_for_included_defn (_idl_objs[i2]->obj)) {
	continue;
      }
      
      CORBA::IRObject_ptr obj = _idl_objs[i2]->obj;
      
      switch (obj->def_kind()) {
      case CORBA::dk_Home:
	{
	  CORBA::ComponentIR::HomeDef_var v =
	    CORBA::ComponentIR::HomeDef::_narrow (obj);
	  CORBA::String_var an = v->absolute_name ();
	  string abs_name = an.in() + 2;
	  string flat_name = abs_name;
	  CORBA::ULong idx;
	  
	  while ((idx = flat_name.find ("::")) != (CORBA::ULong) -1) {
	    flat_name.replace (idx, 2, "_");
	  }
	  
	  o << "Components::HomeExecutorBase_ptr "
	    << "create_" << flat_name << " (CORBA::ORB_ptr);"
	    << endl;
	}
      default:
	break;
      }
    }

    o << BL_CLOSE << endl;

    o << "int" << endl;
    o << "main (int argc, char * argv[])" << endl;
    o << BL_OPEN;
    o << "CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);" << endl;
    o << "MICO::CCM::register_all_factories (orb);" << endl;
    o << "string ior;" << endl;
    o << "string nsname;" << endl;
    o << "bool all = true;" << endl;
    o << "string homename;" << endl;
    o << "int arg;" << endl;
    o << endl;

    /*
     * Options: --ior <where> (dump IOR)
     *          --ns <name>   (register homes in Naming Service)
     *          --help        (self-explanatory)
     *          <name>        (register this home only)
     */

    o << "for (arg=1; arg<argc; arg++) " << BL_OPEN;
    o << "if (*argv[arg] != '-') " << BL_OPEN;
    o << "break;" << endl;
    o << BL_CLOSE;
    o << "else if (strcmp (argv[arg], \"--ior\") == 0 && arg+1<argc) "
      << BL_OPEN;
    o << "ior = argv[++arg];" << endl;
    o << BL_CLOSE;
    o << "else if (strcmp (argv[arg], \"--ns\") == 0 && arg+1<argc) "
      << BL_OPEN;
    o << "nsname = argv[++arg];" << endl;
    o << BL_CLOSE;
    o << "else if (strcmp (argv[arg], \"--\") == 0) "
      << BL_OPEN;
    o << "arg++;" << endl;
    o << "break;" << endl;
    o << BL_CLOSE;
    o << "else " << BL_OPEN;
    o << "if (strcmp (argv[arg], \"--help\") != 0) "
      << BL_OPEN;
    o << "cerr << \"error: unknown option: \\\"\" << argv[arg] << \"\\\"\" << endl;" << endl;
    o << BL_CLOSE;
    o << "cerr << \"usage: \" << argv[arg] << \" <options> [name]\" << endl;" << endl;
    o << "cerr << \"possible <options> are:\" << endl;" << endl;
    o << "cerr << \"    --ior <filename>\" << endl;" << endl;
    o << "cerr << \"    --ns <ns name>\" << endl;" << endl;
    o << "cerr << \"    --help\" << endl;" << endl;
    o << "cerr << \"possible names are:\" << endl;" << endl;

    for (CORBA::ULong i4=0; i4<_idl_objs.length(); i4++) {
      if (_idl_objs[i4]->iface_as_forward) {
	continue;
      }
      
      if (ccm_check_for_included_defn (_idl_objs[i4]->obj)) {
	continue;
      }
      
      CORBA::IRObject_ptr obj = _idl_objs[i4]->obj;
      
      switch (obj->def_kind()) {
      case CORBA::dk_Home:
	{
	  CORBA::ComponentIR::HomeDef_var v =
	    CORBA::ComponentIR::HomeDef::_narrow (obj);
	  CORBA::String_var an = v->absolute_name ();
	  CORBA::String_var ln = v->name ();
	  CORBA::String_var id = v->id ();
	  o << "cerr << \"    " << ln << " " << an << " " << id
	    << "\" << endl;" << endl;
	}
      default:
	break;
      }
    }

    o << "exit (1);" << endl;
    o << BL_CLOSE; // end of if
    o << BL_CLOSE << endl; // end of for

    o << "if (arg < argc) " << BL_OPEN;
    o << "homename = argv[arg];" << endl;
    o << "all = false;" << endl;
    o << BL_CLOSE << endl;

    /*
     * Contact Naming Service
     */

    o << "CosNaming::NamingContextExt_var ns;" << endl;
    o << "CORBA::Object_var nsobj;" << endl;
    o << "try " << BL_OPEN;
    o << "nsobj = orb->resolve_initial_references (\"NameService\");" << endl;
    o << "ns = CosNaming::NamingContextExt::_narrow (nsobj);" << endl;
    o << BL_CLOSE;
    o << "catch (...) " << BL_OPEN;
    o << "ns = CosNaming::NamingContextExt::_nil ();" << endl;
    o << BL_CLOSE << endl;
    o << "if (CORBA::is_nil (ns) && (all || nsname.length() > 0)) "
      << BL_OPEN;
    o << "cerr << \"Oops, I need a Naming Service.\" << endl;" << endl;
    o << "exit (1);" << endl;
    o << BL_CLOSE << endl;

    o << "Components::HomeExecutorBase_var he;" << endl;
    o << "MICO::CCM::ContainerBase * c;" << endl;
    o << "Components::CCMHome_var hp;" << endl;
    o << "CosNaming::Name_var n;" << endl;
    o << endl;

    for (CORBA::ULong i3=0; i3<_idl_objs.length(); i3++) {
      if (_idl_objs[i3]->iface_as_forward) {
	continue;
      }
      
      if (ccm_check_for_included_defn (_idl_objs[i3]->obj)) {
	continue;
      }
      
      CORBA::IRObject_ptr obj = _idl_objs[i3]->obj;
      
      switch (obj->def_kind()) {
      case CORBA::dk_Home:
	{
	  CORBA::ComponentIR::HomeDef_var v =
	    CORBA::ComponentIR::HomeDef::_narrow (obj);
	  CORBA::String_var an = v->absolute_name ();
	  CORBA::String_var ln = v->name ();
	  CORBA::String_var id = v->id ();
	  string abs_name = an.in() + 2;
	  string flat_name = abs_name;
	  CORBA::ULong idx;
	  
	  while ((idx = flat_name.find ("::")) != (CORBA::ULong) -1) {
	    flat_name.replace (idx, 2, "_");
	  }

	  o << "/*" << endl
	    << " * Deploy " << an << endl
	    << " */" << endl << endl;

	  o << "if (all || " << endl
	    << "    strcmp (homename.c_str(), \"" << ln << "\") == 0 || "
	    << endl
	    << "    strcmp (homename.c_str(), \"" << an << "\") == 0 || "
	    << endl
	    << "    strcmp (homename.c_str(), \"" << id << "\") == 0) "
	    << BL_OPEN;
	  o << "he = create_" << flat_name << " (orb);" << endl;
	  o << "c  = deploy_" << flat_name << " (orb, he);" << endl;
	  o << "hp = c->get_CCM_home ();" << endl << endl;

	  o << "if (nsname.length() > 0) " << BL_OPEN;
	  o << "n = ns->to_name (nsname.c_str());" << endl;
	  o << "ns->rebind (n.in(), hp.in());" << endl;
	  o << BL_CLOSE;
	  o << "else if (all) " << BL_OPEN;
	  o << "n  = ns->to_name (\"" << ln.in() << "\");" << endl;
	  o << "ns->rebind (n.in(), hp.in());" << endl;
	  o << BL_CLOSE << endl;

	  o << "if (ior.length() > 0) " << BL_OPEN;
	  o << "CORBA::String_var iorstr = orb->object_to_string (hp.in());"
	    << endl;
	  o << "if (strcmp (ior.c_str(), \"-\") == 0) " << BL_OPEN;
	  o << "cout << iorstr << endl;" << endl;
	  o << BL_CLOSE;
	  o << "else " << BL_OPEN;
	  o << "ofstream out (ior.c_str());" << endl;
	  o << "out << iorstr << endl;" << endl;
	  o << BL_CLOSE;
	  o << BL_CLOSE << endl;

	  o << "c->activate ();" << endl;
	  o << BL_CLOSE << endl;
	}
      default:
	break;
      }
    }

    o << endl;
    o << "orb->run ();" << endl;
    o << "return 0;" << endl;
    o << BL_CLOSE << endl;
  }
}

/*
 * ----------------------------------------------------------------------
 *
 * This code produces component glue code. It is a POA Servant, implements
 * the non-functional component methods (navigation and such) and forwards
 * the business methods to the component instance.
 *
 * ----------------------------------------------------------------------
 */

void
CCMCodeGenCpp::emitComponentGlueDecl (CORBA::ComponentIR::ComponentDef_ptr comp)
{
  CORBA::String_var n = comp->name();
  CORBA::String_var a = comp->absolute_name();
  string className = (const char *) ID(n);
  string absName = (const char *) ID(a);
  absName = absName.substr (2);
  CORBA::ULong idx;

  string flatName = absName;

  while ((idx = flatName.find ("::")) != (CORBA::ULong) -1) {
    flatName.replace (idx, 2, "_");
  }

  string ccmAbsName = absName;

  if ((idx = ccmAbsName.rfind ("::")) != (CORBA::ULong) -1) {
    ccmAbsName.insert (idx+2, "CCM_");
  }
  else {
    ccmAbsName.insert (0, "CCM_");
  }

  o << "/*" << endl
    << " * Glue class for component " << absName << endl
    << " */" << endl << endl;

  o << "class " << flatName << "_impl :";

  CORBA::ComponentIR::ComponentDef_var base = comp->base_component ();

  if (CORBA::is_nil (base)) {
    o << indent << indent << endl;
    o << "virtual public POA_" << absName << "," << endl;
    o << "virtual public PortableServer::RefCountServantBase";
    o << exdent << exdent << endl << BL_OPEN;
  }
  else {
    a = base->absolute_name ();
    string baseFlatName = (const char *) ID(a.in());
    baseFlatName = baseFlatName.substr (2);

    while ((idx = baseFlatName.find ("::")) != (CORBA::ULong) -1) {
      baseFlatName.replace (idx, 2, "_");
    }
    
    o << indent << indent << endl;
    o << "virtual public POA_" << absName << "," << endl;
    o << "virtual public " << baseFlatName << "_impl";
    o << exdent << exdent << endl;
    o << BL_OPEN;
  }

  CORBA::ContainedSeq_var c;

  /*
   * We have the context, facet glue and consumer classes as friends
   */

  o << indent;

  o << "friend class " << flatName << "_Context_impl;" << endl;

  {
    c = comp->contents (CORBA::dk_Provides, 1);

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::ProvidesDef_var attr =
	CORBA::ComponentIR::ProvidesDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));
      CORBA::String_var n = attr->name();
      o << "friend class " << flatName << "_" << n << "_impl;" << endl;
    }
  }

  {
    c = comp->contents (CORBA::dk_Consumes, 1);

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::ConsumesDef_var attr =
	CORBA::ComponentIR::ConsumesDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));
      CORBA::ComponentIR::EventDef_var event = attr->event ();
      CORBA::String_var n = attr->name();
      o << "friend class " << flatName << "_" << n << "_impl;" << endl;
    }
  }

  o << exdent << endl;

  o << "private:" << indent << endl;
  if (_etype == MonolithicExecutor) {
    o << ccmAbsName << " * _instance;" << endl;
  }
  else {
    o << "Components::ExecutorLocator * _instance;" << endl;
  }
  o << ccmAbsName << "_Context * _context;" << endl;

  if (_type == ServiceComponent) {
    o << "MICO::CCM::ServiceContainer * _container;" << endl;
  }
  else if (_type == SessionComponent) {
    o << "MICO::CCM::SessionContainer * _container;" << endl;
  }

  /*
   * Variables for facets
   */

  {
    c = comp->contents (CORBA::dk_Provides, 1);
    
    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::ProvidesDef_var attr =
	CORBA::ComponentIR::ProvidesDef::_narrow(c[j]);
      CORBA::InterfaceDef_var iface = attr->interface_type();
      CORBA::String_var n = attr->name ();
      assert (!CORBA::is_nil(attr));
      
      CORBA::String_var a = iface->absolute_name ();
      string ifaceAbsName = (const char *) ID(a);
      ifaceAbsName = ifaceAbsName.substr (2);

      string ccmIfaceAbsName = ifaceAbsName;

      if ((idx = ccmIfaceAbsName.rfind ("::")) != (CORBA::ULong) -1) {
	ccmIfaceAbsName.insert (idx+2, "CCM_");
      }
      else {
	ccmIfaceAbsName.insert (0, "CCM_");
      }

      o << "PortableServer::ServantBase_var _glue_for_facet_" << n << ";" << endl;
      o << ccmIfaceAbsName << "_var "
	<< "_instance_for_facet_" << n << ";" << endl;
      o << ifaceAbsName << "_var "
	<< "_reference_for_facet_" << n << ";" << endl;
    }
  }

  /*
   * Variables for receptacles
   */

  {
    c = comp->contents (CORBA::dk_Uses, 1);
    
    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::UsesDef_var attr =
	CORBA::ComponentIR::UsesDef::_narrow(c[j]);
      CORBA::String_var n = attr->name ();
      assert (!CORBA::is_nil(attr));
      
      if (!attr->is_multiple()) {
	CORBA::IDLType_var type_id = attr->interface_type();
	emit_type_for_variable (type_id, automatic);
	o << " _for_recep_" << n << ";" << endl;
      }
      else {
	o << absName << "::" << n << "Connections" 
	  << " _for_recep_" << n << ";" << endl;
	o << "CORBA::ULong _cookie_for_" << n << ";" << endl;
      }
    }
  }

  /*
   * Variables for emitters
   */

  {
    c = comp->contents (CORBA::dk_Emits, 1);
    
    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::EmitsDef_var attr =
	CORBA::ComponentIR::EmitsDef::_narrow(c[j]);
      CORBA::ComponentIR::EventDef_var event = attr->event();
      CORBA::String_var event_name = event->absolute_name();
      CORBA::String_var n = attr->name ();
      assert (!CORBA::is_nil(attr));
      
      o << event_name << "Consumer_var";
      o << " _for_emitter_" << n << ";" << endl;
    }
  }

  /*
   * Variables for publishers
   */

  {
    c = comp->contents (CORBA::dk_Publishes, 1);

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::PublishesDef_var attr =
	CORBA::ComponentIR::PublishesDef::_narrow(c[j]);
      CORBA::ComponentIR::EventDef_var event = attr->event();
      CORBA::String_var event_name = event->absolute_name();
      CORBA::String_var n = attr->name ();
      assert (!CORBA::is_nil(attr));

      o << "Components::SubscriberDescriptions";
      o << " _for_publisher_" << n << ";" << endl;
      o << "CORBA::ULong _cookie_for_" << n << ";" << endl;
    }
  }

  /*
   * Variables for consumers
   */

  {
    c = comp->contents (CORBA::dk_Consumes, 1);

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::ConsumesDef_var attr =
	CORBA::ComponentIR::ConsumesDef::_narrow(c[j]);
      CORBA::ComponentIR::EventDef_var event = attr->event();
      CORBA::String_var event_name = event->absolute_name();
      CORBA::String_var n = attr->name ();
      assert (!CORBA::is_nil(attr));

      o << event_name << "Consumer_var";
      o << " _for_consumer_" << n << ";" << endl;
    }
  }

  o << exdent << endl;
  o << "public:" << indent << endl;
  o << flatName << "_impl (";
  if (_etype == MonolithicExecutor) {
    o << ccmAbsName << " *, " << endl;
  }
  else {
    o << "Components::ExecutorLocator *, " << endl;
  }
  if (_type == ServiceComponent) {
    o << "MICO::CCM::ServiceContainer *";
  }
  else if (_type == SessionComponent) {
    o << "MICO::CCM::SessionContainer *";
  }
  o << ");" << endl;
  o << "~" << flatName << "_impl ();" << endl;
  o << endl;

  /*
   * Prototypes
   */

  /*
   * Component Attributes
   */

  o << "/*" << endl
    << " * Component Attributes" << endl
    << " */" << endl << endl;

  {
    c = comp->contents (CORBA::dk_Attribute, 1);
    
    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::AttributeDef_var attr = CORBA::AttributeDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));
      CORBA::IDLType_var type_id = attr->type_def();
      CORBA::String_var n = attr->name();
      emit_type_for_result( type_id );
      o << " " << ID( n.in() ) << " ();" << endl;
      
      if( attr->mode() == CORBA::ATTR_NORMAL ) {
	o << "void " << ID( n.in() ) << " (";
	emit_type_for_param( type_id, CORBA::PARAM_IN );
	o << " _value);" << endl;
      }
    }

    if (c->length()) {
      o << endl;
    }
    else {
      o << "// none" << endl << endl;
    }
  }

  /*
   * Supported Interfaces
   */

  o << "/*" << endl
    << " * Supported Interfaces" << endl
    << " */" << endl << endl;

  {
    CORBA::InterfaceDefSeq_var ifs = comp->supported_interfaces ();
    
    for (CORBA::ULong j=0; j<ifs->length(); j++) {
      CORBA::String_var sn = ifs[j]->absolute_name ();
      
      o << "// " << sn << endl << endl;
      
      c = ifs[j]->contents (CORBA::dk_Operation, 0);
      
      for (CORBA::ULong j2=0; j2<c->length(); j2++) {
	CORBA::OperationDef_var op = CORBA::OperationDef::_narrow(c[j2]);
	CORBA::IDLType_var type_id = op->result_def();
	CORBA::String_var n = op->name();
	emit_type_for_result( type_id );
	o << " " << ID( n.in() ) << "(";
	CORBA::ParDescriptionSeq_var p = op->params();
	CORBA::ULong k;
	for( k = 0; k < p->length(); k++ ) {
	  if( k != 0 )
	    o << ",";
	  o << " ";
	  emit_type_for_param( p[ k ].type_def, p[ k ].mode );
	  o << " " << ID(p[ k ].name);
	}
	o << ");" << endl;
      }
      
      if (c->length()) {
	o << endl;
      }
      
      c = ifs[j]->contents (CORBA::dk_Attribute, 0);
      
      for (CORBA::ULong j3=0; j3<c->length(); j3++) {
	CORBA::AttributeDef_var attr = CORBA::AttributeDef::_narrow(c[j3]);
	assert (!CORBA::is_nil(attr));
	CORBA::IDLType_var type_id = attr->type_def();
	CORBA::String_var n = attr->name();
	emit_type_for_result( type_id );
	o << " " << ID( n.in() ) << " ();" << endl;
	
	if( attr->mode() == CORBA::ATTR_NORMAL ) {
	  o << "void " << ID( n.in() ) << " (";
	  emit_type_for_param( type_id, CORBA::PARAM_IN );
	  o << " _value);" << endl;
	}
      }
      
      if (c->length()) {
	o << endl;
      }
      else {
	o << "// none" << endl << endl;
      }
    }

    if (ifs->length() == 0) {
      o << "// none" << endl << endl;
    }
  }
    
  /*
   * Equivalent IDL methods
   */

  o << "/*" << endl
    << " * Equivalent IDL methods" << endl
    << " */" << endl << endl;

  /*
   * Facets
   */

  {
    c = comp->contents (CORBA::dk_Provides, 1);
    
    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::ProvidesDef_var attr =
	CORBA::ComponentIR::ProvidesDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));
      CORBA::IDLType_var type_id = attr->interface_type();
      CORBA::String_var n = attr->name();
      emit_type_for_result( type_id );
      o << " provide_" << n << " ();" << endl;
    }

    if (c->length()) {
      o << endl;
    }
  }

  /*
   * Receptacles
   */

  {
    c = comp->contents (CORBA::dk_Uses, 1);
    
    if (c->length()) {
      o << "// receptacles" << endl << endl;
    }
    else {
      o << "// no receptacles" << endl << endl;
    }
    
    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::UsesDef_var attr =
	CORBA::ComponentIR::UsesDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));
      
      CORBA::IDLType_var type_id = attr->interface_type();
      CORBA::String_var n = attr->name();
      
      if (!attr->is_multiple()) {
	o << "void connect_" << n << " (";
	emit_type_for_param (type_id, CORBA::PARAM_IN);
	o << " _value);" << endl;
	
	emit_type_for_result (type_id);
	o << " disconnect_" << n << " ();" << endl;
	
	emit_type_for_result (type_id);
	o << " get_connection_" << n << " ();" << endl;
      }
      else {
	o << "Components::Cookie * connect_" << n << " (";
	emit_type_for_param (type_id, CORBA::PARAM_IN);
	o << " _value);" << endl;
	
	emit_type_for_result (type_id);
	o << " disconnect_" << n << " (Components::Cookie * _value);"
	  << endl;
	o << absName << "::" << n << "Connections *"
	  << " get_connections_" << n << " ();"
	  << endl;
      }
    }

    if (c->length()) {
      o << endl;
    }
  }

  /*
   * Publishers
   */

  {
    c = comp->contents (CORBA::dk_Publishes, 1);

    if (c->length()) {
      o << "// publishers" << endl << endl;
    }
    else {
      o << "// no publishers" << endl << endl;
    }

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::PublishesDef_var attr =
	CORBA::ComponentIR::PublishesDef::_narrow (c[j]);
      CORBA::ComponentIR::EventDef_var event = attr->event();
      CORBA::String_var event_name = event->absolute_name();
      CORBA::String_var n = attr->name ();
      assert (!CORBA::is_nil (attr));

      o << "Components::Cookie * subscribe_" << n << " (";
      o << event_name << "Consumer_ptr consumer);" << endl;

      o << event_name << "Consumer_ptr";
      o << " unsubscribe_" << n << " (Components::Cookie * _value);"
	<< endl;
    }

    if (c->length()) {
      o << endl;
    }
  }

  /*
   * Emitters
   */

  {
    c = comp->contents (CORBA::dk_Emits, 1);

    if (c->length()) {
      o << "// emitters" << endl << endl;
    }
    else {
      o << "// no emitters" << endl << endl;
    }

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::EmitsDef_var attr =
	CORBA::ComponentIR::EmitsDef::_narrow (c[j]);
      CORBA::ComponentIR::EventDef_var event = attr->event();
      CORBA::String_var event_name = event->absolute_name();
      CORBA::String_var n = attr->name ();
      assert (!CORBA::is_nil (attr));

      o << "void connect_" << n << " (";
      o << event_name << "Consumer_ptr consumer);" << endl;

      o << event_name << "Consumer_ptr";
      o << " disconnect_" << n << " ();"
	<< endl;
    }

    if (c->length()) {
      o << endl;
    }
  }

  /*
   * Consumers
   */

  {
    c = comp->contents (CORBA::dk_Consumes, 1);

    if (c->length()) {
      o << "// consumers" << endl << endl;
    }
    else {
      o << "// no consumers" << endl << endl;
    }

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::ConsumesDef_var attr =
	CORBA::ComponentIR::ConsumesDef::_narrow (c[j]);
      CORBA::ComponentIR::EventDef_var event = attr->event();
      CORBA::String_var event_name = event->absolute_name();
      CORBA::String_var n = attr->name ();
      assert (!CORBA::is_nil (attr));

      o << event_name << "Consumer_ptr";
      o << " get_consumer_" << n << " ();"
	<< endl;
    }

    if (c->length()) {
      o << endl;
    }
  }

  /*
   * CCMObject interfaces
   */

  o << "/*" << endl
    << " * Inherited Navigation interface" << endl
    << " */" << endl << endl;

  o << "CORBA::Object_ptr provide_facet (const char *);" << endl
    << "Components::FacetDescriptions * get_all_facets ();" << endl
    << "Components::FacetDescriptions * get_named_facets (const Components::NameList &);" << endl
    << "CORBA::Boolean same_component (CORBA::Object_ptr ref);" << endl
    << endl;

  o << "/*" << endl
    << " * Inherited Receptacles interface" << endl
    << " */" << endl << endl;

  o << "Components::Cookie * connect (const char *, CORBA::Object_ptr);" << endl
    << "void disconnect (const char * name, Components::Cookie *);" << endl
    << "Components::ConnectionDescriptions * get_connections (const char *);" << endl
    << "Components::ReceptacleDescriptions * get_all_receptacles ();" << endl
    << "Components::ReceptacleDescriptions * get_named_receptacles (const Components::NameList &);" << endl
    << endl;

  o << "/*" << endl
    << " * Inherited Events interface" << endl
    << " */" << endl << endl;

  o << "Components::EventConsumerBase_ptr get_consumer (const char *);" << endl
    << "Components::Cookie * subscribe (const char *, Components::EventConsumerBase_ptr);" << endl
    << "void unsubscribe (const char * name, Components::Cookie *);" << endl
    << "void connect_consumer (const char *, Components::EventConsumerBase_ptr);" << endl
    << "Components::EventConsumerBase_ptr disconnect_consumer (const char *);" << endl
    << "Components::ConsumerDescriptions * get_all_consumers ();" << endl
    << "Components::ConsumerDescriptions * get_named_consumers (const Components::NameList &);" << endl
    << "Components::EmitterDescriptions * get_all_emitters ();" << endl
    << "Components::EmitterDescriptions * get_named_emitters (const Components::NameList &);" << endl
    << "Components::PublisherDescriptions * get_all_publishers ();" << endl
    << "Components::PublisherDescriptions * get_named_publishers (const Components::NameList &);" << endl
    << endl;

  o << "/*" << endl
    << " * Inherited CCMObject interface" << endl
    << " */" << endl << endl;

  o << "CORBA::Object_ptr get_component_def ();" << endl
    << "Components::CCMHome_ptr get_ccm_home ();" << endl
    << "void configuration_complete ();" << endl
    << "void remove ();" << endl
    << "Components::ComponentPortDescription * get_all_ports ();" << endl
    << endl;

  o << "/*" << endl
    << " * Overload default implementation" << endl
    << " */" << endl
    << endl;

  o << "CORBA::Object_ptr _get_component ();" << endl;
  o << exdent << BL_CLOSE_SEMI << endl << endl;

  /*
   * Emit Glue code for local facets
   */

  {
    c = comp->contents (CORBA::dk_Provides, 1);

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::ProvidesDef_var attr =
	CORBA::ComponentIR::ProvidesDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));
      CORBA::InterfaceDef_var iface = attr->interface_type();
      CORBA::String_var n = attr->name();
      emitInterfaceGlueDecl (iface, comp, n);
    }
  }

  /*
   * Emit Glue code for event consumers
   */

  {
    c = comp->contents (CORBA::dk_Consumes, 1);

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::ConsumesDef_var attr =
	CORBA::ComponentIR::ConsumesDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));
      CORBA::ComponentIR::EventDef_var event = attr->event ();
      CORBA::String_var n = attr->name();
      emitConsumerGlueDecl (event, comp, n);
    }
  }
}

void
CCMCodeGenCpp::emitHomeGlueDecl (CORBA::ComponentIR::HomeDef_ptr home)
{
  CORBA::String_var n = home->name();
  CORBA::String_var a = home->absolute_name();
  string className = (const char *) ID(n);
  string absName = (const char *) ID(a);
  absName = absName.substr (2);
  CORBA::ULong idx;

  string flatName = absName;

  while ((idx = flatName.find ("::")) != (CORBA::ULong) -1) {
    flatName.replace (idx, 2, "_");
  }

  string ccmAbsName = absName;

  if ((idx = ccmAbsName.rfind ("::")) != (CORBA::ULong) -1) {
    ccmAbsName.insert (idx+2, "CCM_");
  }
  else {
    ccmAbsName.insert (0, "CCM_");
  }

  o << "/*" << endl
    << " * Glue class for home " << absName << endl
    << " */" << endl << endl;

  o << "class " << flatName << "_impl :";
  o << indent << indent << endl;
  o << "virtual public POA_" << absName << "," << endl;
  o << "virtual public PortableServer::RefCountServantBase";
  o << exdent << exdent << endl << BL_OPEN;

  o << "private:" << indent << endl;
  o << ccmAbsName << " * _instance;" << endl;

  if (_type == ServiceComponent) {
    o << "MICO::CCM::ServiceContainer * _container;" << endl;
  }
  else if (_type == SessionComponent) {
    o << "MICO::CCM::SessionContainer * _container;" << endl;
  }

  o << exdent << endl;
  o << "public:" << indent << endl;
  o << flatName << "_impl (" << ccmAbsName << " *, ";
  if (_type == ServiceComponent) {
    o << "MICO::CCM::ServiceContainer *";
  }
  else if (_type == SessionComponent) {
    o << "MICO::CCM::SessionContainer *";
  }
  o << ");" << endl;
  o << "~" << flatName << "_impl ();" << endl;
  o << endl;

  o << "/*" << endl
    << " * Implicit operations" << endl
    << " */" << endl << endl;

  CORBA::ComponentIR::ComponentDef_var managed = home->managed_component();
  CORBA::ValueDef_var key = home->primary_key ();
  assert (CORBA::is_nil (key));

  CORBA::ContainedSeq_var c;

  {
    emit_type_for_result (managed);
    o << " create ();" << endl;
    o << endl;
  }

  o << "/*" << endl
    << " * Explicit operations" << endl
    << " */" << endl << endl;

  {
    c = home->contents (CORBA::dk_Factory, 1);
    
    if (c->length()) {
      o << "// factories" << endl << endl;
    }
    else {
      o << "// no factories" << endl << endl;
    }

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::OperationDef_var op = CORBA::OperationDef::_narrow(c[j]);
      CORBA::IDLType_var type_id = op->result_def();
      CORBA::String_var n = op->name();
      emit_type_for_result( type_id );
      o << " " << ID( n.in() ) << "(";
      CORBA::ParDescriptionSeq_var p = op->params();
      CORBA::ULong k;
      for( k = 0; k < p->length(); k++ ) {
	if( k != 0 )
	  o << ",";
	o << " ";
	emit_type_for_param( p[ k ].type_def, p[ k ].mode );
	o << " " << ID(p[ k ].name);
      }
      o << ");" << endl;
    }

    if (c->length()) {
      o << endl;
    }
  }

  {
    c = home->contents (CORBA::dk_Finder, 1);
    
    if (c->length()) {
      o << "// finders" << endl << endl;
    }
    else {
      o << "// no finders" << endl << endl;
    }
    
    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::OperationDef_var op = CORBA::OperationDef::_narrow(c[j]);
      CORBA::IDLType_var type_id = op->result_def();
      CORBA::String_var n = op->name();
      emit_type_for_result( type_id );
      o << " " << ID( n.in() ) << "(";
      CORBA::ParDescriptionSeq_var p = op->params();
      CORBA::ULong k;
      for( k = 0; k < p->length(); k++ ) {
	if( k != 0 )
	  o << ",";
	o << " ";
	emit_type_for_param( p[ k ].type_def, p[ k ].mode );
	o << " " << ID(p[ k ].name);
      }
      o << ");" << endl;
    }

    if (c->length()) {
      o << endl;
    }
  }

  {
    c = home->contents (CORBA::dk_Attribute, 1);
    
    if (c->length()) {
      o << "// attributes" << endl << endl;
    }
    else {
      o << "// no attributes" << endl << endl;
    }

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::AttributeDef_var attr = CORBA::AttributeDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));
      CORBA::IDLType_var type_id = attr->type_def();
      CORBA::String_var n = attr->name();
      emit_type_for_result( type_id );
      o << " " << ID( n.in() ) << " ();" << endl;
      
      if( attr->mode() == CORBA::ATTR_NORMAL ) {
	o << "void " << ID( n.in() ) << " (";
	emit_type_for_param( type_id, CORBA::PARAM_IN );
	o << " _value);" << endl;
      }
    }

    if (c->length()) {
      o << endl;
    }
  }

  {
    c = home->contents (CORBA::dk_Operation, 1);

    if (c->length()) {
      o << "// operations" << endl << endl;
    }
    else {
      o << "// no operations" << endl << endl;
    }

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::OperationDef_var op = CORBA::OperationDef::_narrow(c[j]);
      CORBA::IDLType_var type_id = op->result_def();
      CORBA::String_var n = op->name();
      emit_type_for_result( type_id );
      o << " " << ID( n.in() ) << "(";
      CORBA::ParDescriptionSeq_var p = op->params();
      CORBA::ULong k;
      for( k = 0; k < p->length(); k++ ) {
	if( k != 0 )
	  o << ",";
	o << " ";
	emit_type_for_param( p[ k ].type_def, p[ k ].mode );
	o << " " << ID(p[ k ].name);
      }
      o << ");" << endl;
    }

    if (c->length()) {
      o << endl;
    }
  }

  /*
   * XXX todo: supported interfaces
   */
  
  /*
   * CCMHome interfaces
   */

  o << "/*" << endl
    << " * Inherited CCMHome interface" << endl
    << " */" << endl << endl;

  o << "CORBA::Object_ptr get_component_def ();" << endl
    << "CORBA::Object_ptr get_home_def ();" << endl
    << "void remove_component (Components::CCMObject_ptr comp);" << endl
    << endl;

  o << "/*" << endl
    << " * Inherited KeylessCCMHome interface" << endl
    << " */" << endl << endl;
  
  o << "Components::CCMObject_ptr create_component ();" << endl
    << endl;
  o << exdent << BL_CLOSE_SEMI << endl << endl;
}

/*
 * ----------------------------------------------------------------------
 */

void
CCMCodeGenCpp::emitComponentGlueImpl (CORBA::ComponentIR::ComponentDef_ptr comp)
{
  CORBA::String_var comp_name = comp->name();
  CORBA::String_var comp_abs = comp->absolute_name();
  string className = (const char *) ID(comp_name);
  string absName = (const char *) ID(comp_abs);
  absName = absName.substr (2);
  CORBA::ULong idx;

  string flatName = absName;

  while ((idx = flatName.find ("::")) != (CORBA::ULong) -1) {
    flatName.replace (idx, 2, "_");
  }

  string ccmAbsName = absName;

  if ((idx = ccmAbsName.rfind ("::")) != (CORBA::ULong) -1) {
    ccmAbsName.insert (idx+2, "CCM_");
  }
  else {
    ccmAbsName.insert (0, "CCM_");
  }

  o << "/*" << endl
    << " * Glue class for component " << absName << endl
    << " */" << endl << endl;

  CORBA::ComponentIR::ComponentDef_var base = comp->base_component ();
  CORBA::ContainedSeq_var c;

  /*
   * Constructor
   */

  o << "MICO_CCM_impls::" << endl
    << flatName << "_impl::" << flatName << "_impl ";
  if (_etype == MonolithicExecutor) {
    o << "(" << ccmAbsName << " * i, ";
  }
  else {
    o << "(Components::ExecutorLocator * i, ";
  }
  if (_type == ServiceComponent) {
    o << "MICO::CCM::ServiceContainer * c";
  }
  else if (_type == SessionComponent) {
    o << "MICO::CCM::SessionContainer * c";
  }
  o << ")" << endl;
  o << indent << ": _instance (i), _container (c)";

  if (!CORBA::is_nil (base)) {
    CORBA::String_var a = base->absolute_name ();
    string baseFlatName = (const char *) ID(a.in());
    baseFlatName = baseFlatName.substr (2);

    while ((idx = baseFlatName.find ("::")) != (CORBA::ULong) -1) {
      baseFlatName.replace (idx, 2, "_");
    }

    o << ", " << baseFlatName << "_impl (i, c)";
  }

  o << exdent << endl;
  o << BL_OPEN;
  o << "_context = new " << flatName << "_Context_impl (this, c);" << endl;

  // Initialize variables for facets

  {
    c = comp->contents (CORBA::dk_Provides, 1);
    
    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::ProvidesDef_var attr =
	CORBA::ComponentIR::ProvidesDef::_narrow(c[j]);
      CORBA::InterfaceDef_var iface = attr->interface_type();
      CORBA::String_var n = attr->name ();
      assert (!CORBA::is_nil(attr));
      
      CORBA::String_var a = iface->absolute_name ();
      string ifaceAbsName = (const char *) ID(a);
      ifaceAbsName = ifaceAbsName.substr (2);

      string ccmIfaceAbsName = ifaceAbsName;

      if ((idx = ccmIfaceAbsName.rfind ("::")) != (CORBA::ULong) -1) {
	ccmIfaceAbsName.insert (idx+2, "CCM_");
      }
      else {
	ccmIfaceAbsName.insert (0, "CCM_");
      }

      o << "_glue_for_facet_" << n << " = 0;"
	<< endl;
      o << "_instance_for_facet_" << n << " = "
	<< ccmIfaceAbsName << "::_nil ();" << endl;
    }
  }

  // Initialize variables for receptacles

  {
    c = comp->contents (CORBA::dk_Uses, 1);
    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::UsesDef_var attr =
	CORBA::ComponentIR::UsesDef::_narrow(c[j]);
      CORBA::String_var n = attr->name();

      if (attr->is_multiple()) {
	o << "_cookie_for_" << n << " = 0;" << endl;
      }
    }
  }

  // Initialize variables for publishers

  {
    c = comp->contents (CORBA::dk_Publishes, 1);
    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::PublishesDef_var attr =
	CORBA::ComponentIR::PublishesDef::_narrow(c[j]);
      CORBA::String_var n = attr->name();
      o << "_cookie_for_" << n << " = 0;" << endl;
    }
  }

  o << BL_CLOSE << endl;

  /*
   * Destructor
   */
    
  o << "MICO_CCM_impls::" << endl
    << flatName << "_impl::~" << flatName << "_impl ()" << endl;
  o << BL_OPEN;
  o << "delete _instance;" << endl;
  o << "delete _context;" << endl;
  o << BL_CLOSE << endl;

  /*
   * Delegators
   */

  /*
   * Component Attributes
   */

  o << "/*" << endl
    << " * Component Attributes" << endl
    << " */" << endl << endl;

  {
    c = comp->contents (CORBA::dk_Attribute, 1);

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::AttributeDef_var attr = CORBA::AttributeDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));
      CORBA::IDLType_var type_id = attr->type_def();
      CORBA::String_var n = attr->name();

      emit_type_for_result (type_id);
      o << endl;
      o << "MICO_CCM_impls::" << endl;
      o << flatName << "_impl::" << ID(n.in()) << " ()" << endl;
      o << BL_OPEN;
      if (_etype == MonolithicExecutor) {
	o << "return _instance->" << ID(n.in()) << " ();" << endl;
      }
      else {
	o << "CORBA::Object_var _eo = "
	  << "_instance->obtain_executor (\"" << className << "\");"
	  << endl;
	o << ccmAbsName << "_Executor_var _e =" << endl
	  << "  " << ccmAbsName << "_Executor::_narrow (_eo);" << endl;
	emit_type_for_variable (type_id, manual);
	o << " _res = _e->" << ID(n.in()) << " ();" << endl;
	o << "_instance->release_executor (_eo.in());" << endl;
	o << "return _res;" << endl;
      }
      o << BL_CLOSE << endl;
      
      if (attr->mode() == CORBA::ATTR_NORMAL) {
	o << "void " << endl;
	o << "MICO_CCM_impls::" << endl;
	o << flatName << "_impl::" << ID(n.in()) << " (";
	emit_type_for_param (type_id, CORBA::PARAM_IN);
	o << " _value)" << endl;
	o << BL_OPEN;
	if (_etype == MonolithicExecutor) {
	  o << "_instance->" << ID(n.in()) << " (_value);" << endl;
	}
	else {
	  o << "CORBA::Object_var _eo = "
	    << "_instance->obtain_executor (\"" << className << "\");"
	    << endl;
	  o << ccmAbsName << "_Executor_var _e =" << endl
	    << "  " << ccmAbsName << "_Executor::_narrow (_eo);" << endl;
	  o << "_e->" << ID(n.in()) << " (_value);" << endl;
	  o << "_instance->release_executor (_eo.in());" << endl;
	}
	o << BL_CLOSE << endl;
      }
    }
  }

  /*
   * Supported Interfaces
   */

  o << "/*" << endl
    << " * Supported Interfaces" << endl
    << " */" << endl << endl;

  {
    CORBA::InterfaceDefSeq_var ifs = comp->supported_interfaces ();

    for (CORBA::ULong j=0; j<ifs->length(); j++) {
      CORBA::String_var sn = ifs[j]->absolute_name ();

      o << "// " << sn << endl << endl;

      c = ifs[j]->contents (CORBA::dk_Operation, 0);

      for (CORBA::ULong j2=0; j2<c->length(); j2++) {
	CORBA::OperationDef_var op = CORBA::OperationDef::_narrow(c[j2]);
	CORBA::IDLType_var type_id = op->result_def();
	CORBA::ParDescriptionSeq_var p = op->params();
	CORBA::TypeCode_var rtc = type_id->type ();
	CORBA::String_var n = op->name();

	emit_type_for_result (type_id);
	o << endl;
	o << "MICO_CCM_impls::" << endl;
	o << flatName << "_impl::" << ID(n.in()) << " (";
	
	for (CORBA::ULong k1=0; k1<p->length(); k1++) {
	  if (k1 != 0)
	    o << ", ";
	  emit_type_for_param (p[k1].type_def, p[k1].mode);
	  o << " " << ID(p[k1].name);
	}
	o << ")" << endl;
	
	o << BL_OPEN;
	
	if (_etype == MonolithicExecutor) {
	  if (rtc->kind() != CORBA::tk_void) {
	    o << "return ";
	  }
	  o << "_instance->";
	}
	else {
	  o << "CORBA::Object_var _eo = "
	    << "_instance->obtain_executor (\"" << className << "\");"
	    << endl;
	  o << ccmAbsName << "_Executor_var _e =" << endl
	    << "  " << ccmAbsName << "_Executor::_narrow (_eo);" << endl;
	  if (rtc->kind() != CORBA::tk_void) {
	    emit_type_for_variable (type_id, manual);
	    o << " _res = ";
	  }
	  o << "_e->";
	}
	
	o << ID(n.in()) << " (";
	for (CORBA::ULong k2=0; k2<p->length(); k2++) {
	  if (k2 != 0)
	    o << ", ";
	  o << ID(p[k2].name);
	}
	o << ");" << endl;

	if (_etype != MonolithicExecutor) {
	  o << "_instance->release_executor (_eo.in());" << endl;
	  if (rtc->kind() != CORBA::tk_void) {
	    o << "return _res;" << endl;
	  }
	}
	
	o << BL_CLOSE << endl;
      }

      c = ifs[j]->contents (CORBA::dk_Attribute, 0);

      for (CORBA::ULong j3=0; j3<c->length(); j3++) {
	CORBA::AttributeDef_var attr = CORBA::AttributeDef::_narrow(c[j3]);
	assert (!CORBA::is_nil(attr));
	CORBA::IDLType_var type_id = attr->type_def();
	CORBA::String_var n = attr->name();
	
	emit_type_for_result (type_id);
	o << endl;
	o << "MICO_CCM_impls::" << endl;
	o << flatName << "_impl::" << ID(n.in()) << " ()" << endl;
	o << BL_OPEN;

	if (_etype == MonolithicExecutor) {
	  o << "return _instance->" << ID(n.in()) << " ();" << endl;
	}
	else {
	  o << "CORBA::Object_var _eo = "
	    << "_instance->obtain_executor (\"" << className << "\");"
	    << endl;
	  o << ccmAbsName << "_Executor_var _e =" << endl
	    << "  " << ccmAbsName << "_Executor::_narrow (_eo);" << endl;
	  emit_type_for_variable (type_id, manual);
	  o << " _res = _e->" << ID(n.in()) << " ();" << endl;
	  o << "_instance->release_executor (_eo.in());" << endl;
	  o << "return _res;" << endl;
	}
	o << BL_CLOSE << endl;
	
	if (attr->mode() == CORBA::ATTR_NORMAL) {
	  o << "void " << endl;
	  o << "MICO_CCM_impls::" << endl;
	  o << flatName << "_impl::" << ID(n.in()) << " (";
	  emit_type_for_param (type_id, CORBA::PARAM_IN);
	  o << " _value)" << endl;
	  o << BL_OPEN;
	  if (_etype == MonolithicExecutor) {
	    o << "_instance->" << ID(n.in()) << " (_value);" << endl;
	  }
	  else {
	    o << "CORBA::Object_var _eo = "
	      << "_instance->obtain_executor (\"" << className << "\");"
	      << endl;
	    o << ccmAbsName << "_Executor_var _e =" << endl
	      << "  " << ccmAbsName << "_Executor::_narrow (_eo);" << endl;
	    o << "_e->" << ID(n.in()) << " (_value);" << endl;
	    o << "_instance->release_executor (_eo.in());" << endl;
	  }
	  o << BL_CLOSE << endl;
	}
      }
    }
  }

  /*
   * Equivalent IDL methods
   */

  o << "/*" << endl
    << " * Equivalent IDL methods" << endl
    << " */" << endl << endl;

  // Emit facets

  {
    c = comp->contents (CORBA::dk_Provides, 1);

    if (c->length()) {
      o << "// facets" << endl << endl;
    }
    else {
      o << "// no facets" << endl << endl;
    }

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::ProvidesDef_var attr =
	CORBA::ComponentIR::ProvidesDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));
      CORBA::InterfaceDef_var iface = attr->interface_type();
      CORBA::String_var n = attr->name();

      CORBA::String_var a = iface->absolute_name ();
      string ifaceAbsName = (const char *) ID(a);
      ifaceAbsName = ifaceAbsName.substr (2);

      string ccmIfaceAbsName = ifaceAbsName;

      if ((idx = ccmIfaceAbsName.rfind ("::")) != (CORBA::ULong) -1) {
	ccmIfaceAbsName.insert (idx+2, "CCM_");
      }
      else {
	ccmIfaceAbsName.insert (0, "CCM_");
      }

      emit_type_for_result (iface);
      o << endl;
      o << "MICO_CCM_impls::" << endl;
      o << flatName << "_impl::provide_" << n << " ()" << endl;
      o << BL_OPEN;
      o << "if (CORBA::is_nil (_instance_for_facet_" << n << ")) "
	<< BL_OPEN;
      if (_etype == MonolithicExecutor) {
	o << "_instance_for_facet_" << n << " = "
	  << "_instance->get_" << n << " ();" << endl;
      }
      else {
	o << "CORBA::Object_var _eo = "
	  << "_instance->obtain_executor (\"" << n << "\");"
	  << endl;
	o << "_instance_for_facet_" << n << " = "
	  << ccmIfaceAbsName << "::_narrow (_eo);" << endl;
      }
      o << BL_CLOSE;
      o << "if (_glue_for_facet_" << n << ".in() == 0) "
	<< BL_OPEN;
      o << "_glue_for_facet_" << n << " =" << endl
	<< "  new " << flatName << "_" << n << "_impl (this);" << endl;
      o << "CORBA::Object_var _facet_ref =" << endl
	<< "  _container->activate_facet (this, \"" << n << "\"," << endl
	<< "                              _instance_for_facet_" << n << ","
	<< endl
	<< "                              _glue_for_facet_" << n << ");"
	<< endl;
      o << "_reference_for_facet_" << n << " =" << endl
	<< "  " << ifaceAbsName << "::_narrow (_facet_ref);" << endl;
      o << BL_CLOSE;
      o << "return " << ifaceAbsName << "::_duplicate ("
	<< "_reference_for_facet_" << n << ");" << endl;
      o << BL_CLOSE << endl;
    }
  }

  // Emit receptacles

  {
    c = comp->contents (CORBA::dk_Uses, 1);

    if (c->length()) {
      o << "// receptacles" << endl << endl;
    }
    else {
      o << "// no receptacles" << endl << endl;
    }

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::UsesDef_var attr =
	CORBA::ComponentIR::UsesDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));

      CORBA::InterfaceDef_var type_id = attr->interface_type();
      CORBA::String_var ain = type_id->absolute_name ();
      CORBA::String_var n = attr->name();
      string recep_abs_name = ain.in();
      recep_abs_name = recep_abs_name.substr (2);

      if (!attr->is_multiple()) {

	// connect

	o << "void" << endl;
	o << "MICO_CCM_impls::" << endl;
	o << flatName << "_impl::connect_" << n << " (";
	emit_type_for_param (type_id, CORBA::PARAM_IN);
	o << " _value)" << endl;
	o << BL_OPEN;
	o << "_for_recep_" << n << " = " << recep_abs_name << "::_duplicate (_value);"
	  << endl;
	o << BL_CLOSE << endl;

	// disconnect
	
	emit_type_for_result (type_id);
	o << endl;
	o << "MICO_CCM_impls::" << endl;
	o << flatName << "_impl::disconnect_" << n << " ()" << endl;
	o << BL_OPEN;
	o << recep_abs_name << "_var _res = "
	  << recep_abs_name << "::_duplicate (_for_recep_" << n << ");" << endl;
	o << "_for_recep_" << n << " = " << recep_abs_name << "::_nil ();" << endl;
	o << "return _res._retn ();" << endl;
	o << BL_CLOSE << endl;

	// get_connection

	emit_type_for_result (type_id);
	o << endl;
	o << "MICO_CCM_impls::" << endl;
	o << flatName << "_impl::get_connection_" << n << " ()" << endl;
	o << BL_OPEN;
	o << "return " << recep_abs_name << "::_duplicate (_for_recep_" << n << ");"
	  << endl;
	o << BL_CLOSE << endl;
      }

      // multiplex receptacles

      else {
	// connect

	o << "Components::Cookie *" << endl;
	o << "MICO_CCM_impls::" << endl;
	o << flatName << "_impl::connect_" << n << " (";
	emit_type_for_param (type_id, CORBA::PARAM_IN);
	o << " _value)" << endl;
	o << BL_OPEN;
	o << "Components::Cookie_var _ck = new MICO::CCM::Cookie_impl;" << endl;
	o << "_ck->CookieValue().replace (sizeof (CORBA::ULong), sizeof (CORBA::ULong), (CORBA::Octet *) &_cookie_for_" << n << ");" << endl;
	o << "_cookie_for_" << n << "++;" << endl;
	o << "CORBA::ULong _idx = _for_recep_" << n << ".length ();" << endl;
	o << "_for_recep_" << n << ".length (_idx + 1);" << endl;
	o << "_for_recep_" << n << "[_idx].ck = _ck;" << endl;
	o << "_for_recep_" << n << "[_idx].objref = "
	  << recep_abs_name << "::_duplicate (_value);" << endl;
	o << "return _ck._retn();" << endl;
	o << BL_CLOSE << endl;

	// disconnect
	
	emit_type_for_result (type_id);
	o << endl
	  << "MICO_CCM_impls::" << endl;
	o << flatName << "_impl::disconnect_" << n << " (";
	o << "Components::Cookie * _value)" << endl;
	o << BL_OPEN;
	o << "CORBA::ULong _idx, _ck1, _ck2;" << endl;
	o << "if (!_value || _value->CookieValue().length() != sizeof (CORBA::ULong)) "
	  << BL_OPEN;
	o << "throw Components::InvalidConnection();" << endl;
	o << BL_CLOSE << endl;
	o << "_ck1 = * ((CORBA::ULong *) _value->CookieValue().get_buffer());"
	  << endl << endl;
	o << "for (_idx=0; _idx<_for_recep_" << n << ".length(); _idx++) "
	  << BL_OPEN;
	o << "_ck2 = * ((CORBA::ULong *) _for_recep_" << n << "[_idx].ck->CookieValue().get_buffer());" << endl;
	o << "if (_ck1 == _ck2) break;" << endl;
	o << BL_CLOSE << endl;
	o << "if (_idx == _for_recep_" << n << ".length()) " << BL_OPEN;
	o << "throw Components::InvalidConnection();" << endl;
	o << BL_CLOSE << endl;
	o << recep_abs_name << "_var _res = " << recep_abs_name
	  << "::_duplicate (_for_recep_" << n << "[_idx].objref);" << endl;
	o << endl;
	o << "for (;_idx<_for_recep_" << n << ".length()-1; _idx++) " << BL_OPEN;
	o << "_for_recep_" << n << "[_idx] = _for_recep_" << n << "[_idx+1];"
	  << endl;
	o << BL_CLOSE << endl;
	o << "_for_recep_" << n << ".length (_idx);" << endl;
	o << "return _res._retn();" << endl;
	o << BL_CLOSE << endl;

	// get_connections
	
	o << absName << "::" << n << "Connections *" << endl;
	o << "MICO_CCM_impls::" << endl;
	o << flatName << "_impl::get_connections_" << n << " ()" << endl;
	o << BL_OPEN;
	o << "return new " << absName << "::" << n
	  << "Connections (_for_recep_" << n << ");" << endl;
	o << BL_CLOSE << endl;
      }
    }
  }

  // Emit publishers

  {
    c = comp->contents (CORBA::dk_Publishes, 1);

    if (c->length()) {
      o << "// publishers" << endl << endl;
    }
    else {
      o << "// no publishers" << endl << endl;
    }

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::PublishesDef_var attr =
	CORBA::ComponentIR::PublishesDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));
      CORBA::ComponentIR::EventDef_var event = attr->event();
      CORBA::String_var event_name = event->absolute_name();
      CORBA::String_var n = attr->name ();
      
      // subscribe

      o << "Components::Cookie *" << endl;
      o << "MICO_CCM_impls::" << endl;
      o << flatName << "_impl::subscribe_" << n << " ("
	<< event_name << "Consumer_ptr consumer)" << endl;
      o << BL_OPEN;
      o << "Components::Cookie_var _ck = new MICO::CCM::Cookie_impl;" << endl;
      o << "_ck->CookieValue().replace (sizeof (CORBA::ULong), sizeof (CORBA::ULong), (CORBA::Octet *) &_cookie_for_" << n << ");" << endl;
      o << "_cookie_for_" << n << "++;" << endl;
      o << "CORBA::ULong _idx = _for_publisher_" << n << ".length ();" << endl;
      o << "_for_publisher_" << n << ".length (_idx + 1);" << endl;
      o << "_for_publisher_" << n << "[_idx] = new MICO::CCM::SubscriberDescription_impl;" << endl;
      o << "_for_publisher_" << n << "[_idx]->ck (_ck);" << endl;
      o << "_for_publisher_" << n << "[_idx]->consumer (consumer);" << endl;
      o << "return _ck._retn();" << endl;
      o << BL_CLOSE << endl;

      // unsubscribe
	
      o << event_name << "Consumer_ptr" << endl;
      o << "MICO_CCM_impls::" << endl;
      o << flatName << "_impl::unsubscribe_" << n << " (";
      o << "Components::Cookie * _value)" << endl;
      o << BL_OPEN;
      o << "CORBA::ULong _idx, _ck1, _ck2;" << endl;
      o << "if (!_value || _value->CookieValue().length() != sizeof (CORBA::ULong)) "
	<< BL_OPEN;
      o << "throw Components::InvalidConnection();" << endl;
      o << BL_CLOSE << endl;
      o << "_ck1 = * ((CORBA::ULong *) _value->CookieValue().get_buffer());"
	<< endl << endl;
      o << "for (_idx=0; _idx<_for_publisher_" << n << ".length(); _idx++) "
	<< BL_OPEN;
      o << "_ck2 = * ((CORBA::ULong *) _for_publisher_" << n << "[_idx]->ck()->CookieValue().get_buffer());" << endl;
      o << "if (_ck1 == _ck2) break;" << endl;
      o << BL_CLOSE << endl;
      o << "if (_idx == _for_publisher_" << n << ".length()) " << BL_OPEN;
      o << "throw Components::InvalidConnection();" << endl;
      o << BL_CLOSE << endl;
      o << event_name << "Consumer_var _res = " << event_name << "Consumer"
	<< "::_narrow (_for_publisher_" << n << "[_idx]->consumer());" << endl;
      o << endl;
      o << "for (;_idx<_for_publisher_" << n << ".length()-1; _idx++) " << BL_OPEN;
      o << "_for_publisher_" << n << "[_idx] = _for_publisher_" << n << "[_idx+1];"
	<< endl;
      o << BL_CLOSE << endl;
      o << "_for_publisher_" << n << ".length (_idx);" << endl;
      o << "return _res._retn();" << endl;
      o << BL_CLOSE << endl;
    }
  }

  // Emit emitters

  {
    c = comp->contents (CORBA::dk_Emits, 1);

    if (c->length()) {
      o << "// emitters" << endl << endl;
    }
    else {
      o << "// no emitters" << endl << endl;
    }

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::EmitsDef_var attr =
	CORBA::ComponentIR::EmitsDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));

      CORBA::ComponentIR::EventDef_var event = attr->event();
      CORBA::String_var event_name = event->absolute_name();
      CORBA::String_var n = attr->name();

      // connect

      o << "void" << endl;
      o << "MICO_CCM_impls::" << endl;
      o << flatName << "_impl::connect_" << n << " (";
      o << event_name << "Consumer_ptr consumer)" << endl;
      o << BL_OPEN;
      o << "_for_emitter_" << n << " = "
	<< event_name << "Consumer::_duplicate (consumer);"
	<< endl;
      o << BL_CLOSE << endl;

      // disconnect
	
      o << event_name << "Consumer_ptr" << endl;
      o << "MICO_CCM_impls::" << endl;
      o << flatName << "_impl::disconnect_" << n << " ()" << endl;
      o << BL_OPEN;
      o << "if (CORBA::is_nil (_for_emitter_" << n << ")) " << BL_OPEN;
      o << "throw Components::NoConnection();" << endl;
      o << BL_CLOSE;
      o << event_name << "Consumer_var _res = "
	<< event_name << "Consumer::_duplicate (_for_emitter_" << n << ");" << endl;
      o << "_for_emitter_" << n << " = " << event_name << "Consumer::_nil ();" << endl;
      o << "return _res._retn ();" << endl;
      o << BL_CLOSE << endl;
    }
  }

  // Emit consumers

  {
    c = comp->contents (CORBA::dk_Consumes, 1);

    if (c->length()) {
      o << "// consumers" << endl << endl;
    }
    else {
      o << "// no consumers" << endl << endl;
    }

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::ConsumesDef_var attr =
	CORBA::ComponentIR::ConsumesDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));

      CORBA::ComponentIR::EventDef_var event = attr->event();
      CORBA::String_var event_name = event->absolute_name();
      CORBA::String_var n = attr->name();

      // get_consumer

      o << event_name << "Consumer_ptr" << endl;
      o << "MICO_CCM_impls::" << endl;
      o << flatName << "_impl::get_consumer_" << n << " ()" << endl;
      o << BL_OPEN;
      o << "if (CORBA::is_nil (_for_consumer_" << n << ")) " << BL_OPEN;
      o << "PortableServer::ServantBase_var _consumer_servant =" << endl
	<< "  new " << flatName << "_" << n << "_impl (this);" << endl;
      o << "CORBA::Object_var _ref =" << endl
	<< "  _container->activate_facet (this, \"" << n << "\"," << endl
	<< "                              CORBA::LocalObject::_nil()," << endl
	<< "                              _consumer_servant);" << endl;
      o << "_for_consumer_" << n << " =" << endl
	<< "  " << event_name << "Consumer::_narrow (_ref);" << endl;
      o << BL_CLOSE << endl;
      o << "return "
	<< event_name << "Consumer::_duplicate (_for_consumer_" << n << ");"
	<< endl;
      o << BL_CLOSE << endl;
    }
  }

  /*
   * CCMObject interfaces
   */

  o << "/*" << endl
    << " * Inherited Navigation interface" << endl
    << " */" << endl << endl;

  // provide_facet

  {
    o << "CORBA::Object_ptr" << endl;
    o << "MICO_CCM_impls::" << endl;
    o << flatName << "_impl::provide_facet (const char * _name)" << endl;
    o << BL_OPEN;
    
    c = comp->contents (CORBA::dk_Provides, 0);
    
    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::ProvidesDef_var attr =
	CORBA::ComponentIR::ProvidesDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));
      CORBA::String_var n = attr->name();

      o << "if (strcmp (_name, \"" << n << "\") == 0) " << BL_OPEN;
      o << "return provide_" << n << " ();" << endl;
      o << BL_CLOSE;
    }

    o << "throw Components::InvalidName();" << endl;
    o << "return CORBA::Object::_nil ();" << endl;
    o << BL_CLOSE << endl;
  }

  // get_all_facets

  {
    o << "Components::FacetDescriptions *" << endl;
    o << "MICO_CCM_impls::" << endl;
    o << flatName << "_impl::get_all_facets ()" << endl;
    o << BL_OPEN;
    
    c = comp->contents (CORBA::dk_Provides, 0);

    o << "Components::NameList _allnames;" << endl;
    o << "_allnames.length (" << c->length() << ");" << endl;

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::String_var n = c[j]->name ();
      o << "_allnames[" << j << "] = CORBA::string_dup (\"" << n << "\");"
	<< endl;
    }

    o << "return get_named_facets (_allnames);" << endl;
    o << BL_CLOSE << endl;
  }

  // get_named_facets

  {
    o << "Components::FacetDescriptions *" << endl;
    o << "MICO_CCM_impls::" << endl;
    o << flatName << "_impl::get_named_facets (const Components::NameList & _names)" << endl;
    o << BL_OPEN;
    o << "Components::FacetDescriptions_var _res = new Components::FacetDescriptions;" << endl;
    
    c = comp->contents (CORBA::dk_Provides, 0);

    o << "_res->length (_names.length());" << endl << endl;
    o << "for (CORBA::ULong _idx=0; _idx<_names.length(); _idx++) " << BL_OPEN;

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::ProvidesDef_var attr =
	CORBA::ComponentIR::ProvidesDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));
      CORBA::InterfaceDef_var facettype = attr->interface_type ();
      CORBA::String_var facetid = facettype->id ();
      CORBA::String_var n = attr->name();

      o << "if (strcmp (_names[_idx].in(), \"" << n << "\") == 0) " << BL_OPEN;
      o << "_res[_idx] = new MICO::CCM::FacetDescription_impl;" << endl;
      o << "_res[_idx]->name ((const char *) \"" << n << "\");" << endl;
      o << "_res[_idx]->type_id ((const char *) \"" << facetid << "\");"
	<< endl;
      o << "_res[_idx]->facet_ref (provide_facet (\"" << n << "\"));" << endl;
      o << "continue;" << endl;
      o << BL_CLOSE << endl;
    }

    o << "throw Components::InvalidName();" << endl;
    o << BL_CLOSE << endl;
    
    o << "return _res._retn();" << endl;
    o << BL_CLOSE << endl;
  }

  // same_component (TODO)

  {
    o << "CORBA::Boolean" << endl;
    o << "MICO_CCM_impls::" << endl;
    o << flatName << "_impl::same_component (CORBA::Object_ptr _ref)" << endl;
    o << BL_OPEN;
    o << "return 0;" << endl;
    o << BL_CLOSE << endl;
  }

  o << "/*" << endl
    << " * Inherited Receptacles interface" << endl
    << " */" << endl << endl;

  // connect

  {
    o << "Components::Cookie *" << endl;
    o << "MICO_CCM_impls::" << endl;
    o << flatName << "_impl::connect (const char * _name, CORBA::Object_ptr _ref)" << endl;
    o << BL_OPEN;

    c = comp->contents (CORBA::dk_Uses, 0);

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::UsesDef_var attr =
	CORBA::ComponentIR::UsesDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));

      CORBA::InterfaceDef_var type_id = attr->interface_type();
      CORBA::String_var ain = type_id->absolute_name ();
      CORBA::String_var n = attr->name();

      o << "if (strcmp (_name, \"" << n << "\") == 0) " << BL_OPEN;
      o << ain << "_var _tref = " << ain << "::_narrow (_ref);" << endl;
      o << "if (CORBA::is_nil (_tref) && !CORBA::is_nil (_ref)) " << BL_OPEN;
      o << "throw Components::InvalidConnection();" << endl;
      o << BL_CLOSE;
      if (!attr->is_multiple()) {
	o << "connect_" << n << " (_tref);" << endl;
	o << "return 0;" << endl;
      }
      else {
	o << "return connect_" << n << " (_tref);" << endl;
      }
      o << BL_CLOSE << endl;
    }

    o << "throw Components::InvalidName();" << endl;
    o << "return new MICO::CCM::Cookie_impl;" << endl;
    o << BL_CLOSE << endl;
  }

  // disconnect

  {
    o << "void" << endl;
    o << "MICO_CCM_impls::" << endl;
    o << flatName << "_impl::disconnect (const char * _name, Components::Cookie * _ck)" << endl;
    o << BL_OPEN;

    c = comp->contents (CORBA::dk_Uses, 0);

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::UsesDef_var attr =
	CORBA::ComponentIR::UsesDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));
      
      CORBA::InterfaceDef_var type_id = attr->interface_type();
      CORBA::String_var ain = type_id->absolute_name ();
      CORBA::String_var n = attr->name();
      
      o << "if (strcmp (_name, \"" << n << "\") == 0) " << BL_OPEN;
      o << ain << "_var _dummy;" << endl;
      
      if (!attr->is_multiple()) {
	o << "_dummy = disconnect_" << n << " ();" << endl;
      }
      else {
	o << "_dummy = disconnect_" << n << " (_ck);" << endl;
      }
      
      o << "return;" << endl;
      o << BL_CLOSE << endl;
    }
    
    o << "throw Components::InvalidName();" << endl;
    o << BL_CLOSE << endl;
  }

  // get_connections

  {
    o << "Components::ConnectionDescriptions *" << endl;
    o << "MICO_CCM_impls::" << endl;
    o << flatName << "_impl::get_connections (const char * _name)" << endl;
    o << BL_OPEN;
    o << "Components::NameList _thename (1);" << endl;
    o << "_thename[0] = _name;" << endl;
    o << "Components::ReceptacleDescriptions_var _rdv ="
      << " get_named_receptacles (_thename);" << endl;
    o << "return new Components::ConnectionDescriptions (_rdv[(CORBA::ULong)0]->connections());" << endl;
    o << BL_CLOSE;
  }
    
  // get_all_receptacles

  {
    o << "Components::ReceptacleDescriptions *" << endl;
    o << "MICO_CCM_impls::" << endl;
    o << flatName << "_impl::get_all_receptacles ()" << endl;
    o << BL_OPEN;
    
    c = comp->contents (CORBA::dk_Uses, 0);

    o << "Components::NameList _allnames;" << endl;
    o << "_allnames.length (" << c->length() << ");" << endl;

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::String_var n = c[j]->name ();
      o << "_allnames[" << j << "] = CORBA::string_dup (\"" << n << "\");"
	<< endl;
    }

    o << "return get_named_receptacles (_allnames);" << endl;
    o << BL_CLOSE << endl;
  }

  // get_named_receptacles

  {
    o << "Components::ReceptacleDescriptions *" << endl;
    o << "MICO_CCM_impls::" << endl;
    o << flatName << "_impl::get_named_receptacles (const Components::NameList & _names)" << endl;
    o << BL_OPEN;
    o << "Components::ReceptacleDescriptions_var _res = new Components::ReceptacleDescriptions;" << endl;
    
    c = comp->contents (CORBA::dk_Uses, 0);

    o << "_res->length (_names.length());" << endl << endl;
    o << "for (CORBA::ULong _idx=0; _idx<_names.length(); _idx++) " << BL_OPEN;

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::UsesDef_var attr =
	CORBA::ComponentIR::UsesDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));

      CORBA::InterfaceDef_var type_id = attr->interface_type();
      CORBA::String_var ain = type_id->absolute_name ();
      CORBA::String_var theid = type_id->id ();
      CORBA::String_var n = attr->name();

      o << "if (strcmp (_names[_idx].in(), \"" << n << "\") == 0) " << BL_OPEN;
      o << "_res[_idx] = new MICO::CCM::ReceptacleDescription_impl;" << endl;
      o << "_res[_idx]->name ((const char *) \"" << n << "\");" << endl;
      o << "_res[_idx]->type_id ((const char *) \"" << theid << "\");"
	<< endl;
      if (!attr->is_multiple()) {
	o << "_res[_idx]->is_multiplex (0);" << endl;
	o << "if (!CORBA::is_nil (_for_recep_" << n << ")) " << BL_OPEN;
	o << "_res[_idx]->connections().length(1);" << endl;
	o << "_res[_idx]->connections()[0] =" << endl
	  << "  new MICO::CCM::ConnectionDescription_impl;" << endl;
	o << "_res[_idx]->connections()[0]->ck (0);" << endl;
	o << "_res[_idx]->connections()[0]->objref (_for_recep_" << n << ");"
	  << endl;
	o << BL_CLOSE;
      }
      else {
	o << "_res[_idx]->is_multiplex (1);" << endl;
	o << "_res[_idx]->connections().length (_for_recep_" << n << ".length());" << endl;
	o << "for (CORBA::ULong _idx2=0; _idx2<_for_recep_" << n << ".length(); _idx2++) " << BL_OPEN;
	o << "_res[_idx]->connections()[_idx2] =" << endl
	  << "  new MICO::CCM::ConnectionDescription_impl;" << endl;

	o << "_res[_idx]->connections()[_idx2]->ck (_for_recep_" << n << "[_idx2].ck);" << endl;
	o << "_res[_idx]->connections()[_idx2]->objref (_for_recep_" << n << "[_idx2].objref);" << endl;
	o << BL_CLOSE;
      }
      o << "continue;" << endl;
      o << BL_CLOSE << endl;
    }

    o << "throw Components::InvalidName();" << endl;
    o << BL_CLOSE << endl;

    o << "return _res._retn();" << endl;
    o << BL_CLOSE << endl;
  }

  o << "/*" << endl
    << " * Inherited Events interface" << endl
    << " */" << endl << endl;

  // get_consumer

  {
    o << "Components::EventConsumerBase_ptr" << endl;
    o << "MICO_CCM_impls::" << endl;
    o << flatName << "_impl::get_consumer (const char * _name)" << endl;
    o << BL_OPEN;
    
    c = comp->contents (CORBA::dk_Consumes, 0);
    
    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::ConsumesDef_var attr =
	CORBA::ComponentIR::ConsumesDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));
      CORBA::String_var n = attr->name();

      o << "if (strcmp (_name, \"" << n << "\") == 0) " << BL_OPEN;
      o << "return get_consumer_" << n << " ();" << endl;
      o << BL_CLOSE;
    }

    o << "throw Components::InvalidName();" << endl;
    o << "return Components::EventConsumerBase::_nil ();" << endl;
    o << BL_CLOSE << endl;
  }

  // get_all_consumers

  {
    o << "Components::ConsumerDescriptions *" << endl;
    o << "MICO_CCM_impls::" << endl;
    o << flatName << "_impl::get_all_consumers ()" << endl;
    o << BL_OPEN;
    
    c = comp->contents (CORBA::dk_Consumes, 0);

    o << "Components::NameList _allnames;" << endl;
    o << "_allnames.length (" << c->length() << ");" << endl;

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::String_var n = c[j]->name ();
      o << "_allnames[" << j << "] = CORBA::string_dup (\"" << n << "\");"
	<< endl;
    }

    o << "return get_named_consumers (_allnames);" << endl;
    o << BL_CLOSE << endl;
  }

  // get_named_consumers

  {
    o << "Components::ConsumerDescriptions *" << endl;
    o << "MICO_CCM_impls::" << endl;
    o << flatName << "_impl::get_named_consumers (const Components::NameList & _names)" << endl;
    o << BL_OPEN;
    o << "Components::ConsumerDescriptions_var _res = new Components::ConsumerDescriptions;" << endl;
    
    c = comp->contents (CORBA::dk_Consumes, 0);

    o << "_res->length (_names.length());" << endl << endl;
    o << "for (CORBA::ULong _idx=0; _idx<_names.length(); _idx++) " << BL_OPEN;

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::ConsumesDef_var attr =
	CORBA::ComponentIR::ConsumesDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));

      CORBA::ComponentIR::EventDef_var event = attr->event();
      CORBA::String_var event_name = event->absolute_name();
      CORBA::String_var event_id = event->id();
      CORBA::String_var n = attr->name();

      o << "if (strcmp (_names[_idx].in(), \"" << n << "\") == 0) " << BL_OPEN;
      o << "_res[_idx] = new MICO::CCM::ConsumerDescription_impl;" << endl;
      o << "_res[_idx]->name ((const char *) \"" << n << "\");" << endl;
      o << "_res[_idx]->type_id ((const char *) \"" << event_id << "\");"
	<< endl;
      o << "Components::EventConsumerBase_var _tmp =" << endl
	<< "  get_consumer (\"" << n << "\");" << endl;
      o << "_res[_idx]->consumer (_tmp);" << endl;
      o << "continue;" << endl;
      o << BL_CLOSE << endl;
    }

    o << "throw Components::InvalidName();" << endl;
    o << BL_CLOSE << endl;
    
    o << "return _res._retn();" << endl;
    o << BL_CLOSE << endl;
  }

  // subscribe

  {
    o << "Components::Cookie *" << endl;
    o << "MICO_CCM_impls::" << endl;
    o << flatName << "_impl::subscribe (const char * _name, Components::EventConsumerBase_ptr _ref)" << endl;
    o << BL_OPEN;

    c = comp->contents (CORBA::dk_Publishes, 0);

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::PublishesDef_var attr =
	CORBA::ComponentIR::PublishesDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));

      CORBA::ComponentIR::EventDef_var event = attr->event();
      CORBA::String_var event_name = event->absolute_name();
      CORBA::String_var n = attr->name();

      o << "if (strcmp (_name, \"" << n << "\") == 0) " << BL_OPEN;
      o << event_name << "Consumer_var _tref =" << endl
	<< "  " << event_name << "Consumer::_narrow (_ref);" << endl;
      o << "if (CORBA::is_nil (_tref) && !CORBA::is_nil (_ref)) " << BL_OPEN;
      o << "throw Components::InvalidConnection();" << endl;
      o << BL_CLOSE;
      o << "return subscribe_" << n << " (_tref);" << endl;
      o << BL_CLOSE << endl;
    }

    o << "throw Components::InvalidName();" << endl;
    o << "return new MICO::CCM::Cookie_impl;" << endl;
    o << BL_CLOSE << endl;
  }

  // unsubscribe

  {
    o << "void" << endl;
    o << "MICO_CCM_impls::" << endl;
    o << flatName << "_impl::unsubscribe (const char * _name, Components::Cookie * _ck)" << endl;
    o << BL_OPEN;

    c = comp->contents (CORBA::dk_Publishes, 0);

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::PublishesDef_var attr =
	CORBA::ComponentIR::PublishesDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));
      
      CORBA::ComponentIR::EventDef_var event = attr->event();
      CORBA::String_var event_name = event->absolute_name();
      CORBA::String_var n = attr->name();

      o << "if (strcmp (_name, \"" << n << "\") == 0) " << BL_OPEN;
      o << "Components::EventConsumerBase_var _dummy;" << endl;
      o << "_dummy = unsubscribe_" << n << " (_ck);" << endl;
      o << "return;" << endl;
      o << BL_CLOSE << endl;
    }
    
    o << "throw Components::InvalidName();" << endl;
    o << BL_CLOSE << endl;
  }

  // get_all_publishers

  {
    o << "Components::PublisherDescriptions *" << endl;
    o << "MICO_CCM_impls::" << endl;
    o << flatName << "_impl::get_all_publishers ()" << endl;
    o << BL_OPEN;
    
    c = comp->contents (CORBA::dk_Publishes, 0);

    o << "Components::NameList _allnames;" << endl;
    o << "_allnames.length (" << c->length() << ");" << endl;

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::String_var n = c[j]->name ();
      o << "_allnames[" << j << "] = CORBA::string_dup (\"" << n << "\");"
	<< endl;
    }

    o << "return get_named_publishers (_allnames);" << endl;
    o << BL_CLOSE << endl;
  }

  // get_named_publishers

  {
    o << "Components::PublisherDescriptions *" << endl;
    o << "MICO_CCM_impls::" << endl;
    o << flatName << "_impl::get_named_publishers (const Components::NameList & _names)" << endl;
    o << BL_OPEN;
    o << "Components::PublisherDescriptions_var _res = new Components::PublisherDescriptions;" << endl;
    
    c = comp->contents (CORBA::dk_Publishes, 0);

    o << "_res->length (_names.length());" << endl << endl;
    o << "for (CORBA::ULong _idx=0; _idx<_names.length(); _idx++) " << BL_OPEN;

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::PublishesDef_var attr =
	CORBA::ComponentIR::PublishesDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));

      CORBA::ComponentIR::EventDef_var event = attr->event();
      CORBA::String_var event_name = event->absolute_name();
      CORBA::String_var event_id = event->id();
      CORBA::String_var n = attr->name();

      o << "if (strcmp (_names[_idx].in(), \"" << n << "\") == 0) " << BL_OPEN;
      o << "_res[_idx] = new MICO::CCM::PublisherDescription_impl;" << endl;
      o << "_res[_idx]->name ((const char *) \"" << n << "\");" << endl;
      o << "_res[_idx]->type_id ((const char *) \"" << event_id << "\");"
	<< endl;
      o << "_res[_idx]->consumers (_for_publisher_" << n << ");" << endl;
      o << "continue;" << endl;
      o << BL_CLOSE << endl;
    }

    o << "throw Components::InvalidName();" << endl;
    o << BL_CLOSE << endl;

    o << "return _res._retn();" << endl;
    o << BL_CLOSE << endl;
  }

  // connect_consumer

  {
    o << "void" << endl;
    o << "MICO_CCM_impls::" << endl;
    o << flatName << "_impl::connect_consumer (const char * _name, Components::EventConsumerBase_ptr _ref)" << endl;
    o << BL_OPEN;

    c = comp->contents (CORBA::dk_Emits, 0);

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::EmitsDef_var attr =
	CORBA::ComponentIR::EmitsDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));

      CORBA::ComponentIR::EventDef_var event = attr->event();
      CORBA::String_var event_name = event->absolute_name();
      CORBA::String_var n = attr->name();

      o << "if (strcmp (_name, \"" << n << "\") == 0) " << BL_OPEN;
      o << event_name << "Consumer_var _tref =" << endl
	<< "  " << event_name << "Consumer::_narrow (_ref);" << endl;
      o << "if (CORBA::is_nil (_tref) && !CORBA::is_nil (_ref)) " << BL_OPEN;
      o << "throw Components::InvalidConnection();" << endl;
      o << BL_CLOSE;
      o << "connect_" << n << " (_tref);" << endl;
      o << "return;" << endl;
      o << BL_CLOSE << endl;
    }

    o << "throw Components::InvalidName();" << endl;
    o << BL_CLOSE << endl;
  }

  // disconnect_consumer

  {
    o << "Components::EventConsumerBase_ptr" << endl;
    o << "MICO_CCM_impls::" << endl;
    o << flatName << "_impl::disconnect_consumer (const char * _name)" << endl;
    o << BL_OPEN;

    c = comp->contents (CORBA::dk_Emits, 0);

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::String_var n = c[j]->name ();
      o << "if (strcmp (_name, \"" << n << "\") == 0) " << BL_OPEN;
      o << "return disconnect_" << n << " ();" << endl;
      o << BL_CLOSE << endl;
    }
    
    o << "throw Components::InvalidName();" << endl;
    o << BL_CLOSE << endl;
  }

  // get_all_emitters

  {
    o << "Components::EmitterDescriptions *" << endl;
    o << "MICO_CCM_impls::" << endl;
    o << flatName << "_impl::get_all_emitters ()" << endl;
    o << BL_OPEN;
    
    c = comp->contents (CORBA::dk_Emits, 0);

    o << "Components::NameList _allnames;" << endl;
    o << "_allnames.length (" << c->length() << ");" << endl;

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::String_var n = c[j]->name ();
      o << "_allnames[" << j << "] = CORBA::string_dup (\"" << n << "\");"
	<< endl;
    }

    o << "return get_named_emitters (_allnames);" << endl;
    o << BL_CLOSE << endl;
  }

  // get_named_emitters

  {
    o << "Components::EmitterDescriptions *" << endl;
    o << "MICO_CCM_impls::" << endl;
    o << flatName << "_impl::get_named_emitters (const Components::NameList & _names)" << endl;
    o << BL_OPEN;
    o << "Components::EmitterDescriptions_var _res = new Components::EmitterDescriptions;" << endl;
    
    c = comp->contents (CORBA::dk_Emits, 0);

    o << "_res->length (_names.length());" << endl << endl;
    o << "for (CORBA::ULong _idx=0; _idx<_names.length(); _idx++) " << BL_OPEN;

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::EmitsDef_var attr =
	CORBA::ComponentIR::EmitsDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));

      CORBA::ComponentIR::EventDef_var event = attr->event();
      CORBA::String_var event_name = event->absolute_name();
      CORBA::String_var event_id = event->id();
      CORBA::String_var n = attr->name();

      o << "if (strcmp (_names[_idx].in(), \"" << n << "\") == 0) " << BL_OPEN;
      o << "_res[_idx] = new MICO::CCM::EmitterDescription_impl;" << endl;
      o << "_res[_idx]->name ((const char *) \"" << n << "\");" << endl;
      o << "_res[_idx]->type_id ((const char *) \"" << event_id << "\");"
	<< endl;
      o << "_res[_idx]->consumer (_for_emitter_" << n << ");" << endl;
      o << "continue;" << endl;
      o << BL_CLOSE << endl;
    }

    o << "throw Components::InvalidName();" << endl;
    o << BL_CLOSE << endl;

    o << "return _res._retn();" << endl;
    o << BL_CLOSE << endl;
  }

  o << "/*" << endl
    << " * Inherited CCMObject interface" << endl
    << " */" << endl << endl;

  // get_component_def

  {
    o << "CORBA::Object_ptr" << endl;
    o << "MICO_CCM_impls::" << endl;
    o << flatName << "_impl::get_component_def ()" << endl;
    o << BL_OPEN;
    o << "return CORBA::Object::_nil ();" << endl;
    o << BL_CLOSE << endl;
  }

  // get_ccm_home

  {
    o << "Components::CCMHome_ptr" << endl;
    o << "MICO_CCM_impls::" << endl;
    o << flatName << "_impl::get_ccm_home ()" << endl;
    o << BL_OPEN;
    o << "return _container->get_reference_for_home ();" << endl;
    o << BL_CLOSE << endl;
  }

  // configuration_complete

  {
    o << "void" << endl;
    o << "MICO_CCM_impls::" << endl;
    o << flatName << "_impl::configuration_complete ()" << endl;
    o << BL_OPEN;
    o << "_container->configuration_complete (this);" << endl;
    o << "Components::SessionComponent_ptr _sc =" << endl
      << "  Components::SessionComponent::_narrow (_instance);" << endl;
    o << "if (!CORBA::is_nil (_sc)) " << BL_OPEN;
    o << "_sc->set_session_context (_context);" << endl;
    o << "_sc->ccm_activate ();" << endl;
    o << BL_CLOSE;
    o << BL_CLOSE << endl;
  }

  // get_all_ports

  {
    o << "Components::ComponentPortDescription *" << endl;
    o << "MICO_CCM_impls::" << endl;
    o << flatName << "_impl::get_all_ports ()" << endl;
    o << BL_OPEN;

    o << "Components::FacetDescriptions_var _fds = get_all_facets ();"
      << endl;
    o << "Components::ReceptacleDescriptions_var _rds = get_all_receptacles ();"
      << endl;
    o << "Components::ConsumerDescriptions_var _cds = get_all_consumers ();"
      << endl;
    o << "Components::EmitterDescriptions_var _eds = get_all_emitters ();"
      << endl;
    o << "Components::PublisherDescriptions_var _pds = get_all_publishers ();"
      << endl;
    o << endl;
    o << "Components::ComponentPortDescription_var _cpd =" << endl
      << "  new MICO::CCM::ComponentPortDescription_impl;" << endl;
    o << endl;
    o << "_cpd->facets (_fds.in());" << endl;
    o << "_cpd->receptacles (_rds.in());" << endl;
    o << "_cpd->consumers (_cds.in());" << endl;
    o << "_cpd->emitters (_eds.in());" << endl;
    o << "_cpd->publishers (_pds.in());" << endl;
    o << endl;
    o << "return _cpd._retn();" << endl;
    o << BL_CLOSE << endl;
  }

  // remove

  {
    o << "void" << endl;
    o << "MICO_CCM_impls::" << endl;
    o << flatName << "_impl::remove ()" << endl;
    o << BL_OPEN;
    if (_type == ServiceComponent) {
      o << "/*" << endl
	<< " * remove() has no effect on a Service Component" << endl
	<< " */" << endl;
    }
    else if (_type == SessionComponent) {
      o << "Components::CCMHome_var myhome = get_ccm_home ();" << endl;
      o << "Components::CCMObject_var myself = _container->get_reference_for_component (this);" << endl;
      o << "myhome->remove_component (myself);" << endl;
    }
    o << BL_CLOSE << endl;
  }

  o << "/*" << endl
    << " * Overload default implementation" << endl
    << " */" << endl
    << endl;

  {
    o << "CORBA::Object_ptr" << endl;
    o << "MICO_CCM_impls::" << endl;
    o << flatName << "_impl::_get_component ()" << endl;
    o << BL_OPEN;
    o << "return ";
    if (_type == ServiceComponent) {
      o << "_container->get_reference_for_component ();" << endl;
    }
    else if (_type == SessionComponent) {
      o << "_container->get_reference_for_component (this);" << endl;
    }
    o << BL_CLOSE << endl;
  }

  /*
   * Emit Glue code for local facets
   */

  {
    c = comp->contents (CORBA::dk_Provides, 1);

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::ProvidesDef_var attr =
	CORBA::ComponentIR::ProvidesDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));
      CORBA::InterfaceDef_var iface = attr->interface_type();
      CORBA::String_var n = attr->name();
      emitInterfaceGlueImpl (iface, comp, n);
    }
  }

  /*
   * Emit Glue code for event consumers
   */

  {
    c = comp->contents (CORBA::dk_Consumes, 1);

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::ConsumesDef_var attr =
	CORBA::ComponentIR::ConsumesDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));
      CORBA::ComponentIR::EventDef_var event = attr->event ();
      CORBA::String_var n = attr->name();
      emitConsumerGlueImpl (event, comp, n);
    }
  }
}

void
CCMCodeGenCpp::emitHomeGlueImpl (CORBA::ComponentIR::HomeDef_ptr home)
{
  CORBA::String_var n = home->name();
  CORBA::String_var a = home->absolute_name();
  CORBA::String_var home_id = home->id();
  string className = (const char *) ID(n);
  string absName = (const char *) ID(a);
  absName = absName.substr (2);
  CORBA::ULong idx;

  string flatName = absName;

  while ((idx = flatName.find ("::")) != (CORBA::ULong) -1) {
    flatName.replace (idx, 2, "_");
  }

  string ccmAbsName = absName;

  if ((idx = ccmAbsName.rfind ("::")) != (CORBA::ULong) -1) {
    ccmAbsName.insert (idx+2, "CCM_");
  }
  else {
    ccmAbsName.insert (0, "CCM_");
  }

  o << "/*" << endl
    << " * Glue class for home " << absName << endl
    << " */" << endl << endl;

  CORBA::ComponentIR::ComponentDef_var managed = home->managed_component ();
  CORBA::ValueDef_var key = home->primary_key ();
  CORBA::ContainedSeq_var c;

  CORBA::String_var managed_id = managed->id ();
  CORBA::String_var mn = managed->absolute_name ();
  string managedAbsName = (const char *) ID(mn.in());
  managedAbsName = managedAbsName.substr (2);
  string managedFlatName = managedAbsName;

  while ((idx = managedFlatName.find ("::")) != (CORBA::ULong) -1) {
    managedFlatName.replace (idx, 2, "_");
  }

  string ccmManagedAbsName = managedAbsName;

  if ((idx = ccmManagedAbsName.rfind ("::")) != (CORBA::ULong) -1) {
    ccmManagedAbsName.insert (idx+2, "CCM_");
  }
  else {
    ccmManagedAbsName.insert (0, "CCM_");
  }

  o << "MICO_CCM_impls::" << endl
    << flatName << "_impl::" << flatName << "_impl "
    << "(" << ccmAbsName << " * i, ";
  if (_type == ServiceComponent) {
    o << "MICO::CCM::ServiceContainer * c";
  }
  else if (_type == SessionComponent) {
    o << "MICO::CCM::SessionContainer * c";
  }
  o << ")" << endl;
  o << indent << ": _instance (i), _container (c)";
  o << exdent << endl;
  o << BL_OPEN;
  o << BL_CLOSE << endl;
    
  o << "MICO_CCM_impls::" << endl
    << flatName << "_impl::~" << flatName << "_impl ()" << endl;
  o << BL_OPEN;
  o << "delete _instance;" << endl;
  o << BL_CLOSE << endl;

  o << "/*" << endl
    << " * Implicit operations" << endl
    << " */" << endl << endl;

  if (CORBA::is_nil (key)) {
    emit_type_for_result (managed);
    o << endl;
    o << "MICO_CCM_impls::" << endl;
    o << flatName << "_impl::create ()" << endl;
    o << BL_OPEN;
    if (_type == ServiceComponent) {
      o << "Components::CCMObject_var _co =" << endl
	<< "  _container->get_reference_for_component ();" << endl;
      o << "return " << managedAbsName << "::_narrow (_co);" << endl;
    }
    else if (_type == SessionComponent) {
      o << "Components::EnterpriseComponent_var _ci = _instance->create ();"
	<< endl;
      if (_etype == MonolithicExecutor) {
	o << ccmManagedAbsName << "_ptr _ti =" << endl
	  << "  " << ccmManagedAbsName << "::_narrow (_ci);" << endl;
      }
      else {
	o << "Components::ExecutorLocator_ptr _ti =" << endl
	  << "  " << "Components::ExecutorLocator::_narrow (_ci);" << endl;
      }
      o << "PortableServer::ServantBase_var _cs =" << endl
	<< "  new " << managedFlatName << "_impl (_ti, _container);"
	<< endl;
      o << "Components::CCMObject_var _co = " << endl
	<< "  _container->activate_component (_ti, _cs.in());" << endl;
      o << "return " << managedAbsName << "::_narrow (_co);" << endl;
    }
    o << BL_CLOSE << endl;
  }
  else {
    assert (0);

#if 0
    emit_type_for_result (managed);
    o << " create (";
    emit_type_for_param (key, CORBA::PARAM_IN);
    o << ");" << endl;

    emit_type_for_result (managed);
    o << " find_by_primary_key (";
    emit_type_for_param (key, CORBA::PARAM_IN);
    o << ");" << endl;
    
    o << "void remove (";
    emit_type_for_param (key, CORBA::PARAM_IN);
    o << ");" << endl;
#endif
  }

  o << "/*" << endl
    << " * Explicit operations" << endl
    << " */" << endl << endl;

  // Emit factories

  {
    c = home->contents (CORBA::dk_Factory, 1);

    if (c->length()) {
      o << "// factories" << endl << endl;
    }
    else {
      o << "// no factories" << endl << endl;
    }

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::OperationDef_var op = CORBA::OperationDef::_narrow(c[j]);
      CORBA::String_var n = op->name();

      assert (_type == SessionComponent);

      emit_type_for_result (managed);
      o << endl;
      o << "MICO_CCM_impls::" << endl;
      o << flatName << "_impl::" << ID(n.in()) << " (";
      CORBA::ParDescriptionSeq_var p = op->params();
      for (CORBA::ULong k1=0; k1<p->length(); k1++) {
	if (k1 != 0)
	  o << ", ";
	emit_type_for_param (p[k1].type_def, p[k1].mode );
	o << " " << ID(p[k1].name);
      }
      o << ")" << endl;
      o << BL_OPEN;
      o << "Components::EnterpriseComponent_ptr _ci =" << endl
	<< "  _instance->" << ID(n.in()) << " (";
      for (CORBA::ULong k2=0; k2<p->length(); k2++) {
	if (k2 != 0)
	  o << ", ";
	o << ID(p[k2].name);
      }
      o << ");" << endl;
      
      if (_etype == MonolithicExecutor) {
	o << ccmManagedAbsName << "_var _ti =" << endl
	  << "  " << ccmManagedAbsName << "::_narrow (_ci);" << endl;
      }
      else {
	o << "Components::ExecutorLocator_var _ti =" << endl
	  << "  " << "Components::ExecutorLocator::_narrow (_ci);" << endl;
      }
      o << "PortableServer::ServantBase_var _cs = new "
	<< managedFlatName << "_impl (_ti, _container);" << endl;
      o << "Components::CCMObject_var _co = " << endl
	<< "  _container->activate_component (_ti, _cs.in());" << endl;
      o << "return " << managedAbsName << "::_narrow (_co);" << endl;
      o << BL_CLOSE << endl;
    }
  }

  // Emit finder

  {
    c = home->contents (CORBA::dk_Finder, 1);

    if (c->length()) {
      o << "// finders" << endl << endl;
    }
    else {
      o << "// no finders" << endl << endl;
    }

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::OperationDef_var op = CORBA::OperationDef::_narrow(c[j]);
      CORBA::String_var n = op->name();

      assert (_type == SessionComponent);

      emit_type_for_result (managed);
      o << endl;
      o << "MICO_CCM_impls::" << endl;
      o << flatName << "_impl::" << ID(n.in()) << " (";
      CORBA::ParDescriptionSeq_var p = op->params();
      for (CORBA::ULong k1=0; k1<p->length(); k1++) {
	if (k1 != 0)
	  o << ", ";
	emit_type_for_param (p[k1].type_def, p[k1].mode );
	o << " " << ID(p[k1].name);
      }
      o << ")" << endl;
      o << BL_OPEN;
      o << "Components::EnterpriseComponent_var _ci =" << endl
	<< "  _instance->" << ID(n.in()) << " (";
      for (CORBA::ULong k2=0; k2<p->length(); k2++) {
	if (k2 != 0)
	  o << ", ";
	o << ID(p[k2].name);
      }
      o << ");" << endl;
      if (_etype == MonolithicExecutor) {
	o << ccmManagedAbsName << "_var _ti =" << endl
	  << "  " << ccmManagedAbsName << "::_narrow (_ci);" << endl;
      }
      else {
	o << "Components::ExecutorLocator_var _ti =" << endl
	  << "  " << "Components::ExecutorLocator::_narrow (_ci);" << endl;
      }
      o << "CORBA::Object_var _co = _container->get_reference_for_instance (_ti.in());" << endl;
      o << "if (CORBA::is_nil (_co)) " << BL_OPEN;
      o << "PortableServer::ServantBase_var _cs = new "
	<< managedFlatName << "_impl (_ti, _container);" << endl;
      o << " _co =" << endl
	<< "  _container->activate_component (_ti, _cs.in());" << endl;
      o << BL_CLOSE;
      o << "return " << managedAbsName << "::_narrow (_co);" << endl;
      o << BL_CLOSE << endl;
    }
  }

  {
    c = home->contents (CORBA::dk_Attribute, 1);

    if (c->length()) {
      o << "// attributes" << endl << endl;
    }
    else {
      o << "// no attributes" << endl << endl;
    }

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::AttributeDef_var attr = CORBA::AttributeDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));
      CORBA::IDLType_var type_id = attr->type_def();
      CORBA::String_var n = attr->name();

      emit_type_for_result (type_id);
      o << endl;
      o << "MICO_CCM_impls::" << endl;
      o << flatName << "_impl::" << ID(n.in()) << " ()" << endl;
      o << BL_OPEN;
      o << "return _instance->" << ID(n.in()) << " ();" << endl;
      o << BL_CLOSE << endl;

      if( attr->mode() == CORBA::ATTR_NORMAL ) {
	o << "void " << endl;
	o << "MICO_CCM_impls::" << endl;
	o << flatName << "_impl::" << ID(n.in()) << " (";
	emit_type_for_param (type_id, CORBA::PARAM_IN);
	o << " _value)" << endl;
	o << BL_OPEN;
	o << "_instance->" << ID(n.in()) << " (_value);" << endl;
	o << BL_CLOSE << endl;
      }
    }
  }

  {
    c = home->contents (CORBA::dk_Operation, 1);

    if (c->length()) {
      o << "// operations" << endl << endl;
    }
    else {
      o << "// no operations" << endl << endl;
    }

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::OperationDef_var op = CORBA::OperationDef::_narrow(c[j]);
      CORBA::IDLType_var type_id = op->result_def();
      CORBA::ParDescriptionSeq_var p = op->params();
      CORBA::TypeCode_var rtc = type_id->type ();
      CORBA::String_var n = op->name();
    
      emit_type_for_result (type_id);
      o << endl;
      o << "MICO_CCM_impls::" << endl;
      o << flatName << "_impl::" << ID(n.in()) << " (";
    
      for (CORBA::ULong k1=0; k1<p->length(); k1++) {
	if (k1 != 0)
	  o << ", ";
	emit_type_for_param (p[k1].type_def, p[k1].mode);
	o << " " << ID(p[k1].name);
      }
      o << ")" << endl;
    
      o << BL_OPEN;
    
      if (rtc->kind() != CORBA::tk_void) {
	o << "return ";
      }
    
      o << "_instance->" << ID(n.in()) << " (";
      for (CORBA::ULong k2=0; k2<p->length(); k2++) {
	if (k2 != 0)
	  o << ", ";
	o << ID(p[k2].name);
      }
      o << ");" << endl;
    
      o << BL_CLOSE << endl;
    }
  }
  
  /*
   * CCMHome interfaces
   */

  o << "/*" << endl
    << " * Inherited CCMHome interface" << endl
    << " */" << endl << endl;

  // get_component_def

  o << "CORBA::Object_ptr" << endl;
  o << "MICO_CCM_impls::" << endl;
  o << flatName << "_impl::get_component_def ()" << endl;
  o << BL_OPEN;
  o << "return CORBA::Object::_nil ();" << endl;
  o << BL_CLOSE << endl;

  // get_home_def

  o << "CORBA::Object_ptr" << endl;
  o << "MICO_CCM_impls::" << endl;
  o << flatName << "_impl::get_home_def ()" << endl;
  o << BL_OPEN;
  o << "return CORBA::Object::_nil ();" << endl;
  o << BL_CLOSE << endl;

  // remove_component

  o << "void" << endl;
  o << "MICO_CCM_impls::" << endl;
  o << flatName << "_impl::remove_component (Components::CCMObject_ptr _co)"
    << endl;
  o << BL_OPEN;
  if (_type == ServiceComponent) {
    o << "/*" << endl
      << " * remove_component() has no effect on a Service Component" << endl
      << " */" << endl;
  }
  else if (_type == SessionComponent) {
    o << "PortableServer::ServantBase_var _cs =" << endl
      << "  _container->get_skeleton_for_reference (_co);" << endl;
    o << "CORBA::Object_var _ci =" << endl
      << "  _container->get_instance_for_component (_cs.in());" << endl;
    o << ccmManagedAbsName << "_var _cti =" << endl
      << "  " << ccmManagedAbsName << "::_narrow (_ci.in());" << endl;
    o << endl;
    o << "Components::SessionComponent_var _sc =" << endl
      << "  Components::SessionComponent::_narrow (_ci);" << endl;
    o << "if (!CORBA::is_nil (_sc)) " << BL_OPEN;
    o << "_sc->ccm_passivate ();" << endl;
    o << "_sc->ccm_remove ();" << endl;
    o << BL_CLOSE;
    o << "_container->deactivate_component (_cs);" << endl;
  }
  o << BL_CLOSE << endl;

  if (CORBA::is_nil (key)) {
    o << "/*" << endl
      << " * Inherited KeylessCCMHome interface" << endl
      << " */" << endl << endl;

    o << "Components::CCMObject_ptr" << endl;
    o << "MICO_CCM_impls::" << endl;
    o << flatName << "_impl::create_component ()" << endl;
    o << BL_OPEN;
    o << "return create ();" << endl;
    o << BL_CLOSE << endl;
  }
}

/*
 * ----------------------------------------------------------------------
 * This code produces glue for facets that are implemented locally, i.e.
 * a component has returned a local interface reference from a provide_*
 * method. We therefore need to implement the POA skeleton for that inter-
 * face and delegate calls to the user implementation.
 * ----------------------------------------------------------------------
 */

void
CCMCodeGenCpp::emitInterfaceGlueDecl (CORBA::InterfaceDef_ptr iface,
				      CORBA::ComponentIR::ComponentDef_ptr comp,
				      const char * facet_name)
{
  CORBA::String_var n = comp->name();
  CORBA::String_var a = comp->absolute_name();
  string compClassName = (const char *) ID(n);
  string compAbsName = (const char *) ID(a);
  compAbsName = compAbsName.substr (2);
  string compFlatName = compAbsName;
  CORBA::ULong idx;

  while ((idx = compFlatName.find ("::")) != (CORBA::ULong) -1) {
    compFlatName.replace (idx, 2, "_");
  }

  n = iface->name ();
  a = iface->absolute_name ();
  string ifaceClassName = (const char *) ID(n);
  string ifaceAbsName = (const char *) ID(a);
  ifaceAbsName = ifaceAbsName.substr (2);
  string ifaceFlatName = ifaceAbsName;

  while ((idx = ifaceFlatName.find ("::")) != (CORBA::ULong) -1) {
    ifaceFlatName.replace (idx, 2, "_");
  }

  string fullName = compFlatName;
  fullName += "_";
  fullName += facet_name;
  fullName += "_impl";

  o << "/*" << endl
    << " * Glue class for facet " << facet_name << " of component "
    << compAbsName << endl
    << " * type " << ifaceAbsName << endl
    << " */" << endl << endl;

  o << "class " << fullName << " :" << indent << indent << endl;
  o << "virtual public POA_" << ifaceAbsName << "," << endl;
  o << "virtual public PortableServer::RefCountServantBase";
  o << exdent << exdent << endl << BL_OPEN;

  CORBA::ContainedSeq_var c;

  o << "private:" << indent << endl;
  o << compFlatName << "_impl * _glue;" << endl;

  o << exdent << endl;
  o << "public:" << indent << endl;

  o << fullName << " (" << compFlatName << "_impl *_g);" << endl;
  o << "~" << fullName << " ();" << endl;
  o << endl;

  /*
   * Prototypes
   */

  o << "/*" << endl
    << " * Attributes" << endl
    << " */" << endl << endl;

  {
    c = iface->contents (CORBA::dk_Attribute, 0);

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::AttributeDef_var attr = CORBA::AttributeDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));
      CORBA::IDLType_var type_id = attr->type_def();
      CORBA::String_var n = attr->name();
      emit_type_for_result( type_id );
      o << " " << ID( n.in() ) << " ();" << endl;
      
      if( attr->mode() == CORBA::ATTR_NORMAL ) {
	o << "void " << ID( n.in() ) << " (";
	emit_type_for_param( type_id, CORBA::PARAM_IN );
	o << " _value);" << endl;
      }
    }

    if (c->length()) {
      o << endl;
    }
    else {
      o << "// none" << endl << endl;
    }
  }

  o << "/*" << endl
    << " * Operations" << endl
    << " */" << endl << endl;

  {
    c = iface->contents (CORBA::dk_Operation, 0);

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::OperationDef_var op = CORBA::OperationDef::_narrow(c[j]);
      CORBA::IDLType_var type_id = op->result_def();
      CORBA::String_var n = op->name();
      emit_type_for_result( type_id );
      o << " " << ID( n.in() ) << "(";
      CORBA::ParDescriptionSeq_var p = op->params();
      CORBA::ULong k;
      for( k = 0; k < p->length(); k++ ) {
	if( k != 0 )
	  o << ",";
	o << " ";
	emit_type_for_param( p[ k ].type_def, p[ k ].mode );
	o << " " << ID(p[ k ].name);
      }
      o << ");" << endl;
    }
  
    if (c->length()) {
      o << endl;
    }
    else {
      o << "// none" << endl << endl;
    }
  }

  o << "/*" << endl
    << " * Overload default implementation" << endl
    << " */" << endl
    << endl;

  o << "CORBA::Object_ptr _get_component ();" << endl;
  o << exdent << BL_CLOSE_SEMI << endl << endl;
}

void
CCMCodeGenCpp::emitInterfaceGlueImpl (CORBA::InterfaceDef_ptr iface,
				      CORBA::ComponentIR::ComponentDef_ptr comp,
				      const char * facet_name)
{
  CORBA::String_var n = comp->name();
  CORBA::String_var a = comp->absolute_name();
  string compClassName = (const char *) ID(n);
  string compAbsName = (const char *) ID(a);
  compAbsName = compAbsName.substr (2);
  string compFlatName = compAbsName;
  CORBA::ULong idx;

  while ((idx = compFlatName.find ("::")) != (CORBA::ULong) -1) {
    compFlatName.replace (idx, 2, "_");
  }

  n = iface->name ();
  a = iface->absolute_name ();
  string ifaceClassName = (const char *) ID(n);
  string ifaceAbsName = (const char *) ID(a);
  ifaceAbsName = ifaceAbsName.substr (2);
  string ifaceFlatName = ifaceAbsName;

  while ((idx = ifaceFlatName.find ("::")) != (CORBA::ULong) -1) {
    ifaceFlatName.replace (idx, 2, "_");
  }

  string fullName = compFlatName;
  fullName += "_";
  fullName += facet_name;
  fullName += "_impl";

  o << "/*" << endl
    << " * Glue class for facet " << facet_name << " of component "
    << compAbsName << endl
    << " * type " << ifaceAbsName << endl
    << " */" << endl << endl;

  CORBA::ContainedSeq_var c;

  o << "MICO_CCM_impls::" << endl
    << fullName << "::" << fullName << " (" << compFlatName << "_impl * _g)"
    << endl;
  o << BL_OPEN;
  o << "_glue = _g;" << endl;
  o << BL_CLOSE << endl;

  o << "MICO_CCM_impls::" << endl
    << fullName << "::~" << fullName << " ()" << endl;
  o << BL_OPEN;
  o << BL_CLOSE << endl;

  /*
   * Delegators
   */

  o << "/*" << endl
    << " * Attributes" << endl
    << " */" << endl << endl;

  {
    c = iface->contents (CORBA::dk_Attribute, 0);

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::AttributeDef_var attr = CORBA::AttributeDef::_narrow(c[j]);
      assert (!CORBA::is_nil(attr));
      CORBA::IDLType_var type_id = attr->type_def();
      CORBA::String_var n = attr->name();

      emit_type_for_result (type_id);
      o << endl;
      o << "MICO_CCM_impls::" << endl;
      o << fullName << "::" << ID(n.in()) << " ()" << endl;
      o << BL_OPEN;
      o << "return _glue->_instance_for_facet_" << facet_name << "->"
	<< ID(n.in()) << " ();" << endl;
      o << BL_CLOSE << endl;

      if (attr->mode() == CORBA::ATTR_NORMAL) {
	o << "void " << endl;
	o << "MICO_CCM_impls::" << endl;
	o << fullName << "::" << ID(n.in()) << " (";
	emit_type_for_param (type_id, CORBA::PARAM_IN);
	o << " _value)" << endl;
	o << BL_OPEN;
	o << "_glue->_instance_for_facet_" << facet_name << "->"
	  << ID(n.in()) << " (_value);" << endl;
	o << BL_CLOSE << endl;
      }
    }
  }

  o << "/*" << endl
    << " * Operations" << endl
    << " */" << endl << endl;

  {
    c = iface->contents (CORBA::dk_Operation, 0);

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::OperationDef_var op = CORBA::OperationDef::_narrow(c[j]);
      CORBA::IDLType_var type_id = op->result_def();
      CORBA::ParDescriptionSeq_var p = op->params();
      CORBA::TypeCode_var rtc = type_id->type ();
      CORBA::String_var n = op->name();
      
      emit_type_for_result (type_id);
      o << endl;
      o << "MICO_CCM_impls::" << endl;
      o << fullName << "::" << ID(n.in()) << " (";

      for (CORBA::ULong k1=0; k1<p->length(); k1++) {
	if (k1 != 0)
	  o << ", ";
	emit_type_for_param (p[k1].type_def, p[k1].mode);
	o << " " << ID(p[k1].name);
      }
      o << ")" << endl;
    
      o << BL_OPEN;
    
      if (rtc->kind() != CORBA::tk_void) {
	o << "return ";
      }
    
      o << "_glue->_instance_for_facet_" << facet_name << "->" 
	<< ID(n.in()) << " (";
      for (CORBA::ULong k2=0; k2<p->length(); k2++) {
	if (k2 != 0)
	  o << ", ";
	o << ID(p[k2].name);
      }
      o << ");" << endl;
    
      o << BL_CLOSE << endl;
    }
  }

  o << "/*" << endl
    << " * Overload default implementation" << endl
    << " */" << endl
    << endl;

  {
    o << "CORBA::Object_ptr" << endl;
    o << "MICO_CCM_impls::" << endl;
    o << fullName << "::_get_component ()" << endl;
    o << BL_OPEN;
    o << "return ";
    if (_type == ServiceComponent) {
      o << "_glue->_container->get_reference_for_component ();" << endl;
    }
    else if (_type == SessionComponent) {
      o << "_glue->_container->get_reference_for_component (_glue);" << endl;
    }
    o << BL_CLOSE << endl;
  }
}

/*
 * ----------------------------------------------------------------------
 * This code produces glue for event consumers. They implement the
 * EventConsumer interface and forward events to the component.
 * ----------------------------------------------------------------------
 */

void
CCMCodeGenCpp::emitConsumerGlueDecl (CORBA::ComponentIR::EventDef_ptr event,
				     CORBA::ComponentIR::ComponentDef_ptr comp,
				     const char * port_name)
{
  CORBA::String_var n = comp->name();
  CORBA::String_var a = comp->absolute_name();
  string compClassName = (const char *) ID(n);
  string compAbsName = (const char *) ID(a);
  compAbsName = compAbsName.substr (2);
  string compFlatName = compAbsName;
  CORBA::ULong idx;

  while ((idx = compFlatName.find ("::")) != (CORBA::ULong) -1) {
    compFlatName.replace (idx, 2, "_");
  }

  n = event->name ();
  a = event->absolute_name ();
  string eventName = n.in();
  string eventClassName = (const char *) ID(n);
  string eventAbsName = (const char *) ID(a);
  eventAbsName = eventAbsName.substr (2);
  string eventFlatName = eventAbsName;

  while ((idx = eventFlatName.find ("::")) != (CORBA::ULong) -1) {
    eventFlatName.replace (idx, 2, "_");
  }

  string fullName = compFlatName;
  fullName += "_";
  fullName += port_name;
  fullName += "_impl";

  o << "/*" << endl
    << " * Glue class for consumer " << port_name << " of component "
    << compAbsName << endl
    << " * type " << eventAbsName << endl
    << " */" << endl << endl;

  o << "class " << fullName << " :" << indent << indent << endl;
  o << "virtual public POA_" << eventAbsName << "Consumer," << endl;
  o << "virtual public PortableServer::RefCountServantBase";
  o << exdent << exdent << endl << BL_OPEN;

  CORBA::ContainedSeq_var c;

  o << "private:" << indent << endl;
  o << compFlatName << "_impl * _glue;" << endl;

  o << exdent << endl;
  o << "public:" << indent << endl;

  o << fullName << " (" << compFlatName << "_impl * _g);" << endl;
  o << "~" << fullName << " ();" << endl;
  o << endl;

  /*
   * Prototypes
   */

  o << "void push_event (Components::EventBase *);" << endl;
  o << "void push_" << eventName << " (" << eventAbsName << " *);" << endl;

  o << exdent << BL_CLOSE_SEMI << endl << endl;
}

void
CCMCodeGenCpp::emitConsumerGlueImpl (CORBA::ComponentIR::EventDef_ptr event,
				     CORBA::ComponentIR::ComponentDef_ptr comp,
				     const char * port_name)
{
  CORBA::String_var n = comp->name();
  CORBA::String_var a = comp->absolute_name();
  string compClassName = (const char *) ID(n);
  string compAbsName = (const char *) ID(a);
  compAbsName = compAbsName.substr (2);
  string compFlatName = compAbsName;
  CORBA::ULong idx;

  while ((idx = compFlatName.find ("::")) != (CORBA::ULong) -1) {
    compFlatName.replace (idx, 2, "_");
  }

  n = event->name ();
  a = event->absolute_name ();
  string eventName = n.in();
  string eventClassName = (const char *) ID(n);
  string eventAbsName = (const char *) ID(a);
  eventAbsName = eventAbsName.substr (2);
  string eventFlatName = eventAbsName;

  while ((idx = eventFlatName.find ("::")) != (CORBA::ULong) -1) {
    eventFlatName.replace (idx, 2, "_");
  }

  string eventConsumerName = eventAbsName;
  eventConsumerName += "Consumer";
  
  string ccmConsumerName = eventConsumerName;

  if ((idx = ccmConsumerName.rfind ("::")) != (CORBA::ULong) -1) {
    ccmConsumerName.insert (idx+2, "CCM_");
  }
  else {
    ccmConsumerName.insert (0, "CCM_");
  }

  string fullName = compFlatName;
  fullName += "_";
  fullName += port_name;
  fullName += "_impl";

  o << "/*" << endl
    << " * Glue class for consumer " << port_name << " of component "
    << compAbsName << endl
    << " * type " << eventAbsName << endl
    << " */" << endl << endl;

  CORBA::ContainedSeq_var c;

  o << "MICO_CCM_impls::" << endl
    << fullName << "::" << fullName << " (" << compFlatName << "_impl * _g)"
    << endl;
  o << BL_OPEN;
  o << "_glue = _g;" << endl;
  o << BL_CLOSE << endl;

  o << "MICO_CCM_impls::" << endl
    << fullName << "::~" << fullName << " ()" << endl;
  o << BL_OPEN;
  o << BL_CLOSE << endl;

  /*
   * Delegators
   */

  // Components::EventConsumerBase::push_event ()

  o << "void" << endl
    << "MICO_CCM_impls::" << endl
    << fullName << "::push_event (Components::EventBase * ev)" << endl;
  o << BL_OPEN;
  o << eventAbsName << " * tev = " << eventAbsName << "::_downcast (ev);"
    << endl;
  o << "if (tev == 0 && ev != 0) " << BL_OPEN;
  o << "throw Components::BadEventType ((const char *) \""
    << eventAbsName << "\");" << endl;
  o << BL_CLOSE;
  o << "push_" << eventName << " (tev);" << endl;
  o << BL_CLOSE << endl;

  // Components::EventConsumerBase::push_<name> ()

  o << "void" << endl
    << "MICO_CCM_impls::" << endl
    << fullName << "::push_" << eventName
    << " (" << eventAbsName << " * tev)" << endl;
  o << BL_OPEN;
  if (_etype == MonolithicExecutor) {
    o << "_glue->_instance->push_" << port_name << " (tev);" << endl;
  }
  else {
    o << "CORBA::Object_var _eo = "
      << "_glue->_instance->obtain_executor (\"" << port_name << "\");"
      << endl;
    o << ccmConsumerName << "_var _e =" << endl
      << "  " << ccmConsumerName << "::_narrow (_eo);" << endl;
    o << "_e->push (tev);" << endl;
    o << "_glue->_instance->release_executor (_eo.in());" << endl;
  }
  o << BL_CLOSE << endl;
}

/*
 * ----------------------------------------------------------------------
 * This code produces an implementation of the component's context
 * interface.
 * ----------------------------------------------------------------------
 */

void
CCMCodeGenCpp::emitContextDecl (CORBA::ComponentIR::ComponentDef_ptr comp)
{
  CORBA::String_var n = comp->name();
  CORBA::String_var a = comp->absolute_name();
  string compClassName = (const char *) ID(n);
  string compAbsName = (const char *) ID(a);
  compAbsName = compAbsName.substr (2);
  CORBA::ULong idx;

  string compFlatName = compAbsName;

  while ((idx = compFlatName.find ("::")) != (CORBA::ULong) -1) {
    compFlatName.replace (idx, 2, "_");
  }

  string ccmCompAbsName = compAbsName;

  if ((idx = ccmCompAbsName.rfind ("::")) != (CORBA::ULong) -1) {
    ccmCompAbsName.insert (idx+2, "CCM_");
  }
  else {
    ccmCompAbsName.insert (0, "CCM_");
  }

  string fullName = compFlatName;
  fullName += "_Context_impl";

  o << "/*" << endl
    << " * Context class for component " << compAbsName << endl
    << " */" << endl << endl;

  o << "class " << fullName << " :" << indent << indent << endl;

  CORBA::ComponentIR::ComponentDef_var base = comp->base_component ();

  if (!CORBA::is_nil (base)) {
    a = base->absolute_name ();
    string baseFlatName = (const char *) ID(a.in());
    baseFlatName = baseFlatName.substr (2);

    while ((idx = baseFlatName.find ("::")) != (CORBA::ULong) -1) {
      baseFlatName.replace (idx, 2, "_");
    }

    o << baseFlatName << "_Context_impl," << endl;
  }

  o << "virtual public " << ccmCompAbsName << "_Context";
  o << exdent << exdent << endl << BL_OPEN;

  CORBA::ContainedSeq_var c;

  o << "private:" << indent << endl;
  o << compFlatName << "_impl * _glue;" << endl;

  if (_type == ServiceComponent) {
    o << "MICO::CCM::ServiceContainer * _container;" << endl;
  }
  else if (_type == SessionComponent) {
    o << "MICO::CCM::SessionContainer * _container;" << endl;
  }

  o << exdent << endl;
  o << "public:" << indent << endl;

  o << fullName << " (" << compFlatName << "_impl * _g, ";

  if (_type == ServiceComponent) {
    o << "MICO::CCM::ServiceContainer * _c);" << endl;
  }
  else if (_type == SessionComponent) {
    o << "MICO::CCM::SessionContainer * _c);" << endl;
  }

  o << "~" << fullName << " ();" << endl;
  o << endl;

  // receptacle ports

  {
    c = comp->contents (CORBA::dk_Uses, 1);

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::UsesDef_var attr =
	CORBA::ComponentIR::UsesDef::_narrow(c[j]);
      CORBA::String_var n = attr->name ();

      if (!attr->is_multiple()) {
	CORBA::IDLType_var type_id = attr->interface_type();
	emit_type_for_result (type_id);
	o << " get_connection_" << n << " ();" << endl;
      }
      else {
	o << compAbsName << "::" << n << "Connections *";
	o << " get_connections_" << n << " ();" << endl;
      }
    }
  }

  // publisher ports

  {
    c = comp->contents (CORBA::dk_Publishes, 1);

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::PublishesDef_var attr =
	CORBA::ComponentIR::PublishesDef::_narrow(c[j]);
      CORBA::ComponentIR::EventDef_var event = attr->event();
      CORBA::String_var event_name = event->absolute_name();
      CORBA::String_var n = attr->name ();
      assert (!CORBA::is_nil(attr));

      o << "void push_" << n << " (" << event_name << " *);" << endl;
    }
  }

  // emitter ports

  {
    c = comp->contents (CORBA::dk_Emits, 1);

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::EmitsDef_var attr =
	CORBA::ComponentIR::EmitsDef::_narrow(c[j]);
      CORBA::ComponentIR::EventDef_var event = attr->event();
      CORBA::String_var event_name = event->absolute_name();
      CORBA::String_var n = attr->name ();
      assert (!CORBA::is_nil(attr));

      o << "void push_" << n << " (" << event_name << " *);" << endl;
    }
  }

  // SessionContext

  o << "Components::CCMHome_ptr get_CCM_home ();" << endl;
  o << "CORBA::Object_ptr get_CCM_object ();" << endl;

  o << exdent << BL_CLOSE_SEMI << endl << endl;
}

void
CCMCodeGenCpp::emitContextImpl (CORBA::ComponentIR::ComponentDef_ptr comp)
{
  CORBA::ComponentIR::ComponentDef_var base = comp->base_component ();
  CORBA::String_var n = comp->name();
  CORBA::String_var a = comp->absolute_name();
  string compClassName = (const char *) ID(n);
  string compAbsName = (const char *) ID(a);
  compAbsName = compAbsName.substr (2);
  string compFlatName = compAbsName;
  CORBA::ULong idx;

  while ((idx = compFlatName.find ("::")) != (CORBA::ULong) -1) {
    compFlatName.replace (idx, 2, "_");
  }

  string fullName = compFlatName;
  fullName += "_Context_impl";

  o << "/*" << endl
    << " * Context class for component " << compAbsName << endl
    << " */" << endl << endl;

  CORBA::ContainedSeq_var c;

  // constructor

  o << "MICO_CCM_impls::" << endl
    << fullName << "::" << fullName << " (" << compFlatName << "_impl * _g, ";

  if (_type == ServiceComponent) {
    o << "MICO::CCM::ServiceContainer * _c)";
  }
  else if (_type == SessionComponent) {
    o << "MICO::CCM::SessionContainer * _c)";
  }

  if (!CORBA::is_nil (base)) {
    CORBA::String_var ba = base->absolute_name ();
    string baseFlatName = (const char *) ID(ba.in());
    baseFlatName = baseFlatName.substr (2);

    while ((idx = baseFlatName.find ("::")) != (CORBA::ULong) -1) {
      baseFlatName.replace (idx, 2, "_");
    }

    o << indent << indent << endl;
    o << ": " << baseFlatName << "_Context_impl (_g, _c)";
    o << exdent << exdent << endl;
  }
  else {
    o << endl;
  }

  o << BL_OPEN;
  o << "_glue = _g;" << endl;
  o << "_container = _c;" << endl;
  o << BL_CLOSE << endl;

  // destructor

  o << "MICO_CCM_impls::" << endl
    << fullName << "::~" << fullName << " ()" << endl;
  o << BL_OPEN;
  o << BL_CLOSE << endl;

  // receptacle ports

  {
    c = comp->contents (CORBA::dk_Uses, 1);

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::UsesDef_var attr =
	CORBA::ComponentIR::UsesDef::_narrow(c[j]);
      CORBA::String_var n = attr->name ();

      if (!attr->is_multiple()) {
	CORBA::IDLType_var type_id = attr->interface_type();
	emit_type_for_result (type_id);
	o << endl
	  << "MICO_CCM_impls::" << endl
	  << fullName << "::get_connection_" << n << " ()"
	  << endl;
	o << BL_OPEN;
	o << "return _glue->get_connection_" << n << " ();" << endl;
	o << BL_CLOSE << endl;
      }
      else {
	o << compAbsName << "::" << n << "Connections *" << endl;
	o << "MICO_CCM_impls::" << endl
	  << fullName << "::get_connections_" << n << " ()"
	  << endl;
	o << BL_OPEN;
	o << "return _glue->get_connections_" << n << " ();" << endl;
	o << BL_CLOSE << endl;
      }
    }
  }

  // publishes ports

  {
    c = comp->contents (CORBA::dk_Publishes, 1);

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::PublishesDef_var attr =
	CORBA::ComponentIR::PublishesDef::_narrow(c[j]);
      CORBA::ComponentIR::EventDef_var event = attr->event();
      CORBA::String_var event_name = event->absolute_name();
      CORBA::String_var n = attr->name ();
      assert (!CORBA::is_nil(attr));

      o << "void" << endl;
      o << "MICO_CCM_impls::" << endl
	<< fullName << "::push_" << n << " (" << event_name << " * ev)"
	<< endl;
      o << BL_OPEN;
      o << "for (CORBA::ULong idx=0; "
	<< "idx<_glue->_for_publisher_" << n << ".length(); "
	<< "idx++) " << BL_OPEN;
      o << "try " << BL_OPEN;
      o << "_glue->_for_publisher_" << n << "[idx]->consumer()->push_event (ev);" << endl;
      o << BL_CLOSE << " catch (...) " << BL_OPEN;
      o << BL_CLOSE;
      o << BL_CLOSE;
      o << BL_CLOSE << endl;
    }
  }

  // emitter ports

  {
    c = comp->contents (CORBA::dk_Emits, 1);

    for (CORBA::ULong j=0; j<c->length(); j++) {
      CORBA::ComponentIR::EmitsDef_var attr =
	CORBA::ComponentIR::EmitsDef::_narrow(c[j]);
      CORBA::ComponentIR::EventDef_var event = attr->event();
      CORBA::String_var event_name = event->absolute_name();
      CORBA::String_var n = attr->name ();
      assert (!CORBA::is_nil(attr));

      o << "void" << endl;
      o << "MICO_CCM_impls::" << endl
	<< fullName << "::push_" << n << " (" << event_name << " * ev)"
	<< endl;
      o << BL_OPEN;
      o << "if (!CORBA::is_nil (_glue->_for_emitter_" << n << ")) "
	<< BL_OPEN;
      o << "_glue->_for_emitter_" << n << "->push_event (ev);" << endl;
      o << BL_CLOSE;
      o << BL_CLOSE << endl;
    }
  }

  // SessionContext

  {
    o << "Components::CCMHome_ptr" << endl;
    o << "MICO_CCM_impls::" << endl
      << fullName << "::get_CCM_home ()" << endl;
    o << BL_OPEN;
    o << "return _container->get_CCM_home ();" << endl;
    o << BL_CLOSE << endl;
  }

  {
    o << "CORBA::Object_ptr" << endl;
    o << "MICO_CCM_impls::" << endl
      << fullName << "::get_CCM_object ()" << endl;
    o << BL_OPEN;
    o << "return _container->get_CCM_object (_glue->_instance);" << endl;
    o << BL_CLOSE << endl;
  }
}

/*
 * ----------------------------------------------------------------------
 */

bool
CCMCodeGenCpp::ccm_check_for_included_defn (CORBA::IRObject_ptr obj,
					    bool emitinc)
{
  string id;

  if (CORBA::is_nil (obj)) {
    return false;
  }

  CORBA::Contained_var con = CORBA::Contained::_narrow( obj );
  if( !CORBA::is_nil( con ) ) {
    CORBA::String_var tmp = con->id();
    id = tmp;
  } else {
    // Check if this is an anonymous type
    CORBA::IDLType_var idl_type = CORBA::IDLType::_narrow( obj );
    if( CORBA::is_nil( idl_type ) )
      // if it is not an IDLType, then it can't be an anonymous type
      return false;
    id = _db->gen_pseudo_repoid( idl_type );
  }

  /*
   * if --gen-included-defs, generate code anyway
   */

  if (_params.gen_included_defs) {
    return false;
  }
  
  string toplev = _db->get_toplevel_fname ();

  /*
   * If the "toplevel file name" is empty, generate code for everything.
   */

  if (toplev.length() == 0) {
    return false;
  }

  /*
   * If the repo id is not registered in our file name database, we assume
   * that no code needs to be generated, for example because the data has
   * already existed in the IFR prior to loading the toplevel file.
   */

  if (!_db->is_repoid_defined (id)) {
    return true;
  }

  string fname = _db->get_fname_for_repoid (id);

  if (fname != toplev) {
    // "obj" was defined in an include file. Don't emit code for it

    if (!emitinc) {
      return true;
    }

    /*
     * We only care if this is a component or home, to pull in the
     * interfaces defined by mico-ccm. Other information is already
     * available, since we include our "own" IDL file, which in turn
     * loads all IDL dependencies.
     */
    
    if (obj->def_kind() != CORBA::dk_Component ||
	obj->def_kind() != CORBA::dk_Home) {
      return true;
    }
  
    string::size_type pos = fname.rfind( "." );
    if( (int)pos >= 0 && pos < fname.length() )
      fname = fname.substr (0, pos);
    fname += "_ccm.";
    fname += _params.hh_suffix;

    if (_include_files.count( fname ) == 0) {
      _include_files.insert(fname);
      // For the first time we have to generate a #include directive
      if (!_params.mico_core) {
	o << "#include ";
	o << ( _params.use_quotes ? "\"" : "<" );
	o << _params.rewrite_include_path(fname);
	o << ( _params.use_quotes ? "\"" : ">" );
	o << endl;
      }
    }

    return true;
  }

  return false;
}

/*
 * ----------------------------------------------------------------------
 */

void
usage ()
{
  cout << "usage:" << endl
       << "  mico-ccm <options> idl-file" << endl
       << endl
       << "possible <options> are:" << endl
       << "    --session      Make code for Session Components" << endl
       << "    --service      Make code for Service Components" << endl
       << "    --standalone   Make a standalone program" << endl;
}

int
main (int argc, char *argv[])
{
  CORBA::Container_var container;
  CORBA::Repository_var repository;
  DB db;
  
  // ORB initialization
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

#ifdef _WIN32
  // Some parts of the idl-compiler can't handle backslashes in paths
  // replace all the backslashes by slashes
  // runtime library can handle slashes fine
  // assume that only paths have backslashes(true for now)
  // (changing the argv[] is allowed)
  for(int i=1; i<argc; i++)
  {
      char *argp=argv[i];
      while(argp && *argp)
      {
          if(*argp=='\\')
              *argp='/';
          
          argp++;
      }
  }
  
#endif

  /*
   * Process command-line options
   */

  CCMCodeGenCpp::ComponentType type = CCMCodeGenCpp::SessionComponent;
  CCMCodeGenCpp::ExecutorType etype = CCMCodeGenCpp::MonolithicExecutor;
  MICOGetOpt::OptMap opts;
  IDLParam params;
  bool standalone = false;

  // IDL Compiler options
  opts["--c++-suffix"] = "arg-expected";
  opts["--hh-prefix"]  = "arg-expected";
  opts["--hh-suffix"]  = "arg-expected";
  opts["--use-quotes"] = "";
  opts["--no-paths"]   = "";
  opts["--name"]       = "arg-expected";

  // Preprocessor options
  opts["-B"]           = "arg-expected";
  opts["-I"]           = "arg-expected";
  opts["-D"]           = "arg-expected";

  // CCM Code Generation options
  opts["--session"]    = "";
  opts["--service"]    = "";
  opts["--standalone"] = "";
  opts["--monolithic"] = "";
  opts["--locator"]    = "";

  MICOGetOpt opt_parser (opts);
  if (!opt_parser.parse (argc, argv)) {
    usage();
    exit(1);
  }

  if (argc == 2 && *argv[1] != '-') {
    params.name = argv[1];
    params.file = argv[1];
  }
  else if (argc > 2) {
    usage ();
    exit (1);
  }

  for (MICOGetOpt::OptVec::const_iterator i=opt_parser.opts().begin();
       i != opt_parser.opts().end(); i++) {
    string arg = (*i).first;
    string val = (*i).second;

    if (arg == "-B") {
      // remove ./'s, attach trailing /
      int pos;
      while ((pos = val.find ("/./")) >= 0)
	val.replace (pos, 2, "", 0);
      if (val.find ("./") == 0)
	val.replace (0, 2, "", 0);
      if (val == ".")
        val.erase();
      else if (!val.empty() && val[val.length() - 1] != '/')
        val += "/";

      params.base_dir = val;
    } else if (arg == "-I") {
      // remove ./'s, attach trailing /
      int pos;
      while ((pos = val.find ("/./")) >= 0)
	val.replace (pos, 2, "", 0);
      if (val.find ("./") == 0)
	val.replace (0, 2, "", 0);
      if (val == ".")
        val.erase();
      else if (!val.empty() && val[val.length() - 1] != '/')
        val += "/";

      params.cpp_options += " ";
      params.cpp_options += arg;
      params.cpp_options += val.empty() ? string(".") : val;
      // --include-prefix and --include-postfix refer to inc_paths.back()
      params.inc_paths.push_back (val);
      // default include path substitution is empty
      params.i_prefixes[val] = string();
    }
    else if (arg == "-D") {
      params.cpp_options += " ";
      params.cpp_options += arg + val;
    }
    else if (arg == "--c++-suffix") {
      params.cpp_suffix = val;
    }
    else if (arg == "--hh-prefix") {
      // remove ./'s, attach trailing /
      int pos;
      while ((pos = val.find ("/./")) >= 0)
	val.replace (pos, 2, "", 0);
      if (val.find ("./") == 0)
	val.replace (0, 2, "", 0);
      if (val == ".")
        val.erase();
      else if (!val.empty() && val[val.length() - 1] != '/')
        val += "/";

      params.hh_prefix = val;
    }
    else if (arg == "--hh-suffix") {
      params.hh_suffix = val;
    }
    else if (arg == "--use-quotes") {
      params.use_quotes = true;
    }
    else if (arg == "--no-paths") {
      params.no_path_in_include = true;
    }
    else if (arg == "--name") {
      params.name = val;
    }
    else if (arg == "--repo-id") {
      params.repo_id = val;
    }
    else if (arg == "--service") {
      type = CCMCodeGenCpp::ServiceComponent;
    }
    else if (arg == "--session") {
      type = CCMCodeGenCpp::SessionComponent;
    }
    else if (arg == "--monolithic") {
      etype = CCMCodeGenCpp::MonolithicExecutor;
    }
    else if (arg == "--locator") {
      etype = CCMCodeGenCpp::ExecutorLocator;
    }
    else if (arg == "--standalone") {
      standalone = true;
    }
  }

  /*
   * Determine Interface Repository to use
   */

  if (argc == 1) {
    /*
     * When no filename is given on the command line, connect to remote IFR
     */

    CORBA::Object_var obj =
      orb->resolve_initial_references ("InterfaceRepository");
    repository = CORBA::Repository::_narrow (obj);

    if (CORBA::is_nil (repository)) {
      cerr << "error: cannot connect to remote interface repository." << endl;
      exit (1);
    }
  }
  else {
    /*
     * Else, use local IR
     */

    repository = MICO::create_interface_repository (orb);
  }

  assert (!CORBA::is_nil (repository));

  /*
   * Input file processing
   */
  
  if (argc == 2) {

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

    if (params.i_prefixes.end() == params.i_prefixes.find(includepath)) {
      params.i_prefixes[includepath] = string();
    }
    if (params.i_prefixes.end() == params.i_prefixes.find(micopath)) {
      params.i_prefixes[micopath] = "mico/";
    }

    cmd += "-isystem ";
    cmd += includepath;
    cmd += ' ';
    cmd += " -isystem ";
    cmd += micopath;
    cmd += ' ';

    cmd += params.file;

    FILE* inp_file = OSMisc::popen( cmd.c_str(), "r" );

    if( inp_file == NULL ) {
      cerr << "error: cannot open input file: " << argv[1] << endl;
      exit( 1 );
    }
    
    int dummy_char = fgetc (inp_file);
    if (dummy_char == EOF) {
      cerr << "error: cannot execute " << params.cpp << endl;
      exit( 1 );
    }
    ungetc (dummy_char, inp_file);

    if( params.cpp_only() ) {
      int ch = fgetc( inp_file );
      while( ch != EOF ) {
	fputc( ch, stdout );
	ch = fgetc( inp_file );
      }
      exit( 0 );
    }

    /*
     * Step 2: Invoke the Parser (bison)
     */
    
    Parser parser( inp_file, params.file.c_str() );
    parser.parse();

    OSMisc::pclose( inp_file );

    /*
     * Step 3: Traverse the Parse Tree and load Interface Repository
     */

    db.set_toplevel_fname (params.file.c_str());
    IDLParser idlParser (db, params);
    idlParser.collect (repository, parser.getRootNode());
    db.set_repoids (repository);
  }

  /*
   * At this time, we are connected to an Interface Repository, and
   * this IR has been loaded with the contents of the IDL file (if
   * given on the command line).
   */

  /*
   * Determine which portions of the IR to dump. This is selected by
   * using the --repo-id option on the command line, else the whole
   * contents are dumped.
   */

  container = CORBA::Container::_duplicate (repository);
  
  if (params.repo_id != "" && params.repo_id != "::") {
    CORBA::Contained_var contained =
      repository->lookup_id( (char *) params.repo_id.c_str() );
    if( CORBA::is_nil( contained ) ) {
      contained = repository->lookup( (char *) params.repo_id.c_str() );
      if( CORBA::is_nil( contained ) ) {
	cerr << "error: repo-id " << params.repo_id << " not found in IR";
	  cerr << endl;
	  exit( 1 );
      }
    }
    container = CORBA::Container::_narrow( contained );
    if( CORBA::is_nil( container ) ) {
      cerr << "error: repo-id " << params.repo_id << " is not a container";
      cerr << endl;
      exit( 1 );
    }
    if( container->def_kind() != CORBA::dk_Module ) {
      cerr << "error: can only generate code from module";
      cerr << endl;
      exit( 1 );
    }
  }

  /*
   * Determine output file name
   */

  if (argc == 1 && params.name == "") {
    cerr << "error: must have a --name when working from the IFR." << endl;
    exit (1);
  }
  
  string name = params.name;
  string::size_type pos = name.rfind( "." );
  if( (int)pos >= 0 && pos < name.length() )
    name = name.substr( 0, pos );
  pos = name.rfind( "/" );
#ifdef _WIN32
   if (pos==string::npos)
     pos = name.rfind( "\\" );
#endif
  if( (int)pos >=0 && pos < name.length() )
    name = name.substr( pos + 1 );
  
  /*
   * Invoke Code Generator
   */

  CCMCodeGenCpp gen (db, params, container, type, etype, standalone);
  gen.emit (name);

  return 0;
}

