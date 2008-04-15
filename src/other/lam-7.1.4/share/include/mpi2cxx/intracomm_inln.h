// -*- c++ -*-
//
// $Id: intracomm_inln.h,v 1.5 2003/10/22 23:35:36 adharurk Exp $
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

inline
_REAL_MPI_::Intracomm::Intracomm(const MPI_Comm& data) {
  int flag;
  if (MPI::Is_initialized() && (data != MPI_COMM_NULL)) {
    (void)MPI_Comm_test_inter(data, &flag);
    if (flag)
      mpi_comm = MPI_COMM_NULL;
    else
      mpi_comm = data;
  }
  else {
    mpi_comm = data;
  }
}

//
// Collective Communication
//

inline void
_REAL_MPI_::Intracomm::Barrier() const
{
  (void)MPI_Barrier(mpi_comm);
}

inline void
_REAL_MPI_::Intracomm::Bcast(void *buffer, int count, 
      const _REAL_MPI_::Datatype& datatype, int root) const
{ 
  (void)MPI_Bcast(buffer, count, datatype, root, mpi_comm);
}

inline void
_REAL_MPI_::Intracomm::Gather(const void *sendbuf, int sendcount, 
			      const _REAL_MPI_::Datatype & sendtype, 
			      void *recvbuf, int recvcount, 
			      const _REAL_MPI_::Datatype & recvtype, int root) const
{
  (void)MPI_Gather((void *)sendbuf, sendcount, sendtype,
		   recvbuf, recvcount, recvtype, root, mpi_comm);
}

inline void
_REAL_MPI_::Intracomm::Gatherv(const void *sendbuf, int sendcount, 
	const _REAL_MPI_::Datatype & sendtype, void *recvbuf, 
	const int recvcounts[], const int displs[], 
	const _REAL_MPI_::Datatype & recvtype, int root) const
{
  (void)MPI_Gatherv((void *)sendbuf, sendcount,  sendtype,
		    recvbuf, (int *)recvcounts, (int *)displs, 
		    recvtype, root, mpi_comm);
}

inline void
_REAL_MPI_::Intracomm::Scatter(const void *sendbuf, int sendcount, 
	const _REAL_MPI_::Datatype & sendtype, 
	void *recvbuf, int recvcount, 
	const _REAL_MPI_::Datatype & recvtype, int root) const
{ 
  (void)MPI_Scatter((void *)sendbuf, sendcount, sendtype,
		    recvbuf, recvcount, recvtype, root, mpi_comm);
}

inline void
_REAL_MPI_::Intracomm::Scatterv(const void *sendbuf, const int sendcounts[], 
	 const int displs[], const _REAL_MPI_::Datatype & sendtype,
	 void *recvbuf, int recvcount, 
	 const _REAL_MPI_::Datatype & recvtype, int root) const
{
  (void)MPI_Scatterv((void *)sendbuf, (int *) sendcounts, 
		     (int *) displs, sendtype, 
		     recvbuf, recvcount, recvtype, 
		     root, mpi_comm);
}

inline void
_REAL_MPI_::Intracomm::Allgather(const void *sendbuf, int sendcount, 
	  const _REAL_MPI_::Datatype & sendtype, void *recvbuf, 
	  int recvcount, const _REAL_MPI_::Datatype & recvtype) const 
{
  (void)MPI_Allgather((void *) sendbuf, sendcount, 
		      sendtype, recvbuf, recvcount,
		      recvtype, mpi_comm);
}

inline void
_REAL_MPI_::Intracomm::Allgatherv(const void *sendbuf, int sendcount, 
	   const _REAL_MPI_::Datatype & sendtype, void *recvbuf, 
	   const int recvcounts[], const int displs[],
	   const _REAL_MPI_::Datatype & recvtype) const
{
  (void)MPI_Allgatherv((void *)sendbuf, sendcount, 
		       sendtype, recvbuf, 
		       (int *) recvcounts, (int *) displs, 
		       recvtype, mpi_comm);
}

inline void
_REAL_MPI_::Intracomm::Alltoall(const void *sendbuf, int sendcount, 
	 const _REAL_MPI_::Datatype & sendtype, void *recvbuf, 
	 int recvcount, const _REAL_MPI_::Datatype & recvtype) const
{
  (void)MPI_Alltoall((void *) sendbuf, sendcount,
		     sendtype, recvbuf, recvcount,
		     recvtype, mpi_comm);
}

inline void
_REAL_MPI_::Intracomm::Alltoallv(const void *sendbuf, const int sendcounts[], 
	  const int sdispls[], const _REAL_MPI_::Datatype & sendtype, 
	  void *recvbuf, const int recvcounts[], 
	  const int rdispls[], const _REAL_MPI_::Datatype & recvtype) const 
{
    (void)MPI_Alltoallv((void *) sendbuf, (int *) sendcounts, 
			(int *) sdispls, sendtype, recvbuf, 
			(int *) recvcounts, (int *) rdispls, 
			recvtype,mpi_comm);
}

inline void
_REAL_MPI_::Intracomm::Alltoallw(const void *sendbuf, const int sendcounts[],
	const int sdispls[], const Datatype sendtypes[],
	void *recvbuf, const int recvcounts[],
	const int rdispls[], const Datatype recvtypes[]) const
{
  (void)MPI_Alltoallw((void *) sendbuf, (int *) sendcounts,
		      (int *) sdispls, (MPI_Datatype *) sendtypes, recvbuf,
		      (int *) recvcounts, (int *) rdispls,
		      (MPI_Datatype *) recvtypes, mpi_comm);
}

inline void
_REAL_MPI_::Intracomm::Reduce(const void *sendbuf, void *recvbuf, int count, 
       const _REAL_MPI_::Datatype & datatype, const _REAL_MPI_::Op& op, 
       int root) const
{
  current_op = (_REAL_MPI_::Op*)&op;
  (void)MPI_Reduce((void*)sendbuf, recvbuf, count, datatype, op, root, mpi_comm);
  current_op = (Op*)0;
}

inline void
_REAL_MPI_::Intracomm::Allreduce(const void *sendbuf, void *recvbuf, int count,
	  const _REAL_MPI_::Datatype & datatype, const _REAL_MPI_::Op& op) const
{
  current_op = (_REAL_MPI_::Op*)&op;
  (void)MPI_Allreduce ((void*)sendbuf, recvbuf, count, datatype,  op, mpi_comm);
  current_op = (Op*)0;
}

inline void
_REAL_MPI_::Intracomm::Reduce_scatter(const void *sendbuf, void *recvbuf, 
	       int recvcounts[], 
	       const _REAL_MPI_::Datatype & datatype, 
	       const _REAL_MPI_::Op& op) const
{
  current_op = (_REAL_MPI_::Op*)&op;
  (void)MPI_Reduce_scatter((void*)sendbuf, recvbuf, recvcounts,
			   datatype, op, mpi_comm);
  current_op = (Op*)0;
}

inline void
_REAL_MPI_::Intracomm::Scan(const void *sendbuf, void *recvbuf, int count, 
     const _REAL_MPI_::Datatype & datatype, const _REAL_MPI_::Op& op) const
{
  current_op = (_REAL_MPI_::Op*)&op;
  (void)MPI_Scan((void *)sendbuf, recvbuf, count, datatype, op, mpi_comm);
  current_op = (Op*)0;
}

inline void
_REAL_MPI_::Intracomm::Exscan(const void *sendbuf, void *recvbuf, int count,
			      const _REAL_MPI_::Datatype & datatype, 
			      const _REAL_MPI_::Op& op) const
{
  current_op = (_REAL_MPI_::Op*)&op;
  (void)MPI_Exscan((void *)sendbuf, recvbuf, count, datatype, op, mpi_comm);
  current_op = (Op*)0;
}

inline _REAL_MPI_::Intracomm
_REAL_MPI_::Intracomm::Dup() const
{
  MPI_Comm newcomm;
  (void)MPI_Comm_dup(mpi_comm, &newcomm);
  return newcomm;
}

inline _REAL_MPI_::Intracomm&
_REAL_MPI_::Intracomm::Clone() const
{
  MPI_Comm newcomm;
  (void)MPI_Comm_dup(mpi_comm, &newcomm);
  _REAL_MPI_::Intracomm* dup = new _REAL_MPI_::Intracomm(newcomm);
  return *dup;
}

inline _REAL_MPI_::Intracomm
_REAL_MPI_::Intracomm::Create(const _REAL_MPI_::Group& group) const
{
  MPI_Comm newcomm;
  (void)MPI_Comm_create(mpi_comm, group, &newcomm);
  return newcomm;
}

inline _REAL_MPI_::Intracomm
_REAL_MPI_::Intracomm::Split(int color, int key) const
{
  MPI_Comm newcomm;
  (void)MPI_Comm_split(mpi_comm, color, key, &newcomm);
  return newcomm;
}



inline _REAL_MPI_::Intercomm
_REAL_MPI_::Intracomm::Create_intercomm(int local_leader,
					const _REAL_MPI_::Comm& peer_comm,
					int remote_leader, int tag) const
{
  MPI_Comm newintercomm;
  (void)MPI_Intercomm_create(mpi_comm, local_leader, peer_comm,
			     remote_leader, tag, &newintercomm); 
  return newintercomm;
}

inline _REAL_MPI_::Cartcomm
_REAL_MPI_::Intracomm::Create_cart(int ndims, const int dims[],
				   const bool periods[], bool reorder) const
{
  int *int_periods = new int [ndims];
  for (int i=0; i<ndims; i++)
    int_periods[i] = (int) periods[i];
  
  MPI_Comm newcomm;
  (void)MPI_Cart_create(mpi_comm, ndims, (int*)dims, 
		      int_periods, (int)reorder, &newcomm);
  delete [] int_periods;
  return newcomm;
}

inline _REAL_MPI_::Graphcomm
_REAL_MPI_::Intracomm::Create_graph(int nnodes, const int index[],
				    const int edges[], bool reorder) const
{
  MPI_Comm newcomm;
  (void)MPI_Graph_create(mpi_comm, nnodes, (int*)index, 
		       (int*)edges, (int)reorder, &newcomm);
  return newcomm;
}


//
// Process Creation and Management
//

inline _REAL_MPI_::Intercomm
_REAL_MPI_::Intracomm::Accept(const char* port_name, 
			      const _REAL_MPI_::Info& info,
			      int root) const
{
  MPI_Comm newcomm;
  (void) MPI_Comm_accept((char *) port_name, info, root, mpi_comm,
			 &newcomm);
  return newcomm;
}


inline _REAL_MPI_::Intercomm
_REAL_MPI_::Intracomm::Connect(const char* port_name,
			       const _REAL_MPI_::Info& info,
			       int root) const
{
  MPI_Comm newcomm;
  (void) MPI_Comm_connect((char *) port_name, info, root, mpi_comm,
			  &newcomm);
  return newcomm;
}


inline _REAL_MPI_::Intercomm
_REAL_MPI_::Intracomm::Spawn(const char* command, const char* argv[],
			     int maxprocs, const _REAL_MPI_::Info& info, 
			     int root) const
{
  MPI_Comm newcomm;
  (void) MPI_Comm_spawn((char *) command, (char **) argv, maxprocs,
			info, root, mpi_comm, &newcomm, 
			(int *)MPI_ERRCODES_IGNORE);
  return newcomm;
}


inline _REAL_MPI_::Intercomm
_REAL_MPI_::Intracomm::Spawn(const char* command, const char* argv[],
                             int maxprocs, const _REAL_MPI_::Info& info,
                             int root, int array_of_errcodes[]) const
{
  MPI_Comm newcomm;
  (void) MPI_Comm_spawn((char *) command, (char **) argv, maxprocs,
                        info, root, mpi_comm, &newcomm, 
			array_of_errcodes);
  return newcomm;
}


inline _REAL_MPI_::Intercomm
_REAL_MPI_::Intracomm::Spawn_multiple(int count, 
				      const char* array_of_commands[],
				      const char** array_of_argv[],
				      const int array_of_maxprocs[],
				      const Info array_of_info[], int root)
{
  MPI_Comm newcomm;
  MPI_Comm_spawn_multiple(count, (char **) array_of_commands, 
			  (char ***) array_of_argv, (int *) array_of_maxprocs,
			  (MPI_Info *) array_of_info, root,
			  mpi_comm, &newcomm, (int *)MPI_ERRCODES_IGNORE);
  return newcomm;
}


inline _REAL_MPI_::Intercomm
_REAL_MPI_::Intracomm::Spawn_multiple(int count,
                                      const char* array_of_commands[],
                                      const char** array_of_argv[],
                                      const int array_of_maxprocs[],
                                      const Info array_of_info[], int root,
				      int array_of_errcodes[])
{
  MPI_Comm newcomm;
  MPI_Comm_spawn_multiple(count, (char **) array_of_commands, 
                          (char ***) array_of_argv, (int *) array_of_maxprocs,
                          (MPI_Info *) array_of_info, root,
                          mpi_comm, &newcomm, array_of_errcodes);
  return newcomm;
}










