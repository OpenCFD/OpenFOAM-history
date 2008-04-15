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
 * $Id: nmsgconv.c,v 6.6 2003/07/05 21:52:08 jsquyres Exp $
 * 
 *	Function:	- Trollius message byte order conversion routines
 *
 *			- _ltot_sysnmsg: ltot() system part of message
 *			- _ttol_sysnmsg: ttol() system part of message
 *			- _ltot_usrnmsg: ltot() user part of message
 *			- _ttol_usrnmsg: ttol() user part of message
 */

#include <portable.h>
#include <net.h>
#include <t_types.h>
#include <etc_misc.h>

/*
 *	_ltot_sysnmsg
 *
 *	Function:	- convert system part of msg to Trollius byte order
 *			- the data pouch is excluded
 *	Accepts:	- ptr to network message descriptor
 */
void
_ltot_sysnmsg(struct nmsg *nhead)
{
	ltoti4(&(nhead->nh_dl_event), &(nhead->nh_dl_event));
	ltoti4(&(nhead->nh_dl_link), &(nhead->nh_dl_link));
	ltoti4(&(nhead->nh_node), &(nhead->nh_node));
	ltoti4(&(nhead->nh_event), &(nhead->nh_event));
	ltoti4(&(nhead->nh_type), &(nhead->nh_type));
	ltoti4(&(nhead->nh_length), &(nhead->nh_length));
	ltoti4(&(nhead->nh_flags), &(nhead->nh_flags));
}

/*
 *	_ttol_sysnmsg
 *
 *	Function:	- convert system part of msg to local byte order
 *			- the data pouch is excluded
 *	Accepts:	- ptr to network message descriptor
 */
void
_ttol_sysnmsg(struct nmsg *nhead)
{
	ttoli4(&(nhead->nh_dl_event), &(nhead->nh_dl_event));
	ttoli4(&(nhead->nh_dl_link), &(nhead->nh_dl_link));
	ttoli4(&(nhead->nh_node), &(nhead->nh_node));
	ttoli4(&(nhead->nh_event), &(nhead->nh_event));
	ttoli4(&(nhead->nh_type), &(nhead->nh_type));
	ttoli4(&(nhead->nh_length), &(nhead->nh_length));
	ttoli4(&(nhead->nh_flags), &(nhead->nh_flags));
}

/*
 *	_ltot_usrnmsg
 *
 *	Function:	- convert message and data pouch to Trollius byte order
 *			- system part of message assumed in local byte order
 *	Accepts:	- ptr to network message descriptor
 */
void
_ltot_usrnmsg(struct nmsg *nhead)
{
/*
 * Convert the data pouch.
 */
	switch(nhead->nh_flags & DMSKDATA) {

		case 0:
		case DINT4DATA:
		mltoti4((void *) nhead->nh_data, NHDSIZE);
		break;

		case DFLT4DATA:
		mltotf4((void *) nhead->nh_data, NHDSIZE);
		break;

		case DFLT8DATA:
		mltotf8((void *) nhead->nh_data, NHDSIZE / 2);
		break;
	}
/*
 * Convert the message.
 */
	if (nhead->nh_length <= 0) {
		return;
	}

	switch(nhead->nh_flags & DMSKMSG) {

		case DINT4MSG:
		mltoti4(nhead->nh_msg, nhead->nh_length / sizeof(int4));
		break;

		case DFLT4MSG:
		mltotf4(nhead->nh_msg, nhead->nh_length / sizeof(float4));
		break;

		case DFLT8MSG:
		mltotf8(nhead->nh_msg, nhead->nh_length / sizeof(float8));
		break;

		case DINT2MSG:
		mltoti2(nhead->nh_msg, nhead->nh_length / sizeof(int2));
		break;
	}
}

/*
 *	_ttol_usrnmsg
 *
 *	Function:	- convert message and data pouch to local byte order
 *			- system part of message assumed in local byte order
 *	Accepts:	- ptr to network message descriptor
 */
void
_ttol_usrnmsg(struct nmsg *nhead)
{
/*
 * Convert the data pouch.
 */
	switch(nhead->nh_flags & DMSKDATA) {

		case 0:
		case DINT4DATA:
		mttoli4((void *) nhead->nh_data, NHDSIZE);
		break;

		case DFLT4DATA:
		mttolf4((void *) nhead->nh_data, NHDSIZE);
		break;

		case DFLT8DATA:
		mttolf8((void *) nhead->nh_data, NHDSIZE / 2);
		break;
	}
/*
 * Convert the message.
 */
	if (nhead->nh_length <= 0) {
		return;
	}

	switch(nhead->nh_flags & DMSKMSG) {

		case DINT4MSG:
		mttoli4(nhead->nh_msg, nhead->nh_length / sizeof(int4));
		break;

		case DFLT4MSG:
		mttolf4(nhead->nh_msg, nhead->nh_length / sizeof(float4));
		break;

		case DFLT8MSG:
		mttolf8(nhead->nh_msg, nhead->nh_length / sizeof(float8));
		break;

		case DINT2MSG:
		mttoli2(nhead->nh_msg, nhead->nh_length / sizeof(int2));
		break;
	}
}
