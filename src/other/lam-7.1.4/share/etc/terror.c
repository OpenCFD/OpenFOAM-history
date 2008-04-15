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
 *	Copyright 1997 The Ohio State Universtiy
 *	GDB
 *
 *	$Id: terror.c,v 6.10 2002/10/09 20:57:04 brbarret Exp $
 *
 *	Function:	- Trollius error messages
 */

#include <string.h>
#include <unistd.h>

#include "lam_config.h"
#include "sfh.h"
#include "lam.h"

#ifdef WIN32
#include <windows.h>
#endif

#include "portable.h"
#include "terror.h"

/*
 * local variables
 */
static CONST char	*lam_emsg[] = {
		"undiagnosed error",		/* EUNKNOWN */
		"remote node has died",		/* EDEADNODE */
		"out of exit descriptors",	/* ENOEXDESCRIPTORS */
		"out of descriptors",		/* EFULL */
		"not a process",		/* ENOTPROCESS */
		"invalid request",		/* EBADREQUEST */
		"block not allocated",		/* ENOTMALLOCED */
		"GER overflow",			/* EGERFLOW */
		"service unavailable",		/* ESERVICE */
		"bad magic number",		/* EMAGIC */
		"blown stack",			/* EBLOWN */
		"impossible, ergo a bug",	/* EIMPOSSIBLE */
		"operation timed out",		/* LAM_ERR_TO */
		"unknown host",			/* EBADHOST */
		"no running daemon",		/* ENOKERNEL */
		"not attached to daemon",	/* ENOTATTACHED */
		"operation would block",	/* LAM_ERR_BLOCK */
		"improper message size",	/* EMSGLEN */
		"end of file",			/* EEOF */
		"cast spanning tree collision",	/* ECOLLIDE */
		"invalid parameter",		/* LAM_ERR_INVAL */
		"too many processes using shared memory", /* ESHMMAXP */
		"can't find shared segment",	/* ESHARE */
		"invalid link index",		/* EBADLINK */
		"bad acknowledgement",		/* EACK */
		"improper command invocation",	/* EUSAGE */
		"bad/unknown option",		/* EOPTION */
		"creating semaphore",		/* ESEMCREATE */
		"invalid node",			/* EBADNODE */
		"invalid ID specification",	/* EBADIDSPEC */
		"no matching nodes found",	/* ENONODES */
		"out of route descriptors",	/* ENORDESCRIPTORS */
		"out of buffer descriptors",	/* ENOBFDESCRIPTORS */
		"out of boot descriptors",	/* ENOBTDESCRIPTORS */
		"out of flat descriptors",	/* ENOFLDESCRIPTORS */
		"out of kenya descriptors",	/* ENOPDESCRIPTORS */
		"out of process descriptors",	/* ENOKDESCRIPTORS */
		"bad moses request",		/* EBADMOSESREQ */
		"creating shared memory",	/* ESHMCREATE */
		"invalid address tag",		/* EBADTAG */
		"delete from list failed",	/* EDELETE */
		"no bootstrap present",		/* ENOBOOTSTRAP */
		"invalid record format",	/* EBADREC */
		"bad ascii number",		/* EBADASCIINUM */
		"booted system is too big",	/* EHIGHWATER */
		"process already in group",	/* EINGROUP */
		"limit 32 group procs/node",	/* EGMAXPROC */
		"group has a single process",	/* EGSINGLETON */
		"not a group process",		/* EGNOTPROC */
		"invalid group",		/* EBADGROUP */
		"MPI already initialized",	/* EMPIINIT */
		"MPI not initialized",		/* EMPINOINIT */
		"duplicate virtual circuit",	/* LAM_ERR_DUPVC */
		"invalid virtual circuit",	/* LAM_ERR_BADVC */
		"name is published",		/* EPUBLISHED */
		"name is not published",	/* ENOTPUBLISHED */
		"ptys not supported",		/* ENOPTYSUPPORT */
		"passing of file descriptors not supported", /* ENOPFDSUPPORT */
		"MPI already finalized",        /* EMPIALREADYFINALIZE */
		"dynamic memory allocation failed", /* EMPIMEMERROR */ 
			};

static char		*baderror = "error code ";

#ifdef WIN32
extern int		sys_nerr;
extern char		*sys_errlist[];
#endif

/*
 *	lam_errorstr
 *
 *	Function:	- obtains error message string
 *			- uses Trollius, errno and system message tables
 *			- message is stored user provided space
 *	Accepts:	- user's error message buffer
 *			- buffer length
 */
void
lam_errorstr(char *emsgbuf, unint emsgbuflen)
{
	int		ecode;			/* errno code */
	int		found = 0;
	char		anum[16];		/* ascii error code */
	char		*syserrmsg;

	if ((emsgbuf == 0) || (emsgbuflen < 1)) return;

	*emsgbuf = '\0';
	emsgbuflen--;
	ecode = LAMGetLastError();
/*
 * Look for LAM error message.
 *
 * Do this before calling strerror(), because Linux will return
 * "Unknown error X" for any errno that it does not recognize rather
 * than NULL or "".
 */
	if ((ecode >= ELOW) && (ecode <= EHIGH)) {
		strncat(emsgbuf, lam_emsg[ecode - ELOW], emsgbuflen);
		found = 1;
	}
/*
 * Look for C runtime error message (errno).
 */
#ifdef WIN32
	else if ((ecode >= LAM_ERRNO_BASE) &&
			(ecode < (LAM_ERRNO_BASE + sys_nerr))) {
		strncat(emsgbuf, sys_errlist[ecode], emsgbuflen);
	}
#else
	else if ((syserrmsg = strerror(ecode)) != NULL &&
	    strlen(syserrmsg) > 0) {
		strncat(emsgbuf, syserrmsg, emsgbuflen);
	}
#endif
/*
 * Error code is obtained from the system.
 */
	else {
#ifdef WIN32
		found = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, ecode,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR) emsgbuf, emsgbuflen, NULL);
#else
		found = 0;
#endif
/*
 * Return a bad error code message.
 */
		if (!found) {
			sfh_itoa(ecode, anum);
			strncat(emsgbuf, baderror, emsgbuflen);

			if (strlen(baderror) < emsgbuflen) {
				strncat(emsgbuf, anum, 
					emsgbuflen - strlen(baderror));
			}
		}
	}
}

/*
 *	lam_perror
 *
 *	Function:	- prints error messages on stderr
 *	Accepts:	- user's error message
 */
void
lam_perror(CONST char *umsg)
{
	char		emsgbuf[LAM_EMSGBUFLEN];
	unint		l;

	if ((umsg == 0) || (*umsg == '\0')) {
		lam_errorstr(emsgbuf, LAM_EMSGBUFLEN - 1);
		strcat(emsgbuf, "\n");
	}

	else {
		*emsgbuf = '\0';
		l = strlen(umsg);
		strncat(emsgbuf, umsg, LAM_EMSGBUFLEN - 2);
/*
 * Continue building the error message if I have enough room for
 * the separator (2), at least one character of the error message,
 * a newline, and a string termination.
 */
		if (l <= (LAM_EMSGBUFLEN - 5)) {
			strcat(emsgbuf, ": ");
			lam_errorstr(emsgbuf + l + 2,
					(LAM_EMSGBUFLEN - (l + 2)) - 1);
		}

		strcat(emsgbuf, "\n");
	}

	write(2, emsgbuf, strlen(emsgbuf));
}

/*
 * backwards compatibility
 */
void terror(umsg) CONST char *umsg; { lam_perror(umsg); }
