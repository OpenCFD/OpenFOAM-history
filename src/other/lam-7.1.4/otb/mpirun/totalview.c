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
 * $Id: totalview.c,v 6.9 2003/07/17 18:01:41 jsquyres Exp $
 *
 *	Function:	- functions required for totalview support
 *
 * You can also find lot of usefull documentation at
 * http://www-unix.mcs.anl.gov/mpi/mpi-debug/
 * http://www-unix.mcs.anl.gov/mpi/mpi-debug/mpich-attach.txt 
 *
 */

#include <lam_config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <lam-totalview.h>
#include <freq.h>
#include <lam_network.h>
#include <app_mgmt.h>
#include <all_list.h>
#include <sfh.h>
#include <all_opt.h>
#include <all_list.h>
#include <app_schema.h>
#include <dl_inet.h>
#include <kio.h>
#include <etc_misc.h>

/* Structure defination for MPIR_PROCDESC. We need to put it here
   instead of lam-totalview.h because the way to guarantee that
   TotalView has digested the struct definition of MPIR_PROCDESC
   before it is needed is to place the definition in the same file as
   MPIR_Breakpoint */

typedef struct {
  char *host_name;
  char *executable_name;
  int pid;  
} MPIR_PROCDESC;


/*
 * private functions 
 */
static void build_tv_process_table(LIST *app); 
static void nodename(int node, int want_name);

/* 
 * external variables
 */
extern int world_n;
extern struct _gps *mpiworld;


/*
 * Symbols exported for TotalView to see (must be public symbols)
 */
/* Variables */
volatile int MPIR_being_debugged = 0;
volatile MPIR_PROCDESC *MPIR_proctable = NULL;
volatile int MPIR_proctable_size = 0;
volatile int MPIR_debug_state = 0;
volatile int MPIR_i_am_starter = 0;
volatile int MPIR_partial_attach_ok = 0;

/* Functions */
void *MPIR_Breakpoint(void);

/*
 * private variables
 */
/* required to get node/link inforamtion from lamd */
static int4 nlinks;
static struct dolink *links;
static char name[1024];
/* TV command line args if we have to exec TV */
static int tv_argc = 0;
static char **tv_argv = NULL;


/*
 * Initialize the interface
 */
void
lam_tv_init(int argc, char *argv[], OPT *opt)
{
  int i;

  /* Setup convenience command line argument */

  ao_setopt(opt, "tv", 0, 0, 0);

  /* Build the argv for launching totalview (even though we might not
     use it) */

  sfh_argv_add(&tv_argc, &tv_argv, "totalview");
  sfh_argv_add(&tv_argc, &tv_argv, "mpirun");
  sfh_argv_add(&tv_argc, &tv_argv, "-a");
  for (i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "-tv") == 0)
      continue;
    sfh_argv_add(&tv_argc, &tv_argv, argv[i]);
  }
}


/*
 * Be good citizens and free the memory that we have allocated, since
 * it will no longer be used
 */
void
lam_tv_finalize(void)
{
  sfh_argv_free(tv_argv);
}


/*
 * See if the convenience argv option -tv was given
 */
void
lam_tv_check(OPT *opt)
{
  /* Check for TotalView debug request */	

  if (!ao_taken(opt, "tv"))
    return;

  /* Yes, we want totalview support -- so launch the tv_argv that we
     built from above */

  execvp(tv_argv[0], tv_argv);
  show_help("mpirun", "totalview-exec-failed", NULL);
  exit(1);
}


/*
 * If we're being debugged, launch the whole attach-totalview-debugger
 * stuff
 */
void
lam_tv_attach(LIST *app)
{
  /* always build the process table so that debuggers that support
   * joining the party late (like Absoft's parallel debugger) can
   * still find everyone
   */
  build_tv_process_table(app);

  if (MPIR_being_debugged != 1)
    return;
  
  /* Signal the debugger to attach to the executable images on all the
     nodes */
  
  MPIR_debug_state = PROCESSES_SPAWNED;
  
  MPIR_Breakpoint();

}


/* Build the process table which will be used by totalview to attach
 * to all ranks it wants to attach to */ 
static void 
build_tv_process_table(LIST *app) 
{
  struct aschema *pp;  /* ptr process entry */
  int i, j;
  int size_hostname;
  char *cwd = NULL;
  char *executable_path = NULL;

  /* Get link table from lamd */
  if (ldogetlinks(&links, &nlinks)) {
    show_help(NULL, "lib-call-fail", "ldogetlinks", NULL);
    exit(1);
  }

  /* malloc TV process table */
  MPIR_proctable_size = world_n;
  MPIR_proctable = (MPIR_PROCDESC *) malloc ( MPIR_proctable_size * 
					      sizeof(MPIR_PROCDESC) );
  if (MPIR_proctable == NULL) {
    show_help(NULL, "system-call-fail", "malloc", NULL);
    exit(1);
  }

  /* allocate process table and fill pid and hostname for each rank */
  for (i = 0; i < MPIR_proctable_size; ++i) {
    /* fill process id */
    MPIR_proctable[i].pid = mpiworld[i].gps_pid;
    
    /* fill host name */
    nodename(mpiworld[i].gps_node,1);
    if (strcmp(name,"invalid node") == 0) {
      show_help("mpirun", "totalview-invalid-hostname", NULL);
      exit(1);
    }
    
    size_hostname = strlen(name);
    MPIR_proctable[i].host_name = (char *) malloc(sizeof(char) *
						  (size_hostname + 1));
    if (MPIR_proctable[i].host_name == NULL) {
      show_help(NULL, "system-call-fail", "malloc", NULL);
      exit(1);
    }

    strcpy(MPIR_proctable[i].host_name, name);
  } 

  /* fill executable name into process table */
  /* MRC: This is a bit tricky ..  not sure if we are covering all
     possible cases */

  for (pp = al_top(app), j = 0; ( (j < MPIR_proctable_size) && pp ); 
       ++j, pp = al_next(app, pp)) {

    /* Set up current working directory */
    if (pp->asc_env->ape_wrkdir) 
      cwd = pp->asc_env->ape_wrkdir;
    else
       cwd = getworkdir();

    /* If user specified absolute path names then use them directly */
    if ( pp->asc_args->apa_argv[0][0] == '/' ) {
      MPIR_proctable[j].executable_name = 
	(char *) malloc(sizeof(char) * ( strlen(pp->asc_args->apa_argv[0]) 
					 + 1 ) );
      if (MPIR_proctable[j].executable_name == NULL) {
	show_help(NULL, "system-call-fail", "malloc", NULL);
	exit(1);
      }
      strcpy(MPIR_proctable[j].executable_name, pp->asc_args->apa_argv[0]);
      continue;
    }
    
    /* else if user specified relative path name */
    if ( strstr((pp->asc_args->apa_argv[0])+1, "/") != NULL ) {
      MPIR_proctable[j].executable_name = 
	(char *) malloc(sizeof(char) * (strlen(cwd) + 
					strlen(pp->asc_args->apa_argv[0])
					+ 2));
      if (MPIR_proctable[j].executable_name == NULL) {
	show_help(NULL, "system-call-fail", "malloc", NULL);
	exit(1);
      }
      strcpy(MPIR_proctable[j].executable_name, cwd);
      strcat(MPIR_proctable[j].executable_name, "/");
      strcat(MPIR_proctable[j].executable_name, 
	     pp->asc_args->apa_argv[0]);
      
    } else {
      /* user specified just the executable name */
      executable_path = lam_rfpathfind(pp->asc_args->apa_argv[0], 
				       pp->asc_env->ape_wrkdir,
				       pp->asc_node);
      if (executable_path == NULL) {
	/* executable was not found in path on remote lamd, so we
           assume its in current directory on remote node .. trying to
           be good to some stupid users */
	MPIR_proctable[j].executable_name = 
	  (char *) malloc(sizeof(char) * (strlen(cwd) + 
					  strlen(pp->asc_args->apa_argv[0])
					  + 2));
	if (MPIR_proctable[j].executable_name == NULL) {
	  show_help(NULL, "system-call-fail", "malloc", NULL);
	  exit(1);
	}
	strcpy(MPIR_proctable[j].executable_name, cwd);
	strcat(MPIR_proctable[j].executable_name, "/");
	strcat(MPIR_proctable[j].executable_name, pp->asc_args->apa_argv[0]);
      }
      else {
	/* executable was found in path on remote lamd, so we append
           the executable name to the dir in which it was found to get
           absolute path */
	MPIR_proctable[j].executable_name = 
	  (char *) malloc(sizeof(char) * (strlen(executable_path) + 1 ));
	if (MPIR_proctable[j].executable_name == NULL) {
	  show_help(NULL, "system-call-fail", "malloc", NULL);
	  exit(1);
	}
	strcpy(MPIR_proctable[j].executable_name, executable_path);
      }
    }
  } /* for (....) */
}


/* 
 * MPIR_Breakpoint
 * 
 * Function: Dummy function to allow the debugger to set a breakpoint
 *           at this function
 * 
 */
void *
MPIR_Breakpoint(void)
{
  /* Don't need anything here, except maybe to silence some compiler
     warnings */

  if (MPIR_proctable == NULL) {
    show_help("mpirun", "totalview-invalid-process_table", NULL);
  }

  return (void *) 0;

}


/* 
 * nodename
 * 
 * Function: Extracts node name/IP from node ID
 * 
 */
static void
nodename(int node, int want_name)
{
  struct hostent *hent;

  if (node < 0 || node >= nlinks ||
      links[node].dol_link == NOTLINKID) {
    strcpy(name, "invalid node");
  } else {
    hent = lam_gethostbyaddr((char *)&links[node].dol_addr.sin_addr,
			     sizeof(struct in_addr), AF_INET);
    if (hent && want_name)
      lam_strncpy(name, hent->h_name, sizeof(name));
    else
      sprintf(name, "%s", inet_ntoa(links[node].dol_addr.sin_addr));
  }
}
