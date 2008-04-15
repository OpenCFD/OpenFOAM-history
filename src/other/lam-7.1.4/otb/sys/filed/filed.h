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
 *	Copyright 1997 The Ohio State University
 *	GDB
 *
 *	$Id: filed.h,v 6.4 2002/10/09 20:56:35 brbarret Exp $
 *
 *	Function:	- general templates and contants used by filed
 */

#ifndef _FILED_H
#define _FILED_H

#include <portable.h>

/*
 * Trollius file descriptor
 */
struct fdesc {
	int4		f_tfd;		/* Trollius file descriptor */
	int4		f_fd;		/* Unix file descriptor */
	int4		f_ptr;		/* current pointer within file */
	int4		f_tflags;	/* Trollius peculiar flags */
	int4		f_flags;	/* Unix flags */
	int4		f_flow;		/* total data flow */
	int4		f_count;	/* open count O_REUSE */
	int4		f_src_node;	/* last client's node */
	int4		f_src_event;	/* last client's event */
	char		*f_name;	/* file name */
	struct fdesc	*f_next;	/* next entry in file desc. list */
	struct fdesc	*f_prev;	/* previous entry in file desc. list */
};

/*
 * nulls
 */
#define FDNULL		((struct fdesc *) 0)

/*
 * limits
 */
#define ACTIVEMAX	64		/* filed's open descriptors */

/*
 * codes
 */
#define FMT		'f'		/* formatted F77 file */
#define UFMT		'u'		/* unformatted F77 file */


#ifndef __ARGS
#if __STDC__ || defined(c_plusplus) || defined(__cplusplus)
#define __ARGS(a)	a
#else
#define __ARGS(a)	()
#endif
#endif

/*
 * prototypes
 */
#ifdef __cplusplus
extern "C" {
#endif

extern int	fdlock __ARGS((struct fdesc *));

#ifdef __cplusplus
}
#endif

#endif	/* _FILED_H */
