/*
 * $Id: hello.c,v 1.3 2002/11/23 04:06:57 jsquyres Exp $
 *
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
 * Sample hello world MPI program for testing MPI.
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int 
main(int argc, char **argv)
{
  int rank, size;

  /* Start up MPI */

  MPI_Init(&argc, &argv);

  /* Get some info about MPI */

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  /* Print out the canonical "hello world" message */

  printf("Hello, world!  I am %d of %d\n", rank, size);

  /* All done */

  MPI_Finalize();
  return 0;
}
