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
 *	$Id: priority.h,v 6.6 2002/10/09 20:57:25 brbarret Exp $
 *
 *	Function:	- priorities for Trollius system processes
 */

#ifndef _PRIORITY
#define _PRIORITY

/*
 * These priorities ensure message ordering.
 */
#define	PRMAX		1500
#define	PRROUTER	1400
#define	PRBUFFERD	1100
#define	PRDLO		1090
#define	PRDLI		1087

#define PRDIED		1084
#define	PRDAEMON	1085
#define	PRECHOD		PRDAEMON
#define	PRLOADD		PRDAEMON
#define	PRFILED		PRDAEMON
#define	PRBFORWARD	PRDAEMON
#define PRHALTD         PRDAEMON
#define PRVERSIOND      PRDAEMON

#define	PRCMD		1095
#define	PRDOOM		1095
/* see note in otb/sys/kernel/kkill.c */
#define PRLAMHALT       1094

#endif
