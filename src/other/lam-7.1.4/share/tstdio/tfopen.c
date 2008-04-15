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
 *	$Id: tfopen.c,v 6.6 2002/10/09 21:00:54 brbarret Exp $
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
#include <fcntl.h>

#include <kreq.h>
#include <laminternal.h>
#include <portable.h>
#include <tstdio.h>

#define isiodev(d) ((d)<3)	/* instead of isatty(d) */
#define isstdout(d) ((d)==1)
#define isstderr(d) ((d)==2)

/*
 * external variables
 */
extern int		_ioexit;

/*
 * INTERNAL FUNCTION.  Attempt to open <filename> in the given
 * <mode> and attach it to the stream <fp>
 */
static TFILE *
_fopen(filename, mode, fp)
	char *filename;
	char *mode;
	TFILE *fp;
{
	int h, i, iomode = 0, f = 0;

	if (! _ioexit) {
		if (atkexit (tfcloseall, (char *) 0) != 0)
			return (NULL);
		_ioexit = 1;
	}

	/*
	 * The CRLF --> \n translation left in fgetc/fputc in case we ever go
	 * back to MS-DOS.  To compensate we default to binary mode here.
	 */
	f |= _TIOBIN;

	while(*mode) {
		switch(*mode++) {
			case 'r':
				f |= _TIOREAD;
				break;
			case 'w':
				f |= _TIOWRT;
				iomode |= (O_CREAT | O_TRUNC);
				break;
			case 'a':
				f |= _TIOWRT;
				iomode |= (O_CREAT | O_APPEND);
				break;
			case '+':
				f |= (_TIOREAD | _TIOWRT);
				break;
			case 'b':
				f |= _TIOBIN;
				break;
			case 't':
				f &= ~_TIOBIN;
				break;
			case 'T':
				f |= _TIOREMOTE;
				break;
			default:
				return(NULL);
		}
	}

	if((i = (f & (_TIOREAD | _TIOWRT))) == 0)
		return(NULL);
	else if(i == _TIOREAD)
		iomode |= O_RDONLY;
	else if(i == _TIOWRT)
		iomode |= O_WRONLY;
	else
		iomode |= O_RDWR;

	h = lam_iomap_open(f, filename, iomode, 0644);

	if(h < 0) {
		errno = h;
		return(NULL);		/* file open/create error */
	}
	if(isiodev(h)) {
		f |= _TIODEV;
		if (isstderr(h))
			f |= _TIONBF;
		else if (isstdout(h))
			f |= _TIOLBF;
		else
			f |= _TIOFBF;
	} else
		f |= _TIOFBF;
	fp->_file = h;			/* file handle */
	fp->_flag = f;			/* file status flags */
	fp->_base = NULL;		/* base of file buffer */
	fp->_ptr = NULL;		/* current buffer pointer */
	fp->_bsiz = 0;			/* buffer size */
	fp->_cnt = 0;			/* # of bytes in buffer */

	return(fp);
}

TFILE *
tfopen(filename, mode)
	CONST char *filename, *mode;
{
	int i;
	TFILE *fp = NULL;

	for(i=0; (!fp && (i < _TNFILE)); ++i)
		if(!(_tiob[i]._flag & (_TIOREAD | _TIOWRT))) /* empty slot? */
			fp = &_tiob[i];
	if(fp)
		return(_fopen((char*) filename, (char*) mode, fp));
	return(NULL);
}

TFILE *
tfreopen(filename, mode, fp)
	CONST char *filename, *mode;
	TFILE *fp;
{
	if(tfclose(fp))
		return(NULL);
	return(_fopen((char*) filename, (char*) mode, fp));
}
