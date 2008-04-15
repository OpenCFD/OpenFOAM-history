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
 *
 *	Function:	- #defines, variable declaration for totalview 
 */

#ifndef LAM_TOTALVIEW_H
#define LAM_TOTALVIEW_H

#include <mpi.h>
#include <all_opt.h>
#include <all_list.h>

/* These are assigned to MPIR_debug_state to let TV know if we were
   able to spawn off MPI processes well */

#define PROCESSES_SPAWNED   1
#define PROCESSES_ABORTING  2

/*
 * Functions that are invoked from mpirun
 * All in otb/mpirun/totalview.c
 */
void lam_tv_attach(LIST *app);
void lam_tv_check(OPT *opt);
void lam_tv_finalize(void);
void lam_tv_init(int argc, char *argv[], OPT *opt);


#endif /* LAM_TOTALVIEW_H */






