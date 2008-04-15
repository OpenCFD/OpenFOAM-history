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
 * $Id: spawn_f.c,v 6.8 2003/07/06 10:44:01 jsquyres Exp $
 *
 *	Function:	- MPI_Comm_spawn F77 wrapper
 */

#include <lam_config.h>

#include <errno.h>
#include <stdlib.h>

#include <sfh.h>
#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>


void
mpi_comm_spawn_(char *cmd, char *av, int *maxp, int *info, int *root, 
                int *comm, int *icomm, int *ec, int *ierr, int nc, int na)
{
	MPI_Comm	intercomm;		/* parent-child intercomm */
	char		**argv = 0;		/* C argument vector */
	char		*command;		/* C program name */
	int		*errs;			/* C errcode array */
/*
 * Check for special argument values.
 */
	if ((void *) ec == lam_F_errorcodes_ignore) {
		errs = 0;
	} else {
		errs = ec;
	}

	if ((void *) av == lam_F_argvnull) {
		argv = 0;
	} else {
/*
 * Convert F77 argument array to C argument vector.
 */
		if (lam_F2C_argv(av, na, &argv)) {
			lam_setfunc(BLKMPICOMMSPAWN);
			*ierr = lam_errfunc(GETHDL(*comm), BLKMPICOMMSPAWN,
					    lam_mkerr(MPI_ERR_SPAWN, errno));
			return;
		}

		if (argv == 0) {
			argv = 0;
		}
	}
/*
 * Convert F77 command string to C string.
 */
	command = lam_F2C_string(cmd, nc);
	if (command == 0) {
                sfh_argv_free(argv);
		lam_setfunc(BLKMPICOMMSPAWN);
		*ierr = lam_errfunc(GETHDL(*comm), BLKMPICOMMSPAWN,
					lam_mkerr(MPI_ERR_SPAWN, errno));
		return;
	}

	*ierr = MPI_Comm_spawn(command, argv, *maxp, GETHDL(*info), 
				*root, GETHDL(*comm), &intercomm, errs);

	if (lam_F_make_hdl(icomm, ierr, intercomm, BLKMPICOMMSPAWN)) {
		intercomm->c_f77handle = *icomm;
	}
	
	sfh_argv_free(argv);
	free(command);
}
