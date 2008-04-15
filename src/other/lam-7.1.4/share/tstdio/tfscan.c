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
 *	Very confusing software.  See offeeecial copyrights below.
 *	Modified for LAM by JRV
 *
 *	We are just putting our stamp on this for housekeeping reasons.
 *
 *	Function	- tstdio user routines for scanning
 */

/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
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
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)fscan.c	based on 5.1 (Berkeley) 1/20/91";
#endif /* LIBC_SCCS and not lint */

#include <string.h>
#if __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif
#include <lam_config.h>
#include <laminternal.h>
#include <tstdio.h>

/*
 * external functions
 */
extern int		lam_svfscanf();

#if LAM_WANT_PROTOS
int
tfscanf(TFILE *fp, const char *fmt, ...) 
#else
int
tfscanf(fp, fmt, va_alist)
	TFILE *fp;
	char *fmt;
	va_dcl
#endif
{
	int ret;
	va_list ap;

#if __STDC__
	va_start(ap, fmt);
#else
	va_start(ap);
#endif
	ret = lam_svfscanf(fp, fmt, ap);
	va_end(ap);
	return (ret);
}


#if LAM_WANT_PROTOS
int
tscanf(char const *fmt, ...)
#else
int
tscanf(fmt, va_alist)
	char *fmt;
	va_dcl
#endif
{
	int ret;
	va_list ap;

#if __STDC__
	va_start(ap, fmt);
#else
	va_start(ap);
#endif
	ret = lam_svfscanf(tstdin, fmt, ap);
	va_end(ap);
	return (ret);
}



/* ARGSUSED */
/*
 * static int
 * eofread(cookie, buf, len)
 * 	void *cookie;
 * 	char *buf;
 * 	int len;
 * {
 * 
 * 	return (0);
 * }
 */

#if LAM_WANT_PROTOS
int
tsscanf(char *str, char const *fmt, ...)
#else
int
tsscanf(str, fmt, va_alist)
	char *str;
	char *fmt;
	va_dcl
#endif
{
	int ret;
	va_list ap;
	TFILE f;

	f._base = (unsigned char *)str;
	f._bsiz = strlen(str);

#if __STDC__
	va_start(ap, fmt);
#else
	va_start(ap);
#endif
	ret = lam_svfscanf(&f, fmt, ap);
	va_end(ap);
	return (ret);
}
