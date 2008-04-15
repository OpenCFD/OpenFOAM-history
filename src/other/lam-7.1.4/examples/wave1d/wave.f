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
c $Id: wave.f,v 6.6 2003/03/28 22:14:17 jsquyres Exp $
c
c       Function:       - simulates vibrating string
c			- simple parallel version
c			- uses MPI library for communication
c			- nodes are numbered consecutively from 0
c			- same program runs on all nodes
c			- allows non-integral decomposition
c       Reference:      - Solving Problems on Concurrent Processors - Vol I
c                         Fox, Johnson et al
c
      program wave

      include 'mpif.h'

      integer*4 MAXSIZE
      real*8 PI

      parameter (MAXSIZE = 1000)
      parameter (PI = 3.14159265)

      integer*4 ierror
      integer*4 myid
      integer*4 mypts
      integer*4 nnodes
      integer*4 npts
      integer*4 nstep
      integer*4 residual

      real*8 c
      real*8 delt
      real*8 delx
      real*8 fac
      real*8 newval(0:MAXSIZE + 1)
      real*8 oldval(0:MAXSIZE + 1)
      real*8 pfac
      real*8 values(0:MAXSIZE + 1)
      real*8 x

      common /valcom/ values, oldval, newval, npts, mypts
      common /topo/ nnodes, myid

c
c Initialize MPI.
c
      call MPI_INIT(ierror)

      call MPI_COMM_RANK(MPI_COMM_WORLD, myid, ierror)
      call MPI_COMM_SIZE(MPI_COMM_WORLD, nnodes, ierror)
c
c Get scalar parameters.
c
      call gparam(npts, delt, nstep)
      mypts = npts / nnodes
      residual = MOD(npts, nnodes)

      if (mypts .gt. MAXSIZE) then
         print *, 'wave: Use < 1000 points on each node.'
         call MPI_FINALIZE(ierror)
         stop
      endif

      if (residual .gt. myid) then
         mypts = mypts + 1
      endif
c
c Set initial values along the string.
c Determine the actual starting value of k for my node.
c
      fac = 2.0 * pi
      k = myid * (npts / nnodes)

      if (residual .gt. myid) then
         k = k + myid
      else
         k = k + residual
      endif

      do 10 j = 1, mypts
         x = dble(k) / dble(npts - 1)
         values(j) = dsin(fac * x)
         oldval(j) = values(j)
         k = k + 1
 10   continue
c
c I'm not sure what c and delx could be used for and I'm too
c lazy to find out.
c 
      c = 1.0
      delx = 1.0
      pfac = (c * delt / delx) ** 2
 
      do 20 i = 1, nstep
         call exchange()
 
         do 30 j = 1, mypts

            if ((j .eq. 1) .and. (myid .eq. 0)) then
               newval(j) = 0.0
            elseif ((j .eq. mypts) .and. (myid .eq. (nnodes - 1)))
     +              then
               newval(j) = 0.0
            else
               newval(j) = 2.0 * values(j) - oldval(j)
     +              + (values(j + 1) - 2.0 * values(j) +
     +              values(j - 1)) * pfac
            endif

 30      continue
c
c t - 1 is t, t is t + 1, ready for next cycle
c
         do 40 j = 1, mypts
            oldval(j) = values(j)
            values(j) = newval(j)
 40      continue
 
 20   continue
c
c Output final results.
c
      call outres
c
c Exit.
c
      call MPI_FINALIZE(ierror)
      stop
      end

c
c	gparam
c
c	Function:	- reads scalar parameters from file
c	Accepts:	- # simulation points (value returned)
c			- time step size (value returned)
c			- # simulation steps (value returned)
c
      subroutine gparam(npts, delt, nstep)

      integer npts
      integer nstep
      real*8 delt

c	print *, 'Enter number of simulation points.'
c	read(*, *) npts
c	print *, 'Enter size of time step.'
c	read(*, *) delt
c	write(*, *) 'Enter number of simulation steps.'
c	read(*, *) nstep

      npts = 101
      delt = 0.5
      nstep = 100

      return
      end

c
c	outres
c
c	Function:	- writes current amplitude points to file
c
      subroutine outres()

      parameter (MAXSIZE = 1000)
 
      integer myid
      integer mypts
      integer nnodes
      integer npts
      real*8 values(0:MAXSIZE + 1)
      real*8 oldval(0:MAXSIZE + 1)
      real*8 newval(0:MAXSIZE + 1)
      character filename*10
      character fileno*2

      common /valcom/ values, oldval, newval, npts, mypts
      common /topo/ nnodes, myid

      write(fileno, '(i2.2)') myid
      filename = 'wave.out' // fileno
      open(unit = 8, file = filename, iostat = ierrno, err = 9)
 
      write(8, 5) (values(j), j = 1, mypts)
 5    format(f8.4)
 
      close(8)
      return

 9    write(6, 15) ierrno
 15   format('wave: could not open file wave.out, error code = ', i12)
      stop
      end

c
c	exchange
c
c	Function:	- exchanges internal boundary points with neighbors
c
      subroutine exchange()
      include 'mpif.h'

      parameter (MAXSIZE = 1000)
 
      integer mypts
      integer npts
      real*8 values(0:MAXSIZE + 1)
      real*8 oldval(0:MAXSIZE + 1)
      real*8 newval(0:MAXSIZE + 1)

      common /valcom/ values, oldval, newval, npts, mypts

      integer myid
      integer nnodes

      common /topo/ nnodes, myid

      integer status(MPI_STATUS_SIZE)

c
c Odd nodes exchange left and then right.
c Even nodes exchange right and then left.
c
      if (MOD(myid, 2) .eq. 0) then

         if (myid .ne. (nnodes - 1)) then
c
c Send/receive to/from my right.
c
            call MPI_SEND(values(mypts), 1,
     +           MPI_DOUBLE_PRECISION, myid + 1, 0,
     +           MPI_COMM_WORLD, ierror)

            call MPI_RECV(values(mypts + 1), 1,
     +           MPI_DOUBLE_PRECISION, myid + 1, 0,
     +           MPI_COMM_WORLD, status, ierror)      
         endif

         if (myid .ne. 0) then
c
c Receive/send from/to my left.
c
            call MPI_RECV(values(0), 1,
     +           MPI_DOUBLE_PRECISION, myid - 1, 0,
     +           MPI_COMM_WORLD, status, ierror)

            call MPI_SEND(values(1), 1,
     +           MPI_DOUBLE_PRECISION, myid - 1, 0,
     +           MPI_COMM_WORLD, ierror)
         endif
      else
         if (myid .ne. 0) then
c
c Receive/send from/to my left.
c
            call MPI_RECV(values(0), 1,
     +           MPI_DOUBLE_PRECISION, myid - 1, 0,
     +           MPI_COMM_WORLD, status, ierror)

            call MPI_SEND(values(1), 1,
     +           MPI_DOUBLE_PRECISION, myid - 1, 0,
     +           MPI_COMM_WORLD, ierror)
         endif

         if (myid .ne. (nnodes - 1)) then
c
c Send/receive to/from my right.
c
            call MPI_SEND(values(mypts), 1,
     +           MPI_DOUBLE_PRECISION, myid + 1, 0,
     +           MPI_COMM_WORLD, ierror)

            call MPI_RECV(values(mypts + 1), 1,
     +           MPI_DOUBLE_PRECISION, myid + 1, 0,
     +           MPI_COMM_WORLD, status, ierror)
         endif
      endif
      end
