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
 * $Id: lamgrow.c,v 6.23 2003/08/06 01:06:00 jsquyres Exp $
 * 
 *	Function:	- adds a node to an existing LAM session
 */

#include <lam_config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <lam-ssi.h>
#include <lam-ssi-boot.h>
#include <all_opt.h>
#include <args.h>
#include <lamdebug.h>
#include <dl_inet.h>
#include <events.h>
#include <lamnet.h>
#include <portable.h>
#include <preq.h>
#include <priority.h>
#include <rreq.h>
#include <t_types.h>
#include <terror.h>
#include <etc_misc.h>
#include <laminternal.h>
#include <kio.h>
#include <mpisys.h>
#include <sfh.h>


/*
 * local variables
 */
static int fl_verbose = 0;
static int fl_debug = 0;
static char **main_argv = NULL;
static char *new_hostname = NULL;


/* 
 * local functions
 */
static void ensure_unique(int *pnodes, int nnodes);
static int do_lamboot(OPT *ad, struct sockaddr_in *addr, int *new_nodeid,
                      struct lamnode **plamnet, int *nlamnet);
static int add_links(OPT *ad, int *pnodes, int nnodes,
                     struct sockaddr_in new_in_addr, int new_nodeid);
static void bail(int);
static void cleanup(char *context);


/*
 * main
 */
int
main(int argc, char *argv[])
{
  int ret;
  int nnodes;
  int *pnodes;
  int new_nodeid;
  char *prefix = NULL;
  char *suffix = NULL;
  char hostname[MAXHOSTNAMELEN];
  OPT *ad = NULL;
  int nlamnet;
  struct lamnode *lamnet;
  struct sockaddr_in new_in_addr;

  /* Ensure that we are not root */

#ifndef LAM_ALLOW_RUN_AS_ROOT
  if (getuid() == 0 || geteuid() == 0) {
    show_help(NULL, "deny-root", NULL);
    exit(EACCES);
  }
#endif

  main_argv = sfh_argv_dup(argv);

  /* Parse the command line. */

  ad = ao_init();
  if (ad == 0) {
    ret = errno;
    perror("lamgrow (ao_init)");
    exit(ret);
  }
  ao_setflags(ad, AOPRESERVE_ARGV);
  ao_setopt1(ad, "dhvx", 0, 0, 0);
  ao_setopt(ad, "n", 0, 1, AOINT);
  ao_setopt(ad, "cpu", 0, 1, AOINT);
  ao_setopt(ad, "no-schedule", 0, 0, 0);
  ao_setopt(ad, "sessionprefix", 0, 1, 0);
  ao_setopt(ad, "sessionsuffix", 0, 1, 0);

  lam_ssi_base_open(ad);
  lam_ssi_base_ao_setup(ad);

  if (ao_parse(ad, &argc, argv)) {
    show_help("lamgrow", "usage", NULL);
    lam_ssi_base_close();
    exit(EUSAGE);
  }

  /* Just show help? */

  if (ao_taken(ad, "h")) {
    show_help("lamgrow", "usage", NULL);
    sfh_argv_free(main_argv);
    ao_free(ad);
    lam_ssi_base_close();
    kexit(0);
  }

  /* Check for -d and -v *before* we intialize the boot SSI */

  fl_debug = ao_taken(ad, "d");
  fl_verbose = ao_taken(ad, "v");

  if (fl_verbose)
    putenv("LAM_MPI_SSI_boot_verbose=level:0,stderr");
  if (fl_debug)
    putenv("LAM_MPI_SSI_boot_verbose=level:1000,stderr");

  /* Batch system gorp */

  if (ao_taken(ad, "sessionprefix")) {
    prefix = ao_param(ad, "sessionprefix", 0, 0);
  } else {
    prefix = NULL;
  }

  if (ao_taken(ad, "sessionsuffix")) {
    suffix = ao_param(ad, "sessionsuffix", 0, 0);
  } else {
    suffix = NULL;
  }
  lam_tmpdir_init_opt(prefix, suffix, 0);

  /* Let SSI parse all base SSI-related arguments.  It will print out
     its own error message if necessary, so exiting upon failure is
     sufficient here. */

  if (lam_ssi_base_ao_process_args(ad) != 0) {
    lam_ssi_base_close();
    kexit(errno);
  }

  /* Initialize LAM. */

  if (kenter("lamgrow", PRCMD)) {
    gethostname(hostname, MAXHOSTNAMELEN);
    hostname[MAXHOSTNAMELEN - 1] = '\0';

    show_help(NULL, "no-lamd", "lamgrow", hostname, NULL);
    lam_ssi_base_close();
    kexit(LAM_EEXIT);
  }

  /* Find out how many nodes are in the universe */

  nnodes = getnall();
  if (nnodes < 0)
    lamfail("lamgrow (getnall)");
  if (nnodes == 0) {
    errno = EIMPOSSIBLE;
    lam_ssi_base_close();
    lamfail("lamgrow (getnall)");
  }

  /* Alloc space for all of them */

  pnodes = malloc((unsigned) nnodes * sizeof(int4));
  if (pnodes == 0) {
    show_help(NULL, "system-call-fail", "malloc", NULL);
    lam_ssi_base_close();
    kexit(errno);
  }
  getall(pnodes, nnodes);

  /* Try to ensure that we are the only lamgrow running */

  ensure_unique(pnodes, nnodes);

  /* Start the remote process */

  if (do_lamboot(ad, &new_in_addr, &new_nodeid, &lamnet, &nlamnet) != 0) {
    lam_ssi_base_close();
    kexit(1);
  }

  /* Inform all pre-existing nodes of the new addition(s) */

  if (add_links(ad, pnodes, nnodes, new_in_addr, new_nodeid) != 0)
    kexit(1);

  /* All done */

  if (NULL != new_hostname)
    free(new_hostname);
  free(pnodes);
  sfh_argv_free(main_argv);
  ao_free(ad);
  kexit(0);
  return 0;
}


/*
 *	ensure_unique
 *
 *	Function:	- ensures that no other lamgrow is running
 */
static void
ensure_unique(int *pnodes, int nnodes)
{
  int i, j;
  int nps;
  struct pstate *pps;

  pps = malloc((unsigned) PMAX * sizeof(struct pstate));
  if (pps == 0) {
    show_help(NULL, "system-call-fail", "malloc", NULL);
    lam_ssi_base_close();
    kexit(errno);
  }

  /* Scan through the state of them all and ensure that we're the only
     lamnodes running */

  for (i = 0; i < nnodes; ++i) {
    nps = rpstate(pnodes[i], SELECT_ALL, 0, pps, PMAX);
    if (nps < 0)
      lamfail("lamgrow (rpstate)");

    for (j = 0; j < nps; ++j) {
      if ((!strcmp("lamgrow", pps[j].ps_name)) &&
	  ((pps[j].ps_pid != lam_getpid()) ||
	   (pnodes[i] != getnodeid()))) {
	fprintf(stderr, "lamgrow: already running on n%d\n", pnodes[i]);
	free(pps);
        lam_ssi_base_close();
	kexit(EUSAGE);
      }
    }
  }

  /* All done */

  free(pps);
}


/*
 * Invoke the boot SSI to start the new lamd
 */
static int
do_lamboot(OPT *ad, struct sockaddr_in *addr, int *new_nodeid,
           struct lamnode **plamnet, int *nlamnet)
{
  int i;
  int ret = 0;
  int iorigin;
  int num_started;
  struct lamnode *lamnet;

  /* Open the boot SSI */

  if (lam_ssi_boot_open(ad) != 0 ||
      lam_ssi_boot_select(LAM_SSI_BOOT_LOCATION_ROOT) != 0) {
    lam_ssi_base_close();
    exit(1);
  }

  /* Let the boot SSI parse the command line.  Relevant errors will be
     displayed before returning from this function. */

  if (lam_ssi_boot.lsba_parse_options(ad, 0) != 0) {
    lam_ssi_base_close();
    exit(1);
  }

  /* Let the boot SSI generate a list of nodes.  Relevant errors will
     be displayed before returning from this function. */

  if (ao_taken(ad, "n"))
    ao_intparam(ad, "n", 0, 0, &iorigin);
  else
    iorigin = -1;
  if (lam_ssi_boot.lsba_allocate_nodes(plamnet, nlamnet, &iorigin) != 0) {
    lam_ssi_base_close();
    exit(1);
  }
  lamnet = *plamnet;

  /* Find the new node */

  for (i = 0; i < *nlamnet; ++i)
    if ((lamnet[i].lnd_type & NT_BOOT) != 0) {
      *new_nodeid = i;
      break;
    }

  /* Modify the lamnet for the new node based on command line
     arguments */

  if (ao_taken(ad, "cpu"))
    ao_intparam(ad, "cpu", 0, 0, &lamnet[*new_nodeid].lnd_ncpus);
  else 
    lamnet[*new_nodeid].lnd_ncpus = 1;
  if (ao_taken(ad, "no-schedule"))
    lamnet[*new_nodeid].lnd_type |= NT_WASTE;

  /* Let the boot SSI do an error check on the nodes.  Relevant errors
     will be displayed before returning from this function. */

  if (lam_ssi_boot.lsba_verify_nodes(lamnet, *nlamnet) != 0) {
    lam_ssi_base_close();
    exit(1);
  }

  /* Setup the _kio struct */

  _kio.ki_origin = iorigin;

  /* Let the boot SSI prepare to boot.  Relevant errors will be
     displayed before returning from this function. */

  if (lam_ssi_boot.lsba_prepare_boot() != 0) {
    lam_ssi_base_close();
    exit(1);
  }

  /* Clean up on interrupt */

  if (signal(SIGINT, bail) == SIG_ERR) {
    show_help(NULL, "system-call-fail", "signal", NULL);
    lam_ssi_base_close();
    kexit(errno);
  }

  /* Now actually have the boot SSI boot the nodes.  Relevant errors
     will be displayed before returning from this function. */

  if (lam_ssi_boot.lsba_start_rte_procs(lamnet, *nlamnet, 
                                        LAM_SSI_BOOT_PROC_LAMD,
                                        &num_started) != 0 ||
      num_started != 1)
    bail(0);

  /* Now that we've done the startup protocols, the
     lamnet[*new_nodeid].lnd_addr has been updated to include the
     right UDP port.  So save a copy */

  *addr = lamnet[*new_nodeid].lnd_addr;
  new_hostname = strdup(lamnet[*new_nodeid].lnd_hname);

  /* Now that we've started everyone correctly, check to see if there
     any scheduleable nodes */

  for (i = 0; i < *nlamnet; ++i)
    if ((lamnet[i].lnd_type & NT_WASTE) == 0)
      break;
  if (i >= *nlamnet)
    show_help("boot", "no-schedulable-nodes", NULL);

  /* Free up resources */

  if (lam_ssi_boot.lsba_deallocate_nodes(&lamnet, nlamnet) != 0)
    ret = 1;

  /* All done with the boot SSI module (don't care about errors at
     this point) */

  lam_ssi_boot_close();
  lam_ssi_base_close();

  return ret;
}


/*
 * Tell all new lamds about the new link(s)
 */
static int
add_links(OPT *ad, int *pnodes, int nnodes, 
          struct sockaddr_in new_in_addr, int new_nodeid)
{
  int i;
  int type, ncpus;
  struct dilink newdil;		/* new node link */
  struct direq *pdiq;		/* dli request */
  struct direply *pdir;		/* dli reply */
  struct nmsg nhq;		/* request message */

  LAM_ZERO_ME(nhq);
  nhq.nh_flags = NOBUF;
  nhq.nh_msg = (char *) &newdil;
  pdiq = (struct direq *) nhq.nh_data;
  pdir = (struct direply *) nhq.nh_data;

  LAM_ZERO_ME(newdil);
  newdil.dil_link = ltot(new_nodeid);
  newdil.dil_addr = new_in_addr;

  type = (ao_taken(ad, "no-schedule") ? NT_WASTE : 0);
  ncpus = 1;
  if (ao_taken(ad, "cpu"))
    ao_intparam(ad, "cpu", 0, 0, &ncpus);

  if (fl_verbose || fl_debug)
    nodespin_init("update");

  /* Send the new link info to everyone */

  for (i = 0; i < nnodes; ++i) {
    if (fl_verbose || fl_debug)
      nodespin_next(i);

    pdiq->diq_req = DIQSETLINK;
    pdiq->diq_src_node = getnodeid();
    pdiq->diq_src_event = -lam_getpid();
    pdiq->diq_type = type;
    pdiq->diq_ncpus = ncpus;
    nhq.nh_node = pnodes[i];
    nhq.nh_event = EVDLI;
    nhq.nh_type = 0;
    nhq.nh_length = sizeof(struct dilink);
    if (getroute(&nhq))
      cleanup("lamgrow (getroute)");

    /* Re-route the local node. */

    if (nhq.nh_dl_event == nhq.nh_event) {
      nhq.nh_dl_event = EVDL0;
      nhq.nh_dl_link = getnodeid();
    }

    if (dsend(&nhq))
      cleanup("lamgrow (nsend)");

    nhq.nh_event = -lam_getpid();
    nhq.nh_length = 0;
    if (drecv(&nhq))
      cleanup("lamgrow (nrecv)");

    if (pdir->dir_reply != 0) {
      errno = pdir->dir_reply;
      cleanup("lamgrow (DIQSETLINK)");
    }
  }

  if (fl_verbose || fl_debug)
    nodespin_end();

  /* All done */

  return 0;
}


/*
 *	bail
 *
 *	Function:	- cleans up and bails out
 *	Returns:	- does not return, exits with error code
 */
static void
bail(int sig)
{
  show_help("boot", "about-to-lamwipe", "lamgrow", NULL);
  if (fl_verbose || fl_debug) 
    fprintf(stderr, "lamgrow did NOT complete successfully\n");
  cleanup(NULL);
}


/*
 *	cleanup
 *
 *	Function:	- cleans up the mess after something goes wrong
 *	Accepts:	- context error message
 */
static void
cleanup(char *context)
{
  int fd;
  int err_save;
  int cmdn = 0;
  char **cmdv = NULL;
  char file[LAM_PATH_MAX];

  err_save = errno;
  if (context != NULL)
    printf(context);

  if (NULL != new_hostname) {
    sfh_argv_add(&cmdn, &cmdv, DEFTWIPE);
    if (fl_debug)
      sfh_argv_add(&cmdn, &cmdv, "-d");
    if (fl_verbose)
      sfh_argv_add(&cmdn, &cmdv, "-v");

    strcpy(file, "/tmp/lam-wipe-schema-XXXXXX");
    umask(02);
    if ((fd = mkstemp(file)) == -1)
      kexit(err_save);
    sfh_argv_add(&cmdn, &cmdv, file);

    write(fd, new_hostname, strlen(new_hostname));
    write(fd, "\n", 1);
    close(fd);

    _lam_few(cmdv);
    unlink(file);
  }

  kexit(err_save);
}

