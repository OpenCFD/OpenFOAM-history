// -*- c++ -*-
//
// $Id: pcomm.h,v 1.5 2003/06/25 14:51:40 jsquyres Exp $
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

class Comm_Null {
public:

  // construction
  Comm_Null() : mpi_comm(MPI_COMM_NULL) { }
  // copy
  Comm_Null(const Comm_Null& data) : mpi_comm(data.mpi_comm) { }
  // inter-language operability  
  Comm_Null(const MPI_Comm& data) : mpi_comm(data) { }

  // destruction
  virtual ~Comm_Null() { }

  inline Comm_Null& operator=(const Comm_Null& data) {
    mpi_comm = data.mpi_comm; 
    return *this;
  }

 // comparison
  // JGS make sure this is right (in other classes too)
  bool operator==(const Comm_Null& data) const {
    return (bool) (mpi_comm == data.mpi_comm); }

  bool operator!=(const Comm_Null& data) const {
    return (bool) !(*this == data);}

  // inter-language operability (conversion operators)
  operator MPI_Comm() const { return mpi_comm; }
  operator MPI_Comm*() /*const JGS*/ { return &mpi_comm; }

protected:
  MPI_Comm mpi_comm;
};

class Comm : public Comm_Null {
public:

  // $%^#$%^#$^ g++ 2.7.2!!! We have to rename these functions because
  // g++ does not scope properly.  It somehow loses the outter class (MPI
  // or PMPI) -- remember, g++ does not do namespaces! -- and thinks that 
  // we are conflicting with Errhandler_fn et al. in comm.h
  // @#$%@#$@#%$@#% g++!!
  // 3 Dec 1999 Screw g++ < 2.95.2.  Removed the PMPI_ prefixes.
  typedef void Errhandler_fn(Comm&, int*, ...);
  typedef int Copy_attr_function(const Comm& oldcomm, int comm_keyval,
				 void* extra_state, void* attribute_val_in,
				 void* attribute_val_out, 
				 bool& flag);
  typedef int Delete_attr_function(Comm& comm, int comm_keyval, 
				   void* attribute_val, void* extra_state);
#define _MPI2CPP_ERRHANDLERFN_ Errhandler_fn
#define _MPI2CPP_COPYATTRFN_ Copy_attr_function
#define _MPI2CPP_DELETEATTRFN_ Delete_attr_function

  // construction
  Comm() { }
  // copy
  Comm(const Comm_Null& data) : Comm_Null(data) { }
  // inter-language operability  
  Comm(const MPI_Comm& data) : Comm_Null(data) { }

  //
  // Point-to-Point
  //

  virtual void Send(const void *buf, int count, 
		    const Datatype & datatype, int dest, int tag) const;

  virtual void Recv(void *buf, int count, const Datatype & datatype,
		    int source, int tag, Status & status) const;

  virtual void Recv(void *buf, int count, const Datatype & datatype,
		    int source, int tag) const;
  
  virtual void Bsend(const void *buf, int count,
		     const Datatype & datatype, int dest, int tag) const;
  
  virtual void Ssend(const void *buf, int count, 
		     const Datatype & datatype, int dest, int tag) const;
  
  virtual void Rsend(const void *buf, int count,
		     const Datatype & datatype, int dest, int tag) const;
  
  virtual Request Isend(const void *buf, int count,
			const Datatype & datatype,
			int dest, int tag) const;
  
  virtual Request Ibsend(const void *buf, int count, const
			 Datatype & datatype, int dest, int tag) const;
  
  virtual Request Issend(const void *buf, int count,
			 const Datatype & datatype, int dest, int tag) const;
  
  virtual Request Irsend(const void *buf, int count,
			 const Datatype & datatype, int dest, int tag) const;
  
  virtual Request Irecv(void *buf, int count,
			const Datatype & datatype, int source, int tag) const;
  
  virtual bool Iprobe(int source, int tag, Status & status) const;
  
  virtual bool Iprobe(int source, int tag) const;
  
  virtual void Probe(int source, int tag, Status & status) const;
  
  virtual void Probe(int source, int tag) const;
  
  virtual Prequest Send_init(const void *buf, int count,
			     const  Datatype & datatype,
			     int dest, int tag) const;
  
  virtual Prequest Bsend_init(const void *buf, int count,
			      const Datatype & datatype,
			      int dest, int tag) const;
  
  virtual Prequest Ssend_init(const void *buf, int count,
			      const Datatype & datatype,
			      int dest, int tag) const;
  
  virtual Prequest Rsend_init(const void *buf, int count,
			      const Datatype & datatype,
			      int dest, int tag) const;
  
  virtual Prequest Recv_init(void *buf, int count,
			     const Datatype & datatype,
			     int source, int tag) const;
  
  virtual void Sendrecv(const void *sendbuf, int sendcount,
			const Datatype & sendtype, int dest, int sendtag, 
			void *recvbuf, int recvcount, 
			const Datatype & recvtype, int source,
			int recvtag, Status & status) const;
 
  virtual void Sendrecv(const void *sendbuf, int sendcount,
			const Datatype & sendtype, int dest, int sendtag, 
			void *recvbuf, int recvcount, 
			const Datatype & recvtype, int source,
			int recvtag) const;

  virtual void Sendrecv_replace(void *buf, int count,
				const Datatype & datatype, int dest, 
				int sendtag, int source,
				int recvtag, Status & status) const;

  virtual void Sendrecv_replace(void *buf, int count,
				const Datatype & datatype, int dest, 
				int sendtag, int source,
				int recvtag) const;

  //
  // Groups, Contexts, and Communicators
  //
  
  virtual Group Get_group() const;
  
  virtual int Get_size() const;
  
  virtual int Get_rank() const;

  static int Compare(const Comm & comm1, const Comm & comm2);

  virtual void Free(void);
  
  virtual bool Is_inter() const;


  //
  // Process Creation
  //

  virtual void Disconnect();

  static Intercomm Get_parent();

  static Intercomm Join(const int fd);


  //
  //External Interfaces
  //

  virtual void Get_name(char * comm_name, int& resultlen) const;

  virtual void Set_name(const char* comm_name);

  
  //
  //Process Topologies
  //

  virtual int Get_topology() const;
  
  //
  // Environmental Inquiry
  //

  virtual void Abort(int errorcode);

  virtual void Set_errhandler(const Errhandler& errhandler);

  virtual Errhandler Get_errhandler() const;

  static Errhandler Create_errhandler(Errhandler_fn* function);

  //
  // Keys and Attributes
  //

  static int Create_keyval(Copy_attr_function*,
			   Delete_attr_function*,
			   void*);
  
  static void Free_keyval(int& comm_keyval);
  
  virtual void Set_attr(int comm_keyval, const void* attribute_val) const;
  
  virtual bool Get_attr(int comm_keyval, void* attribute_val) const;
  
  virtual void Delete_attr(int comm_keyval);
  
  static int NULL_COPY_FN(const PMPI::Comm& oldcomm, int comm_keyval,
			  void* extra_state, void* attribute_val_in,
			  void* attribute_val_out, bool& flag);
  
  static int DUP_FN(const Comm& oldcomm, int comm_keyval,
		    void* extra_state, void* attribute_val_in,
		    void* attribute_val_out, bool& flag);
  
  static int NULL_DELETE_FN(Comm& comm, int comm_keyval, void* attribute_val,
			    void* extra_state);
private:

public: // JGS friends cause portability problems

  Errhandler* my_errhandler;

  // You really don't want to know.  :-)

  // These all have to do with C callbacks; since we're using the C
  // callback mechanism (since we're implemented on top of C MPI
  // function calls), we have to create cross references when our C++
  // intercept callback routine is called -- it has to lookup the
  // right callback function to invoke.  Woof!!

  typedef mpi2cppMap<Comm*, CommType>::Pair comm_pair_t;
  typedef mpi2cppMap<MPI_Comm, comm_pair_t*> mpi_comm_map_t;
  static mpi_comm_map_t mpi_comm_map;

  typedef mpi2cppMap<MPI_Comm, Comm*> mpi_err_map_t;
  static mpi_err_map_t mpi_err_map;

  typedef mpi2cppMap<Comm::_MPI2CPP_COPYATTRFN_*, Comm::_MPI2CPP_DELETEATTRFN_*>::Pair key_pair_t;
  typedef mpi2cppMap<int, key_pair_t*> key_fn_map_t;
  static key_fn_map_t key_fn_map;
};

