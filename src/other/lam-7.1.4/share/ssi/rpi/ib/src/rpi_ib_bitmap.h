/*
 * Copyright (c) 2001-2004 The Trustees of Indiana University.  
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
 * $Id: ssi_rpi_ib_bitmap.c,v 1.3 2003/12/29 22:00:41 vsahay Exp $
 *
 *	Function:	- Bitmap implementation
 */

#ifndef _IB_BITMAP_H_
#define _IB_BITMAP_H_

#include <stdio.h>
#include <stdlib.h>
#include <typical.h> /* for LAMERROR */


typedef struct lam_ssi_rpi_ib_bitmap_t {
  int *bmap;
  int bsize;
  int bnum_ints;
  int num_available_bits;
}lam_ssi_rpi_ib_bitmap;

lam_ssi_rpi_ib_bitmap *lam_ssi_rpi_ib_bitmap_init(int numelem);

int lam_ssi_rpi_ib_bitmap_extend(lam_ssi_rpi_ib_bitmap *bmap, int num_elements);

int lam_ssi_rpi_ib_bitmap_set_elem(lam_ssi_rpi_ib_bitmap *bmap, int index);

int lam_ssi_rpi_ib_bitmap_clear_elem (lam_ssi_rpi_ib_bitmap *bmap, int index);

int lam_ssi_rpi_ib_bitmap_is_set(lam_ssi_rpi_ib_bitmap *bmap, int index);

int lam_ssi_rpi_ib_bitmap_destroy(lam_ssi_rpi_ib_bitmap *bmap);

/* More functionalities later on  */

/* clearall  */

/* setall  */

#endif
