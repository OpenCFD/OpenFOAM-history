// -*- c++ -*-
//
// $Id: file_inln.h,v 1.1 2003/02/10 02:54:40 vsahay Exp $
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



inline void 
_REAL_MPI_::File::Close() 
{
  (void) MPI_File_close(&mpi_file);
}


inline void 
_REAL_MPI_::File::Delete(const char* filename, const _REAL_MPI_::Info& info) 
{
  (void) MPI_File_delete((char*)filename, info);
}


inline int 
_REAL_MPI_::File::Get_amode() const 
{
  int amode;
  (void) MPI_File_get_amode(mpi_file, &amode);
  return amode;
}


inline bool 
_REAL_MPI_::File::Get_atomicity() const 
{
  int flag;
  (void) MPI_File_get_atomicity(mpi_file, &flag);
  return (bool)flag;
}

inline _REAL_MPI_::Offset
_REAL_MPI_::File::Get_byte_offset(const _REAL_MPI_::Offset disp) const 
{
  MPI_Offset offset, ldisp;
  ldisp = disp;
  (void) MPI_File_get_byte_offset(mpi_file, ldisp, &offset);
  return offset;
}

inline _REAL_MPI_::Group 
_REAL_MPI_::File::Get_group() const
{
  MPI_Group group;
  (void) MPI_File_get_group(mpi_file, &group);
  return group;
}


inline _REAL_MPI_::Info 
_REAL_MPI_::File::Get_info() const
{
  MPI_Info info_used;
  (void) MPI_File_get_info(mpi_file, &info_used);
  return info_used;
}


inline _REAL_MPI_::Offset 
_REAL_MPI_::File::Get_position() const
{
  MPI_Offset offset;
  (void) MPI_File_get_position(mpi_file, &offset);
  return offset;
}


inline _REAL_MPI_::Offset 
_REAL_MPI_::File::Get_position_shared() const
{
  MPI_Offset offset;
  (void) MPI_File_get_position_shared(mpi_file, &offset);
  return offset;
}


inline _REAL_MPI_::Offset 
_REAL_MPI_::File::Get_size() const
{
  MPI_Offset offset;
  (void) MPI_File_get_size(mpi_file, &offset);
  return offset;

}


inline _REAL_MPI_::Aint 
_REAL_MPI_::File::Get_type_extent(const _REAL_MPI_::Datatype& datatype) const
{
  MPI_Aint extent;
  (void) MPI_File_get_type_extent(mpi_file, datatype, &extent);
  return extent;
}


inline void 
_REAL_MPI_::File::Get_view(_REAL_MPI_::Offset& disp,
			   _REAL_MPI_::Datatype& etype, 
			   _REAL_MPI_::Datatype& filetype,
			   char* datarep) const
{
  MPI_Datatype type, ftype;
  type = etype;
  ftype = filetype;
  _REAL_MPI_::Offset odisp = disp;

  (void) MPI_File_get_view(mpi_file, &odisp, &type, &ftype,
			   datarep);
}


inline _REAL_MPI_::Request 
_REAL_MPI_::File::Iread(void* buf, int count, 
			      const _REAL_MPI_::Datatype& datatype)
{
  MPI_Request req;
  (void) MPI_File_iread(mpi_file, buf, count, datatype, &req);
  return req;
}


inline _REAL_MPI_::Request 
_REAL_MPI_::File::Iread_at(_REAL_MPI_::Offset offset, void* buf, int count, 
				 const _REAL_MPI_::Datatype& datatype)
{
  MPI_Request req;
  (void) MPI_File_iread_at(mpi_file, offset, buf, count, datatype, &req);
  return req;
}


inline _REAL_MPI_::Request 
_REAL_MPI_::File::Iread_shared(void* buf, int count,
				     const _REAL_MPI_::Datatype& datatype)
{
  MPI_Request req;
  (void) MPI_File_iread_shared(mpi_file, buf, count, datatype, &req);
  return req;
}


inline _REAL_MPI_::Request 
_REAL_MPI_::File::Iwrite(const void* buf, int count,
			 const _REAL_MPI_::Datatype& datatype)
{
  MPI_Request req;
  (void) MPI_File_iwrite(mpi_file, (void*) buf, count, datatype, &req);
  return req;
}


inline _REAL_MPI_::Request 
_REAL_MPI_::File::Iwrite_at(_REAL_MPI_::Offset offset, const void* buf, 
			    int count, const _REAL_MPI_::Datatype& datatype)
{
  MPI_Request req;
  (void) MPI_File_iwrite_at(mpi_file, offset, (void*) buf, count, datatype,
			    &req);
  return req;
}


inline _REAL_MPI_::Request 
_REAL_MPI_::File::Iwrite_shared(const void* buf, int count,
				const _REAL_MPI_::Datatype& datatype)
{
  MPI_Request req;
  (void) MPI_File_iwrite_shared(mpi_file, (void*) buf, count, datatype, &req);
  return req;
}


inline _REAL_MPI_::File 
_REAL_MPI_::File::Open(const _REAL_MPI_::Intracomm& comm,
				 const char* filename, int amode,
				 const _REAL_MPI_::Info& info)
{
  MPI_File fh;
  (void) MPI_File_open(comm, (char*) filename, amode, info, &fh);
  return fh;
}


inline void 
_REAL_MPI_::File::Preallocate(_REAL_MPI_::Offset size)
{
  (void) MPI_File_preallocate(mpi_file, size);
}


inline void 
_REAL_MPI_::File::Read(void* buf, int count,
		       const _REAL_MPI_::Datatype& datatype)
{
  MPI_Status status;
  (void) MPI_File_read(mpi_file, buf, count, datatype, &status);
}


inline void 
_REAL_MPI_::File::Read(void* buf, int count,
		       const _REAL_MPI_::Datatype& datatype,
		       _REAL_MPI_::Status& status)
{
  (void) MPI_File_read(mpi_file, buf, count, datatype, status);
}


inline void 
_REAL_MPI_::File::Read_all(void* buf, int count,
			   const _REAL_MPI_::Datatype& datatype)
{
  MPI_Status status;
  (void) MPI_File_read_all(mpi_file, buf, count, datatype, &status);
}


inline void 
_REAL_MPI_::File::Read_all(void* buf, int count,
			   const _REAL_MPI_::Datatype& datatype,
			   _REAL_MPI_::Status& status)
{
  (void) MPI_File_read_all(mpi_file, buf, count, datatype, status);
}


inline void 
_REAL_MPI_::File::Read_all_begin(void* buf, int count,
				 const _REAL_MPI_::Datatype& datatype)
{
  (void) MPI_File_read_all_begin(mpi_file, buf, count, datatype);
}


inline void 
_REAL_MPI_::File::Read_all_end(void* buf)
{
  MPI_Status status;
  (void) MPI_File_read_all_end(mpi_file, buf, &status);
}


inline void 
_REAL_MPI_::File::Read_all_end(void* buf, _REAL_MPI_::Status& status)
{
  (void) MPI_File_read_all_end(mpi_file, buf, status);
}


inline void 
_REAL_MPI_::File::Read_at(_REAL_MPI_::Offset offset, 
			  void* buf, int count,
			  const _REAL_MPI_::Datatype& datatype)
{
  MPI_Status status;
  (void) MPI_File_read_at(mpi_file, offset, buf, count, datatype, &status);

}


inline void 
_REAL_MPI_::File::Read_at(_REAL_MPI_::Offset offset, void* buf, int count,
			  const _REAL_MPI_::Datatype& datatype,
			  _REAL_MPI_::Status& status)
{
  (void) MPI_File_read_at(mpi_file, offset, buf, count, datatype, status);
}


inline void 
_REAL_MPI_::File::Read_at_all(_REAL_MPI_::Offset offset, void* buf, int count, 
			      const _REAL_MPI_::Datatype& datatype)
{
  MPI_Status status;
  (void) MPI_File_read_at_all(mpi_file, offset, buf, count, datatype, &status);
}


inline void 
_REAL_MPI_::File::Read_at_all(_REAL_MPI_::Offset offset, void* buf, int count,
			      const _REAL_MPI_::Datatype& datatype,
			      _REAL_MPI_::Status& status)
{
  (void) MPI_File_read_at_all(mpi_file, offset, buf, count, datatype, status);
}


inline void 
_REAL_MPI_::File::Read_at_all_begin(_REAL_MPI_::Offset offset,
				    void* buf, int count,
				    const _REAL_MPI_::Datatype& datatype)
{
  (void) MPI_File_read_at_all_begin(mpi_file, offset, buf, count, datatype);
}


inline void 
_REAL_MPI_::File::Read_at_all_end(void* buf)
{
  MPI_Status status;
  (void) MPI_File_read_at_all_end(mpi_file, buf, &status);
}


inline void 
_REAL_MPI_::File::Read_at_all_end(void* buf, _REAL_MPI_::Status& status)
{
  (void) MPI_File_read_at_all_end(mpi_file, buf, status);
}


inline void 
_REAL_MPI_::File::Read_ordered(void* buf, int count,
			       const _REAL_MPI_::Datatype& datatype)
{
  MPI_Status status;
  (void) MPI_File_read_ordered(mpi_file, buf, count, datatype, &status);
}


inline void 
_REAL_MPI_::File::Read_ordered(void* buf, int count,
			       const _REAL_MPI_::Datatype& datatype,
			       _REAL_MPI_::Status& status)
{
  (void) MPI_File_read_ordered(mpi_file, buf, count, datatype, status);
}


inline void 
_REAL_MPI_::File::Read_ordered_begin(void* buf, int count,
				     const _REAL_MPI_::Datatype& datatype)
{
  (void) MPI_File_read_ordered_begin(mpi_file, buf, count, datatype);
}


inline void 
_REAL_MPI_::File::Read_ordered_end(void* buf)
{
  MPI_Status status;
  (void) MPI_File_read_ordered_end(mpi_file, buf, &status);
}


inline void 
_REAL_MPI_::File::Read_ordered_end(void* buf, _REAL_MPI_::Status& status)
{
  (void) MPI_File_read_ordered_end(mpi_file, buf, status);
}


inline void 
_REAL_MPI_::File::Read_shared(void* buf, int count,
			      const _REAL_MPI_::Datatype& datatype)
{
  MPI_Status status;
  (void) MPI_File_read_shared(mpi_file, buf, count, datatype, &status);
}


inline void 
_REAL_MPI_::File::Read_shared(void* buf, int count,
			      const _REAL_MPI_::Datatype& datatype,
			      _REAL_MPI_::Status& status)
{
  (void) MPI_File_read_shared(mpi_file, buf, count, datatype, status);
}

inline void 
_REAL_MPI_::File::Seek(_REAL_MPI_::Offset offset, int whence)
{
  (void) MPI_File_seek(mpi_file, offset, whence);
}


inline void 
_REAL_MPI_::File::Seek_shared(_REAL_MPI_::Offset offset, int whence)
{
  (void) MPI_File_seek_shared(mpi_file, offset, whence);
}


inline void 
_REAL_MPI_::File::Set_atomicity(bool flag)
{
  (void) MPI_File_set_atomicity(mpi_file, flag);
}


inline void 
_REAL_MPI_::File::Set_info(const _REAL_MPI_::Info& info)
{
  (void) MPI_File_set_info(mpi_file, info);
}


inline void 
_REAL_MPI_::File::Set_size(_REAL_MPI_::Offset size)
{
  (void) MPI_File_set_size(mpi_file, size);
}


inline void 
_REAL_MPI_::File::Set_view(_REAL_MPI_::Offset disp,
			   const _REAL_MPI_::Datatype& etype,
			   const _REAL_MPI_::Datatype& filetype,
			   const char* datarep,
			   const _REAL_MPI_::Info& info)
{
  (void) MPI_File_set_view(mpi_file, disp, etype, filetype, (char*) datarep,
			   info);
}


inline void 
_REAL_MPI_::File::Sync()
{
  (void) MPI_File_sync(mpi_file);
}


inline void 
_REAL_MPI_::File::Write(const void* buf, int count,
		      const _REAL_MPI_::Datatype& datatype)
{
  MPI_Status status;
  (void) MPI_File_write(mpi_file, (void*) buf, count, datatype, &status);
}


inline void 
_REAL_MPI_::File::Write(const void* buf, int count,
			const _REAL_MPI_::Datatype& datatype,
			_REAL_MPI_::Status& status)
{
  (void) MPI_File_write(mpi_file, (void*) buf, count, datatype, status);
}


inline void 
_REAL_MPI_::File::Write_all(const void* buf, int count,
			    const _REAL_MPI_::Datatype& datatype)
{
  MPI_Status status;
  (void) MPI_File_write_all(mpi_file, (void*) buf, count, datatype, &status);
}



inline void 
_REAL_MPI_::File::Write_all(const void* buf, int count,
			    const _REAL_MPI_::Datatype& datatype,
			    _REAL_MPI_::Status& status)
{
  (void) MPI_File_write_all(mpi_file, (void*) buf, count, datatype, status);
}


inline void 
_REAL_MPI_::File::Write_all_begin(const void* buf, int count,
				  const _REAL_MPI_::Datatype& datatype)
{
  (void) MPI_File_write_all_begin(mpi_file, (void*) buf, count, datatype);
}


inline void 
_REAL_MPI_::File::Write_all_end(const void* buf)
{
  MPI_Status status;
  (void) MPI_File_write_all_end(mpi_file, (void*) buf, &status);
}


inline void 
_REAL_MPI_::File::Write_all_end(const void* buf, _REAL_MPI_::Status& status)
{
  (void) MPI_File_write_all_end(mpi_file, (void*) buf, status);
}


inline void 
_REAL_MPI_::File::Write_at(_REAL_MPI_::Offset offset,
			   const void* buf, int count,
			   const _REAL_MPI_::Datatype& datatype)
{
  MPI_Status status;
  (void) MPI_File_write_at(mpi_file, offset, (void*) buf, count,
			   datatype, &status);
}


inline void 
_REAL_MPI_::File::Write_at(_REAL_MPI_::Offset offset,
			   const void* buf, int count,
			   const _REAL_MPI_::Datatype& datatype,
			   _REAL_MPI_::Status& status)
{
  (void) MPI_File_write_at(mpi_file, offset, (void*) buf, count,
			   datatype, status);
}


inline void 
_REAL_MPI_::File::Write_at_all(_REAL_MPI_::Offset offset,
			       const void* buf, int count,
			       const _REAL_MPI_::Datatype& datatype)
{
  MPI_Status status;
  (void) MPI_File_write_at_all(mpi_file, offset, (void*) buf, count,
			       datatype, &status);
}


inline void 
_REAL_MPI_::File::Write_at_all(_REAL_MPI_::Offset offset,
			       const void* buf, int count,
			       const _REAL_MPI_::Datatype& datatype,
			       _REAL_MPI_::Status& status)
{
  (void) MPI_File_write_at_all(mpi_file, offset, (void*) buf, count, 
			       datatype, status);
}


inline void 
_REAL_MPI_::File::Write_at_all_begin(_REAL_MPI_::Offset offset,
				     const void* buf, int count,
				     const _REAL_MPI_::Datatype& datatype)
{
  (void) MPI_File_write_at_all_begin(mpi_file, offset, (void*) buf, count,
				     datatype);
}


inline void 
_REAL_MPI_::File::Write_at_all_end(const void* buf)
{
  MPI_Status status;
  (void) MPI_File_write_at_all_end(mpi_file, (void*) buf, &status);
}


inline void 
_REAL_MPI_::File::Write_at_all_end(const void* buf, _REAL_MPI_::Status& status)
{
  (void) MPI_File_write_at_all_end(mpi_file, (void*) buf, status);
}


inline void 
_REAL_MPI_::File::Write_ordered(const void* buf, int count,
			      const _REAL_MPI_::Datatype& datatype)
{
  MPI_Status status;
  (void) MPI_File_write_ordered(mpi_file, (void*) buf, count, datatype,
				&status);
}


inline void 
_REAL_MPI_::File::Write_ordered(const void* buf, int count,
				const _REAL_MPI_::Datatype& datatype,
				_REAL_MPI_::Status& status)
{
  (void) MPI_File_write_ordered(mpi_file, (void*) buf, count, datatype,
				status);
}


inline void 
_REAL_MPI_::File::Write_ordered_begin(const void* buf, int count, 
				    const _REAL_MPI_::Datatype& datatype)
{
  (void) MPI_File_write_ordered_begin(mpi_file, (void*) buf, count, datatype);
}


inline void 
_REAL_MPI_::File::Write_ordered_end(const void* buf)
{
  MPI_Status status;
  (void) MPI_File_write_ordered_end(mpi_file, (void*) buf, &status);
}


inline void 
_REAL_MPI_::File::Write_ordered_end(const void* buf,
				    _REAL_MPI_::Status& status)
{
  (void) MPI_File_write_ordered_end(mpi_file, (void*) buf, status);
}


inline void 
_REAL_MPI_::File::Write_shared(const void* buf, int count,
			     const _REAL_MPI_::Datatype& datatype)
{
  MPI_Status status;
  (void) MPI_File_write_shared(mpi_file, (void*) buf, count,
			       datatype, &status);
}


inline void 
_REAL_MPI_::File::Write_shared(const void* buf, int count,
			     const _REAL_MPI_::Datatype& datatype, _REAL_MPI_::Status& status)
{
  (void) MPI_File_write_shared(mpi_file, (void*) buf, count,
			       datatype, status);
}


inline _REAL_MPI_::Errhandler
_REAL_MPI_::File::Get_errhandler() const
{
  MPI_Errhandler err;
  (void) MPI_File_get_errhandler(mpi_file, &err);
  return err;
}
  
inline void
_REAL_MPI_::File::Set_errhandler(const _REAL_MPI_::Errhandler& errhandler)
{
  (void) MPI_File_set_errhandler(mpi_file, errhandler);
}
