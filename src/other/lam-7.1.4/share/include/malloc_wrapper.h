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
 *	Function:	- wrap memory allocation for OS-bybass communication
 */

#ifndef MALLOC_WRAPPER_H_
#define MALLOC_WRAPPER_H_

#include <lam_config.h>

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

  typedef void (lam_memory_unpin_t)(void *buf, size_t length);
  void lam_register_free_handler_init(void);
  int lam_register_free_handler(lam_memory_unpin_t *func);
  void lam_handle_free(void *buf, size_t length);

#ifdef __cplusplus
}
#endif

#endif /* MALLOC_WRAPPER_H_ */
