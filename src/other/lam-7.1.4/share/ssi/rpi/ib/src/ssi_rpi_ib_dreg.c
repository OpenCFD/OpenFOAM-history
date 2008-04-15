/*
 * Copyright (c) 2001-2004 The Trustees of Indiana University.  
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
 * $Id: ssi_rpi_ib_dreg.c,v 1.5 2003/12/27 02:59:32 vsahay Exp $
 *
 *	Function:	- dynamic pinned memory handling
 */

#include <lam_config.h>

#include <string.h>

#include <args.h>
#include <rpisys.h>
#include <etc_misc.h>
#include <lamdebug.h>
#include <lamdebug.h>

#include <rpi_ib.h>
#include <rpi_ib_dreg.h>
#include <rpi_ib_interval.h>


/*
 * local variables
 */

static int size_of_keys;
static int PUB(env_mempool);


/*
 * local functions
 */

static void *env_ib_malloc(size_t len);
static void env_ib_free(void *base);


/* 
 *       reg-init()
 *       Initialize the pool of lam_ssi_rpi_ib_buf_ts
 */
void
lam_ssi_rpi_ib_reg_init(void)
{
  /* Kibbles-n-bits */
}


/*
 *       reg_add()
 *
 *       Function:        - add to the  pool of lam_ssi_rpi_ib_buf_ts
 *       Accepts:         - number of procs and number of buffers per proc
 *       Returns:         - integer
 */
int
lam_ssi_rpi_ib_reg_add(int nprocs, int num_bufs)
{
  /* Setup a general memory pool for the envelopes */

  size_of_keys = sizeof(LAM_IB_lkey) + sizeof(LAM_IB_rkey);
  PUB(env_mempool) = lam_mp_init(lam_ssi_rpi_ib_reg_env_len + size_of_keys,
                                 env_ib_malloc, env_ib_free, 
                                 nprocs * num_bufs * 2, 10, 0);

  lam_debug_cond((PUB(did), "initialized pool buffers"));
  return 0;
}


/*
 *       reg_env_malloc()
 *
 *       Function:        - returns a pinned buffer
 *       Accepts:         - the lam_ssi_rpi_proc to assign the memory to
 *       Returns:         - a pinned buffer
 */
struct lam_ssi_rpi_ib_envl *
lam_ssi_rpi_ib_reg_env_malloc(ib_mem_keypair *kp)
{
  char *ret = lam_mp_malloc(PUB(env_mempool));
  lam_debug_cond((PUB(did), "VPOOL malloced : %x", ret));
  memcpy(kp, ret + lam_ssi_rpi_ib_reg_env_len, size_of_keys);
  return (struct lam_ssi_rpi_ib_envl *) ret;
}


/*
 *       reg_env_free()
 *
 *       Function:        - adds a buffer back to the pool
 *       Accepts:         - a buffer to free
 *                        - where it came from
 *       Returns:         - nothing
 */
void
lam_ssi_rpi_ib_reg_env_free(struct lam_ssi_rpi_ib_envl *buf)
{
  lam_mp_free(PUB(env_mempool), buf);
  lam_debug_cond((PUB(did), "VPOOL freed : %x", buf));
}


/* 
 * Arbirary length mallocing, for long messages.  No pooling.  Use a
 * utility function (below) that doesn't do pooling, but does print a
 * nice error message if an error occurs.
 */
char * 
lam_ssi_rpi_ib_reg_malloc(long length, ib_mem_keypair *kp)
{
  char *retb;

  retb = (char *) malloc(length);
  if (retb == NULL)
    return NULL;
  
  if (lam_ssi_rpi_ib_interval_use((void *) retb, length, kp) == LAMERROR)
    return NULL;

  return retb;
}


/* 
 * Arbirary length free, for long messages.  No pooling.
 */
void 
lam_ssi_rpi_ib_reg_free(char* buf)
{
  /* We dont actually unpin it, we just free the buffer from being 
     malloced. This buffer may be unpinned during LRu if required --
     lazy deregistration kinda !*/
  free(buf);
}


/*
 * Free all malloc'ed memory
 */
void 
lam_ssi_rpi_ib_reg_destroy(void)
{
  lam_mp_finalize(PUB(env_mempool));
  PUB(env_mempool) = -1;
}


/***************************************************************************/

static void *
env_ib_malloc(size_t len)
{
  ib_mem_keypair kp;

  /* Malloc the memory, keeping enough space to save the key pair at
     the beginning of the buffer */

  void *buf = malloc(len);

  /* Now register the memory.  Don't ever let the automatic reaper
     release this memory, because it's for an envelope. */

  if (lam_ssi_rpi_ib_interval_use(buf, len, &kp) == LAMERROR) {
    return NULL;
  }

  /* Copy the keypair to the end of the buffer */

  memcpy(((char *) buf + len - size_of_keys), &kp, size_of_keys);

  /* All done */

  return buf;
}


static void 
env_ib_free(void *base)
{
  /* First, unregister the memory */

  if (lam_ssi_rpi_ib_interval_unuse(base, lam_ssi_rpi_ib_reg_env_len +
                                    size_of_keys) == LAMERROR) {
    return;
  }

  /* Now free it */

  free(base);
}
