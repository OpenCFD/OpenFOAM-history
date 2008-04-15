/* -*- Mode: C; c-basic-offset:4 ; -*- */
/* 
 *   $Id: ad_sfs_seek.c,v 1.3 2003/03/25 22:40:24 nsanghvi Exp $    
 *
 *   Copyright (C) 1997 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "ad_sfs.h"

ADIO_Offset ADIOI_SFS_SeekIndividual(ADIO_File fd, ADIO_Offset offset, 
		      int whence, int *error_code)
{
    return ADIOI_GEN_SeekIndividual(fd, offset, whence, error_code);
}
