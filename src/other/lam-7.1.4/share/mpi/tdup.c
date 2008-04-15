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
 * $Id: tdup.c,v 1.5 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- duplicate a datatype
 *	Accepts:	- type to be duplicated
 *			- new duplicate type (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
    MPI_Type_dup - Duplicate a datatype

Input Parameter:
+ type - datatype (handle)

Output Parameter:
+ newtype - Copy of type (handle)

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_TYPE
.N MPI_ERR_OTHER
.N MPI_ERR_INTERN

.N WEB
@*/

int MPI_Type_dup(MPI_Datatype type, MPI_Datatype *newtype)
{
    MPI_Datatype	ntype;			/* new datatype */
    struct _attr	elem;			/* attr. element */
    struct _attr	*p;			/* favourite pointer */
    struct _attrkey	*pk;			/* key pointer */
    int			err;			/* error code */
    int			fl_copy;		/* attr. copy flag */

    lam_initerr_m();
    lam_setfunc_m(BLKMPITDUP);

    if (type == MPI_DATATYPE_NULL) {
	return(lam_errfunc(MPI_COMM_WORLD,
	    		BLKMPITDUP, lam_mkerr(MPI_ERR_TYPE, EINVAL)));
    }

    ntype = (MPI_Datatype) malloc(sizeof(struct _dtype));
    if (ntype == 0) {
	return(lam_errfunc(MPI_COMM_WORLD,
	    		BLKMPITDUP, lam_mkerr(MPI_ERR_OTHER, errno)));
    }

    *ntype = *type;

    ntype->dt_uargs = 0;
    ntype->dt_refcount = 1;
    ntype->dt_f77handle = -1;
    ntype->dt_label = -1;
    ntype->dt_flags &= ~(LAM_PREDEF | LAM_DTISRESIZE);
    ntype->dt_flags |= LAM_DTISDUP;
    memset(ntype->dt_name, 0, sizeof(ntype->dt_name));

    ntype->dt_uargs = (void *) type;
    type->dt_refcount++;
/*
 * Copy the key attributes using the copy callback functions.
 */
    if (type->dt_keys) {

	ntype->dt_keys = ah_init((int4) ah_size(type->dt_keys),
	    (int4) sizeof(struct _attr), (int4) MPI_KEYVAL_INVALID, INT4_NIL);

	if (ntype->dt_keys == 0) {
	    return(lam_errfunc(MPI_COMM_WORLD, BLKMPITDUP,
		lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	p = (struct _attr *) ah_top(type->dt_keys);

	while (p) {
	    pk = lam_getattr(p->a_key);
	    if (pk == 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPITDUP,
		    		lam_mkerr(MPI_ERR_INTERN, errno)));
	    }
/*
 * Call the C or F77 callback function.
 */
	    fl_copy = 0;
	    if ((MPI_Type_copy_attr_function *) (pk->ak_copy)
			!= MPI_TYPE_NULL_COPY_FN) {
		
		if (pk->ak_flags & LAM_LANGF77) {
		    (*((MPI_F_copy_function *) (pk->ak_copy)))
			(&type->dt_f77handle, &p->a_key, (int *) &pk->ak_extra,
			(int *) &p->a_value, (int *) &(elem.a_value),
			&fl_copy, &err);
		} else {
		    err = (*((MPI_Type_copy_attr_function *)(pk->ak_copy)))
			(type, p->a_key, pk->ak_extra,
			p->a_value, &(elem.a_value), &fl_copy);
		}
	    }

	    if (err != MPI_SUCCESS) {
		return(lam_errfunc(MPI_COMM_WORLD,
				   BLKMPITDUP, lam_mkerr(err, EINVAL)));
	    }	
/*
 * If attribute is copied, insert it and increment key reference count.
 */
	    if (fl_copy) {
		elem.a_key = p->a_key;

		if (ah_insert(ntype->dt_keys, &elem)) {
		    return(lam_errfunc(MPI_COMM_WORLD, BLKMPITDUP,
			lam_mkerr(MPI_ERR_OTHER, errno)));
		}

		pk->ak_refcount++;
	    }

	    p = (struct _attr *) ah_next(type->dt_keys, p);
	}
    }

    *newtype = ntype;

    lam_resetfunc_m(BLKMPITDUP);
    return(MPI_SUCCESS);
}
