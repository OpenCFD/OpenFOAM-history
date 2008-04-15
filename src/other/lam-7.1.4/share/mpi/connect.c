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
 * $Id: connect.c,v 6.15 2003/08/22 02:33:18 brbarret Exp $
 *
 *	Function:	- connect to an MPI port
 *	Accepts:	- port name
 *			- info
 *			- client root process rank
 *			- client side communicator
 *			- intercomm between client and server (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <lam_config.h>
#include <sfh.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <app_mgmt.h>
#include <blktype.h>
#include <dl_inet.h>
#include <mpi.h>
#include <mpisys.h>
#include <net.h>
#include <portable.h>
#include <lam-ssi-rpi.h>
#include <rpisys.h>
#include <terror.h>
#include <typical.h>

/*
 * private functions
 */
static int connect_to_port(char *port_name, MPI_Comm comm, int cid, 
			   int *svrcid, int *svrsize, struct _gps **svrgps);
static void fill_fyi(MPI_Comm comm);


/*@
   MPI_Comm_connect - Make a connection to another MPI process

Input Parameters:
+ port_name - port name (only used at root)
. info - extra information handle (only used at root)
. root - rank in comm of root node
- comm - intracommunicator over which the call is collective

Output Parameter:
+ newcomm - intercommunicator with server as remote group

Description:

The 'MPI_Comm_accept' and 'MPI_Comm_connect' calls simulate the
well-understood 'accept()'/'connect()' semantics of standard socket
programming -- a client "connects" to a server.

LAM/MPI allows for previously-unrelated MPI programs to join into a
MPI program with these calls.  Both MPI programs must be running by
the same user in the same LAM universe (i.e., in the scope of a single
lamboot).

.N fortran

.N IMPI_MPI2

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_ROOT
.N MPI_ERR_ARG
.N MPI_ERR_INTERN
.N MPI_ERR_OTHER
.N MPI_ERR_FLAGS

.seealso MPI_Close_port(3), MPI_Comm_accept(3), MPI_Open_port(3)

.N WEB
@*/
int
MPI_Comm_connect(char *port_name, MPI_Info info, int root, MPI_Comm comm,
		 MPI_Comm *newcomm)
{
	struct _proc	**p;
	struct _gps	*g;
	struct _gps	*svrgps;		/* array of server GPS */
	MPI_Group	svrgrp;			/* server group */
	int		svrsize;		/* server communicator size */
	int		svrcid;			/* server global context ID */
	int		rank;			/* my local rank */
	int		size;			/* client communicator size */
	int		cid;			/* client global context ID */
	int		mycid;			/* my context ID */
	int		err;			/* error code */
	int		msg[2];			/* message buffer */
	int		i;
	
	lam_initerr_m();
	lam_setfunc_m(BLKMPICOMMCONNECT);
/*
 * Check the arguments.
 */
	if ((comm == MPI_COMM_NULL) || LAM_IS_INTER(comm)) {
		return(lam_errfunc(comm, BLKMPICOMMCONNECT,
					lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	MPI_Comm_size(comm, &size);

	if ((root >= size) || (root < 0)) {
		return(lam_errfunc(comm, BLKMPICOMMCONNECT,
					lam_mkerr(MPI_ERR_ROOT, EINVAL)));
	}

	if (newcomm == 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPICOMMCONNECT,
					lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	MPI_Comm_rank(comm, &rank);
	
	if ((rank == root) && (strlen(port_name) > LAM_PORTLEN)) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPICOMMCONNECT,
					lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	LAM_TRACE(lam_tr_cffstart(BLKMPICOMMCONNECT));
/*
 * Set debugging parameters.
 */
	g = &(comm->c_group->g_procs[root]->p_gps);

	lam_setparam(BLKMPICOMMCONNECT, root | (g->gps_grank << 16),
				(g->gps_node << 16) | g->gps_idx);
/*
 * Synchronize all members of the client communicator and get the
 * client global context ID for the intercommunicator.  
 */
	mycid = lam_getcid();
	if (mycid < 0) {
		return(lam_errfunc(comm, BLKMPICOMMCONNECT,
				lam_mkerr(MPI_ERR_INTERN, EFULL)));
	}

	err = MPI_Reduce(&mycid, &cid, 1, MPI_INT, MPI_MAX, root, comm);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPICOMMCONNECT, err));
	}

	if (rank == root) {
/*
 * Make a connection.
 */
		err = connect_to_port(port_name, comm,
				      cid, &svrcid, &svrsize, &svrgps);
		if (err != MPI_SUCCESS) {
			msg[0] = -1;
			msg[1] = err;
			MPI_Bcast(msg, 2, MPI_INT, root, comm);
			return(lam_errfunc(comm, BLKMPICOMMCONNECT, err));
		}
/*
 * The intercomm context ID is the maximium of the client and server
 * global context IDs.  
 */
		cid = LAM_max(cid, svrcid);
		msg[0] = cid;
		msg[1] = svrsize;
	}
/*
 * Broadcast the intercomm context ID, server communicator size and array of
 * server GPS to the client communicator.  
 */
	err = MPI_Bcast(msg, 2, MPI_INT, root, comm);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPICOMMCONNECT, err));
	}

	if (rank != root) {
		if (msg[0] < 0) {
			return(lam_errfunc(comm, BLKMPICOMMCONNECT, msg[1]));
		}

		cid = msg[0];
		svrsize = msg[1];
			
		svrgps = (struct _gps *) malloc(svrsize * sizeof(struct _gps));
		if (svrgps == 0) {
			return(lam_errfunc(comm, BLKMPICOMMCONNECT,
					lam_mkerr(MPI_ERR_OTHER, errno)));
		}
	}

	err = MPI_Bcast(svrgps, svrsize * sizeof(struct _gps) / sizeof(int),
		MPI_INT, root, comm);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPICOMMCONNECT, err));
	}
/*
 * Create the remote (server) group.
 */
	svrgrp = (MPI_Group) malloc((unsigned) (sizeof(struct _group) +
					(svrsize * sizeof(struct _proc *))));
	if (svrgrp == 0) {
		free((char *) svrgps);
		return(lam_errfunc(comm, BLKMPICOMMCONNECT,
					lam_mkerr(MPI_ERR_OTHER, errno)));
	}
	svrgrp->g_nprocs = svrsize;
	svrgrp->g_myrank = MPI_UNDEFINED;
	svrgrp->g_refcount = 1;
	svrgrp->g_f77handle = -1;
	svrgrp->g_procs = (struct _proc **)
				((char *) svrgrp + sizeof(struct _group));

	g = svrgps;
	p = svrgrp->g_procs;

	for (i = 0; i < svrsize; ++i, ++p, ++g) {

		if ((*p = lam_procadd(g)) == 0) {
			free((char *) svrgps);
			free((char *) svrgrp);
			return(lam_errfunc(comm, BLKMPICOMMCONNECT,
					lam_mkerr(MPI_ERR_OTHER, errno)));
		}
		if (!((*p)->p_mode & LAM_PRPIINIT)) {
			(*p)->p_mode |= LAM_PCLIENT;
		}
		(*p)->p_refcount++;
	}
/*
 * Create the client-server intercommunicator.
 */
	*newcomm = 0;
	if (lam_comm_new(cid, comm->c_group, svrgrp, LAM_CINTER, newcomm)) {
		free((char *) svrgps);
		free((char *) svrgrp);
		return(lam_errfunc(comm, BLKMPICOMMCONNECT,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	comm->c_group->g_refcount++;
	(*newcomm)->c_errhdl = comm->c_errhdl;
	comm->c_errhdl->eh_refcount++;

	if (!al_insert(lam_comms, newcomm)) {
		free((char *) svrgps);
		free((char *) svrgrp);
		return(lam_errfunc(comm, BLKMPICOMMCONNECT,
				lam_mkerr(MPI_ERR_INTERN, errno)));
	}

	if (lam_tr_comm(*newcomm)) {
		free((char *) svrgps);
		free((char *) svrgrp);
		return(lam_errfunc(comm, BLKMPICOMMCONNECT,
				lam_mkerr(MPI_ERR_INTERN, errno)));
	}

	lam_setcid(cid);
/*
 * setup new processes
 */
	if (RPI_ADDPROCS(svrgrp->g_procs, svrgrp->g_nprocs)) {
		return(lam_errfunc(comm, BLKMPICOMMCONNECT,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	}
	
	/* Let the collective SSI modules battle over who will do
           collectives on this new communicator.  */

        if (lam_ssi_coll_base_init_comm(*newcomm) != 0)
            return(lam_errfunc(comm, BLKMPICOMMCONNECT,
                                    lam_mkerr(MPI_ERR_INTERN, ENOSYS)));

	LAM_TRACE(lam_tr_cffend(BLKMPICOMMCONNECT, root, comm, 0, 0));
        lam_resetfunc_m(BLKMPICOMMCONNECT);
	
	/* Free memory */
	if (svrgps != NULL)
	  free((char*) svrgps);

	return(MPI_SUCCESS);
}

/*
 *	connect_to_port
 *
 *	Function:	- make a connection to a port
 *			- returns server side information
 *			- storage is allocated for the array of server GPS
 *	Accepts:	- port name
 *			- client side communicator
 *			- client side global intercomm context ID
 *			- server side global intercomm context ID (returned)
 *			- server communicator size (returned)
 *			- array of server GPS (returned)
 *	Returns:	- MPI_SUCCESS or error code
 */
static int
connect_to_port(char *port_name, MPI_Comm comm, int cid, int *svrcid, 
		int *svrsize, struct _gps **svrgps)
{
	struct nmsg	inmsg;			/* incoming message header */
	struct nmsg	outmsg;			/* outgoing message header */
	struct _proc	**p;
	struct _gps	*g;
	struct _gps	*cltgps;		/* array of client GPS */
	int		node;			/* server node */
	int		idx;			/* server index */
	int		port;			/* server port number */
	int		cltsize;		/* client communicator size */
	int		cltgpslen;		/* #bytes client GPS array */
	int		svrgpslen;		/* #bytes server GPS array */
	int		i;
/*
 * Find the server index, node, and port.
 */
	sscanf(port_name, "n%d:i%d:%d", &node, &idx, &port);
/*
 * Make the connection.
 * Synchronization used is, cid = -1, dest = server, tag = port number.
 */
	LAM_ZERO_ME(inmsg);
	LAM_ZERO_ME(outmsg);
	outmsg.nh_length = 0;
	outmsg.nh_flags = 0;

	_m2l_fillsys(lam_myproc->p_gps.gps_node, lam_myproc->p_gps.gps_idx,
		node, idx, -1, port, &outmsg);
/*
 * Pass synchronization for return message, client global context ID,
 * communicator size and run-time flags.  
 */
	MPI_Comm_size(comm, &cltsize);

	outmsg.nh_data[0] = lam_myproc->p_gps.gps_node;
	outmsg.nh_data[1] = lam_myproc->p_gps.gps_idx;
	outmsg.nh_data[2] = (int4) cid;
	outmsg.nh_data[3] = (int4) cltsize;
	outmsg.nh_data[4] = _kio.ki_rtf;

	fill_fyi(comm);
	if (nsend(&outmsg)) {
		return(lam_mkerr(MPI_ERR_OTHER, errno));
	}
/*
 * Read the server global context ID and communicator information.
 */
	inmsg.nh_length = 0;
	inmsg.nh_flags = 0;

	_m2l_fillsys(node, idx, lam_myproc->p_gps.gps_node,
		lam_myproc->p_gps.gps_idx, -1, port, &inmsg);

	if (nrecv(&inmsg)) {
		return(lam_mkerr(MPI_ERR_OTHER, errno));
	}
/*
 * Check for error code from server.
 */
	if (inmsg.nh_data[3] != 0) {
		return(lam_mkerr(MPI_ERR_OTHER, inmsg.nh_data[3]));
	}

	*svrcid = inmsg.nh_data[1];
	*svrsize = inmsg.nh_data[2];
/*
 * Prepare client information (proposed context ID, communicator
 * size and GPS array) for passing to server.  
 */
	cltgpslen = cltsize * sizeof(struct _gps);
	cltgps = (struct _gps *) malloc((unsigned) cltgpslen);
	if (cltgps == 0) return(lam_mkerr(MPI_ERR_OTHER, errno));

	p = comm->c_group->g_procs;
	g = cltgps;
	for (i = cltsize; i > 0; i--, p++, g++) {
		*g = (*p)->p_gps;
	}
/*
 * Send client GPS array to server.
 */
	outmsg.nh_flags |= DINT4MSG;
	outmsg.nh_msg = (char *) cltgps;
	outmsg.nh_length = (int4) cltgpslen;

	if (nsend(&outmsg)) {
		free((char *) cltgps);
		return(lam_mkerr(MPI_ERR_OTHER, errno));
	}
/*
 * Allocate storage for the server GPS array and receive it.
 */
	svrgpslen = *svrsize * sizeof(struct _gps);
	*svrgps = (struct _gps *) malloc((unsigned) svrgpslen);
	if (*svrgps == 0) {
		free((char *) cltgps);
		return(lam_mkerr(MPI_ERR_OTHER, errno));
	}

	inmsg.nh_flags |= DINT4MSG;
	inmsg.nh_msg = (char *) *svrgps;
	inmsg.nh_length = (int4) svrgpslen;

	if (nrecv(&inmsg)) {
		free((char *) cltgps);
		free((char *) *svrgps);
		return(lam_mkerr(MPI_ERR_OTHER, errno));
	}
	/*
	 * Free memory 
	 */
	if (cltgps != NULL)
	  free(cltgps);
	
	return(MPI_SUCCESS);
}

/*
 *	fill_fyi
 *
 *	Function:	- fill in process fyi
 *	Accepts:	- communicator
 */
static void
fill_fyi(MPI_Comm comm)
{
	struct _fyiproc *fyi;                   /* FYI space in KIO */

        fyi = (struct _fyiproc *) &_kio.ki_fyi[0];
        lam_getparam(&(fyi->fyp_root), &(fyi->fyp_rootgps));
        fyi->fyp_func = lam_getfunc();
        fyi->fyp_me = (lam_myproc->p_gps.gps_grank << 16)
				| comm->c_group->g_myrank;
        fyi->fyp_cidtag = (lam_mpi_pt2coll(comm->c_contextid) << 16);
}
