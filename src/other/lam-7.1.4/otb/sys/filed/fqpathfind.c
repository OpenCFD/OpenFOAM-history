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
 * $Id: fqpathfind.c,v 6.3 2003/05/31 22:28:48 jsquyres Exp $ 
 */

#include <lam_config.h>

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "filed.h"
#include <freq.h>
#include <net.h>
#include <portable.h>
#include <terror.h>
#include <typical.h>
#include <sfh.h>
#include <etc_misc.h>


/*
 * external variables
 */
extern char		fbuf[];		/* multi purpose buffer */


/*
 * external functions
 */
extern void		fsendr();	/* general purpose file reply */


/*
 *	fqpathfind
 *
 *	Function:	- remote access to sfh_path_find
 *	Accepts:	- 
 */
void
fqpathfind(struct freq *fq)
{
  char *filename;
  char *dir;
  int len;

  /* Ensure that we got a filename */
  
  if (strlen(fbuf) == 0) {
    fsendr(fq->fq_src_node, fq->fq_src_event,
	   errno, (int4) ERROR, INT4_NIL, INT4_NIL);
    return;
  }

  /* If we got one, do an sfh_path_find on it */

  dir = fbuf + strlen(fbuf) + 1;
  if (*dir == '\0')
    dir = NULL;

  filename = sfh_path_env_findv(fbuf, R_OK, 0, dir);
  if (filename == NULL) {
    fbuf[0] = '\0';
    fsendr(fq->fq_src_node, fq->fq_src_event,
	   errno, (int4) ERROR, INT4_NIL, INT4_NIL);
    return;
  }

  /* Found it.  Send it back */

  len = LAM_min(strlen(filename), MAXNMSGLEN - 1);
  lam_strncpy(fbuf, filename, len);
  fbuf[len] = '\0';
  fsendr(fq->fq_src_node, fq->fq_src_event, 0, 0, len + 1, INT4_NIL);
}
