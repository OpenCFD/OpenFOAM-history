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
 *	Ohio Trollius
 *	Copyright 1997 The Ohio State University
 *	RBD/NJN
 *
 *	$Id: ssi_rpi_cbuf.c,v 1.5 2003/10/11 19:15:36 brbarret Exp $
 *
 *	Function:	- client-to-client message buffering
 */

#include <lam_config.h>

#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>

#if defined(HAVE_SYS_SELECT_H) && HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#include <all_hash.h>
#include <mpi.h>
#include <mpisys.h>
#include <portable.h>
#include <lam-ssi-rpi.h>
#include <rpisys.h>
#include <t_types.h>
#include <terror.h>
#include <typical.h>
#include <etc_misc.h>


/*
 * local functions
 */
static MPI_Comm cid_2_comm(int cid);
static void inc_ger_ctr(MPI_Comm comm, int rank, int inc);


/*
 * local variables
 */
static HASH *buftbl = 0;	/* buffer hash table */


/*
 *	lam_ssi_rpi_cbuf_init
 *
 *	Function:	- initialize c2c buffers
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_cbuf_init(void)
{
  lam_ssi_rpi_cbuf_end();

  buftbl = ah_init(INITHASH, sizeof(struct lam_ssi_rpi_cbuf_cid), INT4_MAX, 0);
  if (buftbl == 0)
    return (LAMERROR);

  return (0);
}


/*
 *	lam_ssi_rpi_cbuf_end
 *
 *	Function:	- destroy c2c buffers
 */
void
lam_ssi_rpi_cbuf_end(void)
{
  struct lam_ssi_rpi_cbuf_cid *p;

  if (buftbl == 0)
    return;
/*
 * Loop over all context IDs.
 */
  for (p = (struct lam_ssi_rpi_cbuf_cid *) ah_top(buftbl); p;
       p = (struct lam_ssi_rpi_cbuf_cid *) ah_next(buftbl, p)) {

    if (p->cb_envs)
      al_free(p->cb_envs);
  }

  ah_free(buftbl);
  buftbl = 0;
}


/*
 *	lam_ssi_rpi_cbuf_find
 *
 *	Function:	- find a matching message
 *	Accepts:	- ptr request envlope
 *	Returns:	- ptr message or NULL
 */
struct lam_ssi_rpi_cbuf_msg *
lam_ssi_rpi_cbuf_find(struct lam_ssi_rpi_envl *rqenv)
{
  struct lam_ssi_rpi_cbuf_cid *pcid;	/* ptr CID entry */
  struct lam_ssi_rpi_cbuf_msg *pmsg = 0;	/* ptr message */
/*
 * Don't even bother hashing if the table is empty.
 */
  if (ah_count(buftbl) <= 0)
    return (0);

  pcid = (struct lam_ssi_rpi_cbuf_cid *) ah_find(buftbl, rqenv->ce_cid);
  if (pcid && pcid->cb_envs) {
/*
 * Loop over list of messages finding first match.
 */
    pmsg = al_top(pcid->cb_envs);

    while (pmsg) {
      if (!lam_ssi_rpi_envl_cmp(&(pmsg->cm_env), rqenv))
	return (pmsg);
      pmsg = al_next(pcid->cb_envs, pmsg);
    }
  }

  return (0);
}


/*
 *	lam_ssi_rpi_cbuf_delete
 *
 *	Function:	- remove a message
 *			- context ID entries are never deleted
 *	Accepts:	- message
 */
void
lam_ssi_rpi_cbuf_delete(struct lam_ssi_rpi_cbuf_msg *msg)
{
  struct lam_ssi_rpi_cbuf_cid *pcid;	/* ptr CID entry */

  pcid = (struct lam_ssi_rpi_cbuf_cid *) ah_find(buftbl, msg->cm_env.ce_cid);
  if (pcid == 0 || pcid->cb_envs == 0)
    return;

  if (lam_ger)
    inc_ger_ctr(pcid->cb_comm, msg->cm_env.ce_rank, -1);

  /* Don't delete the buffer if it was a c2c ssend to ourselves,
     becuase in this special case, we just re-used the user's
     buffer */

  if (msg->cm_buf && !msg->cm_dont_delete)
    free(msg->cm_buf);

  al_delete(pcid->cb_envs, msg);
}


/*
 *	lam_ssi_rpi_cbuf_append
 *
 *	Function:	- add new message to end of list
 *			- expand hash table if needed
 *	Accepts:	- message
 *	Returns:	- buffered message or NULL
 */
void *
lam_ssi_rpi_cbuf_append(struct lam_ssi_rpi_cbuf_msg *msg)
{
  struct lam_ssi_rpi_cbuf_cid *pcid;	/* ptr CID entry */
  struct lam_ssi_rpi_cbuf_cid cident;	/* CID entry */
  int newsize;			/* expanded table size */

  pcid = (struct lam_ssi_rpi_cbuf_cid *) ah_find(buftbl, msg->cm_env.ce_cid);
  if (pcid == 0) {
/*
 * Expand hash table if no more free entries.
 */
    if (ah_count(buftbl) == (newsize = ah_size(buftbl))) {
      newsize = next_prime(newsize + newsize);
      if (ah_expand(buftbl, newsize))
	return (0);
    }
/*
 * Create new CID entry.
 */
    LAM_ZERO_ME(cident);
    cident.cb_cid = msg->cm_env.ce_cid;
    cident.cb_envs = 0;
    if (lam_ger)
      cident.cb_comm = cid_2_comm(cident.cb_cid);
    if (ah_insert(buftbl, (void *) &cident))
      return (0);

    pcid = (struct lam_ssi_rpi_cbuf_cid *)
      ah_find(buftbl, msg->cm_env.ce_cid);

    if (pcid == 0) {
      errno = EIMPOSSIBLE;
      return (0);
    }
  }
/*
 * Create message list if necessary.
 */
  if (!pcid->cb_envs) {
    pcid->cb_envs = al_init(sizeof(struct lam_ssi_rpi_cbuf_msg), 0);
    if (!pcid->cb_envs)
      return (0);
  }
/*
 * Link message to end of list.
 */
  if (lam_ger)
    inc_ger_ctr(pcid->cb_comm, msg->cm_env.ce_rank, 1);

  return (al_append(pcid->cb_envs, msg));
}


/*
 *	cid_2_comm
 *
 *	Function:	- finds communicator from context ID
 *	Accepts:	- context ID
 *	Returns:	- communicator
 */
static MPI_Comm
cid_2_comm(int cid)
{
  struct _comm celem;
  MPI_Comm comm;
  MPI_Comm *pcomm;

  celem.c_contextid = cid;
  comm = &celem;

  pcomm = al_find(lam_comms, &comm);

  return ((pcomm) ? *pcomm : MPI_COMM_NULL);
}


/*
 *	inc_ger_ctr
 *
 *	Function:	- increment GER counter
 *	Accepts:	- context ID
 *			- rank of process in communicator
 *			- increment
 */
static void
inc_ger_ctr(MPI_Comm comm, int rank, int inc)
{
  struct _proc *p;

  if (LAM_IS_INTER(comm))
    p = comm->c_rgroup->g_procs[rank];
  else
    p = comm->c_group->g_procs[rank];

  p->p_num_buf_env += inc;
}
