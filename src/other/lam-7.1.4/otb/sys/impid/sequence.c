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
 * $Id: sequence.c,v 1.5 2003/02/04 17:51:30 jsquyres Exp $
 *
 *	Function:	- network functions for talking to local LAM ranks
 */

#include <lam_config.h>
#if LAM_WANT_IMPI

#include <stdio.h>
#include <impi-defs.h>


/*
 * private variables
 */
static IMPI_Uint8 seqnum = (IMPI_Uint8) 1;
static IMPI_Uint8 srqid = (IMPI_Uint8) 1;
static IMPI_Uint8 drqid = (IMPI_Uint8) 1;
static IMPI_Uint8 cid = (IMPI_Uint8) 32; /* There are several
communicators already defined, and we can't collide */


IMPI_Uint8
get_new_seqnum()
{
  /* We will need to someday ensure that we don't collide with
     sequence numbers that haven't been released */

  return seqnum++;
}

void
release_seqnum(IMPI_Uint8 seq)
{
  /* Need to put something here to release this sequence number */
}


IMPI_Uint8
get_new_srqid()
{
  /* We will need to someday ensure that we don't collide with srqid's
     that haven't been released */

  return srqid++;
}


void
release_srqid(IMPI_Uint8 seq)
{
  /* Need to put something here to release this srqid */
}


IMPI_Uint8
get_new_drqid()
{
  /* We will need to someday ensure that we don't collide with drqid's
     that haven't been released */

  return drqid++;
}


void
release_drqid(IMPI_Uint8 seq)
{
  /* Need to put something here to release this drqid */
}


IMPI_Uint8
get_new_localcid()
{
  /* We will need to someday ensure that we don't collide with local
     cid's that haven't been released */

  return cid++;
}


void
release_localcid(IMPI_Uint8 seq)
{
  /* Need to put something here to release this local cid */
}


#endif
