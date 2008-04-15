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
 * $Id: ssi_mpi_init_callback.c,v 1.1 2003/10/22 22:52:28 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <stdio.h>

#include <mpi.h>
#include <all_list.h>
#include <lam-ssi.h>


/*
 * Local variables
 */
LIST *callbacks = NULL;


/*
 * Local functions
 */
static int compare(void **a, void **b);


/*
 * Register a callback to be invoked at the end of MPI_INIT*
 */
int 
lam_ssi_base_mpi_init_callback(lam_ssi_mpi_init_callback_fn_t func)
{
  void *elem;

  if (callbacks == NULL)
    callbacks = al_init(sizeof(lam_ssi_mpi_init_callback_fn_t), compare);

  /* Just to be sure, if this function is in the list already, remove it */

  elem = al_find(callbacks, &func);
  if (elem != NULL)
    al_delete(callbacks, elem);

  /* Now add it to the top of the list */
  
  al_append(callbacks, &func);

  /* All done */

  return MPI_SUCCESS;
}


/*
 * Invoke all previously-registered callbacks, and then free the list.
 */
int 
lam_ssi_base_mpi_init_callbacks_invoke(void)
{
  lam_ssi_mpi_init_callback_fn_t *func;

  /* If there's nothing in the list, we're done */

  if (callbacks == NULL)
    return MPI_SUCCESS;

  /* Call them all */

  for (func = al_top(callbacks); func != NULL; 
       func = al_next(callbacks, func))
    if ((*func)() != MPI_SUCCESS)
      return LAMERROR;

  /* All done */

  al_free(callbacks);
  callbacks = NULL;
  return MPI_SUCCESS;
}


static int
compare(void **a, void **b)
{
  if (*a < *b)
    return -1;
  else if (*a > *b)
    return 1;
  else
    return 0;
}
