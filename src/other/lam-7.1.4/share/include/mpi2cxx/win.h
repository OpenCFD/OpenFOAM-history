// -*- c++ -*-
//
// $Id: win.h,v 1.4 2003/01/20 03:43:05 vsahay Exp $
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


class Win {
public:
#if LAM_WANT_PROFILE

  // construction / destruction
  Win() { }
  virtual ~Win() { }


  // copy / assignment
  Win(const Win& data) : pmpi_win(data.pmpi_win) { }

  Win(const MPI_Win &i) : pmpi_win(i) { }

  Win& operator=(const Win& data) {
    pmpi_win = data.pmpi_win; return *this; }

  // comparison, don't need for win

  // inter-language operability
  Win& operator= (const MPI_Win &i) {
    pmpi_win = i; return *this; }
  operator MPI_Win () const { return pmpi_win; }
  //  operator MPI_Win* () const { return pmpi_win; }
  operator const PMPI::Win&() const { return pmpi_win; }

#else

  Win() { }
  // copy
  Win(const Win& data) : mpi_win(data.mpi_win) { }

  Win(const MPI_Win &i) : mpi_win(i) { }
	
  virtual ~Win() { }

  Win& operator=(const Win& data) {
    mpi_win = data.mpi_win; return *this; }

  // comparison, don't need for win

  // inter-language operability
  Win& operator= (const MPI_Win &i) {
    mpi_win = i; return *this; }
  operator MPI_Win () const { return mpi_win; }
  //  operator MPI_Win* () const { return (MPI_Win*)&mpi_win; }

#endif

  //
  // User defined functions
  //
  typedef int Copy_attr_function(const Win& oldwin, int win_keyval, 
				 void* extra_state, void* attribute_val_in, 
				 void* attribute_val_out, bool& flag); 
  
  typedef int Delete_attr_function(Win& win, int win_keyval, 
				   void* attribute_val, void* extra_state); 
  
  typedef void Errhandler_fn(Win &, int *, ... );
  
  //
  // Miscellany
  //
  static MPI::Errhandler Create_errhandler(Errhandler_fn* function); 
  virtual MPI::Errhandler Get_errhandler() const; 
  virtual void Set_errhandler(const MPI::Errhandler& errhandler);

  //
  // One sided communication
  //
  virtual void Accumulate(const void* origin_addr, int origin_count, 
			  const MPI::Datatype& origin_datatype, 
			  int target_rank, MPI::Aint target_disp, 
			  int target_count, 
			  const MPI::Datatype& target_datatype, 
			  const MPI::Op& op) const;
  
  virtual void Complete() const;
  
  static  Win Create(const void* base, MPI::Aint size, int disp_unit, 
		     const MPI::Info& info, const MPI::Intracomm& comm);
  
  virtual void Fence(int assert) const;
  
  virtual void Free(); 
  
  virtual void Get(const void *origin_addr, int origin_count, 
		   const MPI::Datatype& origin_datatype, int target_rank, 
		   MPI::Aint target_disp, int target_count, 
		   const MPI::Datatype& target_datatype) const;
  
  virtual MPI::Group Get_group() const;  

#if LAM_WANT_MPI_STUBS  
  // This function is not yet implemented in LAM/MPI.  So it should
  // fail to compile if the user tries to use it.
  virtual void Lock(int lock_type, int rank, int assert) const;
#endif
  
  virtual void Post(const MPI::Group& group, int assert) const;
  
  virtual void Put(const void* origin_addr, int origin_count, 
		   const MPI::Datatype& origin_datatype, int target_rank, 
		   MPI::Aint target_disp, int target_count, 
		   const MPI::Datatype& target_datatype) const;
  
  virtual void Start(const MPI::Group& group, int assert) const; 
  
#if LAM_WANT_MPI_STUBS  
  // This function is not yet implemented in LAM/MPI.  So it should
  // fail to compile if the user tries to use it.
  virtual bool Test() const;
#endif
  
#if LAM_WANT_MPI_STUBS  
  // This function is not yet implemented in LAM/MPI.  So it should
  // fail to compile if the user tries to use it.
  virtual void Unlock(int rank) const;
#endif
  
  virtual void Wait() const;
  

  //
  // External Interfaces
  //


#if LAM_WANT_MPI_STUBS  
  // This function is not yet implemented in LAM/MPI.  So it should
  // fail to compile if the user tries to use it.
  virtual void Call_errhandler(int errorcode) const;
#endif
  
  static int Create_keyval(Copy_attr_function* win_copy_attr_fn, 
			   Delete_attr_function* win_delete_attr_fn, 
			   void* extra_state);
  
  virtual void Delete_attr(int win_keyval);
  
  static void Free_keyval(int& win_keyval); 
  
  bool Get_attr(const Win& win, int win_keyval,
		void* attribute_val) const;
  
  virtual void Get_name(char* win_name, int& resultlen) const;
  
  virtual void Set_attr(int win_keyval, const void* attribute_val);
  
  virtual void Set_name(const char* win_name);
  
protected:
#if LAM_WANT_PROFILE
  PMPI::Win pmpi_win;
#else
  MPI_Win mpi_win;
#endif
};
