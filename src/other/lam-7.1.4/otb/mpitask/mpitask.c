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
 * $Id: mpitask.c,v 6.16 2003/02/27 03:01:12 jsquyres Exp $
 * 
 *	Function:	- prints MPI task status information
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/param.h>
#include <netdb.h>
#include <unistd.h>

#include <all_opt.h>			/* argument parsing */
#include <args.h>			/* argument parsing */
#include <bfreq.h>			/* for buffer info on blocked tasks */
#include <blktype.h>			/* for sync info */
#include <events.h>                     /* for EVSTOP and EVPAUSE */
#include <ksignal.h>			/* signal constants */
#include <mpisys.h>
#include <mpitrace.h>			/* datatype constants */
#include <net.h>			/* network constants */
#include <preq.h>			/* kenya interface constants */
#include <priority.h>			/* scheduling priority */
#include <terror.h>			/* error codes */
#include <typical.h>			/* commonly used constants */
#include <t_types.h>			/* data conversion */
#include <etc_misc.h>

/*
 * local constants and macros
 */
#define FORMAT		"%-20.20s %-13.13s %-10.10s %-6.6s %-6.6s %-7.7s %s\n"
#define	nullify(s)	((s)[0] = 0)

/*
 * local functions
 */
static void		state_node(int4 nodeid);
static void		run_state_out(int4 nodeid, struct pstate *pstat);
static void		br_state_out(int4 nodeid, struct pstate *pstat);
static void		bs_state_out(int4 nodeid, struct pstate *pstat);
static void		mpiblk_state_out(int4 nodeid, struct pstate *pstat);
static void             unpack_fyi(struct _fyiproc *fyi, int4 *psglobal, 
				   int4 *pslocal, int4 *peernode, 
				   int4 *peeridx, int4 *peerglobal, 
				   int4 *peerlocal, int4 *rootnode, 
				   int4 *rootidx, int4 *rootglobal, 
				   int4 *rootlocal, int4 *cid, int4 *tag);
static void		format_task(int4 nodeid, struct pstate *pstat, 
				    int4 idx, int4 global, int4 local, 
				    char *buf);
static void		cid_vout(int4 nodeid, int4 pid, int4 cid);
static void		dtype_vout(int4 nodeid, int4 pid, int4 dtype);
static char		*mpiblktype(int4 type);
static void		parse_command_line(int *pargc, char *argv[]);
static void		help(void);

/*
 * local variables
 */
static OPT              *ad;			/* argument descriptor */
static struct pstate	ptable[PMAX];		/* kenya process table */
static char		task_str[32];		/* formatting buffer */
static char		func_str[32];		/* formatting buffer */
static char		peer_str[32];		/* formatting buffer */
static char		tag_str[16];		/* formatting buffer */
static char		dtype_str[16];		/* formatting buffer */
static char		cid_str[16];		/* formatting buffer */
static char		count_str[16];		/* formatting buffer */
static int		fl_gps = 0;		/* print GPS flag */
static int		fl_d = 0;		/* print datatype flag */
static int		fl_c = 0;		/* print communicator flag */

/*
 * syntax for mpitask command
 */
static char		*usage =
		"mpitask [-cdh] [-gps] [<nodes>] [<processes>]";

/*
 *	main
 */
int
main(int argc, char *argv[])
{
	int4		n_index;		/* index of node entry */
	int4		nodeid;			/* value of node entry */
	int4		n_flags;		/* extra node entry info */
	int4		nodec;			/* node counter */
	char		*nodev[3];		/* extra node list */

	/* Ensure that we are not root */

#ifndef LAM_ALLOW_RUN_AS_ROOT
	if (getuid() == 0 || geteuid() == 0) {
	  show_help(NULL, "deny-root", NULL);
	  exit(EACCES);
	}
#endif

	parse_command_line(&argc, argv);
/*
 * Attach to kernel.
 */
	if (kinit(PRCMD)) {
	  char hostname[MAXHOSTNAMELEN];
	  gethostname(hostname, MAXHOSTNAMELEN);

	  show_help(NULL, "no-lamd", "mpitask", hostname, NULL);
	  exit(LAM_EEXIT);
	}

	if (nid_parse(&argc, argv) || pid_parse(&argc, argv) ||
			(errno = (argc == 1) ? 0 : EUSAGE)) {
		fprintf(stderr, "usage: %s\n", usage);
		kexit(errno);
	}
/*
 * Loop through all specified nodes.
 */
	nid_get(&n_index, &nodeid, &n_flags);
/*
 * If the user did not give any nodes, use all of them.
 */
	if (n_index < 0) {
		nodev[0] = "mpitask";
		nodev[1] = "N";
		nodev[2] = 0;
		nodec = 2;

		if (nid_parse(&nodec, nodev)) lamfail("mpitask (parse_nodes)");
		
		nid_get(&n_index, &nodeid, &n_flags);

		if (n_index < 0) lamfail("mpitask (nid_get)");
	}

	if (!(fl_c || fl_d)) {
		if (fl_gps) {
			printf(FORMAT, "TASK (GPS/L)", "FUNCTION", "PEER|ROOT",
				"TAG", "COMM", "COUNT", "DATATYPE");
		} else {
			printf(FORMAT, "TASK (G/L)", "FUNCTION", "PEER|ROOT",
				"TAG", "COMM", "COUNT", "DATATYPE");
		}
	}

	do {
		state_node(nodeid);

		nid_get(&n_index, &nodeid, &n_flags);
		
	} while (n_index);

	kexit(0);
	return(0);
}

/*
 *	state_node
 *
 *	Function:	- finds and prints the state of MPI processes on a node
 *	Accepts:	- node identifier
 */
static void
state_node(int4 nodeid)
{
	struct pstate	*pstat;			/* ptr process state */
	int4		nprocs;			/* # processes returned */
	int4		node;			/* node ID output suitable */
	int4		i;			/* favourite index */
	int4		p_index;		/* index of process entry */
	int4		p_value;		/* value of process entry */
	int4		p_flags;		/* process selection method */

	node = (nodeid == LOCAL) ? getnodeid() : nodeid;

	pid_get(&p_index, &p_value, &p_flags);

	if (p_index < 0) p_flags = SELECT_MPI;
/*
 * Get table of info on processes on the node.
 */
	do {
		nprocs = rpstate(nodeid, p_flags, p_value,
					ptable, (int4) PMAX);
		if (nprocs < 0) lamfail("mpitask (rpstate)");
/*
 * Display all processes in the table.
 */
		for (i = 0, pstat = ptable; i < nprocs; ++i, ++pstat) {

			if (pstat->ps_kernel.ks_state == KSSEND) {
				bs_state_out(node, pstat);
			}
			else if (pstat->ps_kernel.ks_state == KSRECV) {
				br_state_out(node, pstat);
			}
			else {
				run_state_out(node, pstat);
			}
		}
		
		pid_get(&p_index, &p_value, &p_flags);
	} while(p_index > 0);

}

/*
 *	run_state_out
 *
 *	Function:	- output status of a running process
 *	Accepts:	- node identifier
 *			- process state
 */
static void
run_state_out(int4 nodeid, struct pstate *pstat)
{
	struct _fyiproc	*fyi;		/* ptr to process FYI */
/*
 * Running process has no associated communicator or datatype.
 */
	if (fl_c || fl_d) return;

	fyi = (struct _fyiproc *) pstat->ps_kernel.ks_fyi;
	mttoli4((int4 *) fyi, sizeof(struct _fyiproc) / sizeof(int4));
	
	format_task(nodeid, pstat, pstat->ps_index, fyi->fyp_me >> 16,
			-1, task_str);

	printf("%-20.20s <running>\n", task_str);
}

/*
 *	br_state_out
 *
 *	Function:	- output status of a process blocked receiving
 *			- this is done using same format as the state command
 *	Accepts:	- node identifier
 *			- process state
 */
static void
br_state_out(int4 nodeid, struct pstate *pstat)
{
	struct _fyiproc	*fyi;		/* ptr process FYI */
	
	fyi = (struct _fyiproc *) pstat->ps_kernel.ks_fyi;
	mttoli4((int4 *) fyi, sizeof(struct _fyiproc) / sizeof(int4));

	format_task(nodeid, pstat, pstat->ps_index, fyi->fyp_me >> 16,
			-1, task_str);

	if (pstat->ps_kernel.ks_event >= 0) {
		mpiblk_state_out(nodeid, pstat);
	}
	else {
		if (fl_c || fl_d) return;
		
		if (pstat->ps_kernel.ks_event == EVSTOP) {
			sprintf(func_str, "<stopped>");
		}
		else if (pstat->ps_kernel.ks_event == EVPAUSE) {
			sprintf(func_str, "<paused>");
		}
		else if (pstat->ps_kernel.ks_event == - pstat->ps_pid) {
			if (blktype(pstat->ps_kernel.ks_type)) {
				sprintf(func_str, "%s",
					blktype(pstat->ps_kernel.ks_type));
			} else {
				sprintf(func_str, "<system>");
			}
		}
		else {
			sprintf(func_str, "<system>");
		}
		
		printf("%-20.20s %-13.13s\n", task_str, func_str);
	}
}

/*
 *	bs_state_out
 *
 *	Function:	- output status of a process blocked sending
 *	Accepts:	- node identifier
 *			- process state
 */
static void
bs_state_out(int4 nodeid, struct pstate* pstat)
{
	struct _fyiproc	*fyi;		/* ptr to process FYI */
	int4		event;		/* kernel state event */
	int4		type;		/* kernel state type */
	
	event = pstat->ps_kernel.ks_event;
	type = pstat->ps_kernel.ks_type;

	fyi = (struct _fyiproc *) pstat->ps_kernel.ks_fyi;
	mttoli4((int4 *) fyi, sizeof(struct _fyiproc) / sizeof(int4));
	
	format_task(nodeid, pstat, pstat->ps_index, fyi->fyp_me >> 16,
			-1, task_str);

	if ((event == EVBUFFERD) || (event == EVDL0)) {
		if (type == DLUSR) {
			if (blktype(fyi->fyp_func)) {
				if (fyi->fyp_func > 0) {		
					mpiblk_state_out(nodeid, pstat);
					return;
				} else {
					sprintf(func_str,
						blktype(fyi->fyp_func));
				}
			}
			else {
				sprintf(func_str, "<blocked>");
			}
		}
		else if (type == DLSYS) {
			sprintf(func_str, "<blocked>");
		}
		else {
			sprintf(func_str, "<system>");
		}
	}
	else if (event == EVROUTER) {
		sprintf(func_str, "<system>");
	}
	else {
		sprintf(func_str, "<blocked>");
	}

	if (!(fl_c || fl_d)) printf("%-20.20s %-13.13s\n", task_str, func_str);
}

/*
 *	mpiblk_state_out
 *
 *	Function:	- output state of a process blocked in MPI
 *	Accepts:	- node identifier
 *			- process state
 */
static void
mpiblk_state_out(int4 nodeid, struct pstate *pstat)
{
	struct _fyiproc	*fyi;		/* ptr process FYI */
	int4		psglobal;	/* process global rank */
	int4		pslocal;	/* process local rank */
	int4		psidx;		/* process index */
	int4		peernode;	/* peer node */
	int4		peeridx;	/* peer process index */
	int4		peerglobal;	/* peer global rank */
	int4		peerlocal;	/* peer local rank */
	int4		rootnode;	/* root node */
	int4		rootidx;	/* root process index */
	int4		rootglobal;	/* root global rank */
	int4		rootlocal;	/* root local rank */
	int4		tag;		/* tag */
	int4		cid;		/* context ID */

	fyi = (struct _fyiproc *) pstat->ps_kernel.ks_fyi;

	unpack_fyi(fyi, &psglobal, &pslocal, &peernode, &peeridx,
		&peerglobal, &peerlocal, &rootnode, &rootidx, &rootglobal,
		&rootlocal, &cid, &tag);

	psidx = pstat->ps_kernel.ks_event & 0x0000FFFF;

	format_task(nodeid, pstat, psidx, psglobal, pslocal, task_str);
/*
 * Format according to the function blocked upon.
 */
	switch(fyi->fyp_func) {
/*
 * These functions show peer/tag/contextid/datatype/count.
 */
	case BLKMPISEND:
	case BLKMPIRECV:
	case BLKMPISSEND:
	case BLKMPIRSEND:
	case BLKMPISENDRECV:
	case BLKMPISENDRECVREP:
	case BLKMPIWAIT:

		if (fl_d) dtype_vout(nodeid, pstat->ps_pid, fyi->fyp_dtype);
		else if (fl_c) cid_vout(nodeid, pstat->ps_pid, cid);
		if (fl_d || fl_c) return;
		
		sprintf(func_str, "%s", mpiblktype(fyi->fyp_func));
		format_proc(peernode, peeridx, peerglobal, peerlocal,
				fl_gps, peer_str);
		format_tag(tag, tag_str);
		format_cid(cid, cid_str);
		format_dtype(fyi->fyp_dtype, dtype_str, 0);
		sprintf(count_str, "%d", fyi->fyp_count);
		
		break;
/*
 * These functions show peer/tag/contextid.
 */
	case BLKMPIPROBE:

		if (fl_c) cid_vout(nodeid, pstat->ps_pid, cid);
		if (fl_d || fl_c) return;

		sprintf(func_str, "%s", mpiblktype(fyi->fyp_func));
		format_proc(peernode, peeridx, peerglobal, peerlocal,
				fl_gps, peer_str);
		format_tag(tag, tag_str);
		format_cid(cid, cid_str);
		nullify(dtype_str);
		nullify(count_str);
		
		break;
/*
 * These functions show root/contextid/datatype/count.
 */
	case BLKMPIBCAST:
	case BLKMPIGATHER:
	case BLKMPIGATHERV:
	case BLKMPISCATTER:
	case BLKMPISCATTERV:
	case BLKMPIREDUCE:

		if (fl_d) dtype_vout(nodeid, pstat->ps_pid, fyi->fyp_dtype);
		else if (fl_c) cid_vout(nodeid, pstat->ps_pid, cid);
		if (fl_d || fl_c) return;

		sprintf(func_str, "%s", mpiblktype(fyi->fyp_func));
		format_proc(rootnode, rootidx, rootglobal, rootlocal,
				fl_gps, peer_str);
		nullify(tag_str);
		format_cid(cid, cid_str);
		format_dtype(fyi->fyp_dtype, dtype_str, 0);
		sprintf(count_str, "%d", fyi->fyp_count);

		break;
/*
 * These functions show contextid/datatype/count.
 */
	case BLKMPIALLGATHER:
	case BLKMPIALLGATHERV:
	case BLKMPIALLTOALL:
	case BLKMPIALLREDUCE:
	case BLKMPISCAN:
			
		if (fl_d) dtype_vout(nodeid, pstat->ps_pid, fyi->fyp_dtype);
		else if (fl_c) cid_vout(nodeid, pstat->ps_pid, cid);
		if (fl_d || fl_c) return;

		sprintf(func_str, "%s", mpiblktype(fyi->fyp_func));
		nullify(peer_str);
		nullify(tag_str);
		format_cid(cid, cid_str);
		format_dtype(fyi->fyp_dtype, dtype_str, 0);
		sprintf(count_str, "%d", fyi->fyp_count);

		break;
/*
 * These functions show contextid/datatype.
 */
	case BLKMPIALLTOALLV:
	case BLKMPIREDUCESCATTER:

		if (fl_d) dtype_vout(nodeid, pstat->ps_pid, fyi->fyp_dtype);
		else if (fl_c) cid_vout(nodeid, pstat->ps_pid, cid);
		if (fl_d || fl_c) return;

		sprintf(func_str, "%s", mpiblktype(fyi->fyp_func));
		nullify(peer_str);
		nullify(tag_str);
		format_cid(cid, cid_str);
		format_dtype(fyi->fyp_dtype, dtype_str, 0);
		nullify(count_str);

		break;
/*
 * These functions show root/contextid.
 */
	case BLKMPICOMMACCEPT:
	case BLKMPICOMMCONNECT:
	case BLKMPILSPAWN:
	case BLKMPICOMMSPAWN:
	case BLKMPICOMMSPAWNMULT:

		if (fl_c) cid_vout(nodeid, pstat->ps_pid, cid);
		if (fl_d || fl_c) return;

		sprintf(func_str, "%s", mpiblktype(fyi->fyp_func));
		format_proc(rootnode, rootidx, rootglobal, rootlocal,
				fl_gps, peer_str);
		nullify(tag_str);
		format_cid(cid, cid_str);
		nullify(dtype_str);
		nullify(count_str);

		break;
/*
 * These functions show contextid.
 */
	case BLKMPIBARRIER:
	case BLKMPICOMMCREATE:
	case BLKMPIICOMMCREATE:
	case BLKMPIICOMMMERGE:
	case BLKMPICOMMSPLIT:
	case BLKMPICOMMDUP:
	case BLKMPICOMMDISCONNECT:
	case BLKMPICARTCREATE:
	case BLKMPICARTSUB:
	case BLKMPIGRAPHCREATE:
	case BLKMPILTRACEON:
	case BLKMPILTRACEOFF:
	case BLKMPIWINFENCE:
	case BLKMPIWINWAIT:
	case BLKMPIWINCOMPLETE:
	case BLKMPIWINPOST:

		if (fl_c) cid_vout(nodeid, pstat->ps_pid, cid);
		if (fl_d || fl_c) return;

		sprintf(func_str, "%s", mpiblktype(fyi->fyp_func));
		nullify(peer_str);
		nullify(tag_str);
		format_cid(cid, cid_str);
		nullify(dtype_str);
		nullify(count_str);

		break;
/*
 * These functions show the function name only.
 */
	case BLKMPIWAITANY:
	case BLKMPIWAITALL:
	case BLKMPIWAITSOME:
	case BLKMPIINIT:
	case BLKMPIFINAL:
	case BLKMPIABORT:
	case BLKMPISTART:
	case BLKMPISTARTALL:
	case BLKMPIBUFDETACH:

		if (fl_d || fl_c) return;

		sprintf(func_str, "%s", mpiblktype(fyi->fyp_func));
		nullify(peer_str);
		nullify(tag_str);
		nullify(cid_str);
		nullify(dtype_str);
		nullify(count_str);

		break;
/*
 * Others are non-blocking and should never occur (unless perhaps there
 * has been an MPI error amd the library is hosed).
 */
	default:

		sprintf(func_str, "<blocked>");
		nullify(peer_str);
		nullify(tag_str);
		nullify(cid_str);
		nullify(dtype_str);
		nullify(count_str);

		break;
	}

	printf(FORMAT, task_str, func_str, peer_str,
		tag_str, cid_str, count_str, dtype_str);
}

/*
 *	unpack_fyi
 *
 *	Function:	- unpack process FYI into constituent elements
 *	Accepts:	- ptr process FYI
 *			- ptrs to locations to unpack fyi into
 */
static void
unpack_fyi(struct _fyiproc *fyi, int4 *psglobal, int4 *pslocal, 
	   int4 *peernode, int4 *peeridx, int4 *peerglobal, int4 *peerlocal,
	   int4 *rootnode, int4 *rootidx, int4 *rootglobal, int4 *rootlocal, 
	   int4 *cid, int4 *tag)
{
	*psglobal = fyi->fyp_me >> 16;
	*pslocal = fyi->fyp_me & 0xFFFF;

	*peernode = fyi->fyp_peergps >> 16;
	*peeridx = fyi->fyp_peergps & 0xFFFF;
	*peerglobal = fyi->fyp_peer >> 16;
	*peerlocal = fyi->fyp_peer & 0xFFFF;

	*rootnode = fyi->fyp_rootgps >> 16;
	*rootidx = fyi->fyp_rootgps & 0xFFFF;
	*rootglobal = fyi->fyp_root >> 16;
	*rootlocal = fyi->fyp_root & 0xFFFF;

	*cid = fyi->fyp_cidtag >> 16;
	*tag = fyi->fyp_cidtag & 0xFFFF;
/*
 * Sign extend.
 */
	if (*cid & 0x8000) *cid |= 0xFFFF0000;
	if (*tag & 0x8000) *tag |= 0xFFFF0000;

	if (*peerglobal & 0x8000) *peerglobal |= 0xFFFF0000;
	if (*peerlocal & 0x8000) *peerlocal |= 0xFFFF0000;
}

/*
 *	format_task
 *
 *	Function:	- formats a processes task
 *	Accepts:	- node identifier
 *			- process state
 *			- process index
 *			- process global rank
 *			- process local rank
 *			- buffer to format into (returned value)
 */
static void
format_task(int4 nodeid, struct pstate *pstat, int4 idx, int4 global, 
	    int4 local, char *buf)
{
	char		*p;		/* favourite pointer */

	format_proc(nodeid, idx, global, local, fl_gps, buf);
/*
 * Set the program name if known.
 */
	if (pstat->ps_name[0]) {
		p = strrchr(pstat->ps_name, STRDIR);
		p = (p == 0) ? pstat->ps_name : p + 1;
		strcat(buf, " ");
		strcat(buf, p);
	}
}

/*
 *	cid_vout
 *
 *	Function:	- output a communicator in vertical format
 *	Accepts:	- node cid defined on
 *			- pid of process that defined cid
 *			- context ID
 */
static void
cid_vout(int4 nodeid, int4 pid, int4 cid)
{
	char		*fmt;
	
	fmt = (fl_gps) ? "GPS/L" : "G/L";

	printf("TASK (%s):\t%s\n", fmt, task_str);

	print_comm(nodeid, pid, cid, fl_gps);

	printf("\n");
}

/*
 *	dtype_vout
 *
 *	Function:	- output a datatype in vertical format
 *	Accepts:	- node datatype defined on
 *			- pid of process that defined datatype
 *			- datatype label
 */
static void
dtype_vout(int4 nodeid, int4 pid, int4 dtype)
{
	char		*fmt;		/* formatting */
	
	fmt = (fl_gps) ? "GPS/L" : "G/L";
	
	format_dtype(dtype, dtype_str, 1);
	
	printf("TASK (%s):\t%s\n", fmt, task_str);
	printf("DATATYPE:\t%s\n", dtype_str);

	if (dtype > TRDTMAX) print_dtype(nodeid, pid, dtype, "  ");

	printf("\n");
}

/*
 *	mpiblktype
 *
 *	Function:	- get description of blocking request type code
 *			- suitable for mpitask output
 *	Accepts:	- request type code
 *	Returns:	- description string or NULL
 */
static char *
mpiblktype(int4 type)
{
	char		*desc;		/* full description */
	char		*under;		/* position of first underscore */
	
	desc = blktype(type);

	if (desc == 0) return(0);

	if ((under = strchr(desc, '_'))) return(under+1);
	else return(desc);
}

/*
 *	parse_command_line
 *
 *	Function:	- parse the command line for options
 *	Accepts:	- ptr argument count
 *			- argument vector
 */
static void
parse_command_line(int *pargc, char *argv[])
{
        ad = ao_init();
        if (ad == 0) {
		perror("mpitask (ao_init)");
		exit(errno);
	}

	ao_setopt(ad, "c", 0, 0, 0);
	ao_setopt(ad, "d", "c", 0, 0);
	ao_setopt(ad, "gps", 0, 0, 0);
	ao_setopt(ad, "h", 0, 0, 0);
	
	if (ao_parse(ad, pargc, argv)) {
		fprintf(stderr, "usage: %s\n", usage);
		exit(errno);
	}

	if (ao_taken(ad, "h")) {
		help();
		exit(0);
	}
			
	fl_c = ao_taken(ad, "c");
	fl_d = ao_taken(ad, "d");
	fl_gps = ao_taken(ad, "gps");
}


/*
 *	help
 *
 *	Function:	- prints helpful information on the mpitask command
 */
static void
help(void)
{
	printf("\nSynopsis:	mpitask [options] [nodes] [processes]\n");
	printf("\nDescription:	Print information on LAM MPI tasks.\n");
	printf("\nOptions:	-h	Print this message.\n");
	printf("\t\t-c  	Print communicator group(s).\n");
	printf("\t\t-d  	Print datatype definition(s).\n");
	printf("\t\t-gps  	Print using GPS.\n");
	nid_usage();
	printf("\t\th (local), o (origin), N (all)\n");
	printf("\nProcesses:	p<list> (identifiers)\n");
	printf("\t\t%%<list> (indices)\n");
	printf("\nExample:	mpitask n3-9,11 %%1 %%2 p303\n");
	printf("\t\t\t\"Get MPI tasks status information for PID 303 and\n");
	printf("\t\t\tindices 1 & 2 for nodes 3 thru 9 and 11.\"\n");
}
