// -*- C++ -*-
//
// $Id: mpi2cxx_map.h,v 1.2 2002/10/09 20:57:37 brbarret Exp $
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

#ifndef MPI2CPP_MAP_H_
#define MPI2CPP_MAP_H_

#include "mpi2cxx/mpi2cxx_list.h"
typedef mpi2cppList mpi2cppContainer;

template <class TYPE1, class TYPE2>
class mpi2cppMap {
  mpi2cppContainer c;
public:

  typedef TYPE1 key_t;
  typedef TYPE2 value_t;
  typedef mpi2cppList::iter iter;

  struct Pair {
    Pair(key_t f, value_t s) : first(f), second(s) {}
    Pair() : first((key_t) 0), second((value_t) 0) { }
    key_t first;
    value_t second;
  };

  mpi2cppMap() { }

  ~mpi2cppMap() {
    for (iter i = c.begin(); i != c.end(); i++) {
      delete (Pair*)(*i);
    }
  }
  
  Pair* begin();
  Pair* end();
  
  value_t& operator[](key_t key)
  {
    value_t* found = (value_t*)0;
    for (iter i = c.begin(); i != c.end(); i++) {
      if (((Pair*)*i)->first == key)
	found = &((Pair*)*i)->second;
    }
    if (! found) {
      iter tmp = c.insert(c.begin(), new Pair(key, (value_t) 0));
      found = &((Pair*)*tmp)->second;
    }
    return *found;
  }

  void erase(key_t key)
  {
    for (iter i = c.begin(); i != c.end(); i++) {
      if (((Pair*)*i)->first == key) {
	delete (Pair*)*i;
	c.erase(i); break;
      }
    }
  }
};

#endif




