// -*- c++ -*-
//
// $Id: pfile.h,v 1.1 2003/02/10 02:54:40 vsahay Exp $
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

class File {

  friend class PComm; //so I can access pmpi_file data member in comm.cc
  friend class PRequest; //and also from request.cc

public:

  inline File() { }
  // copy
  File(const File& data) : mpi_file(data.mpi_file) { }

  File(const MPI_File &i) : mpi_file(i) { }
	
  inline virtual ~File() { }

  File& operator=(const File& data) {
    mpi_file = data.mpi_file; return *this; }

  // comparison, don't need for file

  // inter-language operability
  inline File& operator= (const MPI_File &i) {
    mpi_file = i; return *this; }
  inline operator MPI_File () const { return mpi_file; }
  //  operator MPI_File* () const { return (MPI_File*)&mpi_file; }


  
  inline virtual void Close();

  static void Delete(const char* filename, const Info& info);

  inline virtual int Get_amode() const;

  inline virtual bool Get_atomicity() const;

  inline virtual Offset Get_byte_offset(const Offset disp) const;

  inline virtual Group Get_group() const;

  inline virtual Info Get_info() const;

  inline virtual Offset Get_position() const;

  inline virtual Offset Get_position_shared() const;

  inline virtual Offset Get_size() const;

  inline virtual Aint Get_type_extent(const Datatype& datatype) const;

  inline virtual void Get_view(Offset& disp, Datatype& etype, 
			       Datatype& filetype, char* datarep) const;

  inline virtual Request Iread(void* buf, int count, 
  			      const Datatype& datatype);

  inline virtual Request Iread_at(Offset offset, void* buf, int count, 
				  const Datatype& datatype);

  inline virtual Request Iread_shared(void* buf, int count,
				      const Datatype& datatype);

  inline virtual Request Iwrite(const void* buf, int count,
				const Datatype& datatype);

  inline virtual Request Iwrite_at(Offset offset, const void* buf, 
				   int count, const Datatype& datatype);

  inline virtual Request Iwrite_shared(const void* buf, int count,
				       const Datatype& datatype);

  static File Open(const Intracomm& comm,
		   const char* filename, int amode,
		   const Info& info);

  inline virtual void Preallocate(Offset size);

  inline virtual void Read(void* buf, int count, const Datatype& datatype);

  inline virtual void Read(void* buf, int count, const Datatype& datatype,
			   Status& status);

  inline virtual void Read_all(void* buf, int count, const Datatype& datatype);
  
  inline virtual void Read_all(void* buf, int count, const Datatype& datatype,
			       Status& status);
  
  inline virtual void Read_all_begin(void* buf, int count,
				     const Datatype& datatype);

  inline virtual void Read_all_end(void* buf);

  inline virtual void Read_all_end(void* buf, Status& status);

  inline virtual void Read_at(Offset offset, 
			      void* buf, int count, const Datatype& datatype);

  inline virtual void Read_at(Offset offset, void* buf, int count,
			      const Datatype& datatype, Status& status);

  inline virtual void Read_at_all(Offset offset, void* buf, int count, 
				  const Datatype& datatype);

  inline virtual void Read_at_all(Offset offset, void* buf, int count,
				  const Datatype& datatype, Status& status);

  inline virtual void Read_at_all_begin(Offset offset, void* buf, int count,
					const Datatype& datatype);

  inline virtual void Read_at_all_end(void* buf);

  inline virtual void Read_at_all_end(void* buf, Status& status);

  inline virtual void Read_ordered(void* buf, int count,
				   const Datatype& datatype);

  inline virtual void Read_ordered(void* buf, int count,
				   const Datatype& datatype,
				   Status& status);
  
  inline virtual void Read_ordered_begin(void* buf, int count,
  				   const Datatype& datatype);

  inline virtual void Read_ordered_end(void* buf);
  
  inline virtual void Read_ordered_end(void* buf, Status& status);

  inline virtual void Read_shared(void* buf, int count,
				  const Datatype& datatype);

  inline virtual void Read_shared(void* buf, int count,
				  const Datatype& datatype, Status& status);

  inline virtual void Seek(Offset offset, int whence);
  
  inline virtual void Seek_shared(Offset offset, int whence);

  inline virtual void Set_atomicity(bool flag);

  inline virtual void Set_info(const Info& info);

  inline virtual void Set_size(Offset size);

  inline virtual void Set_view(Offset disp, const Datatype& etype,
			       const Datatype& filetype, const char* datarep,
			       const Info& info);
  
  inline virtual void Sync();

  inline virtual void Write(const void* buf, int count,
			    const Datatype& datatype);

  inline virtual void Write(const void* buf, int count,
			    const Datatype& datatype, Status& status);

  inline virtual void Write_all(const void* buf, int count,
				const Datatype& datatype);

  inline virtual void Write_all(const void* buf, int count,
				const Datatype& datatype, Status& status);

  inline virtual void Write_all_begin(const void* buf, int count,
  				const Datatype& datatype);

  inline virtual void Write_all_end(const void* buf);

  inline virtual void Write_all_end(const void* buf, Status& status);

  inline virtual void Write_at(Offset offset, const void* buf, int count,
			       const Datatype& datatype);
  
  inline virtual void Write_at(Offset offset, const void* buf, int count,
			       const Datatype& datatype, Status& status);

  inline virtual void Write_at_all(Offset offset, const void* buf, int count,
				   const Datatype& datatype);

  inline virtual void Write_at_all(Offset offset, const void* buf, int count,
				   const Datatype& datatype,
				   Status& status);

  inline virtual void Write_at_all_begin(Offset offset, const void* buf,
					 int count, const Datatype& datatype);

  inline virtual void Write_at_all_end(const void* buf);

  inline virtual void Write_at_all_end(const void* buf, Status& status);

  inline virtual void Write_ordered(const void* buf, int count,
				    const Datatype& datatype);

  inline virtual void Write_ordered(const void* buf, int count,
				    const Datatype& datatype, Status& status);

  inline virtual void Write_ordered_begin(const void* buf, int count, 
					  const Datatype& datatype);

  inline virtual void Write_ordered_end(const void* buf);

  inline virtual void Write_ordered_end(const void* buf, Status& status);

  inline virtual void Write_shared(const void* buf, int count,
				   const Datatype& datatype);

  inline virtual void Write_shared(const void* buf, int count,
				   const Datatype& datatype, Status& status);

  inline virtual Errhandler Get_errhandler() const;

  inline virtual void Set_errhandler(const Errhandler& errhandler);

    
protected:
  MPI_File mpi_file;

};

