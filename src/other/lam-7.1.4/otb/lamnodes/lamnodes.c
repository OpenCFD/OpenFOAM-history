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
 * $Id: lamnodes.c,v 1.5 2003/05/31 22:28:47 jsquyres Exp $
 *
 *	Function:	- retrieve hostnames from LAM nx nomenclature
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <args.h>
#include <dl_inet.h>
#include <portable.h>
#include <priority.h>
#include <terror.h>
#include <rreq.h>
#include <lam_config.h>
#include <lam_network.h>
#include <etc_misc.h>


/*
 * Local variables
 */
static char		name[1024];
static int4		nlinks;
static struct dolink	*links;
static int              fl_name;
static int              fl_lamnode;
static int              fl_showcpu;


static void
nodename(int node, int want_name)
{
    struct hostent *hent;

    if (node < 0 || node >= nlinks || links[node].dol_link == NOTLINKID) {
	strcpy(name, "invalid node");
    } else {
	hent = lam_gethostbyaddr((char *)&links[node].dol_addr.sin_addr,
			sizeof(struct in_addr), AF_INET);
	if (hent && want_name)
          lam_strncpy(name, hent->h_name, sizeof(name));
	else
          lam_strncpy(name, inet_ntoa(links[node].dol_addr.sin_addr), 
                      sizeof(name));
    }
}


static void
print_node(int node, char *name, int ncpus, int type)
{
    int showcpu = fl_showcpu;
    int flag = 0;

    if (showcpu && strcmp(name, "invalid node") == 0)
	showcpu = 0;

    if (fl_lamnode)
      printf("n%d\t", node);
    printf("%s", name);
    if (showcpu) {
      printf(":%d:", ncpus);
      if ((type & NT_WASTE) != 0) {
	printf("no_schedule");
	flag = 1;
      }
      if ((type & NT_ORIGIN) != 0) {
	if (flag)
	  printf(",");
	printf("origin");
	flag = 1;
      }
      if ((type & NT_ME) != 0) {
	if (flag)
	  printf(",");
	printf("this_node");
	flag = 1;
      }
    }
    printf("\n");
}


int
main(int argc, char **argv)
{
    int4 node;
    int n_index, n_flags;
    int *pnodes;
    struct route r;

    validopts("hinc");
    if (do_args(&argc, argv)) {
        show_help("lamnodes", "usage", NULL);
	exit(errno);
    }

    if (opt_taken('h')) {
        show_help("lamnodes", "usage", NULL);
	exit(0);
    }
    /* Do we want the IP number or name? */
    fl_name = !opt_taken('i');
    /* Do we want the node numbers (i.e., nx) shown? */
    fl_lamnode = !opt_taken('n');
    /* Do we want the cpu count shown? */
    fl_showcpu = !opt_taken('c');

    if (kinit(PRCMD)) {
      char hostname[MAXHOSTNAMELEN];
      gethostname(hostname, MAXHOSTNAMELEN);

      show_help(NULL, "no-lamd", "lamnodes", hostname, NULL);
      exit(LAM_EEXIT);
    }

    if (ldogetlinks(&links, &nlinks))
	lamfail("lamnodes (ldogetlinks)");

    if (nid_parse(&argc, argv) || (errno = (argc == 1) ? 0 : EUSAGE)) {
        show_help("lamnodes", "usage", NULL);
	kexit(errno);
    }

    pnodes = (int4 *) malloc((unsigned) (nlinks * sizeof(int4)));
    if (pnodes == 0) 
      lamfail("lamnodes (malloc)");

    if (getnodes(pnodes, nlinks, 0, NT_CAST)) 
      lamfail("lamnodes (getnodes)");

    nid_get(&n_index, &node, &n_flags);
    if (n_index < 0) {
	for (node = 0; node < nlinks; node++) {
	    nodename(node, fl_name);
	    r.r_nodeid = node;
	    getrent(&r);
	    print_node(node, name, r.r_ncpus, r.r_nodetype);
	}
    } else {
	do {
	    if (node == LOCAL)
		node = getnodeid();

	    if (node < 0) {
		printf("invalid CPU specifier\n");
	    } else {
		r.r_nodeid = node;
		getrent(&r);
		nodename(node, fl_name);
		print_node(node, name, r.r_ncpus, r.r_nodetype);
	    }

	    nid_get(&n_index, &node, &n_flags);
	} while (n_index);
    }
    free(pnodes);

    return 0;
}
