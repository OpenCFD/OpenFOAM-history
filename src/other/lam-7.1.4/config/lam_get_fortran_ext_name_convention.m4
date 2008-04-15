dnl -*- shell-script -*-
dnl
dnl Copyright (c) 2001-2002 The Trustees of Indiana University.  
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
dnl $Id: lam_get_fortran_ext_name_convention.m4,v 1.4 2002/10/09 20:55:12 brbarret Exp $
dnl

define(LAM_GET_FORTRAN_EXT_NAME_CONVENTION,[
# Determine underscore generation for external names.
# arg is FORTRAN compiler name
AC_MSG_CHECKING($1 external naming convention)
cat > conftestf.f <<EOF
       subroutine FOO_bar(a)
       integer a
       a = 1
       return
       end
EOF
$1 $FFLAGS -c conftestf.f 1>&5 2>&1
if test ! -s conftestf.o; then
    AC_MSG_WARN(unable to produce an object file testing FORTRAN compiler)
else
    nm conftestf.o | grep foo_bar__ > /dev/null 2>&1
    if test $? = "0"; then 
	AC_MSG_RESULT(FORTRANDOUBLEUNDERSCORE)
	lam_fortran_double_underscore=1
	lam_ac_doubleunder=y
    else
	nm conftestf.o | grep foo_bar_ > /dev/null 2>&1
	if test $? = "0"; then 
	    AC_MSG_RESULT(FORTRANUNDERSCORE)
	    lam_fortran_underscore=1
	    lam_ac_singleunder=y
	else
	    # We may get into trouble here if we start accepting mixed
	    # case compilers -- we may need to have caps underscore,
	    # or caps double underscore, for example.  But we haven't
	    # found any that require that yet.  :-)
	    nm conftestf.o | grep FOO_bar > /dev/null 2>&1
	    if test $? = "0"; then 
		AC_MSG_RESULT([mixed case, so FORTRANCAPS])
		lam_fortran_caps=1
		lam_ac_caps=y
	    else
		nm conftestf.o | grep foo_bar > /dev/null 2>&1
		if test $? = "0"; then 
		    AC_MSG_RESULT(FORTRANNOUNDERSCORE)
		    lam_fortran_no_underscore=1
		    lam_ac_nounder=y
		else
		    nm conftestf.o | grep FOO_BAR > /dev/null 2>&1
		    if test $? = "0"; then 
			AC_MSG_RESULT(FORTRANCAPS)
			lam_fortran_caps=1
			lam_ac_caps=y
		    else
			AC_MSG_WARN(could not find name of subroutine foo_bar)
		    fi
		fi
	    fi
	fi
    fi
fi

/bin/rm -f conftestf.f conftestf.o])dnl
dnl #$%#% emacs shell-script mode: '
