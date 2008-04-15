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
dnl $Id: lam_get_alignment.m4,v 1.8 2002/10/09 20:55:12 brbarret Exp $
dnl

define(LAM_GET_ALIGNMENT,[
# Determine datatype alignment. 
# First arg is type, 2nd (optional) arg is config var to define.
AC_MSG_CHECKING([alignment of $1])
AC_TRY_RUN([
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct foo { char c; $1 x; };
int main(int argc, char* argv[])
{
    struct foo *p = (struct foo *) malloc(sizeof(struct foo));
    int diff;
    FILE *f=fopen("conftestval", "w");
    if (!f) exit(1);
    diff = ((char *)&p->x) - ((char *)&p->c);
    fprintf(f, "%d\n", (diff >= 0) ? diff : -diff);
    return 0;
}],[lam_ac_align=`cat conftestval`],[lam_ac_align=-1],[lam_ac_align=-1])

if test "`expr $lam_ac_align \<= 0`" = "1"; then
    AC_MSG_WARN([*** Problem running configure test!])
    AC_MSG_WARN([*** See config.log for details.])
    AC_MSG_ERROR([*** Cannot continue.])
fi

AC_MSG_RESULT([$lam_ac_align])
if test -n "$2"; then
    AC_DEFINE_UNQUOTED($2, $lam_ac_align, [Alignment of type $1])
    eval "$2=$lam_ac_align"
fi
unset lam_ac_align

/bin/rm -f conftest*])dnl



define(LAM_CXX_GET_ALIGNMENT,[
AC_LANG_SAVE
AC_LANG_CPLUSPLUS
LAM_GET_ALIGNMENT($1,$2)
AC_LANG_RESTORE])
