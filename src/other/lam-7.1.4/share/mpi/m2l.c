/*
 * Copyright (c) 2001-2002 The Trustees of Indiana University.  
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
 * $Id: m2l.c,v 6.5 2002/12/11 19:15:12 jsquyres Exp $
 *
 *	Function:	- mapping between MPI synchronization and LAM's
 *			  network messages
 *
 *	The nh_event field is laid out as follows:
 *
 *		bit  31:        0 (1 is reserved for LAM system messages)
 *		bits 30-16:     0x7FFF: "any communicator"
 *		bits 30-18:     MPI context ID
 *		bit  17:        tail of long message
 *		bit  16:        acknowledgement of long message
 *		bit  15:        unused
 *		bits 14-0:      destination process index
 *
 *	The nh_type field is laid out as follows:
 *
 *		bits 31-16:     source process or 0xFFFF for MPI_ANY_SOURCE
 *		bit  31:        1: MPI_ANY_SOURCE, 0: other
 *		bits 30-24:     source process index
 *		bits 23-16:     source node
 *		bits 15-0:      tag or 0xFFFF for MPI_ANY_TAG
 */

#include <app_mgmt.h>
#include <mpi.h>
#include <mpisys.h>
#include <net.h>
#include <rpisys.h>

/*
 * global functions
 */
void			_m2l_ack();
void			_m2l_fill();
void			_m2l_fillsys();
void			_m2l_tail();
struct _proc		*_m2l_getsource();
int			_m2l_gettag();

/*
 *	_m2l_fill
 *
 *	Function:	- fills MPI synchronization info into LAM
 *			  network message
 *	Accepts:	- source process (or NULL)
 *			- destination process
 *			- tag
 *			- context ID
 *			- network message header (filled)
 */
void
_m2l_fill(proc_src, proc_dest, tag, cid, msg)

struct _proc		*proc_src;
struct _proc		*proc_dest;
int			tag;
int			cid;
struct nmsg		*msg;

{
	msg->nh_node = proc_dest->p_gps.gps_node;

	msg->nh_event = ((cid & 0x1FFF) << 18) |
			(proc_dest->p_gps.gps_idx & 0x7FFF);

	if (proc_src == 0) {
		msg->nh_type = 0xFFFF0000;
	} else {
		msg->nh_type = (((proc_src->p_gps.gps_idx & 0xFF) << 8) |
				(proc_src->p_gps.gps_node & 0xFF)) << 16;
	}

	if (tag < 0) {
		tag &= 0xFFFF;
	} else {
		tag &= 0x7FFF;
	}

	msg->nh_type |= tag;
}

/*
 *	_m2l_fillsys
 *
 *	Function:	- fills system MPI synchronization info into LAM
 *			  network message
 *	Accepts:	- source process node
 *			- source process index (< 0 => any source)
 *			- destination process node
 *			- destination process index
 *			- context ID
 *			- tag
 *			- network message header (filled)
 */
void
_m2l_fillsys(src_node, src_idx, dest_node, dest_idx, cid, tag, msg)

int4			src_node;
int4			src_idx;
int4			dest_node;
int4			dest_idx;
int			cid;
int			tag;
struct nmsg		*msg;

{
	msg->nh_node = dest_node;

	msg->nh_event = ((-1 & 0x1FFF) << 18) | (dest_idx & 0x7FFF);
	if (src_idx < 0) {
		msg->nh_type = 0xFFFF0000;
	} else {
		msg->nh_type =
			(((src_idx & 0xFF) << 8) | (src_node & 0xFF)) << 16;
	}

	if (tag < 0) {
		tag &= 0xFFFF;
	} else {
		tag &= 0x7FFF;
	}

	msg->nh_type |= tag;
/*
 * N2ND flag fakes out mpimsg.
 */
	msg->nh_flags |= KSYNCSQL | N2ND;
}

/*
 *	_m2l_ack
 *
 *	Function:	- modify LAM network message for MPI ack
 *			  synchronization
 *	Accepts:	- network message
 */
void
_m2l_ack(msg)

struct nmsg		*msg;

{
	msg->nh_event |= 0x10000;
}

/*
 *	_m2l_tail
 *
 *	Function:	- modify LAM network message for MPI tail data
 *			  synchronization
 *	Accepts:	- network message
 */
void
_m2l_tail(msg)

struct nmsg		*msg;

{
	msg->nh_event |= 0x20000;
}

/*
 *	_m2l_getsource
 *
 *	Accepts:	- network message
 *	Returns:	- MPI source process
 */
struct _proc *
_m2l_getsource(msg)

struct nmsg		*msg;

{
	struct _gps	source;

	source.gps_node = (msg->nh_type & 0x00FF0000) >> 16;
	source.gps_idx = (msg->nh_type & 0xFF000000) >> 24;
	return(lam_procfind(&source));
}

/*
 *	_m2l_gettag
 *
 *	Accepts:	- network message
 *	Returns:	- MPI message tag
 */
int
_m2l_gettag(msg)

struct nmsg		*msg;

{
	return(msg->nh_type & 0xFFFF);
}
