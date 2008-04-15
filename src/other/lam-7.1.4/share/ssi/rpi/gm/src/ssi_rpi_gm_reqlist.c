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
 * $Id: ssi_rpi_gm_reqlist.c,v 1.8 2003/11/13 12:51:53 jsquyres Exp $
 *
 *	Function:	- GM low-level routines
 */

#include <lam_config.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <lamdebug.h>

#include <rpi_gm.h>
#include <rpi_gm_reqlist.h>


/*
 * initialize a given list
 */
int
lam_ssi_rpi_gm_reqlist_init(struct lam_ssi_rpi_gm_reqlist_t *rl)
{
  if (rl == NULL)
    return LAMERROR;

  rl->size = 0;
  rl->top = rl->bottom = NULL;

  return 0;
}


/*
 * Append a request to the end of a queue
 */
int
lam_ssi_rpi_gm_reqlist_append(struct lam_ssi_rpi_gm_reqlist_t *rl, 
			      MPI_Request req)
{
  struct lam_ssi_rpi_gm_reqlist_item_t *rli;

  rli = malloc(sizeof(struct lam_ssi_rpi_gm_reqlist_item_t));
  if (rli == NULL)
    return LAMERROR;

  rli->req = req;
  rli->prev = rl->bottom;
  rli->next = NULL;

  if (rl->bottom != NULL)
    rl->bottom->next = rli;

  rl->bottom = rli;
  if (rl->size == 0)
    rl->top = rli;
  ++(rl->size);
  lam_debug_cond((lam_ssi_rpi_gm_did, "reqlist_append: rl %p, req %p, "
                  "(peer %d, cid %d, tag %d), size now %d", 
                  rl, req, 
                  req->rq_rank, req->rq_comm->c_contextid, req->rq_tag,
                  rl->size));

  return 0;
}


/*
 * See if an envelope can be matched to a request in the reqlist.
 * Returns an item pointer if a match is found, NULL otherwise.
 */
struct lam_ssi_rpi_gm_reqlist_item_t *
lam_ssi_rpi_gm_reqlist_match(struct lam_ssi_rpi_gm_reqlist_t *rl, 
                             struct lam_ssi_rpi_gm_envl *env)
{
  struct lam_ssi_rpi_gm_reqlist_item_t *rli = NULL;

  lam_debug_cond((lam_ssi_rpi_gm_did, 
                  "reqlist_match: rl %p, size now %d", 
                  rl, rl->size));
  for (rli = rl->top; rli != NULL; rli = rli->next) {
    lam_debug_cond((lam_ssi_rpi_gm_did, 
                    "reqlist match: checking req %p", 
                    rli->req));
    if (ENVL_COMPARE(env, rli->req->rq_rpi->cq_envbuf) == 1 &&
        (rli->req->rq_flags & LAM_RQFCANCEL) == 0) {
      lam_debug_cond((lam_ssi_rpi_gm_did, 
                      "reqlist match: found match!"));
      return rli;
    }
  }

  return NULL;
}


/*
 * Remove an item from a reqlist -- a O(1) operation.
 * Returns LAMERROR on error, 0 on success
 */
int
lam_ssi_rpi_gm_reqlist_rm_item(struct lam_ssi_rpi_gm_reqlist_t *rl, 
                               struct lam_ssi_rpi_gm_reqlist_item_t *rli)
{
  struct lam_ssi_rpi_gm_reqlist_item_t *next, *prev;

  if (rli == NULL)
    return LAMERROR;

  lam_debug_cond((lam_ssi_rpi_gm_did,
                  "reqlist_rm_item: rl %p", rl));
  lam_debug_cond((lam_ssi_rpi_gm_did,
                  "reqlist_rm_item: rli:%p, top:%p, "
                  "bottom:%p, rli->next:%p, rli->prev:%p",
                  rli, rl->top, rl->bottom, rli->next, rli->prev));
  next = rli->next;
  prev = rli->prev;
  --(rl->size);

  /* Patch up the prev/next pointers of my neighbors */

  if (prev != NULL)
    prev->next = next; 
  if (next != NULL)
    next->prev = prev;

  /* Fix the top and bottom pointers */

  if (rl->size == 0)
    rl->top = rl->bottom = NULL;
  else if (rl->top == rli)
    rl->top = next;
  else if (rl->bottom == rli)
    rl->bottom = prev;

  lam_debug_cond((lam_ssi_rpi_gm_did,
                  "reqlist_rm_item: top:%p, top->next:%p, "
                  "bottom->prev:%p, bottom:%p",
                  rl->top, 
                  (rl->top != 0) ? rl->top->next : 0,
                  (rl->bottom != 0) ? rl->bottom->prev: 0,
                  rl->bottom));
  
  /* Since we originally malloc'ed rli, then we should free it */

  free(rli);
  lam_debug_cond((lam_ssi_rpi_gm_did, 
                  "reqlist_rm_item:rl %p size now %d", 
                  rl, rl->size));

  return 0;
}


/*
 * Finds and removes a request from a reqlist.
 * Returns 1 it the request was found and removed, 0 if not found, and
 * LAMERROR on error.
 */
int
lam_ssi_rpi_gm_reqlist_rm_req(struct lam_ssi_rpi_gm_reqlist_t *rl, 
			      MPI_Request req)
{
  int found = 0;
  struct lam_ssi_rpi_gm_reqlist_item_t *rli, *rli_temp;

  if (req != NULL) {
    for (rli = rl->top; rli != NULL; rli = rli_temp) {
      rli_temp = rli->next;
      if (rli->req == req) {
	if (lam_ssi_rpi_gm_reqlist_rm_item(rl, rli) == LAMERROR)
	  return LAMERROR;

	found = 1;
	break;
      }
    }
  }

  lam_debug_cond((lam_ssi_rpi_gm_did, 
                  "reqlist_rm_req:rl %p size now %d -- found %d", 
                  rl, rl->size, found));
  return found;
}


/*
 * Remove the top item from a reqlist -- a O(1) operation.
 * Returns LAMERROR on error, 0 on success
 */
int
lam_ssi_rpi_gm_reqlist_pop_top(struct lam_ssi_rpi_gm_reqlist_t *rl)
{
  struct lam_ssi_rpi_gm_reqlist_item_t *rli;

  if (rl == NULL)
    return LAMERROR;

  /* Remove the top item -- optimized */

  if (rl->top != NULL) {
    --(rl->size);

    rli = rl->top;
    rl->top = rl->top->next;
    if (rl->top != NULL)
      rl->top->prev = NULL;
    else
      rl->bottom = NULL;
    
    /* Since we originally malloc'ed rli, then we should free it */

    free(rli);
  }

  lam_debug_cond((lam_ssi_rpi_gm_did, 
		  "reqlist_pop_top:rl %p size now %d", rl, rl->size));
  return 0;
}
