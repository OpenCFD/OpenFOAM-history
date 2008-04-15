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
 *	$Id: show_version.c,v 1.10 2003/11/01 23:20:55 brbarret Exp $
 * 
 *	Function:	- show the version of LAM/MPI
 *
 */

#include <lam_config.h>
#include <lam_build_info.h>

#include <stdio.h>

#include <etc_misc.h>


/*
 * Show a header indicating the version of LAM
 */
void
lam_show_version(int verbose)
{
  printf("\nLAM %s", LAM_VERSION);
#if LAM_WANT_IMPI
  printf("/IMPI");
#endif
#if LAM_WANT_MPI2CPP
  printf("/MPI 2 C++");
#endif
#if LAM_WANT_ROMIO
  printf("/ROMIO");
#endif
#if LAM_HAVE_BPROC
  printf("/bproc");
#endif
  printf(" - Indiana University\n\n");

  if (verbose == 1) {
    printf("\tArch:\t\t%s\n", LAM_ARCH);
    printf("\tPrefix:\t\t%s\n", LAM_PREFIX);
    printf("\tConfigured by:\t%s\n", LAM_CONFIGURE_USER);
    printf("\tConfigured on:\t%s\n", LAM_CONFIGURE_DATE);
    printf("\tConfigure host:\t%s\n", LAM_CONFIGURE_HOST);
   }
}
