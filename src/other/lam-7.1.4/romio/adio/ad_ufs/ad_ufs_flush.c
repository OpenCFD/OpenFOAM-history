/* -*- Mode: C; c-basic-offset:4 ; -*- */
/* 
 *   $Id: ad_ufs_flush.c,v 1.3 2003/03/25 22:40:31 nsanghvi Exp $    
 *
 *   Copyright (C) 1997 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "ad_ufs.h"

void ADIOI_UFS_Flush(ADIO_File fd, int *error_code)
{
    ADIOI_GEN_Flush(fd, error_code);
}
