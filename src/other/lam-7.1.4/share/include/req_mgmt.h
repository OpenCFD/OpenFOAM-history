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
 * $Id: req_mgmt.h,v 6.6 2003/02/04 00:24:17 jsquyres Exp $
 *
 *	Function:	- makes a class with the requests from the isends
 *                        and the corresponding syncack
 */

#ifndef _LAM_REQ_MGMT_H_
#define _LAM_REQ_MGMT_H_

#include <lam_config.h>

#if LAM_WANT_IMPI

#include <impi-defs.h>
#include <impid-cc.h>
#include <impi.h>

/*
 * Use a class for this so that we can get a destructor for free; it
 * is guaranteed to clean itself up (and all associated data) when it
 * gets destroyed.  
 */
class req_mgmt {
public:
  inline req_mgmt(char* _buffer, MPI_Request _mpi_req, 
		  IMPI_Packet* _syncack_pk, MPI_Comm _comm)
    : buffer(_buffer), req(_mpi_req), syncack_pk(_syncack_pk), comm(_comm)
    {}
  inline ~req_mgmt()
    {
      // the syncack_pk will belong to the host_enque_packet and it
      // will be delelted later.  The comm is freed outside of the
      // class because it's a "special" fake comm that needs some
      // special attention to be freed properly. The buffer is the
      // only thing that this object "owns", so we do delete it here
      // when the destructor is called. 

      if (buffer != 0)
	delete[] buffer;
      
#if LAM_IMPID_DEBUG
      buffer = 0;
      req = MPI_REQUEST_NULL;
      syncack_pk = 0;
      comm = 0;
#endif
    }
  
  inline MPI_Request*  get_req() { return &req; }
  inline IMPI_Packet* get_packet() const { return syncack_pk; }
  inline MPI_Comm get_comm() const { return comm; }

protected:
  char *buffer;
  MPI_Request req;
  IMPI_Packet *syncack_pk;
  MPI_Comm comm;

private:
};


#endif /* LAM_WANT_IMPI */
#endif /* _LAM_REQ_MGMT_H_ */
