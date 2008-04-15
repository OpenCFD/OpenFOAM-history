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
 *
 *	$Id: tfflush.c,v 6.4 2002/10/09 21:00:53 brbarret Exp $
 *
 *	We are just putting our stamp on this for housekeeping reasons.
 */

/*
 * This file is originally from dLibs 1.2 by Dale Schumacher et al.
 * It was modified for Trollius by MB, 88/12.
 * It was modified again for portability by GDB, 91/08.
 * And yet again by JRV for LAM, 93/05.
 */

#include <sys/types.h>
#include <tstdio.h>

#include <laminternal.h>

/*
 *	implementation note:  This function has the side effect of
 *	re-aligning the virtual file pointer (in the buffer) with
 *	the actual file pointer (in the file) and is therefore used
 *	in other functions to accomplish this re-sync operation.
 */

static int
_fflush(fp)
	TFILE *fp;
{
	int f, rv = 0;
	long offset;

 	if(fp == NULL)
		return(0);
	f = fp->_flag;
	if((f & _TIONBF)			/* MB: file is not buffered */
	|| (!(f & (_TIOREAD | _TIOWRT))))		/* file not open! */
		return(0);
	if(fp->_cnt)				/* data in the buffer */
	{
		if(f & _TIORW)				/* writing */
		{
			if((lam_iomap_write(f, fp->_file, (char *) fp->_base,
				(long) fp->_cnt)) != fp->_cnt)
			{
				fp->_flag |= _TIOERR;
				rv = EOF;
			}

		} else {				/* reading */

			offset = (long) -(fp->_cnt);
			if(lam_iomap_lseek(f, fp->_file, offset, 1) < 0)
				rv = EOF;
		}
	}
	fp->_ptr = fp->_base;
	fp->_cnt = 0;
	return(rv);
}

int
tfflush(fp)
	TFILE *fp;
{
	int f, i;

 	if(fp)
		return(_fflush(fp));
	else {
		for(i=0; i<_TNFILE; ++i) {
			f = _tiob[i]._flag;
			if(f & (_TIOREAD | _TIOWRT))
				(void) _fflush(&_tiob[i]);
		}
		return(0);
	}
}
