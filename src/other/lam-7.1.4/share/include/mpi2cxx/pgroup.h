// -*- c++ -*-
//
// $Id: pgroup.h,v 1.3 2002/10/09 20:57:37 brbarret Exp $
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

class Group {
public:
  // construction / destruction
  inline Group() : mpi_group(MPI_GROUP_NULL) { }
  inline virtual ~Group() {}
  inline Group(const MPI_Group &i) : mpi_group(i) { }

  // copy / assignment
  Group(const Group& g): mpi_group(g.mpi_group) { }

  Group& operator=(const Group& g) {
    mpi_group = g.mpi_group;
    return *this;
  }

  // comparison
  bool operator== (const Group &a) {
    return (bool)(mpi_group == a.mpi_group);
  }
  bool operator!= (const Group &a) { return (bool)!(*this == a); }
 
  // inter-language operability
  inline Group& operator= (const MPI_Group &i) { mpi_group = i; return *this; }
  inline operator const MPI_Group& () const { return mpi_group; }
  inline operator MPI_Group* () const { return (MPI_Group*)&mpi_group; }

  inline const MPI_Group& mpi() const { return mpi_group; }
  //
  // Groups, Contexts, and Communicators
  //

  virtual int Get_size() const;
  
  virtual int Get_rank() const;

  static void Translate_ranks(const Group& group1, int n, const int ranks1[], 
			      const Group& group2, int ranks2[]);
  
  static int Compare(const Group& group1, const Group& group2);

  static Group Union(const Group &group1, const Group &group2);

  static Group Intersect(const Group &group1, const Group &group2);

  static Group Difference(const Group &group1, const Group &group2);

  virtual Group Incl(int n, const int ranks[]) const;

  virtual Group Excl(int n, const int ranks[]) const;

  virtual Group Range_incl(int n, const int ranges[][3]) const;

  virtual Group Range_excl(int n, const int ranges[][3]) const;

  virtual void Free();

protected:
  MPI_Group mpi_group;

};

