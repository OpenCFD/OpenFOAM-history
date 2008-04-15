/* -*- Mode: C; c-basic-offset:4 ; -*- */
/* 
 *   $Id: ad_nfs.c,v 1.1 2003/03/25 22:39:58 nsanghvi Exp $
 *
 *   Copyright (C) 2001 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "ad_nfs.h"

/* adioi.h has the ADIOI_Fns_struct define */
#include "adioi.h"

struct ADIOI_Fns_struct ADIO_NFS_operations = {
    ADIOI_NFS_Open, /* Open */
    ADIOI_NFS_ReadContig, /* ReadContig */
    ADIOI_NFS_WriteContig, /* WriteContig */
    ADIOI_GEN_ReadStridedColl, /* ReadStridedColl */
    ADIOI_GEN_WriteStridedColl, /* WriteStridedColl */
    ADIOI_GEN_SeekIndividual, /* SeekIndividual */
    ADIOI_NFS_Fcntl, /* Fcntl */
    ADIOI_GEN_SetInfo, /* SetInfo */
    ADIOI_NFS_ReadStrided, /* ReadStrided */
    ADIOI_NFS_WriteStrided, /* WriteStrided */
    ADIOI_NFS_Close, /* Close */
    ADIOI_NFS_IreadContig, /* IreadContig */
    ADIOI_NFS_IwriteContig, /* IwriteContig */
    ADIOI_NFS_ReadDone, /* ReadDone */
    ADIOI_NFS_WriteDone, /* WriteDone */
    ADIOI_NFS_ReadComplete, /* ReadComplete */
    ADIOI_NFS_WriteComplete, /* WriteComplete */
    ADIOI_NFS_IreadStrided, /* IreadStrided */
    ADIOI_NFS_IwriteStrided, /* IwriteStrided */
    ADIOI_GEN_Flush, /* Flush */
    ADIOI_NFS_Resize, /* Resize */
    ADIOI_GEN_Delete, /* Delete */
};
