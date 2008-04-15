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
 * $Id: fqopenfd.c,v 6.11 2003/11/14 00:56:58 pkambadu Exp $
 *
 *  Function: - FQOPENFD request
 *      - opens passed file descriptors
 *      - exits daemon in case of catastrophic error
 */

#include <lam_config.h>
#include <sfh.h>

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <events.h>
#include "filed.h"
#include <freq.h>
#include <laminternal.h>
#include <net.h>
#include <portable.h>
#include <terror.h>
#include <typical.h>
#include <etc_misc.h>

/*
 * public functions
 */
void      fqopenfd();

/*
 * external functions
 */
extern struct fdesc *fdnew();   /* allocate a new descriptor */
extern void   fsendr();   /* general purpose file reply */
extern void   fdactivate();   /* make sure tfd is active */
extern void   fdcut();    /* remove tfd */

/*
 * private functions
 */
static struct fdesc * passed_fd_new();
#if ! LAM_HAVE_FD_PASSING
static void fstddesc (char *fstdin, char* fstdout, char *fstderr);
#endif

/*
 * external variables
 */
extern char   fbuf[];     /* multi purpose buffer */

/*
 * local defines;
 */
#define FNAMELEN  32

/*
 *  fqopenfd
 *
 *  Function: - open passed file descriptors
 *      - we rely on error/eof detection on the connection
 *        to indicate errors and a filed reply is sent only
 *        if all the protocol on the connection gets done
 *  Accepts:  - ptr to filed request
 */
void
fqopenfd(fq)

struct freq   *fq;

{
  struct fdesc  *f[3];      /* target descriptors */
  char  fname[FNAMELEN];  /* generated file name */
  int4  tfd[3];     /* LAM file descriptors */
  int   fd[3];      /* passed in stdio fds */
  int   pass_fd;    /* connection to fd passer */
  int   i;
#if ! LAM_HAVE_FD_PASSING
  char  fstdin[LAM_PATH_MAX];
  char  fstdout[LAM_PATH_MAX];
  char  fstderr[LAM_PATH_MAX];
#endif
  
/*
 * Connect to the server side (filed client) which will be passing the file
 * descriptors.
 */
  pass_fd = sfh_sock_open_clt_unix_stm(fbuf);
  if (pass_fd < 0) {
    fsendr(fq->fq_src_node,
      fq->fq_src_event, errno, 0, INT4_NIL, INT4_NIL);
    return;
  }
/*
 * Receive the file descriptors.
 */
/* Recieve the file descriptors from the server only is FD passing is 
 * supported, else establish connection with the three open sockets that
 * it has created and write everything into those sockets. The other end
 * will take care of reading from these sockets and then dumping it to the 
 * standard IO devices
 */
#if LAM_HAVE_FD_PASSING 
  if ((fd[0] = sfh_recv_fd(pass_fd)) < 0) {
    close(pass_fd);
    return;
  }
  if ((fd[1] = sfh_recv_fd(pass_fd)) < 0) {
    close(fd[0]);
    close(pass_fd);
    return;

  }
  if ((fd[2] = sfh_recv_fd(pass_fd)) < 0) {
    close(fd[0]);
    close(fd[1]);
    close(pass_fd);
    return;

  }
#else 
/* Do the socket trick and establish the connections */

  LAM_ZERO_ME (fstdin);
  LAM_ZERO_ME (fstdout);
  LAM_ZERO_ME (fstderr);
  
  fstddesc (fstdin, fstdout, fstderr);


  if ((fd[0] = sfh_sock_open_clt_unix_stm(fstdin)) < 0) {
    close(pass_fd);
    fsendr(fq->fq_src_node,
           fq->fq_src_event, errno, 0, INT4_NIL, INT4_NIL);
    return;
  }
 
  if ((fd[1] = sfh_sock_open_clt_unix_stm(fstdout)) < 0) {
    close(fd[0]);
    close(pass_fd);
    fsendr(fq->fq_src_node,
           fq->fq_src_event, errno, 0, INT4_NIL, INT4_NIL);
    return;
  }
  
  if ((fd[2] = sfh_sock_open_clt_unix_stm(fstderr)) < 0) {
    close(fd[0]);
    close(fd[1]);
    close(pass_fd);
    fsendr(fq->fq_src_node,
           fq->fq_src_event, errno, 0, INT4_NIL, INT4_NIL);
    return;
  }
 
#endif

/*
 * Open LAM file descriptors for the passed UNIX file descriptors.  If the
 * stdout and stderr file descriptors do not refer to a tty then for want of
 * better names they are named stdout.pid and stderr.pid, where pid is that of
 * the filed client passing the file descriptors.  If stdin is a tty it is
 * closed locally.
 */
  if (isatty(fd[0])) {
    close(fd[0]);
    f[0] = 0;
  } else {
    sprintf(fname, "stdin.%d", -fq->fq_src_event);
    f[0] = passed_fd_new(fd[0], fname, fq);
  }
  
  if (isatty(fd[1])) {
    lam_strncpy(fname, ttyname(fd[1]), FNAMELEN);
  } else {
    sprintf(fname, "stdout.%d", -fq->fq_src_event);
  }
  f[1] = passed_fd_new(fd[1], fname, fq);
  
  if (isatty(fd[2])) {
    lam_strncpy(fname, ttyname(fd[2]), FNAMELEN);
  } else {
    sprintf(fname, "stderr.%d", -fq->fq_src_event);
  }
  f[2] = passed_fd_new(fd[2], fname, fq);
/*
 * The LAM file descriptors are flagged as active and locked because the
 * real filename may be unknown and hence they cannot be closed and re-opened.
 */
  if ((f[0] && fdlock(f[0])) || fdlock(f[1]) || fdlock(f[2])) {
    for (i = 0; i < 3; i++) {
      if (f[i]) {
        close(fd[i]);
        fdcut(f[i]);
      }
    }
    
    close(pass_fd);
    fsendr(fq->fq_src_node,
           fq->fq_src_event, errno, 0, INT4_NIL, INT4_NIL);
    return;
  }
/*
 * Send LAM file descriptors to passer.
 */
  for (i = 0; i < 3; i++) {
    tfd[i] = (f[i]) ? f[i]->f_tfd : 0;
  }
      
  if (mwrite(pass_fd, (char *) tfd, sizeof(tfd)) != sizeof(tfd)) {
    close(pass_fd);
    fsendr(fq->fq_src_node,
           fq->fq_src_event, errno, 0, INT4_NIL, INT4_NIL);
    for (i = 0; i < 3; i++) {
      if (f[i]) {
        close(fd[i]);
        fdcut(f[i]);
      }
    }
  } else {
    close(pass_fd);
    fsendr(fq->fq_src_node,
           fq->fq_src_event, 0, 0, INT4_NIL, INT4_NIL);
  }
}


/*
 *  passed_fd_new
 *
 *  Function: - allocates a new LAM file descriptor
 *  Accepts:  - passed file descriptor
 *      - file name for passed file descriptor
 *      - ptr filed request
 *  Returns:  - ptr to LAM file descriptor
 */
static struct fdesc *
passed_fd_new(fd, fname, fq)

int     fd;
char      *fname;
struct freq   *fq;

{
  struct fdesc  *f;
  
  f = fdnew();
  
  f->f_fd = fd;
  f->f_count = 1;
  f->f_ptr = 0;
  f->f_flow = 0;
  f->f_tflags = (LAM_O_WRONLY | LAM_O_1WAY | LAM_O_ACTIVE);
  f->f_src_node = fq->fq_src_node;
  f->f_src_event = fq->fq_src_event;
  
  f->f_name = malloc((unsigned) (strlen(fname) + 1));
  if (f->f_name == 0) {
    lampanic("filed (malloc)");
  }
  strcpy(f->f_name, fname);

  return(f);
}

#if ! LAM_HAVE_FD_PASSING
/*  fstddesc
 *  
 *  Function: - Populates the buffers with the addresses of the sockets opened 
 *        for IO redirection
 *  Accepts:  - Buffers for populating
 *  Returns:  - Populated buffers
 */
static void fstddesc (char *fstdin, char* fstdout, char *fstderr){
  sprintf(fstdin,"%s-stdin", fbuf);
  sprintf(fstdout,"%s-stdout", fbuf);
  sprintf(fstderr,"%s-stderr", fbuf);
}
#endif
