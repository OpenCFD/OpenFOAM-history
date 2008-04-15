C
C Copyright (c) 2001-2002 The Trustees of Indiana University.  
C                         All rights reserved.
C Copyright (c) 1998-2001 University of Notre Dame. 
C                         All rights reserved.
C Copyright (c) 1994-1998 The Ohio State University.  
C                         All rights reserved.
C 
C This file is part of the LAM/MPI software package.  For license
C information, see the LICENSE file in the top level directory of the
C LAM/MPI source distribution.
C 
C $HEADER$
C 
C $Id: fring.f,v 1.4 2003/06/23 01:55:05 jsquyres Exp $
C
C Simple example of MPI showing the ring message pattern.
C

C
C     The general scheme of this program is to send a message around a
C     "ring" of N MPI processes.  Thank is, rank 0 sends to rank 1, rank
C     1 sends to rank 2, ..., rank (N-1) sends back to rank 0.  This
C     process is repeated M times.  The first integer of the message is
C     used as a counter.  The first time around the ring, it equals M.
C     When rank 0 receives the message, it decrements this first
C     integer.  When the counter reaches 0, each process sends the
C     message on to the next process in the ring and then exits.
C
      program ring_f77
      include 'mpif.h'
      integer message
      integer rank, size, tag, to, from, ierr
      integer status(mpi_status_size)

C     Start up MPI

      call mpi_init(ierr)
      call mpi_comm_rank(MPI_COMM_WORLD, rank, ierr)
      call mpi_comm_size(MPI_COMM_WORLD, size, ierr)

C     Arbitrarily choose 201 to be our tag.  Calculate the rank of the
C     to process in the ring.  Use the modulus operator so that the last
C     process "wraps around" to rank zero.

      message = 0
      tag = 201
      to = mod((rank + 1), size)
      from = mod((rank + size - 1), size)
      
C     If we are the "console" process, get a integer from the user to
C     specify how many times we want to go around the ring

      if (rank .eq. 0) then
         message = 5
         print *, 'Rank 0 starting message around the ring -- ',
     &        '1st of ', message
         call mpi_send(message, 1, MPI_INTEGER, to, tag, 
     &        MPI_COMM_WORLD, ierr)
      endif

C     Pass the message around the ring.  The exit mechanism works as
C     follows: the message (a positive integer) is passed around the
C     ring.  Each time is passes rank 0, it is decremented.  When each
C     processes receives the 0 message, it passes it on to the to
C     process and then quits.  By passing the 0 first, every process
C     gets the 0 message and can quit normally.

 10   call mpi_recv(message, 1, MPI_INTEGER, from, tag, 
     &     MPI_COMM_WORLD, status, ierr)
      print *, 'Rank ', rank, ' received message of "', message, 
     &     '" from rank ', from

      if (rank .eq. 0) then
         message = message - 1
      endif

      print *, 'Rank ', rank, ' sending message of "', message, 
     &     '" to rank ', to
      call mpi_send(message, 1, MPI_INTEGER, to, tag, 
     &     MPI_COMM_WORLD, ierr)
      
      if (message .eq. 0) then
         print *, 'Rank ', rank, ' all done; quitting loop'
         goto 20
      endif
      goto 10

C     Do the final receive 

 20   if (rank .eq. 0) then
         call mpi_recv(message, 1, MPI_INTEGER, from, tag, 
     &        MPI_COMM_WORLD, status, ierr)
         print *, 'Rank 0 consumed final message'
      endif

C     Quit

      call mpi_finalize(ierr)
      end
