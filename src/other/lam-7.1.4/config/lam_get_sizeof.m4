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
dnl $Id: lam_get_sizeof.m4,v 1.8 2002/10/09 20:55:12 brbarret Exp $
dnl

define(LAM_GET_SIZEOF,[
# Determine datatype size. 
# First arg is type, 2nd (optional) arg is config var to define.
AC_MSG_CHECKING(size of $1)
AC_TRY_RUN([
#include <stddef.h>
#include <stdio.h>
int main(int argc, char** argv)
{
    FILE *f=fopen("conftestval", "w");
    if (!f) return 1;
    fprintf(f, "%d\n", sizeof($1));
    return 0;
}],[lam_ac_size=`cat conftestval`],[lam_ac_size=-1],[lam_ac_size=-1])

AC_MSG_RESULT([$lam_ac_size])
if test "`expr $lam_ac_size \<= 0`" = "1"; then
    AC_MSG_WARN([*** Problem running configure test!])
    AC_MSG_WARN([*** See config.log for details.])
    AC_MSG_ERROR([*** Cannot continue.])
fi

if test -n "$2"; then
    tmp_string="$2=$lam_ac_size"
    eval $tmp_string

    AC_DEFINE_UNQUOTED($2, $lam_ac_size, [Size of C type $1])
fi
unset lam_ac_size

/bin/rm -f conftest*])dnl


define(LAM_CXX_GET_SIZEOF,[
AC_LANG_SAVE
AC_LANG_CPLUSPLUS
LAM_GET_SIZEOF($1,$2)
AC_LANG_RESTORE])
