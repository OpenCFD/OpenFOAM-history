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
dnl $Id: lam_get_version.m4,v 1.3 2002/10/09 20:55:12 brbarret Exp $
dnl

define(LAM_GET_VERSION,[
gv_glv_dir="$1"
gv_ver_file="$2"
gv_prefix="$3"

# Find the get_lam_version program

gv_prog="sh $gv_glv_dir/get_lam_version $gv_ver_file"

dnl quote eval to suppress macro expansion with non-GNU m4

gv_run() {
  [eval] ${gv_prefix}_${2}=`$gv_prog --${1}`
}

gv_run full    VERSION
gv_run major   MAJOR_VERSION
gv_run minor   MINOR_VERSION
gv_run release RELEASE_VERSION
gv_run alpha   ALPHA_VERSION
gv_run beta    BETA_VERSION
gv_run svn     SVN_VERSION

# Clean up

unset gv_glv_dir gv_ver_file gv_prefix gv_prog gv_run
])
