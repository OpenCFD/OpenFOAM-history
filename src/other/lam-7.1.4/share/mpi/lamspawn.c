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
 * $Id: lamspawn.c,v 6.16 2003/04/10 19:21:09 ssankara Exp $
 *
 *	Function:	- helper functions to the spawn routines
 */

#include <lam_config.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <etc_misc.h>
#include <mpisys.h>
#include <rpisys.h>
#include <net.h>
#include <mpi_apps.h>
#include <lam-ssi-cr.h>
#include <lam-ssi-rpi.h>


/*
 *	lam_get_mpi_world
 *
 *	Function:	- get MPI world
 *	Accepts:	- size of world
 *			- initial process world
 *			- MPI process world (out)
 *			- string prefix to print in error message
 *	Returns:	- full pathname or NULL
 */
int
lam_get_mpi_world(int4 world_n, struct _gps *world, struct _gps *mpiworld,
		  char *prefix)
{
  int i, j;
  struct nmsg msg;
  struct mpi_app_proc_info *mapi;

  memcpy(mpiworld, world, world_n * sizeof(struct _gps));
  for (i = 0; i < world_n; i++) {
    mpiworld[i].gps_pid = 0;
  }

  LAM_ZERO_ME(msg);
  msg.nh_event = (-lam_getpid()) & 0xBFFFFFFF;
  msg.nh_flags = DINT4DATA;
  msg.nh_length = 0;
  mapi = (struct mpi_app_proc_info *) &msg.nh_data;

  for (i = 0; i < world_n; i++) {
    msg.nh_type = 3;
    if (nrecv(&msg)) {
      terror("mpirun");
      return(LAMERROR);
    }

    if (msg.nh_type == 1) {
      fprintf(stderr, 
              "%s: process terminated before completing MPI_Init()\n",
              prefix);
      return(LAMERROR);
    }

    /* Set the MPI process pid and index. */

    j = mapi->mapi_mcw_rank;
    if (j < 0 || j >= world_n) {
      errno = EIMPOSSIBLE;
      terror("mpirun");
      return(LAMERROR);
    }

    mpiworld[j].gps_pid = mapi->mapi_pid;
    mpiworld[j].gps_idx = mapi->mapi_ps_index;
  }

  return(0);
}


/*
 *	lam_match_ssi_modules
 *
 *	Function:	- match the SSI modules chosen by all spawned
 *	                  processes with the corresponding modules chosen
 *	                  by us.
 *	Accepts:	- size of world
 *			- MPI process world
 *			- string prefix to print error message
 *	Returns:	- 0 or LAMERROR
 */
int
lam_match_ssi_modules(int4 world_n, struct _gps *world, char *prefix)
{
  int i, max;
  struct nmsg msg;
  lam_ssi_module_t *module;
  lam_ssi_rpi_t rpi;
#if 0
  /* See comment below */
  lam_ssi_crmpi_t crmpi;
#endif
  struct mpi_app_extra_info maei;
  char arg1[LAM_MPI_SSI_BASE_MAX_MODULE_NAME_LEN * 2],
       arg2[LAM_MPI_SSI_BASE_MAX_MODULE_NAME_LEN * 2],
       arg3[LAM_MPI_SSI_BASE_MAX_MODULE_NAME_LEN * 2];

  LAM_ZERO_ME(msg);
  msg.nh_event = (-lam_getpid()) & 0xBFFFFFFF;
  msg.nh_flags = DINT4DATA;
  msg.nh_length = sizeof(maei);
  msg.nh_msg = (char *) &maei;

  for (i = 0; i < world_n; i++) {
    msg.nh_type = 3;
    if (nrecv(&msg)) {
      terror("spawn parent");
      return LAMERROR;
    }

    if (msg.nh_type == 1) {
      fprintf(stderr, "%s: process terminated before completing "
              "MPI_Init()\n", prefix);
      return LAMERROR;
    }

    max = (LAM_MPI_SSI_BASE_MAX_MODULE_NAME_LEN * 2) - 1;

    /* 
     * Check to ensure that the RPI module sent by all processes match
     * what we've got.  This has to change when we go to multi-RPI. 
     */

    module = (lam_ssi_module_t *) al_top(lam_ssi_rpi_base_available);
    rpi = *((lam_ssi_rpi_t *) module->lsm_module);
    if (strncmp(rpi.lsr_meta_info.ssi_module_name, 
                maei.maei_rpi.masi_name,
                sizeof(rpi.lsr_meta_info.ssi_module_name)) != 0 ||
        (rpi.lsr_meta_info.ssi_module_major_version !=
           ttol(maei.maei_rpi.masi_major_ver)) ||
        (rpi.lsr_meta_info.ssi_module_minor_version !=
           ttol(maei.maei_rpi.masi_minor_ver)) ||
        (rpi.lsr_meta_info.ssi_module_release_version !=
           ttol(maei.maei_rpi.masi_release_ver))) {

      /* Take pains to print a friendly error message */
      snprintf(arg1, max, "%s (v%d.%d.%d)", 
               rpi.lsr_meta_info.ssi_module_name,
               rpi.lsr_meta_info.ssi_module_major_version,
               rpi.lsr_meta_info.ssi_module_minor_version,
               rpi.lsr_meta_info.ssi_module_release_version);
      snprintf(arg2, max, "%d", msg.nh_data[0]);
      snprintf(arg3, max, "%s (v%d.%d.%d)", maei.maei_rpi.masi_name,
               ttol(maei.maei_rpi.masi_major_ver),
               ttol(maei.maei_rpi.masi_minor_ver),
               ttol(maei.maei_rpi.masi_release_ver));

      show_help("MPI", "spawn-rpi-mismatch", 
                prefix, arg1, arg2, arg3, NULL);
      return LAMERROR;
    }

#if 0
    /* Due to bug 682, spawned processes cannot be checkpointable --
       we can just ignore what they sent for crmpi modules, because
       lam_mpi_init() is hard-wired to select no crmpi module when an
       MPI process is MPI_COMM_SPAWN*'ed. */

    /* 
     * If we did not select any crmpi module to run, we must ensure that
     * the spawned processes send ("none", -1, -1, -1) for the selected
     * crmpi module.  Else we must ensure that the crmpi module sent by the
     * spawned processes match what we've got.
     */

    module = (lam_ssi_module_t *) al_top(lam_ssi_crmpi_base_available);
    if (module != NULL)
      crmpi = *((lam_ssi_crmpi_t *) module->lsm_module);
    
    if (((NULL == module) && 
         (strncmp(maei.maei_crmpi.masi_name, "none",
                  sizeof(maei.maei_crmpi.masi_name)) != 0 ||
          (ttol(maei.maei_crmpi.masi_major_ver) != -1) ||
          (ttol(maei.maei_crmpi.masi_minor_ver) != -1) ||
         (ttol(maei.maei_crmpi.masi_release_ver) != -1)))) {
      /* Take pains to print a friendly error message */
      snprintf(arg1, max, "%s (v%d.%d.%d)", "none", -1, -1, -1);
      snprintf(arg2, max, "%d", msg.nh_data[0]);
      snprintf(arg3, max, "%s (v%d.%d.%d)", maei.maei_crmpi.masi_name,
               ttol(maei.maei_crmpi.masi_major_ver),
               ttol(maei.maei_crmpi.masi_minor_ver),
               ttol(maei.maei_crmpi.masi_release_ver));

      show_help("MPI", "spawn-cr-mismatch", 
                arg1, arg2, arg3, NULL);
      return LAMERROR;
    }

    if ((module != NULL) &&   
        ((strncmp(crmpi.lscrm_meta_info.ssi_module_name,
                  maei.maei_crmpi.masi_name,
                  sizeof(crmpi.lscrm_meta_info.ssi_module_name)) != 0) ||
         (crmpi.lscrm_meta_info.ssi_module_major_version !=
            ttol(maei.maei_crmpi.masi_major_ver)) ||
         (crmpi.lscrm_meta_info.ssi_module_minor_version !=
            ttol(maei.maei_crmpi.masi_minor_ver)) ||
         (crmpi.lscrm_meta_info.ssi_module_release_version !=
            ttol(maei.maei_crmpi.masi_release_ver)))) {

      /* Take pains to print a friendly error message */
      snprintf(arg1, max, "%s (v%d.%d.%d)", 
               crmpi.lscrm_meta_info.ssi_module_name,
               crmpi.lscrm_meta_info.ssi_module_major_version,
               crmpi.lscrm_meta_info.ssi_module_minor_version,
               crmpi.lscrm_meta_info.ssi_module_release_version);
      snprintf(arg2, max, "%d", msg.nh_data[0]);
      snprintf(arg3, max, "%s (v%d.%d.%d)", maei.maei_crmpi.masi_name,
               ttol(maei.maei_crmpi.masi_major_ver),
               ttol(maei.maei_crmpi.masi_minor_ver),
               ttol(maei.maei_crmpi.masi_release_ver));

      show_help("MPI", "spawn-cr-mismatch", 
                arg1, arg2, arg3, NULL);
      return LAMERROR;
    }
#endif
  }
               
  return 0;
}


