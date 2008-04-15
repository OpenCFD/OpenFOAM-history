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
 * $Id: kreq.h,v 6.14 2004/02/04 03:06:21 jsquyres Exp $
 *
 *	Function:	- kernel request constants and templates
 */

#ifndef _KREQ
#define _KREQ

#include <lam_config.h>
#include <all_queue.h>
#include <portable.h>

#define KTYPEHI		0xffff0000
#define KTYPELO		0x0000ffff
#define KEVENTHI	0x7fff0000

#define ksync_type(a, b) ((a == 0) || (b == 0) || (a & b))

#define ksync_type_sql(a, b) ((((a & KTYPEHI) == KTYPEHI) || \
		((b & KTYPEHI) == KTYPEHI) || \
		((a & KTYPEHI) == (b & KTYPEHI))) && \
		(((a & KTYPELO) == KTYPELO) || \
		((b & KTYPELO) == KTYPELO) || \
		((a & KTYPELO) == (b & KTYPELO))))

#define ksync_event(a, b) (a == b)

#define ksync_event_sql(a, b) (((a & ~KEVENTHI) == (b & ~KEVENTHI)) && \
		(((a & KEVENTHI) == KEVENTHI) || \
		((b & KEVENTHI) == KEVENTHI) || \
		((a & KEVENTHI) == (b & KEVENTHI))))
/*
 * KSYNCSQL is triggered by the blocked process.  KSYNCSQL will only
 * be used on the second half of a KQSR and only by external processes.
 */
#define ksync_cmp(p, q) ((p->kp_msg.k_flags & KSYNCSQL) ? \
		(ksync_event_sql(p->kp_msg.k_event, (q).k_event) && \
		ksync_type_sql(p->kp_msg.k_type, (q).k_type)) : \
		(ksync_event(p->kp_msg.k_event, (q).k_event) && \
		ksync_type(p->kp_msg.k_type, (q).k_type)))

/*
 * kernel message descriptor
 */
struct kmsg {
	int4		k_event;	/* message event (==) */
	int4		k_type;		/* message type (& or SQL) */
	int4		k_length;	/* message length */
	int4		k_flags;	/* option flags, jives with nh_flags */
#define	KSYNCSQL	0x01		/* use SQL rules for sync */
#define	KTRY		0x02		/* error if block */
#define	KNMSG		0x04		/* k_msg is struct nmsg */
#define	KPROBE		0x08		/* KQRECV is probe only */
#define	KTRACE		0x20		/* time kernel operations */
	char		*k_msg;		/* message body */
};

/*
 * kernel request codes
 */
#define KQSEND		0		/* send a message to a process */
#define KQRECV		1		/* receive a message from a process */
#define KQSTATE		2		/* get the state of a process */
#define KQDOOM		3		/* signal a process */
#define KQSR		4		/* send then recv */

#define KQATTACH	5		/* attach process */
#define KQSURRENDER	6		/* create a process */
#define KQMAX		7		/* # kernel client requests */

/*
 * non-client kernel requests
 */
#define KQDETACH	7		/* end kernel session */
#define KQDUMP		8		/* print process descriptors */

/*
 * process states
 */
#define KSREADY		0		/* ready to run */
#define KSSEND		1		/* send blocked */
#define KSRECV		2		/* receive blocked */
#define KSFREE		3		/* not in use */

/*
 * process flags
 */
#define KSURRENDER	2		/* process should surrender */
#define KIPROC		0x10		/* internal process */
#define KMSG2		0x20		/* send blocked process is KQSR */

/*
 * limits
 */
/* KPMAX is PMAX + 32, but we didn't want to create depenencies on preq.h */
#define KPMAX		(256+32)	/* max # kernel processes */
#define KLENQ		8		/* initial kreq queue length */
#define KNFYI		9		/* debugging info size */
#define KPKTLEN		(8192 + sizeof(struct nmsg))
					/* message transfer packet length */

/*
 * kernel request
 */
struct kreq {
	int4		kq_req;
	int4		kq_index;
	int4		kq_pid;
	int4		kq_priority;
	int4		kq_signal;
	int4		kq_fyi[KNFYI];
	struct kmsg	kq_msg;
	struct kmsg	kq_msg2;	/* 2nd half of KQSR */
};

/*
 * kernel reply
 */
struct kreply {
	int4		kr_reply;
	int4		kr_signal;
	int4		kr_length;
	int4		kr_event;
	int4		kr_type;
	int4		kr_state;
	int4		kr_priority;
	int4		kr_index;
	int4		kr_sigs;	/* for KQSTATE */
	int4		kr_fyi[KNFYI];
	double		kr_blktime;	/* time spent blocking */
};

/*
 * kernel process status
 */
struct kstatus {
	int4		ks_state;
	int4		ks_priority;
	int4		ks_signal;
	int4		ks_event;
	int4		ks_type;
	int4		ks_length;
	int4		ks_fyi[KNFYI];
};

/*
 * kernel process descriptor
 */
struct kproc {
	struct kproc	*kp_next;	/* linked list */
	int4		kp_pid;		/* process identifier */
	int4		kp_index;	/* desc. index */
	int4		kp_priority;	/* process priority */
	int4		kp_port;	/* communication port */
	int4		kp_state;	/* process state */
	int4		kp_signal;	/* pending signals */
	int4		kp_flags;	/* misc. information */
	int4		kp_fyi[KNFYI];	/* misc. debugging info */
	struct kmsg	kp_msg;		/* blocked message */
	struct kmsg	kp_msg2;	/* 2nd half of KQSR */
	QUEUE		*kp_kqueue;	/* internal request queue */
	void		(*((*kp_run)(int, char **)))();
					/* internal client code */
	double		kp_blktime;	/* start of blocking */
};

#ifdef __cplusplus
extern "C" {
#endif

extern void             kboot(int *argc, char **argv);
extern int		atkexit(void (*exitfunc)(void), char *arg);
extern int		kattach(int pri);
extern int		kbgetpid(void);
extern int		kcreate(const char *path, char **argv);
extern int		kcreatev(const char *path, char **argv,
				 char **envv, char *wrkdir, int *fds,
				 int umask_mode);
extern int		kdetach(int pid);
  extern int		kenter(const char *name, int pri);
extern int		kinit(int pri);
extern int		krecv(struct kmsg *pkmsg);
extern int		krecvfront(struct kmsg *pkmsg);
extern int		krecvback(struct kmsg *pkmsg);
extern int		ksend(struct kmsg *pkmsg);
extern int		ksr(struct kmsg *pkmsg, struct kmsg *pkmsg2);
extern int		ksrfront(struct kmsg *pkmsg, struct kmsg *pkmsg2);
extern int		ksrback(struct kmsg *pkmsg2);
extern int		kstate(int pid, struct kstatus *pstate);
extern void		kexit(int status);
extern void		_kexit(int status);
extern void             lam_kexit(int status);

extern void             _ksig_follow(void);
extern void             _ksig_init(void);

#if LAM_FORTRANNOUNDERSCORE
#else
extern void             kpause_(void);
#endif

#ifdef __cplusplus
}
#endif

#endif
