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
dnl $Id: lam_check_prototype.m4,v 1.5 2002/10/09 20:55:11 brbarret Exp $
dnl

define([LAM_CHECK_PROTOTYPE],[
AC_MSG_CHECKING([for prototype of $1])
func=$1

rm -f conftest.c
cat > conftest.c <<EOF
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <strings.h>
#include <netdb.h>
#include <unistd.h>

int main(int argc, char **argv)
{ return 0; }
EOF

LAM_LOG_MSG([$CPP $CFLAGS conftest.c], 1)
$CPP $CPPFLAGS conftest.c > conftest.out 2>&5
lam_status=$?
LAM_LOG_MSG([\$? = $lam_status], 1)

LAM_LOG_MSG([...some ugly egrepping...], 1)
e1="egrep '[[^A-Za-z0-9_]]$func[[	 ]]*\(' conftest.out"
lam_status=$?
LAM_LOG_MSG([\$? = $lam_status], 1)

LAM_LOG_MSG([...some ugly egrepping...], 1)
e2="egrep '^$func[[	 ]]*\(' conftest.out"
lam_status=$?
LAM_LOG_MSG([\$? = $lam_status], 1)

AH_TEMPLATE([LAM_HAVE_PROTO_$1], 
    [Whether we have a prototype for the $func() function or not])
if test "`eval $e1`" != "" -o "`eval $e2`" != ""; then
    AC_DEFINE_UNQUOTED(LAM_HAVE_PROTO_$1, 1)
    AC_MSG_RESULT([yes])
else
    echo configure:__oline__: here is the program that failed: >&5
    cat conftest.c >&5
    AC_DEFINE_UNQUOTED(LAM_HAVE_PROTO_$1, 0)
    AC_MSG_RESULT([no])
fi
rm -f conftest*])dnl
