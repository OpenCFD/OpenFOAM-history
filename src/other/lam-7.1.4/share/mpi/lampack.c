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
 * $Id: lampack.c,v 6.11 2003/04/08 01:33:05 jsquyres Exp $
 *
 *	Function:	- MPI data packing functions
 *			- handles recursive data types
 */

#include <lam_config.h>

#include <string.h>

#include <mpi.h>
#include <mpisys.h>
#include <mpi_types.h>
#include <portable.h>
#include <typical.h>


/*
 * local functions
 */
static void pack(char *dest, char *src, int count, MPI_Datatype dtype);
static void pack_rev(char *dest, char *src, int count, MPI_Datatype dtype);
static void pack_hvector(char *dest, char *src, int num, MPI_Datatype dtype, 
                         int fl_vec);
static void pack_rev_hvector(char *dest, char *src, int num, 
                             MPI_Datatype dtype, int fl_vec);
static void pack_hindexed(char *dest, char *src, int num, MPI_Datatype dtype,
                          int fl_idx);
static void pack_rev_hindexed(char *dest, char *src, int num, 
                              MPI_Datatype dtype, int fl_idx);
static void pack_struct(char *dest, char *src, int num, MPI_Datatype dtype);
static void pack_rev_struct(char *dest, char *src, int num, 
                            MPI_Datatype dtype);


/*
 *	lam_pack
 *
 *	Function:	- pack a message into contiguous space
 *			- packing is done recursively
 *			- insufficient destination packing buffer is an error
 *	Accepts:	- source buffer
 *			- count
 *			- datatype
 *			- destination buffer
 *			- length of destination buffer
 *	Returns:	- # bytes packed or LAMERROR
 */
int
lam_pack(char *src, int count, MPI_Datatype dtype, char *dest, int destlen)
{
	int		size;			/* # bytes required */
/*
 * Check for sufficent packing buffer space.
 */
	size = count * dtype->dt_size;

	if (destlen < size) {
	    return(LAMERROR);
	}
/*
 * Pack the buffer.  Convert to big-endian format if the LAM is
 * heterogeneous and the local archirecture is little-endian.  
 */
	if (size > 0) {
#if LAM_WORDS_BIGENDIAN
		pack(dest, src, count, dtype);
#else
		if (lam_homog) {
			pack(dest, src, count, dtype);
		} else {
			pack_rev(dest, src, count, dtype);
		}
#endif		
	}

	return(size);
}


/*
 *	pack
 *
 *	Function:	- pack a message into contiguous space
 *			- packing is done recursively
 *	Accepts:	- destination buffer
 *			- source buffer
 *			- count
 *			- datatype
 */
static void
pack(char *dest, char *src, int count, MPI_Datatype dtype)
{
/*
 * If no packing or extent adjustment is required do a direct copy.
 * This is always the case for basic types.
 */
	if ((dtype->dt_flags & LAM_DTNOPACK)
	    	&& ((count == 1) || (dtype->dt_flags & LAM_DTNOXADJ))) {
	  lam_memcpy(dest, src, count * dtype->dt_size);
	}
	else {
	    switch (dtype->dt_format) {

	    case LAM_DTCONTIG:
		pack(dest, src, count * dtype->dt_count, dtype->dt_dtype);
		break;

	    case LAM_DTVECTOR:
		pack_hvector(dest, src, count, dtype, 1);
		break;

	    case LAM_DTHVECTOR:
	    case LAM_DTHVECTORCREAT:
		pack_hvector(dest, src, count, dtype, 0);
		break;

	    case LAM_DTINDEXED:
		pack_hindexed(dest, src, count, dtype, 1);
		break;

	    case LAM_DTHINDEXED:
	    case LAM_DTHINDEXEDCREAT:
		pack_hindexed(dest, src, count, dtype, 0);
		break;

	    case LAM_DTSTRUCT:
	    case LAM_DTSTRUCTCREAT:
	    case LAM_DTSUBARRAY:
	    case LAM_DTDARRAY:
		pack_struct(dest, src, count, dtype);
		break;
	    }
	}
}


/*
 *	pack_rev
 *
 *	Function:	- pack and reverse a message into contiguous space
 *			- packing is done recursively
 *	Accepts:	- destination buffer
 *			- source buffer
 *			- count
 *			- datatype
 */
static void
pack_rev(char *dest, char *src, int count, MPI_Datatype dtype)
{
	switch (dtype->dt_format) {
/*
 * Select optimal copy and reverse function.
 */
	case LAM_DTBASIC:
	    switch (dtype->dt_size) {

	    case 1:
	        lam_memcpy(dest, src, count);
		break;
		
	    case 2:
		lam_copyrev2(dest, src, count);
		break;
		
	    case 4:	
		lam_copyrev4(dest, src, count);
		break;

	    case 8:
		lam_copyrev8(dest, src, count);
		break;

	    default:
		lam_copyrevn(dest, src, dtype->dt_size, count);
	    }

	    break;
	
	case LAM_DTCONTIG:
	    pack_rev(dest, src, count * dtype->dt_count, dtype->dt_dtype);
	    break;

	case LAM_DTVECTOR:
	    pack_rev_hvector(dest, src, count, dtype, 1);
	    break;

	case LAM_DTHVECTOR:
	case LAM_DTHVECTORCREAT:
	    pack_rev_hvector(dest, src, count, dtype, 0);
	    break;

	case LAM_DTINDEXED:
	    pack_rev_hindexed(dest, src, count, dtype, 1);
	    break;

	case LAM_DTHINDEXED:
	case LAM_DTHINDEXEDCREAT:
	    pack_rev_hindexed(dest, src, count, dtype, 0);
	    break;

	case LAM_DTSTRUCT:
	case LAM_DTSTRUCTCREAT:
	case LAM_DTSUBARRAY:
	case LAM_DTDARRAY:
	    pack_rev_struct(dest, src, count, dtype);
	    break;
	}
}		


/*
 *	pack_hvector
 *
 *	Function:	- pack vector or hvector
 */
static void
pack_hvector(char *dest, char *src, int num, MPI_Datatype dtype, int fl_vec)
{
	MPI_Datatype	subtype;		/* type of vector elements */
	char		*s;			/* ptr into source buffer */
	int		blksize;		/* size of block */
	int		count;			/* number of blocks */
	int		extent;			/* datatype extent */
	int		stride;			/* stride in bytes */
	int		i, j;

	subtype = dtype->dt_dtype;
	extent = dtype->dt_upper - dtype->dt_lower;
	count = dtype->dt_count;

	stride = dtype->dt_stride;
	if (fl_vec) {
	    stride *= subtype->dt_upper - subtype->dt_lower;
	}

	blksize = dtype->dt_length * subtype->dt_size;

	if ((subtype->dt_flags & LAM_DTNOPACK) && ((dtype->dt_length == 1)
	    	|| (subtype->dt_flags & LAM_DTNOXADJ))) {
/*
 * The subtype blocks do not need to be packed so just loop through the
 * vector copying them.  If the buffers are sufficiently aligned, the
 * blocks are of size 4 or 8, and the stride and extent are multiples
 * of the block size then assignment is used to eliminate memcpy overhead.  
 */
	    if (blksize == 4 && (stride & 0x3) == 0 && (extent & 0x3) == 0 
		    && ((MPI_Aint)dest & 0x3) == 0
		    && ((MPI_Aint)src & 0x3) == 0) {

		uint4	*d = (uint4 *) dest;
		uint4	*s;

		for (i = 0; i < num; ++i, src += extent) {
		    s = (uint4 *) src;
		    for (j = 0; j < count; ++j) {
			d[j] = *s;
			s = (uint4 *) ((char *)s + stride);
		    }
		    d += count;
		}
	    }
	    else if (blksize == 8 && (stride & 0x7) == 0 && (extent & 0x7) == 0
		    && ((MPI_Aint)dest & 0x7) == 0
		    && ((MPI_Aint)src & 0x7) == 0) {

		float8	*d = (float8 *) dest;
		float8	*s;

		for (i = 0; i < num; ++i, src += extent) {
		    s = (float8 *) src;
		    for (j = 0; j < count; ++j) {
			d[j] = *s;
			s = (float8 *) ((char *)s + stride);
		    }
		    d += count;
		}
	    }
	    else {
		for (i = 0; i < num; ++i, src += extent) {
		    s = src;
		    for (j = 0; j < count; ++j) {
			lam_memcpy(dest, s, blksize);
			dest += blksize;
			s += stride;
		    }
		}
	    }
	}
/*
 * Subtype blocks need to be packed.
 */
	else {
	    for (i = 0; i < num; ++i, src += extent) {
		s = src;
		for (j = 0; j < count; ++j) {
		    pack(dest, s, dtype->dt_length, dtype->dt_dtype);
		    dest += blksize;
		    s += stride;
		}
	    }
	}
}


/*
 *	pack_rev_hvector
 *
 *	Function:	- pack and reverse vector or hvector
 */
static void
pack_rev_hvector(char *dest, char *src, int num, MPI_Datatype dtype, 
		 int fl_vec)
{
	MPI_Datatype	subtype;		/* type of vector elements */
	char		*s;			/* ptr into source buffer */
	int		blksize;		/* size of block */
	int		extent;			/* datatype extent */
	int		stride;			/* stride in bytes */
	int		i, j;

	subtype = dtype->dt_dtype;
	extent = dtype->dt_upper - dtype->dt_lower;

	stride = dtype->dt_stride;
	if (fl_vec) {
	    stride *= subtype->dt_upper - subtype->dt_lower;
	}

	blksize = dtype->dt_length * subtype->dt_size;

	if (subtype->dt_format == LAM_DTBASIC) {
/*
 * Select the optimal copy and byte reversal routine.
 */
	    switch (subtype->dt_size) {
	    case 0:
		break;

	    case 1:
		for (i = 0; i < num; ++i, src += extent) {
		    s = src;
		    for (j = 0; j < dtype->dt_count; ++j) {
			lam_memcpy(dest, s, dtype->dt_length);
			dest += blksize;
			s += stride;
		    }
		}
		break;
		
	    case 2:
		for (i = 0; i < num; ++i, src += extent) {
		    s = src;
		    for (j = 0; j < dtype->dt_count; ++j) {
			lam_copyrev2(dest, s, dtype->dt_length);
			dest += blksize;
			s += stride;
		    }
		}
		break;
		
	    case 4:	
		for (i = 0; i < num; ++i, src += extent) {
		    s = src;
		    for (j = 0; j < dtype->dt_count; ++j) {
			lam_copyrev4(dest, s, dtype->dt_length);
			dest += blksize;
			s += stride;
		    }
		}
		break;

	    case 8:
		for (i = 0; i < num; ++i, src += extent) {
		    s = src;
		    for (j = 0; j < dtype->dt_count; ++j) {
			lam_copyrev8(dest, s, dtype->dt_length);
			dest += blksize;
			s += stride;
		    }
		}
		break;

	    default:
		for (i = 0; i < num; ++i, src += extent) {
		    s = src;
		    for (j = 0; j < dtype->dt_count; ++j) {
			lam_copyrevn(dest, s, subtype->dt_size, 
						dtype->dt_length);
			dest += blksize;
			s += stride;
		    }
		}
	    }
	}
	else {
	    for (i = 0; i < num; ++i, src += extent) {
		s = src;
		for (j = 0; j < dtype->dt_count; ++j) {
		    pack_rev(dest, s, dtype->dt_length, subtype);
		    dest += blksize;
		    s += stride;
		}
	    }
	}
}


/*
 *	pack_hindexed
 *
 *	Function:	- pack indexed or hindexed
 */
static void
pack_hindexed(char *dest, char *src, int num, MPI_Datatype dtype,
	      int fl_idx)
{
	int		i, j;
	int		blksize;		/* size of block */
	int		extent;			/* datatype extent */
	int		factor;			/* extent factor */
	int		*len;			/* lengths */
	MPI_Aint	*disp;			/* displacements */
	char		*s;			/* ptr into source buffer */

	extent = dtype->dt_upper - dtype->dt_lower;
	factor = (fl_idx == 0) ? 1 :
			dtype->dt_dtype->dt_upper - dtype->dt_dtype->dt_lower;

	for (i = 0; i < num; ++i, src += extent) {

	    s = src;
	    len = dtype->dt_lengths;
	    disp = dtype->dt_disps;

	    for (j = 0; j < dtype->dt_count; ++j, ++len, ++disp) {

		blksize = *len * dtype->dt_dtype->dt_size;

		if (blksize > 0) {
		    pack(dest, s + (*disp * factor), *len, dtype->dt_dtype);
		    dest += blksize;
		}
	    }
	}
}


/*
 *	pack_rev_hindexed
 *
 *	Function:	- pack and reverse indexed or hindexed
 */
static void
pack_rev_hindexed(char *dest, char *src, int num, MPI_Datatype dtype,
		  int fl_idx)
{
	int		i, j;
	int		blksize;		/* size of block */
	int		extent;			/* datatype extent */
	int		factor;			/* extent factor */
	int		*len;			/* lengths */
	MPI_Aint	*disp;			/* displacements */
	char		*s;			/* ptr into source buffer */

	extent = dtype->dt_upper - dtype->dt_lower;

	factor = (fl_idx == 0) ? 1 :
			dtype->dt_dtype->dt_upper - dtype->dt_dtype->dt_lower;

	for (i = 0; i < num; ++i, src += extent) {

	    s = src;
	    len = dtype->dt_lengths;
	    disp = dtype->dt_disps;

	    for (j = 0; j < dtype->dt_count; ++j, ++len, ++disp) {

		blksize = *len * dtype->dt_dtype->dt_size;

		if (blksize > 0) {
		    pack_rev(dest, s + (*disp * factor),
					*len, dtype->dt_dtype);
		    dest += blksize;
		}
	    }
	}
}


/*
 *	pack_struct
 *
 *	Function:	- pack struct
 */
static void
pack_struct(char *dest, char *src, int num, MPI_Datatype dtype)
{
	int		i, j;
	int		blksize;		/* size of block */
	int		extent;			/* datatype extent */
	int		*len;			/* lengths */
	MPI_Aint	*disp;			/* displacements */
	MPI_Datatype	*type;			/* datatypes */
	char		*s;			/* ptr into source buffer */

	extent = dtype->dt_upper - dtype->dt_lower;

	for (i = 0; i < num; ++i, src += extent) {

		s = src;
		len = dtype->dt_lengths;
		disp = dtype->dt_disps;
		type = dtype->dt_dtypes;

		for (j = 0; j < dtype->dt_count; ++j, ++len, ++disp, ++type) {

			blksize = *len * (*type)->dt_size;

			if (blksize > 0) {
				pack(dest, s + *disp, *len, *type);
				dest += blksize;
			}
		}
	}
}


/*
 *	pack_rev_struct
 *
 *	Function:	- pack and reverse struct
 */
static void
pack_rev_struct(char *dest, char *src, int num, MPI_Datatype dtype)
{
	int		i, j;
	int		blksize;		/* size of block */
	int		extent;			/* datatype extent */
	int		*len;			/* lengths */
	MPI_Aint	*disp;			/* displacements */
	MPI_Datatype	*type;			/* datatypes */
	char		*s;			/* ptr into source buffer */

	extent = dtype->dt_upper - dtype->dt_lower;

	for (i = 0; i < num; ++i, src += extent) {

		s = src;
		len = dtype->dt_lengths;
		disp = dtype->dt_disps;
		type = dtype->dt_dtypes;

		for (j = 0; j < dtype->dt_count; ++j, ++len, ++disp, ++type) {

			blksize = *len * (*type)->dt_size;

			if (blksize > 0) {
				pack_rev(dest, s + *disp, *len, *type);
				dest += blksize;
			}
		}
	}
}
