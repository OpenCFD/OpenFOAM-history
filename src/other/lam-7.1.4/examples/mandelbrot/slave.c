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
 *      $Id: slave.c,v 6.8 2002/10/09 20:55:28 brbarret Exp $
 * 
 *	Function:	- slave program for mandelbrot demo
 *			- runs on all nodes
 *			- initially receives mandelbrot complex plane desc.
 *			- receives work orders from the master
 *			- sends color buffers to the master
 */

#include <stdlib.h>
#include <errno.h>

#include <mpi.h>
#include "mandel.h"

/*
 * local functions
 */
static char		xy2color(float x, float y);


int
main(int argc, char* argv[])
{
	int		ulx, uly;		/* upper-left point */
	int		lrx, lry;		/* lower-right point */
	char		*p;			/* temp pointer */
	char		*region;		/* color buffer */
	int		region_size;		/* color buffer size (bytes) */
	int		i, j;			/* loop counters */
	float		scale_x; 		/* used to convert from */
	float		scale_y;		/* pixels to colors */
	float		translate_x;		/* ... */
	float		translate_y;		/* ... */
	float		init[4];		/* scaling factors */
	int		work[4];		/* work requests */
	int		source;			/* sender's rank */
	int		tag;			/* message tag */
	MPI_Status	status;			/* returned from MPI */

	MPI_Init(&argc, &argv);
/*
 * Receive scaling and translating factors for grid.
 */
	MPI_Recv(init, 4, MPI_FLOAT, MPI_ANY_SOURCE,
			MPI_ANY_TAG, MPI_COMM_WORLD, &status);

	source = status.MPI_SOURCE;

	scale_x = init[0];
	scale_y = init[1];
	translate_x = init[2];
	translate_y = init[3];
/*
 * Loop on work requests.
 */
	for (;;) {

		MPI_Recv(work, 4, MPI_INT, MPI_ANY_SOURCE,
				MPI_ANY_TAG, MPI_COMM_WORLD, &status);
/*
 * If we get a DIE message, we are all done, so just exit.
 */
		tag = status.MPI_TAG;

		if (tag == DIETAG) {
			break;
		}
/*
 * If we get here, we've gotten a message requesting us to do
 * some more work.  Compute the given region, send the result back to 
 * the master process, and wait for a new request.
 */
		ulx = work[0];
		uly = work[1];
		lrx = work[2];
		lry = work[3];

		region_size = (lrx - ulx + 1) * (lry - uly + 1);
		region = malloc((unsigned) region_size);
		
		if (region == 0) {
			MPI_Abort(MPI_COMM_WORLD, errno);
		}

		p = region;

		for(j = uly; j <= lry; j++) {

			for(i = ulx; i <= lrx; i++) {

				*p++ = xy2color(translate_x + scale_x * i,
						translate_y + scale_y * j);

			}
		}
/*
 * Send back the original work request, followed by the result, so the
 * master can collate the answers.
 */
		MPI_Send(work, 4, MPI_INT, source, 0, MPI_COMM_WORLD);
		MPI_Send(region, region_size, MPI_CHAR,
					source, 0, MPI_COMM_WORLD);

		free(region);
	}

	MPI_Finalize();
	return 0;
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
	char		colour;
	int		i;
	complex		z, c;
	float		mag2;
	float		scale;

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

	return(colour);
}
