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
 *	$Id: couter.c,v 1.5 2003/11/02 20:08:41 brbarret Exp $
 *
 *	Function:	- client protocols
 *			- OTB version
 */

#include <lam_config.h>

#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include <kio.h>
#include <kreq.h>
#include <terror.h>
#include <typical.h>
#include <etc_misc.h>

/*
 * local functions
 */
static void		cipc_preempt(void);

/*
 * global functions
 */
int _cipc_init(void);
int _cipc_ksend(struct kreq *pkq, struct kreply *pkr);
int _cipc_krecvfront(struct kreq *pkq);
int _cipc_ksrfront(struct kreq *pkq, struct kreply *pkr);
int _cipc_krecvback(struct kreq *pkq, struct kreply *pkr);
int _cipc_ksrback(struct kreq *pkq, struct kreply *pkr);
int _cipc_kreq(struct kreq *pkq, struct kreply *pkr);
void _cipc_cleanup(void);

/*
 * external functions
 */
extern int		_cio_init(void);	/* initialize kernel I/O */
			/* make a kernel request */
extern int		_cio_kreq(struct kreq *pkq, struct kreply *pkr);
extern int		_cio_kreqback(struct kreply *pkr); /* 1st half of kreq */
extern int		_cio_kreqfront(struct kreq *pkq);  /* 2nd half of kreq */
extern int		_cio_send(struct kmsg *pkmsg);     /* send data */
extern int		_cio_recv(struct kmsg *pkmsg);     /* recv data */
extern void		_cio_cleanup(void);	/* frees resources */
extern void		_ksig_follow();		/* check signals */
extern void		(*_lam_signal())();	/* portable signal() */

/*
 * external variables
 */
extern struct kio_t	_kio;			/* Kernel I/O */

/*
 *	_cipc_init
 *
 *	Function:	- initializes OTB client interface
 *	Returns:	- 0 or ERROR
 */
int
_cipc_init()
{
	if (_cio_init()) return(LAMERROR);
/*
 * Vector LAM_SIGUSR to preemption routine.
 */
	if (_lam_signal(LAM_SIGUSR, cipc_preempt) == SIG_ERR) return(LAMERROR);

	return(0);
}

/*
 *	_cipc_ksend
 *
 *	Function:	- OTB kernel message send
 *			- makes KQSEND request to kernel
 *	Accepts:	- kernel request ptr
 *			- kernel reply ptr
 *	Returns:	- 0 or ERROR
 */
int
_cipc_ksend(struct kreq *pkq, struct kreply *pkr)
{
	int		r;
	sigset_t	sigs_preempt;

	if (_kio.ki_pid != lam_getpid()) {
		errno = ENOTATTACHED;
		return(LAMERROR);
	}

	memcpy((char *) pkq->kq_fyi, (char *) _kio.ki_fyi,
			sizeof(_kio.ki_fyi));

	sigemptyset(&sigs_preempt);
	sigaddset(&sigs_preempt, LAM_SIGUSR);
	sigprocmask(SIG_BLOCK, &sigs_preempt, (sigset_t *) 0);

	if (_cio_kreq(pkq, pkr)) {
		sigprocmask(SIG_UNBLOCK, &sigs_preempt, (sigset_t *) 0);
		return(LAMERROR);
	}

	if (pkr->kr_reply) {
		sigprocmask(SIG_UNBLOCK, &sigs_preempt, (sigset_t *) 0);
		return(0);
	}

	pkq->kq_msg.k_length = pkr->kr_length;
	r = _cio_send(&pkq->kq_msg);
	sigprocmask(SIG_UNBLOCK, &sigs_preempt, (sigset_t *) 0);

	return(r);
}

/*
 *	_cipc_krecvfront
 *
 *	Function:	- OTB kernel message receive
 *			- makes KQRECV request to kernel
 *	Accepts:	- kernel request ptr
 *	Returns:	- client/kernel comm. handle or ERROR
 */
int
_cipc_krecvfront(struct kreq *pkq)
{
	sigset_t	sigs_preempt;
	int		r;

	if (_kio.ki_pid != lam_getpid()) {
		errno = ENOTATTACHED;
		return(LAMERROR);
	}

	memcpy((char *) pkq->kq_fyi, (char *) _kio.ki_fyi,
			sizeof(_kio.ki_fyi));

	sigemptyset(&sigs_preempt);
	sigaddset(&sigs_preempt, LAM_SIGUSR);
	sigprocmask(SIG_BLOCK, &sigs_preempt, (sigset_t *) 0);

	if ((r = _cio_kreqfront(pkq)) < 0) {
		sigprocmask(SIG_UNBLOCK, &sigs_preempt, (sigset_t *) 0);
		return(LAMERROR);
	}

	return(r);
}

/*
 *	_cipc_krecvback
 *
 *	Function:	- OTB kernel message receive
 *			- receives reply for KQRECV from kernel
 *			- transfers message
 *	Accepts:	- kernel request ptr
 *			- kernel reply ptr
 *	Returns:	- 0 or ERROR
 */
int
_cipc_krecvback(struct kreq *pkq, struct kreply *pkr)
{
	int		r;
	sigset_t	sigs_preempt;

	sigemptyset(&sigs_preempt);
	sigaddset(&sigs_preempt, LAM_SIGUSR);

	if (_cio_kreqback(pkr)) {
		sigprocmask(SIG_UNBLOCK, &sigs_preempt, (sigset_t *) 0);
		return(LAMERROR);
	}

	if (pkr->kr_reply) {
		sigprocmask(SIG_UNBLOCK, &sigs_preempt, (sigset_t *) 0);
		return(0);
	}

	pkq->kq_msg.k_length = pkr->kr_length;
	r = _cio_recv(&pkq->kq_msg);
	sigprocmask(SIG_UNBLOCK, &sigs_preempt, (sigset_t *) 0);

	return(r);
}

/*
 *	_cipc_ksrfront
 *
 *	Function:	- OTB kernel message send/receive front end
 *			- makes KQSEND followed by KQRECV request to kernel
 *	Accepts:	- kernel request ptr
 *			- kernel reply ptr
 *	Returns:	- 0 or client/kernel socket
 */
int
_cipc_ksrfront(struct kreq *pkq, struct kreply *pkr)
{
	sigset_t	sigs_preempt;
	int		r;

	if (_kio.ki_pid != lam_getpid()) {
		errno = ENOTATTACHED;
		return(LAMERROR);
	}

	memcpy((char *) pkq->kq_fyi, (char *) _kio.ki_fyi,
			sizeof(_kio.ki_fyi));

	sigemptyset(&sigs_preempt);
	sigaddset(&sigs_preempt, LAM_SIGUSR);
	sigprocmask(SIG_BLOCK, &sigs_preempt, (sigset_t *) 0);

	if ((r = _cio_kreqfront(pkq)) < 0) {
		sigprocmask(SIG_UNBLOCK, &sigs_preempt, (sigset_t *) 0);
		return(LAMERROR);
	}

	if (_cio_kreqback(pkr) < 0) {
		sigprocmask(SIG_UNBLOCK, &sigs_preempt, (sigset_t *) 0);
		return(LAMERROR);
	}

	if (pkr->kr_reply) {
		sigprocmask(SIG_UNBLOCK, &sigs_preempt, (sigset_t *) 0);
		return(0);
	}

	pkq->kq_msg.k_length = pkr->kr_length;

	if (_cio_send(&pkq->kq_msg)) {
		sigprocmask(SIG_UNBLOCK, &sigs_preempt, (sigset_t *) 0);
		return(LAMERROR);
	}

	return(r);
}

/*
 *	_cipc_ksrback
 *
 *	Function:	- OTB kernel message send/receive back end
 */
int
_cipc_ksrback(struct kreq *pkq, struct kreply *pkr)
{
	int		r;
	sigset_t	sigs_preempt;

	sigemptyset(&sigs_preempt);
	sigaddset(&sigs_preempt, LAM_SIGUSR);

	if (_cio_kreqback(pkr)) {
		sigprocmask(SIG_UNBLOCK, &sigs_preempt, (sigset_t *) 0);
		return(LAMERROR);
	}

	if (pkr->kr_reply) {
		sigprocmask(SIG_UNBLOCK, &sigs_preempt, (sigset_t *) 0);
		return(0);
	}

	pkq->kq_msg2.k_length = pkr->kr_length;

	r = _cio_recv(&pkq->kq_msg2);
	sigprocmask(SIG_UNBLOCK, &sigs_preempt, (sigset_t *) 0);

	return(r);
}

/*
 *	_cipc_kreq
 *
 *	Function:	- OTB kernel request
 *			- makes request to kernel
 *	Accepts:	- kernel request ptr
 *			- kernel reply ptr
 *	Returns:	- 0 or ERROR
 */
int
_cipc_kreq(struct kreq *pkq, struct kreply *pkr)
{
	sigset_t	sigs_preempt;

	if (_kio.ki_pid != lam_getpid()) {
		errno = ENOTATTACHED;
		return(LAMERROR);
	}

	memcpy((char *) pkq->kq_fyi, (char *) _kio.ki_fyi,
			sizeof(_kio.ki_fyi));

	do {
		sigemptyset(&sigs_preempt);
		sigaddset(&sigs_preempt, LAM_SIGUSR);
		sigprocmask(SIG_BLOCK, &sigs_preempt, (sigset_t *) 0);

		if (_cio_kreq(pkq, pkr)) {
			sigprocmask(SIG_UNBLOCK, &sigs_preempt, 
					(sigset_t *) 0);
			return(LAMERROR);
		}

		sigprocmask(SIG_UNBLOCK, &sigs_preempt, (sigset_t *) 0);
/*
 * Do we need to follow a signal?
 */
		if (pkr->kr_signal) {
			_kio.ki_signal |= pkr->kr_signal;
			_ksig_follow();
		}
	} while (pkr->kr_reply == EINTR);

	return(0);
}

/*
 *	_cipc_cleanup
 *
 *	Function:	- frees up client resources
 */
void
_cipc_cleanup()
{
	_cio_cleanup();
}

/*
 *	cipc_preempt
 *
 *	Function:	- preempts myself
 *			- signal handler for LAM_SIGUSR
 */
static void
cipc_preempt()
{
	struct kreq	req;			/* kernel request */
	struct kreply	reply;			/* kernel reply */

	if (_kio.ki_pid != lam_getpid()) return;

	LAM_ZERO_ME(req);
	req.kq_req = KQSURRENDER;
	req.kq_index = _kio.ki_index;

	if (_cio_kreq(&req, &reply)) return;

	if (reply.kr_signal) {
		_kio.ki_signal |= reply.kr_signal;
		_ksig_follow();
	}
/*
 * Vector LAM_SIGUSR to preemption routine.
 */
	if (_lam_signal(LAM_SIGUSR, cipc_preempt) == SIG_ERR) return;
}
