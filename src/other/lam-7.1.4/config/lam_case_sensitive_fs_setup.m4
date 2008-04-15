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
dnl $Id: lam_case_sensitive_fs_setup.m4,v 1.3 2003/08/01 05:42:57 brbarret Exp $
dnl

define([LAM_CASE_SENSITIVE_FS_SETUP],[
#
# Arguments: none
#
# Dependencies: None
#
# See if we are on a case sensitive filesystem.  Some filesystems
# (like HFS+ on MacOS X and MS Windows) are not case sensitive - mpicc
# and mpiCC are the same file.
#
# Sets prefix_OS_HAVE_CASE_SENSITIVE_FS to 1 if filesystem is case
# sensitive (ie, mpicc and mpiCC will be different files) or 0 if
# filesystem is not case sensitive (ie, mpicc and mpiCC will be
# the same file).
#
#

have_cs_fs=1

AC_MSG_CHECKING([if build filesystem is case sensitive])
cat > conf_fs_test.$$ <<EOF
lowercase
EOF

cat > CONF_FS_TEST.$$ <<EOF
uppercase
EOF

if test "`cat conf_fs_test.$$`" = "lowercase"; then
    have_cs_fs=1
    AC_MSG_RESULT([yes])
else
    have_cs_fs=0
    AC_MSG_RESULT([no])
fi

rm -f conf_fs_test.$$ CONF_FS_TEST.$$

#
# Now see what the user wants to do...
#
AC_MSG_CHECKING([if configuring for case sensitive fs])
AC_ARG_WITH(cs_fs, [  --with-cs-fs            Destination FS is case sensitive
                          (specify --without-cs-fs if installing on HFS+
                           on MacOS X)])

if test "$with_cs_fs" = "yes"; then
    LAM_WANT_CS_FS=1
elif test -z "$with_cs_fs"; then
    LAM_WANT_CS_FS=$have_cs_fs
else
    LAM_WANT_CS_FS=0
fi

if test "$LAM_WANT_CS_FS" = "1"; then
    AC_MSG_RESULT([yes])
else
    AC_MSG_RESULT([no])
fi

AM_CONDITIONAL(CASE_SENSITIVE_FS, test "$LAM_WANT_CS_FS" = "1")

if test "$LAM_WANT_CS_FS" = "0"; then
	cat <<EOF

*******************************************************************************
NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE
*******************************************************************************

Because LAM/MPI is being installed on a non-case sensitive file system, the
C++ wrapper compiler will be named mpic++ instead of the traditional mpiCC.

Please update any makefiles appropriately.

*******************************************************************************
NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE
*******************************************************************************

EOF
	sleep 5
fi

# Clean up
unset have_cs_fs])dnl
