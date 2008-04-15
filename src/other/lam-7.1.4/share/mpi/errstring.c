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
 * $Id: errstring.c,v 6.10 2003/05/31 22:28:50 jsquyres Exp $
 *
 *	Function:	- return the MPI error message
 *	Accepts:	- error code
 *			- message buffer (out)
 *			- buffer length (out)
 *	Returns:	- MPI_SUCCESS
 */

#include <stdio.h>
#include <string.h>

#include "lam.h"

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>
#include <typical.h>
#include <etc_misc.h>


/*
 * error messages
 */
static CONST char	*mpierrmsg[] = {
	"unused error code",			/* unused */
	"invalid buffer pointer",		/* MPI_ERR_BUFFER */
	"invalid count argument",		/* MPI_ERR_COUNT */
	"invalid datatype argument",		/* MPI_ERR_TYPE */
	"invalid tag argument",			/* MPI_ERR_TAG */
	"invalid communicator",			/* MPI_ERR_COMM */
	"invalid rank",				/* MPI_ERR_RANK */
	"invalid request handle",		/* MPI_ERR_REQUEST */
	"invalid root",				/* MPI_ERR_ROOT */
	"invalid group",			/* MPI_ERR_GROUP */
	"invalid operation",			/* MPI_ERR_OP */
	"invalid topology",			/* MPI_ERR_TOPOLOGY */
	"invalid dimension argument",		/* MPI_ERR_DIMS */
	"invalid argument",			/* MPI_ERR_ARG */
	"unknown error",			/* MPI_ERR_UNKNOWN */
	"message truncated",			/* MPI_ERR_TRUNCATE */
	"unclassified",				/* MPI_ERR_OTHER */
	"internal MPI error",			/* MPI_ERR_INTERN */
	"error code is in status",		/* MPI_ERR_IN_STATUS */
	"pending request",			/* MPI_ERR_PENDING */
	"out of system resources",		/* MPI_ERR_SYSRESOURCE */
	"process in local group is dead",	/* MPI_ERR_LOCALDEAD */
	"process in remote group is dead",	/* MPI_ERR_REMOTEDEAD */
	"info value truncated",			/* MPI_ERR_VALUE */
	"mismatched run-time flags",		/* MPI_ERR_FLAGS */
	"publishing service",			/* MPI_ERR_SERVICE */
	"publishing service",			/* MPI_ERR_NAME */
	"error spawning process",		/* MPI_ERR_SPAWN */
	"invalid key value",			/* MPI_ERR_KEYVAL */
	"no such info key",			/* MPI_ERR_INFO_NOKEY */
	"invalid window",			/* MPI_ERR_WIN */
	"invalid epoch",			/* MPI_ERR_EPOCH */
	"operation not supported on type",	/* MPI_ERR_TYPENOTSUP */
	"invalid info key",			/* MPI_ERR_INFO_KEY */
	"invalid info value",			/* MPI_ERR_INFO_VALUE */
	"could not allocate memory",		/* MPI_ERR_NOMEM */
	"invalid base argument",		/* MPI_ERR_BASE */
	"last error code (huh?)",		/* MPI_ERR_LASTCODE */
	};

/*@
   MPI_Error_string - Return a string for a given error code

Input Parameters:
. errcode - Error code returned by an MPI routine or an MPI error
class

Output Parameter:
+ msg - Text that corresponds to the errorcode 
- plen - Length of string 

Notes:  

Error codes are the values return by MPI routines (in C) or in the
'ierr' argument (in Fortran).  These can be converted into error
classes with the routine 'MPI_Error_class'.

These messages are provided in English (En_US - English/United
States).  If there is an interest in providing messages in different
languages, please submit requests to the LAM mailing list (see
'http://www.lam-mpi.org/contact.php').

.N fortran

.N ACK
@*/
int MPI_Error_string(int errcode, char *msg, 
		     int *plen)
{
	int		class;			/* error class */
	int		funct;			/* error function */
	int		error;			/* UNIX errno */
	int		err;			/* function error code */
	unint		len;			/* error message length */
	char		*p;			/* favourite pointer */
	char		buf[MPI_MAX_ERROR_STRING]; /* error message buffer */

	lam_initerr();
	lam_setfunc(BLKMPIERRSTRING);
/*
 * Check the arguments.
 */
	if ((msg == 0) || (plen == 0)) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIERRSTRING, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Create the error message string.
 */
	err = MPI_SUCCESS;

	if (errcode == 0) {
		sprintf(buf, "MPI: no errors");
	}

	else {
		lam_bkerr(errcode, &class, &funct, &error);
		strcpy(buf, "");

		if (funct == 0) {
			funct = lam_getfunc();
		}

		if ((p = blktype(funct))) {
			strcat(buf, p);
			strcat(buf, ": ");
		}

		if ((class > 0) && (class < MPI_ERR_LASTCODE)) {
			strcat(buf, mpierrmsg[class]);
		} else {
			strcat(buf, "unknown error class");
			err = lam_mkerr(MPI_ERR_ARG, EINVAL);
		}

		if (error > 0) {
			strcat(buf, ": ");
			LAMSetLastError(error);
			len = strlen(buf);
			lam_errorstr(buf + len, MPI_MAX_ERROR_STRING - len);
		}
	}
/*
 * Fill the user's message buffer.
 */
	lam_strncpy(msg, buf, MPI_MAX_ERROR_STRING - 1);
	msg[MPI_MAX_ERROR_STRING - 1] = '\0';
	*plen = strlen(msg);

	if (err != MPI_SUCCESS) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIERRSTRING, err));
	}

	lam_resetfunc(BLKMPIERRSTRING);
	return(MPI_SUCCESS);
}
