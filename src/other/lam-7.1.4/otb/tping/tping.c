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
 * $Id: tping.c,v 6.22 2004/03/06 20:37:21 jsquyres Exp $
 *
 *	Function:	- bounces a message off the echo daemon
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/param.h>
#include <netdb.h>
#include <unistd.h>

#include <args.h>
#include <lamdebug.h>
#include <ereq.h>
#include <net.h>
#include <priority.h>
#include <terror.h>
#include <typical.h>
#include <dl_inet.h>
#include <etc_misc.h>

#define plural(n)	((n != 1) ? "s" : "")

/*
 * local variables
 */
static int		delay;			/* delay between echos */
static int		fl_inf;			/* infinite # messages flag */
static int		fl_intr = FALSE;	/* trap interrupt flag */
static int		fl_verbose;		/* verbose flag */
static int4		msglen;			/* message length */
static int4		nmesg;			/* # messages to send */

/*
 * local functions
 */
static int		setdefaults(void);
static void		trap(int);


int
main(int argc, char *argv[])
{
	int		i;
	int		decr;			/* loop decrement (0 or 1) */
	int		n_index;		/* node index */
	int		first_index;		/* first node index */
	int		n_flags;		/* node flags */
	int4		ret;			/* recho() return value */
	int4		nsent;			/* # messages sent */
	int4		nbytes;			/* # bytes sent */
	int4		nnodes;			/* # remote target nodes */
	int4		localnodes;		/* # local target nodes */
	int4		nodeid;			/* target node ID */
	int4		prevnode;		/* previous node ID */
	char		*outbuf = 0;		/* outgoing message buffer */
	char		*inbuf = 0;		/* incoming message buffer */
	double		tave;			/* average timing */
	double		tmin;			/* min timing */
	double		tmax;			/* max timing */
	double		ttotal;			/* total timing */
	double		ttrip;			/* message trip timing */
	int		*indices;		/* array of indices */
	int4		nlinks;                 /* total number of links */
	struct dolink	*links;                 /* from ldogetlinks */

	/* Ensure that we are not root */

#ifndef LAM_ALLOW_RUN_AS_ROOT
	if (getuid() == 0 || geteuid() == 0) {
	  show_help(NULL, "deny-root", NULL);
	  exit(EACCES);
	}
#endif

/*
 * Parse the command line.
 */
	validopts("hv");
	followed("cdl");

	if (do_args(&argc, argv)) {
	  show_help("tping", "usage", NULL);
	  exit(errno);
	}
/*
 * Check for help request.
 */
	if (opt_taken('h')) {	
	  show_help("tping", "usage", NULL);
	  exit(0);
	}
/*
 * Set the defaults.
 */
	if (setdefaults()) {
		exit(errno);
	}
/*
 * Allocate in/out buffers.
 */
	if (msglen) {
		outbuf = malloc((unsigned) msglen);
		inbuf = malloc((unsigned) msglen);

		if ((outbuf == 0) || (inbuf == 0)) {
			perror("tping (malloc)");
			exit(errno);
		}
	}

	decr = (fl_inf) ? 0 : 1;
/*
 * Set up the trap handler.
 */
	if (signal(SIGINT, trap) == SIG_ERR) {
		fprintf(stderr, "tping: cannot trap SIGINT\n");
		exit(errno);
	}

	if (signal(SIGTERM, trap) == SIG_ERR) {
		fprintf(stderr, "tping: cannot trap SIGTERM\n");
		exit(errno);
	}
/*
 * Initialize the timing variables.
 */
	tmin = 1.0e+20;
	tmax = -1;
	ttotal = 0;
/*
 * Attach to the Trollius kernel.
 */
	if (kinit(PRCMD)) {
	  char hostname[MAXHOSTNAMELEN];
	  gethostname(hostname, MAXHOSTNAMELEN);

	  show_help(NULL, "no-lamd", "tping", hostname, NULL);
	  exit(errno);
	}

	if (nid_parse(&argc, argv) || (errno = (argc == 1) ? 0 : EUSAGE)) {
	  show_help("tping", "usage", NULL);
	  kexit(errno);
	}
/*
 * Get the first node identifier.
 */
	nid_get(&n_index, &nodeid, &n_flags);
	first_index = n_index;

	if (n_index < 0) {
	  show_help("tping", "usage", NULL);
	  errno = EUSAGE;
	  kexit(errno);
	}
	/*
	 * Get the max number of links so that we can size some arrays
	 * below
	 */
	if (ldogetlinks(&links, &nlinks))
	  lamfail("lamhalt (ldogetlinks)");
/*
 * Use that as an upper bound to malloc the nodes array
 */
	indices = (int*) malloc(sizeof(int) * nlinks);
	if (indices == 0) {
	  kexit(errno);
	}
	for (i = 0; i < nlinks; i++)
	  indices[i] = -1;
/*
 * Now fill the indices array with the indices of unique nodes
 */
	localnodes = 0;
	do {
          if ((nodeid >= nlinks) || ((nodeid < 0) && (nodeid != LOCAL))) {
            fprintf(stderr, "tping: invalid node %s\n", nid_fmt(nodeid));
            kexit(EUSAGE);
          }

          if ((nodeid == getnodeid()) || (nodeid == LOCAL)) {
            localnodes = 1;
            i = getnodeid();
          } else {
            i = nodeid;
          }

          if (indices[i] == -1)
            indices[i] = n_index;

	  nid_get(&n_index, &nodeid, &n_flags);
	} while (n_index != first_index);
/*
 * Loop over the # of messages.
 */
	prevnode = nodeid;
	for (nsent = 0; (nmesg > 0) && (!fl_intr); ++nsent, nmesg -= decr) {
		ttrip = ttime();
/*
 * Loop through all specified nodes.
 */
		nnodes = 0;
		do {
                  i = (nodeid != LOCAL) ? nodeid : getnodeid();
                  if (indices[i] == n_index) {
			prevnode = nodeid;
			if (i != getnodeid())
			  ++nnodes;
/*
 * Ping the node.
 */
			ret = recho(nodeid, outbuf, inbuf, msglen);

			if (ret != msglen) {

				if (ret < 0) {
					terror("tping (recho)");
				} else {
					fprintf(stderr,
						"tping: length error\n");
				}

				kexit(errno);
			}
		  }

		  nid_get(&n_index, &nodeid, &n_flags);
		} while (n_index != first_index);
/*
 * Compute timing statistics.
 */
		ttrip = ttime() - ttrip;
		ttotal += ttrip;
		if (ttrip < tmin) {
			tmin = ttrip;
		}
		if (ttrip > tmax) {
			tmax = ttrip;
		}

		if (nnodes + localnodes == 1) {
		  VERBOSE("%3d byte%s from %s: %3.3f secs\n", msglen,
			  plural(msglen), nid_fmt(prevnode), ttrip);
  		} else {
  		  VERBOSE("%3d byte%s from ", msglen, plural(msglen));
  		  if (nnodes > 0)
  		    VERBOSE("%d remote node%s", nnodes, plural(nnodes));
  		  if (nnodes > 0 && localnodes > 0)
  		    VERBOSE(" and ");
  		  if (localnodes > 0)
  		    VERBOSE("1 local node");
  		  VERBOSE(": %3.3f secs\n", ttrip);
  		}
/*
 * Wait for a while.
 */
  		if (delay > 0) {
  			sleep((unsigned) delay);
  		}
  	}
/*
 * Free the buffers.
 */
  	if (outbuf) {
  		free(outbuf);
  	}

  	if (inbuf) {
  		free(inbuf);
  	}
  	free(indices);
/*
 * Print out the final statistics.
 */
  	if (nsent <= 0) {
  		fprintf(stderr, "tping: sent %d messages!\n", nsent);
  		kexit(1);
  	}
  	nbytes = msglen * nsent;
  	tave = ttotal / nsent;

  	printf("\n%d message%s, %d byte%s (%4.3fK), %4.3f secs (%4.3fK/sec)\n",
 		nsent, plural(nsent), nbytes, plural(nbytes),
  		((double) nbytes) / 1024, ttotal,
  		((double) 2 * nbytes) / ttotal / 1024);

  	printf("roundtrip min/avg/max: %3.3f/%3.3f/%3.3f\n", tmin, tave, tmax);

  	kexit(0);
  	return(0);
  }

/*
 *	trap
 *
 *	Function:	- signal trap handler
 *			- set interrupt flag in order to exit the loop
 */
static void
trap(int sig)
{
  	fl_intr = TRUE;
}

/*
 *	setdefaults
 *
 *	Function:	- set the default command parameters
 *	Returns:	- 0 or ERROR
 */
static int
setdefaults(void)
{
/*
 * Get the # of messages to send.
 */
  	if (opt_taken('c')) {
  		fl_inf = FALSE;
 		intparam('c', &nmesg);
  	} else {
  		fl_inf = TRUE;
  		nmesg = 1;
  	}

  	if (nmesg < 0) {
  		fprintf(stderr, "tping: count (%d) must be >= 0\n", nmesg);
  		errno = EUSAGE;
  		return(LAMERROR);
  	} else if (nmesg == 0) {
  		fl_inf = TRUE;
  		nmesg = 1;
  	}
/*
 * Get the delay between messages.
 */
  	if (opt_taken('d')) {
  		intparam('d', &delay);
  	} else {
  		delay = 1;
  	}

  	if (delay < 0) {
  		fprintf(stderr, "tping: delay (%d) must be >= 0\n", delay);
  		errno = EUSAGE;
  		return(LAMERROR);
  	}
/*
 * Set the verbose flag.
 */
  	fl_verbose = ! opt_taken('v');
/*
 * Set the message length.
 */
  	if (opt_taken('l')) {
 		intparam('l', &msglen);
  	} else {
  		msglen = 1;
  	}

  	if (msglen < 0) {
  		fprintf(stderr, "tping: length (%d) must be >= 0\n", msglen);
  		errno = EUSAGE; 
  		return(LAMERROR); 
  	} 
	else if (msglen > MAXNMSGLEN) {
		fprintf(stderr, "tping: using max length (%d)\n", MAXNMSGLEN);
		msglen = MAXNMSGLEN;
	}

	return(0);
}

