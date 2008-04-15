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
 * $Id: nodespin.c,v 6.6 2003/07/05 21:52:08 jsquyres Exp $
 *
 *	Function:	- pretty prints successive node ID messages
 */

#include <stdio.h>

#include <args.h>
#include <portable.h>
#include <etc_misc.h>

/*
 *	nodespin_init
 *
 *	Function:	- prints initial message
 *	Accepts:	- message
 */
void
nodespin_init(char *msg)
{
  printf("%s           ", msg);
}

/*
 *	nodespin_next
 *
 *	Function:	- prints next node ID
 *	Accepts:	- node ID
 */
void
nodespin_next(int4 node)
{
  char buf[16];

  sprintf(buf, "%s...", nid_fmt(node));
  printf("%-10.10s", buf);
  fflush(stdout);
}

/*
 *	nodespin_end
 *
 *	Function:	- completes node message
 */
void
nodespin_end(void)
{
  printf("%-10.10s\n", "done");
}
