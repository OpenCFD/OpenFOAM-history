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
 * $Id: rpi_tcp_macros.h,v 1.3 2002/10/21 04:37:10 brbarret Exp $
 *
 *	Function:	- TCP client-to-client interface header
 */

#ifndef LAM_SSI_RPI_TCP_MACROS_H
#define LAM_SSI_RPI_TCP_MACROS_H

/*
 * data conversion for client-to-client envelopes
 */

#if LAM_WORDS_BIGENDIAN
#define lam_set_out_envelope_m(r) lam_set_out_envelope_no_swap_m(r)
#else	/* not WORDS_BIGENDIAN */
#define lam_set_out_envelope_m(r) lam_set_out_envelope_endian_swap_m(r)
#endif	/* not WORDS_BIGENDIAN */


#define lam_set_out_envelope_no_swap_m(r) \
	{ \
		(r)->cq_nenvout = sizeof(struct lam_ssi_rpi_envl); \
		(r)->cq_envbuf = (char *) &((r)->cq_env); \
	}

#define lam_set_out_envelope_endian_swap_m(r) \
	if (lam_homog) { \
		(r)->cq_nenvout = sizeof(struct lam_ssi_rpi_envl); \
		(r)->cq_envbuf = (char *) &((r)->cq_env); \
	} \
	else { \
		(r)->cq_nenvout = sizeof(struct lam_ssi_rpi_envl); \
		(r)->cq_outenv = (r)->cq_env; \
		mltoti4((int4 *) &((r)->cq_outenv), \
			sizeof(struct lam_ssi_rpi_envl) / sizeof(int4)); \
		(r)->cq_envbuf = (char *) &((r)->cq_outenv); \
	}



#endif	/* LAM_SSI_RPI_TCP_MACROS_H */
