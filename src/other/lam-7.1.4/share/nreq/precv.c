/*
 * Copyright (c) 2001-2006 The Trustees of Indiana University.  
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
 *	Function:	- physical layer receive
 *			- receive messages on virtual circuits
 *			- OTB version (UNIX, STREAM sockets)
 *
 *			- precvopen(): create VC to receive messages
 *			- precv(): receive message on VC
 *			- precvclose(): destroy receiver's VC
 */

#include <lam_config.h>
#include <sfh.h>

#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <etc_misc.h>
#include <portable.h>
#include <net.h>
#include <terror.h>
#include <typical.h>
#include <t_types.h>
#include <vctable.h>
#include <laminternal.h>

#ifndef TO_PRECVO_CONN
#define TO_PRECVO_CONN	100
#endif

/*
 * external functions
 */
extern void		_ltot_usrnmsg();
extern void		_ltot_sysnmsg();
extern void		_ttol_usrnmsg();
extern void		_ttol_sysnmsg();
extern int		sfh_sock_open_clt_inet_stm();
extern int		_vcdelete();
extern int		_vcinsert();
extern struct vcdesc	*_vcfind();

/*
 *	precvopen
 *
 *	Function:	- create a virtual circuit for the receiver
 *			- works only between OTB nodes
 *	Accepts:	- ptr network message descriptor
 *	Returns:	- 0 or ERROR
 */
int
precvopen(nhead)

struct nmsg		*nhead;

{
	int		vcsock = -1;	/* VC socket */
	int		sndport;	/* sender port # */
	int		i;		/* favourite counter */
	unsigned char	sndaddr[4];	/* sender address */
	struct nmsg	protocol;	/* protocol message */
/*
 * Receive the virtual circuit protocol message.
 */
	protocol.nh_event = nhead->nh_event;
	protocol.nh_type = nhead->nh_type;
	protocol.nh_flags = 0;
	protocol.nh_length = 0;
	protocol.nh_msg = 0;

	if (nrecv(&protocol)) {
		return(ERROR);
	}
/*
 * Modify the receiver's nh_type field to match the synchronization.
 */
	nhead->nh_type = protocol.nh_type;
/*
 * Let the sender know the receiver is ready.
 */
	memcpy((char *) sndaddr, (char *) protocol.nh_data, 4);
	ttoli4(&protocol.nh_data[1], &sndport);

	ttoli4(&protocol.nh_data[2], &protocol.nh_node);
	ttoli4(&protocol.nh_data[3], &protocol.nh_event);
	protocol.nh_type = 0;

	protocol.nh_data[0] = -lam_getpid();

	if (nsend(&protocol)) {
		return(ERROR);
	}
/*
 * Receive the OK-to-connect message from the sender.
 */
	protocol.nh_event = -lam_getpid();

	if (nrecv(&protocol)) {
		return(ERROR);
	}
/*
 * Connect the virtual circuit.
 * Keep trying till success or timeout.
 */
	for (i = 0; i < TO_PRECVO_CONN; ++i) {
		if ((vcsock =
			sfh_sock_open_clt_inet_stm(sndaddr, sndport)) >= 0) {

			break;
		}
	}

	if (vcsock < 0) {
		return(ERROR);
	}
/*
 * Insert the virtual circuit in the VC table.
 */
	if (_vcinsert(LOCAL, nhead->nh_event, nhead->nh_type, vcsock)) {
		close(vcsock);
		return(ERROR);
	}

	return(0);
}

/*
 *	precv
 *
 *	Function:	- receive a network message on a virtual circuit
 *	Accepts:	- ptr network message descriptor
 *	Returns:	- 0 or ERROR
 */
int
precv(nhead)

struct nmsg		*nhead;

{
	struct nmsg	incoming;		/* incoming header */
	struct vcdesc	*p;			/* favourite pointer */
	int4		extra_len;		/* extra msg length */
	int4		nbytes;			/* # bytes read */
	int4		save_flags;		/* saved nh_flags */
	char		bitbucket;		/* to trash extra msg */
/*
 * Locate the virtual circuit in the VC table.
 */
	p = _vcfind(LOCAL, nhead->nh_event, nhead->nh_type);
	if (p == 0) {
		return(ERROR);
	}
/*
 * Receive the incoming message header.
 */
	nbytes = mread(p->vc_path, (char *) &incoming, sizeof(struct nmsg));
	if (nbytes < (int)sizeof(struct nmsg)) {
		errno = (nbytes < 0) ? errno : EIO;
		return(ERROR);
	}
/*
 * Convert the system part of the header to local byte order.
 */
	_ttol_sysnmsg(&incoming);
/*
 * Set the message length.
 */
	if (nhead->nh_length > incoming.nh_length) {
		nhead->nh_length = incoming.nh_length;
	}
/*
 * Copy the received data pouch as is (i.e. in Trollius byte order).
 */
	memcpy((char *) nhead->nh_data,
		(char *) incoming.nh_data, NHDSIZE * sizeof(int4));
/*
 * Receive the user's message.
 */
	if (nhead->nh_length > 0) {
		nbytes = mread(p->vc_path, nhead->nh_msg, nhead->nh_length);
		if (nbytes < nhead->nh_length) {
			errno = (nbytes < 0) ? errno : EIO;
			return(ERROR);
		}
	}
/*
 * Use the incoming nh_flags to convert the message to local byte order.
 * Restore the original nh_flags as given by the caller.
 */
	save_flags = nhead->nh_flags;
	nhead->nh_flags = incoming.nh_flags;
	_ttol_usrnmsg(nhead);
	nhead->nh_flags = save_flags;
/*
 * Receive and trash any unused portion of the message sent.
 */
	extra_len = incoming.nh_length - nhead->nh_length;

	for ( ; extra_len > 0; --extra_len) {
		nbytes = mread(p->vc_path, &bitbucket, (int4) 1);
		if (nbytes < 1) {
			errno = (nbytes < 0) ? errno : EIO;
			return(ERROR);
		}
	}

	return(0);
}

/*
 *	precvclose
 *
 *	Function:	- close a virtual circuit for the receiver
 *	Accepts:	- ptr network message descriptor
 *	Returns:	- 0 or ERROR
 */
int
precvclose(nhead)

struct nmsg		*nhead;

{
	struct vcdesc	*p;			/* favourite pointer */
/*
 * Locate the virtual circuit entry in the VC table.
 */
	p = _vcfind(LOCAL, nhead->nh_event, nhead->nh_type);
	if (p == 0) {
		return(ERROR);
	}
/*
 * Close the VC socket and delete the table entry.
 */
	close(p->vc_path);

	return(_vcdelete(LOCAL, nhead->nh_event, nhead->nh_type));
}
