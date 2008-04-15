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
 *	$Id: all_array.h,v 6.1 2003/02/22 15:46:31 jsquyres Exp $
 * 
 *	Function:	- constants and templates associated with all_array
 */

#ifndef LAM_ALL_ARRAY
#define LAM_ALL_ARRAY


/*
 * Relevant structures
 */

typedef int (*lam_array_comp_fn_t)(const void *, const void *);

typedef struct lam_array {
  int la_element_size;
  int la_num_allocated;
  int la_num_used;

  char *la_array;

  lam_array_comp_fn_t la_comp;
} lam_array_t;


/*
 * Access functions
 */
#ifdef __cplusplus
extern "C" {
#endif

  lam_array_t *lam_arr_init(int elemsize, lam_array_comp_fn_t comp);

  void *lam_arr_get(lam_array_t *array);
  int lam_arr_size(lam_array_t *array);
  int lam_arr_append(lam_array_t *array, void *element);
  int lam_arr_insert(lam_array_t *array, void *element, int before);
  int lam_arr_find(lam_array_t *array, void *element);
  int lam_arr_remove(lam_array_t *array, void *element);
  int lam_arr_remove_index(lam_array_t *array, int index);
  int lam_arr_free(lam_array_t *array);

#ifdef __cplusplus
}
#endif

#endif
