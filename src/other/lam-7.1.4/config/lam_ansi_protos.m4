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
dnl $Id: lam_ansi_protos.m4,v 1.3 2002/10/09 20:55:10 brbarret Exp $
dnl

define(LAM_ANSI_PROTOS,[
# Determine if compiler and options accepts ANSI prototypes.
AC_MSG_CHECKING([whether the C compiler ($CC $CFLAGS) accepts ANSI prototypes])
cat > conftest.c <<EOF
static double foo(double bar) { return(2.0*bar); }
int main(int argc, char **argv)
{ double x; x = foo(2.0); return(0); }
EOF
$CC $CFLAGS -o conftest conftest.c 1>&5 2>&1
if test -s conftest && (./conftest; exit) 2>/dev/null; then
    lam_ac_ansi_protos=yes
else
    lam_ac_ansi_protos=no
fi
AC_MSG_RESULT($lam_ac_ansi_protos)
/bin/rm -f conftest*])dnl

