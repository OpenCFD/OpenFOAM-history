// -*- c++ -*-
//
// $Id: request_inln.h,v 1.3 2002/10/09 20:57:38 brbarret Exp $
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

//
// Point-to-Point Communication
//

inline void
_REAL_MPI_::Request::Wait(_REAL_MPI_::Status &status) 
{
  (void)MPI_Wait(&mpi_request, &status.mpi_status);
}

inline void
_REAL_MPI_::Request::Wait() 
{
  (void)MPI_Wait(&mpi_request, MPI_STATUS_IGNORE);
}

inline void
_REAL_MPI_::Request::Free() 
{
  (void)MPI_Request_free(&mpi_request);
}

inline bool
_REAL_MPI_::Request::Test(_REAL_MPI_::Status &status) 
{
  int t;
  (void)MPI_Test(&mpi_request, &t, &status.mpi_status);
  return (bool) t;
}

inline bool
_REAL_MPI_::Request::Test() 
{
  int t;
  (void)MPI_Test(&mpi_request, &t, MPI_STATUS_IGNORE);
  return (bool) t;
}

inline int
_REAL_MPI_::Request::Waitany(int count, _REAL_MPI_::Request array[],
			     _REAL_MPI_::Status& status)
{
  int index, i;
  MPI_Request* array_of_requests = new MPI_Request[count];
  for (i=0; i < count; i++)
    array_of_requests[i] = array[i];
  (void)MPI_Waitany(count, array_of_requests, &index, &status.mpi_status);
  for (i=0; i < count; i++)
    array[i] = array_of_requests[i];
  delete [] array_of_requests;
  return index;
}

inline int
_REAL_MPI_::Request::Waitany(int count, _REAL_MPI_::Request array[])
{
  int index, i;
  MPI_Request* array_of_requests = new MPI_Request[count];
  for (i=0; i < count; i++)
    array_of_requests[i] = array[i];
  (void)MPI_Waitany(count, array_of_requests, &index, MPI_STATUS_IGNORE);
  for (i=0; i < count; i++)
    array[i] = array_of_requests[i];
  delete [] array_of_requests;
  return index; //JGS, Waitany return value
}

inline bool
_REAL_MPI_::Request::Testany(int count, _REAL_MPI_::Request array[],
			     int& index, _REAL_MPI_::Status& status)
{
  int i, flag;
  MPI_Request* array_of_requests = new MPI_Request[count];
  for (i=0; i < count; i++)
    array_of_requests[i] = array[i];
  (void)MPI_Testany(count, array_of_requests, &index, &flag, &status.mpi_status);
  for (i=0; i < count; i++)
    array[i] = array_of_requests[i];
  delete [] array_of_requests;
  return (bool)flag;
}

inline bool
_REAL_MPI_::Request::Testany(int count, _REAL_MPI_::Request array[], int& index)
{
  int i, flag;
  MPI_Request* array_of_requests = new MPI_Request[count];
  for (i=0; i < count; i++)
    array_of_requests[i] = array[i];
  (void)MPI_Testany(count, array_of_requests, &index, &flag, 
		    MPI_STATUS_IGNORE);
  for (i=0; i < count; i++)
    array[i] = array_of_requests[i];
  delete [] array_of_requests;
  return (bool)flag;
}

inline void
_REAL_MPI_::Request::Waitall(int count, _REAL_MPI_::Request req_array[],
			     _REAL_MPI_::Status stat_array[])
{
  int i;
  MPI_Request* array_of_requests = new MPI_Request[count];
  MPI_Status* array_of_statuses = new MPI_Status[count];
  for (i=0; i < count; i++)
    array_of_requests[i] = req_array[i];
  (void)MPI_Waitall(count, array_of_requests, array_of_statuses);
  for (i=0; i < count; i++)
    req_array[i] = array_of_requests[i];
  for (i=0; i < count; i++)
    stat_array[i] = array_of_statuses[i];
  delete [] array_of_requests;
  delete [] array_of_statuses;
}

inline void
_REAL_MPI_::Request::Waitall(int count, _REAL_MPI_::Request req_array[])
{
  int i;
  MPI_Request* array_of_requests = new MPI_Request[count];

  for (i=0; i < count; i++)
    array_of_requests[i] = req_array[i];
  (void)MPI_Waitall(count, array_of_requests, MPI_STATUSES_IGNORE);

  for (i=0; i < count; i++)
    req_array[i] = array_of_requests[i];

  delete [] array_of_requests;
} 

inline bool
_REAL_MPI_::Request::Testall(int count, _REAL_MPI_::Request req_array[],
			     _REAL_MPI_::Status stat_array[])
{
  int i, flag;
  MPI_Request* array_of_requests = new MPI_Request[count];
  MPI_Status* array_of_statuses = new MPI_Status[count];
  for (i=0; i < count; i++)
    array_of_requests[i] = req_array[i];
  (void)MPI_Testall(count, array_of_requests, &flag, array_of_statuses);
  for (i=0; i < count; i++)
    req_array[i] = array_of_requests[i];
  for (i=0; i < count; i++)
    stat_array[i] = array_of_statuses[i];
  delete [] array_of_requests;
  delete [] array_of_statuses;
  return (bool) flag;
}

inline bool
_REAL_MPI_::Request::Testall(int count, _REAL_MPI_::Request req_array[])
{
  int i, flag;
  MPI_Request* array_of_requests = new MPI_Request[count];

  for (i=0; i < count; i++)
    array_of_requests[i] = req_array[i];
  (void)MPI_Testall(count, array_of_requests, &flag, MPI_STATUSES_IGNORE);

  for (i=0; i < count; i++)
    req_array[i] = array_of_requests[i];
  delete [] array_of_requests;

  return (bool) flag;
} 

inline int
_REAL_MPI_::Request::Waitsome(int incount, _REAL_MPI_::Request req_array[],
			      int array_of_indices[], _REAL_MPI_::Status stat_array[]) 
{
  int i, outcount;
  MPI_Request* array_of_requests = new MPI_Request[incount];
  MPI_Status* array_of_statuses = new MPI_Status[incount];
  for (i=0; i < incount; i++)
    array_of_requests[i] = req_array[i];
  (void)MPI_Waitsome(incount, array_of_requests, &outcount,
		     array_of_indices, array_of_statuses);
  for (i=0; i < incount; i++)
    req_array[i] = array_of_requests[i];
  for (i=0; i < incount; i++)
    stat_array[i] = array_of_statuses[i];
  delete [] array_of_requests;
  delete [] array_of_statuses;
  return outcount;
}

inline int
_REAL_MPI_::Request::Waitsome(int incount, _REAL_MPI_::Request req_array[],
			      int array_of_indices[]) 
{
  int i, outcount;
  MPI_Request* array_of_requests = new MPI_Request[incount];

  for (i=0; i < incount; i++)
    array_of_requests[i] = req_array[i];
  (void)MPI_Waitsome(incount, array_of_requests, &outcount,
		     array_of_indices, MPI_STATUSES_IGNORE);

  for (i=0; i < incount; i++)
    req_array[i] = array_of_requests[i];
  delete [] array_of_requests;

  return outcount;
}

inline int
_REAL_MPI_::Request::Testsome(int incount, _REAL_MPI_::Request req_array[],
			      int array_of_indices[], _REAL_MPI_::Status stat_array[]) 
{
  int i, outcount;
  MPI_Request* array_of_requests = new MPI_Request[incount];
  MPI_Status* array_of_statuses = new MPI_Status[incount];
  for (i=0; i < incount; i++)
    array_of_requests[i] = req_array[i];
  (void)MPI_Testsome(incount, array_of_requests, &outcount,
		     array_of_indices, array_of_statuses);
  for (i=0; i < incount; i++)
    req_array[i] = array_of_requests[i];
  for (i=0; i < incount; i++)
    stat_array[i] = array_of_statuses[i];
  delete [] array_of_requests;
  delete [] array_of_statuses;
  return outcount;
}

inline int
_REAL_MPI_::Request::Testsome(int incount, _REAL_MPI_::Request req_array[],
			      int array_of_indices[]) 
{
  int i, outcount;
  MPI_Request* array_of_requests = new MPI_Request[incount];

  for (i=0; i < incount; i++)
    array_of_requests[i] = req_array[i];
  (void)MPI_Testsome(incount, array_of_requests, &outcount,
		     array_of_indices, MPI_STATUSES_IGNORE);

  for (i=0; i < incount; i++)
    req_array[i] = array_of_requests[i];
  delete [] array_of_requests;

  return outcount;
}

inline void
_REAL_MPI_::Request::Cancel(void) const
{
  (void)MPI_Cancel((MPI_Request*)&mpi_request);
}

inline void
_REAL_MPI_::Prequest::Start()
{
  (void)MPI_Start(&mpi_request);
}

inline void
_REAL_MPI_::Prequest::Startall(int count, _REAL_MPI_:: Prequest array_of_requests[])
{
  //convert the array of Prequests to an array of MPI_requests
  MPI_Request* mpi_requests = new MPI_Request[count];
  int i;
  for (i=0; i < count; i++) {
    mpi_requests[i] = array_of_requests[i];
  }
  (void)MPI_Startall(count, mpi_requests); 
  for (i=0; i < count; i++)
    array_of_requests[i].mpi_request = mpi_requests[i] ;
  delete [] mpi_requests;
} 

