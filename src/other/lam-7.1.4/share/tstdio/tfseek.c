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
 *	$Id: tfseek.c,v 6.4 2002/10/09 21:00:54 brbarret Exp $
 *
 *	We are just putting our stamp on this for housekeeping reasons.
 */

/*
 * This file is originally from dLibs 1.2 by Dale Schumacher et al.
 * It was modified for Trollius by MB, 88/12.
 * It was modified again for portability by GDB, 91/08.
 * And yet again by JRV for LAM, 93/05.
 */

#include <errno.h>

#include <laminternal.h>
#include <tstdio.h>


long
tftell(fp)
	TFILE *fp;
{
	long rv;

	tfflush(fp);
	rv = (long) lam_iomap_lseek(fp->_flag, fp->_file, (long) 0, SEEK_CUR);
	return((rv < 0) ? ((errno = ((int) rv)), (-1)) : rv);
}

int
tfseek(fp, offset, origin)
	TFILE *fp;
	long offset;
	int origin;
{
	long rv;

	tfflush(fp);
	rv = (long) lam_iomap_lseek(fp->_flag, fp->_file, offset, origin);
	return((rv < 0) ? ((errno = ((int) rv)), (-1)) : 0);
}

void
trewind(fp)
	TFILE *fp;
{
	long rv;

	tfflush(fp);
	rv = (long) lam_iomap_lseek(fp->_flag, fp->_file, (long) 0, SEEK_SET);
	if(rv < 0)
		errno = ((int) rv);
	fp->_flag &= ~(_TIOEOF|_TIOERR);
}
