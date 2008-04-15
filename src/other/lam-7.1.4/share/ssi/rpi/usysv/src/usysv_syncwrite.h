/*
 * Copyright (c) 2001-2005 The Trustees of Indiana University.  
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
 * Function: write sync instructions / assembly for PowerPC
 */

/*
 * Forward declare the sync function - may never be used if compiler
 * supports inline assembly instructions
 */
#ifndef __GNUC__
void lam_ssi_rpi_usysv_syncwrite(void);
#endif

/*
 * Set the SYNC_WRITE macro (normally empty)
 */
#if LAM_ASSEMBLY_TYPE_POWERPC
#ifdef __GNUC__
#include "usysv_syncwrite_powerpc.h"
#else
#define SYNC_WRITE lam_ssi_rpi_usysv_syncwrite()
#endif

#else
/* Add other archs as needed here */
#define SYNC_WRITE
#endif
