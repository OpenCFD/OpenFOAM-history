/*
 * Copyright (c) 2001-2004 The Trustees of Indiana University.  
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
 * $Id: lamdtype.c,v 6.12 2003/09/11 12:26:04 jsquyres Exp $
 *
 *	Function:	- miscellaneous and conversion datatype functions
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <mpi.h>
#include <mpisys.h>
#include <mpitrace.h>
#include <mpi_types.h>
#include <rpisys.h>

/*
 * global functions
 */
int			lam_type_free();
void			lam_type_setdefaults();
int                     lam_bottom_dtsnd();
int                     lam_bottom_dtrcv();
int			lam_dtsndrcv();
void			lam_dtcpy();
void			lam_dtblock();
void			lam_dtalign();
int			lam_dtbuffer();
void			lam_copyrev2();
extern void		lam_copyrev4();
extern void		lam_copyrev8();
extern void		lam_copyrevn();

/*
 * external functions
 */
extern void		lam_tr_dtypefree();

/*
 * local functions
 */
static void cpy_hvector(char *dest, char *src, int num, MPI_Datatype dtype, 
                        int fl_vec);
static void cpy_hindexed(char *dest, char *src, int num, MPI_Datatype dtype, 
                         int fl_idx);
static void cpy_struct(char *dest, char *src, int num, MPI_Datatype dtype);


/*
 *	lam_type_free
 *
 *	Function:	- really free a datatype
 *	Accepts:	- datatype
 *	Returns:	- 0 or LAMERROR
 */
int
lam_type_free(MPI_Datatype dt)
{
    MPI_Datatype	subtype;
    struct _attr	*p;
    int			key;
    int			err;			/* error code */
    int			i;
/*
 * Recursively free all datatype children.
 */
    if (dt->dt_flags & (LAM_DTISDUP | LAM_DTISRESIZE)) {
	subtype = (MPI_Datatype) dt->dt_uargs;
	if (!(subtype->dt_flags & LAM_PREDEF)) {
	    err = MPI_Type_free(&subtype);
	    if (err != MPI_SUCCESS) {
		return(err);
	    }
	}
    }
    else {
	switch(dt->dt_format) {

	case LAM_DTCONTIG:
	case LAM_DTVECTOR:
	case LAM_DTHVECTOR:
	case LAM_DTHVECTORCREAT:
	case LAM_DTINDEXED:
	case LAM_DTHINDEXED:
	case LAM_DTHINDEXEDCREAT:

	    if (!(dt->dt_dtype->dt_flags & LAM_PREDEF)) {
		err = MPI_Type_free(&(dt->dt_dtype));
		if (err != MPI_SUCCESS) return(err);
	    }

	    if (dt->dt_format == LAM_DTHINDEXED ||
                dt->dt_format == LAM_DTHINDEXEDCREAT ||
                dt->dt_format == LAM_DTINDEXED) {
		if (dt->dt_disps) {
		    free((char *) dt->dt_disps);
		}
	    }

	    break;

	case LAM_DTINDEXEDBLK:
	    return(lam_mkerr(MPI_ERR_ARG, EINVAL));

	case LAM_DTSTRUCT:
	case LAM_DTSTRUCTCREAT:
	case LAM_DTDARRAY:
	case LAM_DTSUBARRAY:

	    for (i = 0; i < dt->dt_count; ++i) {
		if (!(dt->dt_dtypes[i]->dt_flags & LAM_PREDEF)) {
		    err = MPI_Type_free(&(dt->dt_dtypes[i]));
		    if (err != MPI_SUCCESS) return(err);
		}
	    }

	    if (dt->dt_dtypes) {
		free((char *) dt->dt_dtypes);
	    }

	    if (dt->dt_uargs) {
		free((char *) dt->dt_uargs);
	    }

	    if (dt->dt_format == LAM_DTDARRAY
	    	|| dt->dt_format ==  LAM_DTSUBARRAY) {

		if (!(dt->dt_dtype->dt_flags & LAM_PREDEF)) {
		    err = MPI_Type_free(&(dt->dt_dtype));
		    if (err != MPI_SUCCESS) return(err);
		}
	    }

	    break;

	case LAM_DTBASIC:
	    break;

	default:
	    return(lam_mkerr(MPI_ERR_ARG, EINVAL));
	}
    }
/*
 * Free the attribute keys of the top datatype.
 */
    if (dt->dt_keys) {
	p = (struct _attr *) ah_top(dt->dt_keys);
	while (p) {
	    key = p->a_key;
	    err = MPI_Type_delete_attr(dt, key);
	    if (err != MPI_SUCCESS) return(err);

	    err = MPI_Type_free_keyval(&key);
	    if (err != MPI_SUCCESS) return(err);

	    p = (struct _attr *) ah_next(dt->dt_keys, (void *) p);
	}

	ah_free(dt->dt_keys);
    }
/*
 * Remove the fortran handle if the user called MPI_Datatype_f2c on
 * this handle
 */
	if (dt->dt_f77handle >= 0)
	  lam_F_free_hdl(dt->dt_f77handle);
/*
 * Free the top datatype.
 */
    lam_tr_dtypefree(dt);
    if (!(dt->dt_flags & LAM_PREDEF)) {
	free((char *) dt);
    }

    return(MPI_SUCCESS);
}


/*
 *	lam_type_setdefaults
 *
 *	Function:	- set fields in datatype to defaults
 *	Accepts:	- datatype
 */
void
lam_type_setdefaults(MPI_Datatype dt)
{
	dt->dt_align = 1;
	dt->dt_commit = 0;
	dt->dt_count = 0;
	dt->dt_disps = 0;
	dt->dt_dtype = MPI_DATATYPE_NULL;
	dt->dt_dtypes = 0;
	dt->dt_f77handle = -1;
	dt->dt_flags = 0;
	dt->dt_keys = 0;
	dt->dt_label = -1;
	dt->dt_length = 0;
	dt->dt_lengths = 0;
	dt->dt_nelem = 0;
	dt->dt_refcount = 1;
	dt->dt_size = -1;
	dt->dt_stride = 0;
	dt->dt_uargs = 0;

	dt->dt_datalow = 0;
	dt->dt_lower = 0;
	dt->dt_dataup = 0;
	dt->dt_upper = 0;

	memset(dt->dt_name, 0, sizeof(dt->dt_name));
}

/*
 *	lam_dtsnd
 *
 *	Function:	- copy MPI message from a process buffer into 
 *                        LAM buffer
 *			- send/recv done if cannot optimize
 *	Accepts:	- send buffer
 *			- send count
 *			- send datatype
 *			- receive buffer
 *			- receive count
 *			- receive datatype
 *	      	        - tag
 *			- communicator
 *	Returns:	- MPI_SUCCESS or error code
 */
int 
lam_bottom_dtsnd(void *sbuf, int scount, MPI_Datatype sdtype, void *rbuf,
		 int rcount, MPI_Datatype rdtype, int tag, MPI_Comm comm)
{
  int		err;			/* error code */
  int		size;			/* packed size */
  int		rank;			/* caller's rank */
  MPI_Status	stat;			/* status info */

  err = lam_mkerr(MPI_ERR_TRUNCATE, EIO);

  /*
   * If same datatypes used, just copy.
   */
  if (sdtype == rdtype) {
    if (scount <= rcount) {
      if (sbuf == MPI_BOTTOM) {
	lam_pack(sbuf, scount, sdtype, rbuf, 
		 scount * sdtype->dt_size);
      } else {
	lam_dtcpy((char *) rbuf, (char *) sbuf, rcount,
		  rdtype);
      }
      err = MPI_SUCCESS;
    }
  }

  /*
   * If receive packed.
   */
  else if (rdtype == MPI_PACKED) {
    MPI_Pack_size(scount, sdtype, MPI_COMM_WORLD, &size);
    if (size <= rcount) {
      if (lam_pack(sbuf, scount, sdtype,
		   rbuf, rcount) == size) {
	err = MPI_SUCCESS;
      }
    }
  }
  /*
   * If send packed.
   */
  else if (sdtype == MPI_PACKED) {
    MPI_Pack_size(rcount, rdtype, MPI_COMM_WORLD, &size);
    if (size >= scount) {
      if (lam_unpack(sbuf, scount,
		     rbuf, rcount, rdtype) == scount) {
	err = MPI_SUCCESS;
      }
    }
  }
  /*
   * Let send/recv handle it.
   */
  else {
    MPI_Comm_rank(comm, &rank);
    err = MPI_Sendrecv(sbuf, scount, sdtype, rank, tag,
		       rbuf, rcount, rdtype, rank, tag, comm, &stat);
  }
  
  return(err);
}

/*
 *	lam_bottom_dtrcv
 *
 *	Function:	- copy MPI message from a LAM buffer into 
 *                        process buffer
 *			- send/recv done if cannot optimize
 *	Accepts:	- send buffer
 *			- send count
 *			- send datatype
 *			- receive buffer
 *			- receive count
 *			- receive datatype
 *	      	        - tag
 *			- communicator
 *	Returns:	- MPI_SUCCESS or error code
 */
int 
lam_bottom_dtrcv(void *sbuf, int scount, MPI_Datatype sdtype, void *rbuf,
		 int rcount, MPI_Datatype rdtype, int tag, MPI_Comm comm)
{
  int		err;			/* error code */
  int		size;			/* packed size */
  int		rank;			/* caller's rank */
  MPI_Status	stat;			/* status info */

  err = lam_mkerr(MPI_ERR_TRUNCATE, EIO);

  /*
   * If same datatypes used, just copy.
   */
  if (sdtype == rdtype) {
    if (scount <= rcount) {
      if (rbuf == MPI_BOTTOM) {
	lam_unpack(sbuf, rcount * rdtype->dt_size, rbuf,
		 rcount, rdtype);
      } else {
	lam_dtcpy((char *) rbuf, (char *) sbuf, rcount,
		  rdtype);
      }
      err = MPI_SUCCESS;
    }
  }

  /*
   * If receive packed.
   */
  else if (rdtype == MPI_PACKED) {
    MPI_Pack_size(scount, sdtype, MPI_COMM_WORLD, &size);
    if (size <= rcount) {
      if (lam_pack(sbuf, scount, sdtype,
		   rbuf, rcount) == size) {
	err = MPI_SUCCESS;
      }
    }
  }
  /*
   * If send packed.
   */
  else if (sdtype == MPI_PACKED) {
    MPI_Pack_size(rcount, rdtype, MPI_COMM_WORLD, &size);
    if (size >= scount) {
      if (lam_unpack(sbuf, scount,
		     rbuf, rcount, rdtype) == scount) {
	err = MPI_SUCCESS;
      }
    }
  }
  /*
   * Let send/recv handle it.
   */
  else {
    MPI_Comm_rank(comm, &rank);
    err = MPI_Sendrecv(sbuf, scount, sdtype, rank, tag,
		       rbuf, rcount, rdtype, rank, tag, comm, &stat);
  }
  
  return(err);
}

/*
 *	lam_dtsndrcv
 *
 *	Function:	- copy MPI message from buffer into another
 *			- send/recv done if cannot optimize
 *	Accepts:	- send buffer
 *			- send count
 *			- send datatype
 *			- receive buffer
 *			- receive count
 *			- receive datatype
 *			- tag
 *			- communicator
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_dtsndrcv(void *sbuf, int scount, MPI_Datatype sdtype, void *rbuf,
	     int rcount, MPI_Datatype rdtype, int tag, MPI_Comm comm)
{
	int		err;			/* error code */
	int		size;			/* packed size */
	int		rank;			/* caller's rank */
	MPI_Status	stat;			/* status info */

	err = lam_mkerr(MPI_ERR_TRUNCATE, EIO);
/*
 * If same datatypes used, just copy.
 */
	if (sdtype == rdtype) {
		if (scount <= rcount) {
		    lam_dtcpy((char *) rbuf, (char *) sbuf, rcount,
			      rdtype);
		  err = MPI_SUCCESS;
		}
	}
/*
 * If receive packed.
 */
	else if (rdtype == MPI_PACKED) {
		MPI_Pack_size(scount, sdtype, MPI_COMM_WORLD, &size);
		if (size <= rcount) {
			if (lam_pack(sbuf, scount, sdtype,
						rbuf, rcount) == size) {
				err = MPI_SUCCESS;
			}
		}
	}
/*
 * If send packed.
 */
	else if (sdtype == MPI_PACKED) {
		MPI_Pack_size(rcount, rdtype, MPI_COMM_WORLD, &size);
		if (size >= scount) {
			if (lam_unpack(sbuf, scount,
					rbuf, rcount, rdtype) == scount) {
				err = MPI_SUCCESS;
			}
		}
	}
/*
 * Let send/recv handle it.
 */
	else {
		MPI_Comm_rank(comm, &rank);
		err = MPI_Sendrecv(sbuf, scount, sdtype, rank, tag,
				rbuf, rcount, rdtype, rank, tag, comm, &stat);
	}

	return(err);
}


/*
 *	lam_dtcpy
 *
 *	Function:	- copy a datatype from a buffer into another
 *			- copying is done recursively
 *	Accepts:	- destination buffer
 *			- source buffer
 *			- count
 *			- datatype
 */
void
lam_dtcpy(char *dest, char *src, int count, MPI_Datatype dtype)
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
	    switch(dtype->dt_format) {

	    case LAM_DTCONTIG:
		lam_dtcpy(dest, src, count * dtype->dt_count, dtype->dt_dtype);
		break;

	    case LAM_DTVECTOR:
		cpy_hvector(dest, src, count, dtype, 1);
		break;

	    case LAM_DTHVECTOR:
	    case LAM_DTHVECTORCREAT:
		cpy_hvector(dest, src, count, dtype, 0);
		break;

	    case LAM_DTINDEXED:
		cpy_hindexed(dest, src, count, dtype, 1);
		break;

	    case LAM_DTHINDEXED:
	    case LAM_DTHINDEXEDCREAT:
		cpy_hindexed(dest, src, count, dtype, 0);
		break;

	    case LAM_DTSTRUCT:
	    case LAM_DTSTRUCTCREAT:
	    case LAM_DTSUBARRAY:
	    case LAM_DTDARRAY:
		cpy_struct(dest, src, count, dtype);
		break;
	    }
	}
}


/*
 *	cpy_hvector
 *
 *	Function:	- copy vector or hvector
 */
static void
cpy_hvector(char *dest, char *src, int num, MPI_Datatype dtype, int fl_vec)
{
	MPI_Datatype	subtype;		/* type of vector elements */
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
 * The subtype blocks are contiguous so just loop through the vector
 * copying them.  If the buffers are sufficiently aligned, the blocks
 * are of size 4 or 8, and the stride and extent are a multiple of the
 * block size then assignment is used to eliminate memcpy overhead.  
 */
	    if (blksize == 4 && (stride & 0x3) == 0 && (extent & 0x3) == 0 
		    && ((MPI_Aint)dest & 0x3) == 0
		    && ((MPI_Aint)src & 0x3) == 0) {

		uint4	*d, *s;
		
		stride /= 4;

		for (i = 0; i < num; ++i, src += extent, dest += extent) {
		    s = (uint4 *) src;
		    d = (uint4 *) dest;

		    for (j = 0; j < count; ++j) {
			d[j * stride] = s[j * stride];
		    }
		}
	    }
	    else if (blksize == 8 && (stride & 0x7) == 0 && (extent & 0x7) == 0
		    && ((MPI_Aint)dest & 0x7) == 0
		    && ((MPI_Aint)src & 0x7) == 0) {

		float8	*d, *s;

		stride /= 8;

		for (i = 0; i < num; ++i, src += extent, dest += extent) {
		    s = (float8 *) src;
		    d = (float8 *) dest;
		    
		    for (j = 0; j < count; ++j) {
			d[j * stride] = s[j * stride];
		    }
		}
	    }
	    else {
		for (i = 0; i < num; ++i, src += extent, dest += extent) {
		    for (j = 0; j < count; ++j) {
		      lam_memcpy(dest + j * stride, 
				 src + j * stride, blksize);
		    }
		}
	    }
	}
/*
 * Subtype blocks are not contiguous and need to be recursively copied.
 */
	else {
	    for (i = 0; i < num; ++i, src += extent, dest += extent) {
		for (j = 0; j < count; ++j) {
		    lam_dtcpy(dest + j * stride, src + j * stride,
				dtype->dt_length, subtype);
		}
	    }
	}
}


/*
 *	cpy_hindexed
 *
 *	Function:	- copy indexed or hindexed
 */
static void
cpy_hindexed(char *dest, char *src, int num, MPI_Datatype dtype, int fl_idx)
{
	MPI_Aint	*disp;			/* ptr displacements */
	int		*len;			/* ptr lengths */
	int		extent;			/* datatype extent */
	int		factor;			/* extent factor */
	int		i, j;

	extent = dtype->dt_upper - dtype->dt_lower;
	factor = (fl_idx == 0) ? 1 :
			dtype->dt_dtype->dt_upper - dtype->dt_dtype->dt_lower;

	for (i = 0; i < num; ++i, src += extent, dest += extent) {

	    len = dtype->dt_lengths;
	    disp = dtype->dt_disps;

	    for (j = 0; j < dtype->dt_count; ++j, ++len, ++disp) {

		lam_dtcpy(dest + (*disp * factor),
			    src + (*disp * factor), *len, dtype->dt_dtype);
	    }
	}
}


/*
 *	cpy_struct
 *
 *	Function:	- copy struct
 */
static void
cpy_struct(char *dest, char *src, int num, MPI_Datatype dtype)
{
	MPI_Aint	*disp;			/* ptr displacements */
	MPI_Datatype	*type;			/* ptr datatypes */
	int		*len;			/* ptr lengths */
	int		extent;			/* datatype extent */
	int		i, j;

	extent = dtype->dt_upper - dtype->dt_lower;

	for (i = 0; i < num; ++i, src += extent, dest += extent) {

		len = dtype->dt_lengths;
		disp = dtype->dt_disps;
		type = dtype->dt_dtypes;

		for (j = 0; j < dtype->dt_count; ++j, ++len, ++disp, ++type) {

		  lam_dtcpy(dest + *disp, src + *disp, *len, *type);
		}
	}
}


/*
 *	lam_dtblock
 *
 *	Function:	- add data block to datatype
 *			- blocks must be added in the order they appear in
 *			  the datatype contructor
 *	Accepts:	- datatype the block is being added to
 *			- block datatype
 *			- block count
 *			- block displacement
 */
void
lam_dtblock(MPI_Datatype newdtype, MPI_Datatype old, int count, int disp)
{
	int		extent;			/* extent of the block */
	int		upper;			/* upper bound of new type */
	int		lower;			/* lower bound of new type */
	int		dataup;			/* new type data upper limit */
	int		datalow;		/* new type data lower limit */
	int		pack;			/* do we need to pack? */
/*
 * Determine the new bounds and alignment constraint.
 */
	if (count > 0) {
		if (old->dt_size > 0 && old->dt_align > newdtype->dt_align) {
			newdtype->dt_align = old->dt_align;
		}

		extent = (old->dt_upper - old->dt_lower) * (count - 1);
		if (extent > 0) {
			upper = old->dt_upper + extent + disp;
			dataup = old->dt_dataup + extent + disp;
			lower = old->dt_lower + disp;
			datalow = old->dt_datalow + disp;
		} else {
			upper = old->dt_upper + disp;
			dataup = old->dt_dataup + disp;
			lower = old->dt_lower + extent + disp;
			datalow = old->dt_datalow + extent + disp;
		}
	} else {
		upper = dataup = 0;
		lower = datalow = 0;
	}
/*
 * First block in the new type.
 */
	if (newdtype->dt_size < 0) {
		newdtype->dt_upper = upper;
		newdtype->dt_dataup = dataup;
		newdtype->dt_lower = lower;
		newdtype->dt_datalow = datalow;

		newdtype->dt_flags = old->dt_flags & (LAM_DTHASUB | LAM_DTHASLB);
		newdtype->dt_size = count * old->dt_size;
		newdtype->dt_nelem = count * old->dt_nelem;

		if (count > 0) {
			newdtype->dt_flags |= (LAM_DTLOWSET | LAM_DTHIGHSET);
		}

		if ((old->dt_flags & LAM_DTNOPACK)
				&& (dataup - datalow == newdtype->dt_size)) {
			newdtype->dt_flags |= LAM_DTNOPACK;
		}
	}
/*
 * Adding a new non-null block.
 */
	else if (count > 0) {

		if (old->dt_size > 0) {

			pack = !(old->dt_flags & LAM_DTNOPACK);

			if (newdtype->dt_size == 0) {
				newdtype->dt_dataup = dataup;
				newdtype->dt_datalow = datalow;
			} else {
				pack = pack || (newdtype->dt_dataup != datalow);

				if (dataup > newdtype->dt_dataup) {
					newdtype->dt_dataup = dataup;
				}
				if (datalow < newdtype->dt_datalow) {
					newdtype->dt_datalow = datalow;
				}
			}

			newdtype->dt_size += count * old->dt_size;
			newdtype->dt_nelem += count * old->dt_nelem;

			if (pack) {
				newdtype->dt_flags &= ~LAM_DTNOPACK;
			}
		}

/*
 * Update the upper bound.
 */
		if (old->dt_flags & LAM_DTHASUB) {
			if (newdtype->dt_flags & LAM_DTHASUB) {
				if (upper > newdtype->dt_upper) {
					newdtype->dt_upper = upper;
				}
			} else {
				newdtype->dt_flags |= LAM_DTHASUB;
				newdtype->dt_upper = upper;
			}
			newdtype->dt_flags |= LAM_DTHIGHSET;
		} else {
			if (!(newdtype->dt_flags & LAM_DTHASUB)
					&& (old->dt_size > 0)) {
				newdtype->dt_upper = newdtype->dt_dataup;
				newdtype->dt_flags |= LAM_DTHIGHSET;
			}
		}
/*
 * Update the lower bound.
 */
		if (old->dt_flags & LAM_DTHASLB) {
			if (newdtype->dt_flags & LAM_DTHASLB) {
				if (lower < newdtype->dt_lower) {
					newdtype->dt_lower = lower;
				}
			} else {
				newdtype->dt_flags |= LAM_DTHASLB;
				newdtype->dt_lower = lower;
			}
			newdtype->dt_flags |= LAM_DTLOWSET;
		} else {
			if (!(newdtype->dt_flags & LAM_DTHASLB)
					&& (old->dt_size > 0)) {
				newdtype->dt_lower = newdtype->dt_datalow;
				newdtype->dt_flags |= LAM_DTLOWSET;
			}
		}
/*
 * This ugliness is required to take care of pathological cases like
 * creating a datatype with a type map {(10, MPI_UB)}.  In this
 * case the lower bound must also be set to 10.
 */
		if ((newdtype->dt_flags & LAM_DTLOWSET)
				&& !(newdtype->dt_flags & LAM_DTHIGHSET)) {
			newdtype->dt_upper = newdtype->dt_lower;
			newdtype->dt_flags |= LAM_DTHIGHSET;
		}
		else if (!(newdtype->dt_flags & LAM_DTLOWSET)
				&& (newdtype->dt_flags & LAM_DTHIGHSET)) {
			newdtype->dt_lower = newdtype->dt_upper;
			newdtype->dt_flags |= LAM_DTLOWSET;
		}
	}
}


/*
 *	lam_dtalign
 *
 *	Function:	- adjust extent of datatype to account for alignment
 *	Accepts:	- datatype
 */
void
lam_dtalign(MPI_Datatype dtype)
{
	int		extent;			/* extent of datatype */
	int		epsilon;		/* extra needed for alignment */
/*
 * No adjustment may be done if MPI_UB is part of the type map.
 */
	if (!(dtype->dt_flags & LAM_DTHASUB)) {

		extent = dtype->dt_upper - dtype->dt_lower;

		if (extent >= 0) {
			epsilon = extent % dtype->dt_align;
			if (epsilon != 0) {
				epsilon = dtype->dt_align - epsilon;
			}
		}
		else {
			epsilon = (-extent) % dtype->dt_align;
		}

		dtype->dt_upper += epsilon;
	}
/*
 * Set the no extent adjustment flag if the upper and lower bounds match
 * exactly the upper and lower limits of the data.
 */
	if (dtype->dt_upper == dtype->dt_dataup
			&& dtype->dt_lower == dtype->dt_datalow) {
		dtype->dt_flags |= LAM_DTNOXADJ;
	} else {
	    dtype->dt_flags &= ~LAM_DTNOXADJ;
	}
}


/*
 *	lam_dtbuffer
 *
 *	Function:	- allocate buffer for copies of datatype
 *	Accepts:	- datatype
 *			- count of copies
 *			- buffer (out)
 *			- origin of buffer (out)
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_dtbuffer(MPI_Datatype dtype, int count, char **buffer, char **origin)
{
	int		extent;			/* space required for extent */
	int		up;			/* upper limit */
	int		low;			/* lower limit */

	if (count <= 0) {
		*buffer = *origin = 0;
		return(MPI_SUCCESS);
	}

	extent = (dtype->dt_upper - dtype->dt_lower) * (count - 1);

	if (extent >= 0) {
		up = dtype->dt_dataup + extent;
		low = dtype->dt_datalow;
	} else {
		up = dtype->dt_dataup;
		low = dtype->dt_datalow + extent;
	}

	if (up > low) {
		*buffer = malloc((unsigned) (up - low));
		if (*buffer == 0) {
			return(lam_mkerr(MPI_ERR_OTHER, errno));
		}
		*origin = *buffer - low;
	} else {
		*buffer = *origin = 0;
	}

	return(MPI_SUCCESS);
}


/*
 *	lam_copyrev2
 *
 *	Function:	- copy and reverse byte ordering of an
 *			  array of 2-byte elements
 *			- the source and destination arrays must not overlap
 *	Accepts:	- destination array
 *			- source array
 *			- # elements in array
 */
void
lam_copyrev2(void *dest, void *src, int num)
{
	unsigned char	*d, *s;
	int		i;

	d = dest;
	s = src;
	num *= 2;

	for (i = 0; i < num; i += 2) {
		d[i] = s[i + 1];
		d[i + 1] = s[i];
	}
}


/*
 *	lam_copyrev4
 *
 *	Function:	- copy and reverse byte ordering of an
 *			  array of 4-byte elements
 *			- the source and destination arrays must not overlap
 *	Accepts:	- destination array
 *			- source array
 *			- # elements in array
 */
void
lam_copyrev4(void *dest, void *src, int num)
{
#ifdef LAM_USE_SHIFTING
	int		i;
	uint4		old;
	uint4		*s, *d;

	d = dest;
	s = src;

	for (i = 0; i < num; ++i, ++s, ++d) {
		old = *s;
		*d = (old & 0x000000FF) << 16;
		*d = (*d | (old & 0x0000FF00)) << 8;
		old >>= 8;
		*d |= (old & 0x0000FF00);
		old >>= 16;
		*d |= (old & 0x000000FF);
	}
#else
        unsigned char   *d, *s;
        int             i;

        d = dest;
        s = src;
        num *= 4;

        for (i = 0; i < num; i += 4) {
                d[i] = s[i + 3];
                d[i + 1] = s[i + 2];
                d[i + 2] = s[i + 1];
                d[i + 3] = s[i];
        }
#endif	
}


/*
 *	lam_copyrev8
 *
 *	Function:	- copy and reverse byte ordering of an
 *			  array of 8-byte elements
 *			- the source and destination arrays must not overlap
 *	Accepts:	- destination array
 *			- source array
 *			- # elements in array
 */
void
lam_copyrev8(void *dest, void *src, int num)
{
	unsigned char	*d, *s;
	int		i;

	d = dest;
	s = src;
	num *= 8;

	for (i = 0; i < num; i += 8) {
		d[i] = s[i + 7];
		d[i + 1] = s[i + 6];
		d[i + 2] = s[i + 5];
		d[i + 3] = s[i + 4];
		d[i + 4] = s[i + 3];
		d[i + 5] = s[i + 2];
		d[i + 6] = s[i + 1];
		d[i + 7] = s[i];
	}
}


/*
 *	lam_copyrevn
 *
 *	Function:	- copy and reverse byte ordering of an
 *			  array of n-byte elements
 *			- the source and destination arrays must not overlap
 *	Accepts:	- destination array
 *			- source array
 *			- element size
 *			- # elements in array
 */
void
lam_copyrevn(void *dest, void *src, int nbytes, int num)
{
	unsigned char	*d, *s;
	int		i, j;

	s = src;
	num *= nbytes;

	for (i = 0; i < num; i += nbytes) {

		d = (unsigned char *) dest + i + nbytes - 1;
		
		for (j = nbytes ; j > 0; --j, ++s, --d) {
			*d = *s;
		}
	}
}
