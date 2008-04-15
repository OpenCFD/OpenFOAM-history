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
 * $Id: ipcdr.c,v 6.11 2003/02/16 06:57:48 brbarret Exp $
 * 
 *	Function:	- IPC diagnosis tool
 *			- sends or receives messages of given length
 *			- reports timings for message transfers
 *			- checks message ordering and integrity
 *			- optional use of buffers
 *
 *	Accepts:	- See the "usage" variable.
 *
 *	Examples:	- ipcdr -v -m 10 -l 200 -s n0
 *			- ipcdr -vcT -m 5 -l 10
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>
#include <netdb.h>
#include <unistd.h>

#include <args.h>
#include <etc_misc.h>
#include <lamdebug.h>
#include <events.h>
#include <net.h>
#include <priority.h>
#include <terror.h>
#include <typical.h>

/*
 * external functions
 */
extern double		ttime();		/* get current time value */

/*
 * local functions
 */
static void		fill_msg();		/* fill the message body */
static void		help();			/* print help menu */
static void		receiver();		/* run as a receiver */
static void		sender();		/* run as a sender */
static void		set_defaults();		/* set the option defaults */
static int		check_msg();		/* check message contents */

/*
 * local variables
 */
static char		*usage =
"ipcdr [-bchoqvCDFIRT] [-e event] [-t type] [-l len] [-m #msgs] [-s nodes]\n";

static int		dtype_size;		/* data type size */
static int		event;
static int		type;
static int		fl_msgtype;		/* message data type */
static int		fl_rtrip;		/* round-trip flag */
static int		fl_verbose;		/* verbose flag */
static int		length;			/* message length */
static int		num_messages;		/* number of loops */
static int		total_errors;		/* transmission errors */
static struct nmsg	nhead;			/* message header */


int
main(argc, argv)

int			argc;
char			*argv[];

{
/*
 * Parse the command line.
 */
	validopts("bchoqsTvR");
	followed("elmt");
	exclusive("CDFI");

	if (do_args(&argc, argv)) {
		fprintf(stderr, usage);
		exit(errno);
	}
/*
 * Check for help request.
 */
	if (opt_taken('h')) {
		help();
		exit(0);
	}
/*
 * Attach to the kernel.
 */
	if (kinit(PRCMD)) {
	  char hostname[MAXHOSTNAMELEN];
	  gethostname(hostname, MAXHOSTNAMELEN);

	  show_help(NULL, "no-lamd", "ipcdr", hostname, NULL);
	  exit(errno);
	}

	if (nid_parse(&argc, argv) || (errno = (argc == 1) ? 0 : EUSAGE)) {
		fprintf(stderr, usage);
		kexit(errno);
	}
/*
 * Set the default options.
 */
	set_defaults();
/*
 * Fill the message header.
 */
	nhead.nh_event = event;
	nhead.nh_type = type;
	nhead.nh_length = length * dtype_size;
	nhead.nh_flags = fl_msgtype | (opt_taken('b') ? NOBUF : 0) |
			(opt_taken('q') ? KSYNCSQL : 0);

	if (nhead.nh_length > 0) {
		nhead.nh_msg = malloc((unsigned) nhead.nh_length);
		if (nhead.nh_msg == 0) lamfail("ipcdr (malloc)");
	}
/*
 * Send or receive depending on the required role.
 */
	if (opt_taken('s')) {
		sender();
	} else {
		receiver();
	}

	kexit(0);
	return(0);
}

/*
 *	sender
 *
 *	Function:	- sends messages to the given nodes
 *			- reports the timing if needed
 */
static void
sender()

{
	int		i, loops;
	int		n_index, nodeid, n_flags;
	double		timing;	

	total_errors = loops = 0;
	fill_msg(&nhead);
/*
 * Get the first nodeid and check for usage.
 */
	nid_get(&n_index, &nodeid, &n_flags);

	if (n_index < 0) {
		fprintf(stderr, usage);
		kexit(EUSAGE);
	}

	timing = ttime();
/*
 * Loop over the number of messages to process.
 */
	for (i=0; i < num_messages; i++) {
/*
 * Write in each message header the message sequence number.
 */
		nhead.nh_data[1] = i;
/*
 * Loop over the given destination nodes.  Update loops count.
 */
		do {
			loops++;

			if ((nhead.nh_node = nodeid) != LOCAL) {
				*nhead.nh_data = getnodeid();
			} else {
				*nhead.nh_data = LOCAL;
			}
/*
 * Send the message.  For a round trip, receive the message.
 */
			if (nsend(&nhead)) lamfail("ipcdr (nsend)");
			VERBOSE("sent msg# %d, node %s\n", i,
					nid_fmt(nhead.nh_node));

			if (fl_rtrip) {
				if (nrecv(&nhead)) lamfail("ipcdr (nrecv)");
				VERBOSE("returned msg# %d, len %d\n",
						i, nhead.nh_length);
			}

			nid_get(&n_index, &nodeid, &n_flags);
		} while(n_index);

	}
/*
 * Get the elapsed time and report timing.
 */
	if (opt_taken('T')) {
		timing = ttime() - timing;
		if (fl_rtrip) loops *= 2;
		printf("total time: %f\taverage message time: %f\n", 
				timing, timing/loops);

		if (nhead.nh_length > 0) {
			printf("\t\t\taverage byte time: %f\n", 
					timing/loops/nhead.nh_length);
		}
	}
}

/*
 *	receiver
 *
 *	Function:	- receives messages and optionally echoes back
 *			- checks message order and integrity
 *			- reports timings
 */
static void
receiver()

{
	int		i, loops;
	int		seq_errors, next_seq;
	int		length;
	double		timing;

	total_errors = seq_errors = loops = 0;
	timing = ttime();
/*
 * Loop over the number of messages to receive.
 */
	length = nhead.nh_length;

	for (i=0, next_seq = 0; i < num_messages; i++, next_seq++) {
/*
 * Receive the message and check errors.  Update loops count.
 * Check the sequencing of the messages if required and report the number
 * of messages out of order.  For a round trip, send the message back.
 */
		loops++;
		nhead.nh_length = length;
		if (nrecv(&nhead)) lamfail("ipcdr (nrecv)");

		VERBOSE("received msg# %d, seq# %d, length %d",
			i, nhead.nh_data[1], nhead.nh_length);

		if (opt_taken('c') && check_msg(&nhead)) {
			total_errors++;
			VERBOSE("  <- data error");
		}

		if (opt_taken('o') && (nhead.nh_data[1] != next_seq)) {
			seq_errors += abs(nhead.nh_data[1] - next_seq);
			next_seq = nhead.nh_data[1];
			VERBOSE("  <- ordering error\n");
		} else {
			VERBOSE("\n");
		}

		if (fl_rtrip) {
			nhead.nh_node = *nhead.nh_data;
			if (nsend(&nhead)) lamfail("ipcdr (nsend)");
			VERBOSE("replied msg# %d, node %d\n", i,
					nhead.nh_node);
		}

	}
/*
 * Get the elapsed time and report timing.
 */
	if (opt_taken('T')) {
		timing = ttime() - timing;
		if (fl_rtrip) loops *= 2;
		printf("total time: %f\taverage message time: %f\n", 
				timing, timing/loops);

		if (nhead.nh_length > 0) {
			printf("\t\t\taverage byte time: %f\n", 
					timing/loops/nhead.nh_length);
		}
	}
/*
 * Report the number of transmission and message ordering errors.
 */
	if (length > 0) {
		free(nhead.nh_msg);
	}

	printf("\n");

	if (total_errors > 0) {
		printf("total transmission errors: %d\n", total_errors);
	}

	if (seq_errors > 0) {
		printf("total msg ordering errors: %d\n", seq_errors);
	}
}

/*
 *	fill_msg
 *
 *	Function:	- fills the body of the message
 *			- handles messages of char, int, float and double
 *	Accepts:	- ptr to a message header
 */
static void
fill_msg(header)

struct nmsg		*header;

{
	int		i, elements;
	int		*p_imsg;
	char		*p_msg;
	float		*p_fmsg;
	double		*p_dmsg;
	char		*filler = "abcdefghij";
/*
 * Fill the message with doubles.
 */
	if (header->nh_flags & DFLT8MSG) {
		p_dmsg = (double *) header->nh_msg;
		elements = header->nh_length / sizeof(double);
		for (i=0; i < elements; i++, p_dmsg++) {
			*p_dmsg = (double) 12345.6789;
		}
	}
/*
 * Fill the message with floats.
 */
	else if (header->nh_flags & DFLT4MSG) {
		p_fmsg = (float *) header->nh_msg;
		elements = header->nh_length / sizeof(float);
		for (i=0; i < elements; i++, p_fmsg++) {
			*p_fmsg = (float) 1234.6875;
		}
	}
/*
 * Fill the message with integers.
 */
	else if (header->nh_flags & DINT4MSG) {
		p_imsg = (int *) header->nh_msg;
		elements = header->nh_length / sizeof(int);
		for (i=0; i < elements; i++, p_imsg++) {
			*p_imsg = 123456789;
		}
	}
/*
 * Fill the message with characters.
 */
	else {
		p_msg = header->nh_msg;
		for (i=0; i < header->nh_length; i++, p_msg++) {
			*p_msg = filler[i % 10];
		}
	}
}

/*
 *	check_msg
 *
 *	Function:	- checks the contents of the message
 *			- handles messages of char, int, float and double
 *	Accepts:	- ptr to a message header
 *	Returns:	- 0 or ERROR
 */
static int
check_msg(header)

struct nmsg		*header;

{
	int 		i, elements;
	int		*p_imsg;
	float		*p_fmsg;
	double		*p_dmsg;
	char 		*p_msg;
	char		*filler = "abcdefghij";
/*
 * Check a message of doubles.
 */
	if (header->nh_flags & DFLT8MSG) {
		p_dmsg = (double *) header->nh_msg;
		elements = header->nh_length / sizeof(double);
		for (i=0; i < elements; i++, p_dmsg++) {
			if (*p_dmsg != (double) 12345.6789) return(LAMERROR);
		}
	}
/*
 * Check a message of floats.
 */
	else if (header->nh_flags & DFLT4MSG) {
		p_fmsg = (float *) header->nh_msg;
		elements = header->nh_length / sizeof(float);
		for (i=0; i < elements; i++, p_fmsg++) {
			if (*p_fmsg != (float) 1234.6875) return(LAMERROR);
		}
	}
/*
 * Check a message of integers.
 */
	else if (header->nh_flags & DINT4MSG) {
		p_imsg = (int *) header->nh_msg;
		elements = header->nh_length / sizeof(int);
		for (i=0; i < elements; i++, p_imsg++) {
			if (*p_imsg != 123456789) return(LAMERROR);
		}
	}
/*
 * Check a message of characters.
 */
	else {
		p_msg = header->nh_msg;
		for (i=0; i < header->nh_length; i++, p_msg++) {
			if (*p_msg != filler[i % 10]) return(LAMERROR);
		}
	}

	return(0);
}

/*
 *	help
 *
 *	Function:	- print the help menu for the tool
 */
static void
help()
{
	printf("\nSynopsis:\tipcdr [options]\n");
	printf("\nDescription:\tTest and time Inter-Process Communication.\n");
	printf("\nOptions:\t-h\t\tPrint this message.\n");
	printf("\t\t-b\t\tDisable buffering.\n");
	printf("\t\t-c\t\tCheck received message integrity.\n");
	printf("\t\t-o\t\tCheck received message order.\n");
	printf("\t\t-q\t\tUse SQL synchronization.\n");
	printf("\t\t-T\t\tTime message transmission.\n");
	printf("\t\t-v\t\tBe verbose.\n");
	printf("\t\t-C\t\tMessage contains characters.\n");
	printf("\t\t-D\t\tMessage contains doubles.\n");
	printf("\t\t-F\t\tMessage contains floats.\n");
	printf("\t\t-I\t\tMessage contains integers.\n");
	printf("\t\t-R\t\tPerform round trip communication.\n");
	printf("\t\t-e event\tSynchronize on given event.\n");
	printf("\t\t-t type\t\tSynchronize on given type.\n");
	printf("\t\t-l length\tSet message length.\n");
	printf("\t\t-m #msgs\tTransmit given # of messages.\n");
	printf("\t\t-s nodes\tSend to given nodes.\n");
	nid_usage();
}

/*
 *	set_defaults
 *
 *	Function:	- sets the default values of key variables
 *			- checks most of the command line arguments
 */
static void
set_defaults()

{
/*
 * Set the flags.
 */
	fl_verbose = opt_taken('v');
	fl_rtrip = opt_taken('R');
/*
 * Set among the mutual exclusive options.
 */
	switch(opt_chosen('C')) {
		case 'C':
			fl_msgtype = DRAWMSG;
			dtype_size = sizeof(char);
			break;
		case 'D':
			fl_msgtype = DFLT8MSG;
			dtype_size = sizeof(double);
			break;
		case 'F':
			fl_msgtype = DFLT4MSG;
			dtype_size = sizeof(float);
			break;
		case 'I':
			fl_msgtype = DINT4MSG;
			dtype_size = sizeof(int);
			break;
		case LAMERROR:
			fl_msgtype = 0;
			dtype_size = sizeof(char);
			break;
		default:
			errno = EIMPOSSIBLE;
			lamfail("ipcdr (opt_chosen)");
	}
/*
 * Set the parameters.
 */
	if (opt_taken('e')) {
		intparam('e', &event);
	} else {
		event = EVIPCDIAG;
	}

	if (opt_taken('t')) {
		intparam('t', &type);
	} else {
		type = 0;
	}

	if (opt_taken('m')) {
		intparam('m', &num_messages);
	} else {
		num_messages = 1;
	}

	if (opt_taken('l')) {
		intparam('l', &length);
	} else {
		length = 0;
	}
}
