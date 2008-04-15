/* -*- Mode: C; c-basic-offset:4 ; -*- */
/* 
 *   $Id: ad_sfs_flush.c,v 1.3 2003/03/25 22:40:16 nsanghvi Exp $    
 *
 *   Copyright (C) 1997 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "ad_sfs.h"

void ADIOI_SFS_Flush(ADIO_File fd, int *error_code)
{
#ifndef PRINT_ERR_MSG
    static char myname[] = "ADIOI_SFS_FLUSH";
#endif

     /* there is no fsync on SX-4 */
#ifdef PRINT_ERR_MSG
     *error_code = MPI_ERR_UNKNOWN; 
#else
     *error_code = MPIR_Err_setmsg(MPI_ERR_UNSUPPORTED_OPERATION, 1,
			      myname, (char *) 0, (char *) 0);
     ADIOI_Error(fd, *error_code, myname);	    
#endif
}
