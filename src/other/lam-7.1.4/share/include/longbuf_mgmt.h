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
 * $Id: longbuf_mgmt.h,v 6.5 2003/02/04 00:24:17 jsquyres Exp $
 *
 *	Function:	- makes long and short packets to place on host queues
 */

#ifndef _LAM_LONGBUF_MGMT_H_
#define _LAM_LONGBUF_MGMT_H_

#include <lam_config.h>

#if LAM_WANT_IMPI

#include <impid-cc.h>
#include <impi-defs.h>


/*
 * Use a class for this so that we can get a destructor for free; it
 * is guaranteed to clean itself up (and all associated data) when it
 * gets destroyed.  
 */
class longbuf_mgmt {
public:
  inline longbuf_mgmt(char* _buffer, IMPI_Uint8 _received, IMPI_Packet& _pk)
    : buffer(_buffer), received(_received), pk(_pk)
    {}
  inline ~longbuf_mgmt()
    {
      delete[] buffer;
#if LAM_IMPID_DEBUG
      buffer = 0;
      received = (IMPI_Uint8) 0;
      memset(&pk, 0, sizeof(pk));
#endif
  }

  inline char*        get_buffer() const { return buffer; }
  inline IMPI_Uint8   get_received() const { return received; }
  inline void         inc_received(IMPI_Uint4 size)
    { received += (IMPI_Uint8) size; }
  inline IMPI_Packet* get_packet() { return &pk; }

protected:
  char        *buffer;
  IMPI_Uint8  received;

  IMPI_Packet pk;

private:
};

#endif /* LAM_WANT_IMPI */
#endif /* _LAM_LONGBUF_MGMT_H_ */

