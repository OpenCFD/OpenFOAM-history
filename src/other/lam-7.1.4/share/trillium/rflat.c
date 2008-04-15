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
 * $Id: rflat.c,v 6.9 2003/07/05 21:56:38 jsquyres Exp $
 * 
 *	Function:	- runtime access to the flat server
 */

#include <lam_config.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <args.h>
#include <etc_misc.h>
#include <events.h>
#include <flatreq.h>
#include <ksignal.h>
#include <net.h>
#include <preq.h>
#include <rreq.h>
#include <terror.h>
#include <typical.h>
#include <sfh.h>


/*
 *	Function:	- load a buffer into a node and assign it a tag
 *	Accepts:	- server's node ID
 *			- load buffer length
 *			- allocated buffer length
 *			- flat tag
 *			- buffer ptr
 */
int
rflat(int4 nodeid, char *buffer, int4 ldlength, int4 malength, int4 tag)
{
	struct flreq	*request;	/* flatd request structure */
	struct flreply	*reply;		/* flatd reply structure */
	struct nmsg	nhead;		/* message descriptor */
	int		mask;		/* signal mask */

	if (malength == 0) return(0);

	LAM_ZERO_ME(nhead);
	request = (struct flreq *) nhead.nh_data;
	reply = (struct flreply *) nhead.nh_data;

	request->flq_src_node =
		((nodeid == LOCAL) || (getrtype(nodeid) & NT_CAST)) ?
		nodeid : getnodeid();
	request->flq_src_event = -lam_getpid();
	request->flq_req = FLQLOAD;
	request->flq_ldlength = ldlength;
	request->flq_malength = malength;
	request->flq_tag = tag;

	nhead.nh_node = nodeid;
	nhead.nh_event = EVFLATD;
	nhead.nh_type = 0;
	nhead.nh_flags = 0;
	nhead.nh_msg = 0;
	nhead.nh_length = 0;
	mask = ksigblock(sigmask(SIGUDIE) | sigmask(SIGARREST));

	if (nsend(&nhead)) {
		ksigsetmask(mask);
		return(ERROR);
	}

	nhead.nh_event = request->flq_src_event;
	
	if (nrecv(&nhead)) {
		ksigsetmask(mask);
		return(ERROR);
	}

	if (reply->flr_reply) {
		errno = reply->flr_reply;
		ksigsetmask(mask);
		return(ERROR);
	}
	else {
/*
 * Load buffer to destination node.
 */
		request->flq_src_node =
			((nodeid == LOCAL) || (getrtype(nodeid) & NT_CAST)) ?
				nodeid : getnodeid();
		request->flq_src_event = -lam_getpid();
		request->flq_req = FLQLOAD;
		request->flq_ldlength = ldlength;
		request->flq_malength = malength;
		request->flq_tag = tag;

		nhead.nh_event = EVFLATD;
		nhead.nh_msg = buffer;
		nhead.nh_type = 0;

		while (ldlength > 0) {
			nhead.nh_length = (ldlength > MAXNMSGLEN)
						? MAXNMSGLEN : ldlength;
			if (nsend(&nhead)) {
				ksigsetmask(mask);
				return(ERROR);
			}

			ldlength -= nhead.nh_length;
			nhead.nh_msg += nhead.nh_length;
		}
	}
/*
 * Receive a reply that ensures that all data has been fully loaded.
 */
	nhead.nh_event = request->flq_src_event;
	nhead.nh_length = 0;
	nhead.nh_msg = 0;

	if (nrecv(&nhead)) {
		ksigsetmask(mask);
		return(ERROR);
	}

	ksigsetmask(mask);

	if (reply->flr_reply) {
		errno = reply->flr_reply;
		return(ERROR);
	}

	return(0);
}

/*
 *	Function:	- load argument and environment vectors
 *			  into a node and assign them a tag
 *			  the argument vector must be non-null
 *	Accepts:	- server's node ID
 *			- argument vector 
 *			- process environment
 *			- size of loaded environment (out)
 *			- tag (inout)
 */
int
rflatv(int4 nodeid, char **argv, struct penv *env, int4 *envsize, int4 *tag)
{
    char		*data = 0;
    char		*args;
    char		*envs;
    int			argsize;
    int			packsize;

    if (argv == 0) {
	errno = LAM_ERR_INVAL;
	return(LAMERROR);
    }

    if (*tag == 0)
	*tag = (int4) lam_getpid();

    args = sfh_argv_glue(argv, 0xa, 0);
    if (args == 0)
	return(LAMERROR);

    argsize = strlen(args);

    if (env->pe_envv) {
	envs = sfh_argv_glue(env->pe_envv, 0xa, 0); 
	if (envs == 0) {
	    free(args);
	    return(LAMERROR);
	}

	*envsize = (int4) strlen(envs);
	packsize = argsize + *envsize + 1;

	data = realloc(args, packsize);
	if (data == 0) {
	    free(args);
	    free(envs);
	    return(LAMERROR);
	}

	memcpy(data + argsize, envs, *envsize + 1);
	free(envs);
    }
    else {
	*envsize = 0;
	data = args;
	packsize = argsize + 1;
    }

    if (rflat(nodeid, data, (int4) packsize, (int4) packsize, *tag)) {
	rflclean(nodeid, *tag);
	free(data);
	return(LAMERROR);
    }

    free(data);
    return(0);
}
