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
 * $Id: impid.h,v 6.20 2003/02/04 00:24:17 jsquyres Exp $
 *
 *	Function:	- IMPI client daemon header file
 */

#ifndef _LAM_IMPID_H_
#define _LAM_IMPID_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <poll.h>

/*
 * Array indices
 */

enum {
  IMPI_NO_LABEL_I,

  IMPI_C_VERSION_I,
  IMPI_C_NHOSTS_I,
  IMPI_C_NPROCS_I,
  IMPI_C_DATALEN_I,
  IMPI_C_TAGUB_I,
  IMPI_C_COLL_XSIZE_I,
  IMPI_C_COLL_MAXLINEAR_I,
  
  IMPI_H_IPV6_I,
  IMPI_H_PORT_I,
  IMPI_H_NPROCS_I,
  IMPI_H_ACKMARK_I,
  IMPI_H_HIWATER_I,
  
  IMPI_P_IPV6_I,
  IMPI_P_PID_I,

  IMPI_MAX_I
};


/*
 * external variables
 * Only within the impid
 */
extern IMPI_Int4 *cdata[IMPI_MAX_I];
extern MPI_Comm parent_intra;

extern IMPI_Uint4 nclients;                
extern IMPI_Uint4 nhosts;                  
extern IMPI_Uint4 nprocs;                  
extern IMPI_Uint4 my_host_rank;
extern IMPI_Uint4 num_lamprocs;  
extern int my_client_rank;
extern int start_rank;
extern int impi_lam_datalen;
extern int impi_lam_ackmark;
extern int impi_lam_hiwater;
extern int impi_lam_coll_xsize;
extern int impi_lam_coll_maxlinear;
extern IMPI_Uint4 impi_nfinal_msgs;
extern IMPI_Uint4 impi_npkfini_msgs;
extern IMPI_Uint4 impi_sent_pkfini_msgs;


/*
 * dance.c
 */
char *impi_sprintf_ipaddr(IMPI_Uint4 addr);
IMPI_Uint4 impi_get_my_ipaddress(void);
int impi_dance(int my_dance_fd);


/*
 * impid.c
 */
void impi_bail(int code);


/*
 * lamnodes.c
 */
int impi_setup_server_info(void);
int impi_distrib_server_info(int err);


/*
 * router.*.c
 */
void do_router(void);


/*
 * endian.c
 */
void lam_swap_endian(void *buf, int size);


/*
 * host.cc
 */
int *impi_host_init(void);
int impi_host_recv(int src_host, struct pollfd *poll_fd);
int impi_host_send_pkfini_msgs(struct pollfd *h_fds);


/*
 * sequence.c
 */
IMPI_Uint8 get_new_seqnum(void);
IMPI_Uint8 release_seqnum(IMPI_Uint8);
IMPI_Uint8 get_new_srqid(void);
IMPI_Uint8 release_srqid(IMPI_Uint8);
IMPI_Uint8 get_new_drqid(void);
IMPI_Uint8 release_drqid(IMPI_Uint8);
IMPI_Uint8 get_new_localcid(void);
IMPI_Uint8 release_localcid(IMPI_Uint8);


/*
 * server.c
 */
int impi_server_connect(char *server, int port);
int impi_server_do_work(IMPI_Uint4 ipaddress, int dance_port);
void impi_server_disconnect(void);
void impi_server_bail(int code);

#ifdef __cplusplus
}
#endif

#endif
