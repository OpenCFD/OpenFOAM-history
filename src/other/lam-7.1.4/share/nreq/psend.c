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
 * $Id: psend.c,v 1.6 2003/02/08 17:48:36 jsquyres Exp $
 *
 *	Function:	- physical layer send
 *			- send messages on virtual circuits
 *			- OTB version (UNIX, STREAM sockets)
 *
 *			- psendopen(): create VC to send messages
 *			- psend(): send message on VC
 *			- psendclose(): destroy sender's VC
 */

#include <lam_config.h>
#include <sfh.h>

#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>				/* Solaris MAXHOSTNAMELEN */
#include <unistd.h>

#include <etc_misc.h>
#include <net.h>
#include <portable.h>
#include <t_types.h>
#include <terror.h>
#include <typical.h>
#include <vctable.h>
#include <laminternal.h>
#include <lam_network.h>
#include <etc_misc.h>
#include <rreq.h>
#include <sfh.h>


/*
 * external functions
 */
extern int		_vcdelete();
extern int		_vcinsert();
extern struct vcdesc	*_vcfind();


/*
 *	psendopen
 *
 *	Function:	- create a virtual circuit for the sender
 *			- works only between different OTB nodes
 *	Accepts:	- ptr network message descriptor
 *	Returns:	- 0 or ERROR
 */
int
psendopen(struct nmsg *nhead)
{
	int		sock;			/* main socket */
	int		vcsock;			/* VC socket */
	int		port;			/* port # */
	int		flag;			/* socket option flag */
	int4		nodetype;		/* destination node type */
	char		host[MAXHOSTNAMELEN];	/* host name */
	unsigned char	hostaddr[4];		/* host address */
	struct nmsg	protocol;		/* protocol message */
	sigset_t	newset;			/* new signal set */
	sigset_t	oldset;			/* old signal set */
/*
 * The destination node must be a different OTB node.
 */
	if ((nhead->nh_node == LOCAL) || (nhead->nh_node == getnodeid())) {
		errno = EBADNODE;
		return(ERROR);
	}

	nodetype = getrtype(nhead->nh_node);
	if ((nodetype == NOTNODETYPE) || tisitb(nodetype)) {
		errno = EBADNODE;
		return(ERROR);
	}
/*
 * Get the host network address.
 */
	if (lam_gethostname(host, MAXHOSTNAMELEN)) {
		return(ERROR);
	}
	if (getinetaddr(host, hostaddr)) {
		return(ERROR);
	}
/*
 * Create a server side socket.
 */
	port = 0;
	if ((sock = sfh_sock_open_srv_inet_stm(&port, -1)) < 0) {
		return(ERROR);
	}
/*
 * Send the VC protocol message.
 * nh_data[0]: host network address in network byte order
 * nh_data[1]: host port number in Trollius byte order
 * nh_data[2]: host nodeid in Trollius byte order
 * nh_data[3]: host negative PID in Trollius byte order
 */
	protocol.nh_node = nhead->nh_node;
	protocol.nh_event = nhead->nh_event;
	protocol.nh_type = nhead->nh_type;
	protocol.nh_flags = DRAWDATA;
	protocol.nh_length = 0;
	protocol.nh_msg = 0;

	memcpy((char *) protocol.nh_data, (char *) hostaddr, 4);
	protocol.nh_data[1] = port;
	protocol.nh_data[2] = getnodeid();
	protocol.nh_data[3] = -lam_getpid();
	ltoti4(&protocol.nh_data[1], &protocol.nh_data[1]);
	ltoti4(&protocol.nh_data[2], &protocol.nh_data[2]);
	ltoti4(&protocol.nh_data[3], &protocol.nh_data[3]);

	if (nsend(&protocol)) {
		close(sock);
		return(ERROR);
	}
/*
 * Wait for the receiver to be ready then accept the connection.
 */
	protocol.nh_event = -lam_getpid();
	protocol.nh_type = 0;
	protocol.nh_flags = 0;

	if (nrecv(&protocol)) {
		close(sock);
		return(ERROR);
	}
/*
 * Send an OK-to-connect to the receiver.
 * Block the SIGUSR1 signal.
 */
	protocol.nh_node = nhead->nh_node;
	protocol.nh_event = protocol.nh_data[0];

	sigemptyset(&newset);
	sigaddset(&newset, LAM_SIGUSR);
	sigprocmask(SIG_BLOCK, &newset, &oldset);

	if (nsend(&protocol)) {
		sigprocmask(SIG_SETMASK, &oldset, (sigset_t *) 0);
		close(sock);
		return(ERROR);
	}
/*
 * Block accepting the connection. 
 */
	if ((vcsock = sfh_sock_accept_tmout(sock, -1)) < 0) {
		sigprocmask(SIG_SETMASK, &oldset, (sigset_t *) 0);
		close(sock);
		return(ERROR);
	}

	sigprocmask(SIG_SETMASK, &oldset, (sigset_t *) 0);
	close(sock);
/*
 * Don't delay sending on the socket.
 */
	flag = 1;
	if (setsockopt(vcsock,
			IPPROTO_TCP, TCP_NODELAY,
			(char *) &flag, sizeof(flag))) {
		close(vcsock);
		return(ERROR);
	}
/*
 * Insert the virtual circuit in the VC table.
 */
	if (_vcinsert(nhead->nh_node,
			nhead->nh_event, nhead->nh_type, vcsock)) {
		close(vcsock);
		return(ERROR);
	}

	return(0);
}


/*
 *	psend
 *
 *	Function:	- send a network message on a virtual circuit
 *	Accepts:	- ptr network message descriptor
 *	Returns:	- 0 or ERROR
 */
int
psend(struct nmsg *nhead)
{
	struct vcdesc	*p;			/* favourite pointer */
	char		*msg;			/* ptr to message */
	int4		msglen;			/* message length */
	int4		nbytes;			/* # of bytes sent */
/*
 * Locate the virtual circuit in the VC table.
 */
	p = _vcfind(nhead->nh_node, nhead->nh_event, nhead->nh_type);
	if (p == 0) {
		return(ERROR);
	}
/*
 * Convert the message to Trollius byte order.
 */
	msg = nhead->nh_msg;
	msglen = nhead->nh_length;

	_ltot_usrnmsg(nhead);
	_ltot_sysnmsg(nhead);
/*
 * Send the message on the virtual circuit.
 */
	nbytes = mwrite(p->vc_path, (char *) nhead, sizeof(struct nmsg));
	if (nbytes < (int)sizeof(struct nmsg)) {
		_ttol_sysnmsg(nhead);
		_ttol_usrnmsg(nhead);
		errno = (nbytes < 0) ? errno : EIO;
		return(ERROR);
	}

	if (msglen > 0) {
		nbytes = mwrite(p->vc_path, msg, msglen);
		if (nbytes < msglen) {
			_ttol_sysnmsg(nhead);
			_ttol_usrnmsg(nhead);
			errno = (nbytes < 0) ? errno : EIO;
			return(ERROR);
		}
	}
/*
 * Convert the message back to local byte order.
 */
	_ttol_sysnmsg(nhead);
	_ttol_usrnmsg(nhead);

	return(0);
}


/*
 *	psendclose
 *
 *	Function:	- close a virtual circuit for the sender
 *	Accepts:	- ptr network message descriptor
 *	Returns:	- 0 or ERROR
 */
int
psendclose(struct nmsg *nhead)
{
	struct vcdesc	*p;			/* favourite pointer */
/*
 * Locate the virtual circuit entry in the VC table.
 */
	p = _vcfind(nhead->nh_node, nhead->nh_event, nhead->nh_type);
	if (p == 0) {
		return(ERROR);
	}
/*
 * Close the VC socket and delete the table entry.
 */
	close(p->vc_path);

	return(_vcdelete(nhead->nh_node, nhead->nh_event, nhead->nh_type));
}
