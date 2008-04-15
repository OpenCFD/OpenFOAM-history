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
 * $Id: ssi_rpi_gm_fast.c,v 1.12 2004/02/12 17:49:11 vsahay Exp $
 *
 *	Function:	- fast version of fast and receive
 */

#include <lam_config.h>

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

#include <gm.h>
#include <rpi_gm.h>
#include <rpi_gm_dreg.h>
#include <rpi_gm_proc.h>
#include <rpi_gm_recv_events.h>
#include <rpi_gm_send_queue.h>


/*
 * Private functions
 */
static void send_callback(struct gm_port *port, void *context, 
                          gm_status_t status);


/*
 * Do the LAM fastsend of a message -- bypass the queues altogether
 * and do whatever is necessary to send the message now (in a blocking
 * manner).
 */
int
lam_ssi_rpi_gm_fastsend(char *buf, int count, MPI_Datatype dtype, int dest,
			int tag, MPI_Comm comm)
{
  int packsize;
  int copy_len = sizeof(lam_ssi_rpi_gm_envl_t);
  lam_ssi_rpi_gm_envl_t *env;
  struct lam_ssi_rpi_proc *destproc;

  /* We only handle tiny messages.  So check and see if this is a tiny
     message.  If it's not, return gracefully (the regular progression
     engine will take over). */

  packsize = count * dtype->dt_size;
  if (packsize > PUB(tinymsglen)) {
    errno = EBUSY;
    return LAMERROR;
  }

  /* Make sure we have enough tokens to send */

  if (!LAM_SSI_RPI_GM_SEND_QUEUE_EMPTY() || 
      (PUB(stokens) < 1) || 
      (1 == lam_ssi_rpi_gm_enqueue_sends)) {
    errno = EBUSY;
    return LAMERROR;
  }

  /* Check common arguments.  */
  
  if (count < 0)
    return lam_mkerr(MPI_ERR_COUNT, 0);
  
  if (dtype == MPI_DATATYPE_NULL || (!dtype->dt_commit))
    return lam_mkerr(MPI_ERR_TYPE, 0);
  
  if (LAM_IS_INTER(comm)) {
    if ((dest < 0) || (dest >= comm->c_rgroup->g_nprocs))
      return lam_mkerr(MPI_ERR_RANK, 0);
    
    destproc = comm->c_rgroup->g_procs[dest]->p_rpi;
  } else {
    if ((dest < 0) || (dest >= comm->c_group->g_nprocs))
      return lam_mkerr(MPI_ERR_RANK, 0);
    
    destproc = comm->c_group->g_procs[dest]->p_rpi;
  }

  /* Setup the envelope to send */

  env = lam_ssi_rpi_gm_dma_env_malloc();
  if (env == NULL)
    return lam_mkerr(MPI_ERR_NO_MEM, 0);

  env->ge_len = packsize;
  env->ge_tag = tag;
  env->ge_flags = C2CTINY;
  env->ge_rank = comm->c_group->g_myrank;
  env->ge_cid = comm->c_contextid;
  env->ge_peer_proc = destproc->cp_peer_proc;

  env->ge_src_request = NULL;
#if LAM_DISINFECT
  env->ge_target = NULL;
#if !LAM_SSI_RPI_GM_HAVE_RDMA_GET
  env->ge_dest_request = NULL;
#endif
#endif
  
  /* For non-zero length messages, copy the message into the
     envelope */
  
  if (count != 0 && dtype->dt_size != 0) {
    copy_len += packsize;

    /* If contiguous, just memcpy into the envelope */
    
    if ((dtype->dt_flags & LAM_DTNOPACK) &&
	((dtype->dt_flags & LAM_DTNOXADJ) || count == 1))
      memcpy(env + 1, buf, packsize);

    /* Otherwise, pack into the envelope. */
    
    else {
      if (lam_pack(buf, count, dtype, (char *) (env + 1), packsize) < 0)
	return lam_mkerr(MPI_ERR_INTERN, errno);
    }
  }

  /* Push the envelope */

  gm_send_with_callback(lam_myproc->p_rpi->cp_gm_port.rgp_gm_port,
                        env,
                        lam_ssi_rpi_gm_env_min_size_for_length,
                        copy_len,
                        LAM_SSI_RPI_GM_PRIORITY,
                        destproc->cp_gm_port.rgp_local_node_id,
                        destproc->cp_gm_port.rgp_port_id,
                        send_callback,
                        env);

  /* We don't update the sent bookmark here because if cr support is
     active, then fastsend was disabled.  So that makes one less thing
     we have do to here, and therfore more latency avoided.  :-) */

  /* There's no need to wait for this send to complete -- the user's
     buffer is immediately available for re-use because we copied its
     contents, and gm will take care of all aspects of the send.  At
     some point later, the callback will be invoked and the envelope
     will be returned to the pool.  

     Question: How cool is that? 
     Answer: Very cool! */

  --PUB(stokens);
  return MPI_SUCCESS;
}


static void 
send_callback(struct gm_port *port, void *context, gm_status_t status)
{
  if (GM_SUCCESS != status) {
    gm_u32_t my_id = 0;
    gm_u8_t my_name[GM_MAX_HOST_NAME_LEN + 1] = { 0 };
    char mystring[GM_MAX_HOST_NAME_LEN + 128];

    gm_get_node_id(port, &my_id);
    gm_get_host_name(port, (char *) my_name);

    snprintf(mystring, GM_MAX_HOST_NAME_LEN + 128,
	     "LAM internal GM send: gmID=%d '%s' send failed to "
	     "complete (see kernel log for details).",
	     my_id, my_name);
    gm_perror ((const char *) mystring, status);
  }
  ++PUB(stokens);
  lam_ssi_rpi_gm_dma_env_free(context);
}

/*************************************************************************/

/*
 * Do the LAM fastrecv of a message -- bypass the queues altogether
 * and block while waiting to receive this one specific message from
 * this one specific MPI process.
 */
int
lam_ssi_rpi_gm_fastrecv(char *buf, int count, MPI_Datatype dtype, int src,
			int *tag, MPI_Comm comm, MPI_Status *stat, int *seqnum)
{
  printf("gm_low.c: lam_ssi_rpi_gm_fastrecv not implemented yet");
  lamlog("gm_low.c: lam_ssi_rpi_gm_fastrecv not implemented yet");

#if 0
  double		local_rep;		/* local data representation */
  double		net_rep;		/* net data representation */
  struct _proc	*srcproc;
  char		*packbuf;
  int			packsize;
  int			err;

  printf("lam_ssi_rpi_gm_fastrecv not implemented in gm RPI yet\n");
  return (LAMERROR);

  lam_debug_cond((lam_ssi_rpi_gm_did, "fastrecv: Starting fastrecv"));
  
  /* Check common arguments.  */

  if (count < 0)
    return(lam_mkerr(MPI_ERR_COUNT, 0));
  
  if (dtype == MPI_DATATYPE_NULL || (!dtype->dt_commit))
    return(lam_mkerr(MPI_ERR_TYPE, 0));
  
  if (LAM_IS_INTER(comm)) {
    if ((src < 0) || (src >= comm->c_rgroup->g_nprocs))
      return(lam_mkerr(MPI_ERR_RANK, 0));
    
    srcproc = comm->c_rgroup->g_procs[src];
  } else {
    if ((src < 0) || (src >= comm->c_group->g_nprocs))
      return(lam_mkerr(MPI_ERR_RANK, 0));
    
    srcproc = comm->c_group->g_procs[src];
  }

  /* Handle zero length messages.  */

  if (count == 0 || dtype->dt_size == 0) {
    packbuf = buf;
    packsize = 0;
  } else {
    local_rep = 1.1;
    ltotf8(&local_rep, &net_rep);

    /* If contiguous, use the caller's buffer.  */

    packsize = count * dtype->dt_size;

    if ((dtype->dt_flags & LAM_DTNOPACK) &&
	((dtype->dt_flags & LAM_DTNOXADJ) || count == 1) &&
	((local_rep == net_rep) || (srcproc->p_mode & LAM_PHOMOG))) {
      packbuf = buf;

      /* Check for bad buffer.  */

      if (packbuf == 0)
	return(lam_mkerr(MPI_ERR_BUFFER, 0));
    }

    /* Otherwise allocate a buffer.  */

    else {
      packbuf = malloc(packsize);
      if (packbuf == 0)
	return(lam_mkerr(MPI_ERR_OTHER, errno));
    }
  }

  err = lam_ssi_rpi_gm_fastrecv_low(packbuf, &packsize,
				    srcproc->p_rpi, src, tag, comm, seqnum);

  if (stat != MPI_STATUS_IGNORE) {
    stat->MPI_SOURCE = src;
    stat->MPI_TAG = *tag;
    stat->MPI_ERROR = err;
    stat->st_length = packsize;
  }

  if (packbuf != buf) {
    if (lam_unpack(packbuf, packsize, buf, count, dtype) < 0)
      return(lam_mkerr(MPI_ERR_INTERN, errno));
    
    free(packbuf);
  }
  
  return(err);
#endif
  return 0;
}
