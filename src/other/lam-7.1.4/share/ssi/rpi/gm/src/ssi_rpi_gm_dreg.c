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
 * $Id: ssi_rpi_gm_dreg.c,v 1.17 2003/11/10 23:40:17 jsquyres Exp $
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

#include <rpi_gm.h>
#include <rpi_gm_dreg.h>
#include <rpi_gm_interval.h>


#define ERRORSTR_SIZE 32


/*
 * public variables
 */
int PUB(env_mempool) = -1;


/*
 * local variables
 */
static struct gm_port *my_port = NULL;
static char *buffer;
static char myrankstr[ERRORSTR_SIZE];
static char lenstr[ERRORSTR_SIZE];
static int errno_save;



/*
 * local functions
 */
static void* dma_malloc(size_t size);
static void dma_free(void *base);


/*
 *       dma_init()
 *
 *       Function:        - initialize pool of lam_ssi_rpi_gm_buf_ts
 *       Accepts:         - a lam_ssi_rpi_proc to initialzie
 *       Returns:         - nothing
 */
int
lam_ssi_rpi_gm_dma_init(struct lam_ssi_rpi_proc *p)
{
  /* Global variable */

  my_port = p->cp_gm_port.rgp_gm_port;

  /* Setup a general memory pool for the envelopes */

  PUB(env_mempool) = lam_mp_init(lam_ssi_rpi_gm_dma_env_len, 
                                 dma_malloc, dma_free, 10, 10, 1);

  /* All done */

  return 0;
}


/* 
 * Arbirary length mallocing, for long messages.  No pooling.  Use a
 * utility function (below) that doesn't do pooling, but does print a
 * nice error message if an error occurs.
 */
char * 
lam_ssi_rpi_gm_dma_malloc(size_t length)
{
  char *ret = dma_malloc(length);

  if (ret == NULL)
    return NULL;

  /* Save the fact that this is pinned memory so that we don't try to
     pin it later (i.e., when the user sends or receives using this
     buffer).  Ensure that we mark it as "LRU not ok" so that it
     doesn't get freed in LRU sweeps. */

  lam_ssi_rpi_gm_interval_use(ret, length, 0, 0);

  /* All done */

  return ret;
}


/* 
 * Arbirary length free, for long messages.  No pooling.
 */
void 
lam_ssi_rpi_gm_dma_free(char *buf, size_t length)
{
  /* Actually free the memory */

  gm_dma_free(my_port, buf);

  /* Unuse the interval */

  lam_ssi_rpi_gm_interval_unuse(buf, length);
}


/*
 * Free all DMA malloc'ed memory
 */
void 
lam_ssi_rpi_gm_dma_destroy(void)
{
  /* Free the envelopes */

  lam_mp_finalize(PUB(env_mempool));
  PUB(env_mempool) = -1;
}


/***************************************************************************/

/*
 * Do the actual dma malloc, and have some nice error handling.  This
 * function is called by the arbitrary ssi_rpi_gm_dma_malloc() (above)
 * and by the handling for the envelope memory pool.
 *
 * This is a standalone function mainly (as opposed to using
 * gm_dma_malloc() directly) because we have to cache/use the value of
 * my_port.
 */
static void *
dma_malloc(size_t len)
{
  /* do the allocation*/

  buffer = gm_dma_malloc(my_port, len);
  if (buffer != NULL)
    return buffer;

  /* error check */

  errno_save = errno;
  memset(myrankstr, 0, ERRORSTR_SIZE);
  snprintf(myrankstr, 31, "%d", lam_myproc->p_gps.gps_grank);
  memset(lenstr, 0, ERRORSTR_SIZE);
  snprintf(lenstr, 31, "%ld", (long) len);
  show_help_file("lam-ssi-rpi-gm-helpfile", "rpi-gm", "gm-dma-malloc-fail", 
                 myrankstr, lenstr, NULL);
  errno = errno_save;
  
  return NULL;
}


/*
 * Free an arbitrary chunk of memory.  Invoked by the arbitrary
 * ssi_rpi_gm_dma_free() (above) and by the handling for the envelope
 * memory pool.
 *
 * This is a standalone function mainly (as opposed to using
 * gm_dma_free() directly) because we have to cache/use the value of
 * my_port.
 */
static void
dma_free(void *base)
{
  gm_dma_free(my_port, base);
}
