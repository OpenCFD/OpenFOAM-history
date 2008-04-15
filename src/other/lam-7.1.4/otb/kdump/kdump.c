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
 *	Copyright 1995 The Ohio State University
 *	GDB
 *
 *      $Id: kdump.c,v 6.5 2002/10/09 20:56:20 brbarret Exp $
 * 
 *	Function:	- prompts kernel to dump process state
 */

#include <errno.h>

#include <kreq.h>
#include <terror.h>


/*
 * external functions
 */
extern int		_cio_kreq();
extern int		_cipc_init();


int
main(int argc, char* argv[])
{
	struct kreq	req;		/* kernel request */
	struct kreply	reply;		/* kernel reply */

	if (_cipc_init()) {
		terror("kdump");
		return(errno);
	}

	req.kq_req = KQDUMP;
	req.kq_index = -1;

	if (_cio_kreq(&req, &reply)) {
		terror("kdump");
		return(errno);
	}

	return(0);
}
