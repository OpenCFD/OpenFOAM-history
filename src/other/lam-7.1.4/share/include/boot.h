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
 * $Id: boot.h,v 1.14 2003/04/15 23:28:12 vsahay Exp $
 * 
 *	Function:	- constants associated with booting
 */

#ifndef LAM_BOOT_H
#define LAM_BOOT_H

#include <stdio.h>

#include <lamnet.h>
#include <all_list.h>
#include <all_opt.h>

/*
 * default configuration files
 */
#define DEFFCONFIGH	"lam-conf.lamd"


/*
 * default tools
 */
#define DEFTRESETH	"tkill"


/*
 * share/boot/bhostparse.c 
 */
#define LAM_MAX_PARSE_LINE 256	       /* input line size */
int bhostparse(char *filename, struct lamnode **plamnet, int *pnlamnet);
void bhostparseline(char linebuf[LAM_MAX_PARSE_LINE], int lineno,
		    char *linedata, LIST *list);
void lam_deallocate_nodes(struct lamnode **nodes, int *nnodes); 


/*
 * share/boot/proc_schema.c
 */
int psc_parse(FILE *fp, LIST **list_psc, char **argv_var);
struct psc *psc_find(LIST *list_psc);
struct psc *psc_fill(LIST *list_psc, char *prefix);
int cnfexec(char *argv[]);

/*
 * share/boot/hbootparse.c
 */
int hbootparse(int debug, OPT *ad, char *inet_topo, 
	       char *rtr_topo, LIST **proc_list);

#endif /* LAM_BOOT_H */
