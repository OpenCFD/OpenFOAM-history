// -*- c++ -*-
//
// $Id: prequest.h,v 1.3 2002/10/09 20:57:38 brbarret Exp $
//
// Copyright (c) 2001-2002 The Trustees of Indiana University.  
//                         All rights reserved.
// Copyright (c) 1998-2001 University of Notre Dame. 
//                         All rights reserved.
// Copyright (c) 1994-1998 The Ohio State University.  
//                         All rights reserved.
// 
// This file is part of the LAM/MPI software package.  For license
// information, see the LICENSE file in the top level directory of the
// LAM/MPI source distribution.
// 
// $HEADER$
//


class Request {
public:
  // construction / destruction
  Request(void) { }
  virtual ~Request() {}
  Request(const MPI_Request &i) : mpi_request(i) { }

  // copy / assignment
  Request(const Request& r) : mpi_request(r.mpi_request) { }

  Request& operator=(const Request& r) {
    mpi_request = r.mpi_request;
    return *this;
  }

  // comparison
  bool operator== (const Request &a)
  { return (bool)(mpi_request == a.mpi_request); }
  bool operator!= (const Request &a) 
  { return (bool)!(*this == a); }

  // inter-language operability
  Request& operator= (const MPI_Request &i) {
    mpi_request = i; return *this; }
  operator MPI_Request () const { return mpi_request; }
  operator MPI_Request* () const { return (MPI_Request*)&mpi_request; }

  //
  // Point-to-Point Communication
  //

  virtual void Wait(Status &status);

  virtual void Wait();

  virtual bool Test(Status &status);

  virtual bool Test();

  virtual void Free(void);

  static int Waitany(int count, Request array[], Status& status);

  static int Waitany(int count, Request array[]);

  static bool Testany(int count, Request array[], int& index, Status& status);

  static bool Testany(int count, Request array[], int& index);

  static void Waitall(int count, Request req_array[], Status stat_array[]);
 
  static void Waitall(int count, Request req_array[]);

  static bool Testall(int count, Request req_array[], Status stat_array[]);
 
  static bool Testall(int count, Request req_array[]);

  static int Waitsome(int incount, Request req_array[],
			     int array_of_indices[], Status stat_array[]);

  static int Waitsome(int incount, Request req_array[],
			     int array_of_indices[]);

  static int Testsome(int incount, Request req_array[],
			     int array_of_indices[], Status stat_array[]);

  static int Testsome(int incount, Request req_array[],
			     int array_of_indices[]);

  virtual void Cancel(void) const;

protected:
  MPI_Request mpi_request;
};


class Prequest : public Request {
public:

  Prequest() { }

  Prequest(const Prequest& p) : Request(p) { }
  
  Prequest(const MPI_Request &i) : Request(i) { }

  virtual ~Prequest() { }

  virtual void Start();

  static void Startall(int count, Prequest array_of_requests[]);

};
