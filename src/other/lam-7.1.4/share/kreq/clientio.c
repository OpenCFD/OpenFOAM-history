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
 *	Ohio Trollius
 *	Copyright 1997 The Ohio State University
 *	GDB/RBD
 *
 *	$Id: clientio.c,v 1.8 2003/11/02 20:08:40 brbarret Exp $
 *
 *	Function:	- interface between kernel runtime request library
 *			  and native environment
 */

#include <lam_config.h>
#include <sfh.h>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>

#include <freq.h>
#include <kio.h>
#include <kreq.h>
#include <laminternal.h>
#include <terror.h>
#include <typical.h>

/*
 * global functions
 */
void			_cio_bail();		/* terminate self */
void			_cio_cleanup();		/* mach. dep. cleanup */
int			_cio_init(void);	/* initialize kernel I/O */
			/* make a kernel request */
int			_cio_kreq(struct kreq *pkq, struct kreply *pkr);
int			_cio_kreqback(struct kreply *pkr); /* 1st half of kreq */
int			_cio_kreqfront(struct kreq *pkq);  /* 2nd half of kreq */
int			_cio_send(struct kmsg *pkmsg);     /* send data */
int			_cio_recv(struct kmsg *pkmsg);     /* recv data */

/*
 * external variables
 */
struct kio_t		_kio;			/* Kernel I/O block */
struct fclient		_ufd[FUMAX];		/* user file desc. */

/*
 * local variables
 */
static int		sd_kernel;		/* kernel socket */

/*
 *	_cio_init
 *
 *	Function:	- initializes kernel interface
 *			- establishes UDP INET domain socket to communicate
 *			  with the kernel and other Trollius processes
 *	Returns:	- 0 or ERROR
 */
int
_cio_init()
{
	char		*p;
	int		errnosave;

	(void) _lam_signal(SIGPIPE, 0);

	if (lam_tmpdir_init(NULL, NULL) != 0)
	  return LAMERROR;
	
	if ((p = lam_get_sockname()) == NULL)
	  return(LAMERROR);

	sd_kernel = sfh_sock_open_clt_unix_stm(p);
	errnosave = errno;
	free(p);

	if (sd_kernel < 0) {

	  /* According to new semantics of
             sfh_sock_open_clt_unix_stm(), ECONNREFUSED and ETIMEDOUT
             will also return a failure.  See lengthy comment in
             share/etc/socket.c.  If this is the case, be sure to
             remove the extra directory that we created to put the
             socket in.  */

	    if ((errnosave == ENOENT) || (errnosave == ENXIO) || 
		(errnosave == ECONNREFUSED) || (errnosave == ETIMEDOUT) ) {
	      lam_tmpdir_remove();
	      errno = ENOKERNEL;
	    }

	    return(LAMERROR);
	}

	if (sfh_sock_set_buf_size(sd_kernel, SFH_UNIX, SO_SNDBUF, 
	    		KPKTLEN + sizeof(struct nmsg))) {
		return(LAMERROR);
	}

	if (sfh_sock_set_buf_size(sd_kernel, SFH_UNIX, SO_RCVBUF, 
	    		KPKTLEN + sizeof(struct nmsg))) {
		return(LAMERROR);
	}
/*
 * Set default stdio node and descriptors.
 */
	_kio.ki_ionode = NOTNODEID;
	_kio.ki_stdin = 0;
	_kio.ki_stdout = 1;
	_kio.ki_stderr = 2;

	if ((p = getenv("TROLLIUSFD"))) {
		_kio.ki_ionode = stoi((unsigned char *) p);
		p = strchr(p, ':') + 1;
		_kio.ki_stdin = stoi((unsigned char *) p);
		p = strchr(p, ':') + 1;
		_kio.ki_stdout = stoi((unsigned char *) p);
		p = strchr(p, ':') + 1;
		_kio.ki_stderr = stoi((unsigned char *) p);
	}

	_kio.ki_pid = -1;
/*
 * Get any inherited runtime flags.
 */
	_kio.ki_rtf = (p = getenv("TROLLIUSRTF")) 
			? stoi((unsigned char *) p) : 0;
/*
 * Have we been forked?
 */
	if ((p = getenv("LAMKENYAPID"))) {
		if (getppid() != stoi((unsigned char *) p)) {
			_kio.ki_rtf &= ~(RTF_WAIT | RTF_FLAT);
			_kio.ki_rtf |= RTF_FORKED;
		}
	}
/*
 * Some APIs want to know the total world population, a-priori.
 */
	_kio.ki_world = (p = getenv("LAMWORLD")) ? stoi((unsigned char *) p) : 0;

	_kio.ki_parent = (p = getenv("LAMPARENT")) 
				? stoi((unsigned char *) p) : 0;
/*
 * Set the LAM job identifier.
 */
	if ((p = getenv("LAMJOBID"))) {
		_kio.ki_jobid.jid_node = stoi((unsigned char *) p);
		p = strchr(p, ':') + 1;
		_kio.ki_jobid.jid_pid = stoi((unsigned char *) p);
	}

	return(0);
}

/*
 *	_cio_kreq
 *
 *	Function:	- makes a kernel request
 *	Accepts:	- kernel request ptr
 *			- kernel reply ptr
 *	Returns:	- 0 or ERROR
 */
int
_cio_kreq(struct kreq *pkq, struct kreply *pkr)
{
	if (_cio_kreqfront(pkq) < 0) {
		return(ERROR);
	}

	return(_cio_kreqback(pkr));
}

/*
 *	_cio_kreqfront
 *
 *	Function:	- front half of a kernel request
 *			- sends the request structure to the kernel
 *			- verifies kernel connection
 *	Accepts:	- kernel request ptr
 *	Returns:	- client/kernel socket or ERROR
 */
int
_cio_kreqfront(struct kreq *pkq)
{
/*
 * Send the request to the kernel.
 */
	if (mwrite(sd_kernel, (char *) pkq, sizeof(struct kreq)) <
			(int)sizeof(struct kreq)) {

		if (errno == EPIPE) {

			if (pkq->kq_req != KQATTACH) {
				exit(ENOKERNEL);
			} else {
				errno = ENOKERNEL;
			}
		}

		return(LAMERROR);
	}

	return(sd_kernel);
}

/*
 *	_cio_kreqback
 *
 *	Function:	- back half of a kernel request
 *			- receives the reply structure from the kernel
 *	Accepts:	- kernel reply ptr
 *	Returns:	- 0 or ERROR
 */
int
_cio_kreqback(struct kreply *pkr)
{
	if (mread(sd_kernel, (char *) pkr, sizeof(struct kreply)) <
			(int)sizeof(struct kreply)) {

		if (errno == EEOF) exit(ENOKERNEL);

		return(LAMERROR);
	}

	return(0);
}

/*
 *	_cio_recv
 *
 *	Function:	- receives a message from another process
 *			  through the kernel
 *	Accepts:	- kernel msg desc.
 *	Returns:	- 0 or ERROR
 */
int
_cio_recv(struct kmsg *pkmsg)
{
	int4		r;
	int4		totlen;
	struct iovec	iov[2];
	char		*sv_msg;

	if (pkmsg->k_flags & KPROBE) {
		return(0);
	}

	totlen = pkmsg->k_length;

	if (pkmsg->k_flags & KNMSG) {
		sv_msg = ((struct nmsg *) (pkmsg->k_msg))->nh_msg;
		iov[0].iov_base = pkmsg->k_msg;
		iov[0].iov_len = sizeof(struct nmsg);
		iov[1].iov_base = sv_msg;
		iov[1].iov_len = pkmsg->k_length;
		totlen += sizeof(struct nmsg);

		r = mreadv(sd_kernel, iov, 2);
		((struct nmsg *) (pkmsg->k_msg))->nh_msg = sv_msg;
	} else {
		r = mread(sd_kernel, pkmsg->k_msg, pkmsg->k_length);
	}

	if (r < totlen) {

		if (errno == EEOF) {
			errno = ENOKERNEL;
		}

		return(LAMERROR);
	}

	return(0);
}

/*
 *	_cio_send
 *
 *	Function:	- sends a message to another process
 *			  through the kernel
 *	Accepts:	- kernel msg desc.
 *	Returns:	- 0 or ERROR
 */
int
_cio_send(struct kmsg *pkmsg)
{
	int4		r;
	int4		totlen;
	struct iovec	iov[2];

	totlen = pkmsg->k_length;

	if (pkmsg->k_flags & KNMSG) {
		iov[0].iov_base = pkmsg->k_msg;
		iov[0].iov_len = sizeof(struct nmsg);
		iov[1].iov_base = ((struct nmsg *) (pkmsg->k_msg))->nh_msg;
		iov[1].iov_len = pkmsg->k_length;
		totlen += sizeof(struct nmsg);

		r = mwritev(sd_kernel, iov, 2);
	} else {
		r = mwrite(sd_kernel, pkmsg->k_msg, pkmsg->k_length);
	}

	if (r < totlen) {

		if (errno == EPIPE) {
			errno = ENOKERNEL;
		}

		return(LAMERROR);
	}

	return(0);
}

/*
 *	_cio_cleanup
 *
 *	Function:	- closes communication sockets
 */
void
_cio_cleanup()
{
	close(sd_kernel);
}

/*
 *	_cio_bail
 *
 *	Function:	- terminates the process
 *	Accepts:	- exit status
 */
void
_cio_bail(int status)
{
	exit(status);
}
