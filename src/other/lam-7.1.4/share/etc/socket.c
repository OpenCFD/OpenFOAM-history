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
 * $Id: socket.c,v 1.15 2003/11/12 23:22:17 jsquyres Exp $
 *
 *	Function:	- socket utilities
 */

#include <lam_config.h>

#include <errno.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#if defined(HAVE_STRINGS_H) && HAVE_STRINGS_H
#include <strings.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/un.h>
#include <unistd.h>

#if defined(HAVE_SYS_SELECT_H) && HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#include <portable.h>
#include <typical.h>
#include <sfh.h>
#include <laminternal.h>


/*
 *	sfh_sock_open_srv_inet_stm
 *
 *	Function:	- create a server-side socket
 *			- Internet Domain, Stream connection
 *			- if port points to a non-zero positive port
 *			  number, that port number is used otherwise a
 *			  port number is dynamically allocated
 *			- if port is non-NULL, the server port number
 *			  is returned in it
 *	Accepts:	- ptr to port number (or NULL)
 *	Returns:	- socket descriptor or -1
 */
int
sfh_sock_open_srv_inet_stm(int *port, int backlog)
{
	int		sockd;		/* socket descriptor */
	LAM_SOCK_OPTLEN_T length;	/* length of socket name */
	struct sockaddr_in
			srvaddr;	/* server address info */
/*
 * Create server socket.
 */
	if ((sockd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		return(-1);
	}
/*
 * Bind the socket.  If a port # is given use it otherwise bind allocates one.
 */
	memset((char *) &srvaddr, 0, sizeof(srvaddr));
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	srvaddr.sin_port = (port && (*port > 0)) ?
			htons((unsigned short) *port) : htons(0);

	if (bind(sockd, (struct sockaddr *) &srvaddr, sizeof(srvaddr))) {
		close(sockd);
		return(-1);
	}
/*
 * If bind allocated a port # dynamically and port is non-NULL,
 * get the port # allocated and return it in port.
 */
	if (port && (*port <= 0)) {
		length = sizeof(srvaddr);
		if (getsockname(sockd, (struct sockaddr *) &srvaddr, 
				&length)) {
			close(sockd);
			return(-1);
		}

		*port = (int) ntohs(srvaddr.sin_port);
	}
/*
 * Create backlog queue.
 */
	if (backlog == -1)
	  backlog = 5;

	if (listen(sockd, backlog)) {
		close(sockd);
		return(-1);
	}

	return(sockd);
}


/*
 *	sfh_sock_open_clt_inet_stm
 *
 *	Function:	- create a client-side socket
 *			- Internet Domain, Stream connection
 *	Accepts:	- server host address (4 bytes, in network byte order)
 *			- server port number
 *	Returns:	- socket descriptor or -1
 */
int
sfh_sock_open_clt_inet_stm(unsigned char *hostaddr, int portnum)
{
	int		sockd;		/* socket descriptor */
	int		ret;		/* connect() return value */
	int		errno_save;
	struct sockaddr_in
			srvaddr;	/* server address info */
/*
 * Loop until successful at creating and connecting
 * a socket with no interruption.
 */
	for (;;) {
/*
 * Create client socket.
 */
		if ((sockd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			return(-1);
		}
/*
 * Setup the server Internet address structure.
 */
		memset((char *) &srvaddr, 0, sizeof(srvaddr));
		srvaddr.sin_family = AF_INET;
		srvaddr.sin_port = htons((unsigned short) portnum);
		memcpy((char *) &srvaddr.sin_addr, (char *) hostaddr, 4);
/*
 * Connect client socket to server socket.
 */
		ret = connect(sockd,
			(struct sockaddr *) &srvaddr, sizeof(srvaddr));

		if (ret == 0) {
			return(sockd);
		} else if (errno != EINTR) {
                  errno_save = errno;
                  close(sockd);
                  errno = errno_save;
                  return(-1);
		} else {
			close(sockd);
		}
	}
}


/*
 *	sfh_sock_open_srv_unix_stm
 *
 *	Function:	- create a server-side socket
 *			- Unix Domain, Stream connection
 *			- if addr is not a zero length string then
 *			  it is used as the socket address otherwise it must
 *			  have room for at least 17 characters and a unique
 *			  address is generated and returned in it
 *	Accepts:	- address string
 *	Returns:	- socket descriptor or -1
 */
int
sfh_sock_open_srv_unix_stm(char *addr)
{
	int		len;
	int		sd;
	struct sockaddr_un server_un;
	mode_t          mode;
        char *orig_dir;
        char *addr_dir;
        char *addr_file;
/*
 * If an address was not given create one.
 * String addr must have room for 17 chars (16 in tmp name + 1 for null char).
 */
	if (strlen(addr) == 0) {
		strcpy(addr, "/tmp/sfh-sXXXXXX");
		if (mkstemp(addr) == 0) {
			return(-1);
		}
	}
/*
 * Create server socket.
 */
	mode = umask(0177);
	if ((sd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		return(-1);
	}

        /* In some cases, we may be trying to open a *really* long
           socket name.  And that may be longer that the size allowed
           by sockaddr_un.  So instead of trying to open the socket
           directly, chdir to the basename of the address and open
           just the filename of the socket.  Then chdr back to the
           original directory.  If the filename of the socket is
           longer than sockaddr_un allows, well, then you're out of
           luck.  :-) */

        orig_dir = getworkdir();
        addr_dir = strdup(addr);
        addr_file = strrchr(addr_dir, '/');
        if (addr_file != NULL) {
          *addr_file = '\0';
          chdir(addr_dir);
          ++addr_file;
        } else {
          addr_file = addr_dir;
        }
        if (strlen(addr_file) > sizeof(server_un.sun_path)) {
          errno = EINVAL;
          return -1;
        }
/*
 * Bind the socket.
 */
	memset(&server_un, 0, sizeof(server_un));
	server_un.sun_family = AF_UNIX;
	strcpy(server_un.sun_path, addr_file);

#if LAM_HAVE_SA_LEN
	len = sizeof(server_un.sun_len) + sizeof(server_un.sun_family) +
		strlen(server_un.sun_path) + 1;
	server_un.sun_len = len;
#else
	len = strlen(server_un.sun_path) + sizeof(server_un.sun_family);
#endif

	if (bind(sd, (struct sockaddr *) &server_un, len) < 0) {
		close(sd);
		umask(mode);
		return(-1);
	}
/*
 * Create backlog queue.
 */
	if (listen(sd, 5) < 0) {
		close(sd);
		umask(mode);
		return(-1);
	}

        /* Go back to the original directory and free temp memory */

        chdir(orig_dir);
        free(orig_dir);
        free(addr_dir);

	umask(mode);
	return (sd);
}


/*
 *	sfh_sock_open_clt_unix_stm
 *
 *	Function:	- connects stream, UNIX, client socket
 *			- retries after certain error conditions
 *	Accepts:	- UNIX address (filename)
 *	Returns:	- socket or -1
 */
int
sfh_sock_open_clt_unix_stm(char *addr)
{
	int		len;
	int		sd;
	int		r;
	struct sockaddr_un server_un;
        char *orig_dir;
        char *addr_dir;
        char *addr_file;
/*
 * Set up address.
 */
	memset(&server_un, 0, sizeof(server_un));
	server_un.sun_family = AF_UNIX;
        /* In some cases, we may be trying to open a *really* long
           socket name.  And that may be longer that the size allowed
           by sockaddr_un.  So instead of trying to open the socket
           directly, chdir to the basename of the address and open
           just the filename of the socket.  Then chdr back to the
           original directory.  If the filename of the socket is
           longer than sockaddr_un allows, well, then you're out of
           luck.  :-) */

        orig_dir = getworkdir();
        addr_dir = strdup(addr);
        addr_file = strrchr(addr_dir, '/');
        if (addr_file != NULL) {
          *addr_file = '\0';
          chdir(addr_dir);
          ++addr_file;
        } else {
          addr_file = addr_dir;
        }
        if (strlen(addr_file) > sizeof(server_un.sun_path)) {
          errno = EINVAL;
          return -1;
        }
	strcpy(server_un.sun_path, addr_file);

#if LAM_HAVE_SA_LEN
	len = sizeof(server_un.sun_len) + sizeof(server_un.sun_family) + 
          strlen(server_un.sun_path) + 1;
	server_un.sun_len = len;
#else
	len = strlen(server_un.sun_path) + sizeof(server_un.sun_family);
#endif
/*
 * Create the socket.
 */
	do {
		if ((sd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
				return(-1);
/*
 * Connect to server.
 */
		r = connect(sd, (struct sockaddr *) &server_un, len);

		if (r < 0) {
		  /* 
		     Joe Griffin (mscsoftware.com) noticed that this
		     code sometimes blocks when you don't want it to.
		     For example, if no lamd is running, and you
		     invoke any LAM command that expects to open the
		     named unix socket to the lamd, it will hang
		     (looping infinitely).

		     This is because linux's connect will set errno ==
		     ECONNREFUSED if a named socket does not exist
		     that we try to connect to.  Solaris, for example,
		     sets errno == ENOENT, meaning that Solaris will
		     dump immediately in the above scenario (as you
		     would expect it to).

		     Without a timeout value, there does not seem to
		     be any reason to accept ECONNREFUSED or ETIMEDOUT
		     as a looping condition.  Indeed, since LAM works
		     under Solaris, there must be no places in the LAM
		     code that depends on waiting for a server unix
		     socket to establish itself (i.e., blocking in
		     this function until the server socket is ready).

		     Hence, we're going to change the semantics to
		     define ECONNREFUSED and ETIMEDOUT as an error.

		  */
#if 0
			if ((errno != ECONNREFUSED) && (errno != EINTR) &&
					(errno != ETIMEDOUT)) {
				close(sd);
				return(-1);
			}
#else
			if (errno != ETIMEDOUT) {
			  int errnosave = errno;
			  close(sd);
			  errno = errnosave;
			  return(-1);
			}
#endif

			if (close(sd)) return(-1);
		}
	} while (r < 0);

        /* Go back to the original directory and free temp memory */

        chdir(orig_dir);
        free(orig_dir);
        free(addr_dir);

	return(sd);
}


/*
 *	sfh_sock_accept_tmout
 *
 *	Function:	- accept a connection with a timeout
 *			- negative timeout values block indefinitely
 *	Accepts:	- socket
 *			- timeout value (seconds)
 *	Returns:	- connection socket or -1
 */
int
sfh_sock_accept_tmout(int sock, int timeout)
{
  return sfh_sock_accept_peer_tmout(sock, timeout, 0, 0);
}


/*
 *	sfh_sock_accept_peer_tmout
 *
 *	Function:	- accept a connection with a timeout
 *			- negative timeout values block indefinitely
 *	Accepts:	- socket
 *			- timeout value (seconds)
 *                      - sockaddr peer pointer (INOUT)
 *                      - sockaddr length pointer (in vs. un)
 *	Returns:	- connection socket or -1
 */
int
sfh_sock_accept_peer_tmout(int sock, int timeout, struct sockaddr *sa,
			   int *optlen)
{
	int		connsock;	/* connection socket */
	int		ret;		/* select return value */
	fd_set		readmask;	/* select read-mask */
	struct timeval	tmout;		/* timeout argument */

	if (timeout >= 0) {
/*
 * Set up the timeout argument and block on a select call.
 */
		tmout.tv_sec = timeout;
		tmout.tv_usec = 0;

		FD_ZERO(&readmask);
		FD_SET(sock, &readmask);

		while (((ret = select(sock + 1, &readmask, (fd_set *) 0,
			(fd_set *) 0, &tmout)) < 0) && (errno == EINTR));

		if (ret <= 0) {
			errno = (ret == 0) ? ETIMEDOUT : errno;
			return(-1);
		}
	}
/*
 * Do the accept call.
 */
	while (((connsock = accept(sock, sa, 
				   (LAM_SOCK_OPTLEN_T *) optlen)) < 0) && 
	       (errno == EINTR))
	  continue;

	return connsock;
}


/*
 *	sfh_sock_open_srv_inet_dgm
 *
 *	Function:	- create a server-side socket
 *			- Internet domain datagrams
 *			- if port points to a non-zero positive port
 *			  number, that port number is used otherwise a
 *			  port number is dynamically allocated
 *			- if port is non-NULL, the server port number
 *			  is returned in it
 *	Accepts:	- ptr to port number (or NULL)
 *	Returns:	- socket descriptor or -1
 */
int
sfh_sock_open_srv_inet_dgm(int *port)
{
	int		sockd;		/* socket descriptor */
	LAM_SOCK_OPTLEN_T length;		/* length of socket name */
	struct sockaddr_in
			srvaddr;	/* server address info */
/*
 * Create server socket.
 */
	if ((sockd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		return(-1);
	}
/*
 * Bind the socket.  If a port # is given use it otherwise bind allocates one.
 */
	sfh_sock_fill_inet_addr((unsigned char *) 0,
				(port) ? *port : 0, &srvaddr);

	if (bind(sockd, (struct sockaddr *) &srvaddr, sizeof(srvaddr))) {
		close(sockd);
		return(-1);
	}
/*
 * If bind allocated a port # dynamically and port is non-NULL,
 * get the port # allocated and return it in port.
 */
	if (port && (*port <= 0)) {
		length = sizeof(srvaddr);
		if (getsockname(sockd,
				(struct sockaddr *) &srvaddr, &length)) {
			close(sockd);
			return(-1);
		}

		*port = (int) ntohs(srvaddr.sin_port);
	}

	return(sockd);
}


/*
 *	sfh_sock_open_clt_inet_dgm
 *
 *	Function:	- create a client-side socket
 *			- Internet domain datagrams
 *	Returns:	- socket descriptor or -1
 */
int
sfh_sock_open_clt_inet_dgm(void)
{
	int		sockd;		/* socket descriptor */
	struct sockaddr_in
			myaddr;		/* local address info */
/*
 * Create client socket.
 */
	sockd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockd < 0) {
		return(-1);
	}
/*
 * Bind local address.
 */
	sfh_sock_fill_inet_addr((unsigned char *) 0, 0, &myaddr);

	if (bind(sockd, (struct sockaddr *) &myaddr, sizeof(myaddr))) {
		close(sockd);
		return(-1);
	}

	return(sockd);
}


/*
 *	sfh_sock_fill_inet_addr
 *
 *	Function:	- fill host/port Internet address structure
 *	Accepts:	- host address or NULL (4 bytes, network byte order)
 *			- port number
 *			- ptr Internet address structure
 */
void
sfh_sock_fill_inet_addr(unsigned char *hostaddr, int port, 
			struct sockaddr_in *addr)
{
	memset((char *) addr, 0, sizeof(struct sockaddr_in));
	addr->sin_family = AF_INET;
	addr->sin_port = (port > 0) ? htons((unsigned short) port) : htons(0);
	if (hostaddr) {
		memcpy((char *) &(addr->sin_addr), (char *) hostaddr, 4);
	} else {
		addr->sin_addr.s_addr = htonl(INADDR_ANY);
	}
}


/*
 *	sfh_sock_set_buf_size
 *
 *	Function:	- set socket buffer size
 *	Accepts:	- socket fd
 *			- domain (SFH_INET or SFH_UNIX)
 *			- buffer (SO_SNDBUF or SO_RCVBUF)
 *			- size to set (bytes)
 */
int
sfh_sock_set_buf_size(int sd, int domain, int buf, unsigned int size)
{
    LAM_SOCK_OPTLEN_T 	optlen;
    unsigned int defsize = 0;

    if (domain == SFH_UNIX) {
#if LAM_BROKEN_SET_UNIX_SO_BUFSIZES
	return(0);
#endif
    } else if (domain == SFH_INET) {
#if LAM_BROKEN_SET_INET_SO_BUFSIZES
	return(0);
#endif
    } else {
	errno = EINVAL;
	return(-1);
    }

    optlen = sizeof(defsize);
    if (getsockopt(sd, SOL_SOCKET, buf, (void *) &defsize, &optlen)) {
	return(-1);
    }

    if (defsize < size) {
	if (setsockopt(sd, SOL_SOCKET, buf, (void *) &size, sizeof(size))) {
	    return(-1);
	}
    }

    return(0);
}
