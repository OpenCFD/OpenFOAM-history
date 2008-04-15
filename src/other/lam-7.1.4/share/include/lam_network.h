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
 *	$Id: lam_network.h,v 1.2 2002/10/09 20:57:24 brbarret Exp $
 *
 *	Function:	- prototypes for LAM wrappers around common
 *                        network functions.  Mainly used for platforms
 *                        where these don't work as expected.  Not in 
 *                        laminternal.h because of all the includes
 */

#ifndef LAM_NETWORK_H_
#define LAM_NETWORK_H_

#include <netdb.h>
#include <unistd.h>

#include "lam_config.h"
#include "laminternal.h"


#ifdef __cplusplus
extern "C" {
#endif

  /* share/etc/lam_gethostname.c */
  int lam_gethostname __ARGS((char*, size_t));

  /* share/etc/lam_gethostby.c */
  struct hostent *lam_gethostbyname __ARGS((const char *));
  struct hostent *lam_gethostbyaddr __ARGS((const char *, int, int));

#ifdef __cplusplus
}
#endif

#endif /* #ifndef LAM_NETWORK_H_ */
