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
 * $Id: mpimsg.c,v 6.14 2003/02/27 03:01:12 jsquyres Exp $
 *
 *	Function:	- prints MPI message buffer status
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/param.h>
#include <netdb.h>
#include <unistd.h>

#include <all_opt.h>
#include <args.h>
#include <bfreq.h>
#include <preq.h>
#include <net.h>
#include <priority.h>
#include <events.h>
#include <terror.h>
#include <mpi.h>
#include <mpisys.h>
#include <mpitrace.h>
#include <etc_misc.h>

/*
 * external functions
 */
extern void		format_proc();
extern void		format_cid();
extern void		format_tag();
extern void		format_dtype();
extern void		mpitr_dataprint();
extern void		print_dtype();
extern void		print_comm();
extern int		mpitr_dtypeget();

/* 
 * local constants and macros
 */
#define global_rank(p)  ((p) >> 16)
#define local_rank(p)   ((p) & 0xFFFF)

/*
 * output formats
 */
#define HDR_FMT		"%-14s %-14s %-7s %-8s %-8s %-11s %s\n"
#define BODY_FMT	"%-14s %-14s %-7s %-8s %-8d %-11s n%d,#%d\n"


/*
 * local functions
 */
static void		mpimsg(int nodeid);
static int		matching_msg(int4 idx, int4 seq);
static void		print_msg(int4 nodeid, int4 srcnode, int4 srcpid,
				  int4 dtype, int4 i, int4 nelts);
static void		cid_vout(int4 nodeid, int4 pid, int4 seq, int4 cid);
static void		dtype_vout(int4 nodeid, int4 pid, int4 seq, 
				   int4 dtype);
static void		parse_command_line(int *pargc, char *argv[]);
static void		help(void);

/*
 * local variables
 */
static OPT              *ad;			/* option descriptor */
static struct bfstatus	*bftable;		/* buffer state descriptor */
static int		bfmax;			/* max entries */
static char		src_str[32];		/* formatting buffer */
static char		dest_str[32];		/* formatting buffer */
static char		tag_str[16];		/* formatting buffer */
static char		dtype_str[16];		/* formatting buffer */
static char		cid_str[16];		/* formatting buffer */
static int		fl_gps = 0;		/* print GPS flag */
static int		fl_d = 0;		/* print datatype flag */
static int		fl_c = 0;		/* print communicator flag */
static int		fl_m = 0;		/* print message flag */
static int		fl_a = 0;		/* show all messages flag */
static int		fl_homog = 0;		/* homogeneity flag */
static int		fl_allprocs;		/* show all processes? */
static int		maxelements;		/* max. elements to show */
static int		seqnum;			/* message sequence number */

/*
 * syntax for mpimsg command
 */
static char		*usage = "mpimsg [<options>] [<nodes>] [<processes>]";


int
main(int argc, char *argv[])
{
	int4		n_index;		/* index of node entry */
	int4		nodeid;			/* value of node entry */
	int4		n_flags;		/* extra node entry info */
	int4		nodec;			/* node counter */
	char		*nodev[3];		/* extra node list */
	int4		p_index;		/* index of process entry */
	int4		p_value;		/* value of process entry */
	int4		p_flags;		/* process selection method */


	/* Ensure that we are not root */

#ifndef LAM_ALLOW_RUN_AS_ROOT
	if (getuid() == 0 || geteuid() == 0) {
	  show_help(NULL, "deny-root", NULL);
	  exit(EACCES);
	}
#endif

	parse_command_line(&argc, argv);
/*
 * Allocate buffer state table.
 */
	if (bfmax > 0) {
		bftable = (struct bfstatus *)
			malloc((unsigned) (bfmax * sizeof(struct bfstatus)));

		if (! bftable) lampanic("mpimsg (malloc)");
	}
/*
 * Attach to kernel.
 */
	if (kinit(PRCMD)) {
	  char hostname[MAXHOSTNAMELEN];
	  gethostname(hostname, MAXHOSTNAMELEN);

	  show_help(NULL, "no-lamd", "mpimsg", hostname, NULL);
	  exit(LAM_EEXIT);
	}

	if (nid_parse(&argc, argv) || pid_parse(&argc, argv) ||
			(errno = (argc == 1) ? 0 : EUSAGE)) {
		fprintf(stderr, "usage: %s\n", usage);
		kexit(errno);
	}
/*
 * Check process specification.
 */
	pid_get(&p_index, &p_value, &p_flags);

	if (p_index < 0) {
		fl_allprocs = 1;
	} else {
		fl_allprocs = 0;
		do {
			if (p_flags != SELECT_INDEX) {
				fprintf(stderr,
				"\nProcesses must be selected by INDEX.\n");

				errno = EUSAGE;
				kexit(errno);
			}

			pid_get(&p_index, &p_value, &p_flags);

		} while(p_index);
	}
/*
 * Process the nodes one by one.
 */
	nid_get(&n_index, &nodeid, &n_flags);
/*
 * If the user did not give any nodes, use all of them.
 */
	if (n_index < 0) {
		nodev[0] = "mpimsg";
		nodev[1] = "N";
		nodev[2] = 0;
		nodec = 2;

		if (nid_parse(&nodec, nodev)) lamfail("mpimsg (parse_nodes)");

		nid_get(&n_index, &nodeid, &n_flags);

		if (n_index < 0) lamfail("mpimsg (getnode)");
	}

	if (!(fl_c || fl_d || fl_m)) {
		if (fl_gps) {
			printf(HDR_FMT, "SRC (GPS/L)", "DEST (GPS/L)", "TAG",
				"COMM", "COUNT", "DATATYPE", "MSG");
		} else {
			printf(HDR_FMT, "SRC (G/L)", "DEST (G/L)", "TAG",
				"COMM", "COUNT", "DATATYPE", "MSG");
		}
	}

	do {
		mpimsg(nodeid);
		
		nid_get(&n_index, &nodeid, &n_flags);

	} while (n_index);

	kexit(0);
	return(0);
}

/*
 *	mpimsg
 *
 *	Function:	- find and print the buffer state of a remote node
 *			- table of buffer state descriptors has one spacer
 *			  descriptor which separates the pending descriptors
 *			  from the waiting descriptors
 *
 *	Accepts:	- node identifier
 */
static void
mpimsg(int nodeid)
{
	struct bfparms	parms;		/* buffer parameters */
	struct bfstatus	*pi;		/* ptr current packet */
	struct _fyimsg	*fyi;		/* message FYI */
	int4		size;		/* # of return blocks */
	int4		limit;		/* # of blocks to print */
	int4		i;		/* favourite index */
	int4		srcnode;	/* msg src node */
	int4		srcidx;		/* msg src process index */
	int4		srcpid;		/* msg src PID */
	int4		destidx;	/* msg dest process index */
	int4		tag;		/* msg tag */
	int4		cid;		/* msg context ID */
	int4		ack;		/* msg ack flag */
	int4		dtype;		/* msg datatype */

	if ((size = rbfstate(nodeid, bftable, bfmax, &parms)) < 0)
		lamfail("mpimsg (rbfstate)");

	if (nodeid == LOCAL) nodeid = getnodeid();
	
	limit = (size < bfmax) ? size : bfmax;

	pi = bftable;

	for (i = 0; i < limit; ++i, ++pi) {
/*
 * Consider only MPI packets that start messages and are not ACKs.
 */
		if ( (pi->bfs_flags & KSYNCSQL) && !(pi->bfs_flags & N2ND)
			&& !(pi->bfs_event & 0x10000) ) {
/*
 * Partially unpack message FYI.
 */
			fyi = (struct _fyimsg *) &pi->bfs_data[4];
			dtype = fyi->fym_dtpid >> 16;
			srcpid = fyi->fym_dtpid & 0xFFFF;
/*
 * Extract synchronization from event and type.
 */
			lam_unsyncsql(pi->bfs_event, pi->bfs_type, &tag,
				&cid, &ack, &destidx, &srcnode, &srcidx);
/*
 * Skip collective call messages.
 */
			if (!fl_a && (cid < 0)) continue;
/*
 * Skip GER messages.
 */
			if (tag == MPI_GER_TAG) continue;
/*
 * Only output messages that match what the user specified.
 */
			if (!matching_msg(destidx, pi->bfs_seq)) continue;
/*
 * Format the message queue (destination) description.
 */
			format_proc(pi->bfs_node, destidx,
				global_rank(fyi->fym_dest),
				local_rank(fyi->fym_dest), fl_gps, dest_str);
/*
 * Check for vertical output of datatype, communicator or message contents.
 */
			if (fl_d) {
				dtype_vout(srcnode, srcpid, pi->bfs_seq,
						dtype);
			}
			else if (fl_c) {
				cid_vout(srcnode, srcpid, pi->bfs_seq, cid);
			}
			else if (fl_m) {
				print_msg(nodeid, srcnode, srcpid, dtype, i,
						maxelements);
			}
			if (fl_d || fl_c || fl_m) return;
/*
 * Output in horizontal format.
 */
			format_proc(srcnode, srcidx, global_rank(fyi->fym_src),
				local_rank(fyi->fym_src),fl_gps, src_str);

			format_tag(tag, tag_str);

			format_cid(cid, cid_str);

			format_dtype(dtype, dtype_str, 0);

			printf(BODY_FMT, src_str, dest_str, tag_str,
				cid_str, fyi->fym_count, dtype_str,
				nodeid, pi->bfs_seq);
		}
	}

	if (!fl_d && !fl_c && !fl_m && (size > bfmax)) {
		printf("...and more buffered messages...\n");
	}
}

/*
 *	matching_msg
 *
 *	Function:	- check if message matches users specification
 *	Accepts:	- index of node message is on
 *			- sequence number of the message
 */
static int
matching_msg(int4 idx, int4 seq)
{
	int4		p_index;		/* index of process entry */
	int4		p_value;		/* value of process entry */
	int4		p_flags;		/* process selection method */
	int4		start_index;		/* starting index */

	if (fl_allprocs) {
		return ((seqnum == -1) || (seqnum == seq));
	}
	else {
		if ((seqnum != -1) && (seqnum != seq)) return(0);
/*
 * Sequence number matches. Now check if process matches.
 */
		pid_get(&p_index, &p_value, &p_flags);
		start_index = p_index;

		do {
			if (p_value == idx) return(1);

			pid_get(&p_index, &p_value, &p_flags);

		} while (start_index != p_index);

		return(0);
	}
}

/*
 *	print_msg
 *
 *	Function:	- print the contents of a bufferd messge
 *	Accepts:	- node ID of node the message is buffered on
 *			- node ID of message source
 *			- PID of source process of message
 *			- datatype label
 *			- index in table of first message packet
 *			- number of leaf elements to print (-1 means all)
 */
static void
print_msg(int4 nodeid, int4 srcnode, int4 srcpid, int4 dtype, int4 i, 
	  int4 nelts)
{
	struct nmsg 	nhead;			/* for rbflook query */
        struct bfstatus *pmsg;			/* ptr first packet */
	char		content[MAXNMSGLEN];	/* holds content of message */
	char		*dtrace;		/* ptr datatype trace */
	int		nolonger;		/* msg no longer buffered? */

	pmsg = bftable + i;

	nhead.nh_msg = content;
	nhead.nh_length = pmsg->bfs_length;
/*
 * Get the contents of the packet. It may no longer be buffered.
 */
	nolonger = 0;
	if (rbflook(nodeid, &nhead, pmsg->bfs_seq)) {
		if (errno == EWOULDBLOCK) nolonger = 1;
		else lamfail("mpimsg (rbflook)");
	}

	printf("MESSAGE: %s #%d\n", dest_str, pmsg->bfs_seq);
	
	if (nolonger) {
		printf("no longer buffered\n");
		return;
	}
/*
 * Get the datatype trace.
 */
	if (mpitr_dtypeget(srcnode, srcpid, dtype, &dtrace)) {
		printf("Data type information unavailable.\n\n");
		return;
	}
/*
 * Print the contents of the message formatted according to the datatype.
 */
	mpitr_dataprint(dtrace, content, pmsg->bfs_length, nelts, fl_homog);

	if (pmsg->bfs_flags & NMORE) printf("...and more message data...\n");
	printf("\n");
	
	free(dtrace);
}

/*
 *	cid_vout
 *
 *	Function:	- output a communicator in vertical format
 *	Accepts:	- node communicator defined on
 *			- PID of process that defined communicator
 *			- message sequence number
 *			- context ID
 */
static void
cid_vout(int4 nodeid, int4 pid, int4 seq, int4 cid)
{
	printf("MESSAGE:\t%s #%d\n", dest_str, seq);
	print_comm(nodeid, pid, cid, fl_gps);
	printf("\n");
}

/*
 *	dtype_vout
 *
 *	Function:	- output a datatype in vertical format
 *	Accepts:	- node datatype defined on
 *			- PID of process that defined datatype
 *			- message sequence number
 *			- datatype label
 */
static void
dtype_vout(int4 nodeid, int4 pid, int4 seq, int4 dtype)
{
	printf("MESSAGE:\t%s #%d\n", dest_str, seq);
	format_dtype(dtype, dtype_str, 1);
	printf("DATATYPE:\t%s\n", dtype_str);

	if (dtype > TRDTMAX) {
		print_dtype(nodeid, pid, dtype, "  ");
	}

	printf("\n");
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
	char		*opt;		/* option string */
	
        ad = ao_init();
        if (ad == 0) {
		perror("mpimsg (ao_init)");
		exit(errno);
	}

	ao_setopt(ad, "a", 0, 0, 0);
	ao_setopt(ad, "gps", 0, 0, 0);
	ao_setopt(ad, "h", 0, 0, 0);
	ao_setopt(ad, "O", 0, 0, 0);

	ao_setopt(ad, "c", 0, 1, AOINT);
	ao_setopt(ad, "d", "c", 1, AOINT);
	ao_setopt(ad, "m", "d", 1, AOINT);

	ao_setopt(ad, "e", 0, 1, AOINT);
	ao_setopt(ad, "B", 0, 1, AOINT);
	
	if (ao_parse(ad, pargc, argv)) {
		fprintf(stderr, "usage: %s\n", usage);
		exit(errno);
	}

	if (ao_taken(ad, "h")) {
		help();
		exit(0);
	}

	if (ao_taken(ad, "B")) ao_intparam(ad, "B", 0, 0, &bfmax);
	else bfmax = BFSMAX;

	if (ao_taken(ad, "e"))  {
		ao_intparam(ad, "e", 0, 0, &maxelements);
		if (maxelements < 0) {
			fprintf(stderr, "usage: %s\n", usage);
			exit(errno);
		}
	} else {
		maxelements = -1;
	}

	if ((opt = ao_chosen(ad, "c"))) {
		ao_intparam(ad, opt, 0, 0, &seqnum);
	} else {
		seqnum = -1;
	}

	fl_a = ao_taken(ad, "a");
	fl_c = ao_taken(ad, "c");
	fl_d = ao_taken(ad, "d");
	fl_gps = ao_taken(ad, "gps");
	fl_m = ao_taken(ad, "m");
	fl_homog = ao_taken(ad, "O");
}

/*
 *	help
 *
 *	Function:	- prints helpful information on the mpimsg command
 */
static void
help(void)
{
	printf("\nSynopsis:	mpimsg [options] [nodes] [processes]\n");
	printf("\nDescription:	Print information on MPI message buffers.\n");
	printf("\nOptions:	-h	Print this message.\n");
	printf("\t\t-gps  	Print using GPS.\n");
	printf("\t\t-c <#> 	Print communicator groups of message #.\n");
	printf("\t\t-d <#> 	Print datatype of message #.\n");
	printf("\t\t-e <#>	Set maximum # of message elements to print.\n");
	printf("\t\t-m <#> 	Print contents of message #.\n");
	printf("\t\t-B <#>	Set maximum # of messages to print.\n");
	nid_usage();
	printf("\t\th (local), o (origin), N (all)\n");
	printf("\nProcesses:	p<list> (identifiers)\n");
	printf("\t\t%%<list> (indices)\n");
	printf("\nExample:	mpimsg h\n");
	printf("\t\t\t\"Get buffer status information on the local node.\"\n");
}
