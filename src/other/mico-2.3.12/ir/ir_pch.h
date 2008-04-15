//
// This file aims to speed-up PCH while using Comeau C/C++ compiler
//

#include <CORBA.h>
#include <mico/ir_creator.h>
#include <ctype.h>
#include "ir_impl.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <mico/os-misc.h>
#include <parser.h>
#include <idlparser.h>
#include <codegen-idl.h>
#include <params.h>
