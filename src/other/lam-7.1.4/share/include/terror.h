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
 *	$Id: terror.h,v 6.13 2003/09/27 05:16:17 brbarret Exp $
 *
 *	Function:	- Trollius error codes
 */

#ifndef _TERROR
#define _TERROR

#include <lam_config.h>
#include <typical.h>

#ifdef WIN32
#include <windows.h>
#undef ERROR
#endif

/*
 * Bit 29 is set for Win32 applications.
 */
#ifdef WIN32
#define LAM_ERR_BASE		0x20000100	/* Win32 appl reserved */
#define LAM_ERRNO_BASE		0x20000000
#else
#define LAM_ERR_BASE		0x400
#define LAM_ERRNO_BASE		0
#endif

#define ELOW			(201 + LAM_ERR_BASE)
#define EUNKNOWN		(201 + LAM_ERR_BASE)
#define EDEADNODE		(202 + LAM_ERR_BASE)
#define ENOEXDESCRIPTORS	(203 + LAM_ERR_BASE)
#define EFULL			(204 + LAM_ERR_BASE)
#define ENOTPROCESS		(205 + LAM_ERR_BASE)
#define EBADREQUEST		(206 + LAM_ERR_BASE)
#define ENOTMALLOCED		(207 + LAM_ERR_BASE)
#define EGERFLOW		(208 + LAM_ERR_BASE)
#define ESERVICE		(209 + LAM_ERR_BASE)
#define EMAGIC			(210 + LAM_ERR_BASE)
#define EBLOWN			(211 + LAM_ERR_BASE)
#define EIMPOSSIBLE		(212 + LAM_ERR_BASE)
#define LAM_ERR_TO		(213 + LAM_ERR_BASE)
#define EBADHOST		(214 + LAM_ERR_BASE)
#define ENOKERNEL		(215 + LAM_ERR_BASE)
#define ENOTATTACHED		(216 + LAM_ERR_BASE)
#define LAM_ERR_BLOCK		(217 + LAM_ERR_BASE)
#define EMSGLEN			(218 + LAM_ERR_BASE)
#define EEOF			(219 + LAM_ERR_BASE)
#define ECOLLIDE		(220 + LAM_ERR_BASE)
#define LAM_ERR_INVAL		(221 + LAM_ERR_BASE)
#define ESHMMAXP		(222 + LAM_ERR_BASE)
#define ESHARE			(223 + LAM_ERR_BASE)
#define EBADLINK		(224 + LAM_ERR_BASE)
#define EACK			(225 + LAM_ERR_BASE)
#define	EUSAGE			(226 + LAM_ERR_BASE)
#define EOPTION			(227 + LAM_ERR_BASE)
#define ESEMCREATE		(228 + LAM_ERR_BASE)
#define	EBADNODE		(229 + LAM_ERR_BASE)
#define	EBADIDSPEC		(230 + LAM_ERR_BASE)
#define ENONODES		(231 + LAM_ERR_BASE)
#define ENORDESCRIPTORS		(232 + LAM_ERR_BASE)
#define ENOBFDESCRIPTORS	(233 + LAM_ERR_BASE)
#define ENOBTDESCRIPTORS	(234 + LAM_ERR_BASE)
#define ENOFLDESCRIPTORS	(235 + LAM_ERR_BASE)
#define ENOPDESCRIPTORS		(236 + LAM_ERR_BASE)
#define ENOKDESCRIPTORS		(237 + LAM_ERR_BASE)
#define EBADMOSESREQ		(238 + LAM_ERR_BASE)
#define ESHMCREATE		(239 + LAM_ERR_BASE)
#define EBADTAG			(240 + LAM_ERR_BASE)
#define EDELETE			(241 + LAM_ERR_BASE)
#define ENOBOOTSTRAP		(242 + LAM_ERR_BASE)
#define EBADREC			(243 + LAM_ERR_BASE)
#define EBADASCIINUMB		(244 + LAM_ERR_BASE)
#define EHIGHWATER		(245 + LAM_ERR_BASE)
#define EINGROUP		(246 + LAM_ERR_BASE)
#define EGMAXPROC		(247 + LAM_ERR_BASE)
#define EGSINGLETON		(248 + LAM_ERR_BASE)
#define EGNOTPROC		(249 + LAM_ERR_BASE)
#define EBADGROUP		(250 + LAM_ERR_BASE)
#define EMPIINIT		(251 + LAM_ERR_BASE)
#define EMPINOINIT		(252 + LAM_ERR_BASE)
#define LAM_ERR_DUPVC		(253 + LAM_ERR_BASE)
#define LAM_ERR_BADVC		(254 + LAM_ERR_BASE)
#define EPUBLISHED		(255 + LAM_ERR_BASE)
#define ENOTPUBLISHED		(256 + LAM_ERR_BASE)
#define ENOPTYSUPPORT		(257 + LAM_ERR_BASE)
#define ENOPFDSUPPORT		(258 + LAM_ERR_BASE)
#define EMPIALREADYFINALIZE	(259 + LAM_ERR_BASE)
#define EMPIMEMERROR            (260 + LAM_ERR_BASE)
#define ENOT_IMPLEMENTED	(261 + LAM_ERR_BASE)

#define EHIGH			(261 + LAM_ERR_BASE)

/*
 * exit code signifying a LAM error
 */
#define LAM_EEXIT		253

/*
 * abstract macros for error code manipulation
 * We are attempting to solve the GetLastError() vs. errno problem
 * in Win32 C.
 */
#ifdef WIN32
#define LAMGetLastError() GetLastError()
#define LAMSetLastError(e) SetLastError(e)
#else
#include <errno.h>
#define LAMGetLastError() errno
#define LAMSetLastError(e) (errno = (e))
#endif

/*
 * other limits
 */
#define LAM_EMSGBUFLEN		320

#ifdef __cplusplus
extern "C" {
#endif

extern void		lam_errorstr __ARGS((char *emsgbuf, unint emsglen));
extern void		lam_perror __ARGS((const char *umsg));
extern void		lamfail __ARGS((char *umsg));
extern void		lampanic __ARGS((char *umsg));

extern void		terror __ARGS((const char *msg));

#ifdef __cplusplus
}
#endif

#endif /* _TERROR */
