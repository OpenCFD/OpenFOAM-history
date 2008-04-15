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
 * $Id: ssi_coll_lam_basic_bcast.c,v 1.2 2003/05/28 00:16:28 jsquyres Exp $
 *
 *	Function:	- Basic collective routines
 */

#include <lam_config.h>
#if LAM_WANT_PROFILE
#define LAM_PROFILELIB 1
#endif
#include <lam-ssi-coll-lam-basic-config.h>

#include <lam-ssi-coll.h>
#include <lam-ssi-coll-lam-basic.h>
#include <mpi.h>
#include <blktype.h>
#include <mpisys.h>


/*
 *	bcast_lin
 *
 *	Function:	- broadcast using O(N) algorithm
 *	Accepts:	- same arguments as MPI_Bcast()
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_coll_lam_basic_bcast_lin(void *buff, int count,
				 MPI_Datatype datatype, int root,
				 MPI_Comm comm)
{
  int i;
  int size;
  int rank;
  int err;
  MPI_Request *preq;
  MPI_Request reqs[LAM_COLLMAXLIN];

  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);
  lam_mkcoll(comm);

  /* Non-root receive the data. */

  if (rank != root) {
    err = MPI_Recv(buff, count, datatype, root,
		   BLKMPIBCAST, comm, MPI_STATUS_IGNORE);
    lam_mkpt(comm);
    if (err != MPI_SUCCESS)
      return err;

    return MPI_SUCCESS;
  }

  /* Root sends data to all others. */

  for (i = 0, preq = reqs; i < size; ++i) {
    if (i == rank)
      continue;

    err = MPI_Send_init(buff, count, datatype, i, BLKMPIBCAST,
			comm, preq++);
    if (err != MPI_SUCCESS) {
      lam_mkpt(comm);
      return err;
    }
  }

  /* Start and wait on all requests. */

  err = MPI_Startall(size - 1, reqs);
  if (err != MPI_SUCCESS) {
    lam_mkpt(comm);
    return err;
  }

  err = MPI_Waitall(size - 1, reqs, MPI_STATUSES_IGNORE);
  lam_mkpt(comm);
  if (err != MPI_SUCCESS)
    return err;

  /* Free the requests. */

  for (i = 0, preq = reqs; i < size; ++i) {
    if (i == rank)
      continue;

    err = MPI_Request_free(preq);
    if (err != MPI_SUCCESS)
      return err;

    ++preq;
  }

  /* All done */

  return (MPI_SUCCESS);
}


/*
 *	bcast_log
 *
 *	Function:	- broadcast using O(log(N)) algorithm
 *	Accepts:	- same arguments as MPI_Bcast()
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_coll_lam_basic_bcast_log(void *buff, int count,
				 MPI_Datatype datatype, int root,
				 MPI_Comm comm)
{
  int size;
  int rank;
  int vrank;
  int vsize;
  int err;

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);
  lam_mkcoll(comm);
  vrank = (rank + size - root) % size;
  vsize = 1;
  while ((vrank & vsize) == 0 && vsize < size)  vsize <<= 1;


  /* Receive data from parent in the tree. */

  if (vrank > 0) {
    int peer =  (vrank - vsize + root) % size;
    err = MPI_Recv(buff, count, datatype, peer,
		   BLKMPIBCAST, comm, MPI_STATUS_IGNORE);
    if (err != MPI_SUCCESS) {
      lam_mkpt(comm);
      return err;
    }
  }

  /* Send data to the children. */

  vsize >>= 1;
  while (vsize > 0) {
    /* make sure that the target node exists
      (might not exist if log_2(size) is not integer) */
    if ((vrank + vsize) < size) {
      int dst = (vrank + vsize + root) % size;
      err = MPI_Send(buff, count, datatype, dst, BLKMPIBCAST, comm);
      if (err != MPI_SUCCESS) {
        lam_mkpt(comm);
        return err;
      }
    }
    vsize >>= 1;
  }


  /* All done */

  lam_mkpt(comm);
  return (MPI_SUCCESS);
}


/*
 *	bcast_lin_lamd
 *
 *	Function:	- LAMD broadcast using O(N) algorithm
 *	Accepts:	- same arguments as MPI_Bcast()
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_coll_lam_basic_bcast_lin_lamd(void *buff, int count,
				      MPI_Datatype datatype, int root,
				      MPI_Comm comm)
{
  int i;
  int size;
  int rank;
  int longproto;
  int err;
  int nreqs;
  int nacks;
  int done;
  int idx[LAM_COLLMAXDIM];
  MPI_Request reqs[LAM_COLLMAXDIM];
  MPI_Status stats[LAM_COLLMAXDIM];

  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);
  longproto = (datatype->dt_size * count) > MAXNMSGLEN;
  lam_mkcoll(comm);

  /* Non-root receives the data. */

  if (rank != root) {
    if (longproto) {
      err = MPI_Recv(buff, 0, MPI_BYTE, root,
		     BLKMPIBCAST, comm, MPI_STATUS_IGNORE);
      if (err != MPI_SUCCESS) {
	lam_mkpt(comm);
	return err;
      }

      err = MPI_Send(buff, 0, MPI_BYTE, root, BLKMPIBCAST, comm);
      if (err != MPI_SUCCESS) {
	lam_mkpt(comm);
	return err;
      }
    }

    err = MPI_Recv(buff, count, datatype, root,
		   BLKMPIBCAST, comm, MPI_STATUS_IGNORE);
    lam_mkpt(comm);
    if (err != MPI_SUCCESS)
      return err;

    return (MPI_SUCCESS);
  }

  /* Root sends data to all others. */

  if (longproto) {
    nreqs = 0;
    for (i = 0; i < size; ++i) {
      if (i == rank)
	continue;

      err = MPI_Send(buff, 0, MPI_BYTE, i, BLKMPIBCAST, comm);
      if (err != MPI_SUCCESS) {
	lam_mkpt(comm);
	return err;
      }

      err = MPI_Irecv(buff, 0, MPI_BYTE,
		      i, BLKMPIBCAST, comm, &reqs[nreqs++]);
      if (err != MPI_SUCCESS) {
	lam_mkpt(comm);
	return err;
      }
    }

    /* Wait for primer ack messages and send data. */

    nacks = 0;
    while (nacks < nreqs) {
      err = MPI_Waitsome(nreqs, reqs, &done, idx, stats);
      if (err != MPI_SUCCESS) {
	lam_mkpt(comm);
	return err;
      }

      if (done == MPI_UNDEFINED) {
	lam_mkpt(comm);
	return err;
      }
      nacks += done;

      for (i = 0; i < done; ++i) {
	err = MPI_Send(buff, count, datatype,
		       stats[i].MPI_SOURCE, BLKMPIBCAST, comm);
	if (err != MPI_SUCCESS) {
	  lam_mkpt(comm);
	  return err;
	}
      }
    }
  }

  /* short protocol */

  else {
    for (i = 0; i < size; ++i) {
      if (i == root)
	continue;

      err = MPI_Send(buff, count, datatype, i, BLKMPIBCAST, comm);
      if (err != MPI_SUCCESS) {
	lam_mkpt(comm);
	return err;
      }
    }
  }

  /* All done */

  lam_mkpt(comm);
  return (MPI_SUCCESS);
}


/*
 *	bcast_log_lamd
 *
 *	Function:	- LAMD broadcast using O(log(N)) algorithm
 *	Accepts:	- same arguments as MPI_Bcast()
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_coll_lam_basic_bcast_log_lamd(void *buff, int count,
				      MPI_Datatype datatype, int root,
				      MPI_Comm comm)
{
  int i;
  int size;
  int rank;
  int vrank;
  int peer;
  int dim;
  int hibit;
  int mask;
  int longproto;
  int err;
  int nreqs;
  int nacks;
  int done;
  int idx[LAM_COLLMAXDIM];
  MPI_Request reqs[LAM_COLLMAXDIM];
  MPI_Status stats[LAM_COLLMAXDIM];

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);
  lam_mkcoll(comm);
  longproto = (datatype->dt_size * count) > MAXNMSGLEN;
  vrank = (rank + size - root) % size;
  dim = comm->c_cube_dim;
  hibit = lam_hibit(vrank, dim);
  --dim;

  /* Receive data from parent in the tree. */

  if (vrank > 0) {
    peer = ((vrank & ~(1 << hibit)) + root) % size;
    if (longproto) {
      err = MPI_Recv(buff, 0, MPI_BYTE, peer,
		     BLKMPIBCAST, comm, MPI_STATUS_IGNORE);
      if (err != MPI_SUCCESS) {
	lam_mkpt(comm);
	return err;
      }

      err = MPI_Send(buff, 0, MPI_BYTE, peer, BLKMPIBCAST, comm);
      if (err != MPI_SUCCESS) {
	lam_mkpt(comm);
	return err;
      }
    }

    err = MPI_Recv(buff, count, datatype, peer,
		   BLKMPIBCAST, comm, MPI_STATUS_IGNORE);
    if (err != MPI_SUCCESS) {
      lam_mkpt(comm);
      return err;
    }
  }

  /* Send data or primer message to the children. */

  nreqs = 0;
  for (i = hibit + 1, mask = 1 << i; i <= dim; ++i, mask <<= 1) {
    peer = vrank | mask;
    if (peer < size) {
      peer = (peer + root) % size;
      if (longproto) {
	err = MPI_Send(buff, 0, MPI_BYTE, peer, BLKMPIBCAST, comm);
	if (err != MPI_SUCCESS) {
	  lam_mkpt(comm);
	  return err;
	}

	err = MPI_Irecv(buff, 0, MPI_BYTE,
			peer, BLKMPIBCAST, comm, &reqs[nreqs++]);
      } else {
	err = MPI_Send(buff, count, datatype, peer, BLKMPIBCAST, comm);
      }
      if (err != MPI_SUCCESS) {
	lam_mkpt(comm);
	return err;
      }
    }
  }

  if (longproto) {

    /* Wait for primer ack messages and send data. */

    nacks = 0;
    while (nacks < nreqs) {
      err = MPI_Waitsome(nreqs, reqs, &done, idx, stats);
      if (err != MPI_SUCCESS) {
	lam_mkpt(comm);
	return err;
      }

      if (done == MPI_UNDEFINED) {
	lam_mkpt(comm);
	return err;
      }

      nacks += done;
      for (i = 0; i < done; ++i) {
	err = MPI_Send(buff, count, datatype,
		       stats[i].MPI_SOURCE, BLKMPIBCAST, comm);
	if (err != MPI_SUCCESS) {
	  lam_mkpt(comm);
	  return err;
	}
      }
    }
  }

  /* All done */

  lam_mkpt(comm);
  return (MPI_SUCCESS);
}


/*
 *	bcast_inter
 *
 *	Function:	- intercomm broadcast
 *	Accepts:	- same arguments as MPI_Bcast()
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_coll_lam_basic_bcast_inter(void *buff, int count,
				   MPI_Datatype datatype, int root,
				   MPI_Comm intercomm)
{
  int local_rank;
  MPI_Comm local_comm;

  local_comm = intercomm->c_ssi_coll_lam_basic_data->local_comm;

  MPI_Comm_rank(intercomm, &local_rank);
  lam_mkcoll(intercomm);

  /* Root sends to rank 0 in the remote group and it then does a local bcast */

  if (root == MPI_PROC_NULL) {
    /* local non-root process, simply return */
    lam_mkpt(intercomm);
    return MPI_SUCCESS;
  
  } else if (root == MPI_ROOT) {
    /* send data to rank 0 in the remote group */
    if ((MPI_Send(buff, count, datatype, 0, BLKMPIBCAST, intercomm)) 
	!= MPI_SUCCESS) {
      lam_mkpt(intercomm);
      return LAMERROR;
    }
  } else {
    /* remote group */
    /* rank 0 recives the data from the root */
    
    if (local_rank == 0) {
      if ((MPI_Recv(buff, count, datatype, root, BLKMPIBCAST, intercomm, 
		    MPI_STATUS_IGNORE)) != MPI_SUCCESS) {
	lam_mkpt(intercomm);
	return LAMERROR;
      }
    }        
    
    /* local broadcast */
    if ((MPI_Bcast(buff, count, datatype, 0, local_comm)) 
	!= MPI_SUCCESS) {
      lam_mkpt(intercomm);
      return LAMERROR;
    }
  }
  
  lam_mkpt(intercomm);
  return MPI_SUCCESS;
}
