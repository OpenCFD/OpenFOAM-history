// -*- C++ -*-
//
// $Id: mpi2cxx_list.h,v 1.3 2002/10/09 20:57:37 brbarret Exp $
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

#ifndef MPI2CPP_LIST_H_
#define MPI2CPP_LIST_H_

#include "lam_config.h"

class mpi2cppList {
public:
  typedef void* Data;
  class iter;

  class Link {
    friend class mpi2cppList;
    friend class iter;
    Data data;
    Link *next;
    Link *prev;
    Link() { }
    Link(Data d, Link* p, Link* n) : data(d), next(n), prev(p) { }
  };

  class iter {
    friend class mpi2cppList;
    Link* node;
  public:
    iter(Link* n) : node(n) { }
    iter& operator++() { node = node->next; return *this; }
    iter operator++(int) { iter tmp = *this; ++(*this); return tmp; }
    Data& operator*() const { return node->data; }
    bool operator==(const iter& x) const { return (bool)(node == x.node); }
    bool operator!=(const iter& x) const { return (bool)(node != x.node); }
  };
  
  mpi2cppList() { _end.prev = &_end; _end.next = &_end; }
  virtual ~mpi2cppList() {
    for (iter i = begin(); i != end(); ) {
      Link* garbage = i.node; i++;
      delete garbage;
    }
  }
  virtual iter begin() { return _end.next; }
  virtual iter end() { return &_end; }
  virtual iter insert(iter p, Data d) {
    iter pos(p);
    Link* n = new Link(d, pos.node->prev, pos.node);
    pos.node->prev->next = n;
    pos.node->prev = n;
    return n;
  }
  void erase(iter pos) {
    pos.node->prev->next = pos.node->next;
    pos.node->next->prev = pos.node->prev;
    delete pos.node;
  }

protected:
  Link _end;
};

#endif




