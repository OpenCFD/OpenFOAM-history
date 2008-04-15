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
dnl $Id: ssi_acinclude.m4,v 1.3 2004/01/20 04:13:20 jsquyres Exp $
dnl

dnl
dnl Tests provided by LAM
dnl General tests
dnl

sinclude(../../../../config/lam_functions.m4)
sinclude(../../../../config/lam_get_version.m4)

dnl
dnl C compiler tests
dnl

sinclude(../../../../config/lam_setup_cc.m4)
sinclude(../../../../config/lam_check_optflags.m4)

dnl
dnl C++ compiler tests
dnl

sinclude(../../../../config/lam_setup_cxx.m4)
sinclude(../../../../config/cxx_have_nothrow_new.m4)
sinclude(../../../../config/cxx_find_template_repository.m4)
sinclude(../../../../config/cxx_find_template_parameters.m4)

dnl
dnl This will be replaced with s_i_n_c_l_u_d_e(configure.stub) if it 
dnl exists for that module.
dnl


