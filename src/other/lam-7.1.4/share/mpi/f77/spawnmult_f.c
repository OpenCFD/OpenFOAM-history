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
 * $Id: spawnmult_f.c,v 6.10 2003/07/06 10:44:02 jsquyres Exp $
 *
 *	Function:	- MPI_Comm_spawn_multiple F77 wrapper
 */

#include <lam_config.h>

#include <errno.h>
#include <stdlib.h>

#include <sfh.h>
#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>
#include <typical.h>

/*
 *	f2c_argvs
 *
 *	Function:	- creates array of C argument vector from an
 *			  F77 2-d array of strings
 *	Accepts:	- first dimension of 2-d array
 *			- F77 2-d array of strings
 *			- length of strings
 *			- array of C argument vectors (out)
 *	Returns:	- 0 or LAMERROR
 */
static int
f2c_argvs(int dim, char *array, int len, char ****argvs)
{
	int		argc;			/* argument vector count */
	char		***argvarr;		/* array of argument vectors */
	char		*cstr;			/* C string */
	char		*p;
	int		i, j;

	if ((argvarr = (char ***) malloc(dim * sizeof(char **))) == 0) {
		return(LAMERROR);
	}

	for (i = 0; i < dim; i++) {

		argc = 0;
		argvarr[i] = 0;
		p = array + len * i;

		while (1) {
			cstr = lam_F2C_string(p, len);

			if (cstr == 0) {
				for (j = 0; j <= i; j++) {
					sfh_argv_free(argvarr[j]);
				}
				return(LAMERROR);
			}

			if (*cstr == 0) break;

			if (sfh_argv_add(&argc, &argvarr[i], cstr)) {
				for (j = 0; j <= i; j++) {
					sfh_argv_free(argvarr[j]);
				}
				return(LAMERROR);
			}

			p += len * dim;
		}
	}

	*argvs = argvarr;
	return(0);
}

/*
 *	f2c_argvs_free
 *
 *	Function:	- free array of C argument vectors
 *	Accepts:	- length of array
 *			- array of C argument vectors
 */
static void 
f2c_argvs_free(int n, char ***argvs)
{
	int i;

	if (argvs) {
		for (i = 0; i < n; i++)
			sfh_argv_free(argvs[i]);

		free(argvs);
	}
}

void
mpi_comm_spawn_multiple_(int *n, char *cmd, char *av, int *mps, int *infs, 
                         int *root, int *comm, int *icomm, int *ec, 
                         int *ierr, int nc, int na)
{
	MPI_Comm	intercomm;
	MPI_Info	*infos;
	char		***argvs = 0;
	char		**commands;
	int		*errs;
	int		i;
/*
 * check arguments
 */
	if (*n <= 0) {
		lam_setfunc(BLKMPICOMMSPAWNMULT);
		*ierr = lam_errfunc(GETHDL(*comm), BLKMPICOMMSPAWNMULT,
                                    lam_mkerr(MPI_ERR_ARG, 0));
		return;
	}
/*
 * Create info array.
 */
	if ((infos = (MPI_Info *) malloc(*n * sizeof(MPI_Info *))) == 0) {
		lam_setfunc(BLKMPICOMMSPAWNMULT);
		*ierr = lam_errfunc(GETHDL(*comm), BLKMPICOMMSPAWNMULT,
                                    lam_mkerr(MPI_ERR_SPAWN, errno));
		return;
	}

	for (i = 0; i < *n; i++) {
		infos[i] = GETHDL(infs[i]);
	}
/*
 * Check for special argument values.
 */
	if ((void *) ec == lam_F_errorcodes_ignore) {
		errs = 0;
	} else {
		errs = ec;
	}

	if ((void *) av == lam_F_argvsnull) {
		argvs = 0;
	} else {
/*
 * Convert F77 argument arrays to C argument vectors.
 */
		if (f2c_argvs(*n, av, na, &argvs)) {
			lam_setfunc(BLKMPICOMMSPAWNMULT);
			*ierr = lam_errfunc(GETHDL(*comm), BLKMPICOMMSPAWNMULT,
                                            lam_mkerr(MPI_ERR_SPAWN, errno));
			free((char *) infos);
			return;
		}
	}
/*
 * Convert F77 command strings to C array of commands.
 */
	if (lam_F2C_argv(cmd, nc, &commands)) {
		lam_setfunc(BLKMPICOMMSPAWNMULT);
		*ierr = lam_errfunc(GETHDL(*comm), BLKMPICOMMSPAWNMULT,
                                    lam_mkerr(MPI_ERR_SPAWN, errno));
		f2c_argvs_free(*n, argvs);
		free((char *) infos);
		return;
	}

	*ierr = MPI_Comm_spawn_multiple(*n, commands, argvs, mps, infos,
                                        *root, GETHDL(*comm), &intercomm, 
                                        errs);

	if (lam_F_make_hdl(icomm, ierr, intercomm, BLKMPICOMMSPAWNMULT)) {
		intercomm->c_f77handle = *icomm;
	}

	sfh_argv_free(commands);
	f2c_argvs_free(*n, argvs);
	free((char *) infos);
}

