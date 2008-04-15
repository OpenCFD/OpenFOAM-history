/* -*- c++ -*-
 *
 * Copyright (c) 2001-2003 The Trustees of Indiana University.  
 *                         All rights reserved.
 * Copyright (c) 1998-2001 University of Notre Dame. 
 *                         All rights reserved.
 * Copyright (c) 1994-1998 The Ohio State University.  
 *                         All rights reserved.
 * 
 * This file is part of the LAM/MPI software package.  For license
 * information, see the LICENSE file in the top level directory of the
 * LAM/MPI source distribution.
 * 
 * $HEADER$
 *
 * $Id: lamdebug-cc.h,v 6.3 2003/02/04 17:53:09 jsquyres Exp $
 *
 *	Function:	- IMPI client daemon header file for C++ functions
 */

#ifndef _LAM_DEBUG_CC_H_
#define _LAM_DEBUG_CC_H_

#ifndef __cplusplus

Error: This should only be included by C++ source files

#endif

#include <iostream>


//
// Brings printf debugging to a new low
//
class Debug {
public:
  inline Debug(bool initial = false) : on(initial) {}

  inline void On() { on = true; }
  inline void Off() { on = false; }
  inline bool isOn() { return on; };

protected:
  bool on;
};


//
// A sick and wrong templated debug function
//
template <class T>
inline Debug&
operator<<(Debug& d, const T& x) {
  if (d.isOn())
    std::cerr << x;
  return d;

}

//
// These need to be here for the endl on some compilers
//
inline Debug&
operator<< (Debug& d, std::ostream& (*_f)(std::ostream&))
{
  if (d.isOn())
    std::cerr << std::endl;
  return d;
}

inline Debug& 
operator<< (Debug& d, std::ios& (*_f)(std::ios&))
{
  if (d.isOn())
    std::cerr << std::endl;
  return d;
}

#endif
