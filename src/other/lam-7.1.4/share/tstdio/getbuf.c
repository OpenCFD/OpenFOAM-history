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
 */

/*
 * This file is originally from dLibs 1.2 by Dale Schumacher et al.
 * It was modified for Trollius by MB, 88/12.
 * It was modified again for portability by GDB, 91/08.
 * And yet again by JRV for LAM, 93/05.
 */

#include <stdlib.h>

#include <tstdio.h>


void
_getbuf(fp)		/* allocate a buffer for a stream */
	register TFILE *fp;
{
	if((fp->_flag & _TIONBF)
	|| ((fp->_base = (unsigned char *)
	malloc((unsigned int) (fp->_bsiz = TBUFSIZ))) == NULL))
	{
		fp->_flag &= ~(_TIOFBF | _TIOLBF | _TIONBF);
		fp->_flag |= _TIONBF;
		fp->_base = &(fp->_ch);			/* use tiny buffer */
		fp->_bsiz = 1;
	}
	else
		fp->_flag |= _TIOMYBUF;			/* use big buffer */
	fp->_ptr = fp->_base;
	fp->_cnt = 0;		/* start out with an empty buffer */
}
