//
// This file aims to speed-up MICO compilation
// while using precompiled-headers
//

#include <CORBA.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#ifdef HAVE_DLFCN_H
#include <dlfcn.h>
#endif
#ifdef HAVE_DL_H
#include <dl.h>
#endif
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

#ifndef _WIN32
#include <sys/wait.h>
#endif

#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <iomanip>
#include <sstream>
#else
#include <iostream.h>
#include <iomanip.h>
#include <strstream.h>
#endif

#include <ctype.h>

#include <mico/os-net.h>
#include <mico/impl.h>
#include <mico/util.h>
#include <mico/throw.h>
#include <mico/os-math.h>
#include <mico/os-misc.h>
#include <mico/template_impl.h>
#include <mico/intercept.h>
#include <mico/basic_seq.h>
#include <mico/ssl.h>
#include <mico/poa_impl.h>
#include <mico/dynany_impl.h>
#include <mico/pi_impl.h>

#include <mico/code_set_registry.h>

#ifdef USE_CSL2
#  include <mico/security/securitylevel1.h>
#  include <mico/security/securitylevel2_impl.h>
#  include <mico/security/AuditIntercept.h>
#  include <mico/security/odm_impl.h>
#  include <mico/security/DomainManager_impl.h>
#endif // USE_CSL2

#ifdef USE_CSIV2
#include <mico/security/csiv2_impl.h>
#endif // USE_CSIV2

#if defined (_WIN32) || defined (_POCKET_PC)
#include <mico/process_impl.h>
#endif
#include <mico/throw.h>
#include <algorithm>
#ifdef USE_MEMTRACE
#include <mico/memtrace.h>
#endif
#if defined(HAVE_BACKTRACE) && defined(HAVE_BACKTRACE_SYMBOLS)
#include <execinfo.h>
#endif // HAVE_BACKTRACE && HAVE_BACKTRACE_SYMBOLS

