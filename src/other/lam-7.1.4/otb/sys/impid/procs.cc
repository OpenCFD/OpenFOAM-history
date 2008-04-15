/*
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
 * $Id: procs.cc,v 1.11 2003/02/04 17:51:30 jsquyres Exp $
 *
 *	Function:	- IMPI flow control rules
 */

#include <lam_config.h>

#if LAM_WANT_IMPI

// IRIX MIPSpro 7.30 compilers require that <new> comes first... @#$@#$!!!
#if LAM_CXX_NOTHROW_NEW
#include <new>
#endif

#include <string.h>

#include <map>

#include <impi-defs.h>
#include <impi.h>
#include <impid-cc.h>
#include <lamdebug-cc.h>

using std::map;
using std::endl;
using std::cerr;


/*
 * private class to do the comparison of IMPI_Proc's for our map
 * compile-time binding is a wonderful thing...
 */
class proc_cmp {
public:
  inline bool operator()(const IMPI_Proc& a, const IMPI_Proc& b) const {
    for (unsigned int i = 0; i < sizeof(a.p_hostid); i++)
      if (a.p_hostid[i] < b.p_hostid[i])
	return true;
      else if (a.p_hostid[i] > b.p_hostid[i])
	return false;
    return (bool) (a.p_pid < b.p_pid);
  }
};


/*
 * private data
 */
typedef map <IMPI_Proc, int, proc_cmp> proc_resolver_t;
static proc_resolver_t *proc_map = 0;
static Debug debug(false);


/*
 *	proc_resolver_init
 *
 *	Function:	- initialize IMPI_Proc resolver data
 *
 *      Returns:        - 0 on success, LAMERROR otherwise
 */
int
proc_resolver_init()
{
  IMPI_Uint4 i;
  IMPI_Proc p;

  // Setup the proc resolver map

#if LAM_CXX_NOTHROW_NEW || !LAM_CXX_EXCEPTIONS
  proc_map = new LAM_CXX_NOTHROW_NEW_ARG proc_resolver_t;
  if (proc_map == 0) {
    cerr << "LAM IMPI host cannot allocate process resolver table" << endl;
    impi_bail(1);
  }
#else
  try {
    proc_map = new proc_resolver_t;
  } catch(...) {
    cerr << "LAM IMPI host cannot allocate process resolver table" << endl;
    impi_bail(1);
  }
#endif
  for (i = 0; i < nprocs; i++) {
    memcpy(p.p_hostid, lam_impi_procs[i].ip_ipv6, sizeof(p.p_hostid));
    p.p_pid = lam_impi_procs[i].ip_pid;

    debug << "PROC_RESOlVER_INIT: Set proc " << i << " to be pid "
	  << (unsigned int) p.p_pid << endl;
    (*proc_map)[p] = i;
  }

  return 0;
}


/*
 *	proc_resolver
 *
 *	Function:	- returns a rank from an IMPI_Proc
 *
 *      Accepts:        - ptr to IMPI_Proc
 *      Returns:        - rank on success, -1 otherwise
 */
int
proc_resolver(IMPI_Proc* p)
{
  proc_resolver_t::iterator i;

  // Note that there are acceptable times when the resolver will fail
  // -- IMPI_PKFINI packets do not include the source/dest proc info,
  // for example.  Hence, any IMPI_PKFINI packet (incoming or
  // outgoing) will fail to be resolved properly.  This is ok, becayse
  // PKFINI packets are identified by which socket they came from --
  // i.e., they are *host* level packets, not *proc* level packets.
  // Hence, being unable to resolve a packet is *not* a fatal error.

  i = proc_map->find(*p);
  if (i == proc_map->end()) {
    debug << "Couldn't resolve address" << endl;
    return -1;
  }

  return (*i).second;
}


/*
 *	proc_resolver_destroy
 *
 *	Function:	- destroy IMPI_Proc resolver data
 *
 *      Returns:        - 0 on success, LAMERROR otherwise
 */
int
proc_resolver_destroy()
{
  if (proc_map != 0) {
    proc_map->clear();
    delete proc_map;
#if LAM_IMPID_DEBUG
    proc_map = 0;
#endif
  }

  return 0;
}

#endif
