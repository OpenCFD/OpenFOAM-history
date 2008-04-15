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
 *	$Id: lam_gethostname.c,v 1.3 2002/10/09 20:57:02 brbarret Exp $
 *
 *	Function:	- wrapper for gethostname to work on Bproc
 */

#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "lam_config.h"
#include "laminternal.h"
#include "lam_network.h"

#if LAM_HAVE_BPROC && LAM_HAVE_BPROC3_API
#include <sys/bproc.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#endif

int
lam_gethostname(char* name, size_t len)
{
#if LAM_HAVE_BPROC && LAM_HAVE_BPROC3_API
  /* This is ugly, but I think safe, as long as 10 > 2 (decimal vs
     binary) */
  char *tmp;
  int currnode;

  currnode = bproc_currnode();

  /* I'm a sucker, special case the master.  ONLY because it will work
     with bproc_getnodebyname() */
  if (currnode == BPROC_NODE_MASTER) {
    tmp = strdup("master");
  } else {
    tmp = malloc(sizeof(char) * sizeof(int) * 8);
    if (tmp == 0)
      return -1;

    sprintf(tmp, "%d", currnode);

    if (len <= strlen(tmp)) {
      errno = EINVAL;
      free(tmp);
      return -1;
    }

  }

  strcpy(name, tmp);
  free(tmp);

  return 0;
#else
  return gethostname(name, len);
#endif
}
