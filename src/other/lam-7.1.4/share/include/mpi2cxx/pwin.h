// -*- c++ -*- 
// 
// $Id: pwin.h,v 1.3 2003/01/20 03:43:06 vsahay Exp $ 
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

class Win {

  friend class PComm; //so I can access pmpi_win data member in comm.cc
  friend class PRequest; //and also from request.cc

public:

  inline Win() { }
  // copy
  Win(const Win& data) : mpi_win(data.mpi_win) { }

  inline Win(const MPI_Win &i) : mpi_win(i) { }

  inline virtual ~Win() {}

  Win& operator=(const Win& data) {
    mpi_win = data.mpi_win; return *this; }

  // comparison, don't need for win

  // inter-language operability
  inline Win& operator= (const MPI_Win &i) {
    mpi_win = i; return *this; }
  inline operator MPI_Win () const { return mpi_win; }
  //  operator MPI_Win* () const { return (MPI_Win*)&mpi_win; }

  
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
  static Errhandler Create_errhandler(Errhandler_fn* fn); 
  
  inline virtual Errhandler Get_errhandler() const; 

  inline virtual void Set_errhandler(const Errhandler& errhandler);

  //
  // One sided communication
  //
  inline virtual void Accumulate(const void* origin_addr, int origin_count, 
				 const Datatype& origin_datatype, 
				 int target_rank, Aint target_disp, 
				 int target_count, 
				 const Datatype& target_datatype, 
				 const Op& op) const;
  
  inline virtual void Complete() const;
  
  static  Win Create(const void* base, Aint size, int disp_unit, 
		     const Info& info, const Intracomm& comm);
  
  inline virtual void Fence(int assert) const;
  
  inline virtual void Free(); 
  
  inline virtual void Get(const void *origin_addr, int origin_count, 
			  const Datatype& origin_datatype, 
			  int target_rank, Aint target_disp, 
			  int target_count, 
			  const Datatype& target_datatype) const;
  
  inline virtual Group Get_group() const;  
  
#if LAM_WANT_MPI_STUBS  
  // This function is not yet implemented in LAM/MPI.  So it should
  // fail to compile if the user tries to use it.
  inline virtual void Lock(int lock_type, int rank, int assert) const;
#endif
  
  inline virtual void Post(const Group& group, int assert) const;
  
  inline virtual void Put(const void* origin_addr, int origin_count, 
			  const Datatype& origin_datatype, 
			  int target_rank, Aint target_disp, 
			  int target_count, 
			  const Datatype& target_datatype) const;
  
  inline virtual void Start(const Group& group, int assert) const; 
  
#if LAM_WANT_MPI_STUBS  
  // This function is not yet implemented in LAM/MPI.  So it should
  // fail to compile if the user tries to use it.
  inline virtual bool Test() const;
#endif
  
#if LAM_WANT_MPI_STUBS  
  // This function is not yet implemented in LAM/MPI.  So it should
  // fail to compile if the user tries to use it.
  inline virtual void Unlock(int rank) const;
#endif
  
  inline virtual void Wait() const;
  

  //
  // External Interfaces
  //


#if LAM_WANT_MPI_STUBS  
  // This function is not yet implemented in LAM/MPI.  So it should
  // fail to compile if the user tries to use it.
  inline virtual void Call_errhandler(int errorcode) const;
#endif
  
  static int Create_keyval(Copy_attr_function* win_copy_attr_fn, 
			   Delete_attr_function* win_delete_attr_fn, 
			   void* extra_state);
  
  inline virtual void Delete_attr(int win_keyval);
  
  static void Free_keyval(int& win_keyval); 
  
  bool Get_attr(const Win& win, int win_keyval, void* attribute_val) const;
  
  inline virtual void Get_name(char* win_name, int& resultlen) const;
  
  inline virtual void Set_attr(int win_keyval, const void* attribute_val);
  
  inline virtual void Set_name(const char* win_name);
  

protected:
  MPI_Win mpi_win;

};
