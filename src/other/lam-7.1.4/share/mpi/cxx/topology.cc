// -*- c++ -*-
//
// $Id: topology.cc,v 1.4 2002/10/09 20:58:57 brbarret Exp $
//
// Copyright (c) 2001-2002 The Trustees of Indiana University.  
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


MPI::Cartcomm::Cartcomm(const MPI_Comm& data) : 
  MPI::Intracomm(data), pmpi_comm(data)
{ 

}


//
// Groups, Contexts, and Communicators
//

MPI::Cartcomm
MPI::Cartcomm::Dup() const
{
  return pmpi_comm.Dup();
}

MPI::Cartcomm& MPI::Cartcomm::Clone() const
{
  return (MPI::Cartcomm&)pmpi_comm.Clone();
}


//
//  Process Topologies
//

int MPI::Cartcomm::Get_dim() const
{
  return pmpi_comm.Get_dim();
}

void MPI::Cartcomm::Get_topo(int maxdims, int dims[], bool periods[],
			     int coords[]) const
{
  pmpi_comm.Get_topo(maxdims, dims, periods, coords);
}

int MPI::Cartcomm::Get_cart_rank(const int coords[]) const
{
  return pmpi_comm.Get_cart_rank(coords);
}

void MPI::Cartcomm::Get_coords(int rank, int maxdims, int coords[]) const
{
  pmpi_comm.Get_coords(rank, maxdims, coords);
}

void MPI::Cartcomm::Shift(int direction, int disp,
			  int &rank_source, int &rank_dest) const
{
  pmpi_comm.Shift(direction, disp, rank_source, rank_dest);
}
  
MPI::Cartcomm MPI::Cartcomm::Sub(const bool remain_dims[])
{
  return pmpi_comm.Sub(remain_dims);
}

int MPI::Cartcomm::Map(int ndims, const int dims[], const bool periods[]) const
{
  return pmpi_comm.Map(ndims, dims, periods);
}

//
//   ========   Graphcomm member functions  ========
//

MPI::Graphcomm::Graphcomm(const MPI_Comm& data) : 
  MPI::Intracomm(data), pmpi_comm(data) 
{

}

//
// Groups, Contexts, and Communicators
//

MPI::Graphcomm
MPI::Graphcomm::Dup() const
{
  return pmpi_comm.Dup();
}

MPI::Graphcomm& MPI::Graphcomm::Clone() const
{
  return (MPI::Graphcomm&)pmpi_comm.Clone();
}

//
//  Process Topologies
//

void
MPI::Graphcomm::Get_dims(int nnodes[], int nedges[]) const 
{
  pmpi_comm.Get_dims(nnodes, nedges);
}

void
MPI::Graphcomm::Get_topo(int maxindex, int maxedges, int index[], 
	 int edges[]) const
{
  pmpi_comm.Get_topo(maxindex, maxedges, index, edges);
}

int
MPI::Graphcomm::Get_neighbors_count(int rank) const 
{
  return pmpi_comm.Get_neighbors_count(rank);
}

void
MPI::Graphcomm::Get_neighbors(int rank, int maxneighbors, 
	      int neighbors[]) const 
{
  pmpi_comm.Get_neighbors(rank, maxneighbors, neighbors);
}

int
MPI::Graphcomm::Map(int nnodes, const int index[], 
    const int edges[]) const 
{
  return pmpi_comm.Map(nnodes, index, edges);
}
