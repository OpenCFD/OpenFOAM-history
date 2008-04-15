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
dnl $Id: lam_check_stdc.m4,v 1.4 2002/10/09 20:55:11 brbarret Exp $
dnl

define(LAM_CHECK_STDC,[
# Check to see if compiler sets __STDC__ to 1 or not
AC_MSG_CHECKING(for __STDC__)
cat > conftest.c <<EOF
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
void exit(int);
#endif
int main(int argc, char* argv[])
{
    FILE *f=fopen("conftestval", "w");
#ifndef __STDC__
    int val = -1;
#else
    int val = __STDC__;
#endif
    if (!f) exit(1);
    fprintf(f, "%d\n", val);
    return 0;
}
EOF
LAM_LOG_COMMAND([$CC $CFLAGS -o conftest conftest.c],
    LAM_LOG_COMMAND([./conftest],
	[MSG=yes lam_ac_stdc=`cat conftestval`],[MSG=no lam_ac_stdc=-1]),
    [MSG=no lam_ac_stdc=-1])
AC_MSG_RESULT([$MSG])

if test "$lam_ac_stdc" = "0"; then
    AC_MSG_WARN([*** Compiler did not set __STDC__ to 1!])
    AC_MSG_WARN([*** Be warned -- this will cause user MPI programs to])
    AC_MSG_WARN([*** not get proper prototypes of MPI functions!])
elif test "$lam_ac_stdc" = "-1"; then
    AC_MSG_WARN([*** Compiler did not set __STDC__!])
    AC_MSG_WARN([*** Be warned -- this will cause user MPI programs to])
    AC_MSG_WARN([*** not get proper prototypes of MPI functions!])
fi

unset lam_ac_stdc
/bin/rm -f conftest*])dnl

