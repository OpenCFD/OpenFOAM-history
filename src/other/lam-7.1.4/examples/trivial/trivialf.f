c
c Copyright (c) 2001-2003 The Trustees of Indiana University.  
c                         All rights reserved.
c Copyright (c) 1998-2001 University of Notre Dame. 
c                         All rights reserved.
c Copyright (c) 1994-1998 The Ohio State University.  
c                         All rights reserved.
c 
c This file is part of the LAM/MPI software package.  For license
c information, see the LICENSE file in the top level directory of the
c LAM/MPI source distribution.
c 
c $HEADER$
c
c $Id: trivialf.f,v 6.4 2003/03/28 22:14:17 jsquyres Exp $
c
c	Transmit a message in a two process system.
c
        program trivial

        include 'mpif.h'

        integer*4 BUFSIZE

        parameter (BUFSIZE = 64)

        integer*4 buffer(BUFSIZE)
        integer rank
        integer size
        integer status(MPI_STATUS_SIZE)

c
c Initialize MPI.
c
        call MPI_INIT(ierror)
c
c Error check the number of processes.
c Determine my rank in the world group.
c The sender will be rank 0 and the receiver, rank 1.
c
        call MPI_COMM_SIZE(MPI_COMM_WORLD, size, ierror)

        if (size .lt. 2) then
           print *,'Need at least 2 processes.'
           call MPI_FINALIZE(ierror)
           stop
        endif

        call MPI_COMM_RANK(MPI_COMM_WORLD, rank, ierror)
c
c As rank 0, send a message to rank 1.
c
        if (rank .eq. 0) then
           call MPI_SEND(buffer(1), BUFSIZE, MPI_INTEGER,
     +          1, 11, MPI_COMM_WORLD, ierror)
           print *,'rank ',rank,' sent message'
c
c As rank 1, receive a message from rank 0.
c
        else if (rank .eq. 1) then
           call MPI_RECV(buffer(1), BUFSIZE, MPI_INTEGER,
     +          0, 11, MPI_COMM_WORLD, status, ierror)      
           print *,'rank ',rank,' received message'
        endif

        call MPI_FINALIZE(ierror)
        stop
        end
