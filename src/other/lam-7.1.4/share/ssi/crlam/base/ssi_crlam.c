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
 * $Id: ssi_crlam.c,v 1.4 2003/09/06 10:19:10 jsquyres Exp $
 *
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <sfh.h>
#include <ndi.h>

#include <lam-ssi-cr.h>


/*
 * local variables
 */
static char **restart_argv = NULL;
static int restart_argc = 0;


/* 
 * private functions
 */
static int sync_mkstemp(const char *filename);
static void i2a(char *string_start, int str_len, int num);


/*
 * external functions
 */


/*
 * external variables
 */
extern char **environ;

#define signal_puts(S)  write(STDOUT_FILENO, (S), strlen(S))


int 
lam_ssi_crlam_base_checkpoint(struct _gps *cr_world, int cr_world_n)
{
  /*
   * Simply turn around and call the module-specific checkpoint routine. In
   * future, there might be glue functionality that needs to be performed
   * here.  Using this function ensures that that transition will be easy.
   */
  return(lam_ssi_crlam.lscrla_checkpoint());
}


int
lam_ssi_crlam_base_continue(void)
{
  /*
   * Simply turn around and call the module-specific checkpoint routine. In
   * future, there might be glue functionality that needs to be performed
   * here. Using this function ensures that that transition will be easy.
   */
  return(lam_ssi_crlam.lscrla_continue()); /* Nothing do be done here. */
}


int
lam_ssi_crlam_base_restart(char *executable, char *app_schema)
{
  /* First, reopen all debug streams. */
  lam_debug_reopen_all();

  /* Turn around and call the module-specific checkpoint routine. */
  return(lam_ssi_crlam.lscrla_restart(executable, app_schema));
}


int
lam_ssi_crlam_base_do_exec(char *executable, char *app_schema)
{
  int fd;
  char  schema_file[] = "/tmp/schema.XXXXXX\0";

  /*
   * Write out app schema to file
   */
  fd = sync_mkstemp(schema_file);

  if (app_schema == NULL) {
    signal_puts("CRLAM_SYNC: app_schema is NULL\n");
    return LAMERROR;
  }

  /*
   * do we have to handle sig interruption here??
   */
  if (write(fd, app_schema, strlen(app_schema)) < 0) {
    /* error writing to app_schema file */
    signal_puts("Error writing schema_file\n");
    return LAMERROR;
  }
  close(fd);

  /*
   * adjust the last thingy
   */
  restart_argv[restart_argc - 1] = schema_file;

  /*
   * exec a brand new mpirun
   *
   * NOTE: POSIX.1 guarantees only execle and execve to be reentrant
   * This means we need a full path to mpirun. So, we assume the
   * installation directory is still correct.
   *
   * Note that BLCR will have a whole boatload of signals blocked in
   * this signal callback.  We do *not* unblock them here because of a
   * potential race condition (i.e., they're blocked for a reason).
   * But we always are invoking mpirun, so we'll always just reset the
   * blocked signals list near the beginning of mpirun.
   */
  if (execve(executable, restart_argv, environ) < 0) {
    /* error in exec */
    signal_puts("Error in exec\n");
    return LAMERROR;
  }

  /* Never reached */
  return 0;
}


int
lam_ssi_crlam_base_create_restart_argv(char **argv, OPT *ad)
{
  int i, j;
  int argc = sfh_argv_count(argv);
  int nparams;

  sfh_argv_add(&restart_argc, &restart_argv, argv[0]);

  for (i = 1; i < argc; i++) {
    if (ao_exam_isopt(ad, argv[i] + 1)) {
      /*
       * It's an option
       */
      if (ao_exam_nparams(ad, argv[i] + 1, &nparams))
        return LAMERROR;

      /* Do we have to ignore it? - ignore either because we are going to
       * add in a bit or because it can't work with schemas...*/
      if ((! strcmp(argv[i] + 1, "c")) ||
          (! strcmp(argv[i] + 1, "np")) ||
          (! strcmp(argv[i] + 1, "s")) ||
          (! strcmp(argv[i] + 1, "rmschema"))) {
        i += nparams;
        continue;
      }

      /* add the option */
      if (sfh_argv_add(&restart_argc, &restart_argv, argv[i])) {
        return LAMERROR;
      }

      /* Eat the params to the option.  If this is a variable, eat
       * until the next thing that could be an option (starts with
       * -, which handles the -- case).  This isn't perfect, but
       * then options parsing shouldn't require an AI to figure out
       * what the user wanted. */
      if (nparams != AOVARNUM) {
        for (j = 0 ; j < nparams ; ++j) {
          if (sfh_argv_add(&restart_argc, &restart_argv, argv[i + 1]))
            return LAMERROR;
          ++i;
        }

      } else {
        /* Note: This case is not well tested, as it is not
         * currently used anywhere in the LAM/MPI tree.  But it
         * might be some day! */
        while ((argv[i + 1])[0] != '-') {
          if (sfh_argv_add(&restart_argc, &restart_argv, argv[i + 1]))
            return LAMERROR;
          ++i;
        }
      }

    } else if (ndi_parse1(argv[i]) != NOTNODEID) {
      /*
       * It's a node identifier - ignore
       */
      continue;
    } else {
      /* binary - we be done */
      break;
    }
  }

  if (sfh_argv_add(&restart_argc, &restart_argv, "-rmschema"))
    return LAMERROR;
  if (sfh_argv_add(&restart_argc, &restart_argv, "/tmp/schema.XXXXXX"))
    return LAMERROR;

  return 0;
}


/*
 * sync_mkstemp
 *
 * Function:
 *  Implement mkstemp in signal safe manner
 *
 * Notes:
 *  This probably isn't as good as mkstemp, but it should work...
 *  No restriction on number of X's in string, must be > 0 and
 *  end string
 */
static int
sync_mkstemp(const char *filename)
{
  int i = 0;
  int space_chars = 0;
  int start_space = 0;
  int space = 0;
  int fd;

  /* Short-circuit */
  if (filename == NULL) {
    errno = EINVAL;
    return -1;
  }

  /* only replace Xs - figure out how many iterations to try... */
  while ((filename[i] != '\0') && (filename[i] != 'X')) {
    ++i;
    ++start_space;
  }
  while ((filename[i] != '\0') && (filename[i] == 'X')) {
    ++i;
    ++space_chars;
  }
  if (filename[i] != '\0') {
    errno = EINVAL;
    return -1;
  }

  space = 1;
  for (i = 0 ; i < space_chars ; ++i) {
    space *= 10;
  }

  /* Now, actually try to create / open a file */
  for (i = 0 ; i < space ; ++i) {
    i2a((char *) &filename[start_space], space_chars, i);

    fd = open(filename, O_CREAT|O_EXCL|O_RDWR, 0666);
    if ((fd < 0) && (errno == EEXIST)) {
      /* already exists - try again... */
      continue;
    } else if (fd < 0) {
      /* error opening file - for now, try again... */
      continue;
    } else {
      return fd;
    }
  }

  errno = EEXIST;
  return -1;
}


/*
 * i2a
 *
 * Function:
 *  Signal safe (but very minimal) int -> string converstion
 *
 * Notes:
 *  Can truncate the number :(
 */
static void
i2a(char *s, int str_len, int orig_num)
{
  int i;
  int mod;
  int num = 0;

  /* First, make everything 0 */
  for (i = 0 ; i < str_len ; ++i) {
    s[i] = '0';
  }

  /* figure out digit for position i */
  for (i = str_len -1, mod = 10 ; i >= 0 ; --i, mod *= 10) {
    num = orig_num % mod;
    num = num / (mod / 10);
    s[i] += num;
  }
}
