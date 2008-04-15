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
 * $Id: pty.c,v 6.13 2003/10/11 19:15:35 brbarret Exp $
 * 
 *	Function:	- pseudo terminal support
 *			- derived from Steven's "Advanced Programming
 *			  in the Unix Environment".
 */

#include <lam_config.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#if defined(HAVE_PTY_H) && HAVE_PTY_H
#include <pty.h>
#endif
#if defined(HAVE_UTIL_H) && HAVE_UTIL_H
#include <util.h>
#endif

#include <typical.h>
#include <terror.h>
#include <etc_misc.h>


/*
 * Some OS's don't seem to prototype these functions properly
 */
extern int grantpt __ARGS ((int __fd));
extern int unlockpt __ARGS ((int __fd));
extern char *ptsname __ARGS ((int __fd));


/*
 * local variables
 */
static int max_name_len = 64;


#if LAM_HAVE_OPENPTY

static int static_fds = -1;

static int
ptym_open(char *pts_name)
{
  char name[64];
  mode_t mode;
  int fdm;

  mode = umask(077);
  if (openpty(&fdm, &static_fds, name, NULL, NULL) < 0) {
    umask(mode);
    return (-1);
  }
  lam_strncpy(pts_name, name, max_name_len);

  umask(mode);
  return fdm;
}


static int
ptys_open(int fdm, char *pts_name)
{
  /* Everything is done in one call, openpty(), above.  We stashed the
     slave fd in statis_fds. */

  return static_fds;
}

#elif LAM_HAVE_SYSV_PTYS && defined(HAVE_STROPTS_H) && HAVE_STROPTS_H

#include <stropts.h>

static int
ptym_open(char *pts_name)
{
    char	*ptr;
    int		fdm;
    mode_t      mode;

    mode = umask(077);
    strcpy(pts_name, "/dev/ptmx");
    if ((fdm = open(pts_name, O_RDWR)) < 0) {
	umask(mode);
	return(-1);
    }
    if (grantpt(fdm) < 0) {
	close(fdm);
	umask(mode);
	return(-1);
    }
    if (unlockpt(fdm) < 0) {
	close(fdm);
	umask(mode);
	return(-1);
    }
    if ((ptr = ptsname(fdm)) == NULL) {
	close(fdm);
	umask(mode);
	return(-1);
    }
    umask(mode);

    strcpy(pts_name, ptr);
    return(fdm);
}

static int
ptys_open(int fdm, char *pts_name)
{
    int		fds;
    mode_t      mode;

    mode = umask(077);
    if ((fds = open(pts_name, O_RDWR)) < 0) {
	close(fdm);
	umask(mode);
	return(-1);
    }
    if (ioctl(fds, I_PUSH, "ptem") < 0) {
	close(fdm);
	close(fds);
	umask(mode);
	return(-1);
    }
    if (ioctl(fds, I_PUSH, "ldterm") < 0) {
	close(fdm);
	close(fds);
	umask(mode);
	return(-1);
    }
    umask(mode);

    return(fds);
}

#elif LAM_HAVE_BSD_PTYS && defined(HAVE_GRP_H) && HAVE_GRP_H

#include <grp.h>

static int
ptym_open(char *pts_name)
{
    int		fdm;
    char	*ptr1, *ptr2;
    mode_t              mode;

    mode = umask(077);
    strcpy(pts_name, "/dev/ptyXY");
    for (ptr1 = "pqrstuvwxyzPQRST"; *ptr1 != 0; ptr1++) {
	pts_name[8] = *ptr1;
	for (ptr2 = "0123456789abcdef"; *ptr2 != 0; ptr2++) {
	    pts_name[9] = *ptr2;

	    if ((fdm = open(pts_name, O_RDWR)) < 0) {
	      if (errno == ENOENT) {
		umask(mode);
		return(-1);
	      }
	      else
		continue;
	    }

	    pts_name[5] = 't';
	    umask(mode);
	    return(fdm);
	}
    }
    umask(mode);

    return(-1);
}

static int
ptys_open(int fdm, char *pts_name)
{
    struct group	*grptr;
    int			gid, fds;
    mode_t              mode;

    mode = umask(077);
    gid = (grptr = getgrnam("tty")) ? grptr->gr_gid : -1;
    chown(pts_name, getuid(), gid);
    chmod(pts_name, S_IRUSR | S_IWUSR);

    if ((fds = open(pts_name, O_RDWR)) < 0) {
	close(fdm);
	umask(mode);
	return(-1);
    }
    umask(mode);

    return(fds);
}

#else

static int
ptym_open(char *pts_name)
{
    errno = ENOPTYSUPPORT;
    return(-1);
}

static int
ptys_open(int fdm, char *pts_name)
{
    errno = ENOPTYSUPPORT;
    return(-1);
}

#endif

int
lam_pty_open(int *fdm, int *fds)
{
    char *pts_name;

    pts_name = malloc(max_name_len);
    if (pts_name == NULL)
      return (LAMERROR);

    if ((*fdm = ptym_open(pts_name)) < 0)
	return(LAMERROR);

    if ((*fds = ptys_open(*fdm, pts_name)) < 0) {
	close(*fdm);
	return(LAMERROR);
    }
    free(pts_name);

    return(0);
}
