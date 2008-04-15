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
 *	Function:	- F77 constants and structures associated with the
 *			  network subsystem
 *			- based on Trollius 2.0 Copyright 1992
 *			  The Ohio State University and Cornell
 *			  Research Foundation
 */

#ifndef _NETF
#define _NETF

/*
 * network constants
 */
#define	LOCAL		(-2147483647)	/* on the local node */
#define	MAXNMSGLEN	(4096)		/* maximum network frame size */
#define	NOTNODEID	(-2147483648)	/* not a valid node ID */
#define	NOTNODETYPE	(-2147483648)	/* not a valid node type */
#define	ORIGIN		(IGORGN())	/* should be user's node */

/*
 * node types
 */
#define NTOTB		(0)		/* OTB, not-ITB */
#define NTITB		(1)		/* ITB, not-OTB */
#define NTCAST		(2)		/* a multicast of nodes */
#define NTWASTE		(4)		/* not fully used/wasted */
#define NTDISK		(8)		/* node has a disk */
#define NTTUBE		(16)		/* node has a video unit */
#define NTJONES		(32)		/* neighbour node */
#define NTBOOT		(64)		/* child node in boot tree */
#define NTALL		(127)		/* match any bit-flag */

/*
 * predefined cast identifiers (implies a nodeid set and a nodeid source)
 */
#define HOST2ALL	(-3)	/* origin otb to all nodes */
#define HOST2ITB	(-4)	/* origin otb to all ITB nodes */
#define HOST2OTB        (-6)	/* origin otb to all OTB nodes */
#define HOST2COMPNODE	(-7)	/* origin otb to all compute nodes */
#define HOST2COMPCPU	(-8)	/* origin otb to all compute CPUs */

/*
 * network message header
 */
#define NHDSIZE		8		/* # of data words in header */

#define NOBUF		(16)		/* don't do buffering at all */
#define NREEL		(1024)		/* multireel */
#define DRAWDATA	(65536)		/* don't touch data pouch */
#define DINT4DATA	(131072)	/* data pouch is integers */
#define DFLT4DATA	(262144)	/* data pouch is floats */
#define DFTL8DATA	(524288)	/* data pouch is doubles */
#define DRAWMSG		(1048576)	/* don't touch message */
#define DINT4MSG	(2097152)	/* message is integers */
#define DFLT4MSG	(4194304)	/* message is floats */
#define DFLT8MSG	(8388608)	/* message is doubles */

/*
 * process runtime flags
 */
#define RTF_SYSGEN	(8)		/* system generated process */
#define RTF_WAIT	(16)		/* notify parent on child exit */
#define RTF_TRACE	(256)		/* enable tracing */
#define RTF_TRSWITCH	(512)		/* start with tracing on/off */

#endif
