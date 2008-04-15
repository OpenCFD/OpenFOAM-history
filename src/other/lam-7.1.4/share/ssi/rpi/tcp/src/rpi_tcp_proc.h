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
 * $Id: rpi_tcp_proc.h,v 1.2 2002/10/09 21:00:39 brbarret Exp $
 *
 *	Function:	- TCP client-to-client interface header
 */

#ifndef LAM_SSI_RPI_TCP_PROC_H
#define LAM_SSI_RPI_TCP_PROC_H

#include <lam_config.h>

#include <lam-ssi-rpi.h>
#include <mpisys.h>


/*
 * TCP-specific MPI process data
 */

struct lam_ssi_rpi_proc {
  /*********************************************************************
                                 WARNING

   Do *not* change anything about these fields without also changing
   the corresponding layouts in the shmem RPIs.  This is necessary
   because of the linker tricks that we play to re-use the TCP code
   with shared memory.
  *********************************************************************/

  struct lam_ssi_rpi_envl cp_env;	/* incoming envelope */
  int		cp_sock;		/* socket descriptor */
  int		cp_nenvin;		/* # env. bytes left to read */
  char		*cp_envbuf;		/* envelope buffer pointer */
  int		cp_nmsgin;		/* # msg. bytes left to read */
  char		*cp_msgbuf;		/* message buffer pointer */
  int		cp_extra;		/* # bytes to go into sink */
  MPI_Request	cp_wreq;		/* req writing to process */
  MPI_Request	cp_rreq;		/* reading req */
  MPI_Request	cp_mreq;		/* req to match from */
  int		(*cp_readfn)();		/* read function */
  struct lam_ssi_rpi_cbuf_msg *cp_bmsg;	/* buffering? */

  /* point up to the main _proc so that we can do checks for things
     like LAM_PHOMOG.  This is an optimization -- in the tcp RPI, we
     usually pass around just a (struct lam_ssi_rpi_proc*) because we
     do 95% of the work with just this.  But periodically, a few
     things new stuff from the upper-level proc (e.g., p_mode &
     LAM_PHOMOG), so we need to be able to find the upper-level proc
     when we only have the c2c_proc. */

  struct _proc  *cp_proc;

  /*********************************************************************
                                 WARNING

   Do not change any of the fields above -- see the explanation above.
  *********************************************************************/
};


#endif	/* LAM_SSI_RPI_TCP_PROC_H */
