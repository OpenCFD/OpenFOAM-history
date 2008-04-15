// -*- c++ -*-
//
// $Id: request.cc,v 1.3 2002/10/09 20:58:57 brbarret Exp $
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

#include "mpicxx.h"

//
// Point-to-Point Communication
//

void MPI::Request::Wait(MPI::Status &status) 
{
  pmpi_request.Wait(status.pmpi_status);
}

void MPI::Request::Wait() 
{
  pmpi_request.Wait();
}

bool MPI::Request::Test(MPI::Status &status) 
{
  return pmpi_request.Test(status.pmpi_status);
}

bool MPI::Request::Test() 
{
  return pmpi_request.Test();
}

void MPI::Request::Free(void) 
{
  pmpi_request.Free();
}

int MPI::Request::Waitany(int count, MPI::Request array[],
			  MPI::Status& status)
{
  int ret, i;
  PMPI::Request* pmpi_array = new PMPI::Request[count];
  for (i=0; i < count; i++)
    pmpi_array[i] = array[i].pmpi_request;
  ret = PMPI::Request::Waitany(count, pmpi_array, status.pmpi_status);
  for (i=0; i < count; i++)
    array[i].pmpi_request = pmpi_array[i] ;
  delete [] pmpi_array;
  return ret;
}

int MPI::Request::Waitany(int count, MPI::Request array[])
{
  int ret, i;
  PMPI::Request* pmpi_array = new PMPI::Request[count];
  for (i=0; i < count; i++)
    pmpi_array[i] = array[i].pmpi_request;
  ret = PMPI::Request::Waitany(count, pmpi_array);
  for (i=0; i < count; i++)
    array[i].pmpi_request = pmpi_array[i] ;
  delete [] pmpi_array;
  return ret;
}

bool MPI::Request::Testany(int count, MPI::Request array[],
			   int& index, MPI::Status& status)
{
  bool ret;
  int i;
  PMPI::Request* pmpi_array = new PMPI::Request[count];
  for (i=0; i < count; i++)
    pmpi_array[i] = array[i].pmpi_request;
  ret = PMPI::Request::Testany(count, pmpi_array, index, status.pmpi_status);
  for (i=0; i < count; i++)
    array[i].pmpi_request = pmpi_array[i] ;
  delete [] pmpi_array;
  return ret;
}

bool MPI::Request::Testany(int count, MPI::Request array[], int& index)
{
  bool ret;
  int i;
  PMPI::Request* pmpi_array = new PMPI::Request[count];
  for (i=0; i < count; i++)
    pmpi_array[i] = array[i].pmpi_request;
  ret = PMPI::Request::Testany(count, pmpi_array, index);
  for (i=0; i < count; i++)
    array[i].pmpi_request = pmpi_array[i] ;
  delete [] pmpi_array;
  return ret;
}

void MPI::Request::Waitall(int count, MPI::Request array[],
			   MPI::Status stat_array[])
{
  PMPI::Request* pmpi_array = new PMPI::Request[count];
  int i;
  for (i=0; i < count; i++)
    pmpi_array[i] = array[i].pmpi_request;
  PMPI::Status* pmpi_status = new PMPI::Status[count];
  for (i=0; i < count; i++)
    pmpi_status[i] = stat_array[i].pmpi_status;
  PMPI::Request::Waitall(count, pmpi_array, pmpi_status);
  for (i=0; i < count; i++)
    array[i].pmpi_request = pmpi_array[i] ;
  for (i=0; i < count; i++)
    stat_array[i] = pmpi_status[i];
  delete [] pmpi_array;
  delete [] pmpi_status;
}
 
void MPI::Request::Waitall(int count, MPI::Request array[])
{
  int i;
  PMPI::Request* pmpi_array = new PMPI::Request[count];
  for (i=0; i < count; i++)
    pmpi_array[i] = array[i].pmpi_request;
  PMPI::Request::Waitall(count, pmpi_array);
  for (i=0; i < count; i++)
    array[i].pmpi_request = pmpi_array[i] ;
  delete [] pmpi_array;
} 

bool MPI::Request::Testall(int count, MPI::Request array[],
			   MPI::Status stat_array[])
{
  bool ret;
  int i;
  PMPI::Request* pmpi_array = new PMPI::Request[count];
  for (i=0; i < count; i++)
    pmpi_array[i] = array[i].pmpi_request;
  PMPI::Status* pmpi_status = new PMPI::Status[count];
  for (i=0; i < count; i++)
    pmpi_status[i] = stat_array[i].pmpi_status;
  ret = PMPI::Request::Testall(count, pmpi_array, pmpi_status);
  for (i=0; i < count; i++)
    array[i].pmpi_request = pmpi_array[i] ;
  for (i=0; i < count; i++)
    stat_array[i] = pmpi_status[i];
  delete [] pmpi_array;
  delete [] pmpi_status;
  return ret;
}
 
bool MPI::Request::Testall(int count, MPI::Request array[])
{
  bool ret;
  int i;
  PMPI::Request* pmpi_array = new PMPI::Request[count];
  for (i=0; i < count; i++)
    pmpi_array[i] = array[i].pmpi_request;
  ret = PMPI::Request::Testall(count, pmpi_array);
  for (i=0; i < count; i++)
    array[i].pmpi_request = pmpi_array[i] ;
  delete [] pmpi_array;
  return ret;
} 

int MPI::Request::Waitsome(int count, MPI::Request array[],
			   int array_of_indices[], MPI::Status stat_array[]) 
{
  int ret;
  int i;
  PMPI::Request* pmpi_array = new PMPI::Request[count];
  for (i=0; i < count; i++)
    pmpi_array[i] = array[i].pmpi_request;
  PMPI::Status* pmpi_status = new PMPI::Status[count];
  for (i=0; i < count; i++)
    pmpi_status[i] = stat_array[i].pmpi_status;
  ret = PMPI::Request::Waitsome(count, pmpi_array, array_of_indices, pmpi_status);
  for (i=0; i < count; i++)
    array[i].pmpi_request = pmpi_array[i] ;
  for (i=0; i < count; i++)
    stat_array[i] = pmpi_status[i];
  delete [] pmpi_array;
  delete [] pmpi_status;
  return ret;
}

int MPI::Request::Waitsome(int count, MPI::Request array[],
			   int array_of_indices[]) 
{
  int ret, i;
  PMPI::Request* pmpi_array = new PMPI::Request[count];
  for (i=0; i < count; i++)
    pmpi_array[i] = array[i].pmpi_request;
  ret = PMPI::Request::Waitsome(count, pmpi_array, array_of_indices);
  for (i=0; i < count; i++)
    array[i].pmpi_request = pmpi_array[i] ;
  delete [] pmpi_array;
  return ret;
}

int MPI::Request::Testsome(int count, MPI::Request array[],
			   int array_of_indices[], MPI::Status stat_array[]) 
{
  int ret;
  int i;
  PMPI::Request* pmpi_array = new PMPI::Request[count];
  for (i=0; i < count; i++)
    pmpi_array[i] = array[i].pmpi_request;
  PMPI::Status* pmpi_status = new PMPI::Status[count];
  for (i=0; i < count; i++)
    pmpi_status[i] = stat_array[i].pmpi_status;
  ret = PMPI::Request::Testsome(count, pmpi_array, array_of_indices, pmpi_status);
  for (i=0; i < count; i++)
    array[i].pmpi_request = pmpi_array[i] ;
  for (i=0; i < count; i++)
    stat_array[i] = pmpi_status[i];
  delete [] pmpi_array;
  delete [] pmpi_status;
  return ret;
}

int MPI::Request::Testsome(int count, MPI::Request array[],
			   int array_of_indices[]) 
{
  int ret, i;
  PMPI::Request* pmpi_array = new PMPI::Request[count];
  for (i=0; i < count; i++)
    pmpi_array[i] = array[i].pmpi_request;
  ret = PMPI::Request::Testsome(count, pmpi_array, array_of_indices);
  for (i=0; i < count; i++)
    array[i].pmpi_request = pmpi_array[i] ;
  delete [] pmpi_array;
  return ret;
}

void MPI::Request::Cancel(void) const
{
  pmpi_request.Cancel();
}

void MPI::Prequest::Start()
{
  pmpi_request.Start();
}

void MPI::Prequest::Startall(int count, MPI::Prequest array[])
{
  int i;
  PMPI::Prequest* pmpi_array = new PMPI::Prequest[count];
  for (i=0; i < count; i++)
    pmpi_array[i] = array[i].pmpi_request;
  PMPI::Prequest::Startall(count, pmpi_array);
  for (i=0; i < count; i++)
    array[i].pmpi_request = pmpi_array[i] ;
  delete [] pmpi_array;
}
