#ifndef __kany_h__
#define __kany_h__

#include <CORBA.h>

#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif

std::ostream& operator<<( std::ostream& out, CORBA::Any& any );

bool printAny( std::ostream& out, CORBA::Any& _any, CORBA::TypeCode_ptr _tc );

#endif
