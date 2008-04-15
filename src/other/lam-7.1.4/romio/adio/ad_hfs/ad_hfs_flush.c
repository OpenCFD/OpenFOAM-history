/* -*- Mode: C; c-basic-offset:4 ; -*- */
/* 
 *   $Id: ad_hfs_flush.c,v 1.3 2003/03/25 22:39:55 nsanghvi Exp $    
 *
 *   Copyright (C) 1997 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "ad_hfs.h"

void ADIOI_HFS_Flush(ADIO_File fd, int *error_code)
{
    ADIOI_GEN_Flush(fd, error_code);
}
