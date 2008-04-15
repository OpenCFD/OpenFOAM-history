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
 *	$Id: lamnet.h,v 6.13 2003/07/31 03:04:17 jsquyres Exp $
 * 
 *	Function:	- LAM network description
 */

#ifndef _LAMNET
#define _LAMNET

#include <lam_config.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <portable.h>
#include <all_list.h>


/*
 * default filenames
 */
#define DEFFBHOST	"lam-bhost.def"	/* default host file */


/*
 * default tools
 */
#define DEFTHBOOT	"hboot"
#define DEFTTKILL	"tkill"
#define DEFTWIPE	"lamwipe"


/* 
 * LIST element structure
 */

typedef struct {
  char *key;
  char *value;
} ELEM;


/*
 * node description
 */
struct lamnode {			/* node information */
	int4		lnd_nodeid;	/* node identifier */
	int4		lnd_type;	/* node type */
	int4		lnd_ncpus;	/* number of CPUs on node */
	int		lnd_bootport;	/* booting port # */
	char		*lnd_hname;	/* host name */
	char		*lnd_uname;	/* user name */
	struct sockaddr_in
			lnd_addr;	/* internet address */

	LIST		*lnd_keyval;	/* list to store key-value pairs */

        /* Extra data that the boot ssi can tag on, if needed... */
        struct lnd_ssi_boot_nodeinfo *lnd_ssi;
};

#ifdef __cplusplus
extern "C" {
#endif

extern int		lamnet_findhosts __ARGS((struct lamnode *lamnet,
					int nlamnet, int *badhost));
extern int		lamnet_findorig __ARGS((struct lamnode *lamnet,
					int nlamnet));

#ifdef __cplusplus
}
#endif

#endif	/* _LAMNET */
