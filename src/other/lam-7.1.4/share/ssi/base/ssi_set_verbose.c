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
 * $Id: ssi_set_verbose.c,v 1.11 2003/08/21 12:17:44 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <syslog.h>

#include <typical.h>
#include <lam-ssi.h>
#include <lamdebug.h>
#include <net.h>
#include <etc_misc.h>


/*
 * Local functions
 */
static void parse_env(char *e, lam_debug_stream_info_t *lds, int *level);


int
lam_ssi_base_set_verbose(int index, lam_debug_stream_info_t *lds,
			 int *level, int *did)
{
  char *e;
  char prefix[BUFSIZ];
  char mpi_file_suffix[BUFSIZ];

  /* If the desired environment variable is defined, parse it, then
     setup the debug stream as appropriate */

  if ((e = lam_ssi_base_param_lookup_string(index)) != NULL) {
    parse_env(e, lds, level);
    free(e);

    /* Setup a reasonable prefix */

    if (lds->lds_prefix == NULL)
      lds->lds_prefix = "";

    snprintf(prefix, sizeof(prefix), "n%d<%d> ssi:%s", getnodeid(),
	     lam_getpid(), lds->lds_prefix);
    lds->lds_prefix = prefix;

    /* If this is an MPI job, we need to add the MPI_COMM_WORLD rank
       number to the file suffix because there may be more than one of
       them running on this node.  Granted, this can happen for
       non-MPI jobs as well, but the most common case is for multiple
       MPI jobs, not multiple [pure] LAM jobs. */

    if ((e = getenv("LAMRANK")) != NULL) {
      snprintf(mpi_file_suffix, sizeof(mpi_file_suffix), "%s-%s", 
	       e, lds->lds_file_suffix);
      lds->lds_file_suffix = mpi_file_suffix;
    }

    /* Now we're all setup -- open the stream. */

    *did = lam_debug_open(lds);
  }

  return 0;
}


/*
 * Parse the value of an environment variable describing verbosity
 */
static void 
parse_env(char *e, lam_debug_stream_info_t *lds, int *level)
{
  int found = 0;
  char *edup = strdup(e);
  char *ptr = edup, *next;

  /* Load up defaults */

  if (lds->lds_fl_syslog == -1)
    lds->lds_fl_syslog = 0;
  if (lds->lds_syslog_priority == -1)
    lds->lds_syslog_priority = LOG_INFO;
  if (lds->lds_syslog_ident == NULL)
    lds->lds_syslog_ident = "lam";
  if (lds->lds_fl_stdout == -1)
    lds->lds_fl_stdout = 0;
  if (lds->lds_fl_file == -1)
    lds->lds_fl_file = 0;
  if (lds->lds_fl_file_append == -1)
    lds->lds_fl_file_append = 1; 
  if (lds->lds_file_suffix == NULL)
    lds->lds_file_suffix = "ssi.txt";
  if (*level == -1)
    *level = 0;

  /* Now parse the environment variable */

  while (ptr != NULL && strlen(ptr) > 0) {
    next = strchr(ptr, ',');
    if (next != NULL)
      *next = '\0';

    if (strcasecmp(ptr, "syslog") == 0)
      found = lds->lds_fl_syslog = 1;
    else if (strncasecmp(ptr, "syslogpri:", 10) == 0) {
      found = lds->lds_fl_syslog = 1;
      if (strcasecmp(ptr + 10, "notice") == 0)
	lds->lds_syslog_priority = LOG_NOTICE;
      else if (strcasecmp(ptr + 10, "INFO") == 0)
	lds->lds_syslog_priority = LOG_INFO;
      else if (strcasecmp(ptr + 10, "DEBUG") == 0)
	lds->lds_syslog_priority = LOG_DEBUG;
    } else if (strncasecmp(ptr, "syslogid:", 9) == 0) {
      found = lds->lds_fl_syslog = 1;
      lds->lds_syslog_ident = ptr + 9;
    }

    else if (strcasecmp(ptr, "stdout") == 0)
      found = lds->lds_fl_stdout = 1;
    else if (strcasecmp(ptr, "stderr") == 0)
      found = lds->lds_fl_stderr = 1;

    else if (strcasecmp(ptr, "file") == 0)
      found = lds->lds_fl_file = 1;
    else if (strncasecmp(ptr, "file:", 5) == 0) {
      found = lds->lds_fl_file = 1;
      lds->lds_file_suffix = ptr + 5;
    } else if (strcasecmp(ptr, "fileappend") == 0) {
      found = lds->lds_fl_file = 1;
      lds->lds_fl_file_append = 1;

    } else if (strncasecmp(ptr, "level", 5) == 0) {
      *level = 1;
      if (ptr[5] == ':')
	*level = atoi(ptr + 6);
    }

    if (next == NULL)
      break;
    ptr = next + 1;
  }

  /* If we didn't find anything, and stderr wasn't set, turn it on */

  if (e != NULL && found == 0 && *level >= 0 && lds->lds_fl_stderr == -1)
      lds->lds_fl_stderr = 1;

  free(edup);
}
