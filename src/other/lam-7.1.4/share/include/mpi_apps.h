/*
 * Copyright (c) 2001-2003 The Trustees of Indiana University.  
 *                         All rights reserved.
 * Copyright (c) 1998-2001 University of Notre Dame. 
 *                         All rights reserved.
 * Copyright (c) 1994-1998 The Ohio State University.  
 *                         All rights reserved.
 * 
 * This file is part of the LAM/MPI software package.  For license
 * information, see the LICENSE file in the top level directory of the
 * LAM/MPI source distribution.
 * 
 * $HEADER$
 *
 * $Id: mpi_apps.h,v 1.3 2003/04/08 18:54:20 ssankara Exp $
 *
 *	Function:	- MPI tracing constants and templates
 */

#ifndef LAM_MPI_APPS_H
#define LAM_MPI_APPS_H

#include <lam_config.h>

#include <lam-ssi.h>


/*
 * structs sent back to mpirun (or spawning app) indicating where a
 * new process is located and some other startup negotiation stuff
 */
struct mpi_app_proc_info {
  int mapi_mcw_rank;
  int mapi_pid;
  int mapi_ps_index;
};

struct mpi_app_ssi_info {
  char masi_name[LAM_MPI_SSI_BASE_MAX_MODULE_NAME_LEN];
  int masi_major_ver;
  int masi_minor_ver;
  int masi_release_ver;
};

struct mpi_app_extra_info {
  struct mpi_app_ssi_info maei_rpi;
  struct mpi_app_ssi_info maei_crmpi;
};

#endif	/* LAM_MPI_APPS_H */
