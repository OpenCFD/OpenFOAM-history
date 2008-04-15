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
 *	$Id: lam_getpid.c,v 1.10 2002/11/03 18:31:29 ssankara Exp $
 * 
 *	Function:	- Wrapper around getpid() so that it always 
 *                        returns the pid of the process that calls
 *                        kinit().  Needed because some OSes (Linux)
 *                        have extrodinarily broken thread packages
 *                        that give each thread a seperate pid.
 *                        ARGH!!!!
 */

#include <sys/types.h>
#include <unistd.h>
#include "etc_misc.h"

static pid_t inited_pid = 0;

void
lam_register_pid(pid_t pid)
{
  /* Note that you have to explicitly deregister the pid if you are
     using the fork() / kinit() trick. */
  if (inited_pid == 0)
    inited_pid = pid;
}


void
lam_reset_pid_cache() {
    inited_pid = 0;
}


pid_t
lam_getpid()
{
  if (inited_pid == 0) {
    /* Not really sure what the "right thing" is here, so
       take a best guess */
    return getpid();
  }

  return inited_pid;
}

