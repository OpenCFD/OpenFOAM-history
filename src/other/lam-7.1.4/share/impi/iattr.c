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
 *	$Id: iattr.c,v 1.11 2002/10/09 20:57:17 brbarret Exp $
 *
 *	Function:	- initializes an IMPID to run an IMPI job
 */

#include <lam_config.h>

#if LAM_WANT_IMPI

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sfh.h>           

#include <kio.h>
#include <mpisys.h>
#include <impi-defs.h> /* This file supplied by IMPI server package */
#include <impi.h>
#include <lamdebug.h>
#include <mpi.h>


/*
 * Local variables -- the values of the attributes
 */
static int impi_client_size = 1;
static int impi_client_color = 0;
static int impi_host_size = 1;
static int impi_host_color = 0;


int
IMPI_Fill_attributes(void)
{
  int err;
  int client, host, host_offset;
  int rank;
  struct _attrkey *keystate;

  /* If this is not an IMPI job, set everything to zero */

  if ((_kio.ki_rtf & RTF_IMPI) != 0) {
    impi_client_size = impi_server_info.nclients;
    impi_host_size = impi_server_info.nhosts;
  }

  /* Set the attributes */
  /* Set the client size in IMPI_CLIENT_SIZE */

  err = MPI_Comm_set_attr(MPI_COMM_WORLD, IMPI_CLIENT_SIZE, 
			  (void *) LAM_INT_TO_VOID_P
			  impi_client_size);
  if (err != MPI_SUCCESS) 
    return LAMERROR;

  /* Set the host size in IMPI_HOST_SIZE */

  err = MPI_Comm_set_attr(MPI_COMM_WORLD, IMPI_HOST_SIZE, 
			  (void *) LAM_INT_TO_VOID_P impi_host_size);
  if (err != MPI_SUCCESS) 
    return LAMERROR;

  /* Determine our client and host colors.  The client color is
     obvious.  But note that the host color *also* has to be relative
     to all the others -- i.e., only one host will be host 0, etc. */

  if ((_kio.ki_rtf & RTF_IMPI) != 0) {
    for (host_offset = rank = client = 0; client < impi_client_size; 
	 client++) {
      for (host = 0; host < lam_impi_clients[client].ic_nhosts; 
	   host++, impi_host_color++) {
	if (rank >= impi_server_info.start_rank) {
	  impi_client_color = client;
	  
	  /* Break out of the loop */
	  
	  client = impi_client_size;
	  break;
	}
	rank += lam_impi_hosts[host + host_offset].ih_nprocs;
      }
      if (client < impi_client_size)
	host_offset += lam_impi_clients[client].ic_nhosts;
    }
  }

  /* Set the client and host colors */
      
  err = MPI_Comm_set_attr(MPI_COMM_WORLD, IMPI_CLIENT_COLOR, 
			  (void *) LAM_INT_TO_VOID_P impi_client_color);
  if (err != MPI_SUCCESS) 
    return LAMERROR;
  err = MPI_Comm_set_attr(MPI_COMM_WORLD, IMPI_HOST_COLOR,
			  (void *) LAM_INT_TO_VOID_P impi_host_color);
  if (err != MPI_SUCCESS) 
    return LAMERROR;
/*
 * Mark them as predefined and mark the style, integer valued is F77 style.
 */
  if ((keystate = lam_getattr(IMPI_CLIENT_SIZE)) == 0) 
    return(LAMERROR);
  keystate->ak_flags |= LAM_PREDEF | LAM_LANGF77;
  
  if ((keystate = lam_getattr(IMPI_CLIENT_COLOR)) == 0) 
    return(LAMERROR);
  keystate->ak_flags |= LAM_PREDEF | LAM_LANGF77;
  
  if ((keystate = lam_getattr(IMPI_HOST_SIZE)) == 0) 
    return(LAMERROR);
  keystate->ak_flags |= LAM_PREDEF | LAM_LANGF77;
  
  if ((keystate = lam_getattr(IMPI_HOST_COLOR)) == 0) 
    return(LAMERROR);
  keystate->ak_flags |= LAM_PREDEF | LAM_LANGF77;
  
  return 0;
}


#endif /* LAM_WANT_IMPI */

