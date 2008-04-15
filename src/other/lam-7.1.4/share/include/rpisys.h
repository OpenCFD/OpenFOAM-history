/*
 * Copyright (c) 2001-2002 The Trustees of Indiana University.  
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
 * $Id: rpisys.h,v 6.22 2003/12/15 05:34:26 vsahay Exp $
 *
 * Function:	- RPI dependent structures
 */

#ifndef LAM_SSI_RPI_RPISYS_H
#define LAM_SSI_RPI_RPISYS_H

#include <sys/types.h>

#include <app_mgmt.h>
#include <all_list.h>
#include <mpi.h>


/*
 * Message envelopes.  RPI's are free to use their own struct for
 * envelopes, but this one is fairly generic and is used by multiple
 * RPI's so we might as well put it at the top level.
 */

struct lam_ssi_rpi_envl {
  int4 ce_len;		/* message length */
  int4 ce_tag;		/* message tag (16 bits) */
  int4 ce_flags;	/* flags */
#define C2CTINY		0x01			/* tiny protocol */
#define C2CSHORT	0x02			/* short protocol */
#define C2CLONG		0x04			/* long protocol */
#define C2CACK		0x08			/* ACK message */
#define C2C2ND		0x10			/* 2nd message chunk */
#define C2CSSEND	0x20			/* synchronous send */
#define C2CBOX		0x40			/* long using postbox */
#define C2CBUFFERED	0x80			/* env previously buffered */
  int4 ce_rank;		/* peer rank */
  int4 ce_cid;		/* message context ID */
  int4 ce_seq;		/* sequence number */
};


/*
 * MPI processes
 */

struct _proc {
  struct _gps	p_gps;			/* process GPS */
  int		p_ger_nsnd;		/* #msgs sent there */
  int		p_mode;			/* mode flags */
#define LAM_PFLAG	0x001		/* used for marking */
#define LAM_PDEAD	0x002		/* node died */
#define LAM_PRPIINIT	0x004		/* RPI initialized */
#define LAM_PCLIENT	0x008		/* in client world */
#define LAM_PHOMOG	0x010		/* homogeneous */
#define LAM_PRPICONNECT	0x020		/* RPI connected */
#define LAM_PRPIADDED   0x040           /* RPI added newly */
  int		p_refcount;		/* reference count */
  int		p_num_buf_env;		/* # buffered envelopes */

  /* These are just pointers.  Hence, they is of fixed length.
     However, in a perversion of "normal" C, the struct of what it
     actually points to is re-defined in each different SSI RPI
     module.  As such, this pointer can only be successfully
     de-referenced in the SSI RPI module in which is was created.
     This is a poor-man's way of doing type safety in C. :-\ */

  struct lam_ssi_rpi_proc *p_rpi;
};


/*
 * Individual MPI requests
 */
 
struct _req {
  char		*rq_name;
  int		rq_type;
#define LAM_RQISEND	0
#define LAM_RQIBSEND	1
#define LAM_RQISSEND	2
#define LAM_RQIRSEND	3
#define LAM_RQIRECV	4
#define LAM_RQIPROBE	5
#define LAM_RQIFAKE	6
  int		rq_state;
#define LAM_RQSINIT	0			/* request initialized */
#define LAM_RQSSTART	1			/* active, nothing done yet */
#define LAM_RQSACTIVE	4			/* active, undone request */
#define LAM_RQSDONE	3			/* request done */
  int		rq_marks;		/* persistent flags */
#define LAM_RQFPERSIST	0x0001			/* persistent request */
#define LAM_RQFDYNBUF	0x0004			/* dynamic buffer */
#define LAM_RQFDYNREQ	0x0008			/* dynamic request */
#define LAM_RQFSOURCE	0x0200			/* source request */
#define LAM_RQFDEST	0x0400			/* destination request */
#define LAM_RQFBLKTYPE	0x0800			/* blocking request type */
#define LAM_RQFOSORIG	0x1000			/* origin 1-sided req. type */
#define LAM_RQFOSTARG	0x2000			/* target 1-sided req. type */
#define LAM_RQFMAND	0x4000			/* mandatory req-must finish */
  int		rq_flags;		/* active req flags */
#define LAM_RQFCANCEL	0x00000002		/* cancelled request */
#define LAM_RQFBLOCK	0x00000010		/* blocking request */
#define LAM_RQFTRUNC	0x00000020		/* truncated message */
#define LAM_RQFHDLDONE	0x00000040		/* handler has been invoked */
#define LAM_RQFORPHAN	0x00000100		/* destroy when done */
#define LAM_RQFCHAR	0x00100000		/* == DRAWMSG */
#define LAM_RQFINT	0x00200000		/* == DINT4MSG */
#define LAM_RQFFLOAT	0x00400000		/* == DFLT4MSG */
#define LAM_RQFDOUBLE	0x00800000		/* == DFLT8MSG */
#define LAM_RQFSHORT	0x01000000		/* == DINT2MSG */
#define LAM_RQFMARK	0x02000000		/* arbitrary mark */
#define LAM_RQFACKDONE	0x04000000		/* ACK is done */
  char		*rq_packbuf;		/* pack buffer */
  int		rq_packsize;		/* pack buffer size */
  int		rq_count;		/* MPI request parameter */
  void		*rq_buf;		/* MPI request parameter */
  MPI_Datatype	rq_dtype;		/* MPI request parameter */
  int		rq_rank;		/* MPI request parameter */
  int		rq_tag;			/* MPI request parameter */
  MPI_Comm	rq_comm;		/* MPI request parameter */
  int		rq_cid;			/* context ID to use */
  int		rq_func;		/* MPI func. which made req. */
  int		rq_seq;			/* seq# of associated msg */
  int		rq_f77handle;		/* F77 handle */
  MPI_Status	rq_status;		/* status info storage */
  struct _bsndhdr *rq_bsend;		/* bsend header */
  struct _proc	*rq_proc;		/* peer process */
  struct _req	*rq_next;		/* next request */

  /* Used for one-sided and IMP.  Should really be unified somehow
     (e.g., when IMPI turns into a real RPI) */

  void		*rq_extra;		/* extra state */
  int		(*rq_hdlr)();		/* handler function */
  MPI_Request     rq_shadow;              /* shadow IMPI requests */

  /* Allow RPI-specific stuff to hang off the request */
  /* These are just pointers.  Hence, they are of fixed length.
     However, in a perversion of "normal" C, the struct of what it
     actually points to is re-defined in each different SSI RPI
     module.  As such, this pointer can only be successfully
     de-referenced in the SSI RPI module in which is was created.
     This is a poor-man's way of doing type safety in C. :-\ */

  struct lam_ssi_rpi_req *rq_rpi;
};


/*
 * Unexpected message buffering
 */

#define INITHASH ((int4) 59)


/*
 * context ID hash table entry
 */

struct lam_ssi_rpi_cbuf_cid {
  int4 cb_cid;			/* context ID */
  MPI_Comm cb_comm;		/* communicator */
  LIST *cb_envs;		/* envelope list */
};


/*
 * message/envelope entry
 */

struct lam_ssi_rpi_cbuf_msg {
  struct _proc *cm_proc;		/* source process */
  struct lam_ssi_rpi_envl cm_env;	/* envelope */
  struct lam_ssi_rpi_cbuf *cm_extra; /* for the rpi to hang info */
  char *cm_buf;				/* short message data */
  int cm_dont_delete;
  MPI_Request cm_req;			/* send req if sender == self */
};


/*
 * Public macros
 */

#define lam_ssi_rpi_comm_dead_m(r)	\
	(((r)->rq_comm->c_flags & (LAM_CLDEAD | LAM_CRDEAD))	\
	? lam_ssi_rpi_comm_dead(r) : 0)


/*
 * Public functions
 */

#ifdef __cplusplus
extern "C" {
#endif

  int lam_ssi_rpi_cbuf_init(void);
  void lam_ssi_rpi_cbuf_delete(struct lam_ssi_rpi_cbuf_msg *);
  void lam_ssi_rpi_cbuf_end(void);
  void *lam_ssi_rpi_cbuf_append(struct lam_ssi_rpi_cbuf_msg *);
  struct lam_ssi_rpi_cbuf_msg *
    lam_ssi_rpi_cbuf_find(struct lam_ssi_rpi_envl *);

  int lam_ssi_rpi_comm_dead(MPI_Request);
  int lam_ssi_rpi_envl_cmp(struct lam_ssi_rpi_envl *,
			   struct lam_ssi_rpi_envl *);
  void lam_ssi_rpi_fill_mpi_status(MPI_Request, int, int, int);

  int lam_ssi_rpi_base_alloc_mem(MPI_Aint size, MPI_Info info, 
				 void *baseptr);
  int lam_ssi_rpi_base_free_mem(void *base);

  int lam_ssi_rpi_base_check_priority(char *module_name, int base, 
				      int want_default, int *priority);

  void lam_ssi_rpi_commfault(int node);

#ifdef __cplusplus
}
#endif

#endif	/* LAM_SSI_RPI_RPISYS_H */
