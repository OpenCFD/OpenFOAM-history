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
 * $Id: mpitrace.h,v 6.8 2003/07/05 21:54:06 jsquyres Exp $
 *
 *	Function:	- MPI tracing constants and templates
 */

#ifndef _MPITRACE_H
#define _MPITRACE_H

#include <lam_config.h>
#include <lam.h>
#include <app_mgmt.h>

/*
 * macros
 */
#define	LAM_S2US(time)		((int) ((time) * 1000000.0))

/*
 * constants
 */
#define	LAM_TRMAGIC	1279348022	/* LAM trace file magic number */
#define TRDTINITBUFSIZE	16		/* initial buffer size */
#define TRDPROGMAX	32		/* max length of program name */

#define TRDTCONTIG	((int4) 0)	/* contiguous */
#define TRDTVECTOR	((int4) 1)	/* vector */
#define TRDTHVECTOR	((int4) 2)	/* hvector */
#define TRDTINDEXED	((int4) 3)	/* indexed */
#define TRDTHINDEXED	((int4) 4)	/* hindexed */
#define TRDTSTRUCT	((int4) 5)	/* structure */

#define TRDTBYTE	((int4) 6)	/* byte */
#define TRDTCHAR	((int4) 7)	/* C char */
#define TRDTUCHAR	((int4) 8)	/* C unsigned character */
#define TRDTSHORT	((int4) 9)	/* C short */
#define TRDTUSHORT	((int4) 10)	/* C unsigned short */
#define TRDTINT		((int4) 11)	/* C int */
#define TRDTUINT	((int4) 12)	/* C unsigned integer */
#define TRDTFLOGICAL	((int4) 13)	/* F77 logical */
#define TRDTLONG	((int4) 14)	/* C long */
#define TRDTULONG	((int4) 15)	/* C unsigned long */
#define TRDTFLOAT	((int4) 16)	/* C float */
#define TRDTDOUBLE	((int4) 17)	/* C double */
#define TRDTFCOMPLEX	((int4) 18)	/* F77 complex */
#define TRDTUB		((int4) 19)	/* upper bound */
#define TRDTLB		((int4) 20)	/* lower bound */
#define TRDTPACKED	((int4) 21)	/* packed */
#define TRDT2INT	((int4) 22)	/* C 2 ints */
#define TRDTFLOATINT	((int4) 25)	/* C float-int */
#define TRDTDOUBLEINT	((int4) 26)	/* C double-int */
#define TRDTLONGINT	((int4) 27)	/* C long-int */
#define TRDTSHORTINT	((int4) 28)	/* C short-int */
#define TRDTLONGDOUBLE	((int4) 29)	/* C long double */
#define TRDTLONGDBLINT	((int4) 30)	/* C long double-int */
#define TRDTFINTEGER	((int4) 31)	/* F77 integer */
#define TRDTFINTEGER1   ((int4) 32) /* F77 integer*1 */
#define TRDTFINTEGER2   ((int4) 33) /* F77 integer*2 */
#define TRDTFINTEGER4   ((int4) 34) /* F77 integer*4 */
#define TRDTFINTEGER8   ((int4) 35) /* F77 integer*8 */
#define TRDTFCHARACTER	((int4) 36)	/* F77 character */
#define TRDTFREAL	((int4) 37)	/* F77 real */
#define TRDTFREAL4      ((int4) 38) /* F77 real*4 */
#define TRDTFREAL8      ((int4) 39) /* F77 real*8 */
#define TRDTFREAL16     ((int4) 40) /* F77 real*16 */
#define TRDTFDBLPREC	((int4) 41)	/* F77 double precision */
#define TRDTFDBLCOMPLEX	((int4) 42)	/* F77 double complex */
#define TRDTF2INTEGER	((int4) 43)	/* F77 2 integers */
#define TRDTF2REAL	((int4) 44)	/* F77 2 reals */
#define TRDTF2DBLPREC	((int4) 45)	/* F77 2 double precision */
#define TRDTLONGLONG	((int4) 46)	/* C long long */
#define TRDTULONGLONG	((int4) 47)	/* C unsigned long long  */
#define TRDTWCHAR	((int4) 48)	/* C wchar_t */

#define TRDTCXXCOMPLEX  ((int4) 49)     /* C++ complex<float> */
#define TRDTCXXDCOMPLEX ((int4) 50)     /* C++ complex<double> */
#define TRDTCXXLDCOMPLEX ((int4) 51)    /* C++ complex<long double> */
#define TRDTCXXBOOL     ((int4) 52)     /* C++ bool */

#define TRDTMAX		((int4) 53)	/* highest flat datatype */

/*
 * trace states
 */
#define TRDTRUN		0			/* running */
#define TRDTBLOCK	1			/* blocked */
#define TRDTSYSTEM	2			/* system overhead */

/*
 * structures
 */
struct trsrc {
	int4		trs_node;	/* node of process that made trace */
	int4		trs_pid;	/* pid of process that made trace */
	int4		trs_listno;	/* list number of the trace */
#define TRRUNTIME	0		/* runtime trace list */	
#define TRCOMM		-2		/* communicator trace list */
#define TRDTYPE		-3		/* datatype trace list */
#define TRONOFF		-4		/* tracing init/on/off trace list */
#define TRWORLD		-5		/* world trace list */
	int4		trs_pad;	/* for alignment */
};

struct trcid {
	int4		trc_cid;	/* cid */
	int4		trc_nlg;	/* size of local group */
	int4		trc_nrg;	/* size of remote group */
	int4		trc_pad;	/* for alignment */
};

struct trdtype {
	int4		trd_param0;
#define trd_dtype	trd_param0	/* datatype */
#define trd_disp	trd_param0	/* displacement (index/struct) */
#define trd_stride	trd_param0	/* stride (vector) */
	int4		trd_param1;
#define trd_count	trd_param1	/* # datatype entries */
#define trd_length	trd_param1	/* length of entry */
};

struct trrthdr {
	float8		trr_time;	/* trace time (sec) */
	int4		trr_type;	/* trace type */
#define TRTINIT		((int4) 0)	/* initialize */
#define TRTSUBCHG	((int4) 1)	/* substrate change */
#define TRTOUTPUT	((int4) 2)	/* output */
#define TRTINPUT	((int4) 3)	/* input */
#define TRTNOIO		((int4) 4)	/* no IO */
#define TRTRON		((int4) 5)	/* tracing on */
#define TRTROFF		((int4) 6)	/* tracing off */
#define TRBUOY		((int4) 7)	/* buoy */
#define TRCOLORON	((int4) 8)	/* color on */
#define TRCOLOROFF	((int4) 9)	/* color off */
#define TRCOMMNAME	((int4) 10)	/* communicator name */
	int4		trr_pad;	/* for alignment */
};

struct tronoff {
	int4		tro_trnum;	/* trace number */
	int4		tro_pad;	/* for alignment */
};

struct trinit {
	float8		tri_skew;		/* clock skew (sec) */
	char		tri_name[TRDPROGMAX];	/* application name */
};

struct trmsg {
	int4		trm_topfunc;	/* top-level function */
	int4		trm_wrapfunc;	/* wrapper function */
	int4            trm_syst;	/* trace time in system (usec) */
	int4		trm_blkt;	/* trace time blocked (usec) */
	int4		trm_peer;	/* peer rank */
	int4		trm_tag;	/* tag */
	int4		trm_cid;	/* context ID */
	int4		trm_dtype;	/* datatype label */
	int4		trm_count;	/* message count */
	int4		trm_mrank;	/* src rank matched in case of wcard */
	int4		trm_mtag;	/* tag matched in case of wcard */
	int4		trm_seqnum;	/* sequence number */
};

struct trxchg {
	int4		trx_topfunc;	/* top-level function */
	int4		trx_sub;	/* new substrate */
};

struct trbuoy {
  char trb_name[32];
};

struct trcolor {
  int4 trcol_red;
  int4 trcol_green;
  int4 trcol_blue;
  int4 trcol_pad;
};

struct trcommname {
  char trcomm_name[128];
  int4 trcomm_cid;
};

#ifdef __cplusplus
extern "C" {
#endif

extern void	format_proc __ARGS((int, int, int, int, int, char *));
extern void	format_cid __ARGS((int, char *));
extern void	format_tag __ARGS((int, char*));
extern void	format_dtype __ARGS((int, char *, int));
extern int mpitr_comget(int4 nodeid, int4 pid, int4 contextid, char **ptrcom);
extern void mpitr_comprint(int cid, char *trcom, int showgps);
extern void	mpitr_dataprint __ARGS((char *, char *, int, int, int));
extern void	print_dtype __ARGS((int, int, int, char *));
extern void	print_comm __ARGS((int, int, int, int));
extern int	mpitr_dtypeget __ARGS((int4, int4, int4, char **));
extern void mpitr_dtypeprint(int4 dtype, char *dtrbuf, int4 indent, 
                             char *pfix, char *outbuf);
extern int trdrain_mpi(int fd, struct _gps *world, int nworld, int keep, 
                       int flush_delay);

#ifdef __cplusplus
}
#endif

#endif	/* _MPITRACE_H */
