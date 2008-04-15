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
 *	$Id: app_schema.h,v 6.10 2002/10/09 20:57:22 brbarret Exp $
 * 
 *	Function:	- application schema templates and constants
 */

#ifndef _APP_SCHEMA_H
#define _APP_SCHEMA_H

#include "all_list.h"
#include "app_mgmt.h"
#include "all_opt.h"
#include "lam.h"

/*
 * GER (Guaranteed Envelope Resources)
 */
#ifndef MPI_GER
#define MPI_GER			8
#endif

/*
 * structures 
 */
struct apparg {
	int4		apa_refcount;		/* reference count */
	int4		apa_argc;		/* # cmd line args */
	char		**apa_argv;		/* cmd line args */
};

struct appenv {
	int4		ape_refcount;		/* reference count */
	char		**ape_envv;		/* cmd line args */
	char		*ape_wrkdir;		/* working dir */
};

struct aschema {
	int4		asc_srcnode;		/* binary source node */
	int4		asc_node;		/* single target node */
	int4		asc_proc_cnt;		/* # processes this entry */
	int4		asc_errno;		/* error code */
	LIST *		asc_nodelist;		/* nodes this entry */
	struct apparg *	asc_args;		/* cmd line args */
	struct appenv *	asc_env;		/* environment */
};

/*
 * prototypes
 */
#ifdef __cplusplus
extern "C" {
#endif

extern LIST *	asc_bufparse(char *buf, int size, int *pl, char **env);
extern LIST *	asc_parse(const char *file, int *pl, char **env);
extern LIST *	asc_schedule(LIST *ap);
extern void	asc_free(LIST *ap);
extern int	asc_compat(int *argc, char ***argv, OPT *ad);
extern int	asc_run(LIST *ap, int np, int4 rtf, int v,
			int e, struct _gps *p);
extern int	asc_ger(LIST *ap, int e);
extern int	asc_environment(int exportme, char *v, char ***env);
extern int	asc_workdir(LIST *ap, char *wrkdir);

#ifdef __cplusplus
}
#endif

#endif	/* _APP_SCHEMA_H */
