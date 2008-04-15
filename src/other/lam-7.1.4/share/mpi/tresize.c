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
 * $Id: tresize.c,v 1.4 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- create a resized datatype
 *	Accepts:	- type to be resized
 *			- new lower bound
 *			- new extent
 *			- new resized type (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
   MPI_Type_create_extent - Create a resized datatype

Input Parameters:
+ type - Inpute datatype
. lb - New lower bound of the datatype
- extent - New extent of the datatype

Output Parameter:
+ newtype - New resized type

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_TYPE
.N MPI_ERR_OTHER
.N MPI_ERR_INTERN

.seealso MPI_Type_get_true_extent(3), MPI_Type_create_resized(3)

.N WEB
@*/

int
MPI_Type_create_resized(MPI_Datatype type, MPI_Aint lb, 
			MPI_Aint extent, MPI_Datatype *newtype)
{
	MPI_Datatype	ntype;			/* new datatype */
	struct _attr	*p;			/* attribute */
	struct _attrkey	*pk;			/* key */

	lam_initerr_m();
	lam_setfunc_m(BLKMPITCREATERESIZED);

	if (type == MPI_DATATYPE_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
				   BLKMPITCREATERESIZED, 
				   lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	}

	ntype = (MPI_Datatype) malloc(sizeof(struct _dtype));
	if (ntype == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
				   BLKMPITCREATERESIZED, 
				   lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	*ntype = *type;

	memset(ntype->dt_name, 0, sizeof(ntype->dt_name));
	ntype->dt_commit = 0;
	ntype->dt_uargs = 0;
	ntype->dt_refcount = 1;
	ntype->dt_f77handle = -1;
	ntype->dt_label = -1;
	ntype->dt_flags &= ~(LAM_PREDEF | LAM_DTISDUP | LAM_DTNOXADJ);
	ntype->dt_flags |= LAM_DTISRESIZE | LAM_DTHASUB | LAM_DTHASLB;
	ntype->dt_lower = lb;
	ntype->dt_upper = lb + extent;
	if (ntype->dt_upper == ntype->dt_dataup
	    	&& ntype->dt_lower == ntype->dt_datalow) {
	    ntype->dt_flags |= LAM_DTNOXADJ;
	}

	ntype->dt_uargs = (void *) type;
	type->dt_refcount++;
/*
 * Copy the key attributes.
 */
	if (type->dt_keys) {

	    ntype->dt_keys = ah_init((int4) ah_size(type->dt_keys),
		    (int4) sizeof(struct _attr),
		    (int4) MPI_KEYVAL_INVALID, INT4_NIL);

	    if (ntype->dt_keys == 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPITCREATERESIZED,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	    }

	    p = ah_top(type->dt_keys);

	    while (p) {
		pk = lam_getattr(p->a_key);
		if (pk == 0) {
		    return(lam_errfunc(MPI_COMM_WORLD, BLKMPITCREATERESIZED,
			    lam_mkerr(MPI_ERR_INTERN, errno)));
		}

		if (ah_insert(ntype->dt_keys, p)) {
		    return(lam_errfunc(MPI_COMM_WORLD, BLKMPITCREATERESIZED,
					lam_mkerr(MPI_ERR_OTHER, errno)));
		}

		pk->ak_refcount++;
		p = ah_next(type->dt_keys, p);
	    }
	}

	*newtype = ntype;

        lam_resetfunc_m(BLKMPITCREATERESIZED);
	return(MPI_SUCCESS);
}
