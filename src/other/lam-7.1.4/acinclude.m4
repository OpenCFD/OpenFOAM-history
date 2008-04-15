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
dnl $Id: acinclude.m4,v 6.39 2003/12/21 19:14:26 jsquyres Exp $
dnl

#
# LSC standard tests
#

sinclude(config/cxx_find_exception_flags.m4)
sinclude(config/cxx_find_template_repository.m4)
sinclude(config/cxx_find_template_parameters.m4)
sinclude(config/cxx_have_bool.m4)
sinclude(config/cxx_have_exceptions.m4)
sinclude(config/cxx_have_nothrow_new.m4)
sinclude(config/cxx_have_stl.m4)
sinclude(config/cxx_have_true_false.m4)

#
# LAM-specific tests
#

sinclude(config/lam_functions.m4)
sinclude(config/lam_ansi_protos.m4)
sinclude(config/lam_bproc_setup.m4)
sinclude(config/lam_case_sensitive_fs_setup.m4)
sinclude(config/lam_config_fd_passing.m4)
sinclude(config/lam_config_subdir.m4)
sinclude(config/lam_config_subdir_args.m4)
sinclude(config/lam_check_bsd43_fd_passing.m4)
sinclude(config/lam_check_bsd44_fd_passing.m4)
sinclude(config/lam_check_fd_setsize.m4)
sinclude(config/lam_check_func.m4)
sinclude(config/lam_check_mpiexec.m4)
sinclude(config/lam_check_optflags.m4)
sinclude(config/lam_check_prefix_memcpy.m4)
sinclude(config/lam_check_prototype.m4)
sinclude(config/lam_check_pthread_pids.m4)
sinclude(config/lam_check_sizeof_struct_stat_members.m4)
sinclude(config/lam_check_stdc.m4)
sinclude(config/lam_check_svr4_fd_passing.m4)
sinclude(config/lam_check_type.m4)
sinclude(config/lam_check_typeof_getsockopt_optlen.m4)
sinclude(config/lam_check_typeof_recvfrom_len.m4)
sinclude(config/lam_config_pthreads.m4)
sinclude(config/lam_config_threads.m4)
sinclude(config/lam_config_solaris_threads.m4)
sinclude(config/lam_get_alignment.m4)
sinclude(config/lam_get_fortran_alignment.m4)
sinclude(config/lam_get_fortran_alignment_nofail.m4)
sinclude(config/lam_get_fortran_ext_name_convention.m4)
sinclude(config/lam_get_libtool_linker_flags.m4)
sinclude(config/lam_get_shmmax.m4)
sinclude(config/lam_get_sizeof.m4)
sinclude(config/lam_get_sizeof_fortran_type.m4)
sinclude(config/lam_get_sizeof_fortran_type_nofail.m4)
sinclude(config/lam_get_version.m4)
sinclude(config/lam_make_stripped_flags.m4)
sinclude(config/lam_mutex_pshared.m4)
sinclude(config/lam_memory_setup.m4)
sinclude(config/lam_prog_f77.m4)
sinclude(config/lam_setup_cc.m4)
sinclude(config/lam_setup_cxx.m4)
sinclude(config/lam_setup_optional_fortran_type.m4)
sinclude(config/lam_check_sysv_semaphores.m4)
sinclude(config/lam_check_sysv_shmem.m4)
sinclude(config/lam_wrapper_extra_ldflags.m4)

#
# What used to be mpi2c++/configure.in
#

sinclude(config/lam_cxx_bindings_setup.m4)
