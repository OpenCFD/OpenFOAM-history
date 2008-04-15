//
// This file aims to speed-up PCH while using Comeau C/C++ compiler
//

#include <CORBA.h>
#include <ctype.h>
#include <stdio.h>
#include <mico/template_impl.h>
#include <mico/util.h>
#include <mico/ir_creator.h>
//#include <mico/os-misc.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <sstream>
#else
#include <strstream.h>
#endif
#ifdef USE_CCM
#include <mico/ir3.h>
#else
#include <mico/ir.h>
#endif

// header for GUIDS
#if defined(_WIN32) && !defined(__MINGW32__)
#include<objbase.h>
#endif

#include "codegen-c++-common.h"
#include "codegen-c++-impl.h"
#include "codegen-c++-stub.h"
#include "codegen-c++-skel.h"
#include "codegen-c++-util.h"
#include "codegen-c++.h"
#include "codegen-idl.h"
#include "codegen-midl.h"
#include "codegen.h"
#include "const.h"
#include "error.h"
#include "dep.h"
#include "idlparser.h"
#include "params.h"
#include "parsenode.h"
#include "prepro.h"
#include "parser.h"
#include "db.h"
#include "keymap.h"
#include "codegen-wsdl.h"

