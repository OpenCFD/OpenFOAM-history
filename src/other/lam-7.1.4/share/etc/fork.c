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
 *	$Id: fork.c,v 1.2 2002/10/09 20:57:01 brbarret Exp $
 * 
 *	Function:	- allow alternate entry point
 */

#include <lam_config.h>

#include <etc_misc.h>

/*
 * The only purpose for this file is to provide a place to instantiate
 * a global variable that points to a valid fork-like function.  The
 * reason for this is because some unrelated portions of LAM (e.g.,
 * the RPI) may need to change the "fork" function, but should *not*
 * create a dependency on liblam (e.g., replacing "fork" with
 * "gm_fork" everywhere means that just because of the choice of RPI
 * -- an MPI decision, btw, not an LAM decision -- then all LAM
 * binaries will have top be linked against -lgm, which ust doesn't
 * seem right.
 *
 * With this mechanism, the gm RPI can just set this global variable
 * and everyone will use it, but only when linked against libmpi.a,
 * and only when the gm RPI init function is invoked.  Hence, all LAM
 * binaries do *not* have to be linked against -lgm.
 *
 * So right now, the only places that use lam_fork() instead of fork()
 * are relatively limited, and are all in libmpi.  This mechanism
 * could conceivably be more useful on a LAM-wide scale someday, but
 * right now the only *need* for it is in areas where the RPI is
 * involved.  So a command decision was to only s/fork/lam_fork/g in
 * relevant places in libmpi.
 */

lam_fork_func_t lam_fork = fork;
