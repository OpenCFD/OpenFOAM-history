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
 * $Id: lamtrace.c,v 6.18 2003/09/11 20:12:40 jsquyres Exp $
 *
 *	Function:	- MPI trace generation
 *
 * Note: there are two levels of control for tracing.
 *
 * Enabled / disabled: This is controlled by the RTF_TRACE run-time
 * flag.  This controls whether *any* tracing information is saved.
 *
 * On / off: This is controlled by the RTF_SWITCH run-time flag.  This
 * only controls whether message traffic is traced.
 *
 * All four cases expressed in (RTF_TRACE, RTF_SWITCH) pairs:
 *
 * (0, 0): No tracing information is saved.
 * (0, 1): No tracing information is saved.
 * (1, 0): Only datatype and communicator information is traced.
 * (1, 1): Datatype, communicator, and message information is traced.
 *
 * The rationale behind this is that if you want to be able to only
 * trace a small portion of your program, then you need to *enable* it
 * at the beginning of the application (i.e., with a switch to mpirun,
 * which does some magic in MPI_INIT).  In this case, we need to trace
 * all datatype and communicator information, regardless of whether
 * you are saving message passing trace information or not.
 *
 * If we didn't always save datatype/communicator info (regardless of
 * on/off state), you can easily imagine a problematic scenario such
 * as the following:
 *
 * T=0 enable tracing
 * T=1 turn off tracing
 * T=2 create communicator 7
 * T=3 turn on tracing
 * T=4 send on communicator 7
 *
 * This would be a problem because the message trace would not be able
 * to be related to any [traced] communicator, and would result in
 * confusion.  Hence, if tracing is *enabled*, we always have to store
 * datatype and communicator tracing information, and conditionally
 * store message tracing information.
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <lam_config.h>
#include <app_mgmt.h>
#include <blktype.h>
#include <etc_misc.h>
#include <mpi.h>
#include <mpisys.h>
#include <mpitrace.h>
#include <net.h>
#include <portable.h>
#include <rpisys.h>
#include <t_types.h>
#include <terror.h>
#include <typical.h>
#include <trreq.h>

/*
 * macros
 */
#define DISP(e, d)	((((e) & 0xFFFF) << 16) | ((d) & 0xFFFF))

/*
 * static variables
 */
static char		*dtbuf = 0;		/* datatype buffer */
static int		dtbufsize = 0;		/* buffer size */
static int		dtbufrec = 0;		/* # buffer records */
static int		dtlabel = TRDTMAX + 1;	/* datatype label */
static int		trace_num = 0;		/* current trace number */
static int		cff_func = 0;		/* collective/fin function */
static double		cff_start;		/* collective/fin start time */

/*
 * local functions
 */
static void setfunctions(struct trmsg *pmsg, int reqtype);
static int checkdtbuf(struct trdtype **pbf, int *psz, int nrec);
static int flat_struct(MPI_Datatype dtype, struct trdtype **pbuff, int *psize);
static int flat_hindexed(MPI_Datatype dtype, struct trdtype **pbuff, 
			 int *psize, int fmt);
static int flat_hvector(MPI_Datatype dtype, struct trdtype **pbuff, 
			int *psize, int fmt);
static int flat_contig(MPI_Datatype dtype, struct trdtype **pbuff, int *psize);
static int flat_basic(MPI_Datatype dtype, struct trdtype **pbuff, int *psize);
static int flat_dtype(MPI_Datatype datatype, struct trdtype **pbuffer, 
		      int *prec);
static int mkflatdtype(MPI_Datatype dtype, int dtlabel, char **pbuf, 
		       int *psize);

/*
 *	lam_tr_comm
 *
 *	Function:	- register communicator trace
 *	Accepts:	- communicator
 *	Returns:	- 0 or LAMERROR
 */
int
lam_tr_comm(MPI_Comm comm)
{
	int		bufsize;		/* buffer size */
	int		size1;			/* group1 size */
	int		size2;			/* group2 size */
	int		i;			/* favourite index */
	char		*buf;			/* comm. buffer */
	struct trsrc	*psrc;			/* ptr src header */
	struct trcid    *pcid;			/* ptr cid data */
	struct _gps	*pgps;			/* ptr GPS info */
	struct _proc	*pproc;			/* ptr process */
	struct _proc	**p;			/* favourite pointer */

        /* Do nothing if tracing is not *enabled* (always record comm
           activity regardless of whether tracing is on or off). */

	if ((_kio.ki_rtf & RTF_TRACE) == 0)
          return 0;

/*
 * Create the flat communicator description.
 */
	size1 = comm->c_group->g_nprocs;
	size2 = LAM_IS_INTER(comm) ? comm->c_rgroup->g_nprocs : 0;
	bufsize = sizeof(struct trsrc) + sizeof(struct trcid)
			+ ((size1 + size2) * sizeof(struct _gps));

	buf = malloc((unsigned) bufsize);
	if (buf == 0) return(LAMERROR);
#if LAM_DISINFECT
	memset(buf, 0, bufsize);
#endif

	psrc = (struct trsrc *) buf;

	psrc->trs_node = getnodeid();
	psrc->trs_pid = lam_getpid();
	psrc->trs_listno = TRCOMM;

	pcid = (struct trcid *) (psrc + 1);
	
	pcid->trc_cid = comm->c_contextid;
	pcid->trc_nlg = size1;
	pcid->trc_nrg = size2;
/*
 * Fill the GPS information.
 */
	pgps = (struct _gps *) (pcid + 1);
	p = comm->c_group->g_procs;

	for (i = 0; i < size1; ++i, ++p, ++pgps) {
		pproc = *p;
		pgps->gps_node = pproc->p_gps.gps_node;
		pgps->gps_pid = pproc->p_gps.gps_pid;
		pgps->gps_idx = pproc->p_gps.gps_idx;
		pgps->gps_grank = pproc->p_gps.gps_grank;
	}

	p = (size2 > 0) ? comm->c_rgroup->g_procs : NULL;

	for (i = 0; i < size2; ++i, ++p, ++pgps) {
		pproc = *p;
		pgps->gps_node = pproc->p_gps.gps_node;
		pgps->gps_pid = pproc->p_gps.gps_pid;
		pgps->gps_idx = pproc->p_gps.gps_idx;
		pgps->gps_grank = pproc->p_gps.gps_grank;
	}
/*
 * Register the communicator trace.
 */
	mltoti4(buf, bufsize / sizeof(int));

	i = lam_rtrstore(LOCAL, TRCOMM,
			lam_myproc->p_gps.gps_pid, (char *) buf, bufsize);

	free(buf);
	
	return(i);
}

/*
 *	lam_tr_dtype
 *
 *	Function:	- register datatype trace
 *			- assign datatype label
 *	Accepts:	- datatype
 *	Returns:	- 0 or LAMERROR
 */
int
lam_tr_dtype(MPI_Datatype dtype)
{
	char		*buf;			/* datatype buffer */
	int		bufsize;		/* buffer size */

        /* Do nothing if tracing is not *enabled* (regardless of
           whether it is currently on or off). */

	if ((_kio.ki_rtf & RTF_TRACE) == 0)
          return 0;

/*
 * If datatype has a label, use it.
 */
	if (dtype->dt_label >= 0) return(0);
/*
 * Create and register flat datatype.
 */
	if (mkflatdtype(dtype, dtlabel, &buf, &bufsize)) return(LAMERROR);

	mltoti4(buf, bufsize / sizeof(int4));

	if (lam_rtrstore(LOCAL, TRDTYPE,
			lam_myproc->p_gps.gps_pid, buf, bufsize)) {
		
		free(buf);
		return(LAMERROR);
	}

	dtype->dt_label = dtlabel++;
	return(0);
}

/*
 *	lam_tr_dtypefree
 *
 *	Function:	- free a datatype entry
 *	Accepts:	- datatype
 */
void
lam_tr_dtypefree(MPI_Datatype dtype)
{
}

/*
 *	lam_tr_timer
 *
 *	Function:	- return timer value in trace-mode
 *	Returns:	- timer value or 0
 */
double
lam_tr_timer()
{
	if ((_kio.ki_rtf & RTF_TRON) == RTF_TRON) {
		return(ttime());
	} else {
		return((double) 0);
	}
}

/*
 *	lam_tr_off
 *
 *	Function:	- turn off message tracing
 *			- stores a run time trace and a on/off list trace
 *	Returns:	- previous trace mode or LAMERROR
 */
int
lam_tr_off()
{
	int		trmode;			/* previous mode */
	int		trsize;			/* trace size */
	struct trsrc	*psrc;			/* ptr src header */
	struct trrthdr  *ptrr;			/* ptr runtime header */
	struct	tronoff	*pto;			/* ptr on/off data */
	float8		time;			/* time tracing turned off */
	
	trmode = _kio.ki_rtf & RTF_TRON;
	_kio.ki_rtf &= ~RTF_TRSWITCH;

        /* If the previous mode was "on" (i.e., had *both* trace flags
           on (TRACE and TRSWITCH)), then we need to drop an "off"
           trace */

	if (trmode == RTF_TRON) {
		trsize = sizeof(struct trsrc) + sizeof(struct trrthdr)
				+ sizeof(struct tronoff);
		psrc = (struct trsrc *) lam_trpoint(trsize);
		
		psrc->trs_node = ltot(getnodeid());
		psrc->trs_pid = ltot(lam_getpid());
		psrc->trs_listno = ltot(TRONOFF);

		ptrr = (struct trrthdr *) (psrc + 1);
		
		ptrr->trr_type = ltot(TRTROFF);

		time = (float8) MPI_Wtime();
		ltotf8(&time, &(ptrr->trr_time));

		pto = (struct tronoff *) (ptrr + 1);

		pto->tro_trnum = ltot(trace_num);
		trace_num++;
		
		if (lam_rtrstore(LOCAL, TRONOFF, lam_myproc->p_gps.gps_pid,
					(char *) psrc, trsize)) {
			return(LAMERROR);
		}

		psrc->trs_listno = ltot(TRRUNTIME);
	}

	return(trmode);
}

/*
 *	lam_tr_on
 *
 *	Function:	- turn on message tracing
 *			- stores a run time trace and a on/off list trace
 *	Returns:	- previous trace mode or LAMERROR
 */
int
lam_tr_on()
{
	int		trmode;			/* previous mode */
	int		trsize;			/* trace size */
	struct trsrc	*psrc;			/* ptr src header */
	struct trrthdr  *ptrr;			/* ptr runtime header */
	struct tronoff	*pto;			/* ptr on/off data */
	float8		time;			/* time tracing turned off */
	
	trmode = _kio.ki_rtf & RTF_TRON;
	_kio.ki_rtf |= RTF_TRSWITCH;

        /* If the previous mode was "off" (i.e., had only TRACE), then
           we need to drop an "on" trace message. */

        if (trmode == RTF_TRACE) {
		trsize = sizeof(struct trsrc) + sizeof(struct trrthdr)
				+ sizeof(struct tronoff);
		psrc = (struct trsrc *) lam_trpoint(trsize);
		
		psrc->trs_node = ltot(getnodeid());
		psrc->trs_pid = ltot(lam_getpid());
		psrc->trs_listno = ltot(TRONOFF);

		ptrr = (struct trrthdr *) (psrc + 1);
		
		ptrr->trr_type = ltot(TRTRON);

		time = (float8) MPI_Wtime();
		ltotf8(&time, &(ptrr->trr_time));

		pto = (struct tronoff *) (ptrr + 1);

		pto->tro_trnum = ltot(trace_num);
		
		if (lam_rtrstore(LOCAL, TRONOFF, lam_myproc->p_gps.gps_pid,
					(char *) psrc, trsize)) {
			return(LAMERROR);
		}

		psrc->trs_listno = ltot(TRRUNTIME);
	}
	
	return(trmode);
}

/*
 *	lam_tr_init
 *
 *	Function:	- store the initialization trace
 *	Accepts:	- application name
 *			- clock skew (sec)
 *	Returns:	- 0 or LAMERROR
 */
int
lam_tr_init(char *name, float8 skew)
{
	struct trsrc	*psrc;			/* ptr src header */
	struct trrthdr  *ptrr;			/* ptr runtime header */
	struct trinit	*pinit;			/* ptr init trace */
        char            *p;             	/* favourite pointer */
	int		trsize;			/* trace size */
	float8		time;			/* time MPI process starts */
	
	if (_kio.ki_rtf & RTF_TRACE) {
		
		trsize = sizeof(struct trsrc) + sizeof(struct trrthdr)
				+ sizeof(struct trinit);
		
		psrc = (struct trsrc *) malloc(trsize);
		if (psrc == 0) return(LAMERROR);
		
		psrc->trs_node = ltot(getnodeid());
		psrc->trs_pid = ltot(lam_getpid());
		psrc->trs_listno = ltot(TRONOFF);

		ptrr = (struct trrthdr *) (psrc + 1);
		
		ptrr->trr_type = ltot(TRTINIT);

		time = (float8) MPI_Wtime();
		ltotf8(&time, &(ptrr->trr_time));

		pinit = (struct trinit *) (ptrr + 1);
		
		if (name) {
			p = strrchr(name, STRDIR);
			p = (p == 0) ? name : p + 1;
			lam_strncpy(pinit->tri_name, p, TRDPROGMAX);
		} else {
			pinit->tri_name[0] = 0;
		}

		ltotf8((float8 *) &skew, &(pinit->tri_skew));

		if (lam_rtrstore(LOCAL, TRONOFF, lam_myproc->p_gps.gps_pid,
					(char *) psrc, trsize)) {
		
			free((char *) psrc);
			return(LAMERROR);
		}

		free((char *) psrc);
	}

	return(0);
}

/*
 *	lam_tr_msg
 *
 *	Function:	- store a message trace
 *	Accepts:	- trace type
 *			- point in time (sec)
 *			- time in system overhead (usec)
 *			- time blocked (usec)
 *			- src/dest rank
 *			- tag
 *			- communicator
 *			- datatype
 *			- count
 *			- matching rank (in case of wildcard)
 *			- matching tag (in case of wildcard)
 *			- message sequence number
 *			- request type
 */
void
lam_tr_msg(int type, double time, int syst, int blkt, int peer, int tag,
	   MPI_Comm comm, MPI_Datatype dtype, int count, int mrank,
	   int mtag, int seqnum, int reqtype)
{
	struct trsrc	*psrc;			/* ptr src header */
	struct trrthdr  *ptrr;			/* ptr runtime header */
	struct trmsg	*pmsg;			/* ptr msg trace */
	int		trsize;			/* trace size */
	
	trsize = sizeof(struct trsrc) + sizeof(struct trrthdr)
				+ sizeof(struct trmsg);
		
	psrc = (struct trsrc *) lam_trpoint(trsize);

	psrc->trs_node = ltot(getnodeid());
	psrc->trs_pid = ltot(lam_getpid());
	psrc->trs_listno = ltot(TRRUNTIME);
	
	ptrr = (struct trrthdr *) (psrc + 1);
		
	ptrr->trr_type = ltot(type);
	ltotf8(&time, &(ptrr->trr_time));

	pmsg = (struct trmsg *) (ptrr + 1);

	setfunctions(pmsg, reqtype);
	pmsg->trm_syst = syst;
	pmsg->trm_blkt = blkt;
	pmsg->trm_peer = peer;
	pmsg->trm_tag = tag;
	pmsg->trm_cid = (comm) ? lam_mpi_coll2pt(comm->c_contextid) : 0;
	pmsg->trm_dtype = (dtype) ? dtype->dt_label : 0;
	pmsg->trm_count = count;
	pmsg->trm_mrank = mrank;
	pmsg->trm_mtag = mtag;
	pmsg->trm_seqnum = seqnum;

	mltoti4((int4 *) pmsg, sizeof(struct trmsg) / sizeof(int4));
}

/*
 *	lam_tr_sub
 *
 *	Function:	- store a message trace
 *	Accepts:	- top level function
 *			- trace time (sec)
 *			- substrate
 */
void
lam_tr_sub(int func, double time, int sub)
{
	struct trsrc	*psrc;			/* ptr src header */
	struct trrthdr  *ptrr;			/* ptr runtime header */
	struct trxchg	*psub;			/* ptr msg trace */
	int		trsize;			/* trace size */

	trsize = sizeof(struct trsrc) + sizeof(struct trrthdr)
				+ sizeof(struct trxchg);
		
	psrc = (struct trsrc *) lam_trpoint(trsize);

	psrc->trs_node = ltot(getnodeid());
	psrc->trs_pid = ltot(lam_getpid());
	psrc->trs_listno = ltot(TRRUNTIME);

	ptrr = (struct trrthdr *) (psrc + 1);
		
	ptrr->trr_type = ltot(TRTSUBCHG);
	ltotf8(&time, &(ptrr->trr_time));

	psub = (struct trxchg *) (ptrr + 1);
		
	psub->trx_topfunc = func;
	psub->trx_sub =  sub;

	mltoti4((int4 *) psub, sizeof(struct trxchg) / sizeof(int4));
}

/*
 *	lam_tr_startall
 *
 *	Function:	- store traces for MPI_Startall.
 *	Accepts:	- number of requests started
 *			- array of requests
 *			- start time of MPI_Startall
 *			- time requests are first advanced in MPI_Startall
 *			- finish time of MPI_Startall
 */
void
lam_tr_startall(int nreq, MPI_Request *preq, double startt, double advt,
		double finisht)
{
	MPI_Request	*p;			/* favourite pointer */
	int		i;			/* favourite index */
	int		trtype;			/* trace type */
	int		ulapse;			/* lapse time in usecs */
	double		lapse;			/* lapse time in secs */

	lapse = (nreq == 1) ? finisht - startt : (advt - startt) / nreq;
	ulapse = LAM_S2US(lapse);

	for (i = 0, p = preq; i < nreq; i++, p++) {

		if ((*p)->rq_rank == MPI_PROC_NULL) continue;
		
		trtype = ((*p)->rq_marks & LAM_RQFDEST) ? TRTNOIO : TRTOUTPUT;
		
		lam_tr_msg(trtype, startt, ulapse, 0, (*p)->rq_rank,
			(*p)->rq_tag, (*p)->rq_comm, (*p)->rq_dtype,
			(*p)->rq_count, 0, 0, (*p)->rq_seq,
			(*p)->rq_type);

		startt += lapse;
	}

	if (startt < finisht) {
		lam_tr_msg(TRTNOIO, startt, LAM_S2US(finisht - startt), 0,
		    -1, 0, (MPI_Comm) 0, (MPI_Datatype) 0, nreq, 0, 0, 0, -1);
	}
}

/*
 *	lam_tr_wrapstart
 *
 *	Function:	- generate trace to mark start of a wrapper function
 *	Accepts:	- wrapper function
 *	Returns:	- true iff this function should be traced
 */
int
lam_tr_wrapstart(int func)
{
	if (func == lam_getfunc() && ! lam_tr_incff()) {
		
		lam_tr_sub(func, MPI_Wtime(), TRDTSYSTEM);

		return(1);
	}
	else {
		return(0);
	}
}

/*
 *	lam_tr_wrapend
 *
 *	Function:	- generate trace to mark end of a top-level function
 *	Accepts:	- wrapper function
 */
void
lam_tr_wrapend(int func)
{
	if (func == lam_getfunc()) {

		lam_tr_sub(0, MPI_Wtime(), TRDTRUN);
	}
}

/*
 *	lam_tr_incff
 *
 *	Function:	- in a collective function or in MPI_Finalize?
 *	Returns:	- true iff caller is inside a collective function or
 *			  in MPI_Finalize
 */
int
lam_tr_incff()
{
	return(cff_func != 0);
}

/*
 *	lam_tr_cffstart
 *
 *	Function:	- start tracing for MPI collective func./MPI_Finalize
 *	Accepts:	- function
 */
void
lam_tr_cffstart(int func)
{
	if (cff_func == 0) {
		_kio.ki_blktime = 0.0;
		cff_func = func;
		cff_start = MPI_Wtime();
	}
}

/*
 *	lam_tr_cffend
 *
 *	Function:	- end tracing for MPI collective function/MPI_Finalize
 *	Accepts:	- function
 *			- root (-1 indicates none)
 *			- communicator (0 indicates none)
 *			- datatype (0 indicates none)
 *			- count (if any)
 */
void
lam_tr_cffend(int func, int root, MPI_Comm comm, MPI_Datatype dtype, 
	      int count)
{
	double		totaltime;		/* total time in function */
	
	if (func == cff_func) {
		
		cff_func = 0;
		totaltime = MPI_Wtime() - cff_start;

		lam_tr_msg(TRTNOIO, cff_start,
			LAM_S2US(totaltime - _kio.ki_blktime),
			LAM_S2US(_kio.ki_blktime),
			root, 0, comm, dtype, count, 0, 0, 0, 0);
	}
}

/*
 *	lam_nuketrace
 *
 *	Function:	- deallocate static trace information
 */
void
lam_nuketrace()
{
        dtlabel = TRDTMAX + 1;

	if (dtbuf) {
		free(dtbuf);
		dtbuf = 0;
	}

	dtbufsize = dtbufrec = 0;
	trace_num = 0;
}


/*
 *      lam_tr_buoy
 *
 *      Function:       - drops a TRBUOY into the trace sea
 *      Accepts:        - name and time for buoy
 */
int
lam_tr_buoy(const char *name, double t)
{
  struct trsrc *psrc;
  struct trrthdr *ptrr;
  struct trbuoy *pbuoy;
  int trsize;

  trsize = (sizeof(struct trsrc) +
	    sizeof(struct trrthdr) +
	    sizeof(struct trbuoy));
  
  psrc = (struct trsrc *) malloc(trsize);
  psrc->trs_node = ltot(getnodeid());
  psrc->trs_pid = ltot(lam_getpid());
  psrc->trs_listno = ltot(TRRUNTIME);
  
  /* advance to the trrthdr struct */

  ptrr = (struct trrthdr *) (psrc + 1);
  ptrr->trr_type = ltot(TRBUOY);
  ltotf8(&t, &(ptrr->trr_time));
  
  /* advance to the trbuoy struct */

  pbuoy = (struct trbuoy *) (ptrr + 1);
  memset(pbuoy->trb_name, 0, sizeof(pbuoy->trb_name));
  lam_strncpy(pbuoy->trb_name, name, 32);
  
  if (lam_rtrstore(LOCAL,
		   TRRUNTIME,
		   lam_myproc->p_gps.gps_pid,
		   (char*) psrc,
		   trsize)){
    free((char*) psrc);
    return LAMERROR;
  }

  free((char*) psrc);
  return 0;
}

/*
 *      lam_tr_coloron
 *
 *      Function:       - drops a TRCOLORON into the trace sea
 *      Accepts:        - red, green, blue values
 *                      - time
 */
int
lam_tr_coloron(int red, int green, int blue, double t)
{
  struct trsrc *psrc;
  struct trrthdr *ptrr;
  struct trcolor *pcolor;
  int trsize;

  trsize = (sizeof(struct trsrc) +
	    sizeof(struct trrthdr) +
	    sizeof(struct trcolor));
  
  psrc = (struct trsrc *) malloc(trsize);
  psrc->trs_node = ltot(getnodeid());
  psrc->trs_pid = ltot(lam_getpid());
  psrc->trs_listno = ltot(TRRUNTIME);
  
  /* advance to the trrthdr struct */

  ptrr = (struct trrthdr *) (psrc + 1);
  ptrr->trr_type = ltot(TRCOLORON);
  ltotf8(&t, &(ptrr->trr_time));
  
  /* advance to the trcolor struct */

  pcolor = (struct trcolor *) (ptrr + 1);
  LAM_ZERO_ME(*pcolor);
  pcolor->trcol_red = ltot(red);
  pcolor->trcol_green = ltot(green);
  pcolor->trcol_blue = ltot(blue);
  
  if (lam_rtrstore(LOCAL,
		   TRRUNTIME,
		   lam_myproc->p_gps.gps_pid,
		   (char*) psrc,
		   trsize)){
    free((char*) psrc);
    return LAMERROR;
  }

  free((char*) psrc);
  return 0;
}


/*
 *      lam_tr_coloroff
 *
 *      Function:       - drops a TRCOLOROFF into the trace sea
 *      Accepts:        - time
 */
int
lam_tr_coloroff(double t)
{
  struct trsrc *psrc;
  struct trrthdr *ptrr;
  int trsize;

  trsize = (sizeof(struct trsrc) +
	    sizeof(struct trrthdr));
  
  psrc = (struct trsrc *) malloc(trsize);
  psrc->trs_node = ltot(getnodeid());
  psrc->trs_pid = ltot(lam_getpid());
  psrc->trs_listno = ltot(TRRUNTIME);
  
  /* advance to the trrthdr struct */

  ptrr = (struct trrthdr *) (psrc + 1);
  ptrr->trr_type = ltot(TRCOLOROFF);
  ltotf8(&t, &(ptrr->trr_time));
  
  if (lam_rtrstore(LOCAL,
		   TRRUNTIME,
		   lam_myproc->p_gps.gps_pid,
		   (char*) psrc,
		   trsize)){
    free((char*) psrc);
    return LAMERROR;
  }

  free((char*) psrc);
  return 0;
}

/*
 *      lam_tr_commname
 *
 *      Function:       - drops a TRCOMMNAME into the trace sea
 *      Accepts:        - communicator name and cid, and time
 */
int
lam_tr_commname(char *name, int cid, double t)
{
  struct trsrc *psrc;
  struct trrthdr *ptrr;
  struct trcommname *pcomm;
  int trsize;

  trsize = (sizeof(struct trsrc) +
	    sizeof(struct trrthdr) +
	    sizeof(struct trcommname));
  
  psrc = (struct trsrc *) malloc(trsize);
  psrc->trs_node = ltot(getnodeid());
  psrc->trs_pid = ltot(lam_getpid());
  psrc->trs_listno = ltot(TRRUNTIME);
  
  /* advance to the trrthdr struct */

  ptrr = (struct trrthdr *) (psrc + 1);
  ptrr->trr_type = ltot(TRCOMMNAME);
  ltotf8(&t, &(ptrr->trr_time));
  
  /* advance to the trbuoy struct */

  pcomm = (struct trcommname *) (ptrr + 1);
  memset(pcomm->trcomm_name, 0, sizeof(pcomm->trcomm_name));
  lam_strncpy(pcomm->trcomm_name, name, 128);
  pcomm->trcomm_cid = ltot(cid);
  
  if (lam_rtrstore(LOCAL,
		   TRRUNTIME,
		   lam_myproc->p_gps.gps_pid,
		   (char*) psrc,
		   trsize)){
    free((char*) psrc);
    return LAMERROR;
  }

  free((char*) psrc);
  return 0;
}

/*
 *	mkflatdtype
 *
 *	Function:	- make a flat datatype representation
 *			- uses depth-first navigation
 *
 *	Format:		- datatype label; length (bytes); followed by
 *			  array of datatype records (2 * int4):
 *
 *			- { C ; # } [subtype]
 *			- { V ; # } { st ; l } [subtype]
 *			- { I ; # } { d ; l } { d ; l } ... [subtype]
 *			- { S ; # } { d ; l } [subtype] { d ; l } [subtype] ...
 *
 *	Accepts:	- datatype
 *			- datatype label
 *			- ptr buffer (returned value)
 *			- ptr buffer size (returned value)
 *
 *	Returns:	- 0 or LAMERROR
 */
static int
mkflatdtype(MPI_Datatype dtype, int dtlabel, char **pbuf, int *psize)
{
	int		nrec;			/* # records available */
	struct trsrc    *psrc;			/* ptr src header */
	struct trdtype  *pdhdr;			/* ptr trace datatype header */
	struct trdtype	*p;			/* favourite pointer */
/*
 * Initialize.
 */
	if (dtbuf == 0) {
		dtbufrec = TRDTINITBUFSIZE;
		dtbufsize = sizeof(struct trsrc) + sizeof(struct trdtype) +
				(dtbufrec * sizeof(struct trdtype));

		dtbuf = malloc((unsigned) dtbufsize);
		if (dtbuf == 0) return(LAMERROR);
#if LAM_DISINFECT
		memset(dtbuf, 0, dtbufsize);
#endif
	}

	psrc = (struct trsrc *) dtbuf;

	psrc->trs_node = getnodeid();
	psrc->trs_pid = lam_getpid();
	psrc->trs_listno = TRDTYPE;

	pdhdr = (struct trdtype *) (psrc + 1);
	
	pdhdr->trd_dtype = dtlabel;

/*
 * Recursively (depth-first) flatten datatype.
 */
	p = (struct trdtype *) (pdhdr + 1);
	nrec = dtbufrec;

	if (flat_dtype(dtype, &p, &nrec)) return(LAMERROR);

	*pbuf = dtbuf;
	*psize = ((long) p) - ((long) dtbuf);
	pdhdr = (struct trdtype *) (dtbuf + sizeof(struct trsrc));
	pdhdr->trd_length = *psize - sizeof(struct trsrc);

	return(0);
}

/*
 *	flat_dtype
 *
 *	Function:	- flatten current datatype
 *			- recursive function
 *	Accepts:	- datatype
 *			- ptr record buffer (value modified)
 *			- ptr # buffer records left (value modified)
 *	Returns:	- 0 or LAMERROR
 */
static int
flat_dtype(MPI_Datatype datatype, struct trdtype **pbuffer, int *prec)
{
	int		ret;			/* return value */
	int		fmt;			/* datatype format */

	fmt = datatype->dt_format;

	switch(fmt) {

	case LAM_DTBASIC:
		ret = flat_basic(datatype, pbuffer, prec);
		break;

	case LAM_DTCONTIG:
		ret = flat_contig(datatype, pbuffer, prec);
		break;

	case LAM_DTVECTOR:
		ret = flat_hvector(datatype, pbuffer, prec, TRDTVECTOR);
		break;

	case LAM_DTHVECTOR:
	case LAM_DTHVECTORCREAT:
		ret = flat_hvector(datatype, pbuffer, prec, TRDTHVECTOR);
		break;

	case LAM_DTINDEXED:
		ret = flat_hindexed(datatype, pbuffer, prec, TRDTINDEXED);
		break;

	case LAM_DTINDEXEDBLK:
		ret = LAMERROR;
		break;

	case LAM_DTHINDEXED:
	case LAM_DTHINDEXEDCREAT:
		ret = flat_hindexed(datatype, pbuffer, prec, TRDTHINDEXED);
		break;

	case LAM_DTSTRUCT:
	case LAM_DTSTRUCTCREAT:
	case LAM_DTSUBARRAY:
	case LAM_DTDARRAY:
		ret = flat_struct(datatype, pbuffer, prec);
		break;

	default:
		ret = LAMERROR;
		break;
	}

	return(ret);
}

/*
 *	flat_basic
 *
 *	Function:	- add basic datatype record to buffer
 *			- expand buffer if necessary
 *	Accepts:	- datatype
 *			- ptr record buffer (value modified)
 *			- ptr buffer size (value modified)
 *	Returns:	- 0 or LAMERROR
 */
static int
flat_basic(MPI_Datatype dtype, struct trdtype **pbuff, int *psize)
{
	int4		type;
/*
 * Make sure datatype buffer has at least 1 extra record.
 */
	if (checkdtbuf(pbuff, psize, 1)) return(LAMERROR);
/*
 * Eliminate any dups.
 */
	while (dtype->dt_flags & LAM_DTISDUP)
		dtype = (MPI_Datatype)dtype->dt_uargs;
/*
 * Add the basic datatype record.
 */
	if (dtype == MPI_BYTE) {
		type = TRDTBYTE;
	} else if (dtype == MPI_PACKED) {
		type = TRDTPACKED;
	} else if (dtype == MPI_UB) {
		type = TRDTUB;
	} else if (dtype == MPI_LB) {
		type = TRDTLB;
	} else if (dtype == MPI_CHAR) {
		type = TRDTCHAR;
	} else if (dtype == MPI_UNSIGNED_CHAR) {
		type = TRDTUCHAR;
	} else if (dtype == MPI_WCHAR) {
		type = TRDTWCHAR;
	} else if (dtype == MPI_SHORT) {
		type = TRDTSHORT;
	} else if (dtype == MPI_UNSIGNED_SHORT) {
		type = TRDTUSHORT;
	} else if (dtype == MPI_INT) {
		type = TRDTINT;
	} else if (dtype == MPI_UNSIGNED) {
		type = TRDTUINT;
	} else if (dtype == MPI_LONG) {
		type = TRDTLONG;
	} else if (dtype == MPI_LONG_LONG_INT) {
		type = TRDTLONGLONG;
	} else if (dtype == MPI_UNSIGNED_LONG) {
		type = TRDTULONG;
	} else if (dtype == MPI_UNSIGNED_LONG_LONG) {
		type = TRDTULONGLONG;
	} else if (dtype == MPI_FLOAT) {
		type = TRDTFLOAT;
	} else if (dtype == MPI_DOUBLE) {
		type = TRDTDOUBLE;
	} else if (dtype == MPI_2INT) {
		type = TRDT2INT;
	} else if (dtype == MPI_FLOAT_INT) {
		type = TRDTFLOATINT;
	} else if (dtype == MPI_DOUBLE_INT) {
		type = TRDTDOUBLEINT;
	} else if (dtype == MPI_LONG_DOUBLE_INT) {
		type = TRDTLONGDBLINT;
	} else if (dtype == MPI_LONG_INT) {
		type = TRDTLONGINT;
	} else if (dtype == MPI_SHORT_INT) {
		type = TRDTSHORTINT;
	} else if (dtype == MPI_LONG_DOUBLE) {
		type = TRDTLONGDOUBLE;
	} else if (dtype == MPI_F_LOGICAL) {
		type = TRDTFLOGICAL;
	} else if (dtype == MPI_F_INTEGER) {
		type = TRDTFINTEGER;
	} else if (dtype == MPI_F_CHARACTER) {
		type = TRDTFCHARACTER;
	} else if (dtype == MPI_F_REAL) {
		type = TRDTFREAL;
	} else if (dtype == MPI_F_DOUBLE_PRECISION) {
		type = TRDTFDBLPREC;
	} else if (dtype == MPI_F_COMPLEX) {
		type = TRDTFCOMPLEX;
	} else if (dtype == MPI_F_DOUBLE_COMPLEX) {
		type = TRDTFDBLCOMPLEX;
	} else if (dtype == MPI_F_2INTEGER) {
		type = TRDTF2INTEGER;
	} else if (dtype == MPI_F_2REAL) {
		type = TRDTF2REAL;
	} else if (dtype == MPI_F_2DOUBLE_PRECISION) {
		type = TRDTF2DBLPREC;
	} else if (dtype == MPI_CXX_COMPLEX) {
	  type = TRDTCXXCOMPLEX;
	} else if (dtype == MPI_CXX_DOUBLE_COMPLEX) {
	  type = TRDTCXXDCOMPLEX;
	} else if (dtype == MPI_CXX_LONG_DOUBLE_COMPLEX) {
	  type = TRDTCXXLDCOMPLEX;
	} else if (dtype == MPI_CXX_BOOL) {
	  type = TRDTCXXBOOL;
	} else {
		return(LAMERROR);
	}

	(*pbuff)->trd_dtype = type;
	(*pbuff)->trd_count = 1;

	++(*pbuff);
	--(*psize);

	return(0);
}

/*
 *	flat_contig
 *
 *	Function:	- add contiguous datatype record to buffer
 *			- expand buffer if necessary
 *	Accepts:	- datatype
 *			- ptr record buffer (value modified)
 *			- ptr buffer size (value modified)
 *	Returns:	- 0 or LAMERROR
 */
static int
flat_contig(MPI_Datatype dtype, struct trdtype **pbuff, int *psize)
{
/*
 * Make sure datatype buffer has at least 1 extra record.
 */
	if (checkdtbuf(pbuff, psize, 1)) return(LAMERROR);
/*
 * Add contiguous record and recurse.
 */
	(*pbuff)->trd_dtype = TRDTCONTIG;
	(*pbuff)->trd_count = dtype->dt_count;

	++(*pbuff);
	--(*psize);

	return(flat_dtype(dtype->dt_dtype, pbuff, psize));
}

/*
 *	flat_hvector
 *
 *	Function:	- add vector or hvector datatype record to buffer
 *			- expand buffer if necessary
 *	Accepts:	- datatype
 *			- ptr record buffer (value modified)
 *			- ptr buffer size (value modified)
 *			- datatype format
 *	Returns:	- 0 or LAMERROR
 */
static int
flat_hvector(MPI_Datatype dtype, struct trdtype **pbuff, int *psize, 
	     int fmt)
{
	int		extent;			/* datatype extent */
/*
 * Make sure datatype buffer has at least 2 extra records.
 */
	if (checkdtbuf(pbuff, psize, 2)) return(LAMERROR);
/*
 * Add the 2 vector records and recurse.
 */
	(*pbuff)->trd_dtype = fmt;
	(*pbuff)->trd_count = dtype->dt_count;

	extent = dtype->dt_dtype->dt_upper - dtype->dt_dtype->dt_lower;

	++(*pbuff);
	(*pbuff)->trd_stride = (fmt == TRDTHVECTOR) ? dtype->dt_stride :
					DISP(extent, dtype->dt_stride);

	(*pbuff)->trd_length = dtype->dt_length;

	++(*pbuff);
	(*psize) -= 2;

	return(flat_dtype(dtype->dt_dtype, pbuff, psize));
}

/*
 *	flat_hindexed
 *
 *	Function:	- add indexed or hindexed datatype record to buffer
 *			- expand buffer if necessary
 *	Accepts:	- datatype
 *			- ptr record buffer (value modified)
 *			- ptr buffer size (value modified)
 *			- datatype format
 *	Returns:	- 0 or LAMERROR
 */
static int
flat_hindexed(MPI_Datatype dtype, struct trdtype **pbuff, int *psize, 
	      int fmt)
{
	int		i;			/* favourite index */
	int		extent;			/* datatype extent */
/*
 * Make sure datatype buffer has at least (count + 1) extra records.
 */
	if (checkdtbuf(pbuff, psize, dtype->dt_count + 1)) return(LAMERROR);
/*
 * Add the indexed records and recurse.
 */
	(*pbuff)->trd_dtype = fmt;
	(*pbuff)->trd_count = dtype->dt_count;

	++(*pbuff);
	--(*psize);

	extent = dtype->dt_dtype->dt_upper - dtype->dt_dtype->dt_lower;

	for (i = 0; i < dtype->dt_count; ++i) {
		(*pbuff)->trd_disp = (fmt == TRDTHINDEXED) ?
					dtype->dt_disps[i] :
					DISP(extent, dtype->dt_disps[i]);
		(*pbuff)->trd_length = dtype->dt_lengths[i];
		++(*pbuff);
	}

	(*psize) -= dtype->dt_count;

	return(flat_dtype(dtype->dt_dtype, pbuff, psize));
}

/*
 *	flat_struct
 *
 *	Function:	- add struct datatype record to buffer
 *			- expand buffer if necessary
 *	Accepts:	- datatype
 *			- ptr record buffer (value modified)
 *			- ptr buffer size (value modified)
 *	Returns:	- 0 or LAMERROR
 */
static int
flat_struct(MPI_Datatype dtype, struct trdtype **pbuff, int *psize)
{
	int		i;			/* favourite index */
/*
 * Make sure datatype buffer has at least 1 extra record.
 */
	if (checkdtbuf(pbuff, psize, 1)) return(LAMERROR);
/*
 * Add each struct record and recurse.
 */
	(*pbuff)->trd_dtype = TRDTSTRUCT;
	(*pbuff)->trd_count = dtype->dt_count;

	++(*pbuff);
	--(*psize);

	for (i = 0; i < dtype->dt_count; ++i) {

		if (checkdtbuf(pbuff, psize, 1)) return(LAMERROR);

		(*pbuff)->trd_disp = dtype->dt_disps[i];
		(*pbuff)->trd_length = dtype->dt_lengths[i];

		++(*pbuff);
		--(*psize);

		if (flat_dtype(dtype->dt_dtypes[i], pbuff, psize)) {
			return(LAMERROR);
		}
	}

	return(0);
}

/*
 *	checkdtbuf
 *
 *	Function:	- check if datatype buffer has enough spare records
 *			- if not, expand it and return new buffer info
 *			- buffer has always an extra space for trace header
 *			- buffer pointer may change in expansion
 *			- uses static buffer and size information
 *	Accepts:	- ptr buffer (value modified)
 *			- ptr size (value modified)
 *			- # records needed
 *	Returns:	- 0 or LAMERROR
 */
static int
checkdtbuf(struct trdtype **pbf, int *psz, int nrec)
{
	char		*newbuf;
	int		newsize;
	int		newrec;
	int		index;
	int		extra;
	int		headsize;	/* size of src and first record */
	
	if (*psz >= nrec) return(0);
/*
 * Compute new buffer size.
 */
	extra = (dtbufrec < nrec) ? nrec : dtbufrec;
	newrec = dtbufrec + extra;
	headsize = sizeof(struct trsrc) + sizeof(struct trdtype);
	newsize = headsize + (newrec * sizeof(struct trdtype));
/*
 * Compute current buffer index.
 */
	index = (((char *) *pbf) - dtbuf - headsize) / sizeof(struct trdtype);
/*
 * Expand buffer.
 */
	newbuf = realloc(dtbuf, (unsigned) newsize);
	if (newbuf == 0) return(LAMERROR);

	dtbuf = newbuf;
	dtbufrec = newrec;
	dtbufsize = newsize;
/*
 * Set record buffer info.
 */
	*psz += extra;
	*pbf = (struct trdtype *) (dtbuf + headsize
					+ (index * sizeof(struct trdtype)));
	return(0);
}

/*
 *	setfunctions
 *
 *	Function:	- set blocking functions for a request
 *	Accepts:	- message trace
 *			- request type
 */
static void
setfunctions(struct trmsg *pmsg, int reqtype)
{
	int		topfunc;

	topfunc = lam_getfunc();

	switch (topfunc) {

	case BLKMPISTART:
	case BLKMPISTARTALL:
	case BLKMPIWAIT:
	case BLKMPIWAITALL:
	case BLKMPIWAITANY:
	case BLKMPIWAITSOME:
	case BLKMPITEST:
	case BLKMPITESTALL:
	case BLKMPITESTANY:
	case BLKMPITESTSOME:

		pmsg->trm_wrapfunc = topfunc;
		
		switch (reqtype) {

		case LAM_RQIRECV: pmsg->trm_topfunc = BLKMPIIRECV; break;

		case LAM_RQISEND: pmsg->trm_topfunc = BLKMPIISEND; break;

		case LAM_RQISSEND: pmsg->trm_topfunc = BLKMPIISSEND; break;

		case LAM_RQIRSEND: pmsg->trm_topfunc = BLKMPIIRSEND; break;

		case LAM_RQIBSEND:
		case LAM_RQIFAKE: pmsg->trm_topfunc = BLKMPIIBSEND; break;

		default:
			pmsg->trm_wrapfunc = 0;
			pmsg->trm_topfunc = topfunc;
		}
		
		break;

	case BLKMPISENDRECV:
	case BLKMPISENDRECVREP:
		
		pmsg->trm_wrapfunc = topfunc;

		switch (reqtype) {
		
		case LAM_RQIRECV: pmsg->trm_topfunc = BLKMPIIRECV; break;

		case LAM_RQISEND: pmsg->trm_topfunc = BLKMPISEND; break;

		default: 
			pmsg->trm_wrapfunc = 0;
			pmsg->trm_topfunc = topfunc;

		}
		break;

	default:
		pmsg->trm_wrapfunc = 0;
		pmsg->trm_topfunc = topfunc;
	}
}
