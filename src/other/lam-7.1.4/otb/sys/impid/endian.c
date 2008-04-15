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
 * $Id: endian.c,v 1.6 2003/05/05 22:49:05 ssankara Exp $
 */

#include <lam_config.h>
#if LAM_WANT_IMPI

#include <stdio.h>

#include <mpisys.h>
#include <t_types.h>
#include <impi.h>
#include <impi-defs.h>
#include <impid.h>


void
lam_swap_endian(void *buf, int size)
{
#ifndef WORDS_BIGENDIAN
  char temp[64];

  switch(size) {
  case 2:
    ttoli2(buf, temp);
    lam_memcpy(buf, temp, 2);
    break;
  case 4:
    ttoli4(buf, temp);
    lam_memcpy(buf, temp, 4);
    break;
  case 8:
    ttoli8(buf, temp);
    lam_memcpy(buf, temp, 8);
    break;
  case 16:
    /* The only 16 byte swaps that we do are for the IPV6 addresses,
       which we manually paste the IPV4 address into the right nybbles
       anyway.  So make this a noop. */
    break;
  default:
    fprintf(stderr, "Unknown endian swap size: %d\n", size);
    break;
  }
#endif
}

#endif
