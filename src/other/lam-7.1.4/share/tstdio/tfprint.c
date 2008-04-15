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
 *	SBD/JRV
 *
 *	$Id: tfprint.c,v 6.9 2002/10/09 21:00:54 brbarret Exp $
 *
 *	Function:	- prints on stdout
 *	Accepts:	- format string
 *			- variable number of items to print
 *	Returns:	- the number of printed characters, or EOF if error
 */

#if __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif
#include <lam_config.h>
#include <laminternal.h>
#include <tstdio.h>

#if LAM_WANT_PROTOS
int
tprintf(const char *format, ...)
#else
int
tprintf(format, va_alist)

char			*format;
va_dcl
#endif

{
	char		*storep	= 0;
	TFILE		*fp	= tstdout;
	int		count;
	va_list		arglist;

#if __STDC__
	va_start(arglist, format);
#else
	va_start(arglist);
#endif
	count = _fmt(fp, storep, format, arglist);
	va_end(arglist);
	return(count);
}


/*
 *	tfprintf
 *
 *	Function:	- prints to file pointed to by fp
 *	Accepts:	- file pointer
 *			- format string
 *			- variable number of items to print
 *	Returns:	- the number of printed characters, of EOF if error
 */
#if LAM_WANT_PROTOS
int
tfprintf(TFILE *fp, const char *format, ...)
#else
int
tfprintf(fp, format, va_alist)

TFILE			*fp;
char			*format;
va_dcl

#endif

{
	char		*storep	= 0;
	int		count;
	va_list		arglist;

#if __STDC__
	va_start(arglist, format);
#else
	va_start(arglist);
#endif
	count = _fmt(fp, storep, format, arglist);
	va_end(arglist);
	return(count);
}


/*
 *	tsprintf
 *
 *	Function:	- prints memory location pointed to by storep
 *	Accepts:	- char pointer to destination
 *			- format string
 *			- variable number of items to format
 *	Returns:	- char pointer to destination
 */	
#if LAM_WANT_PROTOS
char *
tsprintf(char *storep, const char *format, ...)
#else
char *
tsprintf(storep, format, va_alist)

char			*storep;
char			*format;
va_dcl

#endif

{
	TFILE		*fp = 0;
	int		count;
	va_list		arglist;

#if __STDC__
	va_start(arglist, format);
#else
	va_start(arglist);
#endif
	count = _fmt(fp, storep, format, arglist);
	va_end(arglist);
	
	return(storep + count);
}


/*
 *	tvprintf
 *
 *	Function:	- same as printf(), but this takes a va_list
 *	Accepts:	- format string
 *			- variable number of items to format in a va_list
 *	Returns:	- the number of printed characters, of EOF if error
 */
#if LAM_WANT_PROTOS
int
tvprintf(const char *format, ...)
#else
int
tvprintf(format, va_alist)

char			*format;
va_dcl

#endif

{
	char		*storep	= 0;
	TFILE		*fp	= tstdout;
	int		count;
	va_list		ap;

#if __STDC__
	va_start(ap, format);
#else
	va_start(ap);
#endif
	count = _fmt(fp, storep, format, ap);
	va_end(ap);
	return(count);
}


/*
 *      tvfprintf
 *
 *	Function:	- same as fprintf(), but this takes a va_list
 *	Accepts:	- file pointer
 *			- format string
 *			- variable number of items to format in a va_list
 *	Returns:	- the number of printed characters, of EOF if error
 */
#if LAM_WANT_PROTOS
int
tvfprintf(TFILE *fp, const char *format, ...)
#else
int
tvfprintf(fp, format, va_alist)

TFILE			*fp;
char			*format;
va_dcl

#endif

{
	char		*storep	= 0;
	int		count;
	va_list		ap;

#if __STDC__
	va_start(ap, format);
#endif
	count = _fmt(fp, storep, format, ap);
#if __STDC__
	va_end(ap);
#endif
	return(count);
}


/*
 *	tvsprintf
 *
 *	Function:	- same as sprintf, but takes a va_list
 *	Accepts:	- char pointer to destination
 *			- format string
 *			- variable number of items to format in a va_list
 *	Returns:	- char pointer to destination
 */
#if LAM_WANT_PROTOS
char *
tvsprintf(char *storep, const char *format, ...)
#else
char *
tvsprintf(storep, format, va_alist)

char			*storep;
char			*format;
va_dcl

#endif

{
	TFILE		*fp = 0;
	int		count;
	va_list		ap;

#if __STDC__
	va_start(ap, format);
#endif
	count = _fmt(fp, storep, format, ap);
#if __STDC__
	va_end(ap);
#endif
	return(storep + count);
}
