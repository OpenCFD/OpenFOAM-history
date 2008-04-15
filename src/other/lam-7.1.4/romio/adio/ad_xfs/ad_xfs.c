/* -*- Mode: C; c-basic-offset:4 ; -*- */
/* 
 *   $Id: ad_xfs.c,v 1.1 2003/03/25 22:40:34 nsanghvi Exp $
 *
 *   Copyright (C) 2001 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "ad_xfs.h"

/* adioi.h has the ADIOI_Fns_struct define */
#include "adioi.h"

struct ADIOI_Fns_struct ADIO_XFS_operations = {
    ADIOI_XFS_Open, /* Open */
    ADIOI_XFS_ReadContig, /* ReadContig */
    ADIOI_XFS_WriteContig, /* WriteContig */
    ADIOI_GEN_ReadStridedColl, /* ReadStridedColl */
    ADIOI_GEN_WriteStridedColl, /* WriteStridedColl */
    ADIOI_XFS_SeekIndividual, /* SeekIndividual */
    ADIOI_XFS_Fcntl, /* Fcntl */
    ADIOI_XFS_SetInfo, /* SetInfo */
    ADIOI_GEN_ReadStrided, /* ReadStrided */
    ADIOI_GEN_WriteStrided, /* WriteStrided */
    ADIOI_XFS_Close, /* Close */
    ADIOI_XFS_IreadContig, /* IreadContig */
    ADIOI_XFS_IwriteContig, /* IwriteContig */
    ADIOI_XFS_ReadDone, /* ReadDone */
    ADIOI_XFS_WriteDone, /* WriteDone */
    ADIOI_XFS_ReadComplete, /* ReadComplete */
    ADIOI_XFS_WriteComplete, /* WriteComplete */
    ADIOI_XFS_IreadStrided, /* IreadStrided */
    ADIOI_XFS_IwriteStrided, /* IwriteStrided */
    ADIOI_GEN_Flush, /* Flush */
    ADIOI_XFS_Resize, /* Resize */
    ADIOI_GEN_Delete, /* Delete */
};
