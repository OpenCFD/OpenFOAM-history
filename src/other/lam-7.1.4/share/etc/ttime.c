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
 * $Id: ttime.c,v 1.5 2003/07/05 21:50:25 jsquyres Exp $
 *
 *	Function:	- returns the contents of a system timer
 *			  expressed in seconds.
 *			- used primarily for recording elapsed 
 *			  time in the user code, in a portable
 *			  manner
 *			- The resolution is not guaranteed, as it
 *			  depends on the underlying hardware,
 *			  but the units (seconds) are
 *			  guaranteed.
 *			- This code is for OTB and uses the 
 *			  Unix(tm) timing support.
 *			- based on Trollius 2.0, Copyright 1990
 *			  The Ohio State University and Cornell
 *			  Research Foundation
 */

#include <lam_config.h>

#include <errno.h>
#include <sys/time.h>

#include <net.h>

/*
 * ttime() returns the contents of the system timer (a measure
 * of time since an arbitrary but fixed point in time) expressed
 * in seconds. 
 */

double
ttime(void)
{
  struct timeval now;
  double sec;
/*
 * Get the system time.
 */
  if (gettimeofday(&now, (struct timezone *) 0)) {
    return(0);
  }
/*
 * Return time in seconds.
 */
  sec = (double) now.tv_sec + (((double) now.tv_usec) / 1000000.0);
  return(sec);
}
