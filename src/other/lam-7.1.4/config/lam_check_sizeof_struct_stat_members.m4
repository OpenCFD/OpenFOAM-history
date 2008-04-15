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
dnl $Id: lam_check_sizeof_struct_stat_members.m4,v 1.4 2002/10/09 20:55:11 brbarret Exp $
dnl

define(LAM_CHECK_SIZEOF_STRUCT_STAT_MEMBERS,[
# Determine size of members of struct stat
AC_MSG_CHECKING(size of struct stat members)
cat > conftest.c <<EOF
#include <sys/stat.h>
#include <stdio.h>
main()
{
    struct stat foo;
    FILE *f=fopen("conftestval", "w");
    if (!f) exit(1);
    fprintf(f, "%d\n", sizeof(foo.st_dev));
    exit(0);
}
EOF
$CC $CFLAGS -o conftest conftest.c 1>&5 2>&1
if test -s conftest && (./conftest; exit) 2>/dev/null; then
    lam_stat_size=`cat conftestval`
else
    lam_stat_size=0
fi
AC_MSG_RESULT($lam_stat_size)
AH_TEMPLATE([LAM_STAT_MEMBERS_INT4], 
    [Whether members of the stat structure are int4 or not])
if test "$lam_stat_size" = "4"; then
    AC_DEFINE(LAM_STAT_MEMBERS_INT4, 1)
else
    AC_DEFINE(LAM_STAT_MEMBERS_INT4, 0)
fi
/bin/rm -f conftest*])dnl
