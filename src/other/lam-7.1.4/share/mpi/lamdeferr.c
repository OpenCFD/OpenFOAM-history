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
 * $Id: lamdeferr.c,v 6.14 2003/11/02 21:13:10 brbarret Exp $
 *
 *	Function:	- default MPI error handlers
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#if __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif

#include <lam_config.h>
#include <etc_misc.h>
#include <mpi.h>
#include <mpisys.h>
#include <tstdio.h>

/*
 * private functions
 */
static void		report_error(MPI_Comm comm, int errcode, 
                                     char *errmsg, char *type);

/*
 * public functions
 */
#if LAM_WANT_PROTOS
void			lam_comm_errfatal(MPI_Comm *, int *, ...);
void			lam_win_errfatal(MPI_Win *, int *, ...);
void			lam_errreturn(void *, int *, ...);
#else
void			lam_comm_errfatal();
void			lam_win_errfatal();
void			lam_errreturn();
#endif
void                    lam_printfunc(void);


/*
 *	lam_comm_errfatal
 *
 *	Function:	- default MPI error function
 *			- print error message and abort
 *	Accepts:	- communicator
 *			- error class
 *			- error message
 *			- error value (errno)
 */
#if LAM_WANT_PROTOS
void
lam_comm_errfatal(MPI_Comm *errcomm, int *errclass, ...)
#else
void
lam_comm_errfatal(errcomm, errclass, va_alist)

MPI_Comm		*errcomm;
int			*errclass;
va_dcl
#endif

{
	MPI_Comm	comm;
	char		*errmsg;
	int		errcode;		/* combined class, fn, errno */
	int		error;			/* errno */
	va_list		arglist;

#if __STDC__
	va_start(arglist, errclass);
#else
	va_start(arglist);
#endif
	error = va_arg(arglist, int);
	errmsg = va_arg(arglist, char *);
	va_end(arglist);

	comm = (errcomm) ? *errcomm : MPI_COMM_NULL;

	errcode = lam_mkerr(*errclass, error);
	report_error(comm, errcode, errmsg, "comm");

	if (comm != MPI_COMM_NULL) {
		MPI_Abort(comm, errcode);
	}

	kexit(error);
}

/*
 *	lam_win_errfatal
 *
 *	Function:	- default MPI error function
 *			- print error message and abort
 *	Accepts:	- window
 *			- error class
 *			- error message
 *			- error value (errno)
 */
#if LAM_WANT_PROTOS
void
lam_win_errfatal(MPI_Win *win, int *errclass, ...)
#else
void
lam_win_errfatal(win, errclass, va_alist)

MPI_Win			*win;
int			*errclass;
va_dcl
#endif

{
	char		*errmsg;
	int		errcode;		/* combined class, fn, errno */
	int		error;			/* errno */
	va_list		arglist;

#if __STDC__
	va_start(arglist, errclass);
#else
	va_start(arglist);
#endif
	error = va_arg(arglist, int);
	errmsg = va_arg(arglist, char *);
	va_end(arglist);

	errcode = lam_mkerr(*errclass, error);
	report_error((*win)->w_comm, errcode, errmsg, "win");

	MPI_Abort((*win)->w_comm, errcode);
	kexit(error);
}

/*
 *	report_error
 *
 *	Function:	- print out nice error message
 *	Accepts:	- communicator
 *			- error code
 *			- error message
 *			- handle type (win or comm)
 */
static void
report_error(MPI_Comm comm, int errcode, char *errmsg, char *type)
{
	char		mpierrmsg[MPI_MAX_ERROR_STRING];
	char		name[MPI_MAX_OBJECT_NAME+1];
	int		rank;
	int		cid;
	int		len;

	MPI_Error_string(errcode, mpierrmsg, &len);

	if (comm != MPI_COMM_NULL) {
		MPI_Comm_rank(comm, &rank);

		if (comm->c_name[0]) {
		    lam_strncpy(name, comm->c_name, MPI_MAX_OBJECT_NAME);
		} else {
		    MPIL_Comm_id(comm, &cid);
		    cid = lam_mpi_coll2pt(cid);
		    sprintf(name, "%s %d", type, cid);
		}

		if (_kio.ki_rtf & RTF_IO) {
			if (*errmsg) {
				printf("%s: %s (rank %d, %s)\n",
					mpierrmsg, errmsg, rank, name);
			} else {
				printf("%s (rank %d, %s)\n",
					mpierrmsg, rank, name);
			}
			fflush(stdout);
		} else {
			if (*errmsg) {
				tprintf("%s: %s (rank %d, %s)\n",
					mpierrmsg, errmsg, rank, name);
			} else {
				tprintf("%s (rank %d, %s)\n",
					mpierrmsg, rank, name);
			}
		}
	}
	else {
		if (_kio.ki_rtf & RTF_IO) {
			if (*errmsg) {
				printf("%s: %s (node %d, pid %d)\n", mpierrmsg,
					errmsg, getnodeid(), (int) lam_getpid());
			} else {
				printf("%s (node %d, pid %d)\n",
					mpierrmsg, getnodeid(), (int) lam_getpid());
			}
			fflush(stdout);
		} else {
			if (*errmsg) {
				tprintf("%s: %s (node %d, pid %d)\n", mpierrmsg,
					errmsg, getnodeid(), (int) lam_getpid());
			} else {
				tprintf("%s (node %d, pid %d)\n",
					mpierrmsg, getnodeid(), (int) lam_getpid());
			}
		}
	}

/*
 * print out the LAM/MPI call stack
 */
	lam_printfunc();
}

/*
 *	lam_errreturn
 *
 *	Function:	- predefined error function
 *			- stub that does nothing
 *	Accepts:	- handle
 *			- error code
 */
#if LAM_WANT_PROTOS
void
lam_errreturn(void *handle, int *error, ...)
#else
void
lam_errreturn(handle, error, va_alist)

void			*handle;
int			*error;
va_dcl
#endif

{
	;
}
