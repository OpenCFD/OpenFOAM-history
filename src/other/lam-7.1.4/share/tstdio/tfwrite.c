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
 *	$Id: tfwrite.c,v 6.4 2002/10/09 21:00:54 brbarret Exp $
 * 
 *	We are just putting our stamp on this for housekeeping reasons.
 */

/*
 * This file is originally from dLibs 1.2 by Dale Schumacher et al.
 * It was modified for Trollius by MB, 88/12.
 * MB: added call to atkexit() 891012
 * It was modified again for portability by GDB, 91/08.
 * And yet again by JRV for LAM, 93/05.
 */

#include <errno.h>

#include <kreq.h>
#include <laminternal.h>
#include <portable.h>
#include <tstdio.h>

/*
 * external variables
 */
extern int _ioexit;


int
tfwrite(data, size, count, fp)
	CONST char *data;
	int size;
	int count;
	TFILE *fp;
{
	int n, m;
	int f;

	if (! _ioexit) {
		if (atkexit (tfcloseall, (char *) 0) != 0)
			return (-1);
		_ioexit = 1;
	}

	f = (fp->_flag |= _TIORW);
	n = count * size;
	if(f & _TIODEV)				/* device i/o */
	{
		for(m=0; m<n; ++m)
			if(tfputc(*data++, fp) == EOF)
				break;
	}
	else					/* file i/o */
	{
		tfflush(fp);			/* re-sync file pointers */
		m = lam_iomap_write(f, fp->_file, data, n);
	}
	return((m > 0) ? (m / size) : (errno = m));
}
