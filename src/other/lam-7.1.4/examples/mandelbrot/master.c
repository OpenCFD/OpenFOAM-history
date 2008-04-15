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
 *	$Id: master.c,v 6.11 2002/10/09 20:55:27 brbarret Exp $
 *
 *	Function:	- master program of mandelbrot demo
 *			- distributes work orders to the slaves
 *			- collects pixel buffers from the slaves
 *			- creates a Sun rasterfile from the total result
 *			- grid means the whole domain
 *			- region means the subdomain
 *
 *	Returns:	- errno if fatal error terminated program
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <mpi.h>
#include "mandel.h"

/*
 * local structures
 */
struct sunras {					/* Sun raster file header */
	int		ras_magic;
	int		ras_width;
	int		ras_height;
	int		ras_depth;
	int		ras_length;
	int		ras_type;
	int		ras_maptype;
	int		ras_maplength;
};

/*
 * local functions
 */
static int		harvest(void);		/* gets results */
static void		seed(int rank, int x, int y); /* distributes work */
static void		color_map(int fd);	/* generate color map */
static void		check_endian(struct sunras *rasterfile);

/*
 * local variables
 */
static int		grid_dim[2];		/* grid size in pixels */
static int		region_dim[2];		/* region size in pixels */
static char		*grid;			/* color buffer buffer */
static char		*region;		/* color region buffer */
static int		myrank;			/* compute task's rank */


int
main(int argc, char* argv[])
{
	float		grid_coord[4];		/* complex grid description */
	float		init[4];		/* scale and translate data */
	int		n;
	int		ntasks;			/* # compute tasks */
	int		rank;			/* compute task's rank */
	int		x = 0, y;		/* pixel coordinates */
	int		fd;			/* output raster file */
	struct sunras	rasterfile;
/*
 * Initialize MPI.
 */
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
/*
 * Hardcode the universe.
 */
	grid_coord[0] = -2;
	grid_coord[1] = 1.25;
	grid_coord[2] = 0.5;
	grid_coord[3] = -1.25;

	grid_dim[0] = 512;
	grid_dim[1] = 512;

	region_dim[0] = 20;
	region_dim[1] = 20;
/*
 * Allocate the raster file buffer.
 */
	grid = malloc((unsigned) (grid_dim[0] * grid_dim[1]));
	if (grid == 0) MPI_Abort(MPI_COMM_WORLD, errno);
/*
 * Allocate space for region.
 */
	region = malloc((unsigned) (region_dim[0] * region_dim[1]));
	if (region == 0) MPI_Abort(MPI_COMM_WORLD, errno);
/*
 * Send translate and scale factors to each slave.
 */
	init[0] = (grid_coord[2] - grid_coord[0]) / grid_dim[0];
	init[1] = -((grid_coord[1] - grid_coord[3]) / grid_dim[1]);
	init[2] = grid_coord[0];
	init[3] = grid_coord[1];

	for (rank = 0; rank < ntasks; ++rank) {

		if (rank != myrank) {
			MPI_Send(init, 4, MPI_FLOAT, rank,
					WORKTAG, MPI_COMM_WORLD);
		}
	}
/*
 * Distribute first work order to all slaves.
 */
	n = 0;
	y = 0;

	while ((y < grid_dim[1]) && (n < ntasks)) {

		for (x = 0; (x < grid_dim[0]) && (n < ntasks);
						x += region_dim[0]) {

			if (n == myrank) {
				++n;
			}

			if (n < ntasks) {
				seed(n++, x, y);
			}
		}

		if (n < ntasks) {
			y += region_dim[1];
		}
	}
/*
 * Collect a result and return a new work order until the
 * entire grid is computed.
 */
	for (; y < grid_dim[1]; y += region_dim[1]) {

		for (; x < grid_dim[0]; x += region_dim[0]) {
			n = harvest();
			seed(n, x, y);
		}

		x = 0;
	}
/*
 * Collect remaining results.
 */
	for (n = 0; n < ntasks - 1; ++n) {
		harvest();
	}
/*
 * We have all the answers now, so kill the workers.
 */
	for (rank = 0; rank < ntasks; ++rank) {

		if (rank != myrank) {
			MPI_Send(0, 0, MPI_INT, rank, DIETAG, MPI_COMM_WORLD);
		}
	}
/*
 * Store grid as a Sun rasterfile.
 * Try viewing it with xv.
 */
	rasterfile.ras_magic = 0x59a66a95;
	rasterfile.ras_width = grid_dim[0];
	rasterfile.ras_height = grid_dim[1];
	rasterfile.ras_depth = 8;
	rasterfile.ras_length = grid_dim[0] * grid_dim[1];
	rasterfile.ras_type = 1;
	rasterfile.ras_maptype = 1;
	rasterfile.ras_maplength = 768;
/*
 * Oops -- this is not happy under little endian systems.  So check
 * for endianness and flip everything if necessary.
 */
	check_endian(&rasterfile);

	fd = open("mandel.out", O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0) {
	  printf("master: Not able to write to file mandle.out -- aborting\n");
	  MPI_Abort(MPI_COMM_WORLD, errno);
	}
	write(fd, (char *) &rasterfile, sizeof(rasterfile));
	color_map(fd);
	write(fd, grid, grid_dim[0] * grid_dim[1]);
	close(fd);

	printf("master: done.\n");
	MPI_Finalize();

	return 0;
}

/*
 *	harvest
 *
 *	Function:	- receives results from one slave
 * Returns: - slave's rank */
static int
harvest(void)
{
	int		len0, len1;		/* x,y dim region length */
	int		i;
	MPI_Status	status;			/* status returned by recv */
	int		source;			/* who sent last message */
	char		*pgrid;			/* ptr within grid buffer */
	char		*pregion;		/* ptr within region buffer */
	int		result_dim[4];		/* result dimensions */

	MPI_Recv(result_dim, 4, MPI_INT, MPI_ANY_SOURCE,
			MPI_ANY_TAG, MPI_COMM_WORLD, &status);

	source = status.MPI_SOURCE;

	MPI_Recv(region, region_dim[0] * region_dim[1], MPI_CHAR,
			source, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		
	len0 = result_dim[2] - result_dim[0] + 1;
	len1 = result_dim[3] - result_dim[1] + 1;
	pregion = region;
	pgrid = grid + (grid_dim[0] * result_dim[1] + result_dim[0]);

	for (i = 0; i < len1; ++i) {
		memcpy(pgrid, pregion, len0);
		pregion += len0;
		pgrid += grid_dim[0];
	}

	return(source);
}

/*
 *	seed
 *
 *	Function:	- sends out work request
 */
static void
seed(int rank, int x, int y)
{
	int		work[4];	/* network message desc. */

	work[0] = x;			/* pixel coord's */
	work[1] = y;
	work[2] = (x + region_dim[0] > grid_dim[0]) ?
			grid_dim[0] - 1 : x + region_dim[0] - 1;
	work[3] = (y + region_dim[1] > grid_dim[1]) ?
			grid_dim[1] - 1 : y + region_dim[1] - 1;

	printf("master: allocating block (%d, %d) - (%d, %d) to process %d\n",
			work[0], work[1], work[2], work[3], rank);

	MPI_Send(work, 4, MPI_INT, rank, WORKTAG, MPI_COMM_WORLD);
}

/*
 *	color_map
 *
 *	Function:	- generate and write a color map to a file
 *			- I don't know anything about graphics.
 *	Accepts:	- file descriptor
 */
static void
color_map(int fd)
{
	int		i;		/* color map index */
	int		r, g, b;	/* individual color map indices */
	unsigned char	red[256];	/* trivial color map */
	unsigned char	green[256];	/* trivial color map */
	unsigned char	blue[256];	/* trivial color map */

	i = 0;

	for (b = 0; b < 3; ++b) {

		for (g = 0; g < 7; ++g) {

			for (r = 0; r < 7; ++r) {
				red[i] = (r << 5) | 0x10;
				green[i] = (g << 5) | 0x10;
				blue[i] = (b << 6) | 0x20;
				++i;
			}
		}
	}

	write(fd, (char *) red, sizeof(red));
	write(fd, (char *) green, sizeof(green));
	write(fd, (char *) blue, sizeof(blue));
}


/*
 *	check_endian
 *
 *	Function:	- check if machine is little endian
 *			- swap all data if so
 *	Accepts:	- pointer to raterfile header
 *			- pinter to grid data
 *			- size of grid data
 */
static void
check_endian(struct sunras *rasterfile)
{
  int *ip, j, i = 1;
  char temp, *c = (char*) &i;

  /* Note: mttoli4 is an internal LAM function for converting the
     endian-ness of an array of int4's.  See "man ttol" */

  if (c[0] == 1)
    for (ip = (int*) rasterfile, i = 0; 
	 i < (sizeof(struct sunras) / sizeof(int)); i++, ip++) {
      c = (char*) ip;
      for (j = 0; j < sizeof(int) / 2; j++) {
	temp = c[j];
	c[j] = c[sizeof(int) - j - 1];
	c[sizeof(int) - j - 1] = temp;
      }
    }
}
