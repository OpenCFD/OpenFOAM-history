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
 * $Id: hostack_mgmt.h,v 6.6 2003/02/04 00:24:17 jsquyres Exp $
 *
 *	Function:	- makes long and short packets to place on host queues
 */

#ifndef _LAM_HOSTACK_MGMT_H_
#define _LAM_HOSTACK_MGMT_H_

#include <lam_config.h>

#if LAM_WANT_IMPI

#include <impi-defs.h>
#include <list>

/*
 * Use a class for this so that we can get a destructor for free; it
 * is guaranteed to clean itself up (and all associated data) when it
 * gets destroyed.  
 */
class hostack_mgmt {
public:
  inline hostack_mgmt(int _world_src_rank, int _src_rank, 
		      int _dest_rank, IMPI_Packet* _pk)
    : world_src_rank(_world_src_rank), src_rank(_src_rank),
      dest_rank(_dest_rank), pk(*_pk)
  {}

  inline ~hostack_mgmt()
  {
#if LAM_IMPID_DEBUG
    world_src_rank = 0;
    src_rank = 0;
    dest_rank = 0;
    memset(&pk, 0, sizeof(pk));
#endif
  }

  /*
   * Accessor functions
   */

  inline int get_world_src_rank() const { return world_src_rank; }
  inline int get_src_rank() const { return src_rank; }
  inline int get_dest_rank() const { return dest_rank; }
  inline IMPI_Packet* get_packet() { return &pk; }

protected:
  int world_src_rank;
  int src_rank;
  int dest_rank;
  IMPI_Packet pk;

private:
};


#endif /* LAM_WANT_IMPI */
#endif /* _LAM_HOSTACK_MGMT_H_ */
