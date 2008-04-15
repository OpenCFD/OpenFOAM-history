// -*- c++ -*-
//
// $Id: comm.cc,v 1.4 2003/04/15 01:21:48 adharurk Exp $
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

void MPI::Comm::Send(const void *buf, int count, 
		     const MPI::Datatype & datatype,
		     int dest, int tag) const
{
  pmpi_comm.Send(buf, count, datatype, dest, tag);
}

void MPI::Comm::Recv(void *buf, int count, const MPI::Datatype & datatype,
		     int source, int tag, MPI::Status & status) const
{
  //JGS  PMPI::Status pmpistat = status;
  pmpi_comm.Recv(buf, count, datatype, source, tag, status.pmpi_status);
  //  status = pmpistat;
}

void MPI::Comm::Recv(void *buf, int count, const MPI::Datatype & datatype,
		     int source, int tag) const
{
  pmpi_comm.Recv(buf, count, datatype, source, tag);
}

void MPI::Comm::Bsend(const void *buf, int count,
		      const MPI::Datatype & datatype, int dest, int tag) const
{
  pmpi_comm.Bsend(buf, count, datatype, dest, tag);
}

void MPI::Comm::Ssend(const void *buf, int count, 
		      const MPI::Datatype & datatype, int dest, int tag) const 
{
  pmpi_comm.Ssend(buf, count, datatype, dest, tag);
}

void MPI::Comm::Rsend(const void *buf, int count,
		      const MPI::Datatype & datatype, int dest, int tag) const
{
  pmpi_comm.Rsend(buf, count, datatype, dest, tag);
}

MPI::Request MPI::Comm::Isend(const void *buf, int count,
			 const MPI::Datatype & datatype, int dest, int tag) const
{
  return pmpi_comm.Isend(buf, count, datatype, dest, tag);
}

MPI::Request MPI::Comm::Ibsend(const void *buf, int count, const
			  MPI::Datatype & datatype, int dest, int tag) const
{
  return pmpi_comm.Ibsend(buf, count, datatype, dest, tag);
}

MPI::Request MPI::Comm::Issend(const void *buf, int count,
			  const MPI::Datatype & datatype, int dest, int tag) const
{
  return pmpi_comm.Issend(buf, count, datatype, dest, tag);
}

MPI::Request MPI::Comm::Irsend(const void *buf, int count,
			  const MPI::Datatype & datatype, int dest, int tag) const
{
  return pmpi_comm.Irsend(buf, count, datatype, dest, tag);
}

MPI::Request MPI::Comm::Irecv(void *buf, int count,
			 const MPI::Datatype & datatype, int source, int tag) const
{
  return pmpi_comm.Irecv(buf, count, datatype, source, tag);
}

bool MPI::Comm::Iprobe(int source, int tag, MPI::Status & status) const
{
  //JGS  PMPI::Status pmpistat = status;
  return pmpi_comm.Iprobe(source, tag, status.pmpi_status);
  //  status = pmpistat;
}

bool MPI::Comm::Iprobe(int source, int tag) const
{
  return pmpi_comm.Iprobe(source, tag);
}

void MPI::Comm::Probe(int source, int tag, MPI::Status & status) const
{
  //JGS  PMPI::Status pmpistat = status;  
  pmpi_comm.Probe(source, tag, status.pmpi_status);
  //  status = pmpistat;
}

void MPI::Comm::Probe(int source, int tag) const
{
  pmpi_comm.Probe(source, tag);
}

MPI::Prequest MPI::Comm::Send_init(const void *buf, int count,
			      const Datatype & datatype, int dest, int tag) const
{ 
  return pmpi_comm.Send_init(buf, count, datatype, dest, tag);
}

MPI::Prequest MPI::Comm::Bsend_init(const void *buf, int count,
			       const MPI::Datatype & datatype, int dest, int tag) const
{
  return pmpi_comm.Bsend_init(buf, count, datatype, dest, tag);  
}

MPI::Prequest MPI::Comm::Ssend_init(const void *buf, int count,
			       const MPI::Datatype & datatype, int dest, int tag) const
{
  return pmpi_comm.Ssend_init(buf, count, datatype, dest, tag);
}

MPI::Prequest MPI::Comm::Rsend_init(const void *buf, int count,
			       const MPI::Datatype & datatype, int dest, int tag) const
{
  return pmpi_comm.Rsend_init(buf, count, datatype, dest, tag);
}

MPI::Prequest MPI::Comm::Recv_init(void *buf, int count,
			      const Datatype & datatype, int source, int tag) const
{
  return pmpi_comm.Recv_init(buf, count, datatype, source, tag);
}

void MPI::Comm::Sendrecv(const void *sendbuf, int sendcount,
			 const MPI::Datatype & sendtype, int dest, int sendtag, 
			 void *recvbuf, int recvcount, 
			 const MPI::Datatype & recvtype, int source,
			 int recvtag, MPI::Status & status) const
{
  //JGS  PMPI::Status pmpistat = status;
  pmpi_comm.Sendrecv(sendbuf, sendcount, sendtype, dest, sendtag, recvbuf, recvcount,
		     recvtype, source, recvtag, status.pmpi_status);
  //  status = pmpistat;
}

void MPI::Comm::Sendrecv(const void *sendbuf, int sendcount,
			 const MPI::Datatype & sendtype, int dest, int sendtag, 
			 void *recvbuf, int recvcount, 
			 const MPI::Datatype & recvtype, int source,
			 int recvtag) const
{
  pmpi_comm.Sendrecv(sendbuf, sendcount, sendtype, dest, sendtag, recvbuf, recvcount,
		     recvtype, source, recvtag);
}

void MPI::Comm::Sendrecv_replace(void *buf, int count,
				 const MPI::Datatype & datatype, int dest, 
				 int sendtag, int source,
				 int recvtag, MPI::Status & status) const 
{
  //JGS  PMPI::Status pmpistatus = status;
  pmpi_comm.Sendrecv_replace(buf, count, datatype, dest, sendtag, source,
			     recvtag, status.pmpi_status);
  //  status = pmpistatus;
}

void MPI::Comm::Sendrecv_replace(void *buf, int count,
				 const MPI::Datatype & datatype, int dest, 
				 int sendtag, int source,
				 int recvtag) const 
{
  pmpi_comm.Sendrecv_replace(buf, count, datatype, dest, sendtag, source, recvtag);
}

//
// Groups, Contexts, and Communicators
//

MPI::Group MPI::Comm::Get_group() const 
{
  return pmpi_comm.Get_group();
}

int MPI::Comm::Get_size() const 
{
  return pmpi_comm.Get_size();
}

int MPI::Comm::Get_rank() const 
{
  return pmpi_comm.Get_rank();
}
  
int MPI::Comm::Compare(const MPI::Comm & comm1, const MPI::Comm & comm2)
{
  return PMPI::Comm::Compare(comm1.pmpi_comm, comm2.pmpi_comm);
}

void MPI::Comm::Free(void)
{
  pmpi_comm.Free();
}

bool MPI::Comm::Is_inter() const
{
  return pmpi_comm.Is_inter();
}


//
// Process Creation and Management
//

void
MPI::Comm::Disconnect()
{
  pmpi_comm.Disconnect();
}

MPI::Intercomm
MPI::Comm::Get_parent()
{
  return PMPI::Comm::Get_parent();
}

MPI::Intercomm
MPI::Comm::Join(const int fd) 
{
  return PMPI::Comm::Join(fd);
}

//
// External Interfaces
//

void MPI::Comm::Get_name(char* comm_name, int& resultlen) const
{
  pmpi_comm.Get_name(comm_name, resultlen);
}

void MPI::Comm::Set_name(const char* comm_name)
{
  pmpi_comm.Set_name(comm_name);
}

//
//Process Topologies
//

int MPI::Comm::Get_topology() const 
{
  return pmpi_comm.Get_topology();
}

//
// Environmental Inquiry
//

void MPI::Comm::Abort(int errorcode) 
{
  pmpi_comm.Abort(errorcode);
}

void
MPI::Comm::Set_errhandler(const MPI::Errhandler& errhandler)
{
  pmpi_comm.Set_errhandler(errhandler);
}

MPI::Errhandler
MPI::Comm::Get_errhandler() const
{
  return pmpi_comm.Get_errhandler();
}

MPI::Errhandler
MPI::Comm::Create_errhandler(MPI::Comm::Errhandler_fn* function)
{
  return PMPI::Comm::Create_errhandler((PMPI::Comm::_MPI2CPP_ERRHANDLERFN_*)function);
}


//JGS removed const from arguments, see header
int
MPI::Comm::Create_keyval(MPI::Comm::Copy_attr_function* comm_copy_attr_fn,
			 MPI::Comm::Delete_attr_function* comm_delete_attr_fn,
			 void* extra_state)
{
  return PMPI::Comm::Create_keyval((PMPI::Comm::_MPI2CPP_COPYATTRFN_*)comm_copy_attr_fn, 
				   (PMPI::Comm::_MPI2CPP_DELETEATTRFN_*)comm_delete_attr_fn, extra_state);
  
}

void
MPI::Comm::Free_keyval(int& comm_keyval)
{
  PMPI::Comm::Free_keyval(comm_keyval);
}

void
MPI::Comm::Set_attr(int comm_keyval, const void* attribute_val) const
{
  pmpi_comm.Set_attr(comm_keyval, attribute_val);
}

bool
MPI::Comm::Get_attr(int comm_keyval, void* attribute_val) const
{
  return pmpi_comm.Get_attr(comm_keyval, attribute_val);
}

void
MPI::Comm::Delete_attr(int comm_keyval)
{
  pmpi_comm.Delete_attr(comm_keyval);
}

int
MPI::Comm::NULL_COPY_FN(const MPI::Comm& oldcomm, int comm_keyval,
			       void* extra_state, void* attribute_val_in,
			       void* attribute_val_out, bool& flag)
{ 
  if (PMPI::Comm::NULL_COPY_FN != 0)
    return PMPI::Comm::NULL_COPY_FN(oldcomm.pmpi_comm, comm_keyval, 
				    extra_state, attribute_val_in,
				    attribute_val_out, flag);
  else
    return MPI_SUCCESS;
}

int
MPI::Comm::DUP_FN(const MPI::Comm& oldcomm, int comm_keyval,
			 void* extra_state, void* attribute_val_in,
			 void* attribute_val_out, bool& flag)
{
  return PMPI::Comm::DUP_FN(oldcomm.pmpi_comm, comm_keyval, extra_state, attribute_val_in,
			    attribute_val_out, flag);
}

int
MPI::Comm::NULL_DELETE_FN(MPI::Comm& comm, int comm_keyval, void* attribute_val,
				 void* extra_state)
{
  if (PMPI::Comm::NULL_DELETE_FN != 0)
    return PMPI::Comm::NULL_DELETE_FN(comm.pmpi_comm, comm_keyval, 
				      attribute_val, extra_state);
  else
    return MPI_SUCCESS;
}


