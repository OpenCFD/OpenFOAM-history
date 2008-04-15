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
 *
 * NOTE: if this file is changed, be sure to update the assembly as well:
 *  % sh update.sh powerpc
 *
 * The per-architecture assembly is in a seperate file for each
 * architecture to make it easy to create the raw assembly files from
 * the header file for those silly compilers that don't do inline
 * assembly.
 */

#define SYNC_WRITE __asm__ __volatile__ ("sync\n")

static inline void
lam_ssi_rpi_usysv_syncwrite(void)
{
    SYNC_WRITE;
}
