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
 * $Id: cdup.c,v 6.11 2003/02/20 19:45:27 jsquyres Exp $
 *
 *	Function:	- duplicate a communicator
 *	Accepts:	- old communicator
 *			- new communicator (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <lam_config.h>
#include <all_hash.h>
#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>


/*@

MPI_Comm_dup - Duplicates an existing communicator with all its cached
               information

Input Parameter:
. comm - communicator (handle) 

Output Parameter:
. newcomm - copy of 'comm' (handle) 

Notes:

As per standard, this is a collective call over all processes in 'comm'.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_ARG
.N MPI_ERR_INTERN
.N MPI_ERR_EXHAUSTED

.seealso: MPI_Comm_free

.N ACK
@*/
int MPI_Comm_dup(MPI_Comm comm, MPI_Comm* newcomm)
{
	int		cid;			/* global context ID */
	int		err;			/* error code */
	int		fl_copy;		/* attr. copy flag */
	int		size;			/* group size */
	struct _attr	elem;			/* attr. element */
	struct _attr	*p;			/* favourite pointer */
	struct _attrkey	*pk;			/* key pointer */

	lam_initerr_m();
	lam_setfunc_m(BLKMPICOMMDUP);
/*
 * Check the arguments.
 */
	if (comm == MPI_COMM_NULL) {
	    return(lam_errfunc(MPI_COMM_WORLD, BLKMPICOMMDUP,
		    		lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (newcomm == 0) {
	    return(lam_errfunc(comm, BLKMPICOMMDUP,
		    		lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
	
#if LAM_WANT_IMPI

	/* Remove this when IMPI collectives are implemented */

        if (LAM_IS_IMPI(comm)) {
	  return lam_err_comm(comm, MPI_ERR_COMM, 0, 
			      "Collectives not yet implemented on IMPI communicators");
	}
#endif

/*
 * Set up tracing.
 */
	LAM_TRACE(lam_tr_cffstart(BLKMPICOMMDUP));

	/* Collectively agree on a new CID */

	if (LAM_IS_INTER(comm))
	  err = lam_coll_alloc_inter_cid(comm, 0, comm, 0, MPI_MERGE_TAG,
					 BLKMPICOMMDUP, &cid);
	else
	  err = lam_coll_alloc_intra_cid(comm, 1, BLKMPICOMMDUP, &cid);

	if (err != MPI_SUCCESS) {
	    LAM_TRACE(lam_tr_cffend(BLKMPICOMMDUP, -1, comm, 0, 0));
	    lam_resetfunc(BLKMPICOMMDUP);
	    return(lam_errfunc(comm, BLKMPICOMMDUP, err));
	}
/*
 * Create the new communicator.
 */
	*newcomm = 0;
	if (lam_comm_new(cid, comm->c_group, comm->c_rgroup,
                         comm->c_flags & ~LAM_PREDEF & ~LAM_RBASIC,
                         newcomm)) {
	    return(lam_errfunc(comm, BLKMPICOMMDUP,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	}
/*
 * Copy the key attributes using the copy callback functions.
 */
	if (comm->c_keys) {

	    (*newcomm)->c_keys = ah_init((int4) ah_size(comm->c_keys),
		    (int4) sizeof(struct _attr),
		    (int4) MPI_KEYVAL_INVALID, INT4_NIL);

	    if ((*newcomm)->c_keys == 0) {
		return(lam_errfunc(comm, BLKMPICOMMDUP,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	    }

	    p = (struct _attr *) ah_top(comm->c_keys);

	    while (p) {
		pk = lam_getattr(p->a_key);
		if (pk == 0) {
		    return(lam_errfunc(comm, BLKMPICOMMDUP,
			    lam_mkerr(MPI_ERR_INTERN, errno)));
		}
/*
 * Call the C or F77 callback function.
 */
		fl_copy = 0;
		if (((MPI_Copy_function *) (pk->ak_copy)) != MPI_NULL_COPY_FN) {
		    if (pk->ak_flags & LAM_LANGF77) {			
			(*((MPI_F_copy_function *) (pk->ak_copy)))
			    	(&comm->c_f77handle, &p->a_key,
				(int *) &pk->ak_extra, (int *) &p->a_value,
				(int *) &(elem.a_value), &fl_copy, &err);
		    } else {
			err = (*((MPI_Copy_function *) (pk->ak_copy)))
			    	(comm, p->a_key, pk->ak_extra,
				p->a_value, &(elem.a_value), &fl_copy);
		    }
		}

		if (err != MPI_SUCCESS) {
		    return(lam_errfunc(comm, BLKMPICOMMDUP, 
				       lam_mkerr(err, EINVAL)));
		}	
/*
 * If attribute is copied, insert it and increment key reference count.
 */
		if (fl_copy) {
		    elem.a_key = p->a_key;

		    if (ah_insert((*newcomm)->c_keys, &elem)) {
			return(lam_errfunc(comm, BLKMPICOMMDUP,
					lam_mkerr(MPI_ERR_OTHER, errno)));
		    }

		    pk->ak_refcount++;
		}

		p = (struct _attr *) ah_next(comm->c_keys, p);
	    }
	}
/*
 * Copy the topology.
 */
	(*newcomm)->c_topo_type = comm->c_topo_type;
	(*newcomm)->c_topo_nprocs = comm->c_topo_nprocs;
	(*newcomm)->c_topo_ndims = comm->c_topo_ndims;
	(*newcomm)->c_topo_nedges = comm->c_topo_nedges;

	if (LAM_IS_CART(comm)) {
	    size = 2 * comm->c_topo_ndims * sizeof(int);

	    (*newcomm)->c_topo_dims = (int *) malloc((unsigned) size);
	    if ((*newcomm)->c_topo_dims == 0) {
		return(lam_errfunc(comm, BLKMPICOMMDUP,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	    }

	    (*newcomm)->c_topo_coords =
				(*newcomm)->c_topo_dims + comm->c_topo_ndims;
	    memcpy((char *) (*newcomm)->c_topo_dims,
		    (char *) comm->c_topo_dims, size);
	}
	else if (LAM_IS_GRAPH(comm)) {
	    size = (comm->c_topo_nprocs+comm->c_topo_nedges) * sizeof(int);

	    (*newcomm)->c_topo_index = (int *) malloc((unsigned) size);
	    if ((*newcomm)->c_topo_index == 0) {
		return(lam_errfunc(comm, BLKMPICOMMDUP,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	    }

	    (*newcomm)->c_topo_edges =
				(*newcomm)->c_topo_index + comm->c_topo_nprocs;
	    memcpy((char *) (*newcomm)->c_topo_index,
		    (char *) comm->c_topo_index, size);
	}
/*
 * Copy the error handler.
 */
	(*newcomm)->c_errhdl = comm->c_errhdl;

	comm->c_errhdl->eh_refcount++;
	comm->c_group->g_refcount++;
	if (LAM_IS_INTER(comm)) {
	    comm->c_rgroup->g_refcount++;
	}

	if (!al_insert(lam_comms, newcomm)) {
	    return(lam_errfunc(comm, BLKMPICOMMDUP,
				lam_mkerr(MPI_ERR_INTERN, errno)));
	}
	
	if (lam_tr_comm(*newcomm)) {
	    return(lam_errfunc(comm, BLKMPICOMMDUP,
		lam_mkerr(MPI_ERR_INTERN, errno)));
	}

	lam_setcid(cid);

	/* Let the collective SSI modules battle over who will do
	   collectives on this new communicator.  It'll likely be the
	   same as the one that's on this communicator, but we still
	   give them the option to do something different anyway... */

	if (lam_ssi_coll_base_init_comm(*newcomm) != 0)
	  return(lam_errfunc(comm, BLKMPICOMMDUP,
			     lam_mkerr(MPI_ERR_INTERN, ENOSYS)));

	LAM_TRACE(lam_tr_cffend(BLKMPICOMMDUP, -1, comm, 0, 0));

	lam_resetfunc_m(BLKMPICOMMDUP);
	return(MPI_SUCCESS);
}
