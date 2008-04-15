/*
 * Copyright (c) 2001-2002 The Trustees of Indiana University.  
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
 *      $Id: mempool.c,v 6.4 2003/11/14 16:10:18 jsquyres Exp $
 *
 *	Function:	- general memory pool management
 */

#include <lam_config.h>

#include <stdlib.h>
#include <string.h>

#include <etc_misc.h>
#include <lamdebug.h>
#include <typical.h>


/*
 * local variables
 */
typedef struct lam_mp_item {
  char *mpi_instance;
  struct lam_mp_item *mpi_prev, *mpi_next;
} lam_mp_item_t;

typedef struct lam_mp_type {
  int mpt_used;
  lam_mp_item_t *mpt_free_list;
  lam_mp_malloc_fn_t *mpt_malloc_fn;
  lam_mp_free_fn_t *mpt_free_fn;

  int mpt_item_size;
  int mpt_increment_size;
  int mpt_want_disinfect;
} lam_mp_type_t;

static int max_types = 0;
static lam_mp_type_t *types = NULL;
static lam_mp_item_t *item_pool = NULL;


/*
 * Local functions
 */
static lam_mp_item_t *malloc_item(void);
static void add_to_list(lam_mp_item_t **head, lam_mp_item_t *item);
static lam_mp_item_t *pop_front(lam_mp_item_t **head);
static int alloc_more(int num);


/*
 *       lam_mp_init()
 *
 *       Function:        - initialize the memory pool data structures
 *                        - sets up a given type
 *       Accepts:         - size of the type to malloc
 *                        - pointer to malloc-like function, or NULL
 *                        - pointer to free-like function, or NULL
 *                        - initial pool size, or -1 for default
 *                        - pool increment size, or -1 for default
 *       Returns:         - ID on success, LAMERROR on error
 */
int
lam_mp_init(int size, lam_mp_malloc_fn_t *mfn, lam_mp_free_fn_t *ffn, 
	    int initial_pool_size, int increment_size, int want_disinfect)
{
  int num, i;

  /* Set defaults */

  if (mfn == NULL)
    mfn = (lam_mp_malloc_fn_t*) malloc;
  if (ffn == NULL)
    ffn = (lam_mp_free_fn_t*) free;
  if (initial_pool_size == -1)
    initial_pool_size = LAM_DEFAULT_MP_POOL_SIZE;
  if (increment_size == -1)
    increment_size = LAM_DEFAULT_MP_INCR_SIZE;

  /* Find an empty spot in the types array */

  for (num = 0; num < max_types; ++num)
    if (types[num].mpt_used == 0)
      break;

  /* Alloc/extend the types array */

  if (num >= max_types) {
    if (max_types == 0) {
      max_types = LAM_DEFAULT_MP_POOL_SIZE;
      types = malloc(sizeof(lam_mp_type_t) * LAM_DEFAULT_MP_POOL_SIZE);
    } else {
      max_types *= 2;
      types = realloc(types, sizeof(lam_mp_type_t) * max_types);
    }
    if (types == NULL)
      return LAMERROR;

    for (i = num; i < max_types; ++i)
      types[i].mpt_used = 0;
  }

  /* Add an entry for the new type */

  types[num].mpt_used = 1;
  types[num].mpt_free_list = NULL;
  types[num].mpt_malloc_fn = mfn;
  types[num].mpt_free_fn = ffn;

  types[num].mpt_item_size = size;
  types[num].mpt_increment_size = initial_pool_size;
  types[num].mpt_want_disinfect = want_disinfect;

  /* Allocate the initial pool */

  if (alloc_more(num) != 0)
    return LAMERROR;
  types[num].mpt_increment_size = increment_size;

  /* All done */

  return num;
}


/*
 *       lam_mp_malloc()
 *
 *       Function:        - malloc()-like entry point
 *       Accepts:         - index of type (returned from lam_mp_init)
 *       Returns:         - ptr to valid memory, or NULL on error
 */
void *
lam_mp_malloc(int num)
{
  void *instance;
  lam_mp_type_t *type;
  lam_mp_item_t *item;

  /* Sanity check */

  if (num >= max_types || types[num].mpt_used == 0)
    return NULL;
  type = &(types[num]);

  /* Check the freelist to see if anything is already available */

  if (type->mpt_free_list == NULL)
    alloc_more(num);

  /* Take the front item off the list */

  item = pop_front(&(type->mpt_free_list));

  /* Save the instance pointer and return the item to the pool */

  instance = item->mpi_instance;
  add_to_list(&item_pool, item);

  /* Return the item */

  return instance;
}


/*
 *       lam_mp_free()
 *
 *       Function:        - free()-like entry point
 *       Accepts:         - index of type (returned from lam_mp_init)
 *                        - ptr to be returned to freelist
 *       Returns:         - void
 */
void
lam_mp_free(int num, void *instance)
{
  lam_mp_type_t *type;
  lam_mp_item_t *item;

  /* Sanity check */

  if (num >= max_types || types[num].mpt_used == 0)
    return;
  type = &(types[num]);

  /* Get an item, put this instance in it */

  item = malloc_item();
  if (item == NULL) {
    type->mpt_free_fn(instance);
    return;
  }
  item->mpi_instance = instance;

  /* Add the item to the freelist for this type */

  add_to_list(&(type->mpt_free_list), item);
}


/*
 *       lam_mp_finalize()
 *
 *       Function:        - shut down the memory pool subsystem
 *                        - free all memory
 *       Accepts:         - index of type, or -1 or shut down all
 *       Returns:         - 0 on success, LAMERROR on error
 */
int
lam_mp_finalize(int num)
{
  int i;
  lam_mp_item_t *cur, *next;

  /* If called with -1, then recurse to call for all */

  if (num == -1) {
    for (i = 0; i < max_types; ++i)
      if (types[i].mpt_used == 1)
	lam_mp_finalize(i);

    /* Free the item pool */

    for (cur = item_pool; cur != NULL; ) {
      next = cur->mpi_next;
      free(cur);
      cur = next;
    }
    item_pool = NULL;

    /* Free the types array */
    
    if (max_types > 0)
      free(types);
    max_types = 0;
  }

  /* Free up the alloc/free lists for this type, and mark the type as
     unused */

  else {
    if (types[num].mpt_used == 1) {
      for (cur = types[num].mpt_free_list; cur != NULL; ) {
	next = cur->mpi_next;
	types[num].mpt_free_fn(cur->mpi_instance);
	add_to_list(&item_pool, cur);
	cur = next;
      }
      types[num].mpt_used = 0;
    }
  }

  return 0;
}


/*
 *       malloc_item()
 *
 *       Function:        - returns a lam_mp_item_t from the pool
 *                        - expands pool if necessary
 *       Accepts:         - void
 *       Returns:         - ptr on success, NULL on error
 */
static lam_mp_item_t *
malloc_item(void)
{
  int i;
  lam_mp_item_t *item;

  /* If the pool is empty, alloc LAM_DEFAULT_MP_POOL_SIZE items and
     put them in the pool */

  if (item_pool == NULL)
    for (i = 0; i < LAM_DEFAULT_MP_POOL_SIZE; ++i) {
      item = malloc(sizeof(lam_mp_item_t));
      LAM_ZERO_ME(*item);
      add_to_list(&item_pool, item);
    }

  /* Return the front item from the pool */

  return pop_front(&item_pool);
}


/*
 *       add_to_list()
 *
 *       Function:        - put an item on a list
 *                        - if 
 *       Accepts:         - ptr to list head
 *                        - item to put on list
 *       Returns:         - void
 */
static void
add_to_list(lam_mp_item_t **head, lam_mp_item_t *item)
{
  if (*head == NULL) {
    *head = item;
    item->mpi_prev = item->mpi_next = NULL;
  } else {
#if LAM_WANT_DEBUG
    /* If we're in debugging mode, add this to the *end* of the list
       so as to recycle buffers as little as possible -- or, at least
       re-use them with as great a period as possible */
    lam_mp_item_t *p;
    for (p = (*head); p->mpi_next != NULL; p = p->mpi_next)
      continue;
    p->mpi_next = item;
    item->mpi_prev = p;
    item->mpi_next = NULL;
#else
    /* Otherwise, put it on the top of the list -- O(1) */
    
    item->mpi_next = *head;
    item->mpi_prev = NULL;
    
    (*head)->mpi_prev = item;
    *head = item;
#endif
  }
}


/*
 *       pop_front()
 *
 *       Function:        - STL-like func to return front item from list
 *                        - remove front item from list as well
 *       Accepts:         - pointer to head of item list
 *       Returns:         - front item of list, or NULL if list empty
 */
static lam_mp_item_t *
pop_front(lam_mp_item_t **head)
{
  lam_mp_item_t *item;

  if (*head == NULL)
    return NULL;
  else {
    item = *head;
    *head = item->mpi_next;
    if (*head != NULL)
      (*head)->mpi_prev = NULL;

    return item;
  }
}


/*
 *       alloc_more()
 *
 *       Function:        - alloc more items on a given type
 *       Accepts:         - index of type
 *       Returns:         - 0 on success, LAMERROR on error
 */
static int
alloc_more(int num)
{
  int i;
  lam_mp_item_t *item;
  lam_mp_type_t *type;

  /* Sanity check */

  if (num >= max_types || types[num].mpt_used == 0)
    return LAMERROR;

  /* Alloc out more item_t's and a block of new memory to hold the new
     instances */

  type = &(types[num]);
  for (i = 0; i < type->mpt_increment_size; ++i) {
    item = malloc_item();
    if (item == NULL)
      return LAMERROR;

    LAM_ZERO_ME(*item);
    item->mpi_instance = type->mpt_malloc_fn(type->mpt_item_size);
#if LAM_DISINFECT
    if (type->mpt_want_disinfect)
      memset(item->mpi_instance, 0, type->mpt_item_size);
#endif

    add_to_list(&(type->mpt_free_list), item);
  }

  return 0;
}
