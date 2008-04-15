// -*- c++ -*-
//
// $Id: ptopology.h,v 1.3 2002/10/09 20:57:38 brbarret Exp $
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


class Cartcomm : public Intracomm {
public:
   // construction
  Cartcomm() : Intracomm(MPI_COMM_NULL) { }
  // copy
  Cartcomm(const Cartcomm& data) : Intracomm(data) { }
  // inter-language operability
  inline Cartcomm(const MPI_Comm& data);

  //
  // Groups, Contexts, and Communicators
  //

  Cartcomm Dup() const;

  Cartcomm& Clone() const;

  //
  //  Process Topologies
  //

  virtual int Get_dim() const;

  // JGS KCC gives a warning here because of Comm::Get_topo()
  virtual void Get_topo(int maxdims, int dims[], bool periods[],
			      int coords[]) const;

  virtual int Get_cart_rank(const int coords[]) const;
  
  virtual void Get_coords(int rank, int maxdims, int coords[]) const;

  virtual void Shift(int direction, int disp,
		     int &rank_source, int &rank_dest) const;
  
  virtual Cartcomm Sub(const bool remain_dims[]);

  virtual int Map(int ndims, const int dims[], const bool periods[]) const;

};

//===================================================================
//                    Class Graphcomm
//===================================================================

class Graphcomm : public Intracomm {
public:
  // construction
  Graphcomm() : Intracomm(MPI_COMM_NULL) { }
  // copy
  Graphcomm(const Graphcomm& data) : Intracomm(data) { }
  // inter-language operability
  inline Graphcomm(const MPI_Comm& data);

  //
  // Groups, Contexts, and Communicators
  //

  Graphcomm Dup() const;

  Graphcomm& Clone() const;

  //
  //  Process Topologies
  //

  virtual void Get_dims(int nnodes[], int nedges[]) const;

  virtual void Get_topo(int maxindex, int maxedges, int index[], 
			int edges[]) const;

  virtual int Get_neighbors_count(int rank) const;

  virtual void Get_neighbors(int rank, int maxneighbors, 
			     int neighbors[]) const;

  virtual int Map(int nnodes, const int index[], 
		  const int edges[]) const;

};
