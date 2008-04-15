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
 *	Ohio Trollius
 *
 *      $Id: tfputs.c,v 6.4 2002/10/09 21:00:54 brbarret Exp $
 *
 *	We are just putting our stamp on this for housekeeping reasons.
 */

/*
 * This file is originally from dLibs 1.2 by Dale Schumacher et al.
 * It was modified for Trollius by MB, 88/12.
 * It was modified again for portability by GDB, 91/08.
 * And yet again by JRV for LAM, 93/05.
 */

#include <portable.h>
#include <tstdio.h>

int
tfputs(CONST char *data, TFILE *fp)
{
	int n = 0;

	while(*data)
	{
		if(tfputc(*data++, fp) == EOF)
			return(EOF);
		++n;
	}
	return(n);
}

/*
 * This function was renamed from tputs() to lam_tputs() to prevent a
 * name clash with the termcap/ncurses tputs().  However, there's a
 * #define in share/include/tstdio.h that maps tputs to lam_tputs, so
 * most backward compatability should be preserved.  There's a lengthy
 * comment about this in share/include/tstdio.h and HISTORY.  
 */
int
lam_tputs(CONST char *data)
{
	int n;

	if(((n = tfputs(data, tstdout)) == EOF)
	|| (tfputc('\n', tstdout) == EOF))
		return(EOF);
	return(++n);
}
