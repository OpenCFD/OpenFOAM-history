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
 *	$Log: tfdopen.c,v $
 *	Revision 6.3  2002/10/09 21:00:53  brbarret
 *	* Update tree to match the latest, greatest, license for LAM/MPI.  And this
 *	  one is even done properly :)
 *
 *	Revision 6.2  1999/05/27 20:07:05  kmeyer1
 *	added copyright
 *	
 *	Revision 6.1  1996/11/24 00:35:53  nevin
 *	Ohio Release
 *	
 * Revision 6.0  96/02/29  14:06:27  gdburns
 * Ohio Release
 * 
 * Revision 5.2  94/08/22  14:17:31  gdburns
 * Ohio Release
 * 
 * Revision 5.1  94/05/18  12:58:45  gdburns
 * Ohio Release
 * 
 * Revision 2.3  94/04/22  12:46:52  gdburns
 * Ohio Release
 * 
 * Revision 2.2.1.1  94/02/16  14:34:28  raja
 * Use the CONST macro.
 * 
 * Revision 2.2  93/05/10  16:36:51  raja
 * pre-release to 2.3
 * 
 *	We are just putting our stamp on this for housekeeping reasons.
 */

/*
 * This file is originally from dLibs 1.2 by Dale Schumacher et al.
 * It was modified for Trollius by MB, 88/12.
 * It was modified again for portability by GDB, 91/08.
 * And yet again by JRV for LAM, 93/05.
 */

#include <fcntl.h>

#include <portable.h>
#include <tstdio.h>

#define isiodev(d) ((d)<3)	/* instead of isatty(d) */
#define isstdout(d) ((d)==1)
#define isstderr(d) ((d)==2)

TFILE *
tfdopen(h, mode)
	int h;
	CONST char *mode;
{
	int i, f = 0;
	TFILE *fp = NULL;

	for(i=0; (!fp && (i < _TNFILE)); ++i)
		if(!(_tiob[i]._flag & (_TIOREAD | _TIOWRT))) /* empty slot? */
			fp = &_tiob[i];
	if(!fp)
		return(NULL);

	/*
	 * The CRLF --> \n translation left in fgetc/fputc in case we ever go
	 * back to MS-DOS.  To compensate we default to binary mode here.
	 */
	f |= _TIOBIN;

	while(*mode)
	{
		switch(*mode++)
		{
		case 'r':
			f |= _TIOREAD;
			break;
		case 'w':
			f |= _TIOWRT;
			break;
		case 'a':
			f |= _TIOWRT;
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
	if((f & (_TIOREAD | _TIOWRT)) == 0)
		return(NULL);
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
