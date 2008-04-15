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
 *
 *	$Id: args.h,v 6.12 2003/07/05 21:53:25 jsquyres Exp $
 *
 *	Function:	- command line argument parsing
 */

#ifndef _ARGS
#define _ARGS

#include "lam.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int	do_args __ARGS((int *argcp, char **argv));
extern int	expand_nodes __ARGS((void));
extern int	get_2nd_let __ARGS((int let));
extern int	intparam __ARGS((int c, int *p));
extern int	opt_chosen __ARGS((int o));
extern int	opt_taken __ARGS((int let));
extern int	xtoi __ARGS((char **cpp, int *ip));
extern char	*getparam __ARGS((int let));
extern void	exclusive __ARGS((const char *str));
extern void	followed __ARGS((const char *str));
extern void	free_nodes __ARGS((void));
extern void	twoletter __ARGS((const char *str));
extern void	validopts __ARGS((const char *str));
extern void     reset_args(void);

/*
 * ID parsing package
 */
extern int	nid_parse __ARGS((int *argc, char **argv));
extern int	pid_parse __ARGS((int *argc, char **argv));
extern int4	nid_count __ARGS((void));
extern int4	nid_parse1 __ARGS((char *s));
extern char	*nid_fmt __ARGS((int4 node));
extern void	nid_get __ARGS((int *ixp, int *idp, int *flp));
extern void	pid_get __ARGS((int *ixp, int *idp, int *flp));
extern void	nid_free __ARGS((void));
extern void	pid_free __ARGS((void));
extern void	nid_usage __ARGS((void));

#ifdef __cplusplus
}
#endif

#endif	/* _ARGS */
