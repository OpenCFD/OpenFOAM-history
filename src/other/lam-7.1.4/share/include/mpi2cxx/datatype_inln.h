// -*- c++ -*-
//
// $Id: datatype_inln.h,v 1.4 2003/07/05 11:59:27 jsquyres Exp $
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


//
// Point-to-Point Communication
//

inline _REAL_MPI_::Datatype
_REAL_MPI_::Datatype::Create_contiguous(int count) const
{
  MPI_Datatype newtype;
  (void)MPI_Type_contiguous(count, mpi_datatype, &newtype);
  return newtype;
}

inline _REAL_MPI_::Datatype
_REAL_MPI_::Datatype::Create_vector(int count, int blocklength,
			     int stride) const
{
  MPI_Datatype newtype;
  (void)MPI_Type_vector(count, blocklength, stride, mpi_datatype, &newtype);
  return newtype;
}

inline _REAL_MPI_::Datatype
_REAL_MPI_::Datatype::Create_indexed(int count,
				     const int array_of_blocklengths[], 
				     const int array_of_displacements[]) const
{
  MPI_Datatype newtype;
  (void)MPI_Type_indexed(count, (int *) array_of_blocklengths, 
			 (int *) array_of_displacements, mpi_datatype, &newtype);
  return newtype;
}

inline _REAL_MPI_::Datatype
_REAL_MPI_::Datatype::Create_struct(int count, const int array_of_blocklengths[],
				    const _REAL_MPI_::Aint array_of_displacements[],
				    const _REAL_MPI_::Datatype array_of_types[])
{
  MPI_Datatype newtype;
  int i;
  MPI_Datatype* type_array = new MPI_Datatype[count];
  for (i=0; i < count; i++)
    type_array[i] = array_of_types[i];

  (void)MPI_Type_create_struct(count, (int*)array_of_blocklengths,
                               (MPI_Aint*)array_of_displacements, 
                               type_array, &newtype);
  delete[] type_array;
  return newtype;
}

inline _REAL_MPI_::Datatype
_REAL_MPI_::Datatype::Create_hindexed(int count, const int array_of_blocklengths[],
				      const _REAL_MPI_::Aint array_of_displacements[]) const
{
  MPI_Datatype newtype;
  (void)MPI_Type_create_hindexed(count, (int*)array_of_blocklengths,
                                 (MPI_Aint*)array_of_displacements,
                                 mpi_datatype, &newtype) ;
  return newtype;
}

inline _REAL_MPI_::Datatype
_REAL_MPI_::Datatype::Create_hvector(int count, int blocklength,
				     _REAL_MPI_::Aint stride) const
{
  MPI_Datatype newtype;
  (void)MPI_Type_create_hvector(count, blocklength, (MPI_Aint)stride,
                                mpi_datatype, &newtype);

  return newtype;
}

inline int
_REAL_MPI_::Datatype::Get_size() const 
{
  int size;
  (void)MPI_Type_size(mpi_datatype, &size);
  return size;
}

inline void
_REAL_MPI_::Datatype::Get_extent(_REAL_MPI_::Aint& lb, _REAL_MPI_::Aint& extent) const
{
  (void)MPI_Type_get_extent(mpi_datatype, &lb, &extent); 
}

inline void
_REAL_MPI_::Datatype::Commit() 
{
  (void)MPI_Type_commit(&mpi_datatype);
}

inline void
_REAL_MPI_::Datatype::Free()
{
  (void)MPI_Type_free(&mpi_datatype);
}

inline void
_REAL_MPI_::Datatype::Pack(const void* inbuf, int incount,
			   void *outbuf, int outsize,
			   int& position, const _REAL_MPI_::Comm &comm) const
{
  (void)MPI_Pack((void *) inbuf, incount,  mpi_datatype, outbuf,
		 outsize, &position, comm);
}

inline void
_REAL_MPI_::Datatype::Unpack(const void* inbuf, int insize,
			     void *outbuf, int outcount, int& position,
			     const _REAL_MPI_::Comm& comm) const 
{
  (void)MPI_Unpack((void *) inbuf, insize, &position,
		   outbuf, outcount, mpi_datatype, comm);
}

inline int
_REAL_MPI_::Datatype::Pack_size(int incount, const _REAL_MPI_::Comm& comm) const 
{
  int size;
  (void)MPI_Pack_size(incount, mpi_datatype, comm, &size);
  return size;
}


//
// Miscalleny
//

inline _REAL_MPI_::Datatype
_REAL_MPI_::Datatype::Create_subarray(int ndims, const int array_of_sizes[],
				      const int array_of_subsizes[],
				      const int array_of_starts[], int order)
  const
{
  MPI_Datatype type;
  (void) MPI_Type_create_subarray(ndims, (int *) array_of_sizes, 
			   (int *) array_of_subsizes, (int *) array_of_starts,
			   order, mpi_datatype, &type);
  return type;
}


//
// External Interfaces
//


inline _REAL_MPI_::Datatype
_REAL_MPI_::Datatype::Dup() const
{
  MPI_Datatype type;
  (void) MPI_Type_dup(mpi_datatype, &type);
  return type;
}


inline int
_REAL_MPI_::Datatype::Create_keyval(_REAL_MPI_::Datatype::Copy_attr_function*
                               type_copy_attr_fn,
                               _REAL_MPI_::Datatype::Delete_attr_function*
                               type_delete_attr_fn, void* extra_state)
{
  int key;
  (void) MPI_Type_create_keyval((MPI_Type_copy_attr_function *)
				type_copy_attr_fn,
				(MPI_Type_delete_attr_function *)
				type_delete_attr_fn, &key, extra_state);
  return key;
}


inline void
_REAL_MPI_::Datatype::Delete_attr(int type_keyval)
{
  (void) MPI_Type_delete_attr(mpi_datatype, type_keyval);
}


inline void
_REAL_MPI_::Datatype::Free_keyval(int& type_keyval)
{
  (void) MPI_Type_free_keyval(&type_keyval);
}


inline bool
_REAL_MPI_::Datatype::Get_attr(int type_keyval,
                          void* attribute_val) const
{
  int ret;
  (void) MPI_Type_get_attr(mpi_datatype, type_keyval, attribute_val, &ret);
  return (bool) ret;
}


inline void
_REAL_MPI_::Datatype::Get_contents(int max_integers, int max_addresses,
				   int max_datatypes, int array_of_integers[],
				   _REAL_MPI_::Aint array_of_addresses[],
				   _REAL_MPI_::Datatype array_of_datatypes[])
  const
{
  (void) MPI_Type_get_contents(mpi_datatype, max_integers, max_addresses,
			       max_datatypes, (int *)array_of_integers, 
			       (MPI_Aint*) array_of_addresses,
			       (MPI_Datatype *) array_of_datatypes);
}

inline void
_REAL_MPI_::Datatype::Get_envelope(int& num_integers, int& num_addresses,
			  int& num_datatypes, int& combiner) const
{
  (void) MPI_Type_get_envelope(mpi_datatype, &num_integers, &num_addresses,
				&num_datatypes, &combiner);
}

inline void
_REAL_MPI_::Datatype::Get_name(char* type_name, int& resultlen) const
{
  (void) MPI_Type_get_name(mpi_datatype, type_name, &resultlen);
}


inline void
_REAL_MPI_::Datatype::Set_attr(int type_keyval, const void* attribute_val)
{
  (void) MPI_Type_set_attr(mpi_datatype, type_keyval, (void *) attribute_val);
}


inline void
_REAL_MPI_::Datatype::Set_name(const char* type_name)
{
  (void) MPI_Type_set_name(mpi_datatype, (char *)type_name);
}


#if 0
//
// User Defined Functions
//

typedef int _REAL_MPI_::Datatype::Copy_attr_function(const Datatype& oldtype,
						     int type_keyval,
						     void* extra_state,
						     void* attribute_val_in,
						     void* attribute_val_out,
						     bool& flag);

typedef int _REAL_MPI_::Datatype::Delete_attr_function(Datatype& type,
						       int type_keyval,
						       void* attribute_val,
						       void* extra_state);
#endif






