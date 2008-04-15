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
 *	Ohio Trollius
 *	Copyright 1996 The Ohio State University
 *	GDB/NJN
 *
 *	$Id: rfatexit.c,v 6.5 2003/11/02 19:30:51 brbarret Exp $
 *
 *	Function:	- close file descriptors
 *			- don't bother with standard I/O
 */

#include <freq.h>
#include <kio.h>

/*
 * external variables
 */
extern struct fclient	_ufd[];			/* client file descs */
extern struct kio_t	_kio;			/* kernel IO block */

void
lam_rfatexit()
{
    int			i;
    int			tfd;

    for (i = 0 ; i < FUMAX ; i++) {
	
	tfd = _ufd[i].fu_tfd;

	if (tfd > 2) {
/*
 * IO daemon now closes LAM stdio for remote processes (where necessary) but
 * it must still be done here for local processes.  Non-stdio descriptors must
 * be closed regardless.  
 */
	    if (_ufd[i].fu_tflags && !(_kio.ki_rtf & RTF_PFDIO)
		    && tfd != _kio.ki_stdout && tfd != _kio.ki_stderr) {

		lam_rfclose(i);
	    }
	}
    }
}


/*
 * backwards compatibility
 */
void rfatexit()
{ 
  lam_rfatexit();
}
