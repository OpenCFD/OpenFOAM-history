/*
 * Copyright (c) 2001-2005 The Trustees of Indiana University.  
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
 * $Id: alltoallw_f.c,v 6.2 2004/01/02 11:57:09 jsquyres Exp $
 *
 *	Function:	- MPI_Alltoallw	F77 wrapper
 */

#include <lam_config.h>

#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>
#include <blktype.h>

void 
mpi_alltoallw_(sb, sc, sd, st, rb, rc, rd, rt, c, ierr)

char	*sb, *rb;
int	*sc, *sd, *st, *rc, *rd, *rt, *c, *ierr;

{
        int i, np;
        MPI_Datatype *stypes, *rtypes;

        *ierr = MPI_Comm_size(GETHDL(*c), &np);

        if(*ierr == MPI_SUCCESS) {
          stypes = malloc(np * 2 * sizeof(MPI_Datatype));
          if (stypes == NULL) {
            lam_setfunc(BLKMPIALLTOALLW);
            *ierr = lam_err_comm(MPI_COMM_WORLD,
                                 MPI_ERR_OTHER, errno, "malloc failed");
            return;
          }
          rtypes = stypes + np;

          for(i=0; i<np; i++){
            stypes[i] = GETHDL(st[i]);
            rtypes[i] = GETHDL(rt[i]);
          }

          *ierr = MPI_Alltoallw(BUF(sb), sc, sd, stypes,
		                BUF(rb), rc, rd, rtypes, GETHDL(*c));

          free(stypes);
        }
}
