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
c $Id: fpi.f,v 1.4 2003/03/25 20:13:27 jsquyres Exp $
c
c Portions taken from the MPICH distribution example fpi.f.
c
c Example program to calculate the value of pi by integrating f(x) =
c 4 / (1 + x^2).
c
      program main

      include 'mpif.h'

      double precision  PI25DT
      parameter        (PI25DT = 3.141592653589793238462643d0)

      double precision  mypi, pi, h, sum, x, f, a
      integer num_iters, rank, size, i, rc

c     Function to integrate

      f(a) = 4.d0 / (1.d0 + a * a)

c     Normal MPI startup

      call MPI_INIT(ierr)
      call MPI_COMM_RANK(MPI_COMM_WORLD, rank, ierr)
      call MPI_COMM_SIZE(MPI_COMM_WORLD, size, ierr)
      print *, "Process ", rank, " of ", size, " is alive"

c     Loop until finished

      num_iters = 1000
      do 20 iter = 2, num_iters

c     Calculate the interval size

      h = 1.0d0 / iter
      sum  = 0.0d0

      do 10 i = rank + 1, iter, size
         x = h * (dble(i) - 0.5d0)
         sum = sum + f(x)
 10   continue
      mypi = h * sum

c     Collect all the partial sums

      call MPI_REDUCE(mypi, pi, 1, MPI_DOUBLE_PRECISION, MPI_SUM, 0,
     $     MPI_COMM_WORLD, ierr)

c     Node 0 prints the answer.

      if (rank .eq. 0) then
         write(6, 97) iter, pi, abs(pi - PI25DT)
 97      format(i3, ' points: pi is approximately: ', F18.16,
     +          ' error is: ', F18.16)
      endif
 20   continue

c     All finished

      call MPI_FINALIZE(rc)
      stop
      end




