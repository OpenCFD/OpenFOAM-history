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
 * $Id: lamunpack.c,v 6.12 2003/05/21 19:17:03 jsquyres Exp $
 *
 *	Function:	- MPI data unpacking functions
 *			- handles recursive data types
 */

#include <string.h>

#include <mpi.h>
#include <mpisys.h>
#include <mpi_types.h>
#include <portable.h>
#include <t_types.h>
#include <typical.h>


/*
 * local functions
 */
static int unpack(char *dest, char *src, int srclen, int count, 
                  MPI_Datatype dtype);
static int unpack_rev(char *dest, char *src, int srclen, int count, 
                      MPI_Datatype dtype);
static int unpack_hvector(char *dest, char *src, int srclen, int num, 
                          MPI_Datatype dtype, int fl_vec);
static int unpack_rev_hvector(char *dest, char *src, int srclen, int num, 
                              MPI_Datatype dtype, int fl_vec);
static int unpack_hindexed(char *dest, char *src, int srclen, int num, 
                           MPI_Datatype dtype, int fl_idx);
static int unpack_rev_hindexed(char *dest, char *src, int srclen, int num, 
                               MPI_Datatype dtype, int fl_idx);
static int unpack_struct(char *dest, char *src, int srclen, int num, 
                         MPI_Datatype dtype);
static int unpack_rev_struct(char *dest, char *src, int srclen, int num, 
                             MPI_Datatype dtype);


/*
 *	lam_unpack
 *
 *	Function:	- unpack a message from contiguous space
 *			- unpacking is done recursively
 *	Accepts:	- source buffer
 *			- length of source buffer
 *			- destination buffer
 *			- count
 *			- datatype
 *	Returns:	- # bytes unpacked or LAMERROR
 */
int
lam_unpack(char *src, int srclen, char *dest, int count, MPI_Datatype dtype)
{
	int		size;			/* # bytes unpacked */

	if (srclen < 0) {
		return(LAMERROR);
	}
/*
 * At most count copies of the datatype need be unpacked.
 */
	size = count * dtype->dt_size;
	srclen = LAM_min(size, srclen);

	if (srclen == 0) {
		return(0);
	}
/*
 * Unpack the buffer.  Convert to little-endian format if the LAM is
 * heterogeneous and the local archirecture is little-endian.
 */
#if LAM_WORDS_BIGENDIAN
	size = unpack(dest, src, srclen, count, dtype);
#else
	if (lam_homog) {
		size = unpack(dest, src, srclen, count, dtype);
	} else {
		size = unpack_rev(dest, src, srclen, count, dtype);
	}
#endif

	return(size);
}


/*
 *	unpack
 *
 *	Function:	- unpack a message from contiguous space
 *			- unpacking is done recursively
 *	Accepts:	- destination buffer
 *			- source buffer
 *			- source buffer length
 *			- count
 *			- datatype
 *	Returns:	- # bytes unpacked
 */
static int
unpack(char *dest, char *src, int srclen, int count, MPI_Datatype dtype)
{
	int		size;			/* number bytes unpacked */

	if (dtype->dt_size == 0) {
	    return(0);
	}
/*
 * Basic type.  Unpack, up to a maximum of count copies, as many integral 
 * copies of the datatype as there are in the source buffer.
 */
	if (dtype->dt_format == LAM_DTBASIC) {

	    size = srclen / dtype->dt_size;
	    count = LAM_min(count, size);
	    size = count * dtype->dt_size;

	    lam_memcpy(dest, src, size);
	    return(size);
	}

	if ((dtype->dt_flags & LAM_DTNOPACK)
	    	&& ((count == 1) || (dtype->dt_flags & LAM_DTNOXADJ))
		&& (srclen % dtype->dt_size == 0)) {
/*
 * No unpacking is required and the source length is a multiple
 * of the datatype size.
 */
	    size = count * dtype->dt_size;
	    size = LAM_min(size, srclen);
	    lam_memcpy(dest, src, size);
	    return(size);
	}
	else {
	    switch (dtype->dt_format) {

	    case LAM_DTCONTIG:
		return(unpack(dest, src, srclen,
			count * dtype->dt_count, dtype->dt_dtype));

	    case LAM_DTVECTOR:
		return(unpack_hvector(dest, src, srclen, count, dtype, 1));

	    case LAM_DTHVECTOR:
	    case LAM_DTHVECTORCREAT:
		return(unpack_hvector(dest, src, srclen, count, dtype, 0));

	    case LAM_DTINDEXED:
		return(unpack_hindexed(dest, src, srclen, count, dtype, 1));

	    case LAM_DTHINDEXED:
	    case LAM_DTHINDEXEDCREAT:
		return(unpack_hindexed(dest, src, srclen, count, dtype, 0));

	    case LAM_DTSTRUCT:
	    case LAM_DTSTRUCTCREAT:
	    case LAM_DTSUBARRAY:
	    case LAM_DTDARRAY:
		return(unpack_struct(dest, src, srclen, count, dtype));
/*
 * Should never reach here.
 */
	    default:
		return(0);
	    }
	}
}


/*
 *	unpack_rev
 *
 *	Function:	- unpack and reverse a message from contiguous space
 *			- unpacking is done recursively
 *	Accepts:	- destination buffer
 *			- source buffer
 *			- source buffer length
 *			- count
 *			- datatype
 */
static int
unpack_rev(char *dest, char *src, int srclen, int count, MPI_Datatype dtype)
{
	if (dtype->dt_size == 0) {
	    return(0);
	}

	switch (dtype->dt_format) {

	case LAM_DTBASIC:
/*
 * Unpack and reverse an integral number of the basic datatype elements,
 * up to a maximum of count elements.
 */
	    count = LAM_min(count, srclen / dtype->dt_size);

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

	    return(count * dtype->dt_size);

	case LAM_DTCONTIG:
	    return(unpack_rev(dest, src, srclen,
				count * dtype->dt_count, dtype->dt_dtype));

	case LAM_DTVECTOR:
	    return(unpack_rev_hvector(dest, src, srclen, count, dtype, 1));

	case LAM_DTHVECTOR:
	case LAM_DTHVECTORCREAT:
	    return(unpack_rev_hvector(dest, src, srclen, count, dtype, 0));

	case LAM_DTINDEXED:
	    return(unpack_rev_hindexed(dest, src, srclen, count, dtype, 1));

	case LAM_DTHINDEXED:
	case LAM_DTHINDEXEDCREAT:
	    return(unpack_rev_hindexed(dest, src, srclen, count, dtype, 0));

	case LAM_DTSTRUCT:
	case LAM_DTSTRUCTCREAT:
	case LAM_DTSUBARRAY:
	case LAM_DTDARRAY:
	    return(unpack_rev_struct(dest, src, srclen, count, dtype));
/*
 * Should never reach here.
 */
	default:
	    return(0);
	}
}


/*
 *	unpack_hvector
 *
 *	Function:	- unpack vector or hvector
 */
static int
unpack_hvector(char *dest, char *src, int srclen, int num, MPI_Datatype dtype,
	       int fl_vec)
{
	MPI_Datatype	subtype;		/* type of vector elements */
	char		*d;			/* ptr into dest buffer */
	char		*start;			/* saves start of source */
	int		blksize;		/* size of block */
	int		count;			/* number of blocks */
	int		extent;			/* datatype extent */
	int		left;			/* # bytes left of source */
	int		size;			/* # bytes unpacked */
	int		stride;			/* stride in bytes */
	int		i, j;

	start = src;
	count = dtype->dt_count;
	extent = dtype->dt_upper - dtype->dt_lower;
	subtype = dtype->dt_dtype;
	left = LAM_min(srclen, dtype->dt_size * num);

	stride = dtype->dt_stride;
	if (fl_vec) {
	    stride *= subtype->dt_upper - subtype->dt_lower;
	}

	blksize = dtype->dt_length * subtype->dt_size;
	if (blksize == 0) {
		return(0);
	}

	if ((subtype->dt_flags & LAM_DTNOPACK) && ((dtype->dt_length == 1)
	    	|| (subtype->dt_flags & LAM_DTNOXADJ))) {
/*
 * The subtype blocks do not need to be unpacked so just loop through
 * the vector copying them.  If the buffers are sufficiently aligned,
 * the blocks are of size 4 or 8, and the stride and extent are a multiple
 * of the block size then assignment is used to eliminate memcpy overhead.  
 */
	    if (blksize == 4 && (stride & 0x3) == 0 && (extent & 0x3) == 0 
		    && ((MPI_Aint)dest & 0x3) == 0
		    && ((MPI_Aint)src & 0x3) == 0) {

		uint4	*d = (uint4 *) dest;
		uint4	*s = (uint4 *) src;

		for (i = 0; i < num && left >= 4; ++i, dest += extent) {
		    d = (uint4 *) dest;
		    for (j = 0; j < count && left >= 4; ++j) {
			*d = s[j];
			d = (uint4 *) ((char *)d + stride);
			left -= 4;
		    }
		    s += j;
		}
/*
 * Unpack last partial subtype block if any.
 */
		if (left > 0) {
		    return(((char *) s - start) + unpack((char *) d,
			    (char *) s, left, dtype->dt_length, subtype));
		} else {
		    return((char *) s - start);
		}
	    }
	    else if (blksize == 8 && (stride & 0x7) == 0 && (extent & 0x7) == 0
		    && ((MPI_Aint)dest & 0x7) == 0
		    && ((MPI_Aint)src & 0x7) == 0) {

		float8	*d = (float8 *) dest;
		float8	*s = (float8 *) src;

		for (i = 0; i < num && left >= 8; ++i, dest += extent) {
		    d = (float8 *) dest;
		    for (j = 0; j < count && left >= 8; ++j) {
			*d = s[j];
			d = (float8 *) ((char *)d + stride);
			left -= 8;
		    }
		    s += j;
		}

		if (left > 0) {
		    return(((char *) s - start) + unpack((char *) d,
			    (char *) s, left, dtype->dt_length, subtype));
		} else {
		    return((char *) s - start);
		}
	    }
	    else {
		d = dest;

		for (i = 0; i < num && left >= blksize; ++i, dest += extent) {
		    d = dest;
		    for (j = 0; j < count && left >= blksize; ++j) {
			lam_memcpy(d, src, blksize);
			src += blksize;
			left -= blksize;
			d += stride;
		    }
		}

		if (left > 0) {
		    return(src - start
			    + unpack(d, src, left, dtype->dt_length, subtype));
		} else {
		    return(src - start);
		}
	    }
	}
/*
 * Subtype blocks need to be unpacked.
 */
	else {
	    for (i = 0; i < num; ++i, dest += extent) {
		d = dest;
		for (j = 0; j < count; ++j) {
		    size = unpack(d, src, left, dtype->dt_length, subtype);
		    src += size;
/*
 * If only part of the subtype block is unpacked then we must be done.
 */
		    if (size != blksize) {
			return(src - start);
		    } else {
			left -= size;
			d += stride;
		    }
		}
	    }

	    return(src - start);
	}
}


/*
 *	unpack_rev_hvector
 *
 *	Function:	- unpack and reverse vector or hvector
 */
static int
unpack_rev_hvector(char *dest, char *src, int srclen, int num, 
		   MPI_Datatype dtype, int fl_vec)
{
	MPI_Datatype	subtype;		/* type of vector elements */
	char		*d;			/* ptr into dest buffer */
	char		*start;			/* saves start of source */
	int		blksize;		/* size of block */
	int		extent;			/* datatype extent */
	int		left;			/* # bytes left of source */
	int		size;			/* # bytes unpacked */
	int		stride;			/* stride in bytes */
	int		i, j;

	left = LAM_min(srclen, dtype->dt_size * num);
	start = src;
	d = dest;
	subtype = dtype->dt_dtype;
	extent = dtype->dt_upper - dtype->dt_lower;

	stride = dtype->dt_stride;
	if (fl_vec) {
	    stride *= subtype->dt_upper - subtype->dt_lower;
	}

	blksize = dtype->dt_length * subtype->dt_size;
	if (blksize == 0) {
	    return(0);
	}

	if (subtype->dt_format == LAM_DTBASIC) {
/*
 * Select the optimal copy and byte reversal routine.
 */
	    switch (subtype->dt_size) {

	    case 1:
		for (i = 0; i < num && left >= blksize; ++i, dest += extent) {
		    d = dest;
		    for (j = 0; j < dtype->dt_count && left >= blksize; ++j) {
			lam_memcpy(d, src, blksize);
			src += blksize;
			left -= blksize;
			d += stride;
		    }
		}
/*
 * Unpack last partial subtype block if any.
 */
		if (left > 0) {
		    lam_memcpy(d, src, left);
		    src += left;
		}

		break;

	    case 2:
		for (i = 0; i < num && left >= blksize; ++i, dest += extent) {
		    d = dest;
		    for (j = 0; j < dtype->dt_count && left >= blksize; ++j) {
			lam_copyrev2(d, src, dtype->dt_length);
			src += blksize;
			left -= blksize;
			d += stride;
		    }
		}

		if (left > 0) {
		    lam_copyrev2(d, src, left / 2);
		    src += left / 2 * 2;
		}

		break;

	    case 4:
		for (i = 0; i < num && left >= blksize; ++i, dest += extent) {
		    d = dest;
		    for (j = 0; j < dtype->dt_count && left >= blksize; ++j) {
			lam_copyrev4(d, src, dtype->dt_length);
			src += blksize;
			left -= blksize;
			d += stride;
		    }
		}

		if (left > 0) {
		    lam_copyrev4(d, src, left / 4);
		    src += left / 4 * 4;
		}

		break;

	    case 8:
		for (i = 0; i < num && left >= blksize; ++i, dest += extent) {
		    d = dest;
		    for (j = 0; j < dtype->dt_count && left >= blksize; ++j) {
			lam_copyrev8(d, src, dtype->dt_length);
			src += blksize;
			left -= blksize;
			d += stride;
		    }
		}

		if (left > 0) {
		    lam_copyrev8(d, src, left / 8);
		    src += left / 8 * 8;
		}

		break;

	    default:
		for (i = 0; i < num && left >= blksize; ++i, dest += extent) {
		    d = dest;
		    for (j = 0; j < dtype->dt_count && left >= blksize; ++j) {
			lam_copyrevn(d, src,
					subtype->dt_size, dtype->dt_length);
			src += blksize;
			left -= blksize;
			d += stride;
		    }
		}

		if (left > 0) {
		    lam_copyrevn(d, src,
				subtype->dt_size, left / subtype->dt_size);
		    src += left / subtype->dt_size * subtype->dt_size;
		}
	    }
	}
	else {
	    for (i = 0; i < num; ++i, dest += extent) {
		d = dest;
		for (j = 0; j < dtype->dt_count; ++j) {
		    size = unpack_rev(d, src, left, dtype->dt_length, subtype);
		    src += size;
/*
 * If only part of the subtype block is unpacked then we must be done.
 */
		    if (size != blksize) {
			return(src - start);
		    } else {
			left -= blksize;
			d += stride;
		    }
		}
	    }
	}

	return(src - start);
}


/*
 *	unpack_hindexed
 *
 *	Function:	- unpack indexed or hindexed
 */
static int
unpack_hindexed(char *dest, char *src, int srclen, int num, MPI_Datatype dtype,
		int fl_idx)
{
	int		i, j;
	int		size;			/* size unpacked */
	int		blksize;		/* size of block */
	int		extent;			/* datatype extent */
	int		factor;			/* extent factor */
	int		*blklen;		/* lengths */
	MPI_Aint	*disp;			/* displacements */
	char		*d;			/* ptr into dest buffer */
	char		*start;			/* saves start of source */

	start = src;
	extent = dtype->dt_upper - dtype->dt_lower;
	factor = (fl_idx == 0) ? 1 :
			dtype->dt_dtype->dt_upper - dtype->dt_dtype->dt_lower;

	for (i = 0; i < num; ++i, dest += extent) {

	    d = dest;
	    blklen = dtype->dt_lengths;
	    disp = dtype->dt_disps;

	    for (j = 0; j < dtype->dt_count; ++j, ++blklen, ++disp) {

		blksize = *blklen * dtype->dt_dtype->dt_size;

		if (blksize > 0) {
		    size = unpack(d + (*disp * factor),
					src, srclen, *blklen, dtype->dt_dtype);
		    src += size;
		    if (size != blksize) {
			return(src - start);
		    } else {
			srclen -= blksize;
		    }
		}
	    }
	}

	return(src - start);
}


/*
 *	unpack_rev_hindexed
 *
 *	Function:	- unpack and reverse indexed or hindexed
 */
static int
unpack_rev_hindexed(char *dest, char *src, int srclen, int num, 
		    MPI_Datatype dtype, int fl_idx)
{
	int		i, j;
	int		size;			/* size unpacked */
	int		blksize;		/* size of block */
	int		extent;			/* datatype extent */
	int		factor;			/* extent factor */
	int		*blklen;		/* lengths */
	MPI_Aint	*disp;			/* displacements */
	char		*d;			/* ptr into dest buffer */
	char		*start;			/* saves start of source */

	start = src;
	extent = dtype->dt_upper - dtype->dt_lower;
	factor = (fl_idx == 0) ? 1 :
			dtype->dt_dtype->dt_upper - dtype->dt_dtype->dt_lower;

	for (i = 0; i < num; ++i, dest += extent) {

	    d = dest;
	    blklen = dtype->dt_lengths;
	    disp = dtype->dt_disps;

	    for (j = 0; j < dtype->dt_count; ++j, ++blklen, ++disp) {

		blksize = *blklen * dtype->dt_dtype->dt_size;

		if (blksize > 0) {
		    size = unpack_rev(d + (*disp * factor),
					src, srclen, *blklen, dtype->dt_dtype);
		    src += size;
		    if (size != blksize) {
			return(src - start);
		    } else {
			srclen -= blksize;
		    }
		}
	    }
	}

	return(src - start);
}


/*
 *	unpack_struct
 *
 *	Function:	- unpack struct
 */
static int
unpack_struct(char *dest, char *src, int srclen, int num, MPI_Datatype dtype)
{
	int		i, j;
	int		size;			/* size unpacked */
	int		blksize;		/* size of block */
	int		extent;			/* datatype extent */
	int		*blklen;		/* lengths */
	MPI_Aint	*disp;			/* displacements */
	MPI_Datatype	*type;			/* datatypes */
	char		*d;			/* ptr into dest buffer */
	char		*start;			/* saves start of source */

	start = src;
	extent = dtype->dt_upper - dtype->dt_lower;

	for (i = 0; i < num; ++i, dest += extent) {

	    d = dest;
	    blklen = dtype->dt_lengths;
	    disp = dtype->dt_disps;
	    type = dtype->dt_dtypes;

	    for (j = 0; j < dtype->dt_count; ++j, ++blklen, ++disp, ++type) {

		blksize = *blklen * (*type)->dt_size;

		if (blksize > 0) {

		    size = unpack(d + *disp, src, srclen, *blklen, *type);
		    src += size;

		    if (size != blksize) {
			return(src - start);
		    } else {
			srclen -= blksize;
		    }
		}
	    }
	}

	return(src - start);
}


/*
 *	unpack_rev_struct
 *
 *	Function:	- unpack and reverse struct
 */
static int
unpack_rev_struct(char *dest, char *src, int srclen, int num, 
		  MPI_Datatype dtype)
{
	int		i, j;
	int		size;			/* size unpacked */
	int		blksize;		/* size of block */
	int		extent;			/* datatype extent */
	int		*blklen;		/* lengths */
	MPI_Aint	*disp;			/* displacements */
	MPI_Datatype	*type;			/* datatypes */
	char		*d;			/* ptr into dest buffer */
	char		*start;			/* save start of source */

	start = src;
	extent = dtype->dt_upper - dtype->dt_lower;

	for (i = 0; i < num; ++i, dest += extent) {

	    d = dest;
	    blklen = dtype->dt_lengths;
	    disp = dtype->dt_disps;
	    type = dtype->dt_dtypes;

	    for (j = 0; j < dtype->dt_count; ++j, ++blklen, ++disp, ++type) {

		blksize = *blklen * (*type)->dt_size;

		if (blksize > 0) {

		    size = unpack_rev(d + *disp, src, srclen, *blklen, *type);
		    src += size;

		    if (size != blksize) {
			return(src - start);
		    } else {
			srclen -= blksize;
		    }
		}
	    }
	}

	return(src - start);
}
