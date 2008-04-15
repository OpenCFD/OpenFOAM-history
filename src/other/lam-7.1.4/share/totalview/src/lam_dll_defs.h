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
 *	Function: - Prototype of structures used in DLL for LAM/MPI
 *	interface to TotalView debugger.
 */


/* Structure used to store information about the image.  Offsets of
   fields in the actual structure types in LAM are stored here */

typedef struct 
{
  const struct mqs_image_callbacks * image_callbacks;

  /* Fields in req struct */
  int rq_type_offs;
  int rq_state_offs;
  int rq_packbuf_offs;
  int rq_packsize_offs;
  int rq_count_offs;
  int rq_buf_offs;
  int rq_rank_offs;
  int rq_tag_offs;
  int rq_comm_offs;
  int rq_cid_offs;
  int rq_next_offs;
  int rq_proc_offs;

  /* Fields of comm struct */
  int c_contextid_offs;
  int c_flags_offs;
  int c_group_offs;
  int c_rgroup_offs;
  int c_name_offs;

  /* Fields in group struct */
  int g_nprocs_offs;
  int g_myrank_offs;
  int g_procs_offs;

  /* Fields in proc struct */
  int p_gps_offs;

  /* Fields in gps struct */
  int gps_grank_offs;

  /* Fields in ah_desc struct */
  int ah_maxnelem_offs;
  int ah_elemsize_offs;
  int ah_table_offs;

  /* Fields in al_desc struct */
  int al_ltop_offs;
  int al_nelem_offs;

  /* Fields in al_head struct */
  int al_next_offs;
  
  /* Fields in lam_ssi_rpi_cbuf_msg */
  int cm_env_offs;
  int cm_buf_offs;

  /* Fields in lam_ssi_rpi_cbuf_cid */
  int cb_cid_offs;
  int cb_envs_offs;
  
  /* Fields in lam_ssi_rpi_envl */
  int ce_len_offs;
  int ce_tag_offs;
  int ce_rank_offs;

} lam_image_info; 


/* Structure stored in DLL to store information about a
   communicator */

typedef struct {
  mqs_tword_t contextid;	/* context id of comm */
  mqs_taddr_t thiscomm; /* address of communicator in image. used as a
			   unique stamp for a comm */ 
  mqs_tword_t local_rank;       /* rank of process in the communicator */
  mqs_tword_t size;             /* size of communicator */
  char name[64];        /* name of comm */
  mqs_tword_t *group_members;   /* global ranks of the members of comm */
} lam_comm_info;


/* Structure to store information related to process */

typedef struct 
{
  const struct mqs_process_callbacks * process_callbacks; 
  mqs_target_type_sizes sizes;	       /* sizeof various types in
				          process */

  mqs_taddr_t lamcomms_base;           /* address of lam_comms -
				          all_list of communicators in
				          LAM */
  mqs_taddr_t lam_tv_comm_seqnum_addr; /* address of
					  lam_tv_comm_seqnum variable
					  in LAM which keeps track of
					  changes in the comm list in
					  LAM */
  mqs_taddr_t queue_base;              /* address of start of recv and
				          send request lists in LAM */
  mqs_taddr_t unexpected_hash_base;    /* address of HASH (ah_desc) in
				          LAM which stores unexpected
				          messages */
  mqs_taddr_t unexpected_mess_base;    /* address of table in the
					  unexpected hash */
  mqs_taddr_t lam_flinit_base;	       /* address of LAM's global
					  variable lam_flinit which
					  signals MPI_Init has
					  been called */
  mqs_taddr_t lam_flfinal_base;        /* address of LAM's global
					  variable lam_flinit which
					  signals MPI_Finalize has
					  been called */
  mqs_taddr_t lam_queue_ignore_base;   /* address of LAM's flag for
					  ignoring queue debugging */

  mqs_tword_t lam_comm_seq;	       /* lam_tv_comm_seqnum of LAM
					  when the comm list was last
					  rebuilt */
  lam_comm_info *lam_comm_list;        /* array of comm information
					  built by reading the comm
					  list of LAM */
  mqs_tword_t lam_curr_comm;           /* index in array of the comm
					  list for the current
					  communicator during the
					  iteration over the
					  communicators */ 
  mqs_tword_t lam_tot_comm;            /* total number of
					  communicators in the
					  communicator array
					  (lam_comm_list) */
    
  mqs_tword_t lam_unexp_tot_size;      /* size of hash table for the
					  unexpected messages */
  mqs_tword_t lam_unexp_elem_size;             /* size of elements of hash
					  table */
  mqs_tword_t lam_end_of_unexp;                /* flag to signal end of
					  unexpected queue for the
					  current communicator */

  mqs_taddr_t   next_msg;              /* pointer to the next message
					  in the iteration for a
					  particular operation */
  mqs_op_class  what;                  /* type of operation over which
					  iteration is going on at
					  present */
} lam_process_info;
