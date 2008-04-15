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
 * $Id: getinetaddr.c,v 1.4 2003/07/17 18:02:16 jsquyres Exp $
 * 
 *	Function:	- return the machine Internet address (OTB only)
 *			- address is a 4-byte array
 *	Accepts:	- machine name
 *			- 4-byte machine address (returned value)
 *	Returns:	- 0 or ERROR
 */

#include <string.h>

#include <netdb.h>
#include <portable.h>
#include <terror.h>
#include <typical.h>

#include <sys/types.h>
#include <netinet/in.h>

#include <lam_config.h>
#include <lam_network.h>


int
getinetaddr(char *hostname, unsigned char *hostaddr)
{
	struct hostent	*p;
/*
 * Get host name entry.
 */
	p = lam_gethostbyname(hostname);

	if (p == 0) {
		errno = EBADHOST;
		return(LAMERROR);
	}
/*
 * Copy the 4-byte Internet address.
 */
	if (p->h_length == 4) {
		memcpy((char *) hostaddr, p->h_addr, 4);
	}
/*
 * Not an Internet address.  Do nothing for now, it's an error.
 */
	else {
		errno = EINVAL;
		return(LAMERROR);
	}

	return(0);
}
