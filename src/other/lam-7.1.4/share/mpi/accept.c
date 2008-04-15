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
 * $Id: accept.c,v 6.17 2003/08/22 02:33:18 brbarret Exp $
 *
 *	Function:	- accept a connection on an MPI port
 *	Accepts:	- port name
 *			- info
 *			- server root process rank
 *			- server communicator
 *			- intercomm between server and client (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <lam_config.h>
#include <sfh.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <app_mgmt.h>
#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <net.h>
#include <portable.h>
#include <lam-ssi-rpi.h>
#include <rpisys.h>
#include <terror.h>
#include <typical.h>
#include <etc_misc.h>

/*
 * private functions
 */
static int accept_on_port(char *port_name, MPI_Info info, MPI_Comm comm, 
			  int cid, int *cltcid, int *cltsize, 
			  struct _gps **cltgps);
static void fill_fyi(MPI_Comm comm);


/*@
   MPI_Comm_accept - Accept a connection from another MPI process

Input Parameters:
+ port_name - port name (only used at root)
. info - extra information handle (only used at root)
. root - rank in comm of root node
- comm - intracommunicator over which the call is collective

Output Parameter:
+ newcomm - intercommunicator with client as remote group

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

.seealso MPI_Close_port(3), MPI_Comm_connect(3), MPI_Comm_disconnect(3), MPI_Open_port(3)

.N WEB
@*/
int
MPI_Comm_accept(char *port_name, MPI_Info info, int root, MPI_Comm comm,
		MPI_Comm *newcomm)
{
	struct _proc	**p;
	struct _gps	*g;
	struct _gps	*cltgps;		/* array of client GPS */
	MPI_Group	cltgrp;			/* client group */
	int		cltsize;		/* client communicator size */
	int		cltcid;			/* client global context ID */
	int		rank;			/* my local rank */
	int		svrsize;		/* server communicator size */
	int		mycid;			/* my context ID */
	int		cid;			/* new context ID */
	int		err;			/* error code */
	int		msg[2];			/* message buffer */
	int		i;

	lam_initerr_m();
	lam_setfunc_m(BLKMPICOMMACCEPT);
/*
 * Check the arguments.
 */
	if ((comm == MPI_COMM_NULL) || LAM_IS_INTER(comm)) {
		return(lam_errfunc(comm, BLKMPICOMMACCEPT,
					lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	MPI_Comm_size(comm, &svrsize);

	if ((root >= svrsize) || (root < 0)) {
		return(lam_errfunc(comm, BLKMPICOMMACCEPT,
					lam_mkerr(MPI_ERR_ROOT, EINVAL)));
	}

	if (newcomm == 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPICOMMACCEPT,
					lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	MPI_Comm_rank(comm, &rank);

	if ((rank == root) && (strlen(port_name) > LAM_PORTLEN)) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPICOMMACCEPT,
					lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	LAM_TRACE(lam_tr_cffstart(BLKMPICOMMACCEPT));
/*
 * Set debugging parameters.
 */
	g = &(comm->c_group->g_procs[root]->p_gps);

	lam_setparam(BLKMPICOMMACCEPT, root | (g->gps_grank << 16),
				(g->gps_node << 16) | g->gps_idx);
/*
 * Synchronize all members of the server communicator and get the
 * server global context ID.
 */
	mycid = lam_getcid();

	if (mycid < 0) {
		return(lam_errfunc(comm, BLKMPICOMMACCEPT,
				lam_mkerr(MPI_ERR_INTERN, EFULL)));
	}

	err = MPI_Reduce(&mycid, &cid, 1, MPI_INT, MPI_MAX, root, comm);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPICOMMACCEPT, err));
	}

	if (rank == root) {
/*
 * Accept a connection from a client.
 */
		err = accept_on_port(port_name, info,
				     comm, cid, &cltcid, &cltsize, &cltgps);
		if (err != MPI_SUCCESS) {
			msg[0] = -1;
			msg[1] = err;
			MPI_Bcast(msg, 2, MPI_INT, root, comm);
			return(lam_errfunc(comm, BLKMPICOMMACCEPT, err));
		}
/*
 * The intercomm context ID is the maximium of the server and client
 * global context IDs.
 */
		cid = LAM_max(cid, cltcid);
		msg[0] = cid;
		msg[1] = cltsize;
	}
/*
 * Broadcast the intercomm context ID, client communicator size and
 * array of client GPS to the server communicator.
 */
	err = MPI_Bcast(msg, 2, MPI_INT, root, comm);
	if (err != MPI_SUCCESS) {
		if (rank == root) free((char *) cltgps);
		return(lam_errfunc(comm, BLKMPICOMMACCEPT, err));
	}

        if (rank != root) {
		if (msg[0] < 0) {
			return(lam_errfunc(comm, BLKMPICOMMACCEPT, msg[1]));
		}
		
		cid = msg[0];
		cltsize = msg[1];

		cltgps = (struct _gps *) malloc(cltsize * sizeof(struct _gps));
		if (cltgps == 0) {
			return(lam_errfunc(comm, BLKMPICOMMACCEPT,
					lam_mkerr(MPI_ERR_OTHER, errno)));
		}
	}

	err = MPI_Bcast(cltgps, cltsize * sizeof(struct _gps) / sizeof(int),
			MPI_INT, root, comm);
	if (err != MPI_SUCCESS) {
		free((char *) cltgps);
		return(lam_errfunc(comm, BLKMPICOMMACCEPT, err));
	}
/*
 * Create the remote (client) group.
 */
	cltgrp = (MPI_Group) malloc((unsigned) (sizeof(struct _group) +
					(cltsize * sizeof(struct _proc *))));
	if (cltgrp == 0) {
		free((char *) cltgps);
		return(lam_errfunc(comm, BLKMPICOMMACCEPT,
					lam_mkerr(MPI_ERR_OTHER, errno)));
	}
	cltgrp->g_nprocs = cltsize;
	cltgrp->g_myrank = MPI_UNDEFINED;
	cltgrp->g_refcount = 1;
	cltgrp->g_f77handle = -1;
	cltgrp->g_procs = (struct _proc **)
				((char *) cltgrp + sizeof(struct _group));

	g = cltgps;
	p = cltgrp->g_procs;

	for (i = 0; i < cltsize; ++i, ++p, ++g) {
		*p = lam_procadd(g);
		if (*p == 0) {
			free((char *) cltgps);
			free((char *) cltgrp);
			return(lam_errfunc(comm, BLKMPICOMMACCEPT,
					lam_mkerr(MPI_ERR_OTHER, errno)));
		}
		if (!((*p)->p_mode & LAM_PRPIINIT)) {
			(*p)->p_mode |= LAM_PCLIENT;
		}
		(*p)->p_refcount++;
	}
/*
 * Create the server-client intercommunicator.
 */
	*newcomm = 0;
	if (lam_comm_new(cid, comm->c_group, cltgrp, LAM_CINTER, newcomm)) {
		free((char *) cltgps);
		free((char *) cltgrp);
		return(lam_errfunc(comm, BLKMPICOMMACCEPT,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	comm->c_group->g_refcount++;
	(*newcomm)->c_errhdl = comm->c_errhdl;
	comm->c_errhdl->eh_refcount++;

	if (!al_insert(lam_comms, newcomm)) {
		return(lam_errfunc(comm, BLKMPICOMMACCEPT,
				lam_mkerr(MPI_ERR_INTERN, errno)));
	}

	if (lam_tr_comm(*newcomm)) {
		return(lam_errfunc(comm, BLKMPICOMMACCEPT,
				lam_mkerr(MPI_ERR_INTERN, errno)));
	}

	lam_setcid(cid);
/*
 * setup new processes
 */
	if (RPI_ADDPROCS(cltgrp->g_procs, cltgrp->g_nprocs)) {
		return(lam_errfunc(comm, BLKMPICOMMACCEPT,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	/* Let the collective SSI modules battle over who will do
           collectives on this new communicator.  */

        if (lam_ssi_coll_base_init_comm(*newcomm) != 0)
            return(lam_errfunc(comm, BLKMPICOMMACCEPT,
                                    lam_mkerr(MPI_ERR_INTERN, ENOSYS)));

        LAM_TRACE(lam_tr_cffend(BLKMPICOMMACCEPT, root, comm, 0, 0));
        lam_resetfunc_m(BLKMPICOMMACCEPT);

	/* Free memory */
	if (cltgps != NULL)
	  free((char*) cltgps);

	return(MPI_SUCCESS);
}

/*
 *	accept_on_port
 *
 *	Function:	- accept a connection on a port
 *			- returns client side information
 *			- storage is allocated for the array of client GPS
 *	Accepts:	- port name
 *			- info
 *			- server side communicator
 *			- server side global context ID
 *			- client side global context ID (returned)
 *			- client communicator size (returned)
 *			- array of client GPS (returned)
 *	Returns:	- MPI_SUCCESS or error code
 */
static int
accept_on_port(char *port_name, MPI_Info info, MPI_Comm comm, int cid,
	       int *cltcid, int *cltsize, struct _gps **cltgps)
{
	struct nmsg	inmsg;			/* incoming message header */
	struct nmsg	outmsg;			/* outgoing message header */
	struct _proc	**p;
	struct _gps	*g;
	struct _gps	*svrgps;		/* array of server GPS */
	struct _port	*port;			/* port */
	struct _port	search;			/* search key for port */
	int		size;			/* local group size */
	int		svrgpslen;		/* #bytes server GPS array */
	int		cltgpslen;		/* #bytes client GPS array */
	int4		mismatch;		/* run-time flags mismatch? */
	int		i;
/*
 * Find the port.
 */
	lam_strncpy(search.prt_name, port_name, LAM_PORTLEN);

	port = al_find(lam_ports, &search);
	if (port == 0) {
		return(lam_mkerr(MPI_ERR_OTHER, ENOENT));
	}
/*
 * Block waiting for connection.
 * Synchronization used is, cid = -1, src = any, tag = port number.
 */
	LAM_ZERO_ME(inmsg);
	LAM_ZERO_ME(outmsg);
	inmsg.nh_length = 0;
	inmsg.nh_flags = 0;

	_m2l_fillsys(-1, -1, lam_myproc->p_gps.gps_node,
		lam_myproc->p_gps.gps_idx, -1, port->prt_num, &inmsg);

	fill_fyi(comm);
	if (nrecv(&inmsg)) {
		return(lam_mkerr(MPI_ERR_OTHER, errno));
	}

	*cltcid = (int) inmsg.nh_data[2];
	*cltsize = (int) inmsg.nh_data[3];
	mismatch = (inmsg.nh_data[4] ^ _kio.ki_rtf)
			& (RTF_MPIGER | RTF_MPIC2C);
/*
 * Reply to the client with synchronization for the return message, local
 * context ID and communicator size.  Use the synchronization passed by
 * the client with the connect message.
 */
	MPI_Comm_size(comm, &size);

	outmsg.nh_length = 0;
	outmsg.nh_flags = 0;
        outmsg.nh_msg = NULL;

	_m2l_fillsys(lam_myproc->p_gps.gps_node, lam_myproc->p_gps.gps_idx,
		     inmsg.nh_data[0], inmsg.nh_data[1], -1, port->prt_num, 
		     &outmsg);

	outmsg.nh_data[1] = (int4) cid;
	outmsg.nh_data[2] = (int4) size;
	outmsg.nh_data[3] = (mismatch) ? MPI_ERR_FLAGS : 0;

	if (nsend(&outmsg)) {
		return(lam_mkerr(MPI_ERR_OTHER, errno));
	}

	if (mismatch) {
		return(lam_mkerr(MPI_ERR_FLAGS, EFAULT));
	}
/*
 * Allocate storage for the client GPS array and receive it.
 */
	cltgpslen = *cltsize * sizeof(struct _gps);
	*cltgps = (struct _gps *) malloc((unsigned) cltgpslen);
	if (*cltgps == 0) {
		return(lam_mkerr(MPI_ERR_OTHER, errno));
	}

	inmsg.nh_flags = DINT4MSG;

	_m2l_fillsys(inmsg.nh_data[0], inmsg.nh_data[1],
		lam_myproc->p_gps.gps_node, lam_myproc->p_gps.gps_idx,
		-1, port->prt_num, &inmsg);

	inmsg.nh_msg = (char *) *cltgps;
	inmsg.nh_length = (int4) cltgpslen;

	if (nrecv(&inmsg)) {
		free((char *) *cltgps);
		return(lam_mkerr(MPI_ERR_OTHER, errno));
	}
/*
 * Prepare information (proposed context ID, local communicator size and
 * GPS array) for passing to client.
 */
	svrgpslen = size * sizeof(struct _gps);
	svrgps = (struct _gps *) malloc((unsigned) svrgpslen);
	if (svrgps == 0) return(lam_mkerr(MPI_ERR_OTHER, errno));

	p = comm->c_group->g_procs;
	g = svrgps;
	for (i = size; i > 0; i--, p++, g++) {
		*g = (*p)->p_gps;
	}
/*
 * Send server information to the client.
 */
	outmsg.nh_flags |= DINT4MSG;
	outmsg.nh_msg = (char *) svrgps;
	outmsg.nh_length = (int4) svrgpslen;

	if (nsend(&outmsg)) {
		free((char *) svrgps);
		free((char *) *cltgps);
		return(lam_mkerr(MPI_ERR_OTHER, errno));
	}

	if (svrgps != NULL)
	  free((char *) svrgps);
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
        fyi->fyp_me = (lam_myproc->p_gps.gps_grank << 16) |
	                        comm->c_group->g_myrank;
        fyi->fyp_cidtag = (lam_mpi_pt2coll(comm->c_contextid) << 16);
}
