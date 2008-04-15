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
 * $Id: ssi_rpi_ib_hashmap.c,v 1.3 2003/12/24 23:53:29 vsahay Exp $
 *
 * Function   -    Implement a hash-map using red-black tree
 */

#include <string.h>

#include <rpi_ib_hashmap.h>
#include <etc_misc.h>


/*
 * Private variables
 */
static lam_map_entry_ptr nill;
static int no_malloc_free = 0;


/* 
 * Private functions 
 */
static lam_map_entry_ptr btree_insert(lam_map *lmap, void *key, void *value);
static int btree_delete_fixup(lam_map *lmap, lam_map_entry_ptr x);

/* VPS: The only difference between btree_find and lam_map_find is the
   return type. The btre_find returns a node, which is reequired
   during delete operation whereas the public interface just returns
   the key (and that is what the user would want -- dont want to
   expose the internal data structure lam_map_entry_t to the
   user). Could have done this using a wrapper function, which then
   calls the common internal function, but want to avoid the function
   call overhead for find since its a crucial operation and should be
   efficient. So went ahead with code replication */

static lam_map_entry_ptr btree_find(lam_map *lmap, void *key, int *found);

/* VPS: These two below use ptr-to-ptr of node as input  */
static int left_rotate(lam_map *lmap, lam_map_entry_ptr x);
static int right_rotate(lam_map *lmap, lam_map_entry_ptr x);

static void inorder_destroy(lam_map *lmap, lam_map_action *action, 
			    lam_map_entry_ptr node);
static void inorder_traversal(lam_map *lmap, lam_map_condition *cond,
			      lam_map_action *action, lam_map_entry_ptr node);

static lam_map_entry_ptr btree_successor(lam_map_entry_ptr node);
static void process_to_be_deleted(lam_map *lmap);
static void process_to_be_freed(lam_map *lmap);


/* Create the hashmap  */

lam_map *
lam_map_create(comp_fn comp, int keysize, int valuesize) 
{
  lam_map *lmap;
    
  lmap = (lam_map *) malloc (sizeof(lam_map));
  if (lmap == NULL)
    return NULL;

  /* root_ptr is a double pointer to the first node here  */
  lmap->root_ptr = (lam_map_entry_ptr *) malloc (sizeof(lam_map_entry_ptr));
  if (lmap->root_ptr == NULL)
    return NULL;

  lmap->nill = (lam_map_entry_ptr) malloc (sizeof(lam_map_entry));
  if (lmap->nill == NULL)
    return NULL;

  nill = lmap->nill;
  nill->color = BLACK;
  nill->left = NULL;
  nill->right = NULL;
  nill->parent = NULL;
  nill->deleted_next = NULL;
  nill->freed_next = NULL;

  *(lmap->root_ptr) = nill;
  lmap->comp = comp;

  lmap->node_mem_id = lam_mp_init(sizeof(lam_map_entry), NULL, NULL, 1024,
                                  1024, 1);
  lmap->key_mem_id = lam_mp_init(keysize, NULL, NULL, 1024, 1024, 1);
  lmap->value_mem_id = lam_mp_init(valuesize, NULL, NULL, 1024, 1024, 1);
  
  lmap->keysize = keysize;
  lmap->valuesize = valuesize;

  lmap->map_size = 0;
  lmap->to_be_deleted = NULL;
  lmap->to_be_freed = NULL;

  return lmap;
}


/* This inserts the element in the map. It makes a copy of the user
   key and value and hence user has to specify the key and value size 
   (they can be strings of variable size -- hence the size parameter
   in insert function. If the user key/value is a structure, and this 
   structure has any pointers, its the user's responsibility to see
   that the pointers are have global scope if requried. This fucntion
   would just coopy the struct contents in this case -- which includes
   the pointer value in the struct rather than the thing to which
   the pointer is pointing within the struct */

int 
lam_map_insert(lam_map *lmap, void *key, void *value)
{
  lam_map_entry_ptr node, y;

  /* For good measure, process the to-be-deleted and to-be-freed
     lists, if they exist from a previous operation */

  if (lmap->to_be_deleted != NULL)
    process_to_be_deleted(lmap);
  if (lmap->to_be_freed != NULL && no_malloc_free == 0)
    process_to_be_freed(lmap);

  /* Now do the actual insert */

  node = btree_insert(lmap, key, value);
  if (node == NULL)
    return -1;

  while ((node != *(lmap->root_ptr)) && (node->parent->color == RED)) {
    if (node->parent == node->parent->parent->left) {
      y = node->parent->parent->right;
      if (y->color == RED) {
	node->parent->color = BLACK;
	y->color = BLACK;
	node->parent->parent->color = RED;
	node = node->parent->parent;
      } else {
	if (node == node->parent->right) {
	  node = node->parent;
	  left_rotate(lmap, node);
	}
	node->parent->color = BLACK;
	node->parent->parent->color = RED;
	right_rotate(lmap, node->parent->parent);
      }
    } else {
      y = node->parent->parent->left;
      if (y->color == RED) {
	node->parent->color = BLACK;
	y->color = BLACK;
	node->parent->parent->color = RED;
	node = node->parent->parent;
      } else {
	if (node == node->parent->left) {
	  node = node->parent;
	  right_rotate(lmap, node);
	}
	node->parent->color = BLACK;
	node->parent->parent->color = RED;
	left_rotate(lmap, node->parent->parent);
      }
    }
  }

  (*(lmap->root_ptr))->color = BLACK;
  return 0;
}


/* Find value corresponding to a key in the hashmap Takes care of the
   situation where an element of the map is NULL */

void *
lam_map_find(lam_map *lmap, void *key, int *found)
{
  lam_map_entry_ptr node;
  int compvalue;

  node = *(lmap->root_ptr);
  while (node != nill) {
    compvalue = lmap->comp(key, node->key);
    
    if (compvalue == 0) {
      *found = 1;
      return node->value;
    }

    node = (compvalue < 0) ? node->left : node->right;
  }
  
  *found = 0;
  return NULL;
}
 

/* Delete a key-value from the hashmap  */

int 
lam_map_delete(lam_map *lmap, void *key, lam_map_entry_ptr p)
{
  lam_map_entry_ptr todelete, y;
  int found;

  /* For good measure, process the to-be-freed list if it exists from
     a previous operation */

  if (lmap->to_be_freed != NULL && no_malloc_free == 0)
    process_to_be_freed(lmap);

  /* See if we are given the entry pointer or we need to find it based
     on a key */

  if (NULL == p) {
    p = btree_find(lmap, key, &found);
  }

  if ((p == NULL) && (found == 0))
    return -1;

  if ((p->left == nill) || (p->right == nill)) 
    todelete = p;
  else
    todelete = btree_successor(p);

  if (todelete->left != nill) {
    y = todelete->left;
  } else {
    y = todelete->right;
  }

  y->parent = todelete->parent;

  if (y->parent == NULL) {
    *(lmap->root_ptr) = y;
  } else {
    if (todelete == todelete->parent->left)
     todelete->parent->left = y;
    else
      todelete->parent->right = y;
  }
  
  if (todelete != p) {
    p->key = todelete->key;
    p->value = todelete->value;
  }

  if (todelete->color == BLACK)
    btree_delete_fixup(lmap, y);
  
  /* If no_malloc_free is set, then don't actually call lam_mp_free()
     here because it may actually invoke malloc().  Instead, put the
     entry in the to-be-freed list. */

  if (no_malloc_free == 1) {
    if (todelete != p) {
      todelete->key = NULL;
      todelete->value = NULL;
    }
    todelete->freed_next = lmap->to_be_freed;
    lmap->to_be_freed = todelete;
  } else {
    if (todelete == p) {
      lam_mp_free(lmap->key_mem_id, todelete->key);
      lam_mp_free(lmap->value_mem_id, todelete->value);
    }
    lam_mp_free(lmap->node_mem_id, todelete);
  }

  --lmap->map_size;
  return 0;
}


/*
 * This function is *only* invoked during the free hook handler, where
 * we can't call malloc or free.  So set a flag so that
 * lam_map_delete() won't actually do anything that will cause
 * malloc()/free() to be invoked
 */
int 
lam_map_traverse_and_delete(lam_map *lmap, lam_map_condition *cond,
                            lam_map_action *action)
{
  lam_map_entry_ptr node = *(lmap->root_ptr);

  if ((cond == NULL) || (action == NULL)) {
    return -1;
  }
  
  no_malloc_free = 1;
  inorder_traversal(lmap, cond, action, node);

  /* For all the entries that were marked as "to be deleted" during
     the inorder traversal, delete them from the map */

  process_to_be_deleted(lmap);
  no_malloc_free = 0;
  
  return 0;
}


/* 
 * Destroy the hashmap - recursive inorder traversal for delete 
 */
int
lam_map_destroy(lam_map **lmap_ptr, lam_map_action *action)
{
  lam_map *lmap = *(lmap_ptr);

  /* Go kill all the entries.  Don't need to worry about entries on
     the to-be-deleted list -- they'll be deleted as part of this
     inorder traversal. */

  inorder_destroy(lmap, action, *(lmap->root_ptr));

  /* Now free the root -- root does not get free'd in the above
     inorder destroy  */

  if (*(lmap->root_ptr) != nill) {
    lam_mp_free(lmap->node_mem_id, *(lmap->root_ptr));
  }

  /* Now double check that there isn't anything on the to-be-freed
     list */

  if (lmap->to_be_freed != NULL)
    process_to_be_freed(lmap);

  /* Ok, kill the rest */

  lam_mp_finalize(lmap->key_mem_id);
  lam_mp_finalize(lmap->value_mem_id);
  lam_mp_finalize(lmap->node_mem_id);

  free(lmap->root_ptr);
  free(nill);
  lmap->root_ptr = NULL;
  free(lmap);
  *(lmap_ptr) = NULL;
  return 0;
}


/* Find the next inorder successor of a node  */

static lam_map_entry_ptr 
btree_successor(lam_map_entry_ptr node)
{
  lam_map_entry_ptr p, parent;

  if (node->right == nill) {
    p = node->parent;
    while ((p != NULL) && (node == p->right)) {
      node = p;
      p = p->parent;
    }
    return p;
  } 

  parent = node->right;
  p = parent->left;
  while (p != nill) {
    parent = p;
    p = p->left;
  }
  return parent;
}
  

/* Insert an element in the normal binary search tree fashion  */

/* VPS: not seen yet what happens if someone enters a NULL pointer  */

static lam_map_entry_ptr
btree_insert(lam_map *lmap, void *key, void *value)
{
  lam_map_entry_ptr root, n, parent;
  lam_map_entry *node;
  
  root = *(lmap->root_ptr);
  node = lam_mp_malloc(lmap->node_mem_id);
  
  /* init the node  */
  node->key = lam_mp_malloc(lmap->key_mem_id);
  if (node->key == NULL)
    return NULL;
  memcpy(node->key, key, lmap->keysize);

  node->value = lam_mp_malloc(lmap->value_mem_id);
  if (node->value == NULL)
    return NULL;
  memcpy(node->value, value, lmap->valuesize);
  
  node->color = RED;
  node->parent = NULL;
  node->left = nill;
  node->right = nill;
  node->deleted_next = NULL;
  node->freed_next = NULL;

  /* insert this node in the tree now  */
  if (root == nill) {
    *(lmap->root_ptr) = node;
    return node;
  }
  
  n = root;
  while (n!= nill) {
    parent = n;
    (lmap->comp(node->key, n->key) <= 0) ? (n = n->left) : (n = n->right);
  }

  (lmap->comp(node->key, parent->key) <= 0) ? (parent->left = node) :
    (parent->right = node);

  node->parent = parent;
  node->left = nill;
  node->right = nill;
  ++lmap->map_size;
  return node;
}


/* Find an element in the binary search tree  */

static lam_map_entry_ptr
btree_find(lam_map *lmap, void *key, int *found)
{
  lam_map_entry_ptr node;
  int compvalue;

  node = *(lmap->root_ptr);
  while (node != nill) {
    compvalue = lmap->comp(key, node->key);
    if (compvalue == 0) {
      *found = 1;
      return node;
    }

    node = (compvalue < 0) ? node->left : node->right;
  }
  
  *found = 0;
  return NULL;
}
 

/* Fixup the balance of the btree after deletion  */      

static int
btree_delete_fixup(lam_map *lmap, lam_map_entry_ptr x) 
{
  lam_map_entry_ptr w;
  
  while ((x != *(lmap->root_ptr)) && (x->color == BLACK)) {
      if (x == x->parent->left) {
	  w = x->parent->right;

	  if (w->color == RED) {
	      w->color = BLACK;
	      x->parent->color = RED;
	      left_rotate(lmap, x->parent);
	      w = x->parent->right;
	  }

	  if ((w->left->color == BLACK) && (w->right->color == BLACK)) {
	      w->color = RED;
	      x = x->parent;
	  } else {
	      if (w->right->color == BLACK) {
		  w->left->color = BLACK;
		  w->color = RED;
		  right_rotate(lmap, w);
		  w = x->parent->right;
	      }

	      w->color = x->parent->color;
	      x->parent->color = BLACK;

	      w->right->color = BLACK;

	      left_rotate(lmap, x->parent);
	      x = *(lmap->root_ptr);
	  }
      
      } else { /* right  */

	  w = x->parent->left;
	  if (w->color == RED) {
	      w->color = BLACK;
	      x->parent->color = RED;
	      right_rotate(lmap, x->parent);
	      w = x->parent->left;
	  }

	  if ((w->right->color == BLACK) && (w->left->color == BLACK)) {
	      w->color = RED;
	      x = x->parent;
	  } else {
	      if (w->left->color == BLACK) {
		  w->right->color = BLACK;
		  w->color = RED;
		  left_rotate(lmap, w);
		  w = x->parent->left;
	      }

	      w->color = x->parent->color;
	      x->parent->color = BLACK;

	      w->left->color = BLACK;

	      right_rotate(lmap, x->parent);
	      x = *(lmap->root_ptr);
	  }
      }
  }
	 
  x->color = BLACK;
  return 0;
}    


/* Free the nodes in inorder fashion  */

static void
inorder_destroy(lam_map *lmap, lam_map_action *action, 
		lam_map_entry_ptr node)
{
  if (node == nill)
    return;
  
  inorder_destroy(lmap, action, node->left);

  /* Do the action and destroy your internal pointers  */
  action(node->value);
  lam_mp_free(lmap->key_mem_id, node->key);
  lam_mp_free(lmap->value_mem_id, node->value);

  if (node->left != nill) {
    lam_mp_free(lmap->node_mem_id, node->left);
  }

  inorder_destroy(lmap, action, node->right);
  if (node->right != nill) {
    lam_mp_free(lmap->node_mem_id, node->right);
  }
}


/*
 * Perform a test on every element in the hashmap.  If it's true,
 * perform an action on the entry.
 */
static void
inorder_traversal(lam_map *lmap, lam_map_condition *cond,
		  lam_map_action *action, lam_map_entry_ptr node)
{
  if (node == nill)
    return;
  
  inorder_traversal(lmap, cond, action, node->left);

  if (cond(node->value) != 0) {
    action(node->value);

    /* Add this node to the list to be deleted.  Do *NOT* delete it
       now, because that can change the structure of the tree that
       we're currently traversing, and potentially cause us to miss
       some entries, or even visit some entries more than once.
       Instead, add this entry to the to_be_deleted list, which will
       be processed back up in lam_map_traverse_and_delete(). */

    node->deleted_next = lmap->to_be_deleted;
    lmap->to_be_deleted = node;
  }

  inorder_traversal(lmap, cond, action, node->right);
}


/* Left rotate the tree  */

static int
left_rotate(lam_map *lmap, lam_map_entry_ptr x)
{
  lam_map_entry_ptr y;

  if (x == nill)
    return -1;

  y = x->right;
  if (y == nill)
    return -1;

  if (x->parent != NULL) {
    if (x == x->parent->left)
      x->parent->left = y;
    else
      x->parent->right = y;
  } else {
    *(lmap->root_ptr) = y;
  }

  y->parent = x->parent;
  x->parent = y;
  x->right = y->left;
  y->left = x;
  
  if (x->right != nill)
    x->right->parent = x;

  if (*(lmap->root_ptr) == x)
    *(lmap->root_ptr) = y;

  return 0;
}


/* Right rotate the tree  */

static int 
right_rotate(lam_map *lmap, lam_map_entry_ptr x) 
{
  lam_map_entry_ptr y;

  if (x == nill)
    return -1;
  
  y = x->left;
  if (y == nill)
    return -1;

  if (x->parent != NULL) {
    if (x == x->parent->left)
       x->parent->left = y;
    else
      x->parent->right = y;
  } else {
    *(lmap->root_ptr) = y;
  }

  y->parent = x->parent;
  x->parent = y;
  x->left = y->right;
  y->right = x;
  
  if (x->left != nill)
    x->left->parent = x;

  if (*(lmap->root_ptr) == x)
    *(lmap->root_ptr) = y;

  return 0;
}     


static void
process_to_be_deleted(lam_map *lmap)
{
  struct lam_map_entry_t *entry, *next;

  for (entry = lmap->to_be_deleted; entry != NULL; entry = next) {
    next = entry->deleted_next;
    lam_map_delete(lmap, NULL, entry);
  }
  lmap->to_be_deleted = NULL;
}


static void
process_to_be_freed(lam_map *lmap)
{
#if 1
  struct lam_map_entry_t *entry, *next;

  for (entry = lmap->to_be_freed; entry != NULL; entry = next) {
    next = entry->freed_next;
    if (entry->key != NULL)
      lam_mp_free(lmap->key_mem_id, entry->key);
    if (entry->value != NULL)
      lam_mp_free(lmap->value_mem_id, entry->value);
    lam_mp_free(lmap->node_mem_id, entry);
  }
#endif
  lmap->to_be_freed = NULL;
}
