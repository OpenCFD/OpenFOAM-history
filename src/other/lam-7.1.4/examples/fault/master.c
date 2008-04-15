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
 * $Id: master.c,v 6.12 2003/04/23 22:32:12 jsquyres Exp $
 *
 *	Function:	- master program of mandelbrot demo
 *			- fault tolerant version
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
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include <mpi.h>
#include "mandel.h"

#define min(a,b)	(((a) < (b)) ? (a) : (b))

/*
 * local structures
 */
struct sunras
{				/* Sun raster file header */
  int ras_magic;
  int ras_width;
  int ras_height;
  int ras_depth;
  int ras_length;
  int ras_type;
  int ras_maptype;
  int ras_maplength;
};

struct work
{				/* work order */
  int wrk_x_lb;
  int wrk_x_ub;
  int wrk_y_lb;
  int wrk_y_ub;
  struct work *wrk_next;
};

/*
 * local functions
 */
static int harvest(void);	/* gets results */
static void seed(int isslave);	/* distributes work */
static void color_map(int fd);	/* generate color map */
static void insert(struct work *work);	/* insert work into list */

/*
 * local variables
 */
static int grid_dimx;		/* grid size in pixels */
static int grid_dimy;		/* grid size in pixels */
static int region_dimx;		/* region size in pixels */
static int region_dimy;		/* region size in pixels */
static char *grid;		/* color buffer buffer */
static int ntasks;		/* # compute tasks */
static int nwork_out;		/* # work items out there */
static struct work *work_bottom;	/* bottom of work list */
static struct work *work_top;	/* top of work list */
static struct work **work_slaves;	/* slave's current work */
static MPI_Request *req_slaves;	/* receive reqs from slaves */
static MPI_Comm *icomm_slaves;	/* inter comm. to each slave */
static MPI_Datatype dtype_region;	/* place region into grid */


int
main(int argc, char *argv[])
{
  float grid_coord[4];		/* complex grid description */
  float init[4];		/* scale and translate data */
  int islave;			/* harvested slave index */
  int n, i;
  int x, y;			/* pixel coordinates */
  int fd;			/* output raster file */
  struct work *p;
  struct sunras rasterfile;
  int return_status = 0;	/* Return status for master */
  int err;
  MPI_Info appInfo;
  int flag;
  char filename[128];
  FILE *fp;
  void **attrp;

  /* Initialize MPI. */

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

  /* Check that we are the only master */

  if (ntasks != 1) {
    printf("Too many masters. Please launch only 1 copy of\n");
    printf("the master program. It will use MPI_Comm_spawn()\n");
    printf("to create the slaves.\n");
    MPI_Abort(MPI_COMM_WORLD, (1 << 16) + 1);
  }

  /* Get universe size */

  MPI_Attr_get(MPI_COMM_WORLD, MPI_UNIVERSE_SIZE, &attrp, &flag);
  if (flag == 1) {
    ntasks = (int) *attrp;
  } else {
    printf("This MPI does not support Universe Size, which is needed\n");
    printf("for this example. \n");
    MPI_Abort(MPI_COMM_WORLD, (1 << 16) + 1);
  }

  /* Allocate slave communicator arrays. */

  icomm_slaves = (MPI_Comm *)
    malloc((unsigned) (ntasks * sizeof(MPI_Comm)));
  if (icomm_slaves == NULL)
    MPI_Abort(MPI_COMM_WORLD, (errno << 16) + 1);

  /* Spawn off the slaves */

  sprintf(filename, "./lam_fault_appschema.%d", (int) getpid());
  MPI_Info_create(&appInfo);
  MPI_Info_set(appInfo, "file", filename);
  for (i = 0; i < ntasks; ++i) {

    /* Create a temporary app schema file */

    fp = fopen(filename, "w");
    if (fp == NULL) {
      printf("Could not open file %s\n", filename);
      MPI_Finalize();
      return 1;
    }
    fprintf(fp, "c%d ./slave\n", i);
    fclose(fp);

    /* Spawn */

    MPI_Comm_spawn(0, MPI_ARGV_NULL, 0,
		   appInfo, 0, MPI_COMM_SELF, &(icomm_slaves[i]),
		   &err);
    if (err != MPI_SUCCESS) {
      printf("Spawn Error %d\n", err);
      MPI_Abort(MPI_COMM_WORLD, (err << 16) + 1);
    } 
  }
  MPI_Info_free(&appInfo);
  unlink(filename);

  /* Hardcode the universe. */

  grid_coord[0] = -2;
  grid_coord[1] = 1.25;
  grid_coord[2] = 0.5;
  grid_coord[3] = -1.25;

  grid_dimx = 512;
  grid_dimy = 512;

  region_dimx = 16;
  region_dimy = 16;

  /* Allocate the raster file buffer. */

  grid = malloc((unsigned) (grid_dimx * grid_dimy));
  if (grid == NULL)
    MPI_Abort(MPI_COMM_WORLD, errno);

  /* Allocate slave receive request array. */

  req_slaves = (MPI_Request *)
    malloc((unsigned) (ntasks * sizeof(MPI_Request)));
  if (req_slaves == NULL)
    MPI_Abort(MPI_COMM_WORLD, (errno << 16) + 1);
  for (n = 0; n < ntasks; ++n)
    req_slaves[n] = MPI_REQUEST_NULL;

  /* Allocate slave work ptr array. */

  work_slaves = (struct work **)
    malloc((unsigned) (ntasks * sizeof(struct work *)));
  if (work_slaves == NULL)
    MPI_Abort(MPI_COMM_WORLD, (errno << 16) + 1);

  /* Create a datatype to receive a region into the grid.  To
   * handle partial regions at the edges of the grid, we need
   * three extra datatypes.  Right now we are lazy and the
   * region must evenly divide the grid.  */

  MPI_Type_vector(region_dimy, region_dimx, grid_dimx, MPI_CHAR,
		  &dtype_region);
  MPI_Type_commit(&dtype_region);

  /* Send translate and scale factors to each slave. */

  init[0] = (grid_coord[2] - grid_coord[0]) / grid_dimx;
  init[1] = -((grid_coord[1] - grid_coord[3]) / grid_dimy);
  init[2] = grid_coord[0];
  init[3] = grid_coord[1];

  for (n = 0; n < ntasks; ++n)
    MPI_Send((void *) init, 4, MPI_FLOAT, 0, WORKTAG, icomm_slaves[n]);

  /* set communicators error handlers */

  for (n = 0; n < ntasks; ++n)
    MPI_Errhandler_set(icomm_slaves[n], MPI_ERRORS_RETURN);
  MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN);

  /* Create the work list. */

  for (y = 0; y < grid_dimy; y += region_dimy) {
    for (x = 0; x < grid_dimx; x += region_dimx) {
      p = (struct work *)
	malloc((unsigned) sizeof(struct work));
      if (p == 0)
	MPI_Abort(MPI_COMM_WORLD, errno);

      p->wrk_x_lb = x;
      p->wrk_x_ub = min(x + region_dimx, grid_dimx) - 1;
      p->wrk_y_lb = y;
      p->wrk_y_ub = min(y + region_dimy, grid_dimy) - 1;
      p->wrk_next = 0;

      insert(p);
    }
  }

  nwork_out = 0;

  /* Distribute work to slaves until all the answers have been
   * received or their are no more living slaves. */

  while (work_top) {

    /* Distribute a work order to all living slaves. */

    for (n = 0; work_top && (n < ntasks); ++n) {
      if (icomm_slaves[n] == MPI_COMM_NULL)
	continue;
      seed(n);
    }

    if (nwork_out == 0) {
      printf("master: all slaves are dead\n");
      MPI_Abort(MPI_COMM_WORLD, 1);
    }

    /* Collect results and issue new work orders until there are
     * no outstanding work orders. */

    do {
      islave = harvest();

      if (islave < 0)		       /* a node has died */
	++return_status;

      if ((islave >= 0) && work_top)
	seed(islave);
    } while (nwork_out > 0);

    /* After letting living slaves idle due to lack of work, new
     * work orders may appear from slaves that die later.  So
     * start this whole process over with the remaining work
     * orders and living slaves. */
  }

  /* We have all the answers now, so kill the living workers. */

  for (n = 0; n < ntasks; ++n) {
    if (icomm_slaves[n] == MPI_COMM_NULL)
      continue;

    err = MPI_Send((void *) 0, 0, MPI_INT, 0, DIETAG, icomm_slaves[n]);
    if (err != MPI_SUCCESS)
      MPI_Comm_free(&icomm_slaves[n]);
  }

  /* Store grid as a Sun rasterfile. Try viewing it with xv. */

  rasterfile.ras_magic = 0x59a66a95;
  rasterfile.ras_width = grid_dimx;
  rasterfile.ras_height = grid_dimy;
  rasterfile.ras_depth = 8;
  rasterfile.ras_length = grid_dimx * grid_dimy;
  rasterfile.ras_type = 1;
  rasterfile.ras_maptype = 1;
  rasterfile.ras_maplength = 768;

  fd = open("mandel.out", O_CREAT | O_TRUNC | O_WRONLY, 0644);
  if (fd < 0) {
    printf("master: Not able to write to file mandle.out -- aborting\n");
    MPI_Abort(MPI_COMM_WORLD, errno);
  }

  write(fd, (char *) &rasterfile, sizeof(rasterfile));
  color_map(fd);
  write(fd, grid, grid_dimx * grid_dimy);
  close(fd);

  /* If any one of the slaves died, abort on just myself
   * (MPI_COMM_SELF).  If we abort on MPI_COMM_WORLD, LAM will
   * try to kill all processes (one or more of which is now
   * already dead and possibly unresponsive, so LAM would hang.
   * Hence, just abort on MPI_COMM_SELF).  See the MPI_Abort(3)
   * man page for an explanation of the error code. */

  if (return_status != 0)
    MPI_Abort(MPI_COMM_SELF, (return_status << 16) + 1);

  MPI_Finalize();
  return (0);
}


/*
 *	harvest
 *
 *	Function:	- receives results from one slave
 *      Returns:        - slave's index or -1
 */
static int
harvest(void)
{
  int err;
  int idone;

  nwork_out--;
  err = MPI_Waitany(ntasks, req_slaves, &idone, MPI_STATUS_IGNORE);

  if (err != MPI_SUCCESS) {
    if (idone >= 0) {
      req_slaves[idone] = MPI_REQUEST_NULL;
      MPI_Comm_free(&icomm_slaves[idone]);
      insert(work_slaves[idone]);
    }
    return (-1);
  }

  return (idone);
}


/*
 *	seed
 *
 *	Function:	- sends out work request
 */
static void
seed(int islave)
{
  char *pgrid;			/* ptr within grid buffer */
  int err;

  printf("master: allocating block (%d, %d) - (%d, %d) to process %d\n",
	 work_top->wrk_x_lb, work_top->wrk_y_lb,
	 work_top->wrk_x_ub, work_top->wrk_y_ub, islave);

  err = MPI_Send((void *) work_top, 4, MPI_INT, 0, WORKTAG,
		 icomm_slaves[islave]);

  if (err != MPI_SUCCESS) {
    MPI_Comm_free(&icomm_slaves[islave]);
    return;
  }

  ++nwork_out;
  pgrid = grid + (grid_dimx * work_top->wrk_y_lb + work_top->wrk_x_lb);

  work_slaves[islave] = work_top;
  work_top = work_top->wrk_next;

  err = MPI_Irecv(pgrid, 1, dtype_region, 0, REPLYTAG, icomm_slaves[islave],
		  &req_slaves[islave]);

  if (err != MPI_SUCCESS) {
    req_slaves[islave] = MPI_REQUEST_NULL;
    MPI_Comm_free(&icomm_slaves[islave]);
    insert(work_slaves[islave]);
    --nwork_out;
  }
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
  int i;			/* color map index */
  int r, g, b;			/* individual color map indices */
  unsigned char red[256];	/* trivial color map */
  unsigned char green[256];	/* trivial color map */
  unsigned char blue[256];	/* trivial color map */

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
 *	insert
 *
 *	Function:	- inserts work order into a list
 */
static void
insert(struct work *pwrk)
{
  if (work_top == 0) {
    work_top = pwrk;
  }

  if (work_bottom != 0) {
    work_bottom->wrk_next = pwrk;
  }

  work_bottom = pwrk;
  pwrk->wrk_next = 0;
}
