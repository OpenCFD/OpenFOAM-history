// -*- c++ -*-
//
// $Id: request.h,v 1.5 2003/06/02 03:05:12 jsquyres Exp $
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
#if LAM_WANT_PROFILE
  //    friend class PMPI::Request;
#endif
public:
#if LAM_WANT_PROFILE

  // construction
  Request() { }
  Request(const MPI_Request &i) : pmpi_request(i) { }

  // copy / assignment
  Request(const Request& r) : pmpi_request(r.pmpi_request) { }

  Request(const PMPI::Request& r) : pmpi_request(r) { }

  virtual ~Request() {}

  Request& operator=(const Request& r) {
    pmpi_request = r.pmpi_request; return *this; }

  // comparison
  bool operator== (const Request &a) 
  { return (bool)(pmpi_request == a.pmpi_request); }
  bool operator!= (const Request &a) 
  { return (bool)!(*this == a); }

  // inter-language operability
  Request& operator= (const MPI_Request &i) {
    pmpi_request = i; return *this;  }

  operator MPI_Request () const { return pmpi_request; }
  //  operator MPI_Request* () const { return pmpi_request; }
  operator const PMPI::Request&() const { return pmpi_request; }

#else

  // construction / destruction
  Request() { mpi_request = MPI_REQUEST_NULL; }
  virtual ~Request() {}
  Request(const MPI_Request &i) : mpi_request(i) { }

  // copy / assignment
  Request(const Request& r) : mpi_request(r.mpi_request) { }

  Request& operator=(const Request& r) {
    mpi_request = r.mpi_request; return *this; }

  // comparison
  bool operator== (const Request &a) 
  { return (bool)(mpi_request == a.mpi_request); }
  bool operator!= (const Request &a) 
  { return (bool)!(*this == a); }

  // inter-language operability
  Request& operator= (const MPI_Request &i) {
    mpi_request = i; return *this; }
  operator MPI_Request () const { return mpi_request; }
  //  operator MPI_Request* () const { return (MPI_Request*)&mpi_request; }

#endif

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
			     int array_of_indices[], Status stat_array[]) ;

  static int Waitsome(int incount, Request req_array[],
			     int array_of_indices[]);

  static int Testsome(int incount, Request req_array[],
			     int array_of_indices[], Status stat_array[]);

  static int Testsome(int incount, Request req_array[],
			     int array_of_indices[]);

  virtual void Cancel(void) const;


protected:
#if ! LAM_WANT_PROFILE
  MPI_Request mpi_request;
#endif

private:

#if LAM_WANT_PROFILE
  PMPI::Request pmpi_request;
#endif 

};


class Prequest : public Request {
#if LAM_WANT_PROFILE
  //  friend class PMPI::Prequest;
#endif
public:

  Prequest() { }

#if LAM_WANT_PROFILE
  Prequest(const Request& p) : Request(p), pmpi_request(p) { }

  Prequest(const PMPI::Prequest& r) : 
    Request((const PMPI::Request&)r),
    pmpi_request(r) { }

  Prequest(const MPI_Request &i) : Request(i), pmpi_request(i) { }
  
  virtual ~Prequest() { }

  Prequest& operator=(const Request& r) {
    Request::operator=(r);
    pmpi_request = (PMPI::Prequest)r; return *this; }

  Prequest& operator=(const Prequest& r) {
    Request::operator=(r);
    pmpi_request = r.pmpi_request; return *this; }
#else
  Prequest(const Request& p) : Request(p) { }

  Prequest(const MPI_Request &i) : Request(i) { }

  virtual ~Prequest() { }

  Prequest& operator=(const Request& r) {
    mpi_request = r; return *this; }

  Prequest& operator=(const Prequest& r) {
    mpi_request = r.mpi_request; return *this; }
#endif

  virtual void Start();

  static void Startall(int count, Prequest array_of_requests[]);

#if LAM_WANT_PROFILE
private:
  PMPI::Prequest pmpi_request;
#endif 
};
