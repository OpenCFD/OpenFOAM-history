dnl -*- shell-script -*-
dnl
dnl Copyright (c) 2001-2004 The Trustees of Indiana University.  
dnl                         All rights reserved.
dnl Copyright (c) 1998-2001 University of Notre Dame. 
dnl                         All rights reserved.
dnl Copyright (c) 1994-1998 The Ohio State University.  
dnl                         All rights reserved.
dnl 
dnl This file is part of the LAM/MPI software package.  For license
dnl information, see the LICENSE file in the top level directory of the
dnl LAM/MPI source distribution.
dnl
dnl $Id: lam_setup_cxx.m4,v 1.7 2004/01/20 03:41:47 jsquyres Exp $
dnl

AC_DEFUN([LAM_SETUP_CXX],[

# Modularize this setup so that sub-configure.in scripts can use this
# same setup code.

lam_show_subtitle "C++ compiler and preprocessor" 

#
# check for specification of C++ compiler
#

AC_ARG_WITH(cxx, 
    AC_HELP_STRING([--with-cxx=CXX],
		 [DEPRECATED: use the env variable CXX instead]),
  [AC_MSG_WARN([The --with-cxxflags flag is deprecated.  Use the CXX])
   AC_MSG_WARN([environment variable instead.])
   AC_MSG_ERROR([Cannot continue])])
if test "$with_cxx" = no; then
    AC_MSG_WARN([*** LAM/MPI now requires a C++ compiler.])
    AC_MSG_WARN([*** Specify --without-mpi2cpp to build without the MPI 2 C++ bindings])
    AC_MSG_ERROR([*** Cannot continue.])
fi

#
# check for specification of C++ compiler flags
#

AC_ARG_WITH(cxxflags, 
  AC_HELP_STRING([--with-cxxflags],
		 [DEPRECATED: use the env variable CXXFLAGS instead]),
  [AC_MSG_WARN([The --with-cxxflags flag is deprecated.  Use the CXXFLAGS])
   AC_MSG_WARN([environment variable instead.])
   AC_MSG_ERROR([Cannot continue])])

#
# check for specification of C++ LD linker flags
#

AC_ARG_WITH(cxxldflags, 
  AC_HELP_STRING([--with-cxxldflags],
		 [DEPRECATED: use the env variable CXXLDFLAGS instead]),
  [AC_MSG_WARN([The --with-cxxldflags flag is deprecated.  Use the])
   AC_MSG_WARN([CXXLDFLAGS environment variable instead.])
   AC_MSG_ERROR([Cannot continue])])
AC_ARG_VAR([CXXLDFLAGS], [C++ linker flags])


lam_cxxflags_save="$CXXFLAGS"
AC_PROG_CXX
AC_PROG_CXXCPP
BASECXX="`basename $CXX`"
CXXFLAGS="$lam_cxxflags_save"
AC_DEFINE_UNQUOTED(LAM_CXX, "$CXX", [LAM underlying C++ compiler])

# Do we want debugging?
if test "$WANT_DEBUG" = "1"; then
    if test "$GXX" = "yes"; then
	add="-g -Wall -Wundef -Wno-long-long"

        # see if -Wno-long-double works...
        CXXFLAGS_orig="$CXXFLAGS"
        CXXFLAGS="$CXXFLAGS -Wno-long-double"
	AC_LANG_PUSH(C++)
        AC_TRY_COMPILE([], [], add="$add -Wno-long-double")
	AC_LANG_POP
        CXXFLAGS="$CXXFLAGS_orig"
    else
	add="-g"
    fi
    CXXFLAGS="$CXXFLAGS $add"
    LAM_UNIQ(CXXFLAGS)
    AC_MSG_WARN([$add has been added to CXXFLAGS (--with-debug)])
    unset add
fi

# Check for special things due to C++ exceptions

WRAPPER_EXTRA_CFLAGS=
WRAPPER_EXTRA_FFLAGS=
WRAPPER_EXTRA_CXXFLAGS=
WRAPPER_EXTRA_LDFLAGS=
WRAPPER_EXTRA_LIBS=
WANT_CPPEXCEPTIONS=no
HAVE_CPPEXCEPTIONS=0
AC_ARG_WITH(exceptions, 
  AC_HELP_STRING([--with-exceptions],
		 [enable support for C++ exceptions]),
  [WANT_CPPEXCEPTIONS="$withval"])

AC_MSG_CHECKING([if want C++ exception handling])
AC_MSG_RESULT([$WANT_CPPEXCEPTIONS])
if test "$WANT_CPPEXCEPTIONS" = "yes"; then
    # config/cxx_have_exceptions.m4
    LAM_CXX_HAVE_EXCEPTIONS
    # config/cxx_find_exception_flags.m4
    LAM_CXX_FIND_EXCEPTION_FLAGS
    if test "$LAM_CXX_EXCEPTIONS" = "1"; then
	HAVE_CPPEXCEPTIONS=1
	CFLAGS="$CFLAGS $LAM_CXX_EXCEPTIONS_CXXFLAGS"
	FFLAGS="$FFLAGS $LAM_CXX_EXCEPTIONS_CXXFLAGS"
	CXXFLAGS="$CXXFLAGS $LAM_CXX_EXCEPTIONS_CXXFLAGS"
	LDFLAGS="$LDFLAGS $LAM_CXX_EXCEPTIONS_LDFLAGS"

	WRAPPER_EXTRA_CFLAGS="$LAM_CXX_EXCEPTIONS_CXXFLAGS"
	WRAPPER_EXTRA_FFLAGS="$LAM_CXX_EXCEPTIONS_CXXFLAGS"
	WRAPPER_EXTRA_CXXFLAGS="$LAM_CXX_EXCEPTIONS_CXXFLAGS"
    fi
fi
AC_DEFINE_UNQUOTED(LAM_HAVE_CPP_EXCEPTION_SUPPORT, $HAVE_CPPEXCEPTIONS,
    [Whether or not we have compiled with C++ exceptions support])

# Do we have std::nothrow?
# config/cxx_have_nothrow_new.m4
LAM_CXX_HAVE_NOTHROW_NEW

# Find some more characteristics of the C++ compiler

# config/cxx_find_template_repository.m4
LSC_CXX_FIND_TEMPLATE_REPOSITORY(LAM)
# config/cxx_find_template_parameters.m4
LAM_CXX_FIND_TEMPLATE_PARAMETERS

# If we are on HP-UX, ensure that we're using aCC
case "$host" in
*hpux*)
    if test "$BASECXX" = "CC"; then
	AC_MSG_WARN([*** You will probably have problems compiling the MPI 2])
	AC_MSG_WARN([*** C++ bindings with the HP-UX CC compiler.  You should])
	AC_MSG_WARN([*** probably be using the aCC compiler.  Re-run configure])
	AC_MSG_WARN([*** with "--with-cxx=aCC".])
    fi
    ;;
esac

# Same rationale for g++ as with gcc, above.
if test "$GXX" = yes; then
    OPTFLAGS="-O3"
else
    OPTFLAGS="-O"
fi
# config/lam_check_optflags.m4
LAM_CHECK_OPTFLAGS("$CXXFLAGS")
AC_MSG_CHECKING([for C++ optimization flags])
AC_MSG_RESULT([$co_result])
CXXFLAGS="$co_result"
])
