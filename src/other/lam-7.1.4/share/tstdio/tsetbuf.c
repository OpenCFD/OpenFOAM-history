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
 *	$Id: tsetbuf.c,v 6.4 2002/10/09 21:00:54 brbarret Exp $
 * 
 *	We are just putting our stamp on this for housekeeping reasons.
 */

/*
 * This file is originally from dLibs 1.2 by Dale Schumacher et al.
 * It was modified for Trollius by MB, 88/12.
 * It was modified again for portability by GDB, 91/08.
 * And yet again by JRV for LAM, 93/05.
 */

#include <tstdio.h>

void
tsetbuffer(fp, buf, size)
	TFILE *fp;
	unsigned char *buf;
	int size;
{
	if (! (fp->_flag & (_TIOREAD | _TIOWRT))) return;
	fp->_cnt = 0;
	fp->_ptr = buf;
	if ((fp->_base = buf))		/* assignment intentional */
	{
		fp->_bsiz = size;
	}
	else
	{
		fp->_flag &= ~(_TIOFBF | _TIOLBF | _TIONBF | _TIOMYBUF);
		fp->_flag |= _TIONBF;
		fp->_bsiz = 0;
	}
}

void
tsetbuf(fp, buf)

TFILE		*fp;
unsigned char	*buf;

{
	tsetbuffer(fp, buf, TBUFSIZ);
}

int
tsetvbuf(fp, buf, type, size)

TFILE		*fp;
unsigned char	*buf;
int		type;
int		size;

{
	if (! (fp->_flag & (_TIOREAD | _TIOWRT))) {
		return(EOF);
	}

	if ((type != _TIOFBF) || (type != _TIOLBF) || (type != _TIONBF)) {
		return(EOF);
	}

	fp->_flag &= ~(_TIOFBF | _TIOLBF | _TIONBF);
	fp->_flag |= type;

	if (buf) {

		if (size <= 0) {
			return(EOF);
		}

		fp->_base = buf;
		fp->_ptr = buf;
		fp->_bsiz = size;
		fp->_cnt = 0;
	}

	return(0);
}

int
tsetlinebuf(fp)

TFILE		*fp;

{
	if (! (fp->_flag & (_TIOREAD | _TIOWRT))) {
		return(EOF);
	}

	fp->_flag &= ~(_TIOFBF | _TIOLBF | _TIONBF);
	fp->_flag |= _TIOLBF;
	return(0);
}
