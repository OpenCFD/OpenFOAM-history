// -*- c++ -*-
//
// $Id: win.cc,v 1.4 2003/01/20 03:41:21 vsahay Exp $
//
// Copyright (c) 2001-2003 The Trustees of Indiana University.  
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
#include <iostream>


//
// Miscellany
//

MPI::Errhandler
MPI::Win::Create_errhandler(MPI::Win::Errhandler_fn* function)
{
  return PMPI::Win::Create_errhandler((PMPI::Win::Errhandler_fn *)function);
}	

	
MPI::Errhandler
MPI::Win::Get_errhandler() const 
{
  return pmpi_win.Get_errhandler();
}

void
MPI::Win::Set_errhandler(const MPI::Errhandler& errhandler)
{
  pmpi_win.Set_errhandler(errhandler);
}


//
// One sided communication
//

void
MPI::Win::Accumulate(const void* origin_addr, int origin_count, 
		     const MPI::Datatype& origin_datatype, 
		     int target_rank, MPI::Aint target_disp, 
		     int target_count, const MPI::Datatype& target_datatype, 
		     const MPI::Op& op) const
{
  pmpi_win.Accumulate(origin_addr, origin_count, origin_datatype, 
		      target_rank, target_disp, target_count, 
		      target_datatype, op);
} 


void
MPI::Win::Complete() const 
{
  pmpi_win.Complete();
}


MPI::Win
MPI::Win::Create(const void* base, MPI::Aint size, int disp_unit, 
		 const MPI::Info& info, const MPI::Intracomm& comm) 
{
  return (MPI::Win) PMPI::Win::Create(base, size, 
				      disp_unit, info, 
				      comm.pmpi_comm);
}
	

void
MPI::Win::Fence(int assert) const
{
   pmpi_win.Fence(assert);
}


void
MPI::Win::Free()
{
  pmpi_win.Free();
}


void
MPI::Win::Get(const void *origin_addr, int origin_count, 
	      const MPI::Datatype& origin_datatype, int target_rank, 
	      MPI::Aint target_disp, int target_count, 
	      const MPI::Datatype& target_datatype) const 
{
  pmpi_win.Get(origin_addr, origin_count, origin_datatype, 
	       target_rank, target_disp, target_count, 
	       target_datatype);

}


MPI::Group
MPI::Win::Get_group() const
{
  return pmpi_win.Get_group();
}


#if LAM_WANT_MPI_STUBS  
// This function is not yet implemented in LAM/MPI.  So it should
// fail to compile if the user tries to use it.
void
MPI::Win::Lock(int lock_type, int rank, int assert) const 
{
  pmpi_win.Lock(lock_type, rank, assert);
}
#endif


void
MPI::Win::Post(const MPI::Group& group, int assert) const 
{
  pmpi_win.Post(group, assert);
}


void
MPI::Win::Put(const void* origin_addr, int origin_count, 
	      const MPI::Datatype& origin_datatype, int target_rank, 
	      MPI::Aint target_disp, int target_count, 
	      const MPI::Datatype& target_datatype) const 
{
  pmpi_win.Put(origin_addr, origin_count, origin_datatype, 
	       target_rank, target_disp, target_count, 
	       target_datatype);

}
  

void
MPI::Win::Start(const MPI::Group& group, int assert) const
{
  pmpi_win.Start( group, assert);
}


#if LAM_WANT_MPI_STUBS  
// This function is not yet implemented in LAM/MPI.  So it should
// fail to compile if the user tries to use it.
bool
MPI::Win::Test() const 
{
  return pmpi_win.Test();
}
#endif


#if LAM_WANT_MPI_STUBS  
// This function is not yet implemented in LAM/MPI.  So it should
// fail to compile if the user tries to use it.
void
MPI::Win::Unlock(int rank) const 
{
  pmpi_win.Unlock(rank);
}
#endif


void
MPI::Win::Wait() const
{
  pmpi_win.Wait();
}


//
// External Interfaces
//

#if LAM_WANT_MPI_STUBS  
// This function is not yet implemented in LAM/MPI.  So it should
// fail to compile if the user tries to use it.
void
MPI::Win::Call_errhandler(int errorcode) const 
{
  pmpi_win.Call_errhandler(errorcode);
}
#endif


int
MPI::Win::Create_keyval(MPI::Win::Copy_attr_function* win_copy_attr_fn, 
			MPI::Win::Delete_attr_function* win_delete_attr_fn, 
			void* extra_state) 
{
  return PMPI::Win::Create_keyval((PMPI::Win::Copy_attr_function *)
				  win_copy_attr_fn,
				  (PMPI::Win::Delete_attr_function *)
				  win_delete_attr_fn,  extra_state);
}


void
MPI::Win::Delete_attr(int win_keyval) 
{
  pmpi_win.Delete_attr(win_keyval);
}


void
MPI::Win::Free_keyval(int& win_keyval)
{
  PMPI::Win::Free_keyval(win_keyval);
}


bool
MPI::Win::Get_attr(const Win& win, int win_keyval, void* attribute_val) const
{
  return pmpi_win.Get_attr( win,  win_keyval, attribute_val);
}


void
MPI::Win::Get_name(char* win_name, int& resultlen) const
{
  pmpi_win.Get_name(win_name, resultlen);
}


void
MPI::Win::Set_attr(int win_keyval, const void* attribute_val) 
{
  pmpi_win.Set_attr(win_keyval, attribute_val);
}


void
MPI::Win::Set_name(const char* win_name) 
{
  pmpi_win.Set_name(win_name);
}
