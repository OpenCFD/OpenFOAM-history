/* -*- Mode: C; c-basic-offset:4 ; -*- */
/* 
 *   $Id: ad_hints.c,v 1.3 2003/03/25 22:40:38 nsanghvi Exp $    
 *
 *   Copyright (C) 1997 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "adio.h"

void ADIOI_GEN_SetInfo(ADIO_File fd, MPI_Info users_info, int *error_code)
{
/* if fd->info is null, create a new info object. 
   Initialize fd->info to default values.
   Initialize fd->hints to default values.
   Examine the info object passed by the user. If it contains values that
   ROMIO understands, override the default. */

    MPI_Info info;
    char *value;
    int flag, intval, tmp_val, nprocs;

    if (fd->info == MPI_INFO_NULL) MPI_Info_create(&(fd->info));
    info = fd->info;

    /* Note that fd->hints is allocated at file open time; thus it is
     * not necessary to allocate it, or check for allocation, here.
     */

    value = (char *) ADIOI_Malloc((MPI_MAX_INFO_VAL+1)*sizeof(char));
    if (value == NULL) {
	/* NEED TO HANDLE ENOMEM */
    }

    /* initialize info and hints to default values if they haven't been
     * previously initialized
     */
    if (!fd->hints->initialized) {
	/* buffer size for collective I/O */
	MPI_Info_set(info, "cb_buffer_size", ADIOI_CB_BUFFER_SIZE_DFLT); 
	fd->hints->cb_buffer_size = atoi(ADIOI_CB_BUFFER_SIZE_DFLT);

	/* default is to let romio automatically decide when to use
	 * collective buffering
	 */
	MPI_Info_set(info, "romio_cb_read", "automatic"); 
	fd->hints->cb_read = ADIOI_HINT_AUTO;
	MPI_Info_set(info, "romio_cb_write", "automatic"); 
	fd->hints->cb_write = ADIOI_HINT_AUTO;

	fd->hints->cb_config_list = NULL;

	/* number of processes that perform I/O in collective I/O */
	MPI_Comm_size(fd->comm, &nprocs);
	sprintf(value, "%d", nprocs);
	MPI_Info_set(info, "cb_nodes", value);
	fd->hints->cb_nodes = nprocs;

	/* hint indicating that no indep. I/O will be performed on this file */
	MPI_Info_set(info, "romio_no_indep_rw", "false");
	fd->hints->no_indep_rw = 0;

	/* buffer size for data sieving in independent reads */
	MPI_Info_set(info, "ind_rd_buffer_size", ADIOI_IND_RD_BUFFER_SIZE_DFLT);
	fd->hints->ind_rd_buffer_size = atoi(ADIOI_IND_RD_BUFFER_SIZE_DFLT);

	/* buffer size for data sieving in independent writes */
	MPI_Info_set(info, "ind_wr_buffer_size", ADIOI_IND_WR_BUFFER_SIZE_DFLT);
	fd->hints->ind_wr_buffer_size = atoi(ADIOI_IND_WR_BUFFER_SIZE_DFLT);

	/* default is to let romio automatically decide when to use data
	 * sieving
	 */
	MPI_Info_set(info, "romio_ds_read", "automatic"); 
	fd->hints->ds_read = ADIOI_HINT_AUTO;
	MPI_Info_set(info, "romio_ds_write", "automatic"); 
	fd->hints->ds_write = ADIOI_HINT_AUTO;

	fd->hints->initialized = 1;
    }

    /* add in user's info if supplied */
    if (users_info != MPI_INFO_NULL) {
	MPI_Info_get(users_info, "cb_buffer_size", MPI_MAX_INFO_VAL, 
		     value, &flag);
	if (flag && ((intval=atoi(value)) > 0)) {
	    tmp_val = intval;
	    MPI_Bcast(&tmp_val, 1, MPI_INT, 0, fd->comm);
	    if (tmp_val != intval) {
		FPRINTF(stderr, "ADIOI_GEN_SetInfo: the value for key \"cb_buffer_size\" must be the same on all processes\n");
		MPI_Abort(MPI_COMM_WORLD, 1);
	    }
	    else {
		MPI_Info_set(info, "cb_buffer_size", value);
		fd->hints->cb_buffer_size = intval;
	    }
	}

	/* new hints for enabling/disabling coll. buffering on
	 * reads/writes
	 */
	MPI_Info_get(users_info, "romio_cb_read", MPI_MAX_INFO_VAL, value, &flag);
	if (flag) {
	    if (!strcmp(value, "enable") || !strcmp(value, "ENABLE")) {
		MPI_Info_set(info, "romio_cb_read", value);
		fd->hints->cb_read = ADIOI_HINT_ENABLE;
	    }
	    else if (!strcmp(value, "disable") || !strcmp(value, "DISABLE")) {
		MPI_Info_set(info, "romio_cb_read", value);
		fd->hints->cb_read = ADIOI_HINT_DISABLE;
	    }
	    else if (!strcmp(value, "automatic") || !strcmp(value, "AUTOMATIC"))
	    {
		MPI_Info_set(info, "romio_cb_read", value);
		fd->hints->cb_read = ADIOI_HINT_AUTO;
	    }

	    tmp_val = fd->hints->cb_read;
	    MPI_Bcast(&tmp_val, 1, MPI_INT, 0, fd->comm);
	    if (tmp_val != fd->hints->cb_read) {
		FPRINTF(stderr, "ADIOI_GEN_SetInfo: the value for key \"romio_cb_read\" must be the same on all processes\n");
		MPI_Abort(MPI_COMM_WORLD, 1);
	    }
	}
	MPI_Info_get(users_info, "romio_cb_write", MPI_MAX_INFO_VAL, value, &flag);
	if (flag) {
	    if (!strcmp(value, "enable") || !strcmp(value, "ENABLE")) {
		MPI_Info_set(info, "romio_cb_write", value);
		fd->hints->cb_write = ADIOI_HINT_ENABLE;
	    }
	    else if (!strcmp(value, "disable") || !strcmp(value, "DISABLE")) {
		MPI_Info_set(info, "romio_cb_write", value);
		fd->hints->cb_write = ADIOI_HINT_DISABLE;
	    }
	    else if (!strcmp(value, "automatic") || !strcmp(value, "AUTOMATIC"))
	    {
		MPI_Info_set(info, "romio_cb_write", value);
		fd->hints->cb_write = ADIOI_HINT_AUTO;
	    }
	
	    tmp_val = fd->hints->cb_write;
	    MPI_Bcast(&tmp_val, 1, MPI_INT, 0, fd->comm);
	    if (tmp_val != fd->hints->cb_write) {
		FPRINTF(stderr, "ADIOI_GEN_SetInfo: the value for key \"romio_cb_write\" must be the same on all processes\n");
		MPI_Abort(MPI_COMM_WORLD, 1);
	    }
	}

	/* new hint for specifying no indep. read/write will be performed */
	MPI_Info_get(users_info, "romio_no_indep_rw", MPI_MAX_INFO_VAL, value,
		     &flag);
	if (flag) {
	    if (!strcmp(value, "true") || !strcmp(value, "TRUE")) {
		MPI_Info_set(info, "romio_no_indep_rw", value);
		fd->hints->no_indep_rw = 1;
		tmp_val = 1;
	    }
	    else if (!strcmp(value, "false") || !strcmp(value, "FALSE")) {
		MPI_Info_set(info, "romio_no_indep_rw", value);
		fd->hints->no_indep_rw = 0;
		tmp_val = 0;
	    }
	    else {
		/* default is above */
		tmp_val = 0;
	    }
	    MPI_Bcast(&tmp_val, 1, MPI_INT, 0, fd->comm);
	    if (tmp_val != fd->hints->no_indep_rw) {
		FPRINTF(stderr, "ADIOI_GEN_SetInfo: the value for key \"romio_no_indep_rw\" must be the same on all processes\n");
		MPI_Abort(MPI_COMM_WORLD, 1);
	    }
	}
	/* new hints for enabling/disabling data sieving on
	 * reads/writes
	 */
	MPI_Info_get(users_info, "romio_ds_read", MPI_MAX_INFO_VAL, value, 
		     &flag);
	if (flag) {
	    if (!strcmp(value, "enable") || !strcmp(value, "ENABLE")) {
		MPI_Info_set(info, "romio_ds_read", value);
		fd->hints->ds_read = ADIOI_HINT_ENABLE;
	    }
	    else if (!strcmp(value, "disable") || !strcmp(value, "DISABLE")) {
		MPI_Info_set(info, "romio_ds_read", value);
		fd->hints->ds_read = ADIOI_HINT_DISABLE;
	    }
	    else if (!strcmp(value, "automatic") || !strcmp(value, "AUTOMATIC"))
	    {
		MPI_Info_set(info, "romio_ds_read", value);
		fd->hints->ds_read = ADIOI_HINT_AUTO;
	    }
	    /* otherwise ignore */
	}
	MPI_Info_get(users_info, "romio_ds_write", MPI_MAX_INFO_VAL, value, 
		     &flag);
	if (flag) {
	    if (!strcmp(value, "enable") || !strcmp(value, "ENABLE")) {
		MPI_Info_set(info, "romio_ds_write", value);
		fd->hints->ds_write = ADIOI_HINT_ENABLE;
	    }
	    else if (!strcmp(value, "disable") || !strcmp(value, "DISABLE")) {
		MPI_Info_set(info, "romio_ds_write", value);
		fd->hints->ds_write = ADIOI_HINT_DISABLE;
	    }
	    else if (!strcmp(value, "automatic") || !strcmp(value, "AUTOMATIC"))
	    {
		MPI_Info_set(info, "romio_ds_write", value);
		fd->hints->ds_write = ADIOI_HINT_AUTO;
	    }
	    /* otherwise ignore */
	}

	MPI_Info_get(users_info, "cb_nodes", MPI_MAX_INFO_VAL, 
		     value, &flag);
	if (flag && ((intval=atoi(value)) > 0)) {
	    tmp_val = intval;
	    MPI_Bcast(&tmp_val, 1, MPI_INT, 0, fd->comm);
	    if (tmp_val != intval) {
		FPRINTF(stderr, "ADIOI_GEN_SetInfo: the value for key \"cb_nodes\" must be the same on all processes\n");
		MPI_Abort(MPI_COMM_WORLD, 1);
	    }
	    else {
		if (intval < nprocs) {
		    MPI_Info_set(info, "cb_nodes", value);
		    fd->hints->cb_nodes = intval;
		}
	    }
	}

	MPI_Info_get(users_info, "ind_wr_buffer_size", MPI_MAX_INFO_VAL, 
		     value, &flag);
	if (flag && ((intval = atoi(value)) > 0)) {
	    MPI_Info_set(info, "ind_wr_buffer_size", value);
	    fd->hints->ind_wr_buffer_size = intval;
	}

	MPI_Info_get(users_info, "ind_rd_buffer_size", MPI_MAX_INFO_VAL, 
		     value, &flag);
	if (flag && ((intval = atoi(value)) > 0)) {
	    MPI_Info_set(info, "ind_rd_buffer_size", value);
	    fd->hints->ind_rd_buffer_size = intval;
	}

	MPI_Info_get(users_info, "cb_config_list", MPI_MAX_INFO_VAL,
		     value, &flag);
	if (flag) {
	    if (fd->hints->cb_config_list == NULL) {
		/* only set cb_config_list if it isn't already set.
		 * Note that since we set it below, this ensures that
		 * the cb_config_list hint will be set at file open time
		 * either by the user or to the default
		 */
	    	MPI_Info_set(info, "cb_config_list", value);
		fd->hints->cb_config_list = ADIOI_Malloc((strlen(value)+1) * sizeof(char));
		if (fd->hints->cb_config_list == NULL) {
		    /* NEED TO HANDLE ENOMEM */
		}
		strcpy(fd->hints->cb_config_list, value);
	    }
	    /* if it has been set already, we ignore it the second time. 
	     * otherwise we would get an error if someone used the same
	     * info value with a cb_config_list value in it in a couple
	     * of calls, which would be irritating. */
	}
    }

    /* handle cb_config_list default value here; avoids an extra
     * free/alloc and insures it is always set
     */
    if (fd->hints->cb_config_list == NULL) {
	MPI_Info_set(info, "cb_config_list", ADIOI_CB_CONFIG_LIST_DFLT);
	fd->hints->cb_config_list = ADIOI_Malloc((strlen(ADIOI_CB_CONFIG_LIST_DFLT)+1) * sizeof(char));
	if (fd->hints->cb_config_list == NULL) {
	    /* NEED TO HANDLE ENOMEM */
	}
	strcpy(fd->hints->cb_config_list, ADIOI_CB_CONFIG_LIST_DFLT);
    }

    if ((fd->file_system == ADIO_PIOFS) || (fd->file_system == ADIO_PVFS)) {
    /* no data sieving for writes in PIOFS and PVFS, because they do not
       support file locking */
       	MPI_Info_get(info, "ind_wr_buffer_size", MPI_MAX_INFO_VAL,
		     value, &flag);
	if (flag) {
	    /* get rid of this value if it is set */
	    MPI_Info_delete(info, "ind_wr_buffer_size");
	}
	fd->hints->ind_wr_buffer_size = -1;
	MPI_Info_set(info, "romio_ds_write", "disable");
	fd->hints->ds_write = ADIOI_HINT_DISABLE;
    }

    ADIOI_Free(value);

    *error_code = MPI_SUCCESS;
}
