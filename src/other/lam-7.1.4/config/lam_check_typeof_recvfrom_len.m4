dnl -*- shell-script -*-
dnl
dnl Copyright (c) 2001-2003 The Trustees of Indiana University.  
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
dnl $Id: lam_check_typeof_recvfrom_len.m4,v 1.6 2003/07/14 23:31:14 jsquyres Exp $
dnl

define(LAM_CHECK_TYPEOF_RECVFROM_LEN,[
# Determine type of optlen parameter of getsockopt()
AC_MSG_CHECKING(type of recvfrom length parameter)

try_getsockopt_optlen() {
    cat > conftest.c <<EOF
#include <sys/types.h>  
#include <sys/socket.h> 
int foobar(void *buffer)
{
    $type optlen=0;
    return recvfrom(0, buffer, 0, 0, (struct sockaddr*) 0, &optlen);
}
EOF
    cat conftest.c >&5
    $CC $CFLAGS -c conftest.c 1> conftest.out 2>&1
    cat conftest.out >&5
    if test -f conftest.o; then
	foo="wc conftest.out | awk '{ print $"
	foo="${foo}1 }'"
	ret=`eval $foo`
    else
	ret=1
    fi
    rm -f conftest.*
}

# Extra quoting around [ and ] required on the sed arguments to escape
# m4's [silent] wrath

CFLAGS_save="$CFLAGS"
CFLAGS="`echo $CFLAGS | sed -e 's/-v$//' -e 's/-v[[ \t]]/ /'`"
types="socklen_t int size_t unsigned-int long unsigned-long"
found=0
for type in $types; do
    type=`echo $type | sed 's/-/ /g'`
    try_getsockopt_optlen
    if test "$ret" = "0"; then
	found=1
        break
    fi
done
CFLAGS="$CFLAGS_save"

if test "$found" = 1; then
    AC_MSG_RESULT([$type])
else
    AC_MSG_RESULT([not found -- using "unsigned int" and hoping for the best])
    type="unsigned int"
fi

AC_DEFINE_UNQUOTED(LAM_RECVFROM_LEN_T, $type,
    [Type of argument that recvfrom() requires])])dnl
