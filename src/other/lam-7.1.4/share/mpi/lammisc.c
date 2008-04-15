/*
 * Copyright (c) 2001-2002 The Trustees of Indiana University.  
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
 * $Id: lammisc.c,v 6.18 2003/10/11 19:15:35 brbarret Exp $
 *
 *	Function:	- miscellaneous functions
 */

#include <lam_config.h>

#include <string.h>
#if defined(HAVE_STRINGS_H) && HAVE_STRINGS_H
#include <strings.h>
#endif
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#if defined(HAVE_SYS_SELECT_H) && HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif
#if defined(HAVE_SCHED_H) && HAVE_SCHED_H
#include <sched.h>
#endif

#include "mpi.h"
#include "mpisys.h"
#include "terror.h"


/*
 * prototypes for OS's that don't have them (cough cough AIX 4.1 cough cough)
 */
#if LAM_HAVE_YIELD && !LAM_USE_SELECT_YIELD
void yield(void);
#endif

/*
 *	lam_cubedim
 *
 *	Function:	- get dimension of inscribing hypercube
 *	Accepts:	- # processes
 *	Returns:	- dimension or -1
 */
int
lam_cubedim(nprocs)

int			nprocs;

{
	int		dim;
	unsigned	size;

	if (nprocs < 1) return(-1);

	for (dim = 0, size = 1; size < nprocs; ++dim, size <<= 1);

	return(dim);
}

/*
 *	lam_hibit
 *
 *	Function:	- get position of the highest bit set
 *	Accepts:	- process rank
 *			- hypercube dimension
 *	Returns:	- position or -1
 */
int
lam_hibit(rank, dim)

int			rank;
int			dim;

{
	unsigned	mask;

	if ((rank < 1) || (dim < 1)) return(-1);

	--dim;
	mask = 1 << dim;

	for (; dim >= 0; --dim, mask >>= 1) {
		if (rank & mask) break;
	}

	return(dim);
}

/*
 *	lam_emtpystat
 *
 *	Function:	- set status to empty
 *	Accepts:	- ptr status
 */
void
lam_emptystat(pstat)

MPI_Status		*pstat;

{
	pstat->MPI_SOURCE = MPI_ANY_SOURCE;
	pstat->MPI_TAG = MPI_ANY_TAG;
	pstat->MPI_ERROR = MPI_ERR_PENDING;
	pstat->st_length = 0;
}

/*
 *      lam_yield
 *
 *      Function:       - make best effort to yield the processor
 */
void
lam_yield()
{
#if LAM_HAVE_YIELD && !LAM_USE_SELECT_YIELD

	yield();

#elif LAM_HAVE_SCHED_YIELD && !LAM_USE_SELECT_YIELD

	sched_yield();

#else

        struct timeval  timeout;

        timeout.tv_sec = 0;
        timeout.tv_usec = 1;

        select(0, (fd_set *) 0, (fd_set *) 0, (fd_set *) 0, &timeout);
#endif
}

