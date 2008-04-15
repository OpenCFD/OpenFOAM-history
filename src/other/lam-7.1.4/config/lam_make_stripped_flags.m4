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
dnl $Id: lam_make_stripped_flags.m4,v 1.2 2003/06/18 20:25:34 brbarret Exp $
dnl

define(LAM_MAKE_STRIPPED_FLAGS,[

# Modularize this setup so that sub-configure.in scripts can use this
# same setup code.

# Process a set of flags and remove all debugging and optimization
# flags

s_arg="$1"
s_result=
bogus=1
for s_word in $s_arg; do
    case $s_word in
    -g)    ;;
    +K0)   ;;
    +K1)   ;;
    +K2)   ;;
    +K3)   ;;
    +K4)   ;;
    +K5)   ;;
    -O)    ;;
    -O0)   ;;
    -O1)   ;;
    -O2)   ;;
    -O3)   ;;
    -O4)   ;;
    -O5)   ;;
    -O6)   ;;
    -O7)   ;;
    -O8)   ;;
    -O9)   ;;
    -xO)   ;;
    -xO0)  ;;
    -xO1)  ;;
    -xO2)  ;;
    -xO3)  ;;
    -xO4)  ;;
    -xO5)  ;;
    -xO6)  ;;
    -xO7)  ;;
    -xO8)  ;;
    -xO9)  ;;
    -fast) ;;
    *)     s_result="$s_result $s_word"
    esac
done

# Clean up

unset s_word sarg])
