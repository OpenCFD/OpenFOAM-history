/* -*- Mode: C; c-basic-offset:4 ; -*- */
/* 
 *   $Id: ad_sfs_hints.c,v 1.3 2003/03/25 22:40:16 nsanghvi Exp $    
 *
 *   Copyright (C) 1997 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "ad_sfs.h"

void ADIOI_SFS_SetInfo(ADIO_File fd, MPI_Info users_info, int *error_code)
{
    ADIOI_GEN_SetInfo(fd, users_info, error_code); 
}
