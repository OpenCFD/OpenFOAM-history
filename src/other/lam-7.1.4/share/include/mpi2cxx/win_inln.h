// -*- c++ -*-
//
// $Id: win_inln.h,v 1.6 2003/01/21 17:53:12 vsahay Exp $
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



//
// Miscellany
//


inline _REAL_MPI_::Errhandler 
_REAL_MPI_::Win::Create_errhandler(_REAL_MPI_::Win::Errhandler_fn* function)
{
  MPI_Errhandler errhandler;
  (void) MPI_Win_create_errhandler((MPI_Win_errhandler_fn *)function, 
				   &errhandler);
  return errhandler;
}

       
inline _REAL_MPI_::Errhandler 
_REAL_MPI_::Win:: Get_errhandler() const 
{
   MPI_Errhandler errhandler;
   (void) MPI_Win_get_errhandler(mpi_win, &errhandler);
   return errhandler;
}

 
inline void 
_REAL_MPI_::Win::Set_errhandler(const _REAL_MPI_::Errhandler& errhandler)
{
  (void) MPI_Win_set_errhandler(mpi_win, errhandler);
}


//
// One sided communication
//


inline void 
_REAL_MPI_::Win::Accumulate(const void* origin_addr, int origin_count,
	       	const _REAL_MPI_::Datatype& origin_datatype, int target_rank,
	       	_REAL_MPI_::Aint target_disp, int target_count,
	       	const _REAL_MPI_::Datatype& target_datatype,
	       	const _REAL_MPI_::Op& op) const 
{
  (void) MPI_Accumulate((void*) origin_addr, origin_count, origin_datatype,
			target_rank, target_disp, target_count, 
			target_datatype, op, mpi_win);
  
}


inline void
_REAL_MPI_::Win::Complete() const 
{
  (void) MPI_Win_complete(mpi_win);
}


inline  _REAL_MPI_::Win 
_REAL_MPI_::Win::Create(const void* base, _REAL_MPI_::Aint size, 
			int disp_unit, const _REAL_MPI_::Info& info, 
			const _REAL_MPI_::Intracomm& comm) 
{
  MPI_Win newwin;
  (void) MPI_Win_create((void *)base, size, disp_unit, info, comm, &newwin);
  return newwin;
}


inline void 
_REAL_MPI_::Win::Fence(int assert) const 
{
  (void) MPI_Win_fence(assert, mpi_win);
}


inline void 
_REAL_MPI_::Win::Free()
{
  (void) MPI_Win_free(&mpi_win);
}


inline void 
_REAL_MPI_::Win::Get(const void *origin_addr, int origin_count, 
		     const _REAL_MPI_::Datatype& origin_datatype, 
		     int target_rank, _REAL_MPI_::Aint target_disp, 
		     int target_count, 
		     const _REAL_MPI_::Datatype& target_datatype) const 
{
  (void) MPI_Get((void*) origin_addr, origin_count, origin_datatype,
		     target_rank, target_disp, 
		     target_count, target_datatype, mpi_win);

}


inline _REAL_MPI_::Group 
_REAL_MPI_::Win::Get_group() const
{
  MPI_Group mpi_group;
  (void) MPI_Win_get_group(mpi_win, &mpi_group);
  return mpi_group;
}


#if LAM_WANT_MPI_STUBS  
// This function is not yet implemented in LAM/MPI.  So it should
// fail to compile if the user tries to use it.
inline void 
_REAL_MPI_::Win::Lock(int lock_type, int rank, int assert) const 
{
  (void) MPI_Win_lock(lock_type, rank, assert, mpi_win);
}
#endif


inline void 
_REAL_MPI_::Win::Post(const _REAL_MPI_::Group& group, int assert) const 
{
  (void) MPI_Win_post(group, assert, mpi_win);
}


inline void 
_REAL_MPI_::Win::Put(const void* origin_addr, int origin_count, 
		     const _REAL_MPI_::Datatype& origin_datatype, 
		     int target_rank, _REAL_MPI_::Aint target_disp, 
		     int target_count, 
		     const _REAL_MPI_::Datatype& target_datatype) const 
{
  (void) MPI_Put((void*) origin_addr, origin_count, origin_datatype, 
		 target_rank, target_disp, target_count,
		 target_datatype, mpi_win);
  
}


inline void 
_REAL_MPI_::Win::Start(const _REAL_MPI_::Group& group, int assert) const
{
  (void) MPI_Win_start(group, assert, mpi_win);
}


#if LAM_WANT_MPI_STUBS  
// This function is not yet implemented in LAM/MPI.  So it should
// fail to compile if the user tries to use it.
inline bool 
_REAL_MPI_::Win::Test() const
{
  int flag;
  MPI_Win_test(mpi_win, &flag);
  return (bool) flag;
  
}
#endif


#if LAM_WANT_MPI_STUBS  
// This function is not yet implemented in LAM/MPI.  So it should
// fail to compile if the user tries to use it.
inline void 
_REAL_MPI_::Win::Unlock(int rank) const
{
  (void) MPI_Win_unlock(rank, mpi_win);
}
#endif


inline void 
_REAL_MPI_::Win::Wait() const
{
  (void) MPI_Win_wait(mpi_win);
}


//
// External Interfaces
//

#if LAM_WANT_MPI_STUBS  
// This function is not yet implemented in LAM/MPI.  So it should
// fail to compile if the user tries to use it.
inline void 
_REAL_MPI_::Win::Call_errhandler(int errorcode) const
{
  (void) MPI_Win_call_errhandler(mpi_win, errorcode);
}
#endif


inline int
_REAL_MPI_::Win::Create_keyval(_REAL_MPI_::Win::Copy_attr_function* 
			       win_copy_attr_fn, 
			       _REAL_MPI_::Win::Delete_attr_function* 
			       win_delete_attr_fn, void* extra_state)
{
  int val;
  (void) MPI_Win_create_keyval((MPI_Win_copy_attr_function *)win_copy_attr_fn,
			       (MPI_Win_delete_attr_function *)
			       win_delete_attr_fn, &val,extra_state);
  return val;
}


inline void 
_REAL_MPI_::Win::Delete_attr(int win_keyval) 
{
  (void) MPI_Win_delete_attr(mpi_win, win_keyval);
}


inline void 
_REAL_MPI_::Win::Free_keyval(int& win_keyval)
{
  (void) MPI_Win_free_keyval(&win_keyval);
}


inline bool 
_REAL_MPI_::Win::Get_attr(const Win& win, int win_keyval, 
			  void* attribute_val) const
{
  int ret;
  (void) MPI_Win_get_attr(win, win_keyval, attribute_val, &ret);
  return (bool) ret;
}


inline void 
_REAL_MPI_::Win::Get_name(char* win_name, int& resultlen) const
{
  (void) MPI_Win_get_name(mpi_win, win_name, &resultlen);
}


inline void 
_REAL_MPI_::Win::Set_attr(int win_keyval, const void* attribute_val) 
{
  (void) MPI_Win_set_attr(mpi_win, win_keyval,(void *) attribute_val);
}


inline void 
_REAL_MPI_::Win::Set_name(const char* win_name) 
{
  (void) MPI_Win_set_name(mpi_win, (char *)win_name);
}


#if 0
//
// User defined functions
//

typedef int _REAL_MPI_::Win::Copy_attr_function(const Win& oldwin, 
						int win_keyval, 
						void* extra_state, 
						void* attribute_val_in, 
						void* attribute_val_out, 
						bool& flag); 

typedef int _REAL_MPI_::Win::Delete_attr_function(&win, int win_keyval, 
						  void* attribute_val, 
						  void* extra_state); 

typedef void _REAL_MPI_::Win::Errhandler_fn(Win &, int *, ... );

#endif
