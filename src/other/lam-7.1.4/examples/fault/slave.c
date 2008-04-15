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
 * $Id: slave.c,v 6.8 2003/04/23 22:32:12 jsquyres Exp $
 * 
 *	Function:	- slave program for mandelbrot demo
 *			- fault tolerant version
 *			- initially receives mandelbrot complex plane desc.
 *			- receives work orders from the master
 *			- sends color buffers to the master
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <mpi.h>
#include "mandel.h"

/*
 * local functions
 */
static char xy2color(float x, float y);


int
main(int argc, char *argv[])
{
  int ulx, uly;			/* upper-left point */
  int lrx, lry;			/* lower-right point */
  char *p;			/* temp pointer */
  char *region;			/* color buffer */
  int region_size;		/* color buffer size (bytes) */
  int i, j;			/* loop counters */
  float scale_x;		/* used to convert from */
  float scale_y;		/* pixels to colors */
  float translate_x;		/* ... */
  float translate_y;		/* ... */
  float init[4];		/* scaling factors */
  int work[4];			/* work requests */
  int size;			/* world size */
  MPI_Comm comm;		/* communicator with master */
  MPI_Status status;		/* returned from MPI */

  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_get_parent(&comm);
  if (comm == MPI_COMM_NULL) {
    printf("No parent!");
    MPI_Abort(MPI_COMM_WORLD, (1 << 16) + 1);
  }

  /* Receive scaling and translating factors for grid. */

  MPI_Recv((void *) init, 4, MPI_FLOAT, 0, WORKTAG, comm, MPI_STATUS_IGNORE);

  scale_x = init[0];
  scale_y = init[1];
  translate_x = init[2];
  translate_y = init[3];

  /* Loop on work requests. */

  for (;;) {
    MPI_Recv((void *) work, 4, MPI_INT, 0, MPI_ANY_TAG, comm, &status);

    /* If we get a DIE message, we are all done, so just exit. */

    if (status.MPI_TAG == DIETAG)
      break;

    /* If we get here, we've gotten a message requesting us to do some
       more work.  Compute the given region, send the result back to
       the master process, and wait for a new request.  */

    ulx = work[0];
    lrx = work[1];
    uly = work[2];
    lry = work[3];

    region_size = (lrx - ulx + 1) * (lry - uly + 1);
    region = malloc((unsigned) region_size);
    if (region == 0)
      MPI_Abort(MPI_COMM_SELF, errno);

    p = region;

    for (j = uly; j <= lry; j++) {

      for (i = ulx; i <= lrx; i++) {
	*p++ = xy2color(translate_x + scale_x * i,
			translate_y + scale_y * j);
      }
    }

    /* Send back the computed colours. */

    MPI_Send(region, region_size, MPI_CHAR, 0, REPLYTAG, comm);

    free(region);
  }

  MPI_Finalize();
  return (0);
}


/*
 *	xy2color
 * 
 *	Function:	- Given a point on the complex plane x+iy, 
 *			  return color based on how close the
 *			  point is to the Mandelbrot set.
 *	Accepts:	- point in complex plane
 *	Returns:	- 1 byte color
 */
static char
xy2color(float x, float y)
{
  char colour;
  int i;
  complex z, c;
  float mag2;
  float scale;

  scale = ((float) 255) / MAX_ITERATIONS;
  colour = -1;

  z.r = z.i = 0;
  c.r = x;
  c.i = y;

  for (i = 0; i < MAX_ITERATIONS; ++i) {

    x = (z.r * z.r) - (z.i * z.i) + c.r;
    y = (2 * z.r * z.i) + c.i;

    z.r = x;
    z.i = y;

    mag2 = (z.r * z.r) + (z.i * z.i);

    if (mag2 >= 4) {
      colour = (char) ((i * scale) + 0.5);
      break;
    }
  }

  return (colour);
}
