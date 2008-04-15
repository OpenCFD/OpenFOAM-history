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
 *	$Id: tfclose.c,v 6.4 2002/10/09 21:00:53 brbarret Exp $
 * 
 *	We are just putting our stamp on this for housekeeping reasons.
 */

/*
 * This file is originally from dLibs 1.2 by Dale Schumacher et al.
 * It was modified for Trollius by MB, 88/12.
 * It was modified again for portability by GDB, 91/08.
 * And yet again by JRV for LAM, 93/05.
 */

#include <stdlib.h>
#include <tstdio.h>

#include <laminternal.h>


int
tfclose(fp)
	TFILE *fp;
{
	int f;

	if(fp == NULL)
		return(EOF);		/* NULL file pointer file */
	f = fp->_flag;
	if((f & (_TIOREAD | _TIOWRT)) == 0)
		return(EOF);		/* file not open! */
	tfflush(fp);
	if(fp->_flag & _TIOMYBUF)	/* throw away non-standard buffer */
	{
		free(fp->_base);
	}
	fp->_base = NULL;
	fp->_ptr = NULL;
	fp->_bsiz = 0;
	fp->_flag = 0;			/* clear status */
	if(f & _TIODEV)			/* leave tty's alone */
		return(0);

	f = lam_iomap_close(f, fp->_file);

	return(f ? EOF : 0);
}

/*
 *	fcloseall
 *
 *	Function:	- Close all streams.
 */

void
tfcloseall()

{
	int i, f;

	for(i=0; i<_TNFILE; ++i) {
		f = _tiob[i]._flag;
		if(f & (_TIOREAD | _TIOWRT))
			tfclose(&_tiob[i]);
	}
}
