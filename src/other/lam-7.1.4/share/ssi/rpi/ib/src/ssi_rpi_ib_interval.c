/*
 * Copyright (c) 2001-2004 The Trustees of Indiana University.  
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
 * $Id: ssi_rpi_ib_interval.c,v 1.8 2003/12/24 23:52:29 vsahay Exp $
 *
 *	Function:	- registered buffer management
 */

#include <lam_config.h>
#include <lam-ssi-rpi-ib-config.h>

#include <stdlib.h>
#include <unistd.h>

#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <etc_misc.h>

#include <rpi_ib.h>
#include <rpi_ib_interval.h>


/*
 * local types
 */
typedef struct i_entry { 
  LAM_ADDR_T i_base;
  size_t i_len;
  LAM_IB_mr_hndl i_mr_hndl;
  LAM_IB_lkey i_lkey;
  LAM_IB_rkey i_rkey;
} i_entry_t;

typedef struct k_entry {
  LAM_ADDR_T addr;
  int len;
} k_entry_t;


/*
 * Local variables
 */
static lam_map *memhashmap = NULL;
static LAM_ADDR_T free_start, free_end;


/*
 * local functions
 */
static int register_region(LAM_ADDR_T base, size_t len, ib_mem_keypair *kp);
static void unregister_only(void *entry);
static int is_addr_in_there(void *current_entry);
static int ib_comp(void *elem1, void *elem2);


/*
 *	ib_interval_init
 *
 *	Function:	- initialize ib registered buffer hash map
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_ib_interval_init(void)
{
  memhashmap = lam_map_create(ib_comp, sizeof(k_entry_t), sizeof(i_entry_t));
  if (memhashmap == NULL)
    return LAMERROR;

  return 0;
}


/*
 *	ib_interval_destroy
 *
 *	Function:	- destroy all registerned buffers
 */
void
lam_ssi_rpi_ib_interval_finalize(void)
{
  lam_map_destroy(&memhashmap, unregister_only);
}


/*
 *	ib_interval_use
 *
 *	Function:	- register a whole interval
 *                      - increment the ref counts
 *	Accepts:	- base pointer, length
 *	Returns:	- 0 on success, LAMERROR otherwise
 */
int
lam_ssi_rpi_ib_interval_use(void *buf, size_t length, ib_mem_keypair *kp)
{
  i_entry_t *entry;
  k_entry_t key;
  int found;

  key.addr = (LAM_ADDR_T) buf;
  key.len = length;
  entry = (i_entry_t *) lam_map_find(memhashmap, (void *) &key, &found);

  /* If we found it in the map already, then just return the keys.  If
     not, rgister the memory (which will generate new keys) */

  if (found) {
    kp->lkey = entry->i_lkey;
    kp->rkey = entry->i_rkey;
  } else {
    if (register_region(key.addr, length, kp) == LAMERROR) {
      printf("rank %d REGISTER FAILED!\n", lam_myproc->p_gps.gps_grank);
      return LAMERROR;
    }
  }

  /* All done: happiness */

  return 0;
}


/*
 * We don't ever unpin or reference count -- this function is here for
 * future reference in case we ever do reference counting so that we
 * can do LRU sweeps to unregister memory if a memory registration
 * fails (e.g., we're out of registered memory, so we need to
 * unregister some and try again).
 */
int
lam_ssi_rpi_ib_interval_unuse(void *buf, size_t length)
{
  /* All done */

  return 0;
}


/*
 * This function is invoked by the OS / memory manager when memory is
 * about to be given back to the OS.  In this case, we need to
 * traverse the entire hashmap and find all ranges that overlap with
 * the memory that is being returned to the OS and unregister them.
 *
 * Failure to do this will cause Bad Things -- e.g., on OS X, the OS
 * will actually hang when it goes to return the pages because the
 * pages are still marked as "used" by the OS (because the OS knows
 * that VAPI is using them) and it blocks waiting for them to become
 * unused.
 *
 * The action for this is fairly simple -- we load up some globals
 * (SINCE THIS DOES NOT NEED TO BE THREAD SAFE!!!) for the beginning
 * and end of the range addresses that are being returned to the OS.
 * Then we invoke an inorder traversal of the hashmap and a condition
 * and action function for determining if the hash entry is in the
 * range that is being returned, and unregistering the memory
 * (respectively).
 */
void
lam_ssi_rpi_ib_interval_unregister(void *buf, size_t length) 
{
  free_start = (LAM_ADDR_T) buf;
  free_end = free_start + length - 1;
  lam_map_traverse_and_delete(memhashmap, is_addr_in_there, 
                              unregister_only);
}


/************************************************************************/

/*
 * Register a chunk of memory with VAPI
 */
static int
register_region(LAM_ADDR_T base, size_t len, ib_mem_keypair *kp)
{
  k_entry_t key;
  i_entry_t new_entry;
  LAM_IB_mrw req_mr, act_mr;
  LAM_IB_mr_hndl mr_hndl;

  key.addr = base;
  key.len = len;

  /* Try to register it all */
  
  req_mr.type = LAM_IB_MR;
  req_mr.start = (LAM_IB_virt_addr) base;
  req_mr.size = len;
  req_mr.pd_hndl = lam_ssi_rpi_ib_pd_hndl;
  req_mr.acl = LAM_IB_EN_LOCAL_WRITE | LAM_IB_EN_REMOTE_READ | 
    LAM_IB_EN_REMOTE_WRITE | LAM_IB_EN_REMOTE_ATOM;

  if (LAM_IB_register_mr(lam_ssi_rpi_ib_hca_hndl, 
                         &req_mr, &mr_hndl, &act_mr) != LAM_IB_OK) {

    show_help_file("lam-ssi-rpi-ib-helpfile", "rpi-ib", 
		   "no-memory-to-register");
    return LAMERROR;
  }

  /* Prepare for the return of keys  */

  kp->lkey = act_mr.l_key;
  kp->rkey = act_mr.r_key;

  new_entry.i_base = base;
  new_entry.i_len = len;
  new_entry.i_lkey = act_mr.l_key;
  new_entry.i_rkey = act_mr.r_key;
  new_entry.i_mr_hndl = mr_hndl;

  /* Add to the hash */
    
  if (lam_map_insert(memhashmap, &key, &new_entry) == LAMERROR)
    return LAMERROR;

  lam_debug_cond((PUB(did), "RRR: registered from %x to %x"
		  "of len %d with lkey %x",
		  base, base + len - 1, len, kp->lkey));

  /* All done */
  
  return 0;
}


/*
 * Unregister a region
 */
static void
unregister_only(void *entry)
{
  i_entry_t *e = (i_entry_t*) entry;

  /* Tell vapi to forget about this memory */

  LAM_IB_deregister_mr(lam_ssi_rpi_ib_hca_hndl, e->i_mr_hndl);
}


/*
 * This comparison function is called for every element in the hash
 * map.  We're looking to see if each node in the hash map overlaps
 * with the range that is just about to be given back to the OS
 * (i.e., this action will be triggered by the OS free handler).
 *
 * Conditions for which an entry is within the range:
 *
 * 1. Entry buffer is totally within range
 *                  |-----range------|
 *                     |--buffer--|
 * 2. Entry buffer's start overlaps with this range
 *                  |-----range------|
 *                             |--buffer--|
 * 3. Entry buffer's end overlaps with this range
 *                  |-----range------|
 *           |--buffer--|
 * 4. Range is totally within this buffer
 *                     |--range---|
 *                  |-----buffer-----|
 *
 * This boils down to the following tests:
 *
 * - the entry's start is within the range, or
 * - the entry's end is within the range, or
 * - the range's start is within the buffer
 *
 * ([the range's end is within the buffer] is already covered)
 */
static int
is_addr_in_there(void *current_entry) 
{
  i_entry_t *hash_entry;
  LAM_ADDR_T entry_start, entry_end;

  hash_entry = (i_entry_t *) current_entry;
  entry_start = (LAM_ADDR_T) hash_entry->i_base;
  entry_end = entry_start + hash_entry->i_len - 1;

  /* The variables free_start and free_end were loaded with the
     beginning and end of the address range that is being returned to
     the OS back up in lam_ssi_rpi_ib_interval_unregister(). */

  if ((entry_start >= free_start && entry_start <= free_end) ||
      (entry_end >= free_start && entry_end <= free_end) ||
      (free_start >= entry_start && free_start <= entry_end)) {
    return 1;
  }
  return 0;
}


/*
 * Simple hash comparison -- only return true if the elements are
 * *identical*, meaning that their base address *and* their lengths
 * are the same.
 */
static int
ib_comp(void *elem1, void *elem2)
{
  k_entry_t *a, *b;
  LAM_ADDR_T start1, start2, end1, end2;

  a = (k_entry_t *) elem1;
  b = (k_entry_t *) elem2;

  start1 = a->addr;
  start2 = b->addr;

  end1 = a->addr + a->len;
  end2 = b->addr + b->len;
  
  if (start1 == start2 && end1 == end2)
    return 0;

  if (start1 < start2)
    return -1;
  
  return 1;
}

