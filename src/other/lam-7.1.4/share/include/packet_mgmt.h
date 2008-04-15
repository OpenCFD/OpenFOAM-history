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
 * $Id: packet_mgmt.h,v 6.7 2003/02/04 00:24:17 jsquyres Exp $
 *
 *	Function:	- makes long and short packets to place on host queues
 */

#ifndef _LAM_PACKET_MGMT_H_
#define _LAM_PACKET_MGMT_H_

#include <lam_config.h>

#if LAM_WANT_IMPI

#include <impid-cc.h>
#include <impi-defs.h>
#include <list>


/*
 * Use a class for this so that we can get a destructor for free; it
 * is guaranteed to clean itself up (and all associated data) when it
 * gets destroyed.  
 */
class packet_mgmt {
public:
  inline packet_mgmt(char* _top, char* _buffer, bool _wantDelete, 
		     IMPI_Packet* _pk)
    : top(_top), wantDelete(_wantDelete), buffer(_buffer), pk(*_pk)
  {}

  inline ~packet_mgmt()
  {
    if (wantDelete && top != 0) {
      delete[] top;
    }

#if LAM_IMPID_DEBUG
    top = 0;
    buffer = 0;
    wantDelete = false;
    memset(&pk, 0, sizeof(pk));
#endif
  }

  /*
   * Accessor functions
   */

  inline char* get_buffer() const { return buffer; }
  inline IMPI_Packet* get_packet() { return &pk; }

protected:
  char *top;
  bool wantDelete;

  char *buffer;
  IMPI_Packet pk;

private:
};


/*
 * Typedef to make lists of packet_mgmts easier to handle 
 */
typedef std::list<packet_mgmt*> packet_mgmt_list;


#endif /* LAM_WANT_IMPI */
#endif /* _LAM_PACKET_MGMT_H_ */
