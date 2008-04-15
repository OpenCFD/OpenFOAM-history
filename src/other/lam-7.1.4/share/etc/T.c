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
 * $Id: T.c,v 6.8 2003/07/05 21:46:23 jsquyres Exp $
 * 
 *	Function:	- f2c F77 interface for libt functions
 */

#include <lam_config.h>

#include <string.h>
#include <stdlib.h>

#include <freq.h>
#include <rreq.h>
#include <net.h>
#include <portable.h>
#include <t_types.h>
#include <terror.h>

/*
 * external functions
 */
extern char		*f2cstring();

/*
 * F77/C interface
 */
#if LAM_FORTRANNOUNDERSCORE
#define fttime_		fttime
#define igntp_		igntp
#define ignall_		ignall
#define igncmp_		igncmp
#define igndid_		igndid
#define igndtp_		igndtp
#define ignjon_		ignjon
#define ignotb_		ignotb
#define igorgn_		igorgn
#define igrtp_		igrtp
#define i2ltot_		i2ltot
#define i2mltt_		i2mltt
#define i2mttl_		i2mttl
#define i2ttol_		i2ttol
#define i4ltot_		i4ltot
#define i4mltt_		i4mltt
#define i4mttl_		i4mttl
#define i4ttol_		i4ttol
#define f4ltot_		f4ltot
#define f4mltt_		f4mltt
#define f4mttl_		f4mttl
#define f4ttol_		f4ttol
#define f8ltot_		f8ltot
#define f8mltt_		f8mltt
#define f8mttl_		f8mttl
#define f8ttol_		f8ttol
#define gall_		gall
#define gcomps_		gcomps
#define gjones_		gjones
#define gnodes_		gnodes
#define gotbs_		gotbs
#define kentr_		kentr
#define kxit_		kxit
#define lamf_rfclose_	lamf_rfclose
#define lamf_rfread_	lamf_rfread
#define lamf_rfwrite_	lamf_rfwrite
#define lamf_rfopen_	lamf_rfopen
#define nsnd_		nsnd
#define nrcv_		nrcv
#define nprob_		nprob
#define tsnd_		tsnd
#define trcv_		trcv
#define trror_		trror
#define psndo_		psndo
#define psndc_		psndc
#define psnd_		psnd
#define prcvo_		prcvo
#define prcvc_		prcvc
#define prcv_		prcv
#elif LAM_FORTRANDOUBLEUNDERSCORE
#define lamf_rfclose_	lamf_rfclose__
#define lamf_rfread_	lamf_rfread__
#define lamf_rfwrite_	lamf_rfwrite__
#define lamf_rfopen_	lamf_rfopen__
#endif

/*
 * These are (unsupported) functions, so it's not too important to
 * protoype them.  But for the sake of eliminating compiler
 * warnings...
 */
double fttime_(void);
int4 igntp_(int4 *tp, int4 *msk);
int4 ignall_(void);
int4 igncmp_(void);
int4 igndid_(void);
int4 igndtp_(void);
int4 ignjon_(void);
int4 ignotb_(void);
int4 igorgn_(void);
int4 igrtp_(int4 *nd);
void i2ltot_(int2 *l, int2 *t);
void i2mltt_(int2 *a, int *n);
void i2mttl_(int2 *a, int *n);
void i2ttol_(int2 *t, int2 *l);
void i4ltot_(int4 *l, int4 *t);
void i4mltt_(int4 *a, int *n);
void i4mttl_(int4 *a, int *n);
void i4ttol_(int4 *t, int4 *l);
void f4ltot_(float4 *l, float4 *t);
void f4mltt_(float4 *a, int *n);
void f4mttl_(float4 *a, int *n);
void f4ttol_(float4 *t, float4 *l);
void f8ltot_(float8 *l, float8 *t);
void f8mltt_(float8 *a, int *n);
void f8mttl_(float8 *a, int *n);
void f8ttol_(float8 *t, float8 *l);
void gall_(int4 *n, int4 *nn, int4 *ie);
void gcomps_(int4 *n, int4 *nn, int4 *ie);
void gjones_(int4 *n, int4 *nn, int4 *ie);
void gnodes_(int4 *n, int4 *nn, int4 *tp, int4 *msk, int4 *ie);
void gotbs_(int4 *n, int4 *nn, int4 *ie);
void kxit_(int *er);
void lamf_rfclose_(int *ud, int *ie);
void lamf_rfread_(int *ud, char *b, int *i, int *o, int *ie);
void lamf_rfwrite_(int *ud, char *b, int *i, int *o, int *ie);


void trror_(char *pstr, unsigned int lstr);
void lamf_rfopen_(int *udesc, char *file_name, int *flags, int *modes, 
                  int *ierror, unsigned int lstr);
void kentr_(char *name, int *priority, int *ierror, unsigned int lstr);
void nsnd_(int *nnode, int *nevent, int *ntype, int *nlength, int *nflags, 
           int *ndata, int *ndsize, char *nmsg, int *ierror);
void nrcv_(int *nevent, int *ntype, int *nlength, int *nflags, int *ndata, 
           int *ndsize, char *nmsg, int *ierror);
void nprob_(int *nevent, int *ntype, int *nlength, int *nflags, int *nready, 
            int *ierror);
void tsnd_(int *nnode, int *nevent, int *ntype, int *nlength, int *nflags, 
           int *ndata, int *ndsize, char *nmsg, int *ierror);
void trcv_(int *nevent, int *ntype, int *nlength, int *nflags, int *ndata, 
           int *ndsize, char *nmsg, int *ierror);
void psndo_(int4 *pnode, int4* pevent, int4* ptype, int4* ierror);
void psndc_(int4 *pnode, int4 *pevent, int4 *ptype, int4 *ierror);
void psnd_(int4 *pnode, int4 *pevent, int4 *ptype, int4 *plength, 
           int4 *pflags, int4 *pdata, int4 *pdsize, char *pmsg, int4 *ierror);
void prcvo_(int *pevent, int *ptype, int *ierror);
void prcvc_(int4* pevent, int4* ptype, int4* ierror);
void prcv_(int4 *pevent, int4 *ptype, int4 *plength, int4 *pflags, 
           int4 *pdata, int4 *pdsize, char *pmsg, int4 *ierror);


/*
 * simple wrappers
 */
double fttime_(void) { return(ttime()); }
int4 igntp_(int4 *tp, int4 *msk) { return(getntype(*tp, *msk)); }
int4 ignall_(void) { return(getnall()); }
int4 igncmp_(void) { return(getncomp()); }
int4 igndid_(void) { return(getnodeid()); }
int4 igndtp_(void) { return(getnodetype()); }
int4 ignjon_(void) { return(getnjones()); }
int4 ignotb_(void) { return(getnotb()); }
int4 igorgn_(void) { return(getorigin()); }
int4 igrtp_(int4 *nd) { return(getrtype(*nd)); }
void i2ltot_(int2 *l, int2 *t) { ltoti2(l, t); }
void i2mltt_(int2 *a, int *n) { mltoti2(a, *n); }
void i2mttl_(int2 *a, int *n) { mttoli2(a, *n); }
void i2ttol_(int2 *t, int2 *l) { ttoli2(t, l); }
void i4ltot_(int4 *l, int4 *t) { ltoti4(l, t); }
void i4mltt_(int4 *a, int *n) { mltoti4(a, *n); }
void i4mttl_(int4 *a, int *n) { mttoli4(a, *n); }
void i4ttol_(int4 *t, int4 *l){ ttoli4(t, l); }
void f4ltot_(float4 *l, float4 *t) { ltotf4(l, t); }
void f4mltt_(float4 *a, int *n) { mltotf4(a, *n); }
void f4mttl_(float4 *a, int *n) { mttolf4(a, *n); }
void f4ttol_(float4 *t, float4 *l) { ttolf4(t, l); }
void f8ltot_(float8 *l, float8 *t) { ltotf8(l, t); }
void f8mltt_(float8 *a, int *n) { mltotf8(a, *n); }
void f8mttl_(float8 *a, int *n) { mttolf8(a, *n); }
void f8ttol_(float8 *t, float8 *l) { ttolf8(t, l); }
void gall_(int4 *n, int4 *nn, int4 *ie) 
		{ *ie = getall(n, *nn) ? errno : 0; }
void gcomps_(int4 *n, int4 *nn, int4 *ie) 
		{ *ie = getcomps(n, *nn) ? errno : 0; }
void gjones_(int4 *n, int4 *nn, int4 *ie)
		{ *ie = getjones(n, *nn) ? errno : 0; }
void gnodes_(int4 *n, int4 *nn, int4 *tp, int4 *msk, int4 *ie)
		{ *ie = getnodes(n, *nn, *tp, *msk) ? errno : 0; }
void gotbs_(int4 *n, int4 *nn, int4 *ie)
		{ *ie = getotbs(n, *nn) ? errno : 0; }
void kxit_(int *er) { kexit(*er); }
void lamf_rfclose_(int *ud, int *ie) { *ie = lam_rfclose(*ud) ? errno : 0; }
void lamf_rfread_(int *ud, char *b, int *i, int *o, int *ie)
		{ *ie = (*o = lam_rfread(*ud, b, *i)) < 0 ? errno : 0; }
void lamf_rfwrite_(int *ud, char *b, int *i, int *o, int *ie)
		{ *ie = (*o = lam_rfwrite(*ud, b, *i)) < 0 ? errno : 0; }

/*
 * thicker wrappers
 */
void
trror_(char *pstr, unsigned int lstr)
{
	char		*cstr;

	cstr = f2cstring(pstr, lstr);
	if (cstr) {
		terror(cstr);
		free(cstr);
	}
}


/*
 * int			*udesc;		file descriptor (returned) 
 * char			*file_name;	complete node:file spec
 * int			*flags; 	open flags
 * int			*modes;		modes for O_CREAT
 * int			*ierror;	return error code
 * unsigned int		lstr;		file_name length
 */
void
lamf_rfopen_(int *udesc, char *file_name, int *flags, int *modes, 
	     int *ierror, unsigned int lstr)
{
	char		*cstr;

	if ((cstr = f2cstring(file_name, lstr)) == 0) {
		*ierror = errno;
		return;
	}

	if ((*udesc = lam_rfopen(cstr, *flags, *modes)) < 0) {
		*ierror = errno;
	} else {
		*ierror = 0;
	}

	free(cstr);
}


/*
 * char			*name;		process name
 * int			*priority;	process priority
 * int			*ierror;	0 or error code
 * unsigned int		lstr;
 */
void
kentr_(char *name, int *priority, int *ierror, unsigned int lstr)
{
	char		*cstr;

	if ((cstr = f2cstring(name, lstr)) == 0) {
		*ierror = errno;
		return;
	}

	if (kenter(cstr, *priority)) {
		*ierror = errno;
	} else {
		*ierror = 0;
	}

	free(cstr);
}


void
nsnd_(int *nnode, int *nevent, int *ntype, int *nlength, int *nflags, 
      int *ndata, int *ndsize, char *nmsg, int *ierror)
{
	struct nmsg	nhead;

	nhead.nh_node = *nnode;
	nhead.nh_event = *nevent;
	nhead.nh_type = *ntype;
	nhead.nh_length = *nlength;
	nhead.nh_flags = *nflags;
	nhead.nh_msg = nmsg;

	memcpy((char *) nhead.nh_data, (char *) ndata,
		(*ndsize > sizeof(nhead.nh_data))
               	? sizeof(nhead.nh_data) : *ndsize);

	if (nsend(&nhead)) {
		*ierror = errno;
	} else {
		*ierror = 0;
	}
}


void
nrcv_(int *nevent, int *ntype, int *nlength, int *nflags, int *ndata, 
      int *ndsize, char *nmsg, int *ierror)
{
	struct nmsg	nhead;

	nhead.nh_event = *nevent;
	nhead.nh_type = *ntype;
	nhead.nh_length = *nlength;
	nhead.nh_flags = *nflags;
	nhead.nh_msg = nmsg;

	if (nrecv(&nhead)) {
		*ierror = errno;
	} else {
		*ierror = 0;
		memcpy((char *) ndata, (char *) nhead.nh_data,
			(*ndsize > sizeof(nhead.nh_data))
                        ? sizeof(nhead.nh_data) : *ndsize);
                *ntype = nhead.nh_type;
                *nlength = nhead.nh_length;
	}
}


void
nprob_(int *nevent, int *ntype, int *nlength, int *nflags, int *nready, 
       int *ierror)
{
	struct nmsg	nhead;

	nhead.nh_event = *nevent;
	nhead.nh_type = *ntype;

	if ((*nready = nprobe(&nhead)) > 0) {
                *ntype = nhead.nh_type;
                *nlength = nhead.nh_length;
                *nflags = nhead.nh_flags;
		*ierror = 0;
	}

	else if (*nready < 0) {
		*ierror = errno;
	} else {
		*ierror = 0;
	}
}


void
tsnd_(int *nnode, int *nevent, int *ntype, int *nlength, int *nflags, 
      int *ndata, int *ndsize, char *nmsg, int *ierror)
{
	struct nmsg	nhead;		/* network message descriptor */

	nhead.nh_node = *nnode;
	nhead.nh_event = *nevent;
	nhead.nh_type = *ntype;
	nhead.nh_length = *nlength;
	nhead.nh_flags = *nflags;
	nhead.nh_msg = nmsg;
 
	memcpy((char *) nhead.nh_data, (char *) ndata,
		(*ndsize > sizeof(nhead.nh_data))
                ? sizeof(nhead.nh_data) : *ndsize);

	if (tsend(&nhead)) {
		*ierror = errno;
	} else {
		*ierror = 0;
	}
}


void
trcv_(int *nevent, int *ntype, int *nlength, int *nflags, int *ndata, 
      int *ndsize, char *nmsg, int *ierror)
{
	struct nmsg	nhead;		/* network message descriptor */

	nhead.nh_event = *nevent;
	nhead.nh_type = *ntype;
	nhead.nh_length = *nlength;
	nhead.nh_flags = *nflags;
	nhead.nh_msg = nmsg;

	if (trecv(&nhead)) {
		*ierror = errno;
	} else {
		*ierror = 0;
		memcpy((char *) ndata, (char *) nhead.nh_data,
			(*ndsize > sizeof(nhead.nh_data))
                        ? sizeof(nhead.nh_data) : *ndsize);
                *ntype = nhead.nh_type;
                *nlength = nhead.nh_length;
	}
}


void
psndo_(int4 *pnode, int4* pevent, int4* ptype, int4* ierror)
{
	struct nmsg	nhead;

	nhead.nh_node = *pnode;
	nhead.nh_event = *pevent;
	nhead.nh_type = *ptype;

	if (psendopen(&nhead)) {
		*ierror = errno;
	} else {
		*ierror = 0;
	}
}


void
psndc_(int4 *pnode, int4 *pevent, int4 *ptype, int4 *ierror)
{
	struct nmsg	nhead;

	nhead.nh_node = *pnode;
	nhead.nh_event = *pevent;
	nhead.nh_type = *ptype;

	if (psendclose(&nhead)) {
		*ierror = errno;
	} else {
		*ierror = 0;
	}
}


void
psnd_(int4 *pnode, int4 *pevent, int4 *ptype, int4 *plength, int4 *pflags, 
      int4 *pdata, int4 *pdsize, char *pmsg, int4 *ierror)
{
	struct nmsg	nhead;

	nhead.nh_node = *pnode;
	nhead.nh_event = *pevent;
	nhead.nh_type = *ptype;
	nhead.nh_length = *plength;
	nhead.nh_flags = *pflags;
	nhead.nh_msg = pmsg;

	memcpy((char *) nhead.nh_data, (char *) pdata,
		(*pdsize > sizeof(nhead.nh_data))
		? sizeof(nhead.nh_data) : *pdsize);

	if (psend(&nhead)) {
		*ierror = errno;
	} else {
		*ierror = 0;
	}
}


void
prcvo_(int *pevent, int *ptype, int *ierror)
{
	struct nmsg	nhead;

	nhead.nh_event = *pevent;
	nhead.nh_type = *ptype;

	if (precvopen(&nhead)) {
		*ierror = errno;
	} else {
		*ierror = 0;
	}
}


void
prcvc_(int4* pevent, int4* ptype, int4* ierror)
{
	struct nmsg	nhead;

	nhead.nh_event = *pevent;
	nhead.nh_type = *ptype;

	if (precvclose(&nhead)) {
		*ierror = errno;
	} else {
		*ierror = 0;
	}
}


void
prcv_(int4 *pevent, int4 *ptype, int4 *plength, int4 *pflags, int4 *pdata, 
      int4 *pdsize, char *pmsg, int4 *ierror)
{
	struct nmsg	nhead;

	nhead.nh_event = *pevent;
	nhead.nh_type = *ptype;
	nhead.nh_length = *plength;
	nhead.nh_flags = *pflags;
	nhead.nh_msg = pmsg;

	if (precv(&nhead)) {
		*ierror = errno;
	} else {
		*ierror = 0;
		memcpy((char *) pdata, (char *) nhead.nh_data,
			(*pdsize > sizeof(nhead.nh_data))
			? sizeof(nhead.nh_data) : *pdsize);
		*ptype = nhead.nh_type;
		*plength = nhead.nh_length;
	}
}
