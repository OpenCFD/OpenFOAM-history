/* -*- c -*- */
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
 */

/* This file defines all of the constants as specified in the IMPI
   Standard. */

#ifndef _impi_defs_h_
#define _impi_defs_h_

#define IMPI_VERSION 0

#define IMPI_MAX_CLIENTS 32

#define IMPI_MINIMUM_TAG_UB 32767      /* 2^15 - 1 */
#define IMPI_MINIMUM_DATA_LEN 1

/* Maximum number of standardized AUTHs. */

#define MAX_AUTH 2

/* Server-Client protocol constants. */

#define IMPI_CMD_AUTH 0x41555448 /* ASCII for 'AUTH' */
#define IMPI_CMD_IMPI 0x494D5049 /* ASCII for 'IMPI' */
#define IMPI_CMD_COLL 0x434F4C4C /* ASCII for 'COLL' */
#define IMPI_CMD_DONE 0x444F4E45 /* ASCII for 'DONE' */
#define IMPI_CMD_FINI 0x46494E49 /* ASCII for 'FINI' */

/* Client IMPI_CMD_COLL ('COLL') command labels. */
#define IMPI_NO_LABEL 0x0000

/* Per-Client labels for the 'COLL' commands. */
#define IMPI_C_VERSION        0x1000
#define IMPI_C_NHOSTS         0x1100
#define IMPI_C_NPROCS         0x1200
#define IMPI_C_DATALEN        0x1300
#define IMPI_C_TAGUB          0x1400
#define IMPI_C_COLL_XSIZE     0x1500
#define IMPI_C_COLL_MAXLINEAR 0x1600

/* Per-Host labels for the 'COLL' commands. */
#define IMPI_H_IPV6    0x2000
#define IMPI_H_PORT    0x2100
#define IMPI_H_NPROCS  0x2200
#define IMPI_H_ACKMARK 0x2300
#define IMPI_H_HIWATER 0x2400
  
/* Per-Proc labels for the 'COLL' commands. */
#define IMPI_P_IPV6     0x3000
#define IMPI_P_PID      0x3100

/*
 * Various sizes 
 */

#define IMPI_INT4_TYPE int
#define IMPI_INT8_TYPE long long
#define IMPI_UINT4_TYPE unsigned int
#define IMPI_UINT8_TYPE unsigned long long

/*
 * Authentication defines
 */

#define IMPI_AUTH_NONE 0
#define IMPI_AUTH_KEY 1

/*
 * typedefing IMPI sizes
 */

typedef unsigned char IMPI_Uint1;
typedef IMPI_INT4_TYPE IMPI_Int4;
typedef IMPI_INT8_TYPE IMPI_Int8;
typedef IMPI_UINT4_TYPE IMPI_Uint4;
typedef IMPI_UINT8_TYPE IMPI_Uint8;

/* All the structures for the various commands that get sent */

typedef struct {
  IMPI_Int4 cmd; /* command code */
  IMPI_Int4 len; /* length in bytes of command payload */
} IMPI_Cmd;

typedef struct {
  IMPI_Uint4 auth_mask;
} IMPI_Client_auth;

typedef struct {
  IMPI_Int4 which;
  IMPI_Int4 len;
} IMPI_Server_auth;

typedef struct {
  IMPI_Uint8 key; /* 64 bits */
} IMPI_Auth_key;

typedef union {
  IMPI_Int4 rank; /* rank of this client in IMPI job */
  IMPI_Int4 size; /* total # of clients in IMPI job */
} IMPI_Impi;

typedef struct {
  IMPI_Int4 label;
} IMPI_Coll;

typedef struct {
  IMPI_Int4 label;
  IMPI_Uint4 client_mask;
} IMPI_Chdr;

typedef struct {
  IMPI_Uint4 major;
  IMPI_Uint4 minor;
} IMPI_Version;

/* Chapter 3 Structures */

typedef struct {
  IMPI_Uint1 p_hostid[16]; /* host identifier */
  IMPI_Int8 p_pid;            /* local process identifier */
} IMPI_Proc;

typedef struct {
  IMPI_Uint4 pk_type;       /* packet type */
#define IMPI_PK_DATA      0 /* message data */
#define IMPI_PK_DATASYNC  1 /* message data (sync) */
#define IMPI_PK_PROTOACK  2 /* protocol ACK */
#define IMPI_PK_SYNCACK   3 /* synchronization ACK */
#define IMPI_PK_CANCEL    4 /* cancel request */
#define IMPI_PK_CANCELYES 5 /* 'yes' cancel reply */
#define IMPI_PK_CANCELNO  6 /* 'no' cancel reply */
#define IMPI_PK_FINI      7 /* agent end-of-communications */
  IMPI_Uint4 pk_len;        /* packet data length */
  IMPI_Proc pk_src;         /* source process */
  IMPI_Proc pk_dest;        /* destination process */
  IMPI_Uint8 pk_srqid;      /* source request ID */
  IMPI_Uint8 pk_drqid;      /* destination request ID */
  IMPI_Uint8 pk_msglen;     /* total message length */

  IMPI_Int4 pk_lsrank;      /* comm-local source rank */
  IMPI_Int4 pk_tag;         /* message tag */
  IMPI_Uint8 pk_cid;        /* context ID */
  IMPI_Uint8 pk_seqnum;     /* message sequence # */
  IMPI_Int8 pk_count;       /* QoS: message count */
  IMPI_Uint8 pk_dtype;      /* QoS: message datatype */
  IMPI_Uint8 pk_reserved;   /* for future use */
} IMPI_Packet;

#endif
