// -*- c++ -*-
//
// $Id: pintracomm.h,v 1.5 2003/10/22 23:35:36 adharurk Exp $
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

class Intracomm : public Comm {
public:
  // construction
  Intracomm() { }
  // copy
  Intracomm(const Intracomm& data) : Comm(data) { }
  
  // inter-language operability
  inline Intracomm(const MPI_Comm& data);

  //
  // Collective Communication
  //

  virtual void Barrier() const;

  virtual void Bcast(void *buffer, int count, 
		     const Datatype& datatype, int root) const;

  virtual void Gather(const void *sendbuf, int sendcount, 
		      const Datatype & sendtype, 
		      void *recvbuf, int recvcount, 
		      const Datatype & recvtype, int root) const;
  
  virtual void Gatherv(const void *sendbuf, int sendcount, 
		       const Datatype & sendtype, void *recvbuf, 
		       const int recvcounts[], const int displs[], 
		       const Datatype & recvtype, int root) const;
  
  virtual void Scatter(const void *sendbuf, int sendcount, 
		       const Datatype & sendtype, 
		       void *recvbuf, int recvcount, 
		       const Datatype & recvtype, int root) const;
  
  virtual void Scatterv(const void *sendbuf, const int sendcounts[], 
			const int displs[], const Datatype & sendtype,
			void *recvbuf, int recvcount, 
			const Datatype & recvtype, int root) const;
  
  virtual void Allgather(const void *sendbuf, int sendcount, 
			 const Datatype & sendtype, void *recvbuf, 
			 int recvcount, const Datatype & recvtype) const;

  virtual void Allgatherv(const void *sendbuf, int sendcount, 
			  const Datatype & sendtype, void *recvbuf, 
			  const int recvcounts[], const int displs[],
			  const Datatype & recvtype) const;

  virtual void Alltoall(const void *sendbuf, int sendcount, 
			const Datatype & sendtype, void *recvbuf, 
			int recvcount, const Datatype & recvtype) const;

  virtual void Alltoallv(const void *sendbuf, const int sendcounts[], 
			 const int sdispls[], const Datatype & sendtype, 
			 void *recvbuf, const int recvcounts[], 
			 const int rdispls[], const Datatype & recvtype) const;

  virtual void Alltoallw(const void *sendbuf, const int sendcounts[],
                         const int sdispls[], const Datatype sendtypes[],
                         void *recvbuf, const int recvcounts[],
                         const int rdispls[], const Datatype recvtypes[]) const;

  virtual void Reduce(const void *sendbuf, void *recvbuf, int count, 
		      const Datatype & datatype, const Op & op, 
		      int root) const;

  virtual void Allreduce(const void *sendbuf, void *recvbuf, int count,
			 const Datatype & datatype, const Op & op) const;

  virtual void Reduce_scatter(const void *sendbuf, void *recvbuf, 
			      int recvcounts[], 
			      const Datatype & datatype, 
			      const Op & op) const;

  virtual void Scan(const void *sendbuf, void *recvbuf, int count, 
		    const Datatype & datatype, const Op & op) const;

  virtual void Exscan(const void *sendbuf, void *recvbuf, int count,
                    const Datatype & datatype, const Op & op) const;

  Intracomm Dup() const;

  Intracomm& Clone() const;

  virtual Intracomm Create(const Group& group) const;
  
  virtual Intracomm Split(int color, int key) const;

  virtual Intercomm Create_intercomm(int local_leader, const Comm& peer_comm,
				     int remote_leader, int tag) const;
  
  virtual Cartcomm Create_cart(int ndims, const int dims[],
			       const MPI2CPP_BOOL_T periods[], 
			       MPI2CPP_BOOL_T reorder) const;
  
  virtual Graphcomm Create_graph(int nnodes, const int index[],
				 const int edges[], 
				 MPI2CPP_BOOL_T reorder) const;

  
  //
  // Process Creation and Management
  //
  
  virtual Intercomm Accept(const char* port_name, const Info& info, int root)
    const;

  virtual Intercomm Connect(const char* port_name, const Info& info, int root)
    const;

  virtual Intercomm Spawn(const char* command, const char* argv[], 
			  int maxprocs, const Info& info, int root) const;

  virtual Intercomm Spawn(const char* command, const char* argv[],
			  int maxprocs, const Info& info, 
			  int root, int array_of_errcodes[]) const;

  virtual Intercomm Spawn_multiple(int count, const char* array_of_commands[],
				   const char** array_of_argv[],
				   const int array_of_maxprocs[],
				   const Info array_of_info[], int root);

  virtual Intercomm Spawn_multiple(int count, const char* array_of_commands[],
				   const char** array_of_argv[],
				   const int array_of_maxprocs[],
				   const Info array_of_info[], int root,
				   int array_of_errcodes[]);


protected:

public: // JGS, friends issue
  static Op* current_op;

};
