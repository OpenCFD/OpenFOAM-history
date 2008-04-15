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
 *	$Id: tfgetc.c,v 6.4 2002/10/09 21:00:53 brbarret Exp $
 *
 *	We are just putting our stamp on this for housekeeping reasons.
 */

/*
 * This file is originally from dLibs 1.2 by Dale Schumacher et al.
 * It was modified for Trollius by MB, 88/12.
 * It was modified again for portability by GDB, 91/08.
 * And yet again by JRV for LAM, 93/05.
 */

/*
 * The CRLF --> \n translation left in in case we ever go back to MS-DOS.
 */

#include <laminternal.h>
#include <tstdio.h>

/*
 * external variables
 */
extern int _ioexit;

int
tfgetc(fp)
	TFILE *fp;
{
	int c, f, m, look = 0;

	if (! _ioexit) {
		if (atkexit (tfcloseall, (char *) 0) != 0)
			return (EOF);
		_ioexit = 1;
	}

	f = (fp->_flag &= ~_TIORW);
	if(!(f & _TIOREAD) || (f & (_TIOERR | _TIOEOF)))
		return(EOF);
	if(fp->_base == NULL)	/* allocate a buffer if there wasn't one */
		_getbuf(fp);
_fgetc1:
	if(--(fp->_cnt) < 0) {
		m = (int) lam_iomap_read(fp->_flag,
					fp->_file,
					(char *) fp->_base,
					(long) fp->_bsiz);
		if(m <= 0) {
			fp->_flag |= ((m == 0) ? _TIOEOF : _TIOERR);
			c = EOF;
			goto _fgetc2;
		}
		fp->_cnt = (m - 1);
		fp->_ptr = fp->_base;
	}
	c = *(fp->_ptr)++;
_fgetc2:
	if (!(f & _TIOBIN)) {		/* TRANSLATED STREAM PROCESSING */
		if (look) {			/* process lookahead */
			if(c != '\n') {		/* if not CR+LF.. */
				if(c == EOF) {	/* ..undo EOF.. */
					fp->_flag &= ~(_TIOERR | _TIOEOF);
				} else {	/* ..or unget character */
					*--(fp->_ptr) = c;
					++(fp->_cnt);
				}
				c = '\r';	/* return CR */
			}
		} else if(c == '\r') {		/* found CR */
			if(f & _TIODEV)		/* return it for tty */
				c = '\n';
			else {			/* else lookahead for LF */
				look = 1;
				goto _fgetc1;
			}
		} else if(c == 0x00) {		/* Ignore NUL's */
			goto _fgetc1;
		} else if(c == 0x1A) {		/* ^Z becomes EOF */
			fp->_flag |= _TIOEOF;
			c = EOF;
		}
	}
	return(c);
}

int
tfungetc(c, fp)
	int c;
	TFILE *fp;
{
	if((fp->_flag & (_TIOERR | _TIOEOF))	/* error or eof */
	|| (fp->_ptr <= fp->_base))		/* or too many ungets */
		return(EOF);
	++(fp->_cnt);
	return(*--(fp->_ptr) = c);
}
