// -*- c++ -*-
//
// $Id: comm.h,v 1.5 2003/04/15 01:32:56 adharurk Exp $
//
// Copyright (c) 2001-2004 The Trustees of Indiana University.  
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
#if LAM_WANT_PROFILE
  //  friend class PMPI::Comm_Null;
#endif
public:

#if LAM_WANT_PROFILE

  // construction
  inline Comm_Null() { }
  // copy
  inline Comm_Null(const Comm_Null& data) : pmpi_comm(data.pmpi_comm) { }
  // inter-language operability  
  inline Comm_Null(const MPI_Comm& data) : pmpi_comm(data) { }

  inline Comm_Null(const PMPI::Comm_Null& data) : pmpi_comm(data) { }

  // destruction
  virtual inline ~Comm_Null() { }

  inline Comm_Null& operator=(const Comm_Null& data) {
    pmpi_comm = data.pmpi_comm; 
    return *this;
  }

  // comparison
  inline bool operator==(const Comm_Null& data) const {
    return (bool) (pmpi_comm == data.pmpi_comm); }

  inline bool operator!=(const Comm_Null& data) const {
    return (bool) (pmpi_comm != data.pmpi_comm);}

  // inter-language operability (conversion operators)
  inline operator MPI_Comm() const { return pmpi_comm; }
  //  inline operator MPI_Comm*() /*const JGS*/ { return pmpi_comm; }
  inline operator const PMPI::Comm_Null&() const { return pmpi_comm; }

#else

  // construction
  inline Comm_Null() : mpi_comm(MPI_COMM_NULL) { }
  // copy
  inline Comm_Null(const Comm_Null& data) : mpi_comm(data.mpi_comm) { }
  // inter-language operability  
  inline Comm_Null(const MPI_Comm& data) : mpi_comm(data) { }

  // destruction
  virtual inline ~Comm_Null() { }

 // comparison
  // JGS make sure this is right (in other classes too)
  inline bool operator==(const Comm_Null& data) const {
    return (bool) (mpi_comm == data.mpi_comm); }

  inline bool operator!=(const Comm_Null& data) const {
    return (bool) !(*this == data);}

  // inter-language operability (conversion operators)
  inline operator MPI_Comm() const { return mpi_comm; }

#endif

  
protected:

#if LAM_WANT_PROFILE
  PMPI::Comm_Null pmpi_comm;
#else
  MPI_Comm mpi_comm;
#endif
  

};


class Comm : public Comm_Null {
public:

  typedef void Errhandler_fn(Comm&, int*, ...);
  typedef int Copy_attr_function(const Comm& oldcomm, int comm_keyval,
				 void* extra_state, void* attribute_val_in,
				 void* attribute_val_out, 
				 bool& flag);
  typedef int Delete_attr_function(Comm& comm, int comm_keyval, 
				   void* attribute_val,
				   void* extra_state);
#if !LAM_WANT_PROFILE
#define _MPI2CPP_ERRHANDLERFN_ Errhandler_fn
#define _MPI2CPP_COPYATTRFN_ Copy_attr_function
#define _MPI2CPP_DELETEATTRFN_ Delete_attr_function
#endif

  // construction
  Comm() { }

  // copy
  Comm(const Comm_Null& data) : Comm_Null(data) { }

#if LAM_WANT_PROFILE
  Comm(const Comm& data) : 
    Comm_Null(data),
    pmpi_comm((const PMPI::Comm&) data) { }

  // inter-language operability
  Comm(const MPI_Comm& data) : Comm_Null(data), pmpi_comm(data) { }

  Comm(const PMPI::Comm& data) :
    Comm_Null((const PMPI::Comm_Null&)data),
    pmpi_comm(data) { }

  operator const PMPI::Comm&() const { return pmpi_comm; }

  // assignment
  Comm& operator=(const Comm& data) {
    this->Comm_Null::operator=(data);
    pmpi_comm = data.pmpi_comm; 
    return *this;
  }
  Comm& operator=(const Comm_Null& data) {
    this->Comm_Null::operator=(data);
    MPI_Comm tmp = data;
    pmpi_comm = tmp; 
    return *this;
  }
  // inter-language operability
  Comm& operator=(const MPI_Comm& data) {
    this->Comm_Null::operator=(data);
    pmpi_comm = data;
    return *this;
  }

#else
  Comm(const Comm& data) : Comm_Null(data.mpi_comm) { }
  // inter-language operability
  Comm(const MPI_Comm& data) : Comm_Null(data) { }
#endif


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
		     const Datatype & datatype, int dest, int tag) const ;

  virtual void Rsend(const void *buf, int count,
		     const Datatype & datatype, int dest, int tag) const;
  
  virtual Request Isend(const void *buf, int count,
			const Datatype & datatype, int dest, int tag) const;
  
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
			     const Datatype & datatype, int dest, 
			     int tag) const;
  
  virtual Prequest Bsend_init(const void *buf, int count,
			      const Datatype & datatype, int dest, 
			      int tag) const;
  
  virtual Prequest Ssend_init(const void *buf, int count,
			      const Datatype & datatype, int dest, 
			      int tag) const;
  
  virtual Prequest Rsend_init(const void *buf, int count,
			      const Datatype & datatype, int dest, 
			      int tag) const;
  
  virtual Prequest Recv_init(void *buf, int count,
			     const Datatype & datatype, int source, 
			     int tag) const;
  
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
  
  virtual Comm& Clone() const = 0;

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

  //
  // Errhandler
  //

  virtual void Set_errhandler(const Errhandler& errhandler);

  virtual Errhandler Get_errhandler() const;

  //JGS took out const below from fn arg
  static Errhandler Create_errhandler(Comm::Errhandler_fn* function);

  //
  // Keys and Attributes
  //

//JGS I took the const out because it causes problems when trying to
//call this function with the predefined NULL_COPY_FN etc.
  static int Create_keyval(Copy_attr_function* comm_copy_attr_fn,
			   Delete_attr_function* comm_delete_attr_fn,
			   void* extra_state);
  
  static void Free_keyval(int& comm_keyval);

  virtual void Set_attr(int comm_keyval, const void* attribute_val) const;

  virtual bool Get_attr(int comm_keyval, void* attribute_val) const;
  
  virtual void Delete_attr(int comm_keyval);

  static int NULL_COPY_FN(const Comm& oldcomm, int comm_keyval,
			  void* extra_state, void* attribute_val_in,
			  void* attribute_val_out, bool& flag);
  
  static int DUP_FN(const Comm& oldcomm, int comm_keyval,
		    void* extra_state, void* attribute_val_in,
		    void* attribute_val_out, bool& flag);
  
  static int NULL_DELETE_FN(Comm& comm, int comm_keyval, void* attribute_val,
			    void* extra_state);


  //#if LAM_WANT_PROFILE
  //  virtual const PMPI::Comm& get_pmpi_comm() const { return pmpi_comm; }
  //#endif

private:
#if LAM_WANT_PROFILE
  PMPI::Comm pmpi_comm;
#endif

#if ! LAM_WANT_PROFILE
public: // JGS hmmm, these used by errhandler_intercept
        // should make it a friend

  Errhandler* my_errhandler;

  typedef mpi2cppMap<Comm*, CommType>::Pair comm_pair_t;
  typedef mpi2cppMap<MPI_Comm, comm_pair_t*> mpi_comm_map_t;
  static mpi_comm_map_t mpi_comm_map;

  typedef mpi2cppMap<MPI_Comm, Comm*> mpi_err_map_t;
  static mpi_err_map_t mpi_err_map;

  typedef mpi2cppMap<Comm::_MPI2CPP_COPYATTRFN_*, Comm::_MPI2CPP_DELETEATTRFN_*>::Pair key_pair_t;
  typedef mpi2cppMap<int, key_pair_t*> key_fn_map_t;
  static key_fn_map_t key_fn_map;

  void init() {
    my_errhandler = (Errhandler*)0;
  }
#endif

};
