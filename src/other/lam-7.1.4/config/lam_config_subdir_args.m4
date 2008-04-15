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
dnl $Id: lam_config_subdir_args.m4,v 1.3 2004/01/20 03:41:47 jsquyres Exp $
dnl

AC_DEFUN([LAM_CONFIG_SUBDIR_ARGS],[
#
# Invoke configure in subdirectories.
#
# $1 is the name of the variable to assign the output to
#

#
# Make a list of command line args --eliminate the --srcdir and
# --cache-file args, because we need to replace them with our own
# values when invoking the sub-configure script.
#

subdirs_args=
subdirs_skip=no

for subdirs_arg in $ac_configure_args; do
    if test "$subdirs_skip" = "yes"; then
	subdirs_skip=no
    else
	case $subdirs_arg in
	-cache-file | --cache-file | -cache | --cache)
	    subdirs_skip=yes
	    ;;
	--config-cache | -C)
	    ;;
	-cache-file=* | --cache-file=*)
	    ;;
	-srcdir | --srcdir)
	    subdirs_skip=yes
	    ;;
	-srcdir=* | --srcdir=*)
	    ;;
	*) 
	    subdirs_args="$subdirs_args $subdirs_arg" 
	    ;;
	esac
    fi
done

#
# Assign the output
#

subdirs_str="$1="'"'"$subdirs_args"'"'
eval $subdirs_str

#
# Clean up
#

unset subdirs_str subdirs_skip subdirs_args subdirs_arg])dnl
