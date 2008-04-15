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
 * $Id: impi.h,v 6.31 2003/02/04 00:24:17 jsquyres Exp $
 *
 *	Function:	- IMPI header file
 */

#ifndef _LAM_IMPI_H_
#define _LAM_IMPI_H_

#include <lam_config.h>

#if LAM_WANT_IMPI

#include <app_mgmt.h>
#include <mpi.h>
#include <impi-defs.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
 * This is the version of IMPI that we support
 */
#define IMPI_MAJOR_VERSION 0
#define IMPI_MINOR_VERSION 0

/*
 * Tags for messages between impid and local LAM ranks
 */
enum {
  IMPI_SERVER_INFO_TAG,
  IMPI_LAMGIAPPE_TAG,
  IMPI_MESSAGE_TAG,
  IMPI_SYNCACK_TAG,
  IMPI_FINALIZE_TAG,
  IMPI_ABORT_TAG,

  IMPI_MAX_TAG
};

/*
 * IMPI version 0.0 default constants 
 */
#define IMPI_COLL_XSIZE_DEFAULT               1024
#define IMPI_COLL_MAXLINEAR_DEFAULT           4

/* 
 *    Constants for collectives
 */
#define IMPI_UINT8_MAX                        18446744073709551615
#define IMPI_MAX_CID                          18446744073709551600

#define IMPI_BARRIER_TAG                      110
#define IMPI_BCAST_TAG                        120
#define IMPI_GATHER_TAG                       130
#define IMPI_GATHERV_TAG                      140
#define IMPI_SCATTER_TAG                      150
#define IMPI_SCATTERV_TAG                     160
#define IMPI_ALLTOALL_TAG                     170
#define IMPI_ALLTOALLV_TAG                    180
#define IMPI_REDUCE_TAG                       190
#define IMPI_SCAN_TAG                         200
#define IMPI_DUP_TAG                          210
#define IMPI_MERGE_TAG                        220
#define IMPI_COMM_FREE_TAG                    230

#define IMPI_COLL_CROSSOVER                   1024

#define IMPI_MAX_LINEAR_REDUCE                4
#define IMPI_MAX_LINEAR_GATHER                4
#define IMPI_MAX_LINEAR_BCAST                 4
#define IMPI_MAX_LINEAR_BARRIER               4

/*
 * Structure that the IMPI client daemon sends back to the LAM
 * nodes (in MPI_Init()) telling them the new size of MPI_COMM_WORLD,
 * etc.
 */
typedef struct {
  int nclients;       /* total number of IMPI clients */
  int nhosts;         /* total number of IMPI hosts */
  int nprocs;         /* total number of IMPI proccesses */
  int start_rank;     /* rank num in "IMPI_COMM_WORLD" of LAM's local rank 0 */
  unsigned int max_datalen;/* max data allowed on one IMPI packet */
  int min_cid;        /* minimum allowed communicator ID */
  int max_tag;        /* maximum allowable tag */
} IMPI_server_info;

/*
 * Structure used to send meta info about MPI messages from LAM to the impid 
 */
typedef struct {
  IMPI_Uint8 srqid;    /* This is not used in the lamgiappe proper; it
                          is [ab]used for endian byte-swapping
                          purposes in otb/sys/impid/local.cc in
                          impi_local_init().  It is never filled in
                          when we send/receive IMPI_Lamgiappe's. */
  int local_src_rank;  /* Relative to impid_comm */
  int world_src_rank;  /* Relative to MPI_COMM_WORLD */
  int world_dest_rank; /* Relative to MPI_COMM_WORLD */

  int cid;             /* Comm ID that we're really sending on */
  int src_rank;        /* Src relative to cid */
  int dest_rank;       /* Dest relative to cid */
  int tag;             /* MPI message tag */
  int length;          /* Length (in bytes) of MPI message */

  int is_ssend;       /* is this an MPI_Ssend/MPI_Issend? */
} IMPI_Lamgiappe;

/*
 * structure used to hold information about IMPI clients
 */
typedef struct {
  IMPI_Int4 ic_version;
  IMPI_Int4 ic_nhosts;
  IMPI_Int4 ic_hostrank;
  IMPI_Int4 ic_datalen;
  IMPI_Int4 ic_flag;
} IMPI_client;

/*
 * structure used to hold information about IMPI hosts
 */
typedef struct {
  IMPI_Int4 ih_clientrank;
  IMPI_Int4 ih_fd;
  IMPI_Int4 ih_port;
  IMPI_Int4 ih_nprocs;
  IMPI_Int4 ih_procrank;
  unsigned char ih_ipv6[16];
} IMPI_host;

/*
 * structure used to hold information about IMPI procs
 */
typedef struct {
  IMPI_Int4 ip_hostrank;
  IMPI_Uint1 ip_ipv6[16];
  IMPI_Int8 ip_pid;
} IMPI_proc;

/*
 * extern variables
 */
extern struct _gps         gimpid;
extern struct _proc        *lam_impid_proc;
extern IMPI_server_info    impi_server_info;
extern MPI_Comm            impid_comm;          /* IMPI comm after merge */ 
extern MPI_Comm            lam_impid_intercomm;
extern MPI_Datatype        IMPI_LAMGIAPPE;
extern MPI_Datatype        IMPI_TYPE_UINT8;
extern IMPI_Uint4          IMPI_Pk_maxdatalen;
extern IMPI_client         *lam_impi_clients;
extern IMPI_host           *lam_impi_hosts;
extern IMPI_proc           *lam_impi_procs;

/*
 * Functions in impi collectives
 */
int IMPI_Is_master(int rank, MPI_Comm comm);
int IMPI_Num_masters(MPI_Comm comm);
int IMPI_Master_rank(int master_num, MPI_Comm comm);
int IMPI_Master_num(int rank, MPI_Comm comm);
int IMPI_Are_local(int rank1, int rank2, MPI_Comm comm);
int IMPI_Local_master_rank(int rank, MPI_Comm comm);
int IMPI_Local_master_num(int rank, MPI_Comm comm);
int IMPI_Num_local_to_master(int master_num, MPI_Comm comm);
int IMPI_Num_local_to_rank(int rank, MPI_Comm comm);
int IMPI_Locals_to_master(int master_num, MPI_Comm comm);

int IMPI_Rank_impi2shadow(int rank, MPI_Comm comm);
int IMPI_Rank_shadow2impi(int rank, MPI_Comm comm);


/*
 * Helper IMPI functions
 */
int IMPI_Init(int *world_n, int *cid, struct _gps **worlds);
int IMPI_Fill_attributes(void);
int IMPI_Finalize(void);  
int IMPI_Ack_impid(MPI_Request in);
int IMPI_Iack_impid(MPI_Request in, MPI_Request *out);
int IMPI_Abort(void);  
int IMPI_Make_dtype(void);
int IMPI_Free_dtype(void);

/*
 * IMPI collective functions
 */
int IMPI_Barrier(MPI_Comm comm);
int IMPI_Bcast(void *buff, int count, MPI_Datatype datatype, 
	       int root, MPI_Comm comm);
int IMPI_Gather(void *sbuf, int scount, MPI_Datatype sdtype, void *rbuf, 
		int rcount, MPI_Datatype rdtype, int root, MPI_Comm comm);
int IMPI_Reduce(void *sbuf, void* rbuf, int count, MPI_Datatype dtype, 
		MPI_Op op, int root, MPI_Comm comm);

/*
 * Handler for sending back datasync ACKs to impid when a ping receive
 * request finishes
 */
int lam_impi_datasync_handler(MPI_Request req);


/*
 * Other IMPI helpers
 * Different mode of lamgiappe sends
 */
int IMPI_Fill_lamgiappe(int reqtype, int count, MPI_Datatype dtype,
			int dest, int tag, MPI_Comm comm, 
			IMPI_Lamgiappe *lamgiappe);

int IMPI_Send_lamgiappe(int reqtype, int count, MPI_Datatype dtype, 
			int dest, int tag, MPI_Comm comm, int *flag);
int IMPI_Isend_lamgiappe(int reqtype, int count,
			 MPI_Datatype dtype, int dest, int tag, 
			 MPI_Comm comm, MPI_Request *req);
int IMPI_Send_lamgiappe_init(int reqtype, int count,
			     MPI_Datatype dtype, int dest, int tag, 
			     MPI_Comm comm, MPI_Request *req);
int IMPI_Isend_lamgiappe_init(int reqtype, int count,
			      MPI_Datatype dtype, int dest, int tag, 
			      MPI_Comm comm, MPI_Request *req);

int IMPI_Register_datasync(int rank, int tag, MPI_Comm comm, MPI_Request *req);
int IMPI_Register_datasync_init(int rank, int tag, MPI_Comm comm, 
				MPI_Request *req);

int IMPI_Register_syncack(int rank, int tag, MPI_Comm comm, MPI_Request *req);
int IMPI_Register_syncack_init(int rank, int tag, MPI_Comm comm, 
			       MPI_Request *req);


#ifdef __cplusplus
}
#endif

#endif
#endif
