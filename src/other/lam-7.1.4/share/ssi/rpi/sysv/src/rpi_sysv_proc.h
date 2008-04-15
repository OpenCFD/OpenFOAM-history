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
 *	$Id: rpi_sysv_proc.h,v 1.2 2002/10/09 21:00:34 brbarret Exp $
 *
 *	Function:	- shared memory / TCP client-to-client interface header
 */

#ifndef LAM_RPI_SYSV_PROC_H
#define LAM_RPI_SYSV_PROC_H

#include <lam_config.h>

#include <lam-ssi-rpi.h>
#include <mpisys.h>


/*
 * constants
 * some of these values come from lam_config.h (i.e., configure)
 */
#define CACHELINESIZE		64     /* cache line size (or more) */
#define SHMBOXSIZE		(LAM_SHMSHORTMSGLEN + CACHELINESIZE)


/*
 * shared memory postbox
 */
struct lam_ssi_rpi_sysv_boxhdr
{
  struct lam_ssi_rpi_envl bh_env;
  int bh_size;
  int bh_bufoff;
};

union lam_ssi_rpi_sysv_postbox
{
  struct lam_ssi_rpi_sysv_boxhdr pb_header;
  char pb_pad[CACHELINESIZE];
};

typedef union lam_ssi_rpi_sysv_postbox lam_ssi_rpi_sysv_postbox_t;


/*
 * sysv-specific MPI process data
 */
struct lam_ssi_rpi_proc
{
  /*********************************************************************
                                 WARNING

   The following layout must exactly reflect what is in
   share/ssi/rpi/tcp/src/rpi_tcp_proc.h since we do some linker tricks
   to re-used the code that is in that RPI.  Since we have a
   *different* struct lam_ssi_rpi_proc over here (i.e., we have
   additional fields), we must ensure that the fields that the TCP RPI
   expects to find are at the same offsets that it would if it were
   using the TCP RPI as the upper layer.
  *********************************************************************/

  struct lam_ssi_rpi_envl cp_env;	/* incoming envelope */
  int cp_sock;			/* socket descriptor */
  int cp_nenvin;		/* # env. bytes left to read */
  char *cp_envbuf;		/* envelope buffer pointer */
  int cp_nmsgin;		/* # msg. bytes left to read */
  char *cp_msgbuf;		/* message buffer pointer */
  int cp_extra;			/* # bytes to go into sink */
  MPI_Request cp_wreq;		/* req writing to process */
  MPI_Request cp_rreq;		/* reading req */
  MPI_Request cp_mreq;		/* req to match from */
  int (*cp_readfn) ();		/* read function */
  struct lam_ssi_rpi_cbuf_msg *cp_bmsg;	/* buffering? */
  struct _proc *cp_proc;	/* pointer to upper level _proc */

  /*********************************************************************
                                 WARNING

   Do not change any of the fields above -- see the explanation above.
  *********************************************************************/

  /* sysv-specific fields */

  int cp_write;			/* ok to try to write flag */
  int cp_read;			/* ok to try to read flag */
  int cp_shm;			/* shm block id */
  int cp_sem;			/* semaphore set id */
  int cp_ppsem;			/* process pair sema set id */
  int cp_locked;		/* reader has a lock? */
  int cp_shmidx;		/* shared memory index */
  int cp_insize;		/* incoming area size */
  lam_ssi_rpi_sysv_postbox_t *cp_inbox;	/* incoming postbox */
  lam_ssi_rpi_sysv_postbox_t *cp_outbox;	/* outgoing postbox */
  struct sembuf *cp_lop;	/* lock operations */
  struct sembuf *cp_top;	/* try lock operations */
  struct sembuf *cp_uop;	/* unlock operations */
  int cp_nbfde;			/* # buffered envelopes */
};

#endif /* LAM_SSI_RPI_SYSV_PROC_H */
