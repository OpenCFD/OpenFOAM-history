// -*- c++ -*-
//
// $Id: datatype.cc,v 1.3 2003/04/15 01:20:39 adharurk Exp $
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

MPI::Datatype
MPI::Datatype::Create_contiguous(int count) const
{
  return pmpi_datatype.Create_contiguous(count);
}

MPI::Datatype
MPI::Datatype::Create_vector(int count, int blocklength,
			     int stride) const
{
  return pmpi_datatype.Create_vector(count, blocklength, stride);
}

MPI::Datatype
MPI::Datatype::Create_indexed(int count,
			      const int array_of_blocklengths[], 
			      const int array_of_displacements[]) const
{
  return pmpi_datatype.Create_indexed(count, array_of_blocklengths,
				      array_of_displacements);
}

MPI::Datatype
MPI::Datatype::Create_struct(int count, const int array_of_blocklengths[],
			     const Aint array_of_displacements[],
			     const Datatype array_of_types[])
{
  PMPI::Datatype* pmpi_types = new PMPI::Datatype[count];
  int i;
  for (i = 0; i < count; i++)
    pmpi_types[i] = array_of_types[i].pmpi();
  
  PMPI::Datatype data = PMPI::Datatype::Create_struct(count, 
						      array_of_blocklengths,
						      array_of_displacements, 
						      pmpi_types);
  delete[] pmpi_types;
  return data;
}

MPI::Datatype
MPI::Datatype::Create_hindexed(int count, const int array_of_blocklengths[],
			       const Aint array_of_displacements[]) const
{
  return pmpi_datatype.Create_hindexed(count, array_of_blocklengths,
				       array_of_displacements);
}


MPI::Datatype
MPI::Datatype::Create_hvector(int count, int blocklength, MPI::Aint stride) const
{
  return pmpi_datatype.Create_hvector(count, blocklength, stride);
}

int
MPI::Datatype::Get_size() const
{
  return pmpi_datatype.Get_size();
}


void
MPI::Datatype::Get_extent(_REAL_MPI_::Aint& lb, _REAL_MPI_::Aint& extent) const
{
  pmpi_datatype.Get_extent(lb, extent);
}

void
MPI::Datatype::Commit()
{
  pmpi_datatype.Commit();
}

void
MPI::Datatype::Free()
{
  pmpi_datatype.Free();
}



void
MPI::Datatype::Pack(const void* inbuf, int incount,
		    void *outbuf, int outsize,
		    int& position, const MPI::Comm &comm) const
{
  pmpi_datatype.Pack(inbuf, incount, outbuf, outsize, position,
		     comm);
}

void
MPI::Datatype::Unpack(const void* inbuf, int insize,
		      void *outbuf, int outcount, int& position,
		      const MPI::Comm& comm) const 
{
  pmpi_datatype.Unpack(inbuf, insize, outbuf, outcount, position,
		       comm);
}

int
MPI::Datatype::Pack_size(int incount, const MPI::Comm& comm) const 
{
  return pmpi_datatype.Pack_size(incount, comm);
}


//
// Miscellany
//

MPI::Datatype
MPI::Datatype::Create_subarray(int ndims, const int array_of_sizes[],
		     const int array_of_subsizes[],
		     const int array_of_starts[], int order) const
{
  return pmpi_datatype.Create_subarray(ndims, array_of_sizes,
				       array_of_subsizes, 
				       array_of_starts, order);
}


//
// External Interfaces
//

MPI::Datatype
MPI::Datatype::Dup() const
{
  return pmpi_datatype.Dup();
}


int
MPI::Datatype::Create_keyval(MPI::Datatype::Copy_attr_function* 
			     type_copy_attr_fn,
			     MPI::Datatype::Delete_attr_function* 
			     type_delete_attr_fn,
			     void* extra_state)
{
  return PMPI::Datatype::Create_keyval((PMPI::Datatype::Copy_attr_function *)
				       type_copy_attr_fn,
				       (PMPI::Datatype::Delete_attr_function *)
				       type_delete_attr_fn,  extra_state);
}


void
MPI::Datatype::Delete_attr(int type_keyval)
{
  pmpi_datatype.Delete_attr(type_keyval);
}


void
MPI::Datatype::Free_keyval(int& type_keyval)
{
  PMPI::Datatype::Free_keyval(type_keyval);
}


bool
MPI::Datatype::Get_attr(int type_keyval, void* attribute_val) const
{
  return pmpi_datatype.Get_attr(type_keyval, attribute_val);
}

void
MPI::Datatype::Get_contents(int max_integers, int max_addresses,
			    int max_datatypes, int array_of_integers[],
			    Aint array_of_addresses[],
			    MPI::Datatype array_of_types[]) const
{
  PMPI::Datatype* pmpi_types = new PMPI::Datatype[max_datatypes];
  int i;
  for (i = 0; i < max_datatypes; i++)
    pmpi_types[i] = array_of_types[i].pmpi();
  pmpi_datatype.Get_contents(max_integers, max_addresses,
			     max_datatypes, array_of_integers,
			     array_of_addresses,
			     pmpi_types);
  delete[] pmpi_types; 
}

void
MPI::Datatype::Get_envelope(int& num_integers, int& num_addresses,
		  int& num_datatypes, int& combiner) const
{
  pmpi_datatype.Get_envelope(num_integers, num_addresses, num_datatypes,
			     combiner);
}


void
MPI::Datatype::Get_name(char* type_name, int& resultlen) const
{
  pmpi_datatype.Get_name(type_name, resultlen);
}


void
MPI::Datatype::Set_attr(int type_keyval, const void* attribute_val)
{
  pmpi_datatype.Set_attr(type_keyval, attribute_val);
}


void
MPI::Datatype::Set_name(const char* type_name)
{
  pmpi_datatype.Set_name(type_name);
}
