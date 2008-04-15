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
 *	$Id: tfgets.c,v 6.4 2002/10/09 21:00:53 brbarret Exp $
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

char *
tfgets(data, limit, fp)
	char *data;
	int limit;
	TFILE *fp;
{
	char *p = data;
	int c = 0;

	while((--limit > 0) && ((c = tfgetc(fp)) != EOF))
		if((*p++ = c) == '\n')
			break;
	*p = EOS;
	return((c == EOF && p == data) ? NULL : data);	/* NULL == EOF */
}

char *
tgets(data)
	char *data;
{
	char *p = data;
	int c;

	while(((c = tfgetc(tstdin)) != EOF) && (c != '\n')) {
		if(c == '\b') {
			if(p > data)
				--p;
		} else {
			*p++ = c;
		}
	}
	*p = EOS;
	return(((c == EOF) && (p == data)) ? NULL : data);  /* NULL == EOF */
}
