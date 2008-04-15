dnl -*- shell-script -*-
dnl
dnl Copyright (c) 2001-2004 The Trustees of Indiana University.  
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
dnl $Id: lam_functions.m4,v 1.17 2004/01/20 03:41:47 jsquyres Exp $
dnl

AC_DEFUN([LAM_CONFIGURE_SETUP],[

# Some helper script functions.  Unfortunately, we cannot use $1 kinds
# of arugments here because of the m4 substitution.  So we have to set
# special variable names before invoking the function.  :-\

lam_show_title() {
  cat <<EOF

============================================================================
== ${1}
============================================================================
EOF
}


lam_show_subtitle() {
  cat <<EOF

*** ${1}
EOF
}])

dnl #######################################################################

AC_DEFUN([LAM_BASIC_SETUP],[
#
# Save some stats about this build
#

LAM_CONFIGURE_USER="`whoami`"
LAM_CONFIGURE_HOST="`hostname | head -n 1`"
LAM_CONFIGURE_DATE="`date`"

#
# Make automake clean emacs ~ files for "make clean"
#

CLEANFILES="*~"
AC_SUBST(CLEANFILES)

#
# This is useful later
#

AC_CANONICAL_HOST

#
# See if we can find an old installation of LAM to overwrite
#

# Stupid autoconf 2.54 has a bug in AC_PREFIX_PROGRAM -- if lamclean
# is not found in the path and the user did not specify --prefix,
# we'll get a $prefix of "."

lam_prefix_save="$prefix"
AC_PREFIX_PROGRAM(lamclean)
if test "$prefix" = "."; then
    prefix="$lam_prefix_save"
fi
unset lam_prefix_save

#
# Basic sanity checking; we can't install to a relative path
#

case "$prefix" in
  /*/bin)
    prefix="`dirname $prefix`"
    LAM_PREFIX="$prefix"
    ;;
  /*) 
    LAM_PREFIX="$prefix"
    ;;
  NONE)
    LAM_PREFIX="$ac_default_prefix"
    ;;
  *) 
    AC_MSG_ERROR(prefix \"$prefix\" must be an absolute directory path) 
    ;;
esac
echo installing to directory \"$LAM_PREFIX\" 

#
# Figure out exec_prefix as well
#

case "$exec_prefix" in
  /*/bin)
    exec_prefix="`dirname $exec_prefix`"
    LAM_EXEC_PREFIX="$exec_prefix"
    ;;
  /*) 
    LAM_EXEC_PREFIX="$exec_prefix"
    ;;
  NONE)
    LAM_EXEC_PREFIX="$LAM_PREFIX"
    ;;
  *) 
    AC_MSG_ERROR(prefix \"$exec_prefix\" must be an absolute directory path) 
    ;;
esac

#
# Calculate bindir and mandir based on this
# (for use in the lam_module.tcl modulefile and shell script setup files)
#

if test "$bindir" = '${exec_prefix}/bin'; then
    LAM_BINDIR="$LAM_EXEC_PREFIX/bin";
else
    LAM_BINDIR="$bindir";
fi
if test "$mandir" = '${prefix}/man'; then
    LAM_MANDIR="$LAM_PREFIX/man";
else
    LAM_MANDIR="$mandir";
fi
AC_SUBST(LAM_PREFIX)
AC_SUBST(LAM_EXEC_PREFIX)
AC_SUBST(LAM_BINDIR)
AC_SUBST(LAM_MANDIR)

# Allow the --enable-dist flag to be passed in

AC_ARG_ENABLE(dist, 
    AC_HELP_STRING([--enable-dist],
		   [guarantee that that the "dist" make target will be functional, although may not guarantee that any other make target will be functional.]),
    LAM_WANT_DIST=yes, LAM_WANT_DIST=no)

if test "$LAM_WANT_DIST" = "yes"; then
    AC_MSG_WARN([Configuring in 'make dist' mode])
    AC_MSG_WARN([Most make targets may be non-functional!])
fi])dnl

dnl #######################################################################

AC_DEFUN([LAM_LOG_MSG],[
# 1 is the message
# 2 is whether to put a prefix or not
if test -n "$2"; then
    echo "configure:__oline__: $1" >&5
else
    echo $1 >&5
fi])dnl

dnl #######################################################################

AC_DEFUN([LAM_LOG_FILE],[
# 1 is the filename
if test -n "$1" -a -f "$1"; then
    cat $1 >&5
fi])dnl

dnl #######################################################################

AC_DEFUN([LAM_LOG_COMMAND],[
# 1 is the command
# 2 is actions to do if success
# 3 is actions to do if fail
echo "configure:__oline__: $1" >&5
$1 1>&5 2>&1
lam_status=$?
LAM_LOG_MSG([\$? = $lam_status], 1)
if test "$lam_status" = "0"; then
    unset lam_status
    $2
else
    unset lam_status
    $3
fi])dnl

dnl #######################################################################

AC_DEFUN([LAM_UNIQ],[
# 1 is the variable name to be uniq-ized
lam_name=$1

# Go through each item in the variable and only keep the unique ones

lam_count=0
for val in ${$1}; do
    lam_done=0
    lam_i=1
    lam_found=0

    # Loop over every token we've seen so far

    lam_done="`expr $lam_i \> $lam_count`"
    while test "$lam_found" = "0" -a "$lam_done" = "0"; do

	# Have we seen this token already?  Prefix the comparison with
	# "x" so that "-Lfoo" values won't be cause an error.

	lam_eval="expr x$val = x\$lam_array_$lam_i"
	lam_found=`eval $lam_eval`

	# Check the ending condition

	lam_done="`expr $lam_i \>= $lam_count`"

	# Increment the counter

	lam_i="`expr $lam_i + 1`"
    done

    # If we didn't find the token, add it to the "array"

    if test "$lam_found" = "0"; then
	lam_eval="lam_array_$lam_i=$val"
	eval $lam_eval
	lam_count="`expr $lam_count + 1`"
    else
	lam_i="`expr $lam_i - 1`"
    fi
done

# Take all the items in the "array" and assemble them back into a
# single variable

lam_i=1
lam_done="`expr $lam_i \> $lam_count`"
lam_newval=
while test "$lam_done" = "0"; do
    lam_eval="lam_newval=\"$lam_newval \$lam_array_$lam_i\""
    eval $lam_eval

    lam_eval="unset lam_array_$lam_i"
    eval $lam_eval

    lam_done="`expr $lam_i \>= $lam_count`"
    lam_i="`expr $lam_i + 1`"
done

# Done; do the assignment

lam_newval="`echo $lam_newval`"
lam_eval="$lam_name=\"$lam_newval\""
eval $lam_eval

# Clean up

unset lam_name lam_i lam_done lam_newval lam_eval lam_count])dnl

dnl #######################################################################

AC_DEFUN([LAM_SSI_CONFIGURE_ABORT],[
# 1 is the kind
# 2 is the module name
file=ssi_$1_$2_config.sh
cat > $file <<EOF
ABORT=yes 
EOF])dnl
