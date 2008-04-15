/*
 * Copyright (c) 2001-2004 The Trustees of Indiana University.  
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
 *	Function:	- load and execute program on node
 */

#include "lam_config.h"
#include "sfh.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "args.h"
#include "etc_misc.h"
#include "flatreq.h"
#include "kio.h"
#include "laminternal.h"
#include "lreq.h"
#include "net.h"
#include "portable.h"
#include "preq.h"
#include "typical.h"

/*
 * external functions
 */
char			*_path_env_find();

/*
 * external variables
 */
extern struct kio_t	_kio;

/*
 *	Function:	- loads and executes program on node
 *	Accepts:	- source node of program file 
 *			- destination node to load/execute on
 *			- program filename
 *			- process runtime flags
 *			- argument vector
 *			- process environment
 *			- process ID (out)
 *			- process index (out)
 *	Returns:	- 0 or ERROR
 */
int
rploadgov(int4 srcnode, int4 destnode, CONST char *filename, int4 rtflags,
    char **argv, struct penv *env, int4 *pid, int4 *idx)
{
    int			r;
    int4		argvtag;	/* argv flat tag */
    int4		progtag;	/* program flat tag */
    int4		envsize;	/* size of loaded environment */
    char		*fullpath;
    char		*cwd;
/*
 * If the process is to be run locally and is not set to use the
 * application directory and the working dir has not been explicitly set
 * then its working directory is to be changed to
 * that of the caller.
 */
    if ((destnode == LOCAL || destnode == getnodeid())
	    && LAM_isNullStr(env->pe_wrkdir) && !(rtflags & RTF_APPWD)) {
	if ((cwd = getworkdir()) == 0)
  	    return(LAMERROR);
	lam_strncpy(env->pe_wrkdir, cwd, LAM_PATH_MAX);
	free(cwd);
    }
/*
 * If source and destination nodes are the same, call rpcreate().
 */
    if (srcnode == destnode) {

	if ((srcnode == LOCAL) || (srcnode == getnodeid())) {
	    fullpath = sfh_path_env_findv((char*) filename, X_OK,
				env->pe_envv, env->pe_wrkdir);
	    if (fullpath == 0)
		return(LAMERROR);

	    r = rpcreatev(srcnode, fullpath, rtflags, argv, env, pid, idx);
	    free(fullpath);
	}
	else {
	    r = rpcreatev(srcnode, filename, rtflags, argv, env, pid, idx);
	}

	return(r);
    }
/*
 * Source and destination nodes are different, do the real work.
 *
 * Load arguments and environment on the destination node.
 */
    argvtag = 0;
    if (rflatv(destnode, argv, env, &envsize, &argvtag)) {
	return(LAMERROR);
    }
/*
 * Load the program on the destination node.
 */
    progtag = lam_getpid();
    if (rload(srcnode, destnode, progtag, filename)) {
	if (argvtag)
	    rflclean(destnode, argvtag);
	return(LAMERROR);
    }
/*
 * Create/execute the process.
 */
    return(rpgov(destnode, progtag, rtflags,
		argvtag, envsize, env->pe_wrkdir, env->pe_rank, pid, idx));
}

/*
 *	Function:	- loads and executes program on node
 *	Accepts:	- source node of program file 
 *			- destination node to load/execute on
 *			- program filename
 *			- process runtime flags
 *			- argument vector
 *			- process ID (out)
 *			- process index (out)
 *	Returns:	- 0 or ERROR
 */
int
rploadgo(srcnode, destnode, filename, rtflags, argv, pid, idx)

int4			srcnode;
int4			destnode;
CONST char		*filename;
int4			rtflags;
char			**argv;
int4			*pid;
int4			*idx;

{
    struct penv		env;
    char		*cwd;

    env.pe_rank = -1;
    env.pe_envv = 0;
/*
 * If the process is to be run locally and is not set to use the
 * application directory then its working directory is to be changed to
 * that of the caller.
 */
    if ((destnode == LOCAL || destnode == getnodeid())
	    && !(rtflags & RTF_APPWD)) {
	if ((cwd = getworkdir()) == 0)
	    return(LAMERROR);
	lam_strncpy(env.pe_wrkdir, cwd, LAM_PATH_MAX);
	free(cwd);
    } else {
	strcpy(env.pe_wrkdir, "");
    }

    return(rploadgov(srcnode, destnode,
		filename, rtflags, argv, &env, pid, idx));
}

