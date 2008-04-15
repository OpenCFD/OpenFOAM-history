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
 * $Id: lamports.c,v 6.6 2003/11/02 21:13:10 brbarret Exp $
 *
 *	Function:	- port management functions
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <blktype.h>
#include <mpisys.h>
#include <terror.h>
#include <typical.h>

#define LAM_PORTMAX	16
#define LAM_PORTLOW	(BLKHIGH + 1)

/*
 * private functions
 */
static int		port_cmp(struct _port *p1, struct _port *p2);

/*
 * private variables
 */
static int		ports[LAM_PORTMAX];	/* port table */

/*
 *	lam_ports_init
 * 
 *	Function:	- initialize port bookkeeping structures
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ports_init()
{
	lam_ports = al_init(sizeof(struct _port), port_cmp);
	return((lam_ports == 0) ? LAMERROR : 0);
}

/*
 *	lam_ports_free
 * 
 *	Function:	- free port bookkeeping structures
 */
void
lam_ports_free()
{
	al_free(lam_ports);
}

/*
 *	lam_port_open
 * 
 *	Function:	- open a LAM MPI port
 *	Returns:	- port number or LAMERROR
 */
int
lam_port_open()
{
	int		i;
/*
 * Search for free port.
 */
	for (i = 0; i < LAM_PORTMAX; i++) {
		if (ports[i] == 0) {
			ports[i] = 1;
			return(LAM_PORTLOW + i);
		}
	}

	return(LAMERROR);
}

/*
 *	lam_port_is_open
 * 
 *	Function:	- checks if port is open
 *	Returns:	- 1 if open, 0 if not open, or LAMERROR if illegal port
 */
int
lam_port_is_open(int port)
{
	port -= LAM_PORTLOW;
	if (!((0 <= port) && (port < LAM_PORTMAX))) {
		errno = EINVAL;
		return(LAMERROR);
	}

	return(ports[port]);
}

/*
 *	lam_port_close
 * 
 *	Function:	- closes a port
 *	Returns:	- 0 else LAMERROR
 */
int
lam_port_close(int port)
{
	port -= LAM_PORTLOW;
	if (!((0 <= port) && (port < LAM_PORTMAX))) {
		errno = EINVAL;
		return(LAMERROR);
	}
	
	ports[port] = 0; 
	return(0);
}

/*
 *	port_cmp
 *
 *	Function:	- compare two ports
 *	Accepts:	- ptr to two entries
 *	Returns:	- string comparison of the port names
 */
static int
port_cmp(struct _port *p1, struct _port *p2)
{
	return(strcmp(p1->prt_name, p2->prt_name));
}
