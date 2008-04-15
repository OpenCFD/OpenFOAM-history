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
 * $Id: mpisys.h,v 6.64 2004/03/02 03:43:26 jsquyres Exp $
 *
 *	Function:	- MPI system header file
 *			- the real meat is here
 */

#ifndef _MPISYS_H
#define _MPISYS_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <lam_config.h>
#include <all_list.h>
#include <all_hash.h>
#include <app_mgmt.h>
#include <kio.h>
#include <mpi.h>
#include <portable.h>
#include <terror.h>
#include <all_opt.h>
#include <lam-ssi-coll.h>


/*
 * LAM constants & macros
 */
#define LAM_PREDEF		0x01		/* object is predefined */
#define LAM_LANGF77		0x02		/* object created in F77 code */
#define LAM_COLLMAXLIN		4		/* linear coll. op. threshold */
#define LAM_COLLMAXDIM		16		/* maximum cube dimension */
#define LAM_PORTLEN		16		/* maximum port name length */


/*
 * These constants correspond to FORTRAN datatypes.  They are used internally
 * and should not be visible to C MPI user code.
 */
#define	MPI_F_INTEGER		((MPI_Datatype) &lam_mpi_integer)
#define MPI_F_INTEGER1          ((MPI_Datatype) &lam_mpi_integer1) 
#define MPI_F_INTEGER2          ((MPI_Datatype) &lam_mpi_integer2)
#define MPI_F_INTEGER4          ((MPI_Datatype) &lam_mpi_integer4)
#define MPI_F_INTEGER8          ((MPI_Datatype) &lam_mpi_integer8)
#define	MPI_F_REAL		((MPI_Datatype) &lam_mpi_real)
#define MPI_F_REAL4             ((MPI_Datatype) &lam_mpi_real4)
#define MPI_F_REAL8             ((MPI_Datatype) &lam_mpi_real8)
#define MPI_F_REAL16            ((MPI_Datatype) &lam_mpi_real16)
#define	MPI_F_DOUBLE_PRECISION	((MPI_Datatype) &lam_mpi_dblprec)
#define	MPI_F_CHARACTER		((MPI_Datatype) &lam_mpi_character)
#define	MPI_F_LOGICAL		((MPI_Datatype) &lam_mpi_logic)
#define MPI_F_COMPLEX		((MPI_Datatype) &lam_mpi_cplex)
#define MPI_F_DOUBLE_COMPLEX	((MPI_Datatype) &lam_mpi_dblcplex)
#define MPI_F_2INTEGER		((MPI_Datatype) &lam_mpi_2integer)
#define MPI_F_2REAL		((MPI_Datatype) &lam_mpi_2real)
#define MPI_F_2DOUBLE_PRECISION	((MPI_Datatype) &lam_mpi_2dblprec)

/*
 * These constants correspnd to C++ datatypes.  They are used internally
 * and should not be visible to C/Fortran MPI user code.
 */
#define MPI_CXX_BOOL            ((MPI_Datatype) &lam_mpi_cxx_bool)
#define MPI_CXX_COMPLEX         ((MPI_Datatype) &lam_mpi_cxx_cplex)
#define MPI_CXX_DOUBLE_COMPLEX  ((MPI_Datatype) &lam_mpi_cxx_dblcplex)
#define MPI_CXX_LONG_DOUBLE_COMPLEX ((MPI_Datatype) &lam_mpi_cxx_ldblcplex)

/*
 * Fortran copy and delete function generic types.
 */
typedef void (MPI_F_copy_function)(int *, int *, int *,
    					int *, int *, int *, int *);
typedef void (MPI_F_delete_function)(int *, int *, int *, int *, int *);

typedef void (*lam_void_fn_t)(void); 

/*
 * One sided communication.
 */
#define	LAM_OSD_HEADER		0
#define	LAM_OSD_ACCUMULATE	1
#define	LAM_OSD_GET		2
#define	LAM_OSD_PUT		3
#define	LAM_OSD_FENCE		4
#define	LAM_OSD_START		5
#define	LAM_OSD_COMPLETE	6
#define	LAM_OSD_POST		7
#define	LAM_OSD_WAIT		8

/*
 * structures
 */
struct _dtype {
	int		dt_format;
#define LAM_DTBASIC		0		/* basic datatype */
#define LAM_DTCONTIG		1		/* contiguous */
#define LAM_DTVECTOR		2		/* vector */
#define LAM_DTHVECTOR		3		/* hvector */
#define LAM_DTINDEXED		4		/* indexed */
#define LAM_DTHINDEXED		5		/* hindexed */
#define LAM_DTSTRUCT		6		/* struct */
#define LAM_DTHVECTORCREAT	7		/* extended vector */
#define	LAM_DTHINDEXEDCREAT	8		/* extended indexed */
#define	LAM_DTSTRUCTCREAT 	9		/* extended struct */
#define LAM_DTINDEXEDBLK	10		/* indexed block */
#define LAM_DTSUBARRAY		11		/* local array */
#define LAM_DTDARRAY		12		/* distributed array */
	int		dt_flags;
#define LAM_DTHASUB		0x0010		/* ub is in the type map? */
#define LAM_DTHASLB		0x0020		/* lb is in the type map? */
#define LAM_DTNOPACK		0x0040		/* no packing required? */
#define LAM_DTNOXADJ		0x0080		/* extent not adjusted? */
#define LAM_DTLOWSET		0x0100		/* lower bound set */
#define LAM_DTHIGHSET		0x0200		/* upper bound set */
#define LAM_DTF77ORDER		0x0400		/* column major? */
#define LAM_DTISDUP		0x0800		/* duped type */
#define LAM_DTISRESIZE		0x1000		/* resized type */
	int		dt_commit;		/* committed? */
	int		dt_refcount;		/* reference count */
	int		dt_f77handle;		/* F77 handle */
	int		dt_label;		/* flat-form label */
	int		dt_align;		/* alignment */
	int		dt_upper;		/* upper extent */
	int		dt_lower;		/* lower extent */
	int		dt_size;		/* basic size */
	int		dt_dataup;		/* data upper limit */
	int		dt_datalow;		/* data lower limit */
	int		dt_nelem;		/* # top elements */
	int		dt_count;		/* count */
	int		dt_length;		/* vector length */
	MPI_Aint	dt_stride;		/* vector stride */
	MPI_Datatype	dt_dtype;		/* c/v/i datatype */
	int		*dt_lengths;		/* i/s lengths */
	MPI_Aint	*dt_disps;		/* i/s displacements */
	MPI_Datatype	*dt_dtypes;		/* struct datatypes */
	HASH		*dt_keys;		/* keys cache hash table */
	int		*dt_uargs;		/* record user arguments */
	char		dt_name[MPI_MAX_OBJECT_NAME];
        long		dt_reserved[4];		/* for expansion */
};

struct _group {
	int		g_nprocs;		/* # processes */
	int		g_myrank;		/* my local rank */
	int		g_refcount;		/* reference count */
	int		g_f77handle;		/* F77 handle */
	struct _proc	**g_procs;		/* processes */
};

struct _comm {
	int		c_flags;		/* properties */
#define LAM_CINTER	0x10			/* intercommunicator? */
#define LAM_CLDEAD	0x20			/* local group dead? */
#define LAM_CRDEAD	0x40			/* remote group dead? */
#define LAM_CIMPI	0x80			/* IMPI communicators */
#define LAM_CHOMOG	0x100			/* this comm is endian homog */
#define LAM_CHIDDEN	0x200			/* hidden communications */
#define LAM_RBASIC      0x400                   /* lam_basic req - intercomm */
	int		c_contextid;		/* context ID */
	int		c_refcount;		/* reference count */
	MPI_Group	c_group;		/* local group */
	MPI_Group	c_rgroup;		/* remote group */
	HASH		*c_keys;		/* keys cache hash table */
	int		c_cube_dim;		/* inscribing cube dim. */
	int		c_topo_type;		/* topology type */
	int		c_topo_nprocs;		/* # topo. processes */
	int		c_topo_ndims;		/* # cart. dimensions */
	int		c_topo_nedges;		/* # graph edges */
	int		*c_topo_dims;		/* cart. dimensions */
	int		*c_topo_coords;		/* cart. coordinates */
	int		*c_topo_index;		/* graph indices */
	int		*c_topo_edges;		/* graph edges */
	int		c_f77handle;		/* F77 handle */
	MPI_Win		c_window;		/* window (if any) on comm */
	MPI_Errhandler	c_errhdl;		/* error handler */
	char		c_name[MPI_MAX_OBJECT_NAME];
  /* This is not #if WANT_IMPI'ed out so that we can stay binary
     compatible with non-IMPI compiled programs */
        MPI_Comm        c_shadow;               /* shadow com for IMPI comms */
        long            c_reserved[4];          /* used in IMPI :-( */

        lam_ssi_coll_actions_1_1_0_t  c_ssi_coll;
        struct lam_ssi_coll_data *c_ssi_coll_data;
        struct lam_ssi_coll_lam_basic_data *c_ssi_coll_lam_basic_data;

  /* Store lam_basic_finalize pointer because we need to call 
     lam_basic_finalize() whenever a module needs support of lam_basic.  This
     typically happens in case of intercommunicators which need support
     from lam_basic (local communicator that gets created in lam_basic).
     lam_basic_finalize() should be called to destroy this local communicator
     and free the local structure
  */
        lam_ssi_coll_finalize_fn_t lsca_basic_finalize;
};

struct _osdhdr {
	int		os_func;		/* one sided function */
	int		os_op;			/* accumulate operation */
	int		os_count;		/* target count */
	int		os_format;		/* target datatype format */
	int		os_type;		/* target datatype basic type */
	int		os_blklen;		/* target datatype block len. */
	int		os_stride;		/* target datatype stride */
	int		os_dcount;		/* target datatype count */
	int8		os_disp;		/* target displacement */
};

struct _window {
	MPI_Comm	w_comm;			/* window communicator */
	void		*w_base;		/* window base address */
	MPI_Aint	w_size;			/* window size (bytes) */
	int		w_disp_unit;		/* window disp unit (bytes) */
	int		w_flags;		/* window state flags */
#define LAM_WFACCEPOCH	0x010			/* window in access epoch */
#define LAM_WFEXPEPOCH	0x020			/* window in exposure epoch */
#define LAM_WFPOSTED	0x040			/* win/proc has been posted */
#define	LAM_WFSTARTED	0x080			/* win/proc has been started */
#define LAM_WFNOOUT	0x100			/* window closed */
	int		w_ngroup;		/* num procs in 2 party synch */
	int		w_ncomplete;		/* num of procs to complete */
	int		w_nfence;		/* num of procs to fence */
	int		*w_pstate;		/* state of processes */
	int		w_f77handle;		/* F77 handle */
	HASH		*w_keys;		/* attributes */
	MPI_Request	w_req;			/* header receive request */
	struct _osdhdr 	w_header;		/* incoming protocol header */
	MPI_Errhandler	w_errhdl;		/* error handler */
	long		w_reserved[4];		/* for expansion */
};

struct _op {
	void		(*op_func)();		/* reduction function */
	int		op_commute;		/* commutative? */
	int		op_f77handle;		/* F77 handle */
	int		op_flags;		/* properties */
};

struct _errhdl {
	void		(*eh_func)();		/* error function */
	int		eh_refcount;		/* reference count */
	int		eh_f77handle;		/* F77 handle */
	int		eh_flags;		/* properties */
#define LAM_EHCOMM	0x10			/* communicator error handler */
#define LAM_EHWIN	0x20			/* window error handler */
#define LAM_EHFILE	0x40			/* file error handler */
};

struct _attrkey {
	lam_void_fn_t	ak_copy;		/* copy function */
	lam_void_fn_t	ak_del;		/* delete function */
	void		*ak_extra;		/* extra info */
	int		ak_refcount;		/* reference count */
	int		ak_flags;		/* properties */
#define LAM_AKCOMM	0x10			/* communicator key */
#define LAM_AKWIN	0x20			/* window key */
#define LAM_AKTYPE	0x40			/* datatype key */
};

struct _attr {
	int		a_key;			/* attribute key */
	void		*a_value;		/* attribute value */
};

/*
 * If this is changed be sure to update MPI_BSEND_OVERHEAD appropriately
 * in mpi.h and mpif.h.  
 */
struct _bsndhdr {
	MPI_Request	bsh_req;
	int		bsh_size;
	struct _bsndhdr	*bsh_prev;
	struct _bsndhdr	*bsh_next;
};

struct _fyimsg {
	int4		fym_src;		/* src global/local rank */
	int4		fym_dest;		/* dest global/local rank */
	int4		fym_count;		/* count */
	int4		fym_dtpid;		/* datatype/pid */
};

struct _fyiproc {
	int4		fyp_me;			/* my global/local rank */
	int4		fyp_peer;		/* peer global/local rank */
	int4		fyp_peergps;		/* peer node/index */
	int4		fyp_root;		/* root global/local rank */
	int4		fyp_rootgps;		/* root node/index */
	int4		fyp_cidtag;		/* context ID & tag */
	int4		fyp_func;		/* top level function */
	int4		fyp_count;		/* count */
	int4		fyp_dtype;		/* datatype */
};

struct _infoent {
	char		*ie_value;
	char		ie_key[MPI_MAX_INFO_KEY + 1];
};

struct _info {
	LIST		*info_list;
	int		info_f77handle;
};

struct _port {
	char		prt_name[LAM_PORTLEN + 1];
	int		prt_num;
};

/*
 * for internal purposes
 */
extern MPI_Comm		lam_comm_parent;
extern struct _proc	*lam_myproc;
extern struct _proc	**lam_procs;
extern int		lam_num_procs;
extern LIST		*lam_comms;
extern LIST		*lam_ports;
extern float8		lam_clockskew;
extern int		lam_f77init;
extern int		lam_c2c;
#if LAM_WANT_IMPI
extern int		lam_impi;
#endif

extern int		lam_ger;
extern int		lam_homog;
extern struct jobid_t	lam_jobid;
extern int		lam_universe_size;
extern int		lam_universe_ncpus;
extern int		lam_universe_nnodes;
extern int		lam_appnum;
extern int		lam_rq_flblock;
extern int		lam_rq_nreqs;
extern int		lam_rq_nactv;
extern int		lam_seqnum;
extern MPI_Request	lam_rq_top;
extern MPI_Request	lam_rq_bottom;
extern int		lam_topfunc;
extern int		lam_flinit;
extern int		lam_flfinal;
extern int		lam_mpi_max_tag;
extern int		lam_mpi_max_cid;
extern MPI_Datatype	lam_osdhdr_type;

extern OPT              *lam_mpi_ao;

extern char             *lam_argv0;

extern struct kio_t     _kio;			/* kernel I/O block */

/* Variable instantiated & initialised in lammpiinit.c for TotalView
   DLL support */
extern int lam_tv_comm_seqnum;

/*
 * state checking macros
 */
#define	LAM_IS_INTER(c)	((c)->c_flags & LAM_CINTER)	/* is an intercomm */
#define LAM_REQ_COLL_BASIC(c) ((c)->c_flags & LAM_RBASIC)
#define LAM_IS_INTRA(c) (!((c)->c_flags & LAM_CINTER))	/* is an intracomm */
#if LAM_WANT_IMPI
#define LAM_IS_IMPI(c)  ((c)->c_shadow != 0)            /* is an IMPI comm */
#endif
#define LAM_IS_CART(c)	((c)->c_topo_type == MPI_CART)	/* is cartesian */
#define LAM_IS_GRAPH(c) ((c)->c_topo_type == MPI_GRAPH)	/* is graph */

/*
 * useful tracing macros
 */
#define LAM_TRACE(a)	if ((_kio.ki_rtf & RTF_TRON) == RTF_TRON) a

#define LAM_TRACE_TOP() \
		(((_kio.ki_rtf & RTF_TRON) == RTF_TRON) && (!lam_tr_incff()))

/*
 * overhead reduction
 */
#define _mpi_req_add_m(req)				\
{							\
	if (lam_rq_top == 0) {				\
		lam_rq_top = (req);			\
	} else {					\
		lam_rq_bottom->rq_next = (req);		\
	}						\
							\
	lam_rq_bottom = (req);				\
	(req)->rq_next = 0;				\
							\
	++lam_rq_nreqs;					\
}

#define _mpi_req_rem_m(req)					\
{								\
	MPI_Request	_p, _p2;				\
	for (_p = lam_rq_top, _p2 = 0; _p && (_p != (req));	\
		_p2 = _p, _p = _p->rq_next);			\
	if (_p) {						\
	  if (_p == lam_rq_top) {				\
	    lam_rq_top = _p->rq_next;				\
	  } else {						\
	    _p2->rq_next = _p->rq_next;				\
	  }							\
	  if (_p == lam_rq_bottom) lam_rq_bottom = _p2;		\
	  --lam_rq_nreqs;					\
	}							\
}

#define _mpi_req_blkclr_m()				\
{							\
	MPI_Request	_p;				\
	for (_p = lam_rq_top; _p; _p = _p->rq_next) {	\
		_p->rq_flags &= ~LAM_RQFBLOCK;		\
	}						\
	lam_rq_flblock = 0;				\
}

#define _mpi_req_blkset_m(req)			\
{						\
	(req)->rq_flags |= LAM_RQFBLOCK;	\
	lam_rq_flblock = 1;			\
}

/* Used to be macros - now more complex functions */
#define lam_setfunc_m(locfunc) lam_setfunc((locfunc))
#define lam_resetfunc_m(locfunc) lam_resetfunc((locfunc))
#define lam_getfunc_m() lam_getfunc()


#define lam_inited_m()	(lam_flinit)

/* This has been replaced with slightly more complex functionality, so
   just have it call the real function  */

#define lam_initerr_m()	lam_initerr()


/*
 * In some cases, the underlying memcpy() is broken.  So we have our
 * own.  See the note in share/mpi/lammemcpy.c for more details.
 */

#if LAM_WANT_PREFIX_MEMCPY
  extern void *lam_memcpy(void *dest, const void *src, size_t size);
#else
  #define lam_memcpy(a, b, c) memcpy((a), (b), (c))
#endif


/*
 * profiling
 */
#ifdef LAM_PROFILELIB

#define MPIL_Comm_id			PMPIL_Comm_id
#define MPIL_Comm_gps			PMPIL_Comm_gps
#define MPIL_Comm_parent		PMPIL_Comm_parent
#define MPIL_Signal			PMPIL_Signal
#define MPIL_Spawn			PMPIL_Spawn
#define MPIL_Type_id			PMPIL_Type_id
#define MPIL_Spawn			PMPIL_Spawn
#define MPIL_Universe_size		PMPIL_Universe_size
#define MPIL_Trace_on			PMPIL_Trace_on
#define MPIL_Trace_off			PMPIL_Trace_off
#define MPIL_Request_set_name           PMPIL_Request_set_name
#define MPIL_Request_get_name           PMPIL_Request_get_name

#define MPI_Abort			PMPI_Abort 
#define MPI_Accumulate			PMPI_Accumulate
#define MPI_Address			PMPI_Address
#define MPI_Allgather			PMPI_Allgather 
#define MPI_Allgatherv			PMPI_Allgatherv 
#define MPI_Alloc_mem			PMPI_Alloc_mem
#define MPI_Allreduce			PMPI_Allreduce 
#define MPI_Alltoall			PMPI_Alltoall 
#define MPI_Alltoallv			PMPI_Alltoallv 
#define MPI_Alltoallw                   PMPI_Alltoallw
#define MPI_Attr_delete			PMPI_Attr_delete 
#define MPI_Attr_get			PMPI_Attr_get 
#define MPI_Attr_put			PMPI_Attr_put 
#define MPI_Barrier			PMPI_Barrier 
#define MPI_Bcast			PMPI_Bcast 
#define MPI_Bsend			PMPI_Bsend 
#define MPI_Bsend_init			PMPI_Bsend_init 
#define MPI_Buffer_attach		PMPI_Buffer_attach 
#define MPI_Buffer_detach		PMPI_Buffer_detach 
#define MPI_Cancel			PMPI_Cancel 
#define MPI_Cart_coords			PMPI_Cart_coords 
#define MPI_Cart_create			PMPI_Cart_create 
#define MPI_Cart_get			PMPI_Cart_get 
#define MPI_Cart_map			PMPI_Cart_map 
#define MPI_Cart_rank			PMPI_Cart_rank 
#define MPI_Cart_shift			PMPI_Cart_shift 
#define MPI_Cart_sub			PMPI_Cart_sub 
#define MPI_Cartdim_get			PMPI_Cartdim_get 
#define MPI_Close_port			PMPI_Close_port
#define MPI_Comm_accept			PMPI_Comm_accept
#define MPI_Comm_c2f			PMPI_Comm_c2f
#define MPI_Comm_compare		PMPI_Comm_compare 
#define MPI_Comm_connect		PMPI_Comm_connect
#define MPI_Comm_create			PMPI_Comm_create 
#define MPI_Comm_create_errhandler	PMPI_Comm_create_errhandler
#define MPI_Comm_create_keyval		PMPI_Comm_create_keyval
#define MPI_Comm_delete_attr		PMPI_Comm_delete_attr 
#define MPI_Comm_disconnect		PMPI_Comm_disconnect
#define MPI_Comm_dup			PMPI_Comm_dup 
#define MPI_Comm_f2c			PMPI_Comm_f2c
#define MPI_Comm_free			PMPI_Comm_free 
#define MPI_Comm_free_keyval		PMPI_Comm_free_keyval
#define MPI_Comm_get_attr		PMPI_Comm_get_attr 
#define MPI_Comm_get_contextid		PMPI_Comm_get_contextid 
#define MPI_Comm_get_errhandler		PMPI_Comm_get_errhandler
#define MPI_Comm_get_name		PMPI_Comm_get_name
#define MPI_Comm_get_parent		PMPI_Comm_get_parent
#define MPI_Comm_group			PMPI_Comm_group 
#define MPI_Comm_join			PMPI_Comm_join
#define MPI_Comm_rank			PMPI_Comm_rank 
#define MPI_Comm_remote_group		PMPI_Comm_remote_group 
#define MPI_Comm_remote_size		PMPI_Comm_remote_size 
#define MPI_Comm_set_attr		PMPI_Comm_set_attr 
#define MPI_Comm_set_errhandler		PMPI_Comm_set_errhandler
#define MPI_Comm_set_name		PMPI_Comm_set_name 
#define MPI_Comm_size			PMPI_Comm_size 
#define MPI_Comm_spawn			PMPI_Comm_spawn
#define MPI_Comm_spawn_multiple		PMPI_Comm_spawn_multiple
#define MPI_Comm_split			PMPI_Comm_split 
#define MPI_Comm_test_inter		PMPI_Comm_test_inter 
#define MPI_Dims_create			PMPI_Dims_create 
#define MPI_Errhandler_c2f              PMPI_Errhandler_c2f
#define MPI_Errhandler_create		PMPI_Errhandler_create 
#define MPI_Errhandler_free		PMPI_Errhandler_free 
#define MPI_Errhandler_f2c              PMPI_Errhandler_f2c
#define MPI_Errhandler_get		PMPI_Errhandler_get 
#define MPI_Errhandler_set		PMPI_Errhandler_set 
#define MPI_Error_class			PMPI_Error_class 
#define MPI_Error_string		PMPI_Error_string 
#define MPI_Exscan                      PMPI_Exscan
#define MPI_Finalize			PMPI_Finalize 
#define MPI_Finalized			PMPI_Finalized
#define MPI_Free_mem			PMPI_Free_mem
#define MPI_Gather			PMPI_Gather 
#define MPI_Gatherv			PMPI_Gatherv 
#define MPI_Get				PMPI_Get
#define MPI_Get_address			PMPI_Get_address
#define MPI_Get_count			PMPI_Get_count 
#define MPI_Get_elements		PMPI_Get_elements 
#define MPI_Get_processor_name		PMPI_Get_processor_name 
#define MPI_Get_version			PMPI_Get_version
#define MPI_Graph_create		PMPI_Graph_create 
#define MPI_Graph_get			PMPI_Graph_get 
#define MPI_Graph_map			PMPI_Graph_map 
#define MPI_Graph_neighbors		PMPI_Graph_neighbors 
#define MPI_Graph_neighbors_count	PMPI_Graph_neighbors_count 
#define MPI_Graphdims_get		PMPI_Graphdims_get 
#define MPI_Group_c2f			PMPI_Group_c2f
#define MPI_Group_compare		PMPI_Group_compare 
#define MPI_Group_difference		PMPI_Group_difference 
#define MPI_Group_excl			PMPI_Group_excl 
#define MPI_Group_f2c			PMPI_Group_f2c
#define MPI_Group_free			PMPI_Group_free 
#define MPI_Group_incl			PMPI_Group_incl 
#define MPI_Group_intersection		PMPI_Group_intersection 
#define MPI_Group_range_excl		PMPI_Group_range_excl 
#define MPI_Group_range_incl		PMPI_Group_range_incl 
#define MPI_Group_rank			PMPI_Group_rank 
#define MPI_Group_size			PMPI_Group_size 
#define MPI_Group_translate_ranks	PMPI_Group_translate_ranks 
#define MPI_Group_union			PMPI_Group_union 
#define MPI_Ibsend			PMPI_Ibsend 
#define MPI_Info_c2f			PMPI_Info_c2f
#define MPI_Info_create 		PMPI_Info_create 
#define MPI_Info_delete			PMPI_Info_delete
#define MPI_Info_dup 			PMPI_Info_dup 
#define MPI_Info_f2c			PMPI_Info_f2c
#define MPI_Info_free 			PMPI_Info_free 
#define MPI_Info_get 			PMPI_Info_get 
#define MPI_Info_get_nkeys 		PMPI_Info_get_nkeys 
#define MPI_Info_get_nthkey 		PMPI_Info_get_nthkey 
#define MPI_Info_get_valuelen 		PMPI_Info_get_valuelen 
#define MPI_Info_set 			PMPI_Info_set 
#define MPI_Init			PMPI_Init 
#define MPI_Init_thread			PMPI_Init_thread
#define MPI_Initialized			PMPI_Initialized 
#define MPI_Intercomm_create		PMPI_Intercomm_create
#define MPI_Intercomm_merge		PMPI_Intercomm_merge
#define MPI_Iprobe			PMPI_Iprobe 
#define MPI_Irecv			PMPI_Irecv 
#define MPI_Irsend			PMPI_Irsend 
#define MPI_Is_thread_main		PMPI_Is_thread_main
#define MPI_Isend			PMPI_Isend 
#define MPI_Issend			PMPI_Issend 
#define MPI_Keyval_create		PMPI_Keyval_create 
#define MPI_Keyval_free			PMPI_Keyval_free 
#define MPI_Lookup_name			PMPI_Lookup_name
#define MPI_Op_c2f			PMPI_Op_c2f
#define MPI_Op_create			PMPI_Op_create 
#define MPI_Op_f2c			PMPI_Op_f2c
#define MPI_Op_free			PMPI_Op_free 
#define MPI_Open_port			PMPI_Open_port
#define MPI_Pack			PMPI_Pack 
#define MPI_Pack_size			PMPI_Pack_size 
#define MPI_Pcontrol			PMPI_Pcontrol 
#define MPI_Probe			PMPI_Probe 
#define MPI_Publish_name		PMPI_Publish_name
#define MPI_Put				PMPI_Put
#define MPI_Query_thread		PMPI_Query_thread
#define MPI_Recv			PMPI_Recv 
#define MPI_Recv_init			PMPI_Recv_init 
#define MPI_Reduce			PMPI_Reduce 
#define MPI_Reduce_scatter		PMPI_Reduce_scatter 
#define MPI_Request_c2f			PMPI_Request_c2f
#define MPI_Request_f2c			PMPI_Request_f2c
#define MPI_Request_free		PMPI_Request_free 
#define MPI_Rsend			PMPI_Rsend 
#define MPI_Rsend_init			PMPI_Rsend_init 
#define MPI_Scan			PMPI_Scan 
#define MPI_Scatter			PMPI_Scatter 
#define MPI_Scatterv			PMPI_Scatterv 
#define MPI_Send			PMPI_Send 
#define MPI_Send_init			PMPI_Send_init 
#define MPI_Sendrecv			PMPI_Sendrecv 
#define MPI_Sendrecv_replace		PMPI_Sendrecv_replace 
#define MPI_Ssend			PMPI_Ssend 
#define MPI_Ssend_init			PMPI_Ssend_init 
#define MPI_Start			PMPI_Start 
#define MPI_Startall			PMPI_Startall
#define MPI_Status_c2f			PMPI_Status_c2f
#define MPI_Status_f2c			PMPI_Status_f2c
#define MPI_Test			PMPI_Test 
#define MPI_Test_cancelled		PMPI_Test_cancelled
#define MPI_Testall			PMPI_Testall 
#define MPI_Testany			PMPI_Testany 
#define MPI_Testsome			PMPI_Testsome 
#define MPI_Topo_test			PMPI_Topo_test 
#define MPI_Type_c2f			PMPI_Type_c2f
#define MPI_Type_commit			PMPI_Type_commit 
#define MPI_Type_contiguous		PMPI_Type_contiguous 
#define MPI_Type_create_darray		PMPI_Type_create_darray
#define MPI_Type_create_hindexed	PMPI_Type_create_hindexed
#define MPI_Type_create_hvector		PMPI_Type_create_hvector
#define MPI_Type_create_keyval		PMPI_Type_create_keyval
#define MPI_Type_create_resized		PMPI_Type_create_resized
#define MPI_Type_create_struct		PMPI_Type_create_struct
#define MPI_Type_create_subarray	PMPI_Type_create_subarray
#define MPI_Type_delete_attr		PMPI_Type_delete_attr 
#define MPI_Type_dup			PMPI_Type_dup 
#define MPI_Type_extent			PMPI_Type_extent 
#define MPI_Type_f2c			PMPI_Type_f2c
#define MPI_Type_free			PMPI_Type_free 
#define MPI_Type_free_keyval		PMPI_Type_free_keyval
#define MPI_Type_get_attr		PMPI_Type_get_attr 
#define MPI_Type_get_contents		PMPI_Type_get_contents
#define MPI_Type_get_envelope		PMPI_Type_get_envelope
#define MPI_Type_get_extent		PMPI_Type_get_extent
#define MPI_Type_get_name		PMPI_Type_get_name
#define MPI_Type_get_true_extent	PMPI_Type_get_true_extent
#define MPI_Type_hindexed		PMPI_Type_hindexed 
#define MPI_Type_hvector		PMPI_Type_hvector 
#define MPI_Type_indexed		PMPI_Type_indexed 
#define MPI_Type_lb			PMPI_Type_lb 
#define MPI_Type_set_attr		PMPI_Type_set_attr 
#define MPI_Type_set_name		PMPI_Type_set_name
#define MPI_Type_size			PMPI_Type_size 
#define MPI_Type_struct			PMPI_Type_struct 
#define MPI_Type_ub			PMPI_Type_ub 
#define MPI_Type_vector			PMPI_Type_vector 
#define MPI_Unpack			PMPI_Unpack 
#define MPI_Unpublish_name		PMPI_Unpublish_name
#define MPI_Wait			PMPI_Wait 
#define MPI_Waitall			PMPI_Waitall 
#define MPI_Waitany			PMPI_Waitany 
#define MPI_Waitsome			PMPI_Waitsome 
#define MPI_Win_c2f			PMPI_Win_c2f
#define MPI_Win_call_errhandler		PMPI_Win_call_errhandler
#define MPI_Win_complete		PMPI_Win_complete
#define MPI_Win_create			PMPI_Win_create
#define MPI_Win_create_errhandler	PMPI_Win_create_errhandler
#define MPI_Win_create_keyval		PMPI_Win_create_keyval
#define MPI_Win_delete_attr		PMPI_Win_delete_attr 
#define MPI_Win_f2c			PMPI_Win_f2c
#define MPI_Win_fence			PMPI_Win_fence 
#define MPI_Win_free			PMPI_Win_free 
#define MPI_Win_free_keyval		PMPI_Win_free_keyval
#define MPI_Win_get_attr		PMPI_Win_get_attr 
#define MPI_Win_get_errhandler		PMPI_Win_get_errhandler
#define MPI_Win_get_group		PMPI_Win_get_group 
#define MPI_Win_get_name		PMPI_Win_get_name 
#define MPI_Win_lock			PMPI_Win_lock 
#define MPI_Win_post			PMPI_Win_post
#define MPI_Win_set_attr		PMPI_Win_set_attr 
#define MPI_Win_set_errhandler		PMPI_Win_set_errhandler
#define MPI_Win_set_name		PMPI_Win_set_name 
#define MPI_Win_start			PMPI_Win_start
#define MPI_Win_test			PMPI_Win_test
#define MPI_Win_unlock			PMPI_Win_unlock
#define MPI_Win_wait			PMPI_Win_wait
#define MPI_Wtick			PMPI_Wtick 
#define MPI_Wtime			PMPI_Wtime 

#endif	/* LAM_PROFILELIB */

#ifdef __cplusplus
extern "C" {
#endif

extern void	lam_band __ARGS((void *, void *, int *, MPI_Datatype *));
extern void	lam_bkerr __ARGS((int, int *, int *, int *));
extern void	lam_bor __ARGS((void *, void *, int *, MPI_Datatype *));
extern int	lam_bottom_dtsnd __ARGS((void *, int, MPI_Datatype,
				   void *, int, MPI_Datatype, int, MPI_Comm));
extern int	lam_bottom_dtrcv __ARGS((void *, int, MPI_Datatype,
				void *, int, MPI_Datatype, int, MPI_Comm));
extern int	lam_bufattach __ARGS((void *, int));
extern int	lam_bufdetach __ARGS((void *, int *));
extern int	lam_buffreebsend_ __ARGS((MPI_Request));
extern int	lam_bufinit __ARGS((MPI_Request));
extern int	lam_bufinitbsend_ __ARGS((MPI_Request));
extern void	lam_bxor __ARGS((void *, void *, int *, MPI_Datatype *));
extern int	lam_chkarg __ARGS((void *, int, MPI_Datatype,
				int, int, MPI_Comm));
extern int	lam_clocksync __ARGS((int, struct _gps *, float8 *));
extern int	lam_coll2pt __ARGS((int));
extern int	lam_mpi_coll2pt __ARGS((int));
extern void	lam_commfault __ARGS((int));
extern void	lam_comm_errfatal __ARGS((MPI_Comm *, int *, ...));
extern int	lam_comm_free __ARGS((MPI_Comm));
extern int	lam_comm_new __ARGS((int, MPI_Group, MPI_Group,
				int, MPI_Comm *));
extern int	lam_coll_alloc_inter_cid __ARGS((MPI_Comm, int, 
						 MPI_Comm, int,
						 int, int, int *));
extern int	lam_coll_alloc_intra_cid __ARGS((MPI_Comm, int, int, int *));
extern int	lam_cubedim __ARGS((int));
extern int	lam_delkey __ARGS((HASH *, int));
extern void	lam_dtalign __ARGS((MPI_Datatype));
extern void	lam_dtblock __ARGS((MPI_Datatype, MPI_Datatype, int, int));
extern int	lam_dtbuffer __ARGS((MPI_Datatype, int, char **, char **));
extern void	lam_dtcpy __ARGS((char *, char *, int, MPI_Datatype));
extern int	lam_dtsndrcv __ARGS((void *, int, MPI_Datatype,
				void *, int, MPI_Datatype, int, MPI_Comm));
extern void	lam_emptystat __ARGS((MPI_Status *));
extern int	lam_err_comm __ARGS((MPI_Comm, int, int, char *));
extern int	lam_err_win __ARGS((MPI_Win, int, int, char *));
extern int	lam_errfunc __ARGS((MPI_Comm, int, int));
extern void	lam_errreturn __ARGS((void *, int *, ...));
extern int      lam_exchange_endian_info(struct _proc **procs, int numprocs);
extern int	lam_finalized __ARGS((void));
extern int	lam_freekey __ARGS((int, int));
extern void	lam_free_onesided __ARGS((void));
extern int      lam_get_mpi_world __ARGS((int4 world_n, struct _gps *world, 
					  struct _gps *mpiworld,
					  char *prefix));
extern int      lam_match_ssi_modules __ARGS((int4 world_n, struct _gps *world, 
                                              char *prefix));
extern struct _attrkey *lam_getattr __ARGS((int));
extern int	lam_getcid __ARGS((void));
extern int	lam_getfunc __ARGS((void));
extern int	lam_numfuncs __ARGS((void));
extern struct _attr *lam_getkey __ARGS((HASH *, int));
extern void	lam_getparam __ARGS((int *, int*));
extern int	lam_getpublished  __ARGS((char *, char **));
extern int	lam_hibit __ARGS((int, int));
#if LAM_WANT_IMPI
extern int	lam_impicoll2pt __ARGS((int));
extern int	lam_mpi_impicoll2pt __ARGS((int));
#endif
extern int	lam_inited __ARGS((void));
extern void	lam_initerr __ARGS((void));
extern int	lam_initfault __ARGS((void));
extern int	lam_init_cid __ARGS((void));
extern int      lam_init_comm_world __ARGS((struct _gps *mpi_procs, 
					    int mpi_nprocs));
extern int	lam_init_onesided __ARGS((void));
extern int	lam_int_pos __ARGS((void));
extern int	lam_irecv __ARGS((void *, int, MPI_Datatype, int,
				int, MPI_Comm, MPI_Request *));
extern int	lam_isend __ARGS((void *, int, MPI_Datatype, int,
				int, MPI_Comm, MPI_Request *, int));
extern int      lam_get_proc_array __ARGS((struct _gps **, int *, int *, 
					   int *, int));
extern int	lam_keyval_create __ARGS((lam_void_fn_t, lam_void_fn_t,
                                          int *, void *, int));
extern void	lam_land __ARGS((void *, void *, int *, MPI_Datatype *));
extern int	lam_linit __ARGS((int, char **, char *, int *, int *, int *, 
				struct _gps **, int *));
extern void	lam_lor __ARGS((void *, void *, int *, MPI_Datatype *));
extern void	lam_lxor __ARGS((void *, void *, int *, MPI_Datatype *));
extern void	lam_max __ARGS((void *, void *, int *, MPI_Datatype *));
extern void	lam_maxloc __ARGS((void *, void *, int *, MPI_Datatype *));
extern void	lam_min __ARGS((void *, void *, int *, MPI_Datatype *));
extern void	lam_minloc __ARGS((void *, void *, int *, MPI_Datatype *));
extern void	lam_replace __ARGS((void *, void *, int *, MPI_Datatype *));
extern void	lam_mkcoll __ARGS((MPI_Comm));
extern void	lam_mpi_mkcoll __ARGS((MPI_Comm));
extern int	lam_mkerr __ARGS((int, int));
#if LAM_WANT_IMPI
extern void     lam_mkimpicoll __ARGS((MPI_Comm comm));
extern void     lam_mpi_mkimpicoll __ARGS((MPI_Comm comm));
#endif
extern int	lam_mkkey __ARGS((void));
extern void	lam_mkpt __ARGS((MPI_Comm));
extern void	lam_mpi_mkpt __ARGS((MPI_Comm));
extern int      lam_mpi_init(int *pargc, char ***pargv, int requested, 
			     int *pprovided, int blk);
extern void	lam_mpi_reset_sighandlers  __ARGS((void));
extern int	lam_mpi_set_sighandlers  __ARGS((void));
extern struct _proc *lam_nextproc __ARGS((void));
extern int      lam_nbarrier(struct _proc **procs, int nprocs);
extern int	lam_nprocs __ARGS((void));
extern void	lam_nukecids __ARGS((void));
extern void	lam_nukefunc __ARGS((void));
extern void	lam_nukekeys __ARGS((void));
extern void	lam_nukeprocs __ARGS((void));
extern void	lam_nukepublished __ARGS((void));
extern void	lam_nuketrace __ARGS((void));
extern int	lam_osd_blkadv __ARGS((MPI_Win));
extern int	lam_osd_complete __ARGS((MPI_Win, int));
extern int	lam_osd_fillheader __ARGS((int, int, MPI_Aint, int,
					MPI_Datatype, struct _osdhdr *));
extern int	lam_osd_handler __ARGS((MPI_Request));
extern int	lam_osd_push __ARGS((void));
extern int	lam_osd_recv __ARGS((void *, int, MPI_Datatype, int, int,
				MPI_Comm, int));
extern int	lam_osd_send __ARGS((void *, int, MPI_Datatype, int, int,
				MPI_Comm, int, int));
extern int	lam_pack __ARGS((char *, int, MPI_Datatype, char *, int));
extern int	lam_ports_init __ARGS((void));
extern void	lam_ports_free __ARGS((void));
extern int	lam_port_close __ARGS((int));
extern int	lam_port_is_open __ARGS((int));
extern int	lam_port_open __ARGS((void));
extern struct _proc *lam_procadd __ARGS((struct _gps *));
extern int	lam_proccmp __ARGS((struct _proc *, struct _proc *));
extern void     lam_procsort(struct _proc **procs, int nprocs);
extern struct _proc *lam_procfind __ARGS((struct _gps *));
extern int	lam_procfree __ARGS((struct _proc *));
extern int	lam_procrm __ARGS((struct _proc *));
extern void	lam_prod __ARGS((void *, void *, int *, MPI_Datatype *));
extern int	lam_pt2coll __ARGS((int));
extern int	lam_mpi_pt2coll __ARGS((int));
#if LAM_WANT_IMPI
extern int	lam_pt2impicoll __ARGS((int));
extern int	lam_mpi_pt2impicoll __ARGS((int));
extern int      lam_pt2impidatasync __ARGS((int));
extern int      lam_mpi_pt2impidatasync __ARGS((int));
extern int      lam_pt2impisyncack __ARGS((int));
extern int      lam_mpi_pt2impisyncack __ARGS((int));
#endif
extern int	lam_publish __ARGS((char *, char *));
extern int	lam_putkey __ARGS((HASH **, int));
extern int	lam_rank_in_group __ARGS((MPI_Group, struct _proc *));
extern int	lam_mpi_rank_in_group __ARGS((MPI_Group, struct _proc *));
extern int	lam_recv __ARGS((void *, int, MPI_Datatype, int,
				int, MPI_Comm, MPI_Status *));
extern void	lam_resetfunc __ARGS((int));
extern void	lam_rmcid __ARGS((int));
extern int	lam_send __ARGS((void *, int, MPI_Datatype, int, int,
				MPI_Comm, int));
extern void	lam_setcid __ARGS((int));
extern void	lam_setfunc __ARGS((int));
extern void	lam_setparam __ARGS((int, int, int));
extern void	lam_sum __ARGS((void *, void *, int *, MPI_Datatype *));
extern int	lam_test __ARGS((MPI_Request *, int, int *, MPI_Status *));
extern struct _proc *lam_topproc __ARGS((void));
extern void	lam_tr_cffend __ARGS((int, int, MPI_Comm,
				MPI_Datatype, int));
extern void	lam_tr_cffstart __ARGS((int));
extern int	lam_tr_comm __ARGS((MPI_Comm));
extern int	lam_tr_dtype __ARGS((MPI_Datatype));
extern void	lam_tr_dtypefree __ARGS((MPI_Datatype));
extern int	lam_tr_incff __ARGS((void));
extern int	lam_tr_init __ARGS((char *, float8));
extern void	lam_tr_msg __ARGS((int, double, int, int, int, int,
			MPI_Comm, MPI_Datatype, int, int, int, int, int));
extern int	lam_tr_off __ARGS((void));
extern int	lam_tr_on __ARGS((void));
extern void	lam_tr_startall __ARGS((int, MPI_Request *, double,
				double, double));
extern void	lam_tr_sub __ARGS((int, double, int));
extern double	lam_tr_timer __ARGS((void));
extern void	lam_tr_wrapend __ARGS((int));
extern int	lam_tr_wrapstart __ARGS((int));
extern int      lam_tr_buoy __ARGS((const char *, double));
extern int      lam_tr_coloron __ARGS((int, int, int, double));
extern int      lam_tr_coloroff __ARGS((double));
extern int      lam_tr_commname __ARGS((char *, int, double));
extern void     *lam_tv_load_type_defs(void);
extern int	lam_type_free __ARGS((MPI_Datatype));
extern void	lam_type_setdefaults __ARGS((MPI_Datatype));
extern int	lam_unpack __ARGS((char *, int, char *, int, MPI_Datatype));
extern int	lam_unpublish __ARGS((char *));
extern int      lam_updown_init __ARGS((void));
extern int      lam_updown_finalize __ARGS((void));
extern int      lam_updown_reset_thr_level __ARGS((void));
extern void	lam_unsyncsql __ARGS((int, int, int *, int *,
					int *, int *, int *, int *));
extern void	lam_mpi_unsyncsql __ARGS((int, int, int *, int *,
                                          int *, int *, int *, int *));
extern void	lam_win_errfatal __ARGS((MPI_Win *, int *, ...));
extern void	lam_yield __ARGS((void));

extern void	_m2l_ack __ARGS((struct nmsg *));
extern void	_m2l_fill __ARGS((struct _proc *, struct _proc *, int, int,
				struct nmsg *));
extern void	_m2l_fillsys __ARGS((int4, int4, int4, int4, int, int,
				struct nmsg *));
extern struct _proc *_m2l_getsource __ARGS((struct nmsg *));
extern int	_m2l_gettag __ARGS((struct nmsg *));
extern void	_m2l_tail __ARGS((struct nmsg *));
extern void	_mpi_req_add __ARGS((MPI_Request));
extern int	_mpi_req_advance __ARGS((void));
extern void	_mpi_req_blkclr __ARGS((void));
extern void	_mpi_req_blkset __ARGS((MPI_Request));
extern int	_mpi_req_build __ARGS((void *, int, MPI_Datatype, int, int,
				MPI_Comm, int, MPI_Request *));
extern int	_mpi_req_destroy __ARGS((MPI_Request *));
extern int	_mpi_req_end __ARGS((MPI_Request));
extern void	_mpi_req_get __ARGS((int, MPI_Request *));
extern int	_mpi_req_getn __ARGS((void));
extern void	_mpi_req_rem __ARGS((MPI_Request));
extern int	_mpi_req_start __ARGS((MPI_Request));
extern int	_rpi_c2c_addprocs __ARGS((void));
extern int	_rpi_c2c_advance __ARGS((MPI_Request, int));
extern int	_rpi_c2c_build __ARGS((MPI_Request));
extern int	_rpi_c2c_destroy __ARGS((MPI_Request));
extern int	_rpi_c2c_finalize __ARGS((struct _proc *));
extern char  	*_rpi_c2c_get_name __ARGS((void));
extern int	_rpi_c2c_init __ARGS((void));
extern int	_rpi_c2c_iprobe __ARGS((MPI_Request));
extern int	_rpi_c2c_start __ARGS((MPI_Request, MPI_Request));
extern int	_rpi_lamd_addprocs __ARGS((void));
extern int	_rpi_lamd_advance __ARGS((MPI_Request, int));
extern int	_rpi_lamd_build __ARGS((MPI_Request));
extern int	_rpi_lamd_destroy __ARGS((MPI_Request));
extern int	_rpi_lamd_finalize __ARGS((struct _proc *));
extern int	_rpi_lamd_init __ARGS((void));
extern int	_rpi_lamd_iprobe __ARGS((MPI_Request));
extern int	_rpi_lamd_start __ARGS((MPI_Request, MPI_Request));
extern int	_rpi_c2c_fastsend __ARGS((char *, int, MPI_Datatype,
    					int, int, MPI_Comm));
extern int	_rpi_c2c_fastrecv __ARGS((char *, int, MPI_Datatype, int,
    					int *, MPI_Comm, MPI_Status *, int *));
extern int	_rpi_c2c_alloc_mem __ARGS((MPI_Aint size, MPI_Info info,
					   void *baseptr));
extern int	_rpi_c2c_free_mem __ARGS((void *base));

#ifdef __cplusplus
}
#endif

/*
 * Fortran support
 */

#define LAM_MAXF77PREDEF	55	/* max # predef. F77 handles */
#define LAM_TYPETBLSIZE		256		/* initial handle table size */

/*
 * external variables
 */
extern int		lam_F_maxhdl;
extern int		lam_F_nhdl;
extern int		lam_F_sizehdlarray;
extern void		**lam_F_handles;
extern char		*lam_F_bottom;
extern void		*lam_F_argvnull;
extern void		*lam_F_argvsnull;
extern void		*lam_F_errorcodes_ignore;
extern void		*lam_F_status_ignore;
extern void		*lam_F_statuses_ignore;

/*
 * prototypes of LAM MPI library F77 internal functions
 */
#ifdef __cplusplus
extern "C" {
#endif

extern char		*lam_F2C_string __ARGS((char *, int));
extern int		lam_F2C_argv __ARGS((char *, int, char ***));
extern void		lam_F2C_stat __ARGS((int *, MPI_Status *));
extern void		lam_C2F_stat __ARGS((MPI_Status *, int *));
extern void		lam_C2F_string __ARGS((char *, char *, int));
extern int		lam_F_alloc_hdl __ARGS((void));
extern int		lam_F_find_hdl __ARGS((void *));
extern int		lam_F_make_hdl __ARGS((int *, int *, void *, int));
extern void		lam_F_free_hdl __ARGS((int));

#ifdef __cplusplus
}
#endif

#endif	/* _MPISYS_H */
