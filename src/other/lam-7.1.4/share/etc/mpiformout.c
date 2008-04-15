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
 *	Ohio Trollius
 *	Copyright 1997 The Ohio State University
 *	NJN
 *
 *	$Id: mpiformout.c,v 6.5 2002/10/09 20:57:02 brbarret Exp $
 * 
 *	Function:	- format/output mpi information
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lam.h"

#include <mpi.h>
#include <mpisys.h>
#include <mpitrace.h>

/*
 * global functions
 */
void			format_proc();
void			format_cid();
void			format_tag();
void			format_dtype();
void			print_dtype();
void			print_comm();

/*
 * external variables
 */
extern int		mpitr_comget();
extern int		mpitr_dtypeget();
extern void		mpitr_dtypeprint();
extern void		mpitr_comprint();

/*
 * basic datatypes
 */
static CONST char	*dtfull[TRDTMAX + 1] = {
				"MPI_CONTIG", "MPI_VECTOR", "MPI_HVECTOR",
				"MPI_INDEX", "MPI_HINDEX", "MPI_STRUCT",
				"MPI_BYTE", "MPI_CHAR", "MPI_UNSIGNED_CHAR", 
				"MPI_SHORT", "MPI_UNSIGNED_SHORT", "MPI_INT",
				"MPI_UNSIGNED", "MPI_LOGICAL", "MPI_LONG",
				"MPI_UNSIGNED_LONG", "MPI_FLOAT","MPI_DOUBLE",
				"MPI_COMPLEX", "MPI_UB", "MPI_LB",
				"MPI_PACKED", "MPI_2INT", "", "",
				"MPI_FLOAT_INT",
				"MPI_DOUBLE_INT", "MPI_LONG_INT",
				"MPI_SHORT_INT", "MPI_LONG_DOUBLE", 
				"MPI_LONG_DOUBLE_INT", "MPI_INTEGER",
				"MPI_CHARACTER", "MPI_REAL", 
				"MPI_DOUBLE_PRECISION",  "MPI_DOUBLE_COMPLEX",
				"MPI_2INTEGER", "MPI_2REAL",
				"MPI_2DOUBLE_PRECISION", "MPI_LONG_LONG_INT",
				"MPI_UNSIGNED_LONG_LONG", "MPI_WCHAR",
				"MPI::COMPLEX", "MPI::DOUBLE_COMPLEX",
				"MPI::LONG_DOUBLE_COMPLEX", "MPI::BOOL"
			};

static CONST char	*dtshort[TRDTMAX + 1] = {
				"CONTIG", "VECTOR", "HVECTOR",
				"INDEX", "HINDEX", "STRUCT",
				"BYTE", "CHAR", "UCHAR", "SHORT",
				"USHORT", "INT", "UINT", "LOGICAL",
				"LONG", "ULONG", "FLOAT", "DOUBLE",
				"COMPLEX", "UB", "LB", "PACKED", 
				"2INT", "2FLOAT", "2DOUBLE",
				"FLOAT_INT", "DOUBLE_INT", "LONG_INT",
				"SHORT_INT", "LONG_DOUBLE", "LONGDBL_INT",
				"INTEGER", "CHARACTER", "REAL",
				"DBL_PRECSN", "DBL_COMPLEX", 
				"2INTEGER", "2REAL", "2DBL_PRECSN",
				"LONG_LONG", "ULONG_LONG", "WCHAR",
				"COMPLEX", "DBL_COMPLEX",
				"L_DBL_COMPLX", "BOOL"
			};

/*
 *	format_proc
 *
 *	Function:	- format a process description for output 
 *	Accepts:	- process node
 *			- process index
 *			- process global rank
 *			- process local rank
 *			- GPS flag (on implies use GPS)
 *			- buffer to format into (returned value)
 */
void
format_proc(node, index, grank, lrank, gps, str)

int			node;
int			index;
int			grank;
int			lrank;
int			gps;
char			*str;

{
	if (node == -1 || grank == -1) {
		strcpy(str, "ANY");
	}
	else if (gps) {
		if (lrank == -1) sprintf(str, "n%d,i%d", node, index);
		else sprintf(str, "n%d,i%d/%d", node, index, lrank);
	}
	else {
		if (lrank == -1) sprintf(str, "%d", grank);
		else sprintf(str, "%d/%d", grank, lrank);
	}
}

/*
 *	format_cid
 *
 *	Function:	- format a cid for output
 *	Accepts:	- context id
 *			- buffer to format into (returned value)
 */
void
format_cid(cid, str)

int			cid;
char			*str;

{
	int 		collective;	/*  is comm collective? */

	collective = (cid < 0);

	cid = lam_coll2pt(cid);
	
	if (cid == 0) {
		strcpy(str, "WORLD");
	}
	else if (cid == 1) {
		strcpy(str, "SELF");
	}
	else {
		sprintf(str, "<%d>", cid);
	}

	if (collective) strcat(str, "*");
}

/*
 *	format_tag
 *
 *	Function:	- format a tag for output
 *	Accepts:	- tag
 *			- buffer to format into (returned value)
 */
void
format_tag(tag, str)

int			tag;
char			*str;

{
	if (tag == MPI_ANY_TAG) {
		strcpy(str, "ANY");
	} else {
		sprintf(str, "%d", tag);
	}
}

/*
 *	format_dtype
 *
 *	Function:	- format a datatype for output
 *	Accepts:	- datatype
 *			- buffer to format into (returned value)
 *			- flag, on means print full name
 */
void
format_dtype(dtype, str, full)

int			dtype;
char			*str;
int			full;

{
	*str = 0;
	
	if (dtype < 0) return;

	if (dtype <= TRDTMAX) {
		strcpy(str, (full) ? dtfull[dtype] : dtshort[dtype]);
	}
	else {
		sprintf(str, "<%d>", dtype);
	}
}

/*
 *	print_dtype
 *
 *	Function:	- gets datatype trace and prints to stdout
 *	Accepts:	- node of process that defined datatype
 *			- pid of process that defined datatype
 *			- datatype label
 *			- prefix for output
 */
void
print_dtype(node, pid, dtype, prefix)

int			node;
int			pid;
int			dtype;
char			*prefix;

{
	char		*dtbuf;		/* ptr datatype trace buffer */
	
	if (mpitr_dtypeget(node, pid, dtype, &dtbuf)) {
		printf("%sData type information unavailable.\n", prefix);
		return;
	}

	mpitr_dtypeprint(dtype, dtbuf, 2, prefix, 0);

	free(dtbuf);
}

/*
 *	print_comm
 *
 *	Function:	- gets communicator trace and prints to stdout
 *	Accepts:	- node of process that defined communicator
 *			- pid of process that defined communicator
 *			- context ID
 *			- GPS flag, (on means show GPS's) 
 */
void
print_comm(node, pid, cid, gps)

int			node;
int			pid;
int			cid;
int			gps;

{
	char		*commbuf;	/* ptr to comm trace buffer */

	if ((lam_coll2pt(cid) != 1)
			&& mpitr_comget(node, pid, cid, &commbuf)) {

		printf("Communicator information unavailable.\n");
		return;
	}

	mpitr_comprint(cid, commbuf, gps);

	free(commbuf);
}
