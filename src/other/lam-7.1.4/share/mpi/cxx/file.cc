// -*- c++ -*-
//
// $Id: file.cc,v 1.1 2003/02/10 02:58:02 vsahay Exp $
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

void 
MPI::File::Close() 
{
  pmpi_file.Close();
}


void 
MPI::File::Delete(const char* filename, const MPI::Info& info) 
{
  PMPI::File::Delete(filename, info);
}


int 
MPI::File::Get_amode() const
{
  return pmpi_file.Get_amode();
}


bool 
MPI::File::Get_atomicity() const 
{
  return pmpi_file.Get_atomicity();
}

MPI::Offset
MPI::File::Get_byte_offset(const MPI::Offset disp) const 
{
  return pmpi_file.Get_byte_offset(disp);
}

MPI::Group 
MPI::File::Get_group() const
{
  return pmpi_file.Get_group();
}


MPI::Info 
MPI::File::Get_info() const
{
  return (MPI::Info) pmpi_file.Get_info();
}


MPI::Offset 
MPI::File::Get_position() const
{
  return pmpi_file.Get_position();
}


MPI::Offset 
MPI::File::Get_position_shared() const
{
  return pmpi_file.Get_position_shared();
}


MPI::Offset 
MPI::File::Get_size() const
{
  return pmpi_file.Get_size();
}


MPI::Aint 
MPI::File::Get_type_extent(const MPI::Datatype& datatype) const
{
  return pmpi_file.Get_type_extent(datatype);
}

void 
MPI::File::Get_view(MPI::Offset& disp, MPI::Datatype& etype, 
		    MPI::Datatype& filetype, char* datarep) const
{
  pmpi_file.Get_view(disp, (PMPI::Datatype&) etype,
		     (PMPI::Datatype&) filetype, datarep);
}


MPI::Request 
MPI::File::Iread(void* buf, int count, 
		 const MPI::Datatype& datatype)
{
  return pmpi_file.Iread(buf, count, datatype);
}


MPI::Request 
MPI::File::Iread_at(MPI::Offset offset, void* buf, int count, 
				 const MPI::Datatype& datatype)
{
  return pmpi_file.Iread_at(offset, buf, count, datatype);
}


MPI::Request 
MPI::File::Iread_shared(void* buf, int count,
				     const MPI::Datatype& datatype)
{
  return pmpi_file.Iread_shared(buf, count, datatype);
}


MPI::Request 
MPI::File::Iwrite(const void* buf, int count,
			       const MPI::Datatype& datatype)
{
  return pmpi_file.Iwrite(buf, count, datatype);
}


MPI::Request 
MPI::File::Iwrite_at(MPI::Offset offset, const void* buf, 
				  int count, const MPI::Datatype& datatype)
{
  return pmpi_file.Iwrite_at(offset, buf, count, datatype);
}


MPI::Request 
MPI::File::Iwrite_shared(const void* buf, int count,
			 const MPI::Datatype& datatype)
{
  return pmpi_file.Iwrite_shared(buf, count, datatype);
}


MPI::File 
MPI::File::Open(const MPI::Intracomm& comm,
		const char* filename, int amode,
		const MPI::Info& info)
{
  return (MPI::File) PMPI::File::Open(comm.pmpi_comm, filename, amode, info);
}


void 
MPI::File::Preallocate(MPI::Offset size)
{
  pmpi_file.Preallocate(size);
}


void 
MPI::File::Read(void* buf, int count, const MPI::Datatype& datatype)
{
  pmpi_file.Read(buf, count, datatype);
}


void 
MPI::File::Read(void* buf, int count, const MPI::Datatype& datatype,
		MPI::Status& status)
{
  pmpi_file.Read(buf, count, datatype, (PMPI::Status&) status);
}


void 
MPI::File::Read_all(void* buf, int count, const MPI::Datatype& datatype)
{
  pmpi_file.Read_all(buf, count, datatype);
}


void 
MPI::File::Read_all(void* buf, int count, const MPI::Datatype& datatype,
			 MPI::Status& status)
{
  pmpi_file.Read_all(buf, count, datatype, (PMPI::Status&) status);
}


void 
MPI::File::Read_all_begin(void* buf, int count,
			       const MPI::Datatype& datatype)
{
  pmpi_file.Read_all_begin(buf, count, datatype);
}


void 
MPI::File::Read_all_end(void* buf)
{
  pmpi_file.Read_all_end(buf);
}


void 
MPI::File::Read_all_end(void* buf, MPI::Status& status)
{
  pmpi_file.Read_all_end(buf, (PMPI::Status&) status);
}


void 
MPI::File::Read_at(MPI::Offset offset, 
			void* buf, int count, const MPI::Datatype& datatype)
{
  pmpi_file.Read_at(offset, buf, count, datatype);
}


void 
MPI::File::Read_at(MPI::Offset offset, void* buf, int count,
			const MPI::Datatype& datatype, MPI::Status& status)
{
  pmpi_file.Read_at(offset, buf, count, datatype, (PMPI::Status&) status);
}


void 
MPI::File::Read_at_all(MPI::Offset offset, void* buf, int count, 
			    const MPI::Datatype& datatype)
{
  pmpi_file.Read_at_all(offset, buf, count, datatype);
}


void 
MPI::File::Read_at_all(MPI::Offset offset, void* buf, int count,
			    const MPI::Datatype& datatype, MPI::Status& status)
{
  pmpi_file.Read_at_all(offset, buf, count, datatype, (PMPI::Status&) status);
}


void 
MPI::File::Read_at_all_begin(MPI::Offset offset, void* buf, int count,
				  const MPI::Datatype& datatype)
{
  pmpi_file.Read_at_all_begin(offset, buf, count, datatype);
}


void 
MPI::File::Read_at_all_end(void* buf)
{
  pmpi_file.Read_at_all_end(buf);
}


void 
MPI::File::Read_at_all_end(void* buf, MPI::Status& status)
{
  pmpi_file.Read_at_all_end(buf, (PMPI::Status&) status);
}


void 
MPI::File::Read_ordered(void* buf, int count,
			     const MPI::Datatype& datatype)
{
  pmpi_file.Read_ordered(buf, count, datatype);
}


void 
MPI::File::Read_ordered(void* buf, int count,
			     const MPI::Datatype& datatype,
			     MPI::Status& status)
{
  pmpi_file.Read_ordered(buf, count, datatype, (PMPI::Status&) status);
}


void 
MPI::File::Read_ordered_begin(void* buf, int count,
				   const MPI::Datatype& datatype)
{
  pmpi_file.Read_ordered_begin(buf, count, datatype);
}


void 
MPI::File::Read_ordered_end(void* buf)
{
  pmpi_file.Read_ordered_end(buf);
}


void 
MPI::File::Read_ordered_end(void* buf, MPI::Status& status)
{
  pmpi_file.Read_ordered_end(buf, (PMPI::Status&) status);
}


void 
MPI::File::Read_shared(void* buf, int count,
			    const MPI::Datatype& datatype)
{
  pmpi_file.Read_shared(buf, count, datatype);
}


void 
MPI::File::Read_shared(void* buf, int count,
			    const MPI::Datatype& datatype, MPI::Status& status)
{
  pmpi_file.Read_shared(buf, count, datatype, (PMPI::Status&) status);
}

void 
MPI::File::Seek(MPI::Offset offset, int whence)
{
  pmpi_file.Seek(offset, whence);
}


void 
MPI::File::Seek_shared(MPI::Offset offset, int whence)
{
  pmpi_file.Seek_shared(offset, whence);
}


void 
MPI::File::Set_atomicity(bool flag)
{
  pmpi_file.Set_atomicity(flag);
}


void 
MPI::File::Set_info(const MPI::Info& info)
{
  pmpi_file.Set_info(info);
}


void 
MPI::File::Set_size(MPI::Offset size)
{
  pmpi_file.Set_size(size);
}


void 
MPI::File::Set_view(MPI::Offset disp, const MPI::Datatype& etype,
			 const MPI::Datatype& filetype, const char* datarep,
			 const MPI::Info& info)
{
  pmpi_file.Set_view(disp, etype, filetype, datarep, info);
}


void 
MPI::File::Sync()
{
  pmpi_file.Sync();
}


void 
MPI::File::Write(const void* buf, int count,
		      const MPI::Datatype& datatype)
{
  pmpi_file.Write(buf, count, datatype);
}


void 
MPI::File::Write(const void* buf, int count,
		      const MPI::Datatype& datatype, MPI::Status& status)
{
  pmpi_file.Write(buf, count, datatype, (PMPI::Status&) status);
}


void 
MPI::File::Write_all(const void* buf, int count,
			  const MPI::Datatype& datatype)
{
  pmpi_file.Write_all(buf, count, datatype);
}



void 
MPI::File::Write_all(const void* buf, int count,
			  const MPI::Datatype& datatype, MPI::Status& status)
{
  pmpi_file.Write_all(buf, count, datatype, (PMPI::Status&) status);
}


void 
MPI::File::Write_all_begin(const void* buf, int count,
				const MPI::Datatype& datatype)
{
  pmpi_file.Write_all_begin(buf, count, datatype);
}


void 
MPI::File::Write_all_end(const void* buf)
{
  pmpi_file.Write_all_end(buf);
}


void 
MPI::File::Write_all_end(const void* buf, MPI::Status& status)
{
  pmpi_file.Write_all_end(buf, (PMPI::Status&) status);
}


void 
MPI::File::Write_at(MPI::Offset offset, const void* buf, int count,
			 const MPI::Datatype& datatype)
{
  pmpi_file.Write_at(offset, buf, count, datatype);
}


void 
MPI::File::Write_at(MPI::Offset offset, const void* buf, int count,
			 const MPI::Datatype& datatype, MPI::Status& status)
{
  pmpi_file.Write_at(offset, buf, count, datatype, (PMPI::Status&) status);
}


void 
MPI::File::Write_at_all(MPI::Offset offset, const void* buf, int count,
			     const MPI::Datatype& datatype)
{
  pmpi_file.Write_at_all(offset, buf, count, datatype);
}


void 
MPI::File::Write_at_all(MPI::Offset offset, const void* buf, int count,
			     const MPI::Datatype& datatype,
			     MPI::Status& status)
{
  pmpi_file.Write_at_all(offset, buf, count, datatype, (PMPI::Status&) status);
}


void 
MPI::File::Write_at_all_begin(MPI::Offset offset, const void* buf,
				   int count, const MPI::Datatype& datatype)
{
  pmpi_file.Write_at_all_begin(offset, buf, count, datatype);
}


void 
MPI::File::Write_at_all_end(const void* buf)
{
  pmpi_file.Write_at_all_end(buf);
}


void 
MPI::File::Write_at_all_end(const void* buf, MPI::Status& status)
{
  pmpi_file.Write_at_all_end(buf, (PMPI::Status&) status);
}


void 
MPI::File::Write_ordered(const void* buf, int count,
			      const MPI::Datatype& datatype)
{
  pmpi_file.Write_ordered(buf, count, datatype);
}


void 
MPI::File::Write_ordered(const void* buf, int count,
			      const MPI::Datatype& datatype, MPI::Status& status)
{
  pmpi_file.Write_ordered(buf, count, datatype, (PMPI::Status&) status);
}


void 
MPI::File::Write_ordered_begin(const void* buf, int count, 
				    const MPI::Datatype& datatype)
{
  pmpi_file.Write_ordered_begin(buf, count, datatype);
}


void 
MPI::File::Write_ordered_end(const void* buf)
{
  pmpi_file.Write_ordered_end(buf);
}


void 
MPI::File::Write_ordered_end(const void* buf, MPI::Status& status)
{
  pmpi_file.Write_ordered_end(buf, (PMPI::Status&) status);
}


void 
MPI::File::Write_shared(const void* buf, int count,
			     const MPI::Datatype& datatype)
{
  pmpi_file.Write_shared(buf, count, datatype);
}


void 
MPI::File::Write_shared(const void* buf, int count,
			     const MPI::Datatype& datatype, MPI::Status& status)
{
  pmpi_file.Write_shared(buf, count, datatype, (PMPI::Status&) status);
}


MPI::Errhandler
MPI::File::Get_errhandler() const
{
  return pmpi_file.Get_errhandler();
}

void
MPI::File::Set_errhandler(const MPI::Errhandler& errhandler)
{
  pmpi_file.Set_errhandler(errhandler);
}




