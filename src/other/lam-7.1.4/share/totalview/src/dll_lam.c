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
 * $Id: dll_lam.c,v 1.12 2003/07/11 20:11:04 jsquyres Exp $
 *
 *	Function:	- DLL for LAM/MPI interface to TotalView debugger
 */


/* Support for MPI 2 is not available in DLL */
#define FOR_MPI2 0

#include <lam_config.h>

#include <stdlib.h>	
#include <string.h>

#include <mpisys.h>
#include <mpi_interface.h>
#include <lam_dll_defs.h>

#define concat(a,b) a##b
#define stringize(a) #a

/* Easy names for callback functions into the debugger */

#define mqs_malloc           (mqs_basic_entrypoints->mqs_malloc_fp)
#define mqs_free             (mqs_basic_entrypoints->mqs_free_fp)
#define mqs_prints           (mqs_basic_entrypoints->mqs_eprints_fp)
#define mqs_put_image_info   (mqs_basic_entrypoints->mqs_put_image_info_fp)
#define mqs_get_image_info   (mqs_basic_entrypoints->mqs_get_image_info_fp)
#define mqs_put_process_info (mqs_basic_entrypoints->mqs_put_process_info_fp)
#define mqs_get_process_info (mqs_basic_entrypoints->mqs_get_process_info_fp)

#define mqs_find_type        (i_info->image_callbacks->mqs_find_type_fp)
#define mqs_field_offset     (i_info->image_callbacks->mqs_field_offset_fp)
#define mqs_get_type_sizes   (i_info->image_callbacks->mqs_get_type_sizes_fp)
#define mqs_find_function    (i_info->image_callbacks->mqs_find_function_fp)
#define mqs_find_symbol      (i_info->image_callbacks->mqs_find_symbol_fp)

#define mqs_get_image        (p_info->process_callbacks->mqs_get_image_fp)
#define mqs_get_global_rank (p_info->process_callbacks->mqs_get_global_rank_fp)
#define mqs_fetch_data       (p_info->process_callbacks->mqs_fetch_data_fp)
#define mqs_target_to_host   (p_info->process_callbacks->mqs_target_to_host_fp)

/* Macro to get the offset of the fields in actual structure in LAM in
   the library struct of i_info .
   NOTE: do not put spaces between the arguements and commas. */

#define GETOFFSET(type,field)\
  i_info->concat(field,_offs) = mqs_field_offset(type,stringize(field));

static const mqs_basic_callbacks *mqs_basic_entrypoints;
static int host_is_big_endian;


/* Symbolic names for errors.  The user errors starts from
   mqs_first_user_code.  If need for more error codes arises then add
   to this list */

enum {
  err_ignore_queue_debugging = mqs_first_user_code,
  err_rpi_noqueue,
  err_type_req,
  err_type_comm,
  err_type_group,
  err_type_proc,
  err_type_gps,
  err_type_ah_desc,
  err_type_al_desc,
  err_type_al_head,
  err_type_lam_ssi_rpi_cbuf_msg,
  err_type_lam_ssi_rpi_cbuf_cid,
  err_type_lam_ssi_rpi_envl
};


/*
 *	fetch_pointer
 *
 *	Function: - Retrieve pointer data type from the image using the 
 *      Accepts: -  pointer to mqs_process of debugged process
 *                  address where the pointer is stored
 *                  pointer to lam_process_info of debugged process
 */

static mqs_taddr_t 
fetch_pointer(mqs_process * proc, mqs_taddr_t addr, lam_process_info *p_info)
{
  int asize = p_info->sizes.pointer_size;
  char data [8];
  mqs_taddr_t res = 0;
  if (mqs_ok == mqs_fetch_data(proc, addr, asize, data))
    mqs_target_to_host(proc, data, ((char *) &res) + 
		       (host_is_big_endian ? sizeof(mqs_taddr_t)-asize : 0),
		       asize);
  return res;
}


/*
 *	fetch_pointer
 *
 *	Function: - Retrieve integer data type from the image using the 
 *      Accepts: -  pointer to mqs_process of debugged process
 *                  address where the integer is stored
 *                  pointer to lam_process_info of debugged process
 */

static mqs_tword_t 
fetch_int(mqs_process * proc, mqs_taddr_t addr, lam_process_info *p_info)
{
  int isize = p_info->sizes.int_size;
  char buffer[8];
  mqs_tword_t res = 0;
  if (mqs_ok == mqs_fetch_data(proc, addr, isize, buffer))
    mqs_target_to_host(proc, buffer, ((char *) &res) + 
			(host_is_big_endian ? sizeof(mqs_tword_t)-isize : 0), 
			isize);
  return res;
}


/*
 *	mqs_setup_basic_callbacks
 *
 *	Function: - Register the callback functions to debugger.
 *      Accepts: -  pointer to structure having callback function pointers.
 */

void 
mqs_setup_basic_callbacks(const mqs_basic_callbacks * cb)
{
  int t = 1;
  host_is_big_endian    = (*(char *) &t) != 1;
  mqs_basic_entrypoints = cb;
}


/*
 *	mqs_version_compatibility
 *
 *	Function: - return version information
 */

int 
mqs_version_compatibility(void)
{
  return MQS_INTERFACE_COMPATIBILITY;
}


/*
 *      mqs_version_string
 *
 *	Function: - return string of verion information.
 */

char *
mqs_version_string(void)
{
  return "ETNUS LAM message queue support for LAM " LAM_VERSION;
}


/*
 *	mqs_dll_taddr_width
 *
 *	Function: - return sizeof mqs_taddr_t data type to the debugger.
 */

int 
mqs_dll_taddr_width(void)
{
  return sizeof(mqs_taddr_t);
}

/*
 *	build_comm_list
 *
 *	Function: - Build the array of communicators (and its information) 
 *                  in lam_comm_list field of lam_process_info struct by 
 *		    reading the all_list of lam_comms in LAM.  
 *      Accepts: -  pointer to mqs_process of debugged process. 
 */

static void 
build_comm_list(mqs_process *proc)
{
  lam_process_info *p_info = (lam_process_info *) mqs_get_process_info(proc);
  mqs_image * image          = mqs_get_image(proc);
  lam_image_info *i_info   = (lam_image_info *) mqs_get_image_info(image);
  mqs_taddr_t group, procs, g_procs, aldesc, alhead, commptr, commbase;
  mqs_tword_t numcomm, flag;
  int i, j;

  /* Get al_desc for communicator list and the number of comms in the
     list.  Allocate memory for that many comm info */
  aldesc = fetch_pointer(proc, p_info->lamcomms_base, p_info);
  numcomm = fetch_int(proc, aldesc + i_info->al_nelem_offs, p_info);
  p_info->lam_tot_comm = numcomm;

  if (numcomm == 0)
    return;
  p_info->lam_comm_list = (lam_comm_info *) mqs_malloc(numcomm * 
						       sizeof(lam_comm_info));
  
  /* Traverse through the list using al_head */
  alhead = fetch_pointer(proc, aldesc + i_info->al_ltop_offs, p_info);
  for (i = 0; alhead != 0; ++i) {

    /* Get the pointer to the comm in the list and jump to the next
       al_head */
    commptr = alhead + 2 * p_info->sizes.pointer_size;
    commbase = fetch_pointer(proc, commptr, p_info);
    alhead = fetch_pointer(proc, alhead + i_info->al_next_offs, p_info);
    
    /* Get the required data from the _comm struct */
    p_info->lam_comm_list[i].thiscomm = commbase;
    p_info->lam_comm_list[i].contextid = 
      fetch_int(proc, commbase + i_info->c_contextid_offs, p_info);
    mqs_fetch_data(proc, commbase + i_info->c_name_offs, 64, 
		   p_info->lam_comm_list[i].name);
    p_info->lam_comm_list[i].name[63] = 0;
    if (p_info->lam_comm_list[i].name[0] == 0)
      strcpy(p_info->lam_comm_list[i].name, "-- unnamed --");
    
    flag = fetch_int(proc, commbase + i_info->c_flags_offs, p_info);
    
    /* Use c_rgroup if its an intercomm otherwise use c_group */
    if (flag & LAM_CINTER)
      group = fetch_pointer(proc, commbase + i_info->c_rgroup_offs, p_info);
    else 
      group = fetch_pointer(proc, commbase + i_info->c_group_offs, p_info);

    /* Set a signal for hidden communicators */
    if (flag & LAM_CHIDDEN)
      p_info->lam_comm_list[i].contextid = -1;
    
    /* Get the required data from the _group struct */
    p_info->lam_comm_list[i].size = 
      fetch_int(proc, group + i_info->g_nprocs_offs, p_info);
    p_info->lam_comm_list[i].local_rank = 
      fetch_int(proc, group + i_info->g_myrank_offs, p_info);
    g_procs = fetch_pointer(proc, group + i_info->g_procs_offs, p_info);

    if (p_info->lam_comm_list[i].size == 0) 
      continue;

    /* Allocate the memory for storing global ranks of the member
       processes */
    p_info->lam_comm_list[i].group_members = 
      (mqs_tword_t *) mqs_malloc(p_info->lam_comm_list[i].size * sizeof(int));
    
    /* Iterate through the list of _proc pointer in the group to get
       their global ranks */ 
    for (j = 0; j < p_info->lam_comm_list[i].size; ++j) {
      procs = fetch_pointer(proc, g_procs + j * p_info->sizes.pointer_size, 
			    p_info);
      p_info->lam_comm_list[i].group_members[j] = 
	fetch_int(proc, procs + i_info->p_gps_offs + i_info->gps_grank_offs, 
		  p_info);
    }
  }
}  


/*
 *	free_list
 *
 *	Function: - Free lam_comm_list array and their group_members array. 
 *      Accepts : - Pointer to lam_process_info of debugged process. 
 */

static void 
free_list(lam_process_info *p_info)
{
  int i, size = p_info->lam_tot_comm;
  for (i = 0; i < size; ++i) {
    if (p_info->lam_comm_list[i].size != 0)
      mqs_free(p_info->lam_comm_list[i].group_members);
  }
  if (p_info->lam_comm_list != NULL)
    mqs_free(p_info->lam_comm_list);
  p_info->lam_comm_list = NULL;
}


/*
 *	mqs_setup_image
 *
 *	Function: - setup the process image information
 *      Accepts: -  pointer to mqs_image of debugged process
 *                  pointer to struct of callback function pointers.
 */

int 
mqs_setup_image(mqs_image *image, const mqs_image_callbacks *icb)
{
  lam_image_info *i_info = (lam_image_info *) mqs_malloc(sizeof 
							(lam_image_info));

  memset ((void *) i_info, 0, sizeof(lam_image_info));
  i_info->image_callbacks = icb;
  mqs_put_image_info(image, (mqs_image_info *) i_info);
  return mqs_ok;
}


/*
 *	mqs_image_has_queues
 *
 *	Function: - record the offsets of fields in actual LAM structs.
 *      Accepts: -  pointer to mqs_image of debugged process
 *                  error string.  no error is likely to occur so we set 
 *		    it to NULL.
 */

int 
mqs_image_has_queues(mqs_image *image, char **message)
{
  lam_image_info * i_info = (lam_image_info *) mqs_get_image_info(image);
  mqs_type *type;
  *message = NULL;

  type = mqs_find_type(image, "struct _req", mqs_lang_c);
  if (type != NULL) {
    GETOFFSET(type,rq_type);
    GETOFFSET(type,rq_state);
    GETOFFSET(type,rq_count);
    GETOFFSET(type,rq_packbuf);
    GETOFFSET(type,rq_packsize);
    GETOFFSET(type,rq_buf);
    GETOFFSET(type,rq_rank);
    GETOFFSET(type,rq_tag);
    GETOFFSET(type,rq_comm);
    GETOFFSET(type,rq_cid);
    GETOFFSET(type,rq_next);
    GETOFFSET(type,rq_proc);
  } else 
    return err_type_req;
 
  type = mqs_find_type(image, "struct _comm", mqs_lang_c);
  if (type != NULL) {
    GETOFFSET(type,c_contextid);
    GETOFFSET(type,c_flags);
    GETOFFSET(type,c_group);
    GETOFFSET(type,c_rgroup);
    GETOFFSET(type,c_name);
  } else 
    return err_type_comm;

  type = mqs_find_type(image, "struct _group", mqs_lang_c);
  if (type != NULL) {
    GETOFFSET(type,g_nprocs);
    GETOFFSET(type,g_myrank);
    GETOFFSET(type,g_procs);
  } else 
    return err_type_group;

  type = mqs_find_type(image, "struct _proc", mqs_lang_c);
  if (type != NULL) {
    GETOFFSET(type,p_gps);
  } else 
    return err_type_proc;
  
  type = mqs_find_type(image, "struct _gps", mqs_lang_c);
  if (type != NULL) {
    GETOFFSET(type,gps_grank);
  } else 
    return err_type_gps;
  
  type = mqs_find_type(image, "struct ah_desc", mqs_lang_c);
  if (type != NULL) {
    GETOFFSET(type,ah_maxnelem);
    GETOFFSET(type,ah_elemsize);
    GETOFFSET(type,ah_table);
  } else 
    return err_type_ah_desc;

  type = mqs_find_type(image, "struct al_desc", mqs_lang_c);
  if (type != NULL) {
    GETOFFSET(type,al_ltop);
    GETOFFSET(type,al_nelem);
  } else 
    return err_type_al_desc;
  
  type = mqs_find_type(image, "struct al_head", mqs_lang_c);
  if (type != NULL) {
    GETOFFSET(type,al_next);
  } else 
    return err_type_al_head;
  
  type = mqs_find_type(image, "struct lam_ssi_rpi_cbuf_msg", mqs_lang_c);
  if (type != NULL) {
    GETOFFSET(type,cm_env);
    GETOFFSET(type,cm_buf);
  } else 
    return err_type_lam_ssi_rpi_cbuf_msg;

  type = mqs_find_type(image, "struct lam_ssi_rpi_cbuf_cid", mqs_lang_c);
  if (type != NULL) {
    GETOFFSET(type,cb_cid);
    GETOFFSET(type,cb_envs);
  } else 
    return err_type_lam_ssi_rpi_cbuf_cid;
  
  type = mqs_find_type(image, "struct lam_ssi_rpi_envl", mqs_lang_c);
  if (type != NULL) {
    GETOFFSET(type,ce_len);
    GETOFFSET(type,ce_tag);
    GETOFFSET(type,ce_rank);
  } else 
    return err_type_lam_ssi_rpi_envl;
  
  return mqs_ok;
} 


/*
 *	mqs_setup_process
 *
 *	Function: - initialize process specific variables and setup 
 *	            process callbacks.
 *      Accepts: -  pointer to mqs_process of debugged process. 
 *                  pointer to callbacks
 */

int 
mqs_setup_process(mqs_process *process, const mqs_process_callbacks *pcb)
{ 
  lam_process_info *p_info = 
    (lam_process_info *) mqs_malloc(sizeof(lam_process_info));
  mqs_image        *image;
  lam_image_info *i_info;
  
  p_info->process_callbacks = pcb;
  image  = mqs_get_image(process);
  i_info = (lam_image_info *) mqs_get_image_info(image);
  
  /* LAM lam_tv_comm_seqnum starts from 0, so initialize with -1 to
     force building first time */
  p_info->lam_comm_seq = -1;
  p_info->lam_comm_list = NULL;
  p_info->lam_tot_comm = 0;
  p_info->what = mqs_pending_sends;
  
  mqs_get_type_sizes(process, &p_info->sizes);
  mqs_put_process_info(process, (mqs_process_info *) p_info);
  return mqs_ok;
}


/*
 *	process_has_queues
 *
 *	Function: - Find queue and comm list base symbols in image.
 *      Accepts: -  pointer to mqs_process of debugged process
 *                  error string. we will come across any errors.
 */

int 
mqs_process_has_queues(mqs_process *proc, char **msg)
{
  lam_process_info *p_info = (lam_process_info *) mqs_get_process_info(proc);
  mqs_image * image        = mqs_get_image(proc);
  lam_image_info *i_info   = (lam_image_info *) mqs_get_image_info(image);

  *msg = 0;

  if (mqs_find_symbol(image, "lam_tv_ignore_queue_debugging", 
		      &p_info->lam_queue_ignore_base) == mqs_ok) {
    if (fetch_int(proc, p_info->lam_queue_ignore_base, p_info) != 0)
      return err_ignore_queue_debugging;
  } else 
    p_info->lam_queue_ignore_base = 0;
  
  mqs_find_symbol(image, "lam_comms", &p_info->lamcomms_base);
  mqs_find_symbol(image, "lam_rq_top", &p_info->queue_base);
  mqs_find_symbol(image, "buftbl", &p_info->unexpected_hash_base);
  mqs_find_symbol(image, "lam_tv_comm_seqnum",
		  &p_info->lam_tv_comm_seqnum_addr);
  
  mqs_find_symbol(image, "lam_flinit", &p_info->lam_flinit_base);
  mqs_find_symbol(image, "lam_flfinal", &p_info->lam_flfinal_base);

  return mqs_ok;
} 


/*
 *	mqs_update_communicator_list
 *
 *	Function: - If comm list has changed in the image then rebuild
 *	            the comm array.
 *      Accepts: -  pointer to mqs_process of debugged process. 
 */

int 
mqs_update_communicator_list(mqs_process *proc)
{
  lam_process_info *p_info = (lam_process_info *) mqs_get_process_info(proc);
  mqs_tword_t new_seq = fetch_int(proc, p_info->lam_tv_comm_seqnum_addr, 
				  p_info);
  mqs_tword_t flinit = fetch_int(proc, p_info->lam_flinit_base, p_info);
  mqs_tword_t flfinal = fetch_int(proc, p_info->lam_flfinal_base, p_info);

  /* If MPI_Init is not called yet or MPI_Finalize has been called
     then there would be no communicators in LAM */
  if (flinit == 0 || flfinal != 0) {
    p_info->lam_comm_list = NULL;
    p_info->lam_tot_comm = 0;
    return mqs_ok;
  }

  /* If in LAM it has been detected that RPI does not support queue
     give a message saying so and stop building the queue */
  if (p_info->lam_queue_ignore_base != 0 &&
      fetch_int(proc, p_info->lam_queue_ignore_base, p_info) != 0) {
    p_info->lam_comm_list = NULL;
    p_info->lam_tot_comm = 0;
    return err_rpi_noqueue;
  }
  
  /* If the last stored seq num is different from that of LAM's then
     something has changed.  So rebuild the array again, otherwise
     continue with the old one */
  if (new_seq != p_info->lam_comm_seq) {
    if (p_info->lam_comm_list != NULL) {
      free_list(p_info);
    }
    build_comm_list(proc);
    p_info->lam_comm_seq = new_seq;
  }
  return mqs_ok;
}


/*
 *	mqs_setup_communicator_iterator
 *
 *	Function: - prepare for iterating over the communicator list.
 *      Accepts: -  pointer to mqs_process of debugged process. 
 */

int 
mqs_setup_communicator_iterator(mqs_process *proc)
{
  lam_process_info *p_info = (lam_process_info *) mqs_get_process_info(proc);
  p_info->lam_curr_comm = 0;
  p_info->next_msg = 0;
  return p_info->lam_comm_list == NULL ? mqs_end_of_list : mqs_ok;
}


/*
 *	mqs_get_communicator
 *
 *	Function: - fill the information about the current communicator
 *	            during the iteration.
 *      Accepts: -  pointer to mqs_process of debugged process
 *                  pointer to comm structure in which information is filled
 */

int 
mqs_get_communicator(mqs_process *proc, mqs_communicator *comm)
{
  lam_process_info *p_info = (lam_process_info *) mqs_get_process_info(proc);
  lam_comm_info c_info;
  
  if (p_info->lam_comm_list == NULL || 
      p_info->lam_curr_comm >= p_info->lam_tot_comm)
    return mqs_end_of_list;
  
  c_info = p_info->lam_comm_list[p_info->lam_curr_comm];

  /* Keep on ignoring the communicators in the list which have
     negative contextid - a signal for hidden communicators */

  while (c_info.contextid < 0) {
    if (p_info->lam_curr_comm >= p_info->lam_tot_comm - 1)
      return mqs_end_of_list;
    c_info = p_info->lam_comm_list[++(p_info->lam_curr_comm)];
  }
  
  comm->unique_id  = c_info.thiscomm;
  comm->local_rank = c_info.local_rank;
  comm->size       = c_info.size;
  strcpy(comm->name, c_info.name);
  
  return mqs_ok;
} 


/*
 *	mqs_get_comm_group
 *
 *	Function: - return the global rank of the group members of the 
 *	            present comm in the iteration.
 *      Accepts: -  pointer to mqs_process of debugged process. 
 *                  int array to return the result in.
 */

int 
mqs_get_comm_group(mqs_process *proc, int *group_members)
{
  lam_process_info *p_info = (lam_process_info *) mqs_get_process_info(proc);
  lam_comm_info c_info = p_info->lam_comm_list[p_info->lam_curr_comm];
  int i, size = c_info.size;
  for (i = 0; i < size; ++i) 
    group_members[i] = c_info.group_members[i];
  return mqs_ok;
}


/*
 *	mqs_next_communicator
 *
 *	Function: - Increment the current communicator pointer to the next
 *	            communicator in the list.
 *      Accepts: -  pointer to mqs_process of debugged process. 
 */

int 
mqs_next_communicator(mqs_process *proc)
{
  lam_process_info *p_info = (lam_process_info *) mqs_get_process_info(proc);
  ++(p_info->lam_curr_comm);
  if (p_info->lam_curr_comm < p_info->lam_tot_comm)
    return mqs_ok;
  else {
    return mqs_end_of_list;
  }
}


/*
 *	mqs_setup_operation_iterator
 *
 *	Function: - prepare for iteration over the messages of the type
 *	            specified in arguement 2
 *      Accepts: -  pointer to mqs_process of debugged process. 
 *                  type of operation.
 */

int 
mqs_setup_operation_iterator(mqs_process *proc, int op)
{
  lam_process_info *p_info = (lam_process_info *) mqs_get_process_info(proc);
  mqs_image * image          = mqs_get_image(proc);
  lam_image_info *i_info   = (lam_image_info *) mqs_get_image_info(image);
  mqs_taddr_t buftbl;
  p_info->what = (mqs_op_class)op;

  switch (op) {
  case mqs_pending_sends:
  case mqs_pending_receives:
    /* Initialize first message to the start of the list */
    p_info->next_msg = p_info->queue_base;
    return mqs_ok;
  case mqs_unexpected_messages:
    p_info->next_msg = 0;
    
    /* Find the unexpected hash table and initialize the required
       variables from its ah_desc */
    buftbl = fetch_pointer(proc, p_info->unexpected_hash_base, p_info);
    p_info->lam_unexp_tot_size = fetch_int(proc, buftbl 
					   + i_info->ah_maxnelem_offs, p_info);
    p_info->lam_unexp_elem_size = 
      fetch_int(proc, buftbl + i_info->ah_elemsize_offs, p_info);
    p_info->lam_end_of_unexp = 0;
    p_info->unexpected_mess_base = fetch_int(proc, buftbl + 
					     i_info->ah_table_offs, p_info);
    return mqs_ok;
  }
  return mqs_first_user_code;
} 


/*
 *	fetch_request
 *
 *	Function: - Fill in the information about the next message in the 
 *	            send or receive queue. 
 *      Accepts: -  pointer to mqs_process of debugged process. 
 *                  pointer to lam_process_info of debugged process.
 *                  structure pointer to store message information in.
 *                  type of information ( send or receive ).
 */

static int 
fetch_request(mqs_process *proc, lam_process_info *p_info,
	      mqs_pending_operation *res, int operation)
{
  mqs_image *image = mqs_get_image(proc);
  lam_image_info *i_info   = (lam_image_info *) mqs_get_image_info(image);
  lam_comm_info c_info = p_info->lam_comm_list[p_info->lam_curr_comm];
  
  /* Get contextid for the current communicator */
  mqs_tword_t wanted_context = c_info.contextid;
  mqs_taddr_t base = fetch_pointer(proc, p_info->next_msg, p_info);

  /* Walk through the list of requests */
  while (base != 0) {
    
    /* Get the operation type and context id of this request */
    mqs_tword_t actual_context = fetch_int(proc, base + i_info->rq_cid_offs,
					    p_info);
    mqs_tword_t actual_operation = fetch_int(proc, base + i_info->rq_type_offs,
					     p_info);
    int act_op = actual_operation < 4 ? 0 : 1;
    
    /* Get the info if it is of required comm and operation type */
    if (actual_context == wanted_context && act_op == operation) {
      
      /* Extract the information from the _req struct */
      mqs_tword_t tag = fetch_int(proc, base + i_info->rq_tag_offs, p_info);
      mqs_tword_t state = fetch_int(proc, base + i_info->rq_state_offs, 
				    p_info);
      mqs_tword_t count = fetch_int(proc, base + i_info->rq_count_offs,
				    p_info);
      mqs_tword_t rank = fetch_int(proc, base + i_info->rq_rank_offs, p_info);
      mqs_taddr_t buf = fetch_pointer(proc, base + i_info->rq_buf_offs,
				      p_info);
      mqs_taddr_t next = fetch_pointer(proc, base + i_info->rq_next_offs,
				       p_info);
      
      /* Get the address of the _proc of the peer process and find its
	 global rank */
      mqs_taddr_t procs = fetch_pointer(proc, base + i_info->rq_proc_offs,
					p_info);
      mqs_taddr_t peerrank = fetch_int(proc, procs + i_info->p_gps_offs +
				       i_info->gps_grank_offs, p_info);
      
      /* Populate the passed structure with the above information */
      res->status = (state == 3) ? mqs_st_matched : mqs_st_pending;
      res->desired_local_rank = rank;
      res->desired_global_rank = peerrank;
      res->desired_tag = tag;
      res->tag_wild = (tag == -1) ? 1 : 0;
      res->system_buffer  = 0;
      res->buffer         = buf;
      res->desired_length = count;
      if ((state == 3) || (operation == 0)) {
	res->actual_tag        = tag;
	res->actual_local_rank = rank;
	res->actual_global_rank = peerrank;
	res->actual_length = count;
      }
      
      /* Goto the next req in the list */
      p_info->next_msg = next;
      return mqs_ok;
    } else {
      base = fetch_pointer(proc, base + i_info->rq_next_offs, p_info);
    }
  }
  
  /* Signal end of list at the end of the list */
  p_info->next_msg = 0;
  return mqs_end_of_list;
}


/*
 *	fetch_unexpected
 *
 *	Function: - Fill in the information about the next message in the 
 *	            unexpected queue. 
 *      Accepts: -  pointer to mqs_process of debugged process. 
 *                  pointer to lam_process_info of debugged process.
 *                  structure pointer to store message information in.
 */

static int 
fetch_unexpected(mqs_process *proc, lam_process_info *p_info,
		 mqs_pending_operation *res)
{
  int i;
  mqs_tword_t cid;
  mqs_taddr_t list, msg;
  mqs_image *image = mqs_get_image(proc);
  lam_image_info *i_info   = (lam_image_info *) mqs_get_image_info(image);
  lam_comm_info c_info = p_info->lam_comm_list[p_info->lam_curr_comm];
  
  /* Get the contextid of the present comm in the iteration */
  mqs_tword_t wanted_context = c_info.contextid;
  
  /* If this is the first call for this comm, then find its contextid
     entry in hash */
  if (p_info->next_msg == 0) {

    for (i = 0; i < p_info->lam_unexp_tot_size; ++i) {
      cid = fetch_int(proc, p_info->unexpected_mess_base + i_info->cb_cid_offs
		      + (i * p_info->lam_unexp_elem_size), p_info);
      if (cid == wanted_context)
	break;
    }
    
    /* if entry not found signal end of list */
    if (i == p_info->lam_unexp_tot_size)
      return mqs_end_of_list;
    
    /* Get the address of the list of envelopes related to this contextid */
    list = 
      fetch_pointer(proc, p_info->unexpected_mess_base + i_info->cb_envs_offs +
		    (i * p_info->lam_unexp_elem_size), p_info);

    /* If list is empty signal end of list */
    if (list == 0)
      p_info->lam_end_of_unexp = 1;
    
    /* Get the address of the first message in the list */
    p_info->next_msg = fetch_pointer(proc, list + i_info->al_ltop_offs,
				     p_info);
  }
  
  /* Get the address of the actual data in the list node */
  msg = p_info->next_msg + 2 * p_info->sizes.pointer_size;
  
  /* Fill the passed structure with the required data */
  res->status = 0;
  res->desired_local_rank = res->desired_global_rank = 
    fetch_pointer(proc, msg + i_info->cm_env_offs + i_info->ce_rank_offs,
		  p_info);
  res->tag_wild = 0;
  res->desired_tag = fetch_pointer(proc, msg + i_info->cm_env_offs + 
				   i_info->ce_tag_offs, p_info);
  res->desired_length = fetch_pointer(proc, msg + i_info->cm_env_offs + 
				      i_info->ce_len_offs, p_info);
  res->system_buffer = 1;
  res->buffer = fetch_pointer(proc, msg + i_info->cm_buf_offs,	p_info);
  
  /* Goto to the next message */
  p_info->next_msg = fetch_pointer(proc, p_info->next_msg + 
				   i_info->al_next_offs, p_info);

  /* If this was the last message set the flag for signalling end of
     list in the next call to mqs_next_operation */ 
  if (p_info->next_msg == 0)
    p_info->lam_end_of_unexp = 1;

  return mqs_ok;
}


/*
 *	mqs_next_operation
 *
 *	Function: - Fill in the information about the next message of the 
 *	            given type of information.
 *      Accepts: -  pointer to mqs_process of debugged process. 
 *                  structure pointer to store message information in.
 */

int 
mqs_next_operation(mqs_process *proc, mqs_pending_operation *res)
{
  lam_process_info *p_info = (lam_process_info *) mqs_get_process_info(proc);
  switch (p_info->what) {
  case mqs_pending_receives:
    return fetch_request(proc,p_info,res,1);
  case mqs_pending_sends:
    return fetch_request(proc,p_info,res,0);
  case mqs_unexpected_messages:
    
    /* If end of unexpected list for this comm was reached after
       previous call to fetch_unexpected signal end of list.  Also do
       so in case the list is empty */
    if (p_info->unexpected_mess_base == 0 || p_info->lam_end_of_unexp == 1) 
      return mqs_end_of_list;
    else 
      return fetch_unexpected(proc, p_info, res);
  }
  return mqs_first_user_code;
}


/*
 *	mqs_destroy_process_info
 *
 *	Function: - free resources related to process_info
 *      Accepts: -  pointer to mqs_process_info of debugged process. 
 */

void 
mqs_destroy_process_info(mqs_process_info *mp_info)
{
  lam_process_info *p_info = (lam_process_info *) mp_info;
  free_list(p_info);
  mqs_free(p_info);
}


/*
 *	mqs_destroy_image_info
 *
 *	Function: - free resources related to image_info
 *      Accepts: -  pointer to mqs_image_info of debugged process. 
 */

void 
mqs_destroy_image_info(mqs_image_info *info)
{
  mqs_free(info);
}


/*
 *	mqs_dll_error_string
 *
 *	Function: - convert a given error code to message string
 *      Accepts:  -  error code
 *
 * We arent using this facility of error handling because we assume that 
 * if versions are compatible this library will almost never have errors, 
 * like in finding symbols etc.
 */

char * 
mqs_dll_error_string(int errcode)
{
  switch (errcode) {
    
  case err_rpi_noqueue:
    return "This RPI does have support for TotalView queue debugging";
  case err_ignore_queue_debugging:
    return "";
  case err_type_req:
    return "The library could not find the type definition of \
struct _req in libmpi";
  case err_type_comm:
    return "The library could not find the type definition of \
struct _comm in libmpi";
  case err_type_group:
    return "The library could not find the type definition of \
struct _group in libmpi";
  case err_type_proc:
    return "The library could not find the type definition of \
struct _proc in libmpi";
  case err_type_gps:
    return "The library could not find the type definition of \
struct _gps in libmpi";
  case err_type_ah_desc:
    return "The library could not find the type definition of \
struct ah_desc in libmpi";
  case err_type_al_desc:
    return "The library could not find the type definition of \
struct al_desc in libmpi";
  case err_type_al_head:
    return "The library could not find the type definition of \
struct al_head in libmpi";
  case err_type_lam_ssi_rpi_cbuf_msg:
    return "The library could not find the type definition of \
struct lam_ssi_rpi_cbug_msg in libmpi";
  case err_type_lam_ssi_rpi_cbuf_cid:
    return "The library could not find the type definition of \
struct lam_ssi_rpi_cbuf_cid in libmpi";
  case err_type_lam_ssi_rpi_envl:
    return "The library could not find the type definition of \
struct lam_ssi_rpi_envl in libmpi";
  default:
    return "Unknown error";
  }
}
