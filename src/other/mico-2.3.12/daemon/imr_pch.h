//
// This file aims to speed-up PCH while using Comeau C/C++ compiler
//

#include <CORBA.h>
#include <mico/util.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iomanip>
#else
#include <iomanip.h>
#endif
#include "poamediator.h"
#include "imr_impl.h"
