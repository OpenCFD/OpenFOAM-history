// -*- c++ -*-
//
// $Id: intracomm.cc,v 1.5 2003/10/22 23:42:12 adharurk Exp $
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

void MPI::Intracomm::Barrier() const
{
  pmpi_comm.Barrier();
}

void MPI::Intracomm::Bcast(void *buffer, int count, 
			   const MPI::Datatype& datatype, int root) const
{
  pmpi_comm.Bcast(buffer, count, datatype, root);
}

void MPI::Intracomm::Gather(const void *sendbuf, int sendcount, 
			    const MPI::Datatype & sendtype, 
			    void *recvbuf, int recvcount, 
			    const MPI::Datatype & recvtype, int root) const
{
  pmpi_comm.Gather(sendbuf, sendcount, sendtype, recvbuf, recvcount,
		   recvtype, root);
}


void MPI::Intracomm::Gatherv(const void *sendbuf, int sendcount, 
			     const MPI::Datatype & sendtype, void *recvbuf, 
			     const int recvcounts[], const int displs[], 
			     const MPI::Datatype & recvtype, int root) const
{
  pmpi_comm.Gatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs,
		    recvtype, root);
}

void MPI::Intracomm::Scatter(const void *sendbuf, int sendcount, 
			      const MPI::Datatype & sendtype, 
			      void *recvbuf, int recvcount, 
			      const MPI::Datatype & recvtype, int root) const
{
  pmpi_comm.Scatter(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root);
}

void MPI::Intracomm::Scatterv(const void *sendbuf, const int sendcounts[], 
			      const int displs[], const MPI::Datatype & sendtype,
			      void *recvbuf, int recvcount, 
			      const MPI::Datatype & recvtype, int root) const
{
  pmpi_comm.Scatterv(sendbuf, sendcounts, displs, sendtype,
		     recvbuf, recvcount, recvtype, root);
}

void MPI::Intracomm::Allgather(const void *sendbuf, int sendcount, 
			       const MPI::Datatype & sendtype, void *recvbuf, 
			       int recvcount, const MPI::Datatype & recvtype) const 
{
  pmpi_comm.Allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype);
}

void MPI::Intracomm::Allgatherv(const void *sendbuf, int sendcount, 
				const MPI::Datatype & sendtype, void *recvbuf, 
				const int recvcounts[], const int displs[],
				const MPI::Datatype & recvtype) const
{
  pmpi_comm.Allgatherv(sendbuf, sendcount, sendtype,
		       recvbuf, recvcounts, displs, recvtype);
}

void MPI::Intracomm::Alltoall(const void *sendbuf, int sendcount, 
			      const MPI::Datatype & sendtype, void *recvbuf, 
			      int recvcount, const MPI::Datatype & recvtype) const
{
  pmpi_comm.Alltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype);
}

void MPI::Intracomm::Alltoallv(const void *sendbuf, const int sendcounts[], 
			       const int sdispls[], const MPI::Datatype & sendtype, 
			       void *recvbuf, const int recvcounts[], 
			       const int rdispls[], const MPI::Datatype & recvtype) const 
{
  pmpi_comm.Alltoallv(sendbuf, sendcounts, sdispls, sendtype,
		      recvbuf, recvcounts, rdispls, recvtype);
}

void MPI::Intracomm::Alltoallw(const void *sendbuf, const int sendcounts[],
                    const int sdispls[], const MPI::Datatype sendtypes[],
                    void *recvbuf, const int recvcounts[],
                    const int rdispls[], const MPI::Datatype recvtypes[]) const
{
  pmpi_comm.Alltoallw(sendbuf, sendcounts, sdispls, 
		      (PMPI::Datatype *) sendtypes,
                      recvbuf, recvcounts, rdispls, 
		      (PMPI::Datatype *) recvtypes);
}

void MPI::Intracomm::Reduce(const void *sendbuf, void *recvbuf, int count, 
			    const MPI::Datatype & datatype, const Op & op, 
			    int root) const
{
  pmpi_comm.Reduce(sendbuf, recvbuf, count, datatype, op, root);
}

void MPI::Intracomm::Allreduce(const void *sendbuf, void *recvbuf, int count,
			       const MPI::Datatype & datatype, const Op & op) const
{
  pmpi_comm.Allreduce(sendbuf, recvbuf, count, datatype, op);
}

void MPI::Intracomm::Reduce_scatter(const void *sendbuf, void *recvbuf, 
				    int recvcounts[], 
				    const MPI::Datatype & datatype, 
				    const Op & op) const
{
  pmpi_comm.Reduce_scatter(sendbuf, recvbuf, recvcounts, datatype, op);
}

void MPI::Intracomm::Scan(const void *sendbuf, void *recvbuf, int count, 
			  const MPI::Datatype & datatype, const Op & op) const
{
  pmpi_comm.Scan(sendbuf, recvbuf, count, datatype, op);
}

void MPI::Intracomm::Exscan(const void *sendbuf, void *recvbuf, int count,
			    const MPI::Datatype & datatype, const Op & op)
                            const
{
  pmpi_comm.Exscan(sendbuf, recvbuf, count, datatype, op);
}

MPI::Intracomm MPI::Intracomm::Dup() const
{
  return pmpi_comm.Dup();
}

MPI::Intracomm& MPI::Intracomm::Clone() const
{
  return (MPI::Intracomm&)pmpi_comm.Clone();
}

MPI::Intracomm
MPI::Intracomm::Create(const MPI::Group& group) const
{
  return pmpi_comm.Create(group);
}

MPI::Intracomm
MPI::Intracomm::Split(int color, int key) const
{
  return pmpi_comm.Split(color, key);
}


MPI::Intercomm MPI::Intracomm::Create_intercomm(int local_leader,
						const MPI::Comm& peer_comm,
						int remote_leader, int tag) const
{
  return pmpi_comm.Create_intercomm(local_leader, peer_comm, remote_leader, tag);
}

MPI::Cartcomm MPI::Intracomm::Create_cart(int ndims, const int dims[],
					  const bool periods[], bool reorder) const
{
  return pmpi_comm.Create_cart(ndims, dims, periods, reorder);
}

MPI::Graphcomm MPI::Intracomm::Create_graph(int nnodes, const int index[],
					    const int edges[], bool reorder) const
{
  return pmpi_comm.Create_graph(nnodes, index, edges, reorder);
}


//
// Process Creation and Management
//

MPI::Intercomm
MPI::Intracomm::Accept(const char* port_name, const MPI::Info& info, int root)
  const
{
  return pmpi_comm.Accept(port_name, info, root);
}


MPI::Intercomm
MPI::Intracomm::Connect(const char* port_name, const MPI::Info& info, int root)
  const
{
  return pmpi_comm.Connect(port_name, info, root);
}


MPI::Intercomm 
MPI::Intracomm::Spawn(const char* command, const char* argv[],
		      int maxprocs, const MPI::Info& info, int root) const
{
  return pmpi_comm.Spawn(command, argv, maxprocs, info, root);
}


MPI::Intercomm
MPI::Intracomm::Spawn(const char* command, const char* argv[],
                      int maxprocs, const MPI::Info& info, int root,
		      int array_of_errcodes[]) const
{
  return pmpi_comm.Spawn(command, argv, maxprocs, info, root, 
			 array_of_errcodes);
}

MPI::Intercomm
MPI::Intracomm::Spawn_multiple(int count, const char* array_of_commands[],
			       const char** array_of_argv[],
			       const int array_of_maxprocs[],
			       const MPI::Info array_of_info[], int root)
{
  return pmpi_comm.Spawn_multiple(count, array_of_commands, array_of_argv,
				  array_of_maxprocs, 
				  (PMPI::Info *)array_of_info, root);
}


MPI::Intercomm
MPI::Intracomm::Spawn_multiple(int count, const char* array_of_commands[],
                               const char** array_of_argv[],
                               const int array_of_maxprocs[],
                               const MPI::Info array_of_info[], int root,
			       int array_of_errcodes[])
{
  return pmpi_comm.Spawn_multiple(count, array_of_commands, array_of_argv,
				  array_of_maxprocs, 
				  (PMPI::Info *)array_of_info, root,
				  array_of_errcodes);
}






