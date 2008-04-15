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
 * $Id: ssi_rpi_gm_interval.c,v 1.12 2003/11/05 13:39:18 jsquyres Exp $
 *
 *	Function:	- pinned buffer management
 */

#include <lam_config.h>
#include <lam-ssi-rpi-gm-config.h>

#include <stdlib.h>
#include <unistd.h>

#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <etc_misc.h>
#include <malloc_wrapper.h>

#include <rpi_gm.h>
#include <rpi_gm_interval.h>


/*
 * local variables
 */
typedef struct ientry { 
  unsigned long i_base;
  int i_refcount;
  int i_lru_ok;
  int i_unpinnable;
  struct ientry *i_prev;
  struct ientry *i_next;
} ientry_t;

static int maxlen = 4096;
static int imemid = -1;
static int hash_size = 0;
static struct gm_port *my_port = NULL;
static struct gm_hash *memhash = NULL;
static ientry_t *memlist_head = NULL;
static ientry_t *memlist_tail = NULL;


/*
 * local functions
 */
static int pin_pages(unsigned long base, int num_pages, int lru_ok);
static void unpin_pages(unsigned long base, int num_pages);
static void unpin_lru_memory(void);
static void memlist_delete(ientry_t *entry);
static void memlist_append(ientry_t *entry);


/*
 *	gm_interval_init
 *
 *	Function:	- initialize gm pinned buffers buffer list
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_gm_interval_init(struct lam_ssi_rpi_proc *p)
{
  /* Allocate the hash and a memory pool to manage entries in the
     hash  */

  memhash = gm_create_hash(gm_hash_compare_ptrs, gm_hash_hash_ptr, 
                           0, 0, maxlen, 0);
  if (memhash == NULL)
    return LAMERROR;

  imemid = lam_mp_init(sizeof(ientry_t), NULL, NULL, 1024, 1024, 1);
  if (imemid == -1) {
    gm_destroy_hash(memhash);
    return LAMERROR;
  }

  /* Save the port value */

  my_port = p->cp_gm_port.rgp_gm_port;

  /* See if we can intercept pinned memory */

  if (lam_ssi_rpi_gm_can_register_mem != 0) {
    if (lam_register_free_handler(lam_ssi_rpi_gm_interval_free)) {
      lam_ssi_rpi_gm_can_register_mem = 0;
    }
  }

  return 0;
}


/*
 *	gm_interval_destroy
 *
 *	Function:	- destroy all pinned buffers
 */
void
lam_ssi_rpi_gm_interval_destroy(void)
{
  if (memhash == NULL)
    return;

  /* Disengage the memory handler agent */

  lam_register_free_handler(NULL);

  /* Destroy the memhash itself. Don't bother unpinning all the memory
     because it takes forever and when we exit, it will automatically
     be unpinned.  Yay gm.  :-) */

  lam_mp_finalize(imemid);
  gm_destroy_hash(memhash);

  memhash = NULL;
  imemid = -1;

  my_port = NULL;
}


/*
 *	gm_interval_use
 *
 *	Function:	- pin a whole interval
 *                      - increment the ref counts
 *	Accepts:	- base pointer, length
 *	Returns:	- 0 on success, LAMERROR otherwise
 */
int
lam_ssi_rpi_gm_interval_use(void *buf, size_t length, int lru_ok,
                            int need_to_pin)
{
  unsigned long addr, end;
  unsigned long base_to_pin;
  int num_pages_to_pin;
  ientry_t *entry;

  /* We go through the motions here, even if we're on a system that
     can't pin arbitrary memory because we have the ability to
     MPI_MEM_ALLOC, and so we need to remember what DMA-able memory we
     have issued to the user so that when they use that memory as a
     buffer to send/receive, we can know that we don't need to
     allocate a temporary buffer. */

  /* Figure out what pages we need to pin */

  addr = ((unsigned long) buf) & ~(GM_PAGE_LEN - 1);
  end = ((unsigned long) buf) + length;

  /* Find any page in the interval that is not pinned and try to pin
     it.  Outter loop is O(N). */

  base_to_pin = 0;
  num_pages_to_pin = 0;
  while (addr < end) {

    /* See if this page is already pinned.  Look it up in the hash
       (O(log N)).  If it's not, try to make as long a sequence of
       unpinned pages as possible before actually performing the pin.
       So if this is the first in a new sequence, just save it.  */

    entry = (ientry_t *) gm_hash_find(memhash, (void*) addr);
    if (entry == NULL) {
      if (base_to_pin == 0)
        base_to_pin = (unsigned long) addr;
      ++num_pages_to_pin;
    } 

    /* Ok, *this* page is pinned.  See if we traversed other pages
       before this that were not pinned.  If so, pin them. */

    else {
      ++entry->i_refcount;
      if (lru_ok == 1)
        entry->i_lru_ok = 1;

      if (base_to_pin != 0 && need_to_pin == 1) {
        if (pin_pages(base_to_pin, num_pages_to_pin, lru_ok) == LAMERROR) {
          --entry->i_refcount;
          return LAMERROR;
        }
      }

      base_to_pin = 0;
      num_pages_to_pin = 0;

      /* Move entry to end of LRU memlist (O(1)) */

      memlist_delete(entry);
      memlist_append(entry);
    }

    addr += GM_PAGE_LEN;
  }

  /* Did we end up with any pages to pin? */

  if (base_to_pin != 0 && need_to_pin == 1) {
    if (pin_pages(base_to_pin, num_pages_to_pin, lru_ok) == LAMERROR) {
      if (entry != NULL)
        --entry->i_refcount;
      return LAMERROR;
    }
  }

  /* All done: happiness */

  return 0;
}


/*
 *	gm_interval_check
 *
 *	Function:	- check to see if an interval is fully pinned
 *	Accepts:	- base pointer, length
 *	Returns:	- 1 if fully pinned, 0 if not
 */
int 
lam_ssi_rpi_gm_interval_check(void *buf, size_t length)
{
  unsigned long addr, end;
  ientry_t *entry;

  /* Figure out what pages we need to examine */

  addr = ((unsigned long) buf) & ~(GM_PAGE_LEN - 1);
  end = ((unsigned long) buf) + length;

  /* Outter loop is O(N). */

  while (addr < end) {

    /* See if this page is already pinned.  Look it up in the hash
       (O(log N)).  If it's not, we're done. */

    entry = (ientry_t *) gm_hash_find(memhash, (void*) addr);
    if (entry == NULL)
      return 0;

    /* Ok, *this* page is pinned.  Move entry to end of LRU memlist
       (O(1)) */

    else {
      memlist_delete(entry);
      memlist_append(entry);
    }

    addr += GM_PAGE_LEN;
  }

  /* If we got here, the whole interval is pinned, so return true. */

  return 1;
}


/*
 *	gm_interval_unuse
 *
 *	Function:	- decrement refcounts on all pages in an interval
 *	Accepts:	- base pointer, length
 *	Returns:	- 1 if found, 0 if not
 */
int
lam_ssi_rpi_gm_interval_unuse(void *buf, size_t length)
{
  unsigned long addr, end;
  ientry_t *entry;

  /* If we don't support pinning, don't even bother trying (doing so
     will result in syslog messages) */

  if (lam_ssi_rpi_gm_can_register_mem == 0)
    return 0;

  /* Otherwise, figure out what pages to de-reference */

  addr = ((unsigned long) buf) & ~(GM_PAGE_LEN - 1);
  end = ((unsigned long) buf) + length;

  /* Go find and decrement all the refcounts.  Outter loop is O(N),
     inner lookup is O(log N). */

  while (addr < end) {
    entry = (ientry_t *) gm_hash_find(memhash, (void*) addr);
    if (entry != NULL && entry->i_refcount > 0)
      --entry->i_refcount;
    addr += GM_PAGE_LEN;
  }

  return 0;
}


/*
 *	gm_interval_free
 *
 *	Function:	- invoked from back-ends to free() and friends
 *                      - remove interval from hash map
 *                      - de-register all memory
 *	Accepts:	- base pointer, length
 */
void
lam_ssi_rpi_gm_interval_free(void *buf, size_t length)
{
  unsigned long addr, end;
  ientry_t *entry;
  unsigned long base_to_unpin;
  int num_pages_to_unpin;

  /* Figure out what pages to free */

  addr = ((unsigned long) buf) & ~(GM_PAGE_LEN - 1);
  end = ((unsigned long) buf) + length;
  base_to_unpin = 0;
  num_pages_to_unpin = 0;

  /* Find all pages */

  while (memhash != NULL && addr < end) {

    entry = (ientry_t *) gm_hash_find(memhash, (void*) addr);
    if (entry != NULL) {

      /* Remove this entry from the hash mamp */

      gm_hash_remove(memhash, (void*) addr);

      /* Make a consecutive set of pages to remove */

      if (base_to_unpin == 0)
        base_to_unpin = addr;
      ++num_pages_to_unpin;

      /* Remove from the LRU list */

      memlist_delete(entry);
    }

    else if (base_to_unpin != 0) {
      unpin_pages(base_to_unpin, num_pages_to_unpin);
      base_to_unpin = 0;
      num_pages_to_unpin = 0;
    }

    addr += GM_PAGE_LEN;
  }

  if (base_to_unpin != 0)
    unpin_pages(base_to_unpin, num_pages_to_unpin);
}


/************************************************************************/

static int
pin_pages(unsigned long base, int num_pages, int lru_ok)
{
  int i;
  ientry_t *entry;

  /* Try to pin it all */

  if (gm_register_memory(my_port, (void*) base, GM_PAGE_LEN * num_pages) != 
      GM_SUCCESS) {

    /* If we fail, go unpin a bunch of old pinned memory and try
       again */

    unpin_lru_memory();
    if (gm_register_memory(my_port, (void*) base, GM_PAGE_LEN * num_pages) !=
        GM_SUCCESS)
      return LAMERROR;
  }

  /* Once we've pinned it, add all the pages in that range to the
     hash, and to the end of the LRU list. */

  for (i = 0; i < num_pages; ++i) {
    entry = lam_mp_malloc(imemid);
    entry->i_refcount = 1;
    entry->i_lru_ok = lru_ok;
    entry->i_base = base + (i * GM_PAGE_LEN);

    /* Add to the hash */

    if (gm_hash_insert(memhash, (void *) entry->i_base, 
                       (void *) entry) != GM_SUCCESS)
      return LAMERROR;
    ++hash_size;

    /* Append to the LRU list */

    memlist_append(entry);
  }

  /* All done */

  return 0;
}


static void
unpin_pages(unsigned long base, int num_pages)
{
  gm_deregister_memory(my_port, (void *) base, num_pages * GM_PAGE_LEN);
}


static void 
unpin_lru_memory(void)
{
  ientry_t *cur, *next;
  unsigned long addr_to_unpin;
  int num_pages_to_unpin;

  addr_to_unpin= 0;
  num_pages_to_unpin = 0;
  cur = memlist_head;
  while (hash_size > maxlen) {
    if (cur->i_refcount == 0 && cur->i_lru_ok == 1) {

      /* See if we can run a bunch together and unpin them all at once */

      if (addr_to_unpin == 0) {
        addr_to_unpin = cur->i_base;
        num_pages_to_unpin = 1;
      } 

      /* We might have a contiguous interval going here; check and see
         if this page is in the internal, and if so, augment the
         interval */

      else if (addr_to_unpin + (num_pages_to_unpin * GM_PAGE_LEN) == 
               cur->i_base) {
        ++num_pages_to_unpin;
      }

      /* Nope -- the interval ended.  Free the last interval */

      else {
        unpin_pages(addr_to_unpin, num_pages_to_unpin);
        addr_to_unpin = cur->i_base;
        num_pages_to_unpin = 1;
      }

      /* Either way, remove it from the hash and LRU list */

      next = cur->i_next;
      gm_hash_remove(memhash, (void *) cur->i_base);
      --hash_size;
      memlist_delete(cur);
      lam_mp_free(imemid, cur);
    }

    else
      next = cur->i_next;

    cur = next;
    if (next == NULL)
      break;
  }

  /* Unpin the final interval */

  if (addr_to_unpin != 0)
    unpin_pages(addr_to_unpin, num_pages_to_unpin);

  /* All done */
}


static void 
memlist_delete(ientry_t *entry)
{
  /* This shouldn't happen, but... */

  if (memlist_head == NULL)
    return;

  /* Set entry's previous peer to point to the next entry */

  if (entry->i_prev == NULL)
    memlist_head = entry->i_next;
  else
    entry->i_prev->i_next = entry->i_next;

  /* Set entry's next peer to point to the previous entry */

  if (entry->i_next == NULL)
    memlist_tail = entry->i_prev;
  else
    entry->i_next->i_prev = entry->i_prev;
}


static void 
memlist_append(ientry_t *entry)
{
  entry->i_next = NULL;

  /* Empty list */

  if (memlist_head == NULL) {
    memlist_head = memlist_tail = entry;
    entry->i_prev = NULL;
  }

  /* Non-empty list */

  else {
    entry->i_prev = memlist_tail;
    memlist_tail->i_next = entry;
    memlist_tail = entry;
  }
}
