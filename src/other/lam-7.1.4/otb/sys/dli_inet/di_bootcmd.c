/*
 * Copyright (c) 2001-2004 The Trustees of Indiana University.  
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
 * $Id: di_bootcmd.c,v 6.13 2003/03/19 21:16:42 jsquyres Exp $
 *
 *	Function:	- obtains link information from command line
 *	Accepts:	- link array (value returned)
 *			- link array size (value returned)
 *			- argument vector length
 *			- argument vector
 */

#include <lam_config.h>
#include <sfh.h>

#include <netdb.h>			/* Solaris MAXHOSTNAMELEN */
#include <stdlib.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <fcntl.h>

#include <all_opt.h>
#include <dl_inet.h>
#include <laminternal.h>
#include <terror.h>
#include <typical.h>
#include <lam_network.h>
#include <lam-ssi-boot.h>


/*
 * local variables
 */
static char my_hname[MAXHOSTNAMELEN + 1];


/*
 * global (to lam daemons) options
 */
extern OPT* lam_daemon_optd;



int
di_bootcmd(struct dilink **pdilinks, struct diinfo **pdiinfo, int4 *pndil,
	   int *mynodeid, int argc, char **argv)
{
	int		dli_port;	/* datalink input port */
	int		dli_sd;		/* datalink input socket */
	int4		i;
	int4		my_link;	/* my datalink # */
	int4		my_ncpus;	/* my number of CPUs */
	int4		ngbr_link;	/* neighbour link # */
	unsigned char	ngbr_haddr[4];	/* neighbour host address */
	unsigned char	my_haddr[4];	/* my host address */

	if ((argc % 3) != 0) {
		errno = EUSAGE;
		lampanic("dli_inet: <host> <link> <cpus> go in tuples");
	}

        /* Find out what my node ID is */

        *mynodeid = lam_ssi_boot_base_get_nodeid(lam_daemon_optd);

/*
 * Get my dli port number.
 */
	if (ao_taken(lam_daemon_optd, "p")) {
		ao_intparam(lam_daemon_optd, "p", 0, 0, &dli_port);
	} else {
		dli_port = DLDEFPORT;
	}
/*
 * Create the inter-node communication socket.
 */
	if ((dli_sd = sfh_sock_open_srv_inet_dgm(&dli_port)) < 0) {
		lampanic("dli_inet (sfh_sock_open_srv_inet_dgm)");
	}

        /* set socket to be close-on-exec */
        if (fcntl(dli_sd, F_SETFD, 1) != 0)
            lampanic("dli_inet: setting close-on-exec");

	if (sfh_sock_set_buf_size(dli_sd, SFH_INET, SO_RCVBUF, 
	    		sizeof(struct dlframe) * DOMAXPENDING)) {
		lampanic("dli_inet (sfh_sock_set_buf_size)");
	}
/*
 * Get my link #.
 */
	if (ao_taken(lam_daemon_optd, "n")) {
		ao_intparam(lam_daemon_optd, "n", 0, 0, &my_link);
	} else {
		my_link = 0;
	}
/*
 * Get my number of CPUs
 */
	if (ao_taken(lam_daemon_optd, "c")) {
		ao_intparam(lam_daemon_optd, "c", 0, 0, &my_ncpus);
	} else {
		my_ncpus = 1;
	}
/*
 * Get my host network address.
 */
	if (lam_gethostname(my_hname, MAXHOSTNAMELEN + 1)) {
		lampanic("dli_inet (lam_gethostname)");
	}

	if (getinetaddr(my_hname, my_haddr)) {
		lampanic("dli_inet (getinetaddr)");
	}

	*pndil = 0;

	for (i = 1; i < argc; i += 3) {
		*pndil = LAM_max(atoi(argv[i]), *pndil);
	}

	*pndil = LAM_max(my_link, *pndil);
	(*pndil)++;
/*
 * Create the links array.
 */
	*pdilinks = (struct dilink *) malloc((unsigned) *pndil *
					     sizeof(struct dilink));
	if (*pdilinks == 0) 
	    lampanic("dli_inet (malloc)");
	*pdiinfo = (struct diinfo *) malloc((unsigned) *pndil * 
					    sizeof(struct diinfo));
	if (*pdiinfo == 0)
	    lampanic("dli_inet (malloc)");
/*
 * Init all entries to invalid link.
 */
	for (i = 0; i < *pndil; ++i) {
		(*pdilinks)[i].dil_link = NOTLINKID;
		(*pdilinks)[i].dil_seqrecv = 0;
	}
/*
 * Parse active link information off the command line.
 */
	for (i = 0; i < (argc / 3); ++i) {
		if (getinetaddr(*argv++, ngbr_haddr))
				lampanic("dli_inet (getinetaddr)");
		ngbr_link = atoi(*argv++);

		if ((ngbr_link < 0) || (ngbr_link >= *pndil)) {
			errno = EBADLINK;
			lampanic("dli_inet");
		}

		(*pdilinks)[ngbr_link].dil_link = ngbr_link;
		sfh_sock_fill_inet_addr(ngbr_haddr, (int) dli_port,
				&((*pdilinks)[ngbr_link].dil_addr));

		(*pdiinfo)[ngbr_link].dii_num_cpus = atoi(*argv++);
		/* Update 19 Mar 2003: now we also need to get the
		   node type flags.  We need to get it here and put it
		   into the diinfo, like currently happens with the
		   di_bootcmd.  So I added the next line here
		   (atoi(+argv++)), but it probably doesn't work at
		   all -- I didn't do anything anywhere else to
		   actually place this argument on the command line.
		   This code apparently is unused right now, so I'm
		   not inspired to fix this... */
		(*pdiinfo)[ngbr_link].dii_node_type = atoi(*argv++);
	}
/*
 * Set my link.
 */
	(*pdilinks)[my_link].dil_link = my_link;
	(*pdiinfo)[my_link].dii_num_cpus = my_ncpus;
	/* Update 19 Mar 2003: I have no idea how to get the flags for
	   my node and put them here.  This code is apparently unused
	   right now, so I'm not inspired to fix this... */
	(*pdiinfo)[my_link].dii_node_type = NT_ME;
	sfh_sock_fill_inet_addr(my_haddr, (int) dli_port,
			&((*pdilinks)[my_link].dil_addr));

	return(dli_sd);
}
