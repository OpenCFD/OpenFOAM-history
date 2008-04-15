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
 *	$Id: all_array.c,v 6.2 2003/08/21 14:21:44 jsquyres Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <all_array.h>
#include <lam.h>


/*
 * local variables
 */
static const int alloc_block = 10;


/*
 * local functions
 */
static int expand_array(lam_array_t *la, int how_many);



/*
 *	lam_arr_init
 *
 */
lam_array_t *
lam_arr_init(int elemsize, lam_array_comp_fn_t comp)
{
  /* Create the entry and fill it up */

  lam_array_t *a = malloc(sizeof(lam_array_t));
  if (a == NULL)
    return NULL;

  a->la_element_size = elemsize;
  a->la_num_allocated = 0;
  a->la_num_used = 0;
  a->la_array = NULL;
  a->la_comp = comp;

  expand_array(a, alloc_block);

  return a;
}


void *
lam_arr_get(lam_array_t *la)
{
  return (void*) la->la_array;
}


int 
lam_arr_size(lam_array_t *la)
{
  return la->la_num_used;
}


int 
lam_arr_append(lam_array_t *la, void *element)
{
  /* Expand the array if we need to */

  if (la->la_num_used == la->la_num_allocated)
    if (expand_array(la, alloc_block) == LAMERROR)
      return LAMERROR;

  /* Append the new element at the end of the list */

  memcpy(la->la_array + (la->la_num_used * la->la_element_size),
	 element, la->la_element_size);
  ++la->la_num_used;

  return 0;
}


int 
lam_arr_insert(lam_array_t *la, void *element, int before)
{
  int i, size;

  /* Expand the array if we need to.  First catch the case where we
     are trying to insert before an item which is actually outside the
     range of the currently allocated array.  Expand the array enough
     to fit. */

  if (before >= la->la_num_allocated) {
    size = la->la_num_allocated;
    while (size < before + 1)
      size += alloc_block;
    if (expand_array(la, size - (la->la_num_allocated)) == LAMERROR)
      return LAMERROR;
  }

  /* Next, catch the case where the array is currently full, and we
     need to expand by at least one.  If we met the previous case, we
     won't meet this one. */

  if (la->la_num_used == la->la_num_allocated ||
      before >= la->la_num_allocated)
    if (expand_array(la, alloc_block) == LAMERROR)
      return LAMERROR;

  /* Shift everybody down one.  Unfortunately, there's no way to do
     this in one step.  So either 2 memcpy's (out to a temp buffer),
     or loop over copying the individual elements down one.  Do the
     latter, as a) this isn't high performance, and b) it's a little
     more clear. */

  for (i = la->la_num_used; i >= before; --i)
    memcpy(la->la_array + ((i + 1) * la->la_element_size),
	   la->la_array + (i * la->la_element_size),
	   la->la_element_size);
  ++la->la_num_used;

  /* Now copy the new element into the vacant slot */

  memcpy(la->la_array + (before * la->la_element_size), element,
	 la->la_element_size);
  return 0;
}


int 
lam_arr_find(lam_array_t *la, void *element)
{
  int i;

  /* Loop through the array calling the user-provided comparison
     function, or memcmp (if one was not provided) */

  for (i = 0; i < la->la_num_used; ++i)
    if ((la->la_comp != NULL &&
	 la->la_comp(element, 
		     la->la_array + (i * la->la_element_size)) == 0) ||
	memcmp(element, la->la_array + (i * la->la_element_size), 
	       la->la_element_size) == 0)
      return i;

  return LAMERROR;
}


int 
lam_arr_remove(lam_array_t *la, void *element)
{
  /* Find the index where this element lives */
  
  int index = lam_arr_find(la, element);
  if (index == LAMERROR)
    return LAMERROR;

  /* Now remove it */

  return lam_arr_remove_index(la, index);
}


int 
lam_arr_remove_index(lam_array_t *la, int index)
{
  /* If we are deleting the last entry, just decremember
     la_num_used */

  if (index == la->la_num_used - 1)
    --la->la_num_used;

  /* If there are entires below this one, memcpy the whole block of
     entries below the index to be deleted up one index.  Technically,
     it's not safe to memcpy(a, b, size) where (a+size)>b, so we have
     to do it in stages.  Bonk. */

  else if (index < la->la_num_used) {
    --la->la_num_used;
    for (; index < la->la_num_used; ++index) {
      memcpy(la->la_array + (index * la->la_element_size),
             la->la_array + ((index + 1) * la->la_element_size),
             la->la_element_size);
    }
  }

  else
    return LAMERROR;

  return 0;
}


int 
lam_arr_free(lam_array_t *la)
{
  /* Free the array and container */

  if (la != NULL) {
    if (la->la_array != NULL)
      free(la->la_array);
    free(la);
  }

  return 0;
}


static int 
expand_array(lam_array_t *la, int how_many)
{
  /* If this is the first time, just malloc it */

  if (la->la_array == NULL) {
    la->la_array = malloc(la->la_element_size * how_many);
    if (la->la_array == NULL)
      return LAMERROR;
    la->la_num_allocated = how_many;
  } 

  /* Otherwise, realloc it */

  else {
    la->la_array = realloc(la->la_array, la->la_element_size *
			      (la->la_num_allocated + how_many));
    if (la->la_array == NULL)
      return LAMERROR;
    la->la_num_allocated += how_many;
  }

  return 0;
}
