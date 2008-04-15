/*
 * Copyright (c) 2001-2003 The Trustees of Indiana University.  
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
 * $Id: lamcomm.c,v 6.20 2003/05/08 22:40:13 jsquyres Exp $
 *
 *	Function:	- miscellaneous communicator functions
 */

#include <stdlib.h>
#include <string.h>

#include <lam_config.h>
#include <mpi.h>
#include <mpisys.h>
#include <typical.h>
#include <rpisys.h>
#include <lamdebug.h>
#if LAM_WANT_IMPI
#include <impi.h>
#endif
#include <etc_misc.h>


/*
 * private functions
 */
#if LAM_WANT_IMPI
static int num_remotes(MPI_Group group);
static MPI_Group make_local(MPI_Group orig);
#endif


/*
 * private variables
 */
static int comm_mpid = -1;


/* Variable to keep track of changes in lam communicators for TV queue
   support */
extern int lam_tv_comm_seqnum;


/*
 *	lam_comm_free
 *
 *	Function:	- really free a communicator
 *	Accepts:	- communicator
 *	Returns:	- 0 or LAMERROR
 */
int
lam_comm_free(MPI_Comm comm)
{
	int		err;
	int		key;
	struct _attr	*p;

/* 
 * Drop a trace saying that this name is no longer being used 
 */
	MPI_Comm_set_name(comm, "");
/*
 * Free the groups.
 */
	err = MPI_Group_free(&(comm->c_group));
	if (err != MPI_SUCCESS) return(err);

	if (LAM_IS_INTER(comm)) {
		err = MPI_Group_free(&(comm->c_rgroup));
		if (err != MPI_SUCCESS) return(err);
	}
/*
 * Free the attribute keys.
 */
	if (comm->c_keys) {
		p = (struct _attr *) ah_top(comm->c_keys);
		while (p) {
			key = p->a_key;
			err = MPI_Comm_delete_attr(comm, key);
			if (err != MPI_SUCCESS) return(err);

			err = MPI_Comm_free_keyval(&key);
			if (err != MPI_SUCCESS) return(err);

			p = (struct _attr *) ah_next(comm->c_keys, (void *) p);
		}
		ah_free(comm->c_keys);
	}
/*
 * Free the error handler.
 */
	err = MPI_Errhandler_free(&(comm->c_errhdl));
	if (err != MPI_SUCCESS) return(err);
/*
 * Free topology information.
 */
	if (LAM_IS_CART(comm)) {
		free((char *) comm->c_topo_dims);
	} else if (LAM_IS_GRAPH(comm)) {
		free((char *) comm->c_topo_index);
	}
/*
 * Free the context ID.
 */
	lam_rmcid(comm->c_contextid);
/*
 * Remove the fortran handle if the user called MPI_Comm_f2c on this
 * handle
 */
	if (comm->c_f77handle >= 0)
	  lam_F_free_hdl(comm->c_f77handle);
/*
 * Remove the communicator from the communicator list and free it.
 */
	al_delete(lam_comms, al_find(lam_comms, &comm));
	lam_mp_free(comm_mpid, comm);

	/* Signal for TV queue library that lam_comms has changed */
	++lam_tv_comm_seqnum;

	return(MPI_SUCCESS);
}


/*
 *	lam_comm_new
 *
 *	Function:	- initalize a new communicator
 *			- allocates storage for new communicator if necessary
 *	Accepts:	- context ID
 *			- local group
 *			- remote group
 *			- communicator flags
 *			- communicator (inout)
 *	Returns:	- 0 or LAMERROR
 */
int
lam_comm_new(int cid, MPI_Group l_group, MPI_Group r_group, int flags,
	     MPI_Comm *comm)
{
	int i;
	int homog = 1;
        struct _proc *p;

	if (*comm == 0) {
          if (comm_mpid == -1)
 	    comm_mpid = lam_mp_init(sizeof(struct _comm), NULL, NULL,
                                    LAM_DEFAULT_MP_POOL_SIZE,
 				    LAM_DEFAULT_MP_INCR_SIZE, 1);
 	  if ((*comm = (MPI_Comm) lam_mp_malloc(comm_mpid)) == 0)
            return(LAMERROR);
        }

	(*comm)->c_flags = flags;
	(*comm)->c_f77handle = -1;
	(*comm)->c_refcount = 1;
	(*comm)->c_cube_dim = lam_cubedim(l_group->g_nprocs);
	(*comm)->c_group = l_group;
	(*comm)->c_rgroup = (flags & LAM_CINTER) ? r_group : MPI_GROUP_NULL;
	(*comm)->c_contextid = cid;
	(*comm)->c_errhdl = MPI_ERRORS_ARE_FATAL;
	(*comm)->c_keys = 0;
	(*comm)->c_topo_type = MPI_UNDEFINED;
	(*comm)->c_topo_nprocs = 0;
	(*comm)->c_topo_ndims = 0;
	(*comm)->c_topo_dims = 0;
	(*comm)->c_topo_index = 0;
	(*comm)->c_topo_nedges = 0;
	(*comm)->c_topo_edges = 0;
	(*comm)->c_window = MPI_WIN_NULL;

	memset((*comm)->c_name, 0, sizeof((*comm)->c_name));

	/* Zero out the SSI function data, just so that there's no
	   confusion */
	
	memset(&((*comm)->c_ssi_coll), 0, sizeof((*comm)->c_ssi_coll));
	(*comm)->c_ssi_coll_data = NULL;

	/* Is this communicator endian homogeneous? */

	if (l_group != MPI_GROUP_NULL)
	  for (i = 0; i < l_group->g_nprocs; ++i) {
	    p = l_group->g_procs[i];
	    if (p != lam_myproc && !(p->p_mode & LAM_PHOMOG)) {
	      homog = 0;
	      break;
	    }
	  }
	if ((flags & LAM_CINTER) && homog)
	  for (i = 0; i < r_group->g_nprocs; ++i) {
	    p = r_group->g_procs[i];
	    if (!(p->p_mode & LAM_PHOMOG)) {
	      homog = 0;
	      break;
	    }
	  }
	if (homog)
	  (*comm)->c_flags |= LAM_CHOMOG;

	(*comm)->c_shadow = 0;
#if LAM_WANT_IMPI
        /* If at least one member of the local or remote group is not
	   within this LAM, we need to build a duplicate communicator
	   in c_shadow, but only with members that are local to this
	   LAM.  Shadow ranks will be kept in the same order as the
	   original communicator, but will be compressed to start with
	   zero. */
	if (_kio.ki_rtf & RTF_IMPI) {
	  if (num_remotes(l_group) > 0 || 
	      num_remotes((*comm)->c_rgroup) > 0) {
	    MPI_Group newl;
	    MPI_Group newr;
	    
	    /* This communicator is no longer endian-homogenous */

	    (*comm)->c_flags &= ~(LAM_CHOMOG);

	    newl = make_local(l_group);
	    newr = make_local((*comm)->c_rgroup);
	    
	    /* Trickyness in lamcid.c -- cid's are incremented by 2, so
	       we know that we can grab this contextid + 1 */
	    
	    lam_comm_new(cid + 1, newl, newr, flags, &((*comm)->c_shadow));
	    (*comm)->c_shadow->c_errhdl->eh_refcount++;

            if (lam_ssi_coll_base_init_comm((*comm)->c_shadow) != 0)
              return LAMERROR;
	  }
	}
#endif

	/* Signal for TV queue library that lam_comms has changed */
	++lam_tv_comm_seqnum;

	return(0);
}


#if LAM_WANT_IMPI
/*
 *	num_remotes
 *
 *	Function:	- count to see how many procs in this group
 *                        are not in this LAM
 *	Accepts:	- group
 *	Returns:	- number of remote processes
 */
static int
num_remotes(MPI_Group group)
{
  int i, count = 0;

  if (group == MPI_GROUP_NULL || group == MPI_GROUP_EMPTY) {
    return 0;
  }

  for (i = 0; i < group->g_nprocs; i++)
    if (LAM_GPSCMP(&(group->g_procs[i]->p_gps), &gimpid) == 0) {
      count++;
    }

  return count;
}


/*
 *	make_local
 *
 *	Function:	- make a new group that only has local members
 *	Accepts:	- group
 *	Returns:	- new group, subset of original, containing only
 *                        processes local to this LAM
 *
 */
static MPI_Group
make_local(MPI_Group orig)
{
  int i, j, num;
  int *ranks;
  MPI_Group newg;
  
  if (orig == MPI_GROUP_NULL || orig == MPI_GROUP_EMPTY)
    return orig;

  num = num_remotes(orig);
  ranks = malloc(sizeof(int) * num);
  for (j = i = 0; i < orig->g_nprocs; i++)
    if (LAM_GPSCMP(&(orig->g_procs[i]->p_gps), &gimpid) == 0) {
      ranks[j++] = i;
    }

  if (MPI_Group_excl(orig, num, ranks, &newg) != MPI_SUCCESS) {
    free(ranks);
    return MPI_GROUP_NULL;
  }
  free(ranks);

  return newg;
}

#endif
