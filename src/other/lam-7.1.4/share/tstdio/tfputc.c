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
 *	$Id: tfputc.c,v 6.4 2002/10/09 21:00:54 brbarret Exp $
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

/*
 * the CRLF --> \n translation left in case we ever go back to MS-DOS
 */

#include <kreq.h>
#include <laminternal.h>
#include <tstdio.h>

/*
 * external variables
 */
extern int _ioexit;

int
tfputc(c, fp)
	int c;
	TFILE *fp;
{
	int f, m, nl = 0;

	if (! _ioexit) {
		if (atkexit (tfcloseall, (char *) 0) != 0)
			return (EOF);
		_ioexit = 1;
	}

	f = (fp->_flag |= _TIORW);
	if(!(f & _TIOWRT)			/* not opened for write? */
	|| (f & (_TIOERR | _TIOEOF)))		/* error/eof conditions? */
		return(EOF);
	if(fp->_base == NULL)	/* allocate a buffer if there wasn't one */
		_getbuf(fp);
	if(!(f & _TIOBIN) && (c == '\n'))	/* NL -> CR+LF ? */
	{
#ifdef	DOS
		c = '\r';
		nl = 1;
#endif
	}
_fputc:
	*(fp->_ptr)++ = c;
	if (((++(fp->_cnt)) >= fp->_bsiz)
	||  (c == '\n' && (f & _TIOLBF)))	/* - MB: added */
	{
		m = fp->_cnt;			/* MB: may be < _bsiz */
		fp->_cnt = 0;
		fp->_ptr = fp->_base;
		if((lam_iomap_write(f,
				fp->_file,
				(char *) fp->_base,
				(long) m)) != m)
		{
			fp->_flag |= _TIOERR;
			return(EOF);
		}
	}
	if(nl && (c == '\r'))			/* NL -> CR+LF ? */
	{
		c = '\n';
		goto _fputc;
	}
	return(c);
}
