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
 *  $Id: kill.c,v 6.41 2004/03/12 13:21:49 jsquyres Exp $
 *
 *  Function: - kill file useful routines
 */

#include <stdio.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netdb.h>      /* MAXHOSTNAMELEN in Solaris */
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
   
#include <lamdebug.h>
#include <laminternal.h>
#include <terror.h>
#include <typical.h>
#include <etc_misc.h>


static int check_dir_perms(void);
static char* lam_get_batchid(void);

#define LAM_DEFAULT_TMPDIR "/tmp"
#define LAM_KILLFILE "lam-killfile"
#define LAM_SOCKET "lam-kernel-socket"
#define LAM_IOSOCKET "lam-io-socket"

#define LAM_PREFIX_ENV "LAM_MPI_SESSION_PREFIX"
#define LAM_SUFFIX_ENV "LAM_MPI_SESSION_SUFFIX"

static int initialized = 0;
static char lam_tmpdir[MAXPATHLEN];

static char lam_tmpdir_suffix[MAXPATHLEN];
static char lam_tmpdir_prefix[MAXPATHLEN];


/*
 *  lam_tmpdir_init
 *
 *  Function: - set up tmpdir for LAM to store stuff
 *      - creates directory, with "good" perms
 *  Accepts:  - suffix to use
 *  Returns:  - 0 or LAMERROR
 *      Notes:          - see lam_tmpdir_init()
 *                      - see lam_tmpdir_setsuffix()
 *                      - directory in form of:
 *                         <tmp prefix>/<user>@<host>[-<suffix>]/
 */
int
lam_tmpdir_init(char *prefix, char *suffix)
{
  return lam_tmpdir_init_opt(prefix, suffix, 1);
}

int
lam_tmpdir_init_opt(char *prefix, char *suffix, int create)
{
  char *tmpsuffix = NULL;
  char *tmpprefix = NULL;
  char *tmp;
  char host[MAXHOSTNAMELEN + 1];
  char *user;
  struct passwd *pwdent;
  size_t len;

  /* We only do this once.  see, if we do it multiple times, then we
   *  could have two different names in the same process for the same
   *  socket.  Which would be bad...  So you only get a chance to pass
   *  the -b *BEFORE* you call kinit... 
   */
  if (initialized != 0)
    return 0;

  initialized = 1;

  /* In case something goes wrong... */
  snprintf(lam_tmpdir, sizeof(lam_tmpdir), "%s/lam-failure@failure",
     LAM_DEFAULT_TMPDIR);

  /* set up the prefix */
  if (prefix != NULL) {
    tmpprefix = strdup(prefix);
  } else if ((tmp = getenv(LAM_PREFIX_ENV)) != NULL) {
    tmpprefix = strdup(tmp);
  } else if ((tmp = getenv("TMPDIR")) != NULL) {
    tmpprefix = strdup(tmp);
  } else {
    tmpprefix = strdup(LAM_DEFAULT_TMPDIR);
  }

  /* set up the prefix environment */
  /* after careful consideration, it was decided not to put this
   * back in the environment.  Processes that inherit from this
   * process will have the right things to get the same answer.  But
   * this became a major issue all around, because the session
   * prefix is not always the same across nodes and setting the env
   * caused MPIRUN to push the env variable out, which was just
   * causing major badness. 
   */

  /* set up the middle */
  if ((pwdent = getpwuid(getuid())) != 0) {
    user = strdup(pwdent->pw_name);
  } else {
    user = strdup("unknown");
  }

  /* set up the suffix */
  if (gethostname(host, sizeof(host))) {
    host[0] = '\0';
  }

  if (suffix != NULL) {
    tmpsuffix = strdup(suffix);
  } else if ((tmp = getenv(LAM_SUFFIX_ENV)) != NULL) {
    tmpsuffix = strdup(tmp);
  } else if ((tmp = getenv("LAM_MPI_SOCKET_SUFFIX")) != NULL) {
    /* This is here for historical reasons - used to only have one
     * socket, without own directory, in /tmp/.  Use of SOCKET_SUFFIX
     * is deprecated, but it will not go away any time soon */
    tmpsuffix = strdup(tmp);
  } else if ((tmp = lam_get_batchid()) != NULL) {
    tmpsuffix = tmp;
  } else {
    tmpsuffix = NULL;
  }

  /* set up suffix environment */
  if (tmpsuffix != NULL) {
    len = strlen(tmpsuffix) + strlen(LAM_SUFFIX_ENV) + 2;
    tmp = (char*) malloc(len);
    if (tmp == NULL)
      return LAMERROR;
    snprintf(tmp, len, "%s=%s", LAM_SUFFIX_ENV, tmpsuffix);
    putenv(tmp);
  }

  /* we now have a beginning, middle, and end - set everything up. */
  if (tmpsuffix == NULL) {
    snprintf(lam_tmpdir, sizeof(lam_tmpdir), "%s/lam-%s@%s",
       tmpprefix, user, host);
  } else {
    snprintf(lam_tmpdir, sizeof(lam_tmpdir), "%s/lam-%s@%s-%s",
       tmpprefix, user, host, tmpsuffix);
  }

  /* make the freaking directory (or check perms or whatever) */
  if (create != 0) {
    if (check_dir_perms() == LAMERROR) {
      /* try never to leave a directory around... */
      lam_tmpdir_remove();
      return LAMERROR;
    }
  }

  /* set up the storage for the get_{suffix,prefix} */
  if (tmpprefix != NULL) {
    snprintf(lam_tmpdir_prefix, sizeof(lam_tmpdir_prefix), 
             "%s", tmpprefix);
  } else {
    lam_tmpdir_prefix[0] = '\0';
  }

  if (tmpsuffix != NULL) {
    snprintf(lam_tmpdir_suffix, sizeof(lam_tmpdir_suffix),
             "%s", tmpsuffix);
  } else {
    lam_tmpdir_suffix[0] = '\0';
  }

  /* clean up */
  if (tmpprefix != NULL)
    free(tmpprefix);
  if (tmpsuffix != NULL)
    free(tmpsuffix);
  if (user != NULL)
    free(user);

  return 0;
}


/*
 * lam_get_tmpdir
 *
 *  Function: - get path to internal LAM tmpdir
 *  Returns:  - NULL or pathname (must be freed by caller)
 */
char*
lam_get_tmpdir(void)
{
  char *ret;

  ret = malloc(MAXPATHLEN);
  if (ret == NULL)
    return ret;

  snprintf(ret, MAXPATHLEN, "%s", lam_tmpdir);

  return ret;
}


/*
 * lam_get_tmpdir_prefix
 *
 *  Function: - 
 *  Returns:  - NULL or pathname (must be freed by caller)
 */
char*
lam_get_tmpdir_prefix(void)
{
  char *ret;

  if (lam_tmpdir_prefix[0] == '\0')
    return NULL;

  ret = malloc(MAXPATHLEN);
  if (ret == NULL)
    return ret;

  snprintf(ret, MAXPATHLEN, "%s", lam_tmpdir_prefix);

  return ret;
}


/*
 * lam_get_tmpdir_suffix
 *
 *  Function: - 
 *  Returns:  - NULL or pathname (must be freed by caller)
 */
char*
lam_get_tmpdir_suffix(void)
{
  char *ret;

  if (lam_tmpdir_suffix[0] == '\0')
    return NULL;

  ret = malloc(MAXPATHLEN);
  if (ret == NULL)
    return ret;

  snprintf(ret, MAXPATHLEN, "%s", lam_tmpdir_suffix);

  return ret;
}


/*
 * lam_get_killfile
 *
 *  Function: - get full path name to killfile
 *  Returns:  - NULL or filename (must be freed by caller)
 */
char*
lam_get_killfile(void)
{
  char *ret;

  ret = malloc(MAXPATHLEN);
  if (ret == NULL)
    return ret;

  snprintf(ret, MAXPATHLEN, "%s/%s", lam_tmpdir, LAM_KILLFILE);

  return ret;
}



/*
 * lam_get_sockname
 *
 *  Function: - get full path name to kernel socket name
 *  Returns:  - NULL or filename (must be freed by caller)
 */
char*
lam_get_sockname(void)
{
  char *ret;

  ret = malloc(MAXPATHLEN);
  if (ret == NULL)
    return ret;

  snprintf(ret, MAXPATHLEN, "%s/%s", lam_tmpdir, LAM_SOCKET);

  return ret;
}


/*
 * lam_get_iosockname
 *
 *  Function: - get full path name to I/O socket name
 *  Returns:  - NULL or filename (must be freed by caller)
 */
char*
lam_get_iosockname(void)
{
  char *ret;

  ret = malloc(MAXPATHLEN);
  if (ret == NULL)
    return ret;

  snprintf(ret, MAXPATHLEN, "%s/%s", lam_tmpdir, LAM_IOSOCKET);

  return ret;
}


/*
 *  lam_mktmpid
 *
 *  Function: - create temporary file name based on an id
 *      - returns error if insufficient space
 *  Accepts:  - id number to make name unique
 *      - temporary file name (out)
 *      - size of storage for temporary name
 *  Returns:  - 0 or LAMERROR
 */
int
lam_mktmpid(int id, char *name, unint n)
{
  if (check_dir_perms() == LAMERROR) {
    /* try never to leave a directory around... */
    lam_tmpdir_remove();
    memset(name, 0, n);
    return LAMERROR;
  }

  snprintf(name, n, "%s/lam-%d", lam_tmpdir, id);
  
  return 0;
}


/*
 * remove the entire directory where the lam socket lives
 */
void
lam_tmpdir_remove(void)
{
  int ret, badness, count;
  DIR *dp;
  struct dirent *entry;

  /* Try to go to that directory */
  do {
    ret = chdir(lam_tmpdir);
    if (ret == -1) {
      if (errno == EINTR)
        continue;
      else if (errno == ENOENT) {
        return;
      } else {
        show_help(NULL, "unable-rmdir", "tkill", lam_tmpdir, NULL);
        return;
      }
    }
  } while (ret != 0);

  /* First, remove all files in the session directory */
  dp = opendir(lam_tmpdir);
  if (dp == NULL) {
    show_help(NULL, "unable-rmdir", "tkill", lam_tmpdir, NULL);
    return;
  }
  badness = 0;
  while ((entry = readdir(dp)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 ||
        strcmp(entry->d_name, "..") == 0)
      continue;

    for (count = 0, ret = -1 ; ret != 0 && count < 10 ; ++count) {
      ret = remove(entry->d_name);
      if (ret == -1) {
        if (errno == EINTR)
          continue;
        else
          badness++;
      }
    }
  }
  closedir(dp);

  /* Did we remove everything successfully? */
  if (badness != 0) {
    show_help(NULL, "unable-rmdir", "tkill", lam_tmpdir, NULL);
    return;
  }

  /* Move back one directory so that we can remove the target dir */
  do {
    ret = chdir("..");
    if (ret == -1) {
      if (errno == EINTR)
        continue;
      else {
        show_help(NULL, "unable-rmdir", "tkill", lam_tmpdir, NULL);
        return;
      }
    }
  } while (ret != 0);

  /* Now go remove the directory itself. */
  do {
    ret = rmdir(lam_tmpdir);
    if (ret == -1) {
      if (errno == EINTR)
        continue;
      else {
        show_help(NULL, "unable-rmdir", "tkill", lam_tmpdir, NULL);
        return;
      }
    }
  } while (ret != 0);
}


/*
 * make the directory if it doesn't exist, ensure the perms are secure
 */
static int
check_dir_perms(void)
{
  int ret, statret, mode;
  int badfile = 0;
  int baddir = 0;
  struct stat s;

  do {
    statret = stat(lam_tmpdir, &s);
    if (statret == -1) {
      if (errno == EINTR)
        continue;
      else if (errno == ENOENT)
        break;
      else if (errno == EACCES)
        return (LAMERROR);
    }
  } while (statret != 0);

  /* If there is some file by this name, ensure that it is a
     directory, has the right permissions, and has the right owner */

  if (statret == 0) {

    /* Check that it's a directory */

    if ((s.st_mode & S_IFDIR) != S_IFDIR)
      badfile = 1;
    else {

    /* Check permissions */

      mode = (S_IRWXU | S_IRWXG | S_IRWXO);
      if ((s.st_mode & mode) != S_IRWXU)
        baddir = 1;

      /* Check owner */

      if (s.st_uid != getuid())
        baddir = 1;
    }

    /* If we need to remove something, do it */

    if (badfile == 1 || baddir == 1) {
      do {
        if (badfile == 1)
          ret = unlink(lam_tmpdir);
        else
          ret = rmdir(lam_tmpdir);

        if (ret == -1) {
          if (errno == EINTR)
            continue;
          else if (errno == ENOENT)
            /* shouldn't happen, but... */
            break;
          else
            /* If we couldn't remove it, badness */
            return (LAMERROR);
        }
      } while (ret != 0);
    } else
      /* Otherwise, we have goodness */
      return 0;
  }

  /* Make the directory */
  mode = umask(077);
  ret = mkdir(lam_tmpdir, 0700);
  if (ret != 0)
    perror("mkdir");
  umask(mode);

  if (ret == 0)
    return 0;
  else
    return (LAMERROR);
}


/*
 *  lam_get_batchid
 *
 *  Function: - get batch job ID
 *      - only checks for PBS, LSF, SGE, and SLURM at the moment
 *      - this prevents LAM from using the same socket
 *        name on multiple jobs in a batch environment,
 *        where the scheduler may put multiple LAM/MPI
 *        jobs on the same host
 *
 *  Returns:  - job ID in a string (which must be free()ed) or
 *                        NULL
 */
static char*
lam_get_batchid(void)
{
  char *tmp;
  char *tmp2;
  char *retstr = NULL;
  size_t retstrlen = 0;

  /* Are we running under a PBS job? */
  if (getenv("PBS_ENVIRONMENT") != NULL && 
      (tmp = getenv("PBS_JOBID")) != NULL) {
    retstrlen = strlen(tmp) + strlen("pbs-") + 2;
    retstr = malloc(retstrlen);
    snprintf(retstr, retstrlen, "pbs-%s", tmp);
  } 

  /* Are we running under a SLURM job? */
  else if ((tmp = getenv("SLURM_JOBID")) != NULL) {
    retstrlen = strlen(tmp) + strlen("slurm-") + 2;
    retstr = malloc(retstrlen);
    snprintf(retstr, retstrlen, "slurm-%s", tmp);
  }

  /* Are we running under an LSF job? */
  else if ((tmp = getenv("LSB_JOBID")) != NULL && 
           (tmp2 = getenv("LSB_JOBINDEX")) != NULL) {
    retstrlen = strlen(tmp) + strlen(tmp2) + strlen("lsf--") + 2;
    retstr = malloc(retstrlen);
    snprintf(retstr, retstrlen, "lsf-%s-%s", tmp, tmp2);
  }

  /* Are we running under an SGE job? */
  else if (getenv("SGE_ROOT") != NULL &&
           (tmp = getenv("JOB_ID")) != NULL) {
    tmp2 = getenv("SGE_TASK_ID");
    if (tmp2 == NULL)
      tmp2 = "undefined";

    retstrlen = strlen("sge-") + strlen(tmp) + strlen(tmp2) + 3;
    retstr = malloc(retstrlen);
    snprintf(retstr, retstrlen, "sge-%s-%s", tmp, tmp2);
  }

  /* add others here... */

  return retstr;
}



/*
 *  lam_mktmpid_stdin
 *
 *  Function: - create temporary file name based on an id
 *      - returns error if insufficient space
 *  Accepts:  - id number to make name unique
 *      - temporary file name (out)
 *      - size of storage for temporary name
 *  Returns:  - 0 or LAMERROR
 */
int
lam_mktmpid_stdin(int id, char *name, unint n)
{
  if (check_dir_perms() == LAMERROR) {
    /* try never to leave a directory around... */
    lam_tmpdir_remove();
    memset(name, 0, n);
    return LAMERROR;
  }

  snprintf(name, n, "%s/lam-%d-stdin", lam_tmpdir, id);
  
  return 0;
}


/*
 *  lam_mktmpid_stdout
 *
 *  Function: - create temporary file name based on an id
 *      - returns error if insufficient space
 *  Accepts:  - id number to make name unique
 *      - temporary file name (out)
 *      - size of storage for temporary name
 *  Returns:  - 0 or LAMERROR
 */
int
lam_mktmpid_stdout(int id, char *name, unint n)
{
  if (check_dir_perms() == LAMERROR) {
    /* try never to leave a directory around... */
    lam_tmpdir_remove();
    memset(name, 0, n);
    return LAMERROR;
  }

  snprintf(name, n, "%s/lam-%d-stdout", lam_tmpdir, id);
  
  return 0;
}



/*
 *  lam_mktmpid_stderr
 *
 *  Function: - create temporary file name based on an id
 *      - returns error if insufficient space
 *  Accepts:  - id number to make name unique
 *      - temporary file name (out)
 *      - size of storage for temporary name
 *  Returns:  - 0 or LAMERROR
 */
int
lam_mktmpid_stderr(int id, char *name, unint n)
{
  if (check_dir_perms() == LAMERROR) {
    /* try never to leave a directory around... */
    lam_tmpdir_remove();
    memset(name, 0, n);
    return LAMERROR;
  }

  snprintf(name, n, "%s/lam-%d-stderr", lam_tmpdir, id);
  
  return 0;
}
