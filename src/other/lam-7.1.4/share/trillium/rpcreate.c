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
 * $Id: rpcreate.c,v 6.13 2003/05/31 22:28:52 jsquyres Exp $
 */

#include <lam_config.h>
#include <sfh.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <args.h>
#include <etc_misc.h>
#include <events.h>
#include <flatreq.h>
#include <kio.h>
#include <ksignal.h>
#include <laminternal.h>
#include <net.h>
#include <portable.h>
#include <preq.h>
#include <rreq.h>
#include <terror.h>
#include <typical.h>
#include <t_types.h>

/*
 * private functions
 */
static int		send_stdio_fds();

/*
 * external variables
 */
extern struct kio_t	_kio;

/*
 * 	Function:	- creates a process on a given node
 * 			- executable already loaded with flatd
 * 	Accepts:	- target node ID
 *			- program file name
 * 			- process runtime flags
 * 			- argument vector
 *			- process environment
 *			- process ID (out)
 *			- process index (out)
 * 	Returns:	- 0 or ERROR
 */
int
rpcreatev(int4 nodeid, CONST char *filename, int4 rtf, char **argv,
    	    struct penv *env, int4 *pid, int4 *idx)
{
	struct nmsg	nhead;			/* network header */
	struct preq	request;		/* kenya request */
	struct preply	reply;			/* kenya reply */
	int4		mask;
	int4		envsize;		/* size of loaded environment */
	int4		tag;			/* program/argv flat tag */
	int		err;			/* return code */
	int		sd = -1;		/* fd server file descriptor */
	char		server[LAM_PATH_MAX];	/* fd server socket name */
/*
 * Check trivial cases.
 */
	if (filename == 0) {
		errno = EINVAL;
		return(LAMERROR);
	}

	if (strlen(filename) >= PSMAXNAME) {
		errno = ENAMETOOLONG;
		return(LAMERROR);
	}
/*
 * Pack and tag the arguments and environemnt.
 */
	tag = 0;
	if (rflatv(nodeid, argv, env, &envsize, &tag)) {
		return(LAMERROR);
	}
/*
 * Send the `create' request.
 */
	LAM_ZERO_ME(nhead);
	LAM_ZERO_ME(request);
	request.pq_req = ltot(PQCREATE);
	request.pq_rtf = ltot(rtf);
	request.pq_argv = ltot(tag);
	request.pq_envsize = ltot(envsize);
	request.pq_rank = ltot(env->pe_rank);
	request.pq_jobid.jid_node = ltot(_kio.ki_jobid.jid_node);
	request.pq_jobid.jid_pid = ltot(_kio.ki_jobid.jid_pid);
	if ((rtf & RTF_IO) && (nodeid == LOCAL || nodeid == getnodeid())) {
	    request.pq_stdin = ltot(_kio.ki_stdin);
	} else {
	    request.pq_stdin = ltot(0);
	}
	request.pq_stdout = ltot(_kio.ki_stdout);
	request.pq_stderr = ltot(_kio.ki_stderr);
	request.pq_world = ltot(_kio.ki_world);
	request.pq_parent = ltot(_kio.ki_parent);
	strcpy(request.pq_name, filename);
	if (!LAM_isNullStr(env->pe_wrkdir)) {
	    lam_strncpy(request.pq_wrkdir, env->pe_wrkdir, LAM_PATH_MAX);
	} else {
	    strcpy(request.pq_wrkdir, "");
	}

	request.pq_src_node = ((nodeid == LOCAL) ||
			(tiscast(getrtype(nodeid)))) ?
			ltot(nodeid) : ltot(getnodeid());
	request.pq_src_event = ltot(-((int4) lam_getpid()));
/*
 * If passing file descriptors open the server.
 */
	if (rtf & RTF_PFDIO) {
		if (lam_mktmpid( (int) lam_getpid(), server, sizeof(server))) {
			return(LAMERROR);
		}

		if ((sd = sfh_sock_open_srv_unix_stm(server)) < 0) {
			return(LAMERROR);
		}
	}

	nhead.nh_node = nodeid;
	nhead.nh_event = EVKENYAD;
	nhead.nh_flags = 0;
	nhead.nh_type = 0;
	nhead.nh_length = sizeof(request);
	nhead.nh_msg = (char *) &request;
	mask = ksigblock(sigmask(SIGUDIE) | sigmask(SIGARREST));

	if (nsend(&nhead)) {
		if (rtf & RTF_PFDIO) {
			close(sd);
			unlink(server);
		}
		ksigsetmask(mask);
		return(LAMERROR);
	}
/*
 * Pass the stdio file descriptors to kenyad.
 */
	if (rtf & RTF_PFDIO) {
		err = send_stdio_fds(sd);
		close(sd);
		unlink(server);
		if (err) {
			ksigsetmask(mask);
			return(LAMERROR);
		}
	}

	nhead.nh_event = -lam_getpid();
	nhead.nh_length = sizeof(reply);
	nhead.nh_msg = (char *) &reply;

	if (nrecv(&nhead)) {
		ksigsetmask(mask);
		return(LAMERROR);
	}

	ksigsetmask(mask);

	if (reply.pr_reply) {
		errno = ttol(reply.pr_reply);
		return(LAMERROR);
	} else {
		*pid = ttol(reply.pr_pid);
		*idx = ttol(reply.pr_index);
		return(0);
	}
}

/*
 * 	Function:	- creates a process on a given node
 * 			- executable already loaded with flatd
 * 	Accepts:	- target node ID
 *			- program file name
 * 			- process runtime flags
 * 			- argument vector
 *			- process ID (out)
 *			- process index (out)
 * 	Returns:	- 0 or ERROR
 */
int
rpcreate(int4 nodeid, CONST char *filename, int4 rtf, char **argv,
    	    int4 *pid, int4 *idx)
{
    struct penv		env;

    env.pe_rank = -1;
    env.pe_envv = 0;
    strcpy(env.pe_wrkdir, "");

    return(rpcreatev(nodeid, filename, rtf, argv, &env, pid, idx));
}

/*
 *	send_stdio_fds
 *
 *	Function:	- accept connection and send stdio file
 *			  descriptors to client
 *	Accepts:	- server file descriptor
 *	Returns:	- 0 or LAMERROR
 */
static int
send_stdio_fds(sd)

int			sd;

{
	int		pass_fd;		/* stream to pass fds over */
	
	if ((pass_fd = sfh_sock_accept_tmout(sd, -1)) < 0) {
		return(LAMERROR);
	}

	if (sfh_send_fd(pass_fd, 0)
		|| sfh_send_fd(pass_fd, 1) || sfh_send_fd(pass_fd, 2)) {

		close(pass_fd);
		return(LAMERROR);
	}

	close(pass_fd);
	return(0);
}
