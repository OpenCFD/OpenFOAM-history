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
 * $Id: kouter.c,v 6.22 2003/05/21 19:16:15 jsquyres Exp $
 * 
 *	Function:	- simple message rendez-vous server
 */

#include <lam_config.h>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

#include <lamdebug.h>
#include <kreq.h>
#include <net.h>
#include <terror.h>
#include <typical.h>
#include <all_opt.h>
#include <laminternal.h>
#include <lamdebug.h>
#include <etc_misc.h>
#include <priority.h>

/*
 * external functions
 */
extern struct kreq	*kio_req();
extern int		kio_fd_ready();
extern int		kio_recv();
extern int		kkillrewrite();
extern int		kkillwrite();
extern void		kboot();
extern void		kio_close();
extern void		kio_init();
extern void		kio_reply();
extern void		kio_send();
extern void		kio_transfer();
extern void		kpinsert();
extern void		kpdelete();
extern void		kqdoom();
extern void		kqstate();
extern void		kqsync();

/*
 * local functions
 */
static void kqattach(struct kproc *pclient, struct kreq *pkq);
static void kqdetach(struct kreq *pkq);
static void kqsurrender(struct kproc *pclient, struct kreq *pkq);
static void kqdump();
static void kdump1(struct kproc *p);

/*
 * local variables
 */
static void		(*kq[KQMAX])() = {
				kqsync,
				kqsync,
				kqstate,
				kqdoom,
				kqsync,
				kqattach,
				kqsurrender,
			};

static char		*states[3] = {"R ", "BS", "BR"};
static struct kproc	procs[KPMAX];	/* process table */

/*
 * external variables
 */
extern struct kproc	*pblock;	/* blocked process list */

/*
 * global variables
 */
struct kproc		*pfree;		/* top of free list */
struct kproc		*pready;	/* ready process list*/
struct kproc		*prun;		/* currently running internal proc */

/*
 * global functions
 */
void knuke(struct kproc *pkp);
void kpreempt(struct kproc *ploose);
struct kproc *kpfind(int4 pid);
struct kproc *kpfindport(int4 port);

/*
 *	main
 */
int
run_kernel(int argc, char* argv[])
{
	struct kproc	*pclient;	/* current customer */
	struct kproc	*pnext;		/* next client in ready list */
	struct kreq	*pkq;		/* external kernel request */
	struct kreq	request;	/* internal kernel request */
	int		go = 1;
	int		i;
/*
 * Now setup our sockets and the like.  
 */
	kio_init();
/*
 * Initialize process table.
 */
	for (i = 0; i < KPMAX; ++i) {
		procs[i].kp_index = i;
		procs[i].kp_state = KSFREE;
		procs[i].kp_next = procs + (i + 1);
	}

	procs[KPMAX - 1].kp_next = 0;
	pfree = procs;
	pready = 0;
/*
 * Boot internal processes.
 */
	kboot(&argc, argv);

	lamlog("kernel:  initialized\n");

	while (go) {
/* 
 * Service internal processes until none are ready with requests.
 */
	    prun = pready;

	    while (prun) {
		pnext = prun->kp_next;

		if (prun->kp_flags & KIPROC) {

		    if (aq_count(prun->kp_kqueue) <= 0) {
			prun->kp_run = (void (*((*)()))())
				(*prun->kp_run)(argc, argv);
		    }

		    if (aq_count(prun->kp_kqueue) > 0) {
			memcpy((char *) &request,
				(char *) aq_find(prun->kp_kqueue),
				sizeof(struct kreq));
			aq_delete(prun->kp_kqueue);

			if (request.kq_req == KQDETACH) {
			    kqdetach(&request);
			} else {
			    (*kq[request.kq_req])(prun, &request);
			    pnext = pready;
			}
		    }
		}

		prun = pnext;
	    }
/*
 * Get an external kernel request.
 */
	    if (! (pkq = kio_req())) {
		continue;
	    }
/*
 * Service special or non-client requests.
 */
	    if (pkq->kq_req == KQDETACH) {
		kqdetach(pkq);
	    } else if ((pkq->kq_req == KQATTACH) && (pkq->kq_index == -1)) {
		kqattach((struct kproc *) 0, pkq);
	    } else if (pkq->kq_req == KQDUMP) {
		kqdump();
	    } else {
/*
 * Service external process request.
 */
		if ((pkq->kq_req < 0) || (pkq->kq_req >= KQMAX))
		    continue;
		if ((pkq->kq_index < 0) || (pkq->kq_index >= KPMAX))
		    continue;

		pclient = procs + pkq->kq_index;
		prun = pclient;
		(*kq[pkq->kq_req])(pclient, pkq);
	    }
	}

	return(0);
}

/*
 *	kqattach
 *
 *	Function:	- attaches a new client process
 *	Accepts:	- client process
 *			- client request
 */
static void
kqattach(struct kproc *pclient, struct kreq *pkq)
{
	struct kreply	reply;
	LAM_ZERO_ME(reply);

	lamlog("kouter: attached process pid=%d, pri=%d, fd=%d", 
	       pkq->kq_pid, pkq->kq_priority, kio_fd_ready());

	if (pclient != 0) {
		kpdelete(&pready, pclient, (struct kproc *) 0);
		pclient->kp_priority = pkq->kq_priority;
		kpinsert(&pready, pclient);
	} else {

		if (pfree == 0) {
			lamlog("kouter: tried to attach, but we're out of descriptors");
			reply.kr_reply = ENOKDESCRIPTORS;
			kio_reply(&reply, kio_fd_ready());
			kio_close(kio_fd_ready());
			return;
		}
/*
 * Allocate and initialize a new process descriptor.
 */
		pclient = pfree;
		kpdelete(&pfree, pfree, pfree);
		pclient->kp_priority = pkq->kq_priority;
		pclient->kp_port = (int4) kio_fd_ready();
		pclient->kp_pid = pkq->kq_pid;
		pclient->kp_flags = 0;
		pclient->kp_signal = 0;
		pclient->kp_msg.k_event = 0;
		pclient->kp_msg.k_type = 0;
		pclient->kp_state = KSREADY;
		kpinsert(&pready, pclient);

                /* see note in kkill.c about the lamhalt special
                   priority */
                if (pclient->kp_priority != PRLAMHALT) {
                    kkillwrite(pkq->kq_pid);
                }
	}

	reply.kr_reply = 0;
	reply.kr_signal = 0;
	reply.kr_index = pclient->kp_index;
	kio_reply(&reply, pclient->kp_port);
}

/*
 *	kqdetach
 *
 *	Function:	- detaches client
 *	Accepts:	- client request
 */
static void
kqdetach(struct kreq *pkq)
{
	struct kproc	*p;

	lamlog("kouter: kqdetach detached process pid=%d", pkq->kq_pid);

	p = kpfind(pkq->kq_pid);
	if (!p) return;

	if (!(p->kp_flags & KIPROC)) {
	  lamlog("kouter: kqdetach calling kio_close");
		kio_close(p->kp_port);
	}

	lamlog("kouter: kqdetach calling knuke");
	knuke(p);
}

/*
 *	kqsurrender
 *
 *	Function:	- process surrenders to the kernel
 *	Accepts:	- client process
 *			- client request
 */
static void
kqsurrender(struct kproc *pclient, struct kreq *pkq)
{
	struct kreply	reply;

	lamlog("kouter: surrendered process pid=%d", pkq->kq_pid);

	pclient->kp_flags &= ~KSURRENDER;
	reply.kr_reply = 0;
	reply.kr_signal = pclient->kp_signal;
	pclient->kp_signal = 0;
	kio_reply(&reply, pclient->kp_port);
}

/*
 *	kqdump
 *
 *	Function:	- prints process descriptors for all current clients
 */
static void
kqdump()
{
	struct kreply	reply;
	struct kproc	*p;

	printf("PID       PRI    STATE                  FLAGS\n");

	for (p = pready; p; p = p->kp_next) {
		kdump1(p);
	}

	for (p = pblock; p; p = p->kp_next) {
		kdump1(p);
	}

	reply.kr_reply = 0;
	kio_reply(&reply, kio_fd_ready());
	kio_close(kio_fd_ready());
}

/*
 *	kdump1
 *
 *	Function:	- prints one process descriptor
 *	Accepts:	- process descriptor ptr
 */
static void
kdump1(struct kproc *p)
{
	char		buffer[32];
	char		*format;

	printf("%-8d", p->kp_pid);
	printf("  %-5d", p->kp_priority);

	if ((p->kp_state == KSRECV) && (p->kp_msg.k_flags & KSYNCSQL)) {
		format = "%s (%x %x)";
	} else {
		format = "%s (%d %x)";
	}

	sprintf(buffer, format, states[p->kp_state],
			p->kp_msg.k_event, p->kp_msg.k_type); 
	printf("  %-23.23s", buffer);

	if (p->kp_state == KSRECV) {

		if (p->kp_msg.k_flags & KSYNCSQL) {
			printf("sql ");
		}

		if (p->kp_msg.k_flags & KPROBE) {
			printf("probe ");
		}
	}

	if (p->kp_flags & KSURRENDER) {
		printf("surrender ");
	}

	if (p->kp_flags & KIPROC) {
		printf("internal ");
	}

	if (p->kp_flags & KMSG2) {
		printf("msg2 ");
	}

	printf("\n");
}

/*
 *	knuke
 *
 *	Function:	- removes and cleans up one process
 *	Accepts:	- process desc.
 */
void
knuke(struct kproc *pkp)
{
	if (!pkp) return;

	if (pkp->kp_state == KSREADY) {
		kpdelete(&pready, pkp, (struct kproc *) 0);
	} else {
		kpdelete(&pblock, pkp, (struct kproc *) 0);
	}

	pkp->kp_state = KSFREE;
	kpinsert(&pfree, pkp);

	kkillrewrite(procs);
}

/*
 *	kpreempt
 *
 *	Function:	- preempts current active process
 *			- sends it a signal to prompt a kernel call
 */
void
kpreempt(struct kproc *ploose)
{
	if ( !(ploose->kp_flags & KSURRENDER)) {
		ploose->kp_flags |= KSURRENDER;

		if (kill(ploose->kp_pid, LAM_SIGUSR) < 0) {
			kpdelete(&pready, ploose, (struct kproc *) 0);
			ploose->kp_state = KSFREE;
			kpinsert(&pfree, ploose);
			kkillrewrite(procs);
		}
	}
}

/*
 *	transfer
 *
 *	Function:	- transfers message between two processes
 *	Accepts:	- source process ptr
 *			- destination process ptr
 *	Returns:	- 0 or ERROR
 */
int
transfer(struct kproc *pfrom, struct kproc *pto)
{
	char		*ptomsg;
	char		*pfrmsg;
	struct kreply	reply;

	LAM_ZERO_ME(reply);
	reply.kr_reply = 0;
	reply.kr_signal = 0;
	reply.kr_length = (pfrom->kp_msg.k_length < pto->kp_msg.k_length) ?
			pfrom->kp_msg.k_length : pto->kp_msg.k_length;
/*
 * internal transfer, trivial case
 */
	if ((pfrom->kp_flags & KIPROC) && (pto->kp_flags & KIPROC)) {

		if (pfrom->kp_msg.k_flags & KNMSG) {
			ptomsg = ((struct nmsg *) pto->kp_msg.k_msg)->nh_msg;
			pfrmsg = ((struct nmsg *) pfrom->kp_msg.k_msg)->nh_msg;

			memcpy(pto->kp_msg.k_msg, pfrom->kp_msg.k_msg,
					sizeof(struct nmsg));
			((struct nmsg *) pto->kp_msg.k_msg)->nh_msg = ptomsg;
		} else {
			ptomsg = pto->kp_msg.k_msg;
			pfrmsg = pfrom->kp_msg.k_msg;
		}

		if (reply.kr_length) {
			memcpy(ptomsg, pfrmsg, reply.kr_length);
		}
	}

	else if (pto->kp_flags & KIPROC) {
		reply.kr_blktime = pfrom->kp_blktime;
		kio_reply(&reply, pfrom->kp_port);

		if (kio_recv(&(pto->kp_msg), reply.kr_length, pfrom->kp_port))
				return(LAMERROR);
	}

	else if (pfrom->kp_flags & KIPROC) {
		reply.kr_type = pfrom->kp_msg.k_type;
		reply.kr_blktime = pto->kp_blktime;
		kio_reply(&reply, pto->kp_port);
		kio_send(&pfrom->kp_msg, reply.kr_length, pto->kp_port);
	}

	else {
		reply.kr_blktime = pfrom->kp_blktime;
		kio_reply(&reply, pfrom->kp_port);
		reply.kr_type = pfrom->kp_msg.k_type;
		reply.kr_blktime = pto->kp_blktime;
		kio_reply(&reply, pto->kp_port);
		kio_transfer(&(pfrom->kp_msg), pfrom->kp_port, pto->kp_port,
				reply.kr_length);
	}
/*
 * Free an internal sender's message.
 */
	if (pfrom->kp_flags & KIPROC) {

		if (pfrom->kp_msg.k_flags & KNMSG) {
			pfrmsg = ((struct nmsg *) pfrom->kp_msg.k_msg)->nh_msg;
			free(pfrom->kp_msg.k_msg);
		} else {
			pfrmsg = pfrom->kp_msg.k_msg;
		}

		if (pfrom->kp_msg.k_length) {
			free(pfrmsg);
		}
	}

	return(0);
}

/*
 *	kpfind
 *
 *	Function:	- finds process descriptor by process ID
 *			- searches ready, blocked & interrupt lists
 *	Accepts:	- process identifier
 *	Returns:	- process descriptor ptr
 */
struct kproc *
kpfind(int4 pid)
{
	int		i;

	for (i = 0; (i < KPMAX) && ((procs[i].kp_state == KSFREE) ||
			(procs[i].kp_pid != pid)); ++i);

	if (i < KPMAX) {
		return(&procs[i]);
	} else {
		return(0);
	}
}

/*
 *	kpfindport
 *
 *	Function:	- finds process descriptor by communication port
 *			- searches ready, blocked & interrupt lists
 *	Accepts:	- port number
 *	Returns:	- process descriptor ptr
 */
struct kproc *
kpfindport(int4 port)
{
	int		i;

	for (i = 0; (i < KPMAX) && ((procs[i].kp_state == KSFREE) ||
			(procs[i].kp_port != port)); ++i);

	if (i < KPMAX) {
		return(&procs[i]);
	} else {
		return(0);
	}
}
