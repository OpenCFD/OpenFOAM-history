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
 * $Id: lfopenfd.c,v 6.11 2003/11/14 00:56:58 pkambadu Exp $
 *
 *  Function: - pass stdio file descriptors to local filed
 *  Accepts:  - name to use for connection
 *  Returns:  - 0 or LAMERROR
 */

#include <lam_config.h>
#include <sfh.h>

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#if defined(HAVE_SYS_SELECT_H) && HAVE_SYS_SELECT_H
  #include <sys/select.h>
#endif
#include <sys/time.h>
#include <sys/types.h>
#include <events.h>
#include <freq.h>
#include <ksignal.h>
#include <net.h>
#include <portable.h>
#include <typical.h>
#include <etc_misc.h>
#include <laminternal.h>

/*
 * external variables
 */
extern struct fclient _ufd[FUMAX];    /* client file desc. table */


int
lam_lfopenfd(char *server)
{
  struct nmsg nhead;      /* network message desc. */
  struct freq *request;   /* filed request */
  struct freply *reply;     /* filed reply */
  int4    tfds[3];    /* LAM fds returned by filed */
  int   err = 0;    /* error code */
  int   mask;     /* signal mask */
  int   serv_fd;    /* server file desc. */
  int   pass_fd;    /* file desc. for passing fds */
  int   i;
/* These are the variables required for socket tricks */
#if ! LAM_HAVE_FD_PASSING
  pid_t   serv_fd_stdout_pid;
  int   serv_stdin_fd;
  int   serv_stdout_fd;
  int   serv_stderr_fd;
  int   fd_stdin;
  int   fd_stderr;
  int   fd_stdout;
  char  fstdin[LAM_PATH_MAX];
  char  fstdout[LAM_PATH_MAX];
  char  fstderr[LAM_PATH_MAX];
  char  buffer[MAXNMSGLEN];
  int   maxfdpi;
  int   read_err;
  fd_set    rset;
#endif
 
/* 
 * Open file descriptor passing server side.
 */
  if ((serv_fd = sfh_sock_open_srv_unix_stm(server)) < 0) {
    unlink(server);
    return(LAMERROR);
  }
/*
 * Send request to filed.
 */
  LAM_ZERO_ME(nhead);
  request = (struct freq *) nhead.nh_data;
  reply = (struct freply *) nhead.nh_data;

  request->fq_src_node = getnodeid();
  request->fq_src_event = -lam_getpid();
  request->fq_req = FQOPENFD;

  nhead.nh_node = LOCAL;
  nhead.nh_event = EVFILED;
  nhead.nh_type = 0;
  nhead.nh_flags = 0;
  nhead.nh_msg = server;
  nhead.nh_length = strlen(server) + 1;

  if (nhead.nh_length > MAXNMSGLEN) {
    close(serv_fd);
    unlink(server);
    errno = ENAMETOOLONG;
    return(LAMERROR);
  }

#if LAM_HAVE_FD_PASSING 
/*
 * Don't block SIGUDIE since we may wish to kill a process hanging in
 * this protocol.  
 */
  mask = ksigblock(sigmask(SIGARREST));

  if (nsend(&nhead)) {
    close(serv_fd);
    unlink(server);
    ksigsetmask(mask);
    return(LAMERROR);
  }
/*
 * Accept connection from filed.
 */
  pass_fd = sfh_sock_accept_tmout(serv_fd, -1);

  close(serv_fd);
  unlink(server);

  if (pass_fd < 0) {
    return(LAMERROR);
  }
  
  if (sfh_send_fd(pass_fd, 0)
      || sfh_send_fd(pass_fd, 1) || sfh_send_fd(pass_fd, 2)) {
    close(pass_fd);
    return(LAMERROR);
  }
#else
/* Do the socket trick by establishing the right connections
 * open three server side sockets
 * Creating the socket names should be left to lam_mktmpid() function
 */

  LAM_ZERO_ME (fstdin);
  LAM_ZERO_ME (fstdout);
  LAM_ZERO_ME (fstderr);
 
  if (lam_mktmpid_stdin((int) lam_getpid(), fstdin, sizeof(fstdin))) {
    close (serv_fd);
    unlink (server);
    return(LAMERROR);
  }

  if (lam_mktmpid_stdout((int) lam_getpid(), fstdout, sizeof(fstdout))) {
    close (serv_fd);
    unlink (server);
    return(LAMERROR);
  }

  if (lam_mktmpid_stderr((int) lam_getpid(), fstderr, sizeof(fstderr))) {
    close (serv_fd);
    unlink (server);
    return(LAMERROR);
  }

  /* open the sockets and listen on them */

  if ((serv_stdin_fd = sfh_sock_open_srv_unix_stm(fstdin)) < 0) {
    close (serv_fd);
    unlink (server);
    unlink(fstdin);
    return(LAMERROR);
  }


  if ((serv_stdout_fd = sfh_sock_open_srv_unix_stm(fstdout)) < 0) {
    close (serv_fd);
    unlink (server);
    close (serv_stdin_fd);
    unlink(fstdin);
    unlink(fstdout);
    return(LAMERROR);
  }

  if ((serv_stderr_fd = sfh_sock_open_srv_unix_stm(fstderr)) < 0) {
    close (serv_fd);
    unlink (server);
    close (serv_stdin_fd);
    unlink(fstdin);
    close (serv_stdout_fd);
    unlink(fstdout);
    unlink(fstderr);
    return(LAMERROR);
  }
/*
 * Don't block SIGUDIE since we may wish to kill a process hanging in
 * this protocol.  
 */
  mask = ksigblock(sigmask(SIGARREST));

  if (nsend(&nhead)) {
    close(serv_fd);
    unlink(server);
    close (serv_stdin_fd);
    unlink(fstdin);
    close (serv_stdout_fd);
    unlink(fstdout);
    close (serv_stderr_fd);
    unlink(fstderr);
    ksigsetmask(mask);
    return(LAMERROR);
  }

/*
 * Accept connections from filed.
 */
  pass_fd = sfh_sock_accept_tmout(serv_fd, -1);

  close(serv_fd);
  unlink(server);

  if (pass_fd < 0) {
    close (serv_stdin_fd);
    unlink (fstdin);
    close (serv_stdout_fd);
    unlink (fstdout);
    close (serv_stderr_fd);
    unlink (fstderr);
    return(LAMERROR);
  }
/* send stdin */
  fd_stdin = sfh_sock_accept_tmout(serv_stdin_fd, -1);

  close (serv_stdin_fd);
  unlink (fstdin);
  
  if (fd_stdin < 0) {
    close (pass_fd);
    close (serv_stdout_fd);
    unlink (fstdout);
    close (serv_stderr_fd);
    unlink (fstderr);
    return(LAMERROR);
  }
  
/* send stdout */
  fd_stdout = sfh_sock_accept_tmout(serv_stdout_fd, -1);
  
  close (serv_stdout_fd);
  unlink (fstdout);
  
  if (fd_stdout < 0) {
    close (pass_fd);
    close (fd_stdin);
    close (serv_stderr_fd);
    unlink (fstderr);
    return(LAMERROR);
  }
  
/* send stderr */
  fd_stderr = sfh_sock_accept_tmout(serv_stderr_fd, -1);
  
  close (serv_stderr_fd);
  unlink (fstderr);
  
  if (fd_stderr < 0) {
    close (pass_fd);
    close (fd_stdin);
    close (fd_stdout);
    return(LAMERROR);
  }

/* Create the children who will listen on the sockets to print it out to the
 * tty. They will exit once the output is complete
 */

  if ((serv_fd_stdout_pid = fork()) < 0) {
    close (pass_fd);
    close (fd_stdin);
    close (fd_stdout);
    close (fd_stderr);
    return(LAMERROR);
  }
            
  else if (serv_fd_stdout_pid == 0) { /* Create child to listen for stdout/err */
    close (pass_fd);
    close (fd_stdin);

  /* select and poll on both stdin and stdout */
    FD_ZERO (&rset);

    for ( ; ; ) {
      LAM_ZERO_ME (buffer);
      FD_SET (fd_stdout, &rset);
      FD_SET (fd_stderr, &rset);
      maxfdpi = (fd_stderr > fd_stdout)? (fd_stderr + 1) : (fd_stdout + 1);

      select (maxfdpi, &rset, NULL, NULL, NULL);

      if ( FD_ISSET (fd_stdout, &rset) ) { /* read on stdout */
        read_err = read(fd_stdout, buffer, MAXNMSGLEN);
        if ( read_err == 0) { /* EOF reached */
          close (fd_stdout);
          close (fd_stderr);
          exit (0);
        }
        if ( read_err < 0 ) { /* error occured on reading */
          close (fd_stdout);
          close (fd_stderr);
          return (LAMERROR);
        }
        else { /* output to the terminal */
          fprintf (stdout,"%s", buffer);
          fflush (stdout);
        }
      }
        
      if ( FD_ISSET (fd_stderr, &rset) ) { /* read on stderr */
        read_err = read(fd_stderr, buffer, MAXNMSGLEN);
        if ( read_err == 0) { /* EOF reached */
          close (fd_stdout);
          close (fd_stderr);
          exit (0);
        }
        if ( read_err < 0 ) { /* error occured on reading */
          close (fd_stdout);
          close (fd_stderr);
          return (LAMERROR);
        }
        else { /* output to the terminal */
          fprintf (stderr,"%s", buffer);
          fflush (stdout);
        }
      }
    } /* end for */
  } /* child ends here */

/* Now we are back in the parent process
 * should we or should we NOT close the sockets here?
 * for now let me close them in the parent
 */
  close (fd_stdin); 
  close (fd_stdout);
  close (fd_stderr);

#endif
  
/*
 * Receive LAM file descriptors from filed.
 */
  if (mread(pass_fd, (char *) tfds, sizeof(tfds)) != sizeof(tfds)) {
    err = errno;
  }
  close(pass_fd);
/*
 * Receive response from filed.
 */
  nhead.nh_event = -lam_getpid();
  nhead.nh_length = 0;
  nhead.nh_msg = 0;

  if (nrecv(&nhead)) {
    ksigsetmask(mask);
    return(LAMERROR);
  }

  ksigsetmask(mask);

  if (err || reply->fr_errno) {
    errno = (reply->fr_errno) ? reply->fr_errno : err;
    return(LAMERROR);
  }
/*
 * Successfully received LAM file descriptors.  Fill in client fd table with
 * returned information.  If stdin is a tty leave it as the default.
 */
  if (!isatty(0)) {
    _ufd[0].fu_tflags = LAM_O_RDONLY | LAM_O_1WAY;
    _ufd[0].fu_node = nhead.nh_node;
    _ufd[0].fu_tfd = tfds[0];
  }

  for (i = 1; i < 3; i++) {
    _ufd[i].fu_tflags = LAM_O_WRONLY | LAM_O_1WAY;
    _ufd[i].fu_node = nhead.nh_node;
    _ufd[i].fu_tfd = tfds[i];
  }

  return(0);
}


