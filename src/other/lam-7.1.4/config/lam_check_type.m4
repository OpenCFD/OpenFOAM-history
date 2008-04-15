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
dnl $Id: lam_check_type.m4,v 1.5 2002/10/09 20:55:11 brbarret Exp $
dnl

define(LAM_CHECK_TYPE,[
# Autoconfs AC_CHECK_TYPE is not sufficient
# This, too, is not foolproof, but will have to do...
AC_MSG_CHECKING(for type $1)
cat > conftest.$ac_ext <<EOF
#include "confdefs.h"
#include <sys/types.h>
#if STDC_HEADERS
#include <stdlib.h>
#include <stddef.h>
#endif
EOF

touch conftest.out >&5 2>&5
rm -f conftest.out >&5 2>&5

LAM_LOG_MSG([$ac_cpp conftest.$ac_ext], 1)
eval "$ac_cpp conftest.$ac_ext" 2>&5 > conftest.out
lam_status=$?
LAM_LOG_MSG([\$? = $lam_status], 1)

LAM_LOG_MSG([...some ugly egrepping...], 1)
egrep "$1[[^a-zA-Z_0-9]]" conftest.out > conftest.out2 2>&5
lam_status=$?
LAM_LOG_MSG([\$? = $lam_status], 1)

AH_TEMPLATE([LAM_HAVE_$1], [Whether we have $1 or not])
if test -s "conftest.out2"; then
  AC_DEFINE(LAM_HAVE_$1, 1)
  AC_MSG_RESULT([yes])
else
  AC_DEFINE(LAM_HAVE_$1, 0)
  AC_MSG_RESULT([no])
fi

unset lam_status
rm -rf conftest*])dnl
