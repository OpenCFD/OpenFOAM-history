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
 * $Id: srfd.c,v 1.14 2003/02/03 16:54:41 brbarret Exp $
 *
 *	Function:	- file descriptor passing
 */


#ifndef LAM_RUNNING_CONFIGURE_TEST
/* special build case - we use this file in configure tests and don't
   want these header files included in that case */
#include <lam_config.h>
#include <sfh.h>
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <stdlib.h>
#include <errno.h>
#include <stddef.h>
#include <unistd.h>


#if LAM_HAVE_RFC2292_FD_PASSING || LAM_HAVE_POSIX_1G_FD_PASSING

#include <sys/un.h>

/* It appears that FreeBSD 4.2 has a broken CMSG_DATA, which requires
 *  sys/param.h to be included in order for it to work correctly.
 *  Otherwise, there would be link-time error of ALIGN missing.
 */
#include <sys/param.h>

#elif LAM_HAVE_SVR4_FD_PASSING

#include <stropts.h>

#endif

#if LAM_HAVE_POSIX_1G_FD_PASSING
/* size of control buffer to send/recv one file descriptor */
#define CONTROLLEN (sizeof(struct cmsghdr) + sizeof(int))
#endif



/*
 *	sfh_send_fd
 *
 *	Function:	- pass a single file descriptor over a stream
 *			- based on code from Stevens, "Advanced
 *			  Programming in the Unix Environment"
 *	Accepts:	- stream file descriptor
 *			- file descriptor to pass
 *	Returns:	- 0 (success) or -1 (error)
 */
#if LAM_HAVE_BSD43_FD_PASSING

int
sfh_send_fd(int stream, int fd)
{
	struct iovec	iov[1];
	struct msghdr	msg;
	char		buf[1];
	int		r;

/*
 * Pass one char and access rights.
 */
	buf[0] = 0;
	iov[0].iov_base = buf;
	iov[0].iov_len = 1;
	LAM_ZERO_ME(msg);
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_name = (caddr_t) 0;
	msg.msg_namelen = 0;
	msg.msg_accrights = (caddr_t) &fd;
	msg.msg_accrightslen = sizeof(int);

	do {
	  r = sendmsg(stream, &msg, 0);
	  if (r == 1)
	    break;
	  else if (r < 0 && errno == EINTR)
	    continue;
	  else {
	    return(-1);
	  }
	} while (1);

	/* Receive a handshake back indicating that the fd has been
           received properly.  This slows things down a bit, but it
           seems that some OS's allow buffered sending down streams
           and when the sender closes, it ditches the buffered sends
           (SCO).  So this extra sync here will prevent this.  */

	do {
	  r = read(stream, buf, 1);
	  if (r == 1)
	    break;
	  else if (r < 0 && errno == EINTR)
	    continue;
	  else {
	    return(-1);
	  }
	} while (1);

	return(0);
}

#elif LAM_HAVE_RFC2292_FD_PASSING

int
sfh_send_fd(int stream, int fd)
{
  struct iovec iov[1];
  struct msghdr msg;
  struct cmsghdr *cmsg;
  char fdbuf[CMSG_SPACE(sizeof(fd))];
  char commbuf[1];
  int *fdptr;
  int ret;

  commbuf[0] = 0;
  iov[0].iov_base = commbuf;
  iov[0].iov_len = 1;
  LAM_ZERO_ME(msg);
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;
  msg.msg_name = (caddr_t) 0;
  msg.msg_namelen = 0;

  /* set up the message to send */
  msg.msg_control = fdbuf;
  msg.msg_controllen = sizeof(fdbuf);

  /* Fill in the CMSG part */
  cmsg = CMSG_FIRSTHDR(&msg);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN(sizeof(int));

  /* Initialize the payload: */
  fdptr = (int *) CMSG_DATA(cmsg);
  memcpy(fdptr, &fd, sizeof(int));

  /* Sum of the length of all control messages in the buffer: */
  msg.msg_controllen = cmsg->cmsg_len;

  do {
    ret = sendmsg(stream, &msg, 0);
    if (ret == 1)
      break;
    else if (ret < 0 && errno == EINTR)
      continue;
    else
      return(-1);
  } while (1);

  /* Receive a handshake back indicating that the fd has been
     received properly.  This slows things down a bit, but it
     seems that some OS's allow buffered sending down streams
     and when the sender closes, it ditches the buffered sends
     (SCO).  So this extra sync here will prevent this.  */

  do {
    ret = read(stream, commbuf, 1);
    if (ret == 1)
      break;
    else if (ret < 0 && errno == EINTR)
      continue;
    else {
      return(-1);
    }
  } while (1);
  
  return(0);
}

#elif LAM_HAVE_POSIX_1G_FD_PASSING

int
sfh_send_fd(int stream, int fd)
{
  struct iovec iov[1];
  struct msghdr msg;
  struct cmsghdr *cmsg;
  char buf[1];
  int ret;

  buf[0] = 0;
  iov[0].iov_base = buf;
  iov[0].iov_len = 1;
  LAM_ZERO_ME(msg);
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;
  msg.msg_name = NULL;
  msg.msg_namelen = 0;


  /* malloc out enough space for the cmsg header and the fd.  Malloc
     instead of stick it on the stack for alignment reasons.*/
  cmsg = (struct cmsghdr*) malloc(CONTROLLEN);
  if (cmsg == NULL) {
    return -1;
  }

  /* Fill in the CMSG part */
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CONTROLLEN;

  /* Initialize the payload: */
  *(int *)CMSG_DATA(cmsg) = fd;

  /* fill in remaining fields of the msghdr */
  msg.msg_control = (caddr_t) cmsg;
  msg.msg_controllen = cmsg->cmsg_len;

  do {
    ret = sendmsg(stream, &msg, 0);
    if (ret == 1) {
      break;
    } else if (ret < 0 && errno == EINTR) {
      continue;
    } else {
      free(cmsg);
      return(-1);
    }
  } while (1);

  /* Receive a handshake back indicating that the fd has been
     received properly.  This slows things down a bit, but it
     seems that some OS's allow buffered sending down streams
     and when the sender closes, it ditches the buffered sends
     (SCO).  So this extra sync here will prevent this.  */

  do {
    ret = read(stream, buf, 1);
    if (ret == 1) {
      break;
    } else if (ret < 0 && errno == EINTR) {
      continue;
    } else {
      free(cmsg);
      return(-1);
    }
  } while (1);

  free(cmsg);
  
  return(0);
}


#elif LAM_HAVE_SVR4_FD_PASSING

int
sfh_send_fd(int stream, int fd)
{
	char		buf[1];
        int r;
/*
 * Send one char and the file descriptor.
 */
	if (write(stream, buf, 1) != 1) {
		return(-1);
	}

	if (ioctl(stream, I_SENDFD, fd) < 0) {
		return(-1);
	}

	/* Receive a handshake back indicating that the fd has been
           received properly.  This slows things down a bit, but it
           seems that some OS's allow buffered sending down streams
           and when the sender closes, it ditches the buffered sends
           (SCO).  So this extra sync here will prevent this.  */

	do {
	  r = read(stream, buf, 1);
	  if (r == 1)
	    break;
	  else if (r < 0 && errno == EINTR)
	    continue;
	  else {
	    return(-1);
	  }
	} while (1);

	return(0);
}

#else

/*
 * Not supported.
 */
int
sfh_send_fd(int stream, int fd)
{
#if defined(ENOTSUP)
	errno = ENOTSUP;
#else
	errno = ENOSYS;
#endif
	return(-1);
}

#endif


/*
 *	sfh_recv_fd
 *
 *	Function:	- receive a single file descriptor from a stream
 *			- based on code from Stevens, "Advanced
 *			  Programming in the Unix Environment"
 *	Accepts:	- stream file descriptor
 *	Returns:	- file descriptor (success) or -1 (error)
 */
#if LAM_HAVE_BSD43_FD_PASSING

int
sfh_recv_fd(int stream)
{
	int		newfd = -1;
	struct msghdr	msg;
	struct iovec	iov[1];
	char		buf[1];
	int		r;
/*
 * Receive one char and access rights.
 */
	iov[0].iov_base = buf;
	iov[0].iov_len = 1;
	LAM_ZERO_ME(msg);
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_name = (caddr_t) 0;
	msg.msg_namelen = 0;
	msg.msg_accrights = (caddr_t) &newfd;
	msg.msg_accrightslen = sizeof(int);

	do {
		r = recvmsg(stream, &msg, 0);
		if (r == 1)
			break;
		else if (r < 0 && errno == EINTR)
			continue;
		else
			return(-1);
	} while (1);

	if (msg.msg_accrightslen != sizeof(int))
		return(-1);

	/* Send a handshake back indicating that the fd has been
           received properly.  This slows things down a bit, but it
           seems that some OS's allow buffered sending down streams
           and when the sender closes, it ditches the buffered sends
           (SCO).  So this extra sync here will prevent this.  */

	do {
	  r = write(stream, buf, 1);
	  if (r == 1)
	    break;
	  else if (r < 0 && errno == EINTR)
	    continue;
	  else {
	    return(-1);
	  }
	} while (1);

	return(newfd);
}

#elif LAM_HAVE_RFC2292_FD_PASSING

int
sfh_recv_fd(int stream)
{
  struct iovec iov[1];
  struct msghdr msg;
  struct cmsghdr *cmsg;
  char fdbuf[CMSG_SPACE(sizeof(int))];
  char commbuf[1];
  int ret;

  commbuf[0] = 0;
  iov[0].iov_base = commbuf;
  iov[0].iov_len = 1;
  LAM_ZERO_ME(msg);
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;
  msg.msg_name = (caddr_t) 0;
  msg.msg_namelen = 0;

  /* Set msg_control infor for fdbuf (which includes enough space for
     the cmsg struct and the payload */
  msg.msg_control = fdbuf;
  msg.msg_controllen = sizeof(fdbuf);

  do {
    ret = recvmsg(stream, &msg, 0);
    if (ret == 1)
      break;
    else if (ret < 0 && errno == EINTR)
      continue;
    else
      return(-1);
  } while (1);

  /* Send a handshake back indicating that the fd has been
     received properly.  This slows things down a bit, but it
     seems that some OS's allow buffered sending down streams
     and when the sender closes, it ditches the buffered sends
     (SCO).  So this extra sync here will prevent this.  */

  do {
    ret = write(stream, commbuf, 1);
    if (ret == 1)
      break;
    else if (ret < 0 && errno == EINTR)
      continue;
    else {
      return(-1);
    }
  } while (1);

  /* Get the cmsg struct out of the msg (from fdbug, above) */
  cmsg = CMSG_FIRSTHDR(&msg);

  /* Return the fd from the cmsg */
  return(*(int *)CMSG_DATA(cmsg));
}

#elif LAM_HAVE_POSIX_1G_FD_PASSING

int
sfh_recv_fd(int stream)
{
  struct iovec iov[1];
  struct msghdr msg;
  struct cmsghdr *cmsg;
  char buf[1];
  int ret;
  int newfd;

  buf[0] = 0;
  iov[0].iov_base = buf;
  iov[0].iov_len = 1;
  LAM_ZERO_ME(msg);
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;
  msg.msg_name = NULL;
  msg.msg_namelen = 0;

  /* malloc enough space for the cmsg struct and the payload.  We
     malloc rather than put on stack for alignment reasons */
  cmsg = (struct cmsghdr*) malloc(CONTROLLEN);
  if (cmsg == NULL)
    return -1;

  /* Set msg_control information to hold the cmsg header and
     payload */
  msg.msg_control = (caddr_t) cmsg;
  msg.msg_controllen = CONTROLLEN;

  do {
    ret = recvmsg(stream, &msg, 0);
    if (ret == 1) {
      break;
    } else if (ret < 0 && errno == EINTR) {
      continue;
    } else {
      free(cmsg);
      return(-1);
    }
  } while (1);

  /* Send a handshake back indicating that the fd has been
     received properly.  This slows things down a bit, but it
     seems that some OS's allow buffered sending down streams
     and when the sender closes, it ditches the buffered sends
     (SCO).  So this extra sync here will prevent this.  */
  do {
    ret = write(stream, buf, 1);
    if (ret == 1) {
      break;
    } else if (ret < 0 && errno == EINTR) {
      continue;
    } else {
      free(cmsg);
      return(-1);
    }
  } while (1);

  /* Return the fd from the cmsg */
  newfd = *(int *)CMSG_DATA(cmsg); 
  free(cmsg);

  return(newfd);
}

#elif LAM_HAVE_SVR4_FD_PASSING

int
sfh_recv_fd(int stream)
{
	int		r;
	int		flag;
	char		buf[1];
	struct strbuf	dat;
	struct strrecvfd recvfd;
/*
 * Receive one char and the file descriptor.
 */
	dat.buf = buf;
	dat.maxlen = 1;
	flag = 0;

	do {
		r = getmsg(stream, (void *) 0, &dat, &flag);
		if (r >= 0)
			break;
		if (errno != EINTR)
			return(-1);
	} while (1);

	if (ioctl(stream, I_RECVFD, &recvfd) < 0) {
		return(-1);
	}

	/* Send a handshake back indicating that the fd has been
           received properly.  This slows things down a bit, but it
           seems that some OS's allow buffered sending down streams
           and when the sender closes, it ditches the buffered sends
           (SCO).  So this extra sync here will prevent this.  */

	do {
	  r = write(stream, buf, 1);
	  if (r == 1)
	    break;
	  else if (r < 0 && errno == EINTR)
	    continue;
	  else {
	    return(-1);
	  }
	} while (1);

	return(recvfd.fd);
}

#else

/*
 * Not supported.
 */
int
sfh_recv_fd(int stream)
{
#if defined(ENOTSUP)
	errno = ENOTSUP;
#else
	errno = ENOSYS;
#endif
	return(-1);
}

#endif
