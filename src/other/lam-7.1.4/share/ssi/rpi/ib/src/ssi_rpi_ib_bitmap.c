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
 * $Id: ssi_rpi_ib_bitmap.c,v 1.3 2003/12/29 22:00:41 vsahay Exp $
 *
 *	Function:	- Bitmap implementation
 */


#include <rpi_ib_bitmap.h>

lam_ssi_rpi_ib_bitmap*
lam_ssi_rpi_ib_bitmap_init(int numelem)
{
  int  i, remainder_bits;
  lam_ssi_rpi_ib_bitmap *map;
  int bsize;

  bsize = numelem / sizeof(int);
  if ((numelem % (sizeof(int))) != 0)
    ++bsize;

  map = (lam_ssi_rpi_ib_bitmap *) malloc (sizeof(lam_ssi_rpi_ib_bitmap));
  if (map == NULL)
    return NULL;
  map->bmap = (int *) malloc (bsize * sizeof(int));

  /* Zero out the map */
  for (i = 0; i < bsize; ++i)
    map->bmap[i] = 0;

  if (map->bmap == NULL)
    return NULL;

  map->bsize = numelem;
  remainder_bits = (numelem % sizeof(int));
  map->num_available_bits = (remainder_bits ? (sizeof(int) - remainder_bits)
			     : 0);
  map->bnum_ints = bsize;
  return map;
}


int
lam_ssi_rpi_ib_bitmap_extend(lam_ssi_rpi_ib_bitmap *map, int nelem)
{
  int *newspace;
  int addsize;
  int i;

  /* We need to expand the bitmap -- new space */		       
  if (nelem > map->num_available_bits) {

    /* Is the newspace needed more than double the space bitmap
       occupies now (which is the default increment length */
    if (nelem / sizeof(int) > map->bnum_ints)
      addsize = nelem / sizeof(int) + ((nelem % sizeof(int) == 0) ? 0 : 1);
    else
      addsize = map->bnum_ints;
    newspace = (int *) malloc ((map->bnum_ints + addsize) *  sizeof(int));
    if (newspace == NULL)
      return LAMERROR;

    /* Copy over the older bitmap */
    for (i = 0; i < map->bnum_ints; ++i)
      newspace[i] = map->bmap[i];

    /* Zero out the new space */
    for (i = map->bnum_ints; i < (map->bnum_ints + addsize); ++i)
      newspace[i] = 0;
    
    /* free the old bitmap bmap */
    free(map->bmap);

    map->bmap = newspace;
    map->bnum_ints += addsize;
    map->bsize += nelem;
    map->num_available_bits += addsize * sizeof(int) - nelem;
  } else {
    /* we still have some bits left in this bitmap */
    map->num_available_bits -= nelem;
    map->bsize += nelem;
  }
    

  return 0;
}


int
lam_ssi_rpi_ib_bitmap_set_elem(lam_ssi_rpi_ib_bitmap *bmap, int i) 
{
  int index, bit;

  if (i >= bmap->bnum_ints) /* Index starts from 0  */
    return -1;

  index = i / (sizeof(int));
  bit = 1 << (i % (sizeof(int)));
  
  bmap->bmap[index] |= bit;
  
  return 0;
}


int
lam_ssi_rpi_ib_bitmap_clear_elem(lam_ssi_rpi_ib_bitmap *bmap, int i)
{
  int index, bit;
 
  if (i >= bmap->bsize) /* Index starts from 0  */
    return -1;
 
  index = i / (sizeof(int));
  bit = 1 << (i % (sizeof(int)));
 
  bmap->bmap[index] &= (~bit);
 
  return 0;
}


int
lam_ssi_rpi_ib_bitmap_is_set(lam_ssi_rpi_ib_bitmap *bmap, int i)
{
  int index, bit;
 
  if (i >= bmap->bsize) /* Index starts from 0  */
    return 0;
 
  index = i / (sizeof(int));
  bit = 1 << (i % (sizeof(int)));
 
  if (bmap->bmap[index] & bit)
    return 1;
  else
    return 0;
}


int
lam_ssi_rpi_ib_bitmap_destroy(lam_ssi_rpi_ib_bitmap *bmap)
{
  if (bmap != NULL) {
    free(bmap->bmap);
    free(bmap);
  }
  return 0;
}
