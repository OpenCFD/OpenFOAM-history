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
 *	$Id: rpi_usysv_macros.h,v 1.2 2002/10/09 21:00:43 brbarret Exp $
 *
 *	Function:	- shared memory / TCP client-to-client interface header
 */

#ifndef LAM_RPI_USYSV_MACROS_H
#define LAM_RPI_USYSV_MACROS_H

#include <errno.h>


/*
 * constants for shared memory allocator internals
 */
#define LAM_LOG_ALIGN	4
#define LAM_ALIGNMENT	(1 << LAM_LOG_ALIGN)


/*
 * lock control
 */
#define READPOLLCOUNT	10000			/* # loops before usleep() */
#define WRITEPOLLCOUNT	10			/* # loops before usleep() */


/*
 * locking macros
 */
#define lam_ssi_rpi_usysv_readtrylock(p)  \
	((semop((p)->cp_ppsem, (p)->cp_top, 1)) ? \
		((errno == EAGAIN) ? 1 : -1) : 0)

#define lam_ssi_rpi_usysv_writetrylock(p) \
	((semop((p)->cp_ppsem, (p)->cp_top + 1, 1)) ? \
		((errno == EAGAIN) ? 1 : -1) : 0)

#define lam_ssi_rpi_usysv_readunlock(p) \
	semop((p)->cp_ppsem, (p)->cp_uop, 1)
#define lam_ssi_rpi_usysv_writeunlock(p) \
	semop((p)->cp_ppsem, (p)->cp_uop + 1, 1)

#endif /* LAM_SSI_RPI_USYSV_MACROS_H */
