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
 *	Ohio Trollius
 *	Copyright 1997 The Ohio State University
 *	NJN/RBD
 *
 *	$Id: attrget_f.c,v 6.7 2002/10/09 20:59:00 brbarret Exp $
 *
 *	Function:	- MPI_Attr_get F77 wrapper
 */

#include <lam_config.h>

#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void 
mpi_attr_get_(c, k, a, f, ierr)

int	*c, *k, *a, *f, *ierr;

{
	struct _attrkey *pk;
	int *ip;

	/* The union here avoids compiler warnings, if sizeof(void *)
	   > sizeof(int). This is because casting a void * to an int
	   will result in chopping off some bits resulting in compiler
	   warnings */
	union {
	    void *voidval;
	    int intval;
	} valueptr;

	ip = &valueptr.intval;

	*ierr = MPI_Attr_get(GETHDL(*c), *k, &valueptr.voidval, f);

	if (*ierr == MPI_SUCCESS && *f) {

		pk = lam_getattr(*k);

		if (pk->ak_flags & LAM_LANGF77) {
/*
 * This stuff is very confusing.  Be sure to see MPI-2 4.12.7.
 */
/*
 * If this attribute was created in fortran, we'll get a pointer back
 * to the integer.  But if sizeof(void*) > sizeof(int), the pointer to
 * the value can be viewed as an array of integers -- we need to
 * select the lower (sizeof(int)*8) bits to return.
 */
		  *a = ((int *) valueptr.voidval)[lam_int_pos()];
		} else {
/*
 * However, if this attribute was created in C, we need to grab just
 * the lower (sizeof(int)*8) bits, since sizeof(int) may be <
 * sizeof(void*) -- don't dereference it to be an int value.
 */
		  *a = ip[lam_int_pos()];
		}
	}
}
 
