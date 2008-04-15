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
 *	$Id: simple.c,v 1.8 2002/11/23 04:06:58 jsquyres Exp $
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>

#define SIZE (65536)

/* Each process writes to separate files and reads them back. 
   The file name is taken as a command-line argument, and the process rank 
   is appended to it. */ 

int main(int argc, char **argv)
{
    int *buf, i, rank, nints, len;
    char *filename, *tmp;
    MPI_File fh;
    MPI_Status status;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

/* process 0 takes the file name as a command-line argument and 
   broadcasts it to other processes */
    if (!rank) {
	i = 1;
	while ((i < argc) && strcmp("-fname", *argv)) {
	    i++;
	    argv++;
	}
	if (i >= argc) {
	    printf("\n*#  Usage: simple -fname filename\n\n");
	    MPI_Abort(MPI_COMM_WORLD, 1);
	}
	argv++;
	len = strlen(*argv);
	filename = (char *) malloc(len+10);
	strcpy(filename, *argv);
	MPI_Bcast(&len, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(filename, len+10, MPI_CHAR, 0, MPI_COMM_WORLD);
    }
    else {
	MPI_Bcast(&len, 1, MPI_INT, 0, MPI_COMM_WORLD);
	filename = (char *) malloc(len+10);
	MPI_Bcast(filename, len+10, MPI_CHAR, 0, MPI_COMM_WORLD);
    }
    

    buf = (int *) malloc(SIZE);
    nints = SIZE/sizeof(int);
    for (i=0; i<nints; i++) buf[i] = rank*100000 + i;

    /* each process opens a separate file called filename.'myrank' */
    tmp = (char *) malloc(len+10);
    strcpy(tmp, filename);
    sprintf(filename, "%s.%d", tmp, rank);

    MPI_File_open(MPI_COMM_SELF, filename, MPI_MODE_CREATE | MPI_MODE_RDWR,
		   MPI_INFO_NULL, &fh);
    MPI_File_set_view(fh, 0, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
    MPI_File_write(fh, buf, nints, MPI_INT, &status);
    MPI_File_close(&fh);

    /* reopen the file and read the data back */

    for (i=0; i<nints; i++) buf[i] = 0;
    MPI_File_open(MPI_COMM_SELF, filename, MPI_MODE_CREATE | MPI_MODE_RDWR, 
                  MPI_INFO_NULL, &fh);
    MPI_File_set_view(fh, 0, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
    MPI_File_read(fh, buf, nints, MPI_INT, &status);
    MPI_File_close(&fh);

    /* check if the data read is correct */
    for (i=0; i<nints; i++) 
	if (buf[i] != (rank*100000 + i)) {
	    printf("Process %d: error, read %d, should be %d\n", rank, buf[i], rank*100000+i);
	}

    if (!rank) printf("Done\n");

    free(buf);
    free(filename);
    free(tmp);

    MPI_Finalize();
    return 0; 
}
