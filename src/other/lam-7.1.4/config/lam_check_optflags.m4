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
dnl $Id: lam_check_optflags.m4,v 1.2 2002/10/09 20:55:11 brbarret Exp $
dnl

define(LAM_CHECK_OPTFLAGS,[

# Modularize this setup so that sub-configure.in scripts can use this
# same setup code.

##################################
# Optimization flags
##################################

# If the user did not specify optimization flags, add some (the value
# from $OPTFLAGS)

co_arg="$1"
co_found=0
for co_word in $co_arg; do
    case $co_word in
    -g)    co_found=1 ;;
    +K0)   co_found=1 ;;
    +K1)   co_found=1 ;;
    +K2)   co_found=1 ;;
    +K3)   co_found=1 ;;
    +K4)   co_found=1 ;;
    +K5)   co_found=1 ;;
    -O)    co_found=1 ;;
    -O0)   co_found=1 ;;
    -O1)   co_found=1 ;;
    -O2)   co_found=1 ;;
    -O3)   co_found=1 ;;
    -O4)   co_found=1 ;;
    -O5)   co_found=1 ;; 
    -O6)   co_found=1 ;;
    -O7)   co_found=1 ;;
    -O8)   co_found=1 ;;
    -O9)   co_found=1 ;;
    -xO)   co_found=1 ;;
    -xO0)  co_found=1 ;;
    -xO1)  co_found=1 ;;
    -xO2)  co_found=1 ;;
    -xO3)  co_found=1 ;;
    -xO4)  co_found=1 ;;
    -xO5)  co_found=1 ;; 
    -xO6)  co_found=1 ;;
    -xO7)  co_found=1 ;;
    -xO8)  co_found=1 ;;
    -xO9)  co_found=1 ;;
    -fast) co_found=1 ;;
    esac
done

if test "$co_found" = "0"; then
    co_result="$OPTFLAGS $co_arg"
else
    co_result="$co_arg"
fi

# Clean up

unset co_found co_word co_arg
])
