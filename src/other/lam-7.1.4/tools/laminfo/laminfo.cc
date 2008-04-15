//
// Copyright (c) 2001-2004 The Trustees of Indiana University.  
//                         All rights reserved.
// Copyright (c) 1998-2001 University of Notre Dame. 
//                         All rights reserved.
// Copyright (c) 1994-1998 The Ohio State University.  
//                         All rights reserved.
// 
// This file is part of the LAM/MPI software package.  For license
// information, see the LICENSE file in the top level directory of the
// LAM/MPI source distribution.
// 
// $HEADER$
//
// $Id: laminfo.cc,v 1.25 2004/02/15 14:07:17 jsquyres Exp $
//

#include <lam_config.h>

#include <iostream>
#include <string>
#include <utility>

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/param.h>

#include <all_opt.h>
#include <etc_misc.h>
#include <vreq.h>
#include <priority.h>
#include <lam_build_info.h>
#include <lam-ssi.h>
#include <lam-ssi-boot.h>
#include <lam-ssi-coll.h>
#include <lam-ssi-rpi.h>
#include <lam-ssi-cr.h>
#include <all_array.h>

using namespace std;


//
// Local functions
//

static void do_version(bool want_all, OPT *ad);
static void show_lam_version(const string& scope);
static void show_boot_version(const lam_ssi_t **boot_modules, 
			      const string& which, const string& scope,
			      const string& ver_type);
static void show_coll_version(const lam_ssi_t **coll_modules, 
			      const string& which, const string& scope,
			      const string& ver_type);
static void show_rpi_version(const lam_ssi_t **rpi_modules, 
			     const string& which, const string& scope,
			     const string& ver_type);
static void show_cr_version(const lam_ssi_t **cr_modules, 
                             const string& which, const string& scope,
                             const string& ver_type);
static void show_ssi_version(const lam_ssi_t *ls, const string& type,
			     const string& scope, const string& ver_type);
static string make_version_str(const string& scope,
			       int major, int minor, int release, 
			       int alpha, int beta, int svn);

static void do_params(bool want_all);
static void show_ssi_params(const string& type, const string& module, 
                            const string& param);

static void do_path(bool want_all, OPT *ad);
static void show_path(const string& type, const string& value);
static void do_arch(OPT *ad);
static void do_config(bool want_all);
static void out(const string& pretty_message, const string& plain_message, 
                const string &value);
static void out(const string& pretty_message, const string &plain_message, 
                int value);
static void open_modules();
static void close_modules();


//
// Local variables
//

static bool opened_modules = false;
static bool pretty = true;
static int centerpoint = 20;
static int screen_width = 78;

static string ver_full = "full";
static string ver_major = "major";
static string ver_minor = "minor";
static string ver_release = "release";
static string ver_alpha = "alpha";
static string ver_beta = "beta";
static string ver_svn = "svn";

static string type_lam = "lam";
static string type_all = "all";
static string type_base = "base";
static string type_boot = "boot";
static string type_coll = "coll";
static string type_rpi = "rpi";
static string type_cr = "cr";

static string module_all = "all";
static string param_all = "all";

static string ver_ssi = "ssi";
static string ver_type = "type";
static string ver_module = "module";

static string path_prefix = "prefix";
static string path_bindir = "bindir";
static string path_libdir = "libdir";
static string path_incdir = "incdir";
static string path_pkglibdir = "pkglibdir";
static string path_sysconfdir = "sysconfdir";

static OPT *ad = NULL;


int
main(int argc, char *argv[])
{
  int ret;
  bool acted = false;
  bool want_all = false;

  // Initialize the argv parsing handle

  ad = ao_init();
  if (ad == 0) {
    ret = errno;
    show_help(NULL, "lib-call-fail", "ao_init", NULL);
    exit(ret);
  }
  ao_setflags(ad, AOPRESERVE_ARGV);
  ao_setopt1(ad, "h", 0, 0, 0);

  // Actions

  ao_setopt(ad, "version", 0, 2, 0);
  ao_setopt(ad, "param", 0, 2, 0);
  ao_setopt(ad, "path", 0, 1, 0);
  ao_setopt(ad, "arch", 0, 0, 0);
  ao_setopt(ad, "config", 0, 0, 0);
  ao_setopt(ad, "help", 0, 0, 0);
  ao_setopt(ad, "pretty", "parable", 0, 0);
  ao_setopt(ad, "parsable", "pretty", 0, 0);
  ao_setopt(ad, "hostname", 0, 0, 0);
  ao_setopt(ad, "all", 0, 0, 0);

  // Get SSI parameters, if any */

  lam_ssi_base_open(ad);
  lam_ssi_base_ao_setup(ad);

  // Do the parsing

  if (ao_parse(ad, &argc, argv) ||
      ao_taken(ad, "help") || ao_taken(ad, "h")) {
    show_help("laminfo", "usage", NULL);
    exit(EUSAGE);
  }
  lam_ssi_base_ao_process_args(ad);

  // Execute the desired action(s)

  if (ao_taken(ad, "pretty"))
    pretty = true;
  else if (ao_taken(ad, "parsable"))
    pretty = false;

  want_all = ao_taken(ad, "all");
  if (want_all || ao_taken(ad, "version")) {
    do_version(want_all, ad);
    acted = true;
  }
  if (want_all || ao_taken(ad, "path")) {
    do_path(want_all, ad);
    acted = true;
  }
  if (want_all || ao_taken(ad, "arch")) {
    do_arch(ad);
    acted = true;
  }
  if (want_all || ao_taken(ad, "config")) {
    do_config(true);
    acted = true;
  }
  if (want_all || ao_taken(ad, "param")) {
    do_params(want_all);
    acted = true;
  }

  // If no command line args are specified, show [a concise] everything

  if (!acted) {
    show_lam_version(ver_full);
    show_path(path_prefix, LAM_PREFIX);
    do_arch(ad);
    do_config(false);

    open_modules();

    show_boot_version(lam_ssi_boot_modules, type_all, ver_full, ver_module);
    show_coll_version(lam_ssi_coll_modules, type_all, ver_full, ver_module);
    show_rpi_version(lam_ssi_rpi_modules, type_all, ver_full, ver_module);
    show_cr_version(lam_ssi_crmpi_modules, type_all, ver_full, ver_module);
  }

  // All done

  close_modules();
  ao_free(ad);
  return 0;
}


static void
do_version(bool want_all, OPT *ad)
{
  int i, count;
  string arg1, scope, module;

  open_modules();

  if (want_all) {
    show_lam_version(ver_full);
    show_boot_version(lam_ssi_boot_modules, type_all, ver_full, type_all);
    show_coll_version(lam_ssi_coll_modules, type_all, ver_full, type_all);
    show_rpi_version(lam_ssi_rpi_modules, type_all, ver_full, type_all);
    show_cr_version(lam_ssi_crmpi_modules, type_all, ver_full, type_all);
  } else {
    count = ao_ninsts(ad, "version");
    for (i = 0; i < count; ++i) {
      arg1 = ao_param(ad, "version", i, 0);
      scope = ao_param(ad, "version", i, 1);

      // Version of LAM/MPI
    
      if (arg1 == type_lam)
        show_lam_version(scope);

      // Version of all boot modules or a specific boot module

      else if (arg1 == type_boot) {
        show_boot_version(lam_ssi_boot_modules, type_all, scope, type_all);
      } else if (arg1.substr(0, type_boot.length() + 1) == (type_boot + ":")) {
        show_boot_version(lam_ssi_boot_modules, 
                          arg1.substr(type_boot.length() + 1), scope, 
                          type_all);
      } 

      // Version of all coll modules or a specific coll module

      else if (arg1 == type_coll) {
        show_coll_version(lam_ssi_coll_modules, type_all, scope, type_all);
      } else if (arg1.substr(0, type_coll.length() + 1) == (type_coll + ":")) {
        show_coll_version(lam_ssi_coll_modules, 
                          arg1.substr(type_coll.length() + 1), scope, 
                          type_all);
      } 

      // Version of all rpi modules or a specific rpi module

      else if (arg1 == type_rpi) {
        show_rpi_version(lam_ssi_rpi_modules, type_all, scope, type_all);
      } else if (arg1.substr(0, type_rpi.length() + 1) == (type_rpi + ":")) {
        show_rpi_version(lam_ssi_rpi_modules, 
                         arg1.substr(type_rpi.length() + 1), scope, type_all);
      } 

      // Version of all cr modules or a specific cr module
      // Note: we only list the compiled crmpi modules. But the list of crmpi 
      // modules available should be the same as this list.
      
      else if (arg1 == type_cr) {
        show_cr_version(lam_ssi_crmpi_modules, type_all, scope, type_all);
      } else if (arg1.substr(0, type_cr.length() + 1) == (type_cr + ":")) {
        show_cr_version(lam_ssi_crmpi_modules,
                        arg1.substr(type_cr.length() + 1), scope, type_all);
      }

      // Fall through
      
      else {
        show_help("laminfo", "usage");
        exit(1);
      }
    }
  }
}


static void
show_lam_version(const string& scope)
{
  out("LAM/MPI", "version:" + type_lam, 
      make_version_str(scope, 
                       LAM_MAJOR_VERSION, LAM_MINOR_VERSION, 
                       LAM_RELEASE_VERSION, 
                       LAM_ALPHA_VERSION, LAM_BETA_VERSION, LAM_SVN_VERSION));
}


static void 
show_boot_version(const lam_ssi_t **boot_modules, 
		  const string& which, const string& scope, 
		  const string& ver_type)
{
  int i;
  const lam_ssi_t *ls;
  bool want_all_modules = (which == type_all);

  for (i = 0; boot_modules[i] != NULL; ++i) {
    ls = boot_modules[i];
    if (!want_all_modules && which != ls->ssi_module_name)
      continue;

    show_ssi_version(ls, type_boot, scope, ver_type);
  }
}


static void 
show_coll_version(const lam_ssi_t **coll_modules, 
		  const string& which, const string& scope, 
		  const string& ver_type)
{
  int i;
  const lam_ssi_t *ls;
  bool want_all_modules = (which == type_all);

  for (i = 0; coll_modules[i] != NULL; ++i) {
    ls = coll_modules[i];
    if (!want_all_modules && which != ls->ssi_module_name)
      continue;

    show_ssi_version(ls, type_coll, scope, ver_type);
  }
}


static void 
show_rpi_version(const lam_ssi_t **rpi_modules, 
		  const string& which, const string& scope, 
		  const string& ver_type)
{
  int i;
  const lam_ssi_t *ls;
  bool want_all_modules = (which == type_all);

  for (i = 0; rpi_modules[i] != NULL; ++i) {
    ls = rpi_modules[i];
    if (!want_all_modules && which != ls->ssi_module_name)
      continue;

    show_ssi_version(ls, type_rpi, scope, ver_type);
  }
}


static void 
show_cr_version(const lam_ssi_t **cr_modules, 
                  const string& which, const string& scope, 
                  const string& ver_type)
{
  int i;
  const lam_ssi_t *ls;
  bool want_all_modules = (which == type_all);

  if (cr_modules != NULL) {
    for (i = 0; cr_modules[i] != NULL; ++i) {
      ls = cr_modules[i];
      if (!want_all_modules && which != ls->ssi_module_name)
        continue;
      
      show_ssi_version(ls, type_cr, scope, ver_type);
    }
  }
}


static void 
show_ssi_version(const lam_ssi_t *ls, const string &type,
		  const string& scope, 
		  const string& ver_type)
{
  bool printed;
  bool want_ssi = (ver_type == type_all || ver_type == ver_ssi);
  bool want_type = (ver_type == type_all || ver_type == ver_type);
  bool want_module = (ver_type == type_all || ver_type == ver_module);
  string message, content;
  string ssi_version;
  string api_version;
  string module_version;
  string empty;

  ssi_version = make_version_str(scope, ls->ssi_major_version,
                                 ls->ssi_minor_version,
                                 ls->ssi_release_version, 0, 0, 0);
  api_version = make_version_str(scope, ls->ssi_kind_major_version,
                                 ls->ssi_kind_minor_version,
                                 ls->ssi_kind_release_version, 0, 0, 0);
  module_version = make_version_str(scope, ls->ssi_module_major_version,
                                    ls->ssi_module_minor_version,
                                    ls->ssi_module_release_version, 0, 0, 0);

  if (pretty) {
    message = "SSI " + type;
    printed = false;

    content = ls->ssi_module_name + string(" (");
    if (want_ssi) {
      content += "SSI v" + ssi_version;
      printed = true;
    }
    if (want_type) {
      if (printed)
        content += ", ";
      content += "API v" + api_version;
      printed = true;
    }
    if (want_module) {
      if (printed)
        content += ", ";
      content += "Module v" + module_version;
      printed = true;
    }
    out(message, empty, content + ")");
  } else {
    message = "ssi:" + type + ":" + ls->ssi_module_name + ":version";
    if (want_ssi)
      out(empty, message, "ssi:" + ssi_version);
    if (want_type)
      out(empty, message, "api:" + api_version);
    if (want_module)
      out(empty, message, "module:" + module_version);
  }
}


static string 
make_version_str(const string& scope,
		 int major, int minor, int release, int alpha, int beta, 
                 int svn)
{
  string str;
  char temp[BUFSIZ];

  temp[BUFSIZ - 1] = '\0';
  if (scope == ver_full) {
    snprintf(temp, BUFSIZ - 1, "%d.%d", major, minor);
    str = temp;
    if (release > 0) {
      snprintf(temp, BUFSIZ - 1, ".%d", release);
      str += temp;
    }
    if (alpha > 0) {
      snprintf(temp, BUFSIZ - 1, "a%d", alpha);
      str += temp;
    }
    else if (beta > 0) {
      snprintf(temp, BUFSIZ - 1, "b%d", beta);
      str += temp;
    }
    if (svn > 0) {
      str += "svn";
      if (svn > 1) {
        snprintf(temp, BUFSIZ - 1, "%d", svn);
        str += temp;
      }
    }
  } else if (scope == ver_major)
    snprintf(temp, BUFSIZ - 1, "%d", major);
  else if (scope == ver_minor)
    snprintf(temp, BUFSIZ - 1, "%d", minor);
  else if (scope == ver_release)
    snprintf(temp, BUFSIZ - 1, "%d", release);
  else if (scope == ver_alpha)
    snprintf(temp, BUFSIZ - 1, "%d", alpha);
  else if (scope == ver_beta)
    snprintf(temp, BUFSIZ - 1, "%d", beta);
  else if (scope == ver_svn)
    snprintf(temp, BUFSIZ - 1, "%d", svn);
  else {
    show_help("laminfo", "usage");
    exit(1);
  }

  if (str.empty())
    str = temp;

  return str;
}


static void
do_params(bool want_all)
{
  int i, count;
  string type, module;

  if (want_all) {
    show_ssi_params(type_all, module_all, param_all);
  } else {
    count = ao_ninsts(ad, "param");
    for (i = 0; i < count; ++i) {
      type = ao_param(ad, "param", i, 0);
      module = ao_param(ad, "param", i, 1);

      if (type != type_all &&
          type != type_base &&
          type != type_boot &&
          type != type_coll &&
          type != type_cr &&
          type != type_rpi) {
        show_help("laminfo", "usage");
        exit(1);
      }

      show_ssi_params(type, module, param_all);
    }
  }
}


static void 
show_ssi_params(const string& type, const string& module, const string& param)
{
  int i, size;
  lam_ssi_base_param_t *array;
  char *default_value_string, temp[BUFSIZ];
  string message, content;

  // Ensure that we've opened the modules (so that they can register
  // their parameters).

  open_modules();

  if (lam_ssi_base_params == NULL)
    return;

  size = lam_arr_size(lam_ssi_base_params);
  array = (lam_ssi_base_param_t*) lam_arr_get(lam_ssi_base_params);

  for (i = 0; i < size; ++i) {
    if (type == type_all || type == array[i].lsbp_type_name) {
      if (module == module_all || 
          (array[i].lsbp_module_name != NULL &&
           module == array[i].lsbp_module_name)) {
        if (param == param_all || param == array[i].lsbp_param_name) {

          // Make a string for the default value

          temp[0] = '\0';
          if (array[i].lsbp_type == LAM_SSI_BASE_PARAM_TYPE_STRING) {
            if (array[i].lsbp_default_value.stringval != NULL)
              default_value_string = array[i].lsbp_default_value.stringval;
            else
              default_value_string = temp;
          } else {
            default_value_string = temp;
            snprintf(default_value_string, BUFSIZ, "%d", 
                     array[i].lsbp_default_value.intval);
          }
          content = default_value_string;

          // Build up the strings to output.

          if (pretty) {
            message = "SSI ";
            message += array[i].lsbp_type_name;

            // Put in the real, full name (which may be different than
            // the categorization).

            content = (array[i].lsbp_env_var_name != NULL) ?
              "parameter \"" : "information \"";
            content += array[i].lsbp_full_name;
            content += (array[i].lsbp_env_var_name != NULL) ?
              "\" (default value: " : "\" (value: ";

            if (strlen(default_value_string) == 0)
              content += "<none>)";
            else {
              content += "\"";
              content += default_value_string;
              content += "\")";
            }

            out(message, message, content);
          } else {
            message = "ssi:";
            message += array[i].lsbp_type_name;
            message += ":";

            if (array[i].lsbp_module_name != NULL) {
              message += array[i].lsbp_module_name;
            } else {
              message += "base";
            }
            message += (array[i].lsbp_env_var_name != NULL) ?
              ":param:" : ":info:";

            // Put in the real, full name (which may be different than
            // the categorization).

            message += array[i].lsbp_full_name;

            content = default_value_string;

            out(message, message, content);
          }
        }
      }
    }
  }
}


static void
do_path(bool want_all, OPT *ad)
{
  int i, count;
  string scope;

  if (want_all) {
    show_path(path_prefix, LAM_PREFIX);
    show_path(path_bindir, LAM_BINDIR);
    show_path(path_libdir, LAM_LIBDIR);
    show_path(path_incdir, LAM_INCDIR);
    show_path(path_pkglibdir, LAM_PKGLIBDIR);
    show_path(path_sysconfdir, LAM_SYSCONFDIR);
  } else {
    count = ao_ninsts(ad, "path");
    for (i = 0; i < count; ++i) {
      scope = ao_param(ad, "path", i, 0);

      if (scope == path_prefix)
        show_path(path_prefix, LAM_PREFIX);
      else if (scope == path_bindir)
        show_path(path_bindir, LAM_BINDIR);
      else if (scope == path_libdir)
        show_path(path_libdir, LAM_LIBDIR);
      else if (scope == path_incdir)
        show_path(path_incdir, LAM_INCDIR);
      else if (scope == path_pkglibdir)
        show_path(path_pkglibdir, LAM_PKGLIBDIR);
      else if (scope == path_sysconfdir)
        show_path(path_sysconfdir, LAM_SYSCONFDIR);
      else {
        show_help("laminfo", "usage");
        exit(1);
      }
    }
  }
}


static void 
show_path(const string& type, const string& value)
{
  string pretty(type);
  pretty[0] &= toupper(pretty[0]);
  out(pretty, "path:" + type, value);
}


static void
do_arch(OPT *ad)
{
  string prefix;
  char hostname[MAXHOSTNAMELEN];

  if (ao_taken(ad, "hostname")) {
    gethostname(hostname, MAXHOSTNAMELEN);
    prefix = hostname + string(":");
  }
  out("Architecture", prefix + "arch", LAM_ARCH);
}


static void 
do_config(bool want_all)
{
  const string cxx(LAM_WANT_MPI2CPP ? "yes" : "no");
  const string fortran(LAM_WANT_FORTRAN ? "yes" : "no");
  const string fortran_symbols(LAM_WANT_FORTRAN ? 
                               (LAM_FORTRANCAPS ? "caps" :
                                (LAM_FORTRANDOUBLEUNDERSCORE ? 
                                 "double_underscore" :
                                 (LAM_FORTRANUNDERSCORE ? "underscore" :
                                  (LAM_FORTRANNOUNDERSCORE ? "plain" :
                                   "unknown")))) : "none");
  const string threads(LAM_HAVE_THREADS ? "yes" : "no");
  const string romio(LAM_WANT_ROMIO ? "yes" : "no");
  const string impi(LAM_WANT_IMPI ? "yes" : "no");
  const string debug(LAM_WANT_DEBUG ? "yes" : "no");
  const string purify(LAM_DISINFECT ? "yes" : "no");
  const string cprofiling(LAM_WANT_PROFILE ? "yes" : "no");
  const string cxxprofiling(LAM_WANT_PROFILE ? "yes" : "no");
  const string fprofiling((LAM_WANT_PROFILE && LAM_WANT_FORTRAN) ?
                          "yes" : "no");
  const string cxxexceptions(LAM_HAVE_CPP_EXCEPTION_SUPPORT ? "yes" : "no");
  const string mem_manager = 
#if LAM_WANT_PTMALLOC 
    "ptmalloc"
#elif LAM_WANT_PTMALLOC2 
    "ptmalloc2"
#elif LAM_WANT_DARWIN7MALLOC 
    "darwin7malloc"
#elif LAM_WANT_EXTERNALMALLOC
    "external"
#else
    "none"
#endif
    ;

  out("Configured by", "config:user", LAM_CONFIGURE_USER);
  out("Configured on", "config:timestamp", LAM_CONFIGURE_DATE);
  out("Configure host", "config:host", LAM_CONFIGURE_HOST);

  out("Memory manager", "memory_manager:", mem_manager);

  out("C bindings", "bindings:c", "yes");
  out("C++ bindings", "bindings:cxx", cxx);
  out("Fortran bindings", "bindings:fortran", fortran);

  out("C compiler", "compiler:c:command", LAM_CC);

  if (want_all) {
    out("C char size", "compiler:c:sizeof:char", sizeof(char));
    out("C bool size", "compiler:c:sizeof:bool", sizeof(bool));
    out("C short size", "compiler:c:sizeof:short", sizeof(short));
    out("C int size", "compiler:c:sizeof:int", sizeof(int));
    out("C long size", "compiler:c:sizeof:long", sizeof(long));
    out("C float size", "compiler:c:sizeof:float", sizeof(float));
    out("C double size", "compiler:c:sizeof:double", sizeof(double));
    out("C pointer size", "compiler:c:sizeof:pointer", sizeof(void *));

    out("C char align", "compiler:c:align:char", LAM_ALIGNMENT_CHAR);
    out("C bool align", "compiler:c:align:bool", LAM_ALIGNMENT_CXX_BOOL);
    out("C int align", "compiler:c:align:int", LAM_ALIGNMENT_INT);
    out("C float align", "compiler:c:align:float", LAM_ALIGNMENT_FLOAT);
    out("C double align", "compiler:c:align:double", 
        LAM_ALIGNMENT_DOUBLE);
  }

  out("C++ compiler", "compiler:cxx:command", LAM_CXX);

  out("Fortran compiler", "compiler:fortran:command", LAM_F77);
  out("Fortran symbols", "compiler:fortran:symbols", fortran_symbols);
  if (want_all) {
    out("Fort integer size", "compiler:fortran:sizeof:integer", 
        LAM_SIZEOF_FORTRAN_INT);
    out("Fort real size", "compiler:fortran:sizeof:real", 
        LAM_SIZEOF_FORTRAN_REAL);
    out("Fort dbl prec size", 
        "compiler:fortran:sizeof:double_precision", LAM_SIZEOF_FORTRAN_REAL);
    out("Fort cplx size", "compiler:fortran:sizeof:complex", 
        LAM_SIZEOF_FORTRAN_REAL);
    out("Fort dbl cplx size",
        "compiler:fortran:sizeof:double_complex", 
        LAM_SIZEOF_FORTRAN_REAL);

    out("Fort integer align", "compiler:fortran:align:integer", 
        LAM_ALIGNMENT_FORTRAN_INT);
    out("Fort real align", "compiler:fortran:align:real", 
        LAM_ALIGNMENT_FORTRAN_REAL);
    out("Fort dbl prec align", 
        "compiler:fortran:align:double_precision", LAM_ALIGNMENT_FORTRAN_REAL);
    out("Fort cplx align", "compiler:fortran:align:complex", 
        LAM_ALIGNMENT_FORTRAN_REAL);
    out("Fort dbl cplx align",
        "compiler:fortran:align:double_complex", 
        LAM_ALIGNMENT_FORTRAN_REAL);
  }

  out("C profiling", "option:profiling:c", cprofiling);
  out("C++ profiling", "option:profiling:cxx", cxxprofiling);
  out("Fortran profiling", "option:profiling:fortran", fprofiling);

  out("C++ exceptions", "option:cxx_exceptions", cxxexceptions);
  out("Thread support", "option:threads", threads);
  out("ROMIO support", "option:romio", romio);
  out("IMPI support", "option:impi", impi);
  out("Debug support", "option:debug", debug);
  out("Purify clean", "option:purify", purify);
}


static void
out(const string& pretty_message, const string &plain_message,
    const string& value)
{
  if (pretty) {
    string::size_type pos, max_value_width;
    string spaces(LAM_max(centerpoint - pretty_message.length(), 0), ' ');
    string v = value;
    string filler;

    max_value_width = screen_width - spaces.length() -
      pretty_message.length() - 2;
    filler = spaces + pretty_message + ": ";

    while (1) {
      if (v.length() < max_value_width) {
        cout << filler << v << endl;
        break;
      } else {
        string spaces(centerpoint + 2, ' ');

        // Work backwards to find the first space before
        // max_value_width

        pos = v.rfind(' ', max_value_width);
        if (pos == string::npos) {

          // No space found < max_value_width.  Look for the first
          // space after max_value_width.

          pos = v.find(' ', max_value_width);
          if (pos == string::npos) {

            // There's just no spaces.  So just print it and be done.

            cout << filler << v << endl;
            break;
          } else {
            cout << filler << v.substr(0, pos) << endl;
            v = v.substr(pos + 1);
          }
        } else {
          cout << filler << v.substr(0, pos) << endl;
          v = v.substr(pos + 1);
        }

        // Reset for the next iteration

        filler = spaces;
      }
    }
  } else {
    cout << plain_message << ":" << value << endl;
  }
}


static void
out(const string& pretty_message, const string &plain_message, int value)
{
  if (pretty) {
    string spaces(LAM_max(centerpoint - pretty_message.length(), 0), ' ');
    cout << spaces << pretty_message << ": " << value << endl;
  } else {
    cout << plain_message << ":" << value << endl;
  }
}


//
// Open all SSI modules so that they can register their SSI
// parameters.  Take a shotgun approach here and indiscriminately open
// all modules -- don't be selective.  To this end, we need to clear
// out the environment of all LAM_MPI_ssi_<type> variables to ensure
// that the open algorithms don't try to only open one module.
//
static void
open_modules()
{
  if (opened_modules)
    return;

  // Clear out the environment

  if (getenv("LAM_MPI_ssi_boot") != NULL)
    putenv((char *) "LAM_MPI_ssi_boot=");
  if (getenv("LAM_MPI_ssi_coll") != NULL)
    putenv((char *) "LAM_MPI_ssi_coll=");
  if (getenv("LAM_MPI_ssi_cr") != NULL)
    putenv((char *) "LAM_MPI_ssi_cr=");
  if (getenv("LAM_MPI_ssi_rpi") != NULL)
    putenv((char *) "LAM_MPI_ssi_rpi=");

  // Open all modules

  lam_ssi_base_open(ad);
  lam_ssi_boot_open(ad);
  lam_ssi_rpi_base_open(ad);
  lam_ssi_coll_base_open(ad);
  lam_ssi_crmpi_base_open(ad);

  // All done

  opened_modules = true;
}


static void
close_modules()
{
  if (opened_modules) {
    lam_ssi_crmpi_base_close();
    lam_ssi_coll_base_close();
    lam_ssi_rpi_base_close();
    lam_ssi_boot_close();
    lam_ssi_base_close();
  }
}
