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
 * $Id: rpi_ib_hashmap.h,v 1.2 2003/12/23 02:06:41 vsahay Exp $
 */

#ifndef LAM_MAP_H
#define LAM_MAP_H

#include <stdlib.h>

/* 
 * Data structures and datatypes 
 */

typedef enum {RED, BLACK} nodecolor;

/* Forward reference  */
struct lam_map_entry_t;

/* the tree node  */
typedef struct lam_map_entry_t {
  void *key;
  void *value;
  nodecolor color;
  struct lam_map_entry_t *parent; /* parent - can be NULL */
  struct lam_map_entry_t *left; /* left child - can be nill  */
  struct lam_map_entry_t *right; /* right child - can be nill  */

  struct lam_map_entry_t *deleted_next; /* Only set if on to_be_deleted list */
  struct lam_map_entry_t *freed_next; /* Only set if on to_be_freed list */
} lam_map_entry;


typedef lam_map_entry *lam_map_entry_ptr; /* node ptr  */
typedef int (*comp_fn)(void *elem1, void *elem2);

/* actual map  */
typedef struct {
  lam_map_entry_ptr *root_ptr;
  comp_fn comp;
  lam_map_entry_ptr nill;
  int node_mem_id;
  int key_mem_id;
  int value_mem_id;
  int keysize;
  int valuesize;
  int map_size;

  struct lam_map_entry_t *to_be_deleted;
  struct lam_map_entry_t *to_be_freed;
} lam_map;


/* Function pointers for map traversal function */

typedef int lam_map_condition(void *entry);
typedef void lam_map_action(void *entry);
  
/*
 * Function protoypes 
 */

/* Public  */
lam_map *lam_map_create(comp_fn comp, int keysize, int valuesize);
int lam_map_insert(lam_map *lmap, void *key, void *value);
void *lam_map_find(lam_map *lmap, void *key, int *found);
int lam_map_delete(lam_map *lmap, void *key, lam_map_entry_ptr p);
int lam_map_destroy(lam_map **lmap, lam_map_action *action);

int lam_map_traverse_and_delete(lam_map *lmap, lam_map_condition *cond,
				 lam_map_action *action);


#endif

