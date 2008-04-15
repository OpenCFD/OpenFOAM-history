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
 *	$Id: kernelio.c,v 6.33 2003/12/09 13:56:46 jsquyres Exp $
 * 
 *	Function:	- receives kernel requests and send replies
 *			- moves messages between clients
 */

#include <lam_config.h>
#include <sfh.h>

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(HAVE_STRINGS_H) && HAVE_STRINGS_H
#include <strings.h>
#endif
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#if defined(HAVE_SYS_SELECT_H) && HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#include <dl_inet.h>
#include <lamdebug.h>
#include <laminternal.h>
#include <kreq.h>
#include <preq.h>
#include <terror.h>
#include <typical.h>
#include <sfh.h>

#define KIMAX		(4 + 2 * PMAX)
#define localaddr_t	struct sockaddr *

/*
 * global functions
 */
struct kreq *kio_req(void);                     /* request from client */
void kio_cleanup(void);                         /* free resources */
void kio_close(int fd_client);                  /* close connection */
void kio_init(void);                            /* initialize */
void kio_intr(int fd, void (*f)(), int bclear); /* register interrupt */
void kio_intr_clr(int fd);                      /* clear interrupt */
void kio_reply(struct kreply *rep, int4 fd_clnt); /* reply to client */
void kio_send(struct kmsg *sendkmsg, int4 minlen, int fd_client);
                                                /* send from internal proc */
void kio_transfer(struct kmsg *sendkmsg, int fd_send, int fd_recv, 
		  int4 minlen);                 /* move msg, ext to ext */
int kio_fd_ready(void);                         /* get fd_ready */
int kio_recv(struct kmsg *recvkmsg, int4 minlen, int fd_client); 
                                                /* recv to internal proc */
int kio_to(struct timeval *delay, void (*f)()); /* register timeout */

/*
 * external functions
 */
extern struct kproc	*kpfindport();		/* find client by port */
extern void		kkillall();		/* kill all clients */
extern void		knuke();		/* remove client */
extern void		(*_lam_signal())();	/* portable signal() */
extern int		kkillopen();		/* open kill file */
extern int		mread();
extern int		mreadv();
extern int		mwrite();
extern int		mwritev();

/*
 * external variables
 */
extern struct kproc	*prun;			/* current running client */
extern char **environ;

/*
 * local variables
 */
static struct kreq	request;		/* kernel request */
static struct timeval	to_actual;		/* timeout delay */
static void		(*tofunc)();		/* timeout function */
static fd_set		allfds;			/* current input lines */
static fd_set		exceptfds;		/* error lines */
static fd_set		readfds;		/* active read lines */
static fd_set		clientfds;		/* client input lines */
static int		sd_kernel;		/* kernel I/O socket */
static int		fd_ready;		/* current ready line */
static int		fd_max;			/* max fd value */
static int		nfd_ready;		/* # currently ready lines */
static int		to_count;		/* timeout estimater */

static char		fwdbuf[KPKTLEN];	/* forwarding buffer */

static struct {
	void		(*kn_func)();		/* interrupt function */
	int		kn_fd;			/* interrupt file desc. */
	struct kproc	*kn_client;		/* associated kernel client */
	int		kn_bclear;		/* clear after trigger? */
} kintrs[KIMAX];

/*
 * local functions
 */
static void		kio_shutdown();		/* cleanup and exit */

/*
 *	kio_init
 *
 *	Function:	- initializes communication structures for
 *			  sending and receiving kernel requests
 */
void
kio_init()
{
	char		*f_kill;		/* kill filename */
	char		*f_sock;		/* socket filename */
	int		i, len;
	mode_t          mode;
        struct sockaddr_un kernel_un;
        char *orig_dir;
        char *addr_dir;
        char *addr_file;
/*
 * Initialize the interrupt fd->func table.
 */
	for (i = 0; i < KIMAX; ++i) {
		kintrs[i].kn_fd = -1;
		kintrs[i].kn_func = 0;
		kintrs[i].kn_client = 0;
	}
/*
 * Disable timeouts.
 */
	tofunc = 0;
	to_count = 0;
	to_actual.tv_sec = 0;
	to_actual.tv_usec = 0;
/*
 * I want to return an error on a broken connection.
 */
	if (_lam_signal(SIGPIPE, SIG_IGN) == SIG_ERR)
	  lampanic("lamd kernel: problem with internal call _lam_signal() (1)");
/*
 * Set up local address.
 */
	mode = umask(0177);
	if (lam_tmpdir_init(NULL, NULL) != 0)
	  lampanic("lamd kernel: problem making tmp directory");

/*
 * Create the socket.
 */
	if ((sd_kernel = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
	  lampanic("lamd kernel: problem with socket()");

	fd_max = sd_kernel;

        /* In some cases, we may be trying to open a *really* long
           socket name.  And that may be longer that the size allowed
           by sockaddr_un.  So instead of trying to open the socket
           directly, chdir to the basename of the address and open
           just the filename of the socket.  Then chdr back to the
           original directory.  If the filename of the socket is
           longer than sockaddr_un allows, well, then you're out of
           luck.  :-) */

        orig_dir = getworkdir();

        /* Get the socket name.  Ensure to malloc a big enough
           kernel_un to hold the entire path (the socket name may be
           longer than the default socket_un struct allows) */

	f_sock = lam_get_sockname();
        addr_dir = f_sock;
        addr_file = strrchr(addr_dir, '/');
        if (addr_file != NULL) {
          *addr_file = '\0';
          if (chdir(addr_dir) != 0) {
            lampanic("chdir failed!");
          }
          ++addr_file;
        } else {
          addr_file = addr_dir;
        }
        if (strlen(addr_file) > sizeof(kernel_un.sun_path)) {
          lampanic("lamd kernel: kernelio unix socket filename too long");
        }

	memset(&kernel_un, 0, sizeof(kernel_un));
	kernel_un.sun_family = AF_UNIX;
	memcpy((char *) kernel_un.sun_path, addr_file, strlen(addr_file) + 1);

#if LAM_HAVE_SA_LEN
	len = sizeof(kernel_un.sun_len) + sizeof(kernel_un.sun_family) +
		strlen(kernel_un.sun_path) + 1;
	kernel_un.sun_len = len;
#else
	len = strlen(kernel_un.sun_path) + sizeof(kernel_un.sun_family);
#endif

/*
 * Bind the kernel's address to the socket.
 */
	if (bind(sd_kernel, (localaddr_t) &kernel_un, len) != 0) {
	  if (errno == EACCES) {
	    fprintf(stderr, "lamd kernel: permission denied trying to bind to the named socket:\n");
	    fprintf(stderr, "lamd kernel: \"%s\"\n", kernel_un.sun_path);
	    fprintf(stderr, "lamd kernel: check the unix permissions");
	    lampanic("lamd kernel");
	  } else {
            char name[4096];
            snprintf(name, sizeof(name) - 1,
                     "lamd kernel: problem with bind (%s/%s)",
                     addr_dir, kernel_un.sun_path);
            name[sizeof(name) - 1] = '\0';
            lampanic(name);
          }
	}
/*
 * Establish backlog queue.
 */
	if (listen(sd_kernel, 5)) 
	  lampanic("lamd kernel: problem with listen()");

        /* Go back to the original directory and free temp memory */

        chdir(orig_dir);
        free(orig_dir);
        free(addr_dir);

        /* set socket to be close-on-exec */
	if (fcntl(sd_kernel, F_SETFD, 1) != 0)
            lampanic("lamd kernel: setting close-on-exec");

/*
 * Open the kill record file.
 */
	if ((f_kill = lam_get_killfile()) == 0) 
	  lampanic("lamd kernel: problem with internal call killname()");

	if (kkillopen(f_kill)) {
	  fprintf(stderr, "lamd kernel: problem with internal call kkillopen(): %s: ", f_kill);
	  lampanic("");
	}
	free(f_kill);
/*
 * Catch SIGTERM and SIGINT and kill all attached processes.
 */
	if (_lam_signal(SIGTERM, kio_shutdown) == SIG_ERR)
	  lampanic("lamd kernel: problem with internal call _lam_signal() (2)");

	if (_lam_signal(SIGINT, kio_shutdown) == SIG_ERR)
	  lampanic("lamd kernel: problem with internal call _lam_signal() (3)");
	
	FD_ZERO(&allfds);
	FD_ZERO(&clientfds);
	nfd_ready = 0;
	umask(mode);
}

/*
 *	kio_req
 *
 *	Function:	- receives the next kernel request
 *	Returns:	- kernel request or 0 if interrupt/timeout
 */
struct kreq *
kio_req()
{
	int		i;
	LAM_SOCK_OPTLEN_T length;
	int		sd_new;		/* new client socket */
	struct timeval to;
	struct timeval	*pto;		/* timeout on this loop */
	struct sockaddr_un
			client_un;	/* client address */

	for (;;) {
	    fd_ready++;
/*
 * Select on all current clients, external interrupt sources,
 * kernel server socket and timeout.
 */
	    while (nfd_ready == 0) {
		fd_ready = 0;
		memcpy((char *) &readfds, (char *) &allfds, sizeof(fd_set));
		FD_SET(sd_kernel, &readfds);
		memcpy((char *) &exceptfds, (char *) &readfds, sizeof(fd_set));

		if (tofunc == 0) {
		    pto = 0;
		} else {
		  to.tv_sec = to_actual.tv_sec;
		  to.tv_usec = to_actual.tv_usec;
		  pto = &to;
/*
 * Declare a timeout based on a wild estimate.
 */
		    if (to_count > TO_DLO_ESTIMATE) {
			to_count = 0;
			(*tofunc)();
			return(0);
		    }
		}

		while (((nfd_ready = select(fd_max + 1, &readfds,
			(fd_set *) 0, (fd_set *) &exceptfds, pto)) < 0) &&
			(errno == EINTR)) {
		  memcpy((char *) &readfds, (char *) &allfds, sizeof(fd_set));
		  FD_SET(sd_kernel, &readfds);
		  memcpy((char *) &exceptfds, (char *) &readfds, sizeof(fd_set));
		}
		if (nfd_ready < 0) 
		  lampanic("lamd kernel: problem with select() (1)");
/*
 * The actual timeout expired.
 */
		if (nfd_ready == 0) {
		    to_count = 0;
		    (*tofunc)();
		    return(0);
		}
/*
 * Advance the estimated timeout.
 */
		if (tofunc != 0) {
		    to_count++;
		}
/*
 * Check for new clients.  No known client actions can cause an
 * error on accept.  Therefore any error condition is fatal.
 */
		if (FD_ISSET(sd_kernel, &readfds) ||
			FD_ISSET(sd_kernel, &exceptfds)) {
		    length = sizeof(client_un.sun_path);
		    sd_new = accept(sd_kernel, (localaddr_t) &client_un,
			    &length);
		    if (sd_new < 0) 
		      lampanic("lamd kernel: problem with accept()");
		    lamlog("kio_req: new client on fd=%d", sd_new);
		    nfd_ready--;
		    FD_SET(sd_new, &allfds);
		    FD_SET(sd_new, &clientfds);
		    FD_CLR(sd_kernel, &readfds);

		    if (sd_new > fd_max) {
			fd_max = sd_new;
		    }
/*
 * Set send/receive buffer sizes and close on exec.
 */
		    if (sfh_sock_set_buf_size(sd_new, SFH_UNIX, SO_SNDBUF,
			    	KPKTLEN + sizeof(struct nmsg))) {
		      lampanic("lamd kernel: problem with internal call sfh_sock_set_buf_size() (1)");
		    }

		    if (sfh_sock_set_buf_size(sd_new, SFH_UNIX, SO_RCVBUF,
			    	KPKTLEN + sizeof(struct nmsg))) {
		      lampanic("lamd kernel: problem with internal call sfh_sock_set_buf_size() (2)");
		    }

		    if (fcntl(sd_new, F_SETFD, 1) == -1) {
		      lampanic("lamd kernel: problem with fcntl()");
		    }
		}
	    }
/*
 * Find next ready descriptor.
 */
	    while ((! FD_ISSET(fd_ready, &readfds)) &&
		    (! FD_ISSET(fd_ready, &exceptfds))) {
	      ++fd_ready;
	    }
	    if (FD_ISSET(fd_ready, &readfds))
	      --nfd_ready;
	    if (FD_ISSET(fd_ready, &exceptfds))
	      --nfd_ready;
/*
 * Clean up errnoneous descriptor if there is nothing to read on that
 * fd.  It seems that some OSes will set the fd in both fd_sets, which
 * can cause problems for stdio forwarding (among others).  Because of
 * this, we were leaking resources badly on those OSes.
 *
 * We do this here (rather than just shoving this below the interupt
 * code) because we need to protect ourselves from reading off a
 * client fd that has nothing to read, but has an error set.  So we
 * have the following logic:
 *
 *  1) Deal with any erroneous fds that do not have something ready to
 *     read
 *  2) Deal with any client fds that have something to read - we know
 *     that there is something to read because the fd made it through the
 *     tests above to find the first fd that is either errneous or has
 *     something to read AND this test, which eliminates the
 *     erroroneous-only possibility.
 *  3) Deal with any interupts 
 */
	    if (FD_ISSET(fd_ready, &exceptfds) && 
		! FD_ISSET(fd_ready, &readfds)) {
		knuke(kpfindport(fd_ready));
		kio_close(fd_ready);
	    }
/*
 * Is this a client?
 */
	    else if (FD_ISSET(fd_ready, &clientfds)) {

		if (mread(fd_ready, (char *) &request, sizeof(struct kreq)) <
			(int)sizeof(struct kreq)) {
		    knuke(kpfindport(fd_ready));
		    kio_close(fd_ready);
		} else {
		    return(&request);
		}
	    }
/*
 * This must be an interrupt.
 */
	    else {

		for (i = 0; (i < KIMAX) && (kintrs[i].kn_fd != fd_ready); ++i);

		if (i >= KIMAX) {
		    errno = EIMPOSSIBLE;
		    lampanic("lamd kernel: problem with select() (2)");
		}

		prun = kintrs[i].kn_client;
		(*kintrs[i].kn_func)(kintrs[i].kn_fd);

		if (kintrs[i].kn_bclear) {
		    FD_CLR(kintrs[i].kn_fd, &allfds);
		    kintrs[i].kn_fd = -1;
		}

		return(0);
	    }
	}
}

/*
 *	kio_reply
 *
 *	Function:	- sends reply back to the client
 *			- message contains result of kernel request
 *	Accepts:	- client socket
 */
void
kio_reply(struct kreply *reply, int4 fd_client)
{
	mwrite(fd_client, (char *) reply, sizeof(struct kreply));
}

/*
 *	kio_intr
 *
 *	Function:	- associates an input fd with a function
 *	Accepts:	- interrupt fd
 *			- interrupt function
 *			- clear interrupt after trigger?
 */
void
kio_intr(int fd, void (*func)(), int bclear)
{
	int		i;

	for (i = 0; (i < KIMAX) && (kintrs[i].kn_fd >= 0) &&
			(kintrs[i].kn_fd != fd); ++i);

	if (i >= KIMAX) {
		errno = EFULL;
		lampanic("lamd kernel: problem with internal call kio_intr()");
	}

	kintrs[i].kn_fd = fd;
	kintrs[i].kn_func = func;
	kintrs[i].kn_client = prun;
	kintrs[i].kn_bclear = bclear;

	FD_SET(fd, &allfds);

	if (fd > fd_max) {
		fd_max = fd;
	}
}

/*
 *	kio_intr_clr
 *
 *	Function:	- clears interrupt associated with an input fd
 *	Accepts:	- interrupt fd
 */
void
kio_intr_clr(int fd)
{
	int		i;

	for (i = 0; (i < KIMAX) && (kintrs[i].kn_fd != fd); ++i);

	if (i < KIMAX) {
		kintrs[i].kn_fd = -1;
		FD_CLR(fd, &allfds);
	}
}

/*
 *	kio_to
 *
 *	Function:	- associates a timeout delay with a function
 *			- overwrites existing registration
 *	Accepts:	- delay
 *			- function
 *	Returns:	- 0
 */
int
kio_to(struct timeval *delay, void (*func)())
{
	tofunc = func;

	if (delay) {
		to_actual.tv_sec = delay->tv_sec;
		to_actual.tv_usec = delay->tv_usec;
	}

	return(0);
}

/*
 *	kio_recv
 *
 *	Function:	- receives a message from an external process
 *			  to an internal process
 *	Accepts:	- receive message descriptor
 *			- minimum message length
 *			- send client socket
 */
int
kio_recv(struct kmsg *recvkmsg, int4 minlen, int fd_client)
{
	struct iovec	iov[2];			/* scatter vectors */
	char		*sv_msg;

	if (recvkmsg->k_flags & KNMSG) {
		sv_msg = ((struct nmsg *) (recvkmsg->k_msg))->nh_msg;
		iov[0].iov_base = recvkmsg->k_msg;
		iov[0].iov_len = sizeof(struct nmsg);
		iov[1].iov_base = ((struct nmsg *) (recvkmsg->k_msg))->nh_msg;
		iov[1].iov_len = minlen;

		if (mreadv(fd_client, iov, 2) < (int)(iov[0].iov_len +
				iov[1].iov_len)) return(LAMERROR);
		((struct nmsg *) (recvkmsg->k_msg))->nh_msg = sv_msg;
	} else {
		if (mread(fd_client, recvkmsg->k_msg, minlen) < minlen)
				return(LAMERROR);
	}

	return(0);
}

/*
 *	kio_send
 *
 *	Function:	- sends a message from an internal process
 *			  to an external process
 *	Accepts:	- send message descriptor
 *			- minimum message length
 *			- receive client socket
 */
void
kio_send(struct kmsg *sendkmsg, int4 minlen, int fd_client)
{
	struct iovec	iov[2];			/* scatter vectors */

	if (sendkmsg->k_flags & KNMSG) {
		iov[0].iov_base = sendkmsg->k_msg;
		iov[0].iov_len = sizeof(struct nmsg);
		iov[1].iov_base = ((struct nmsg *) (sendkmsg->k_msg))->nh_msg;
		iov[1].iov_len = minlen;

		mwritev(fd_client, iov, 2);
	} else {
		mwrite(fd_client, sendkmsg->k_msg, minlen);
	}
}

/*
 *	kio_transfer
 *
 *	Function:	- moves message between two external processes
 *	Accepts:	- send message descriptor
 *			- send client socket
 *			- receive client socket
 *			- minimum message length
 */
void
kio_transfer(struct kmsg *sendkmsg, int fd_send, int fd_recv, int4 minlen)
{
	int4		remain;			/* remaining length */
	int4		len;			/* packet length */

	remain = minlen;

	if (sendkmsg->k_flags & KNMSG) {
		remain += sizeof(struct nmsg);
	}
/*
 * Transfer the message in blocks.
 */
	while (remain > 0) {
		len = (remain > KPKTLEN) ? KPKTLEN : remain;
		mread(fd_send, fwdbuf, len);
		mwrite(fd_recv, fwdbuf, len);
		remain -= len;
	}
}

/*
 *	kio_close
 *
 *	Function:	- closes a client socket
 *	Accepts:	- client socket
 */
void
kio_close(int fd_client)
{
	shutdown(fd_client, 2);
	close(fd_client);
	FD_CLR(fd_client, &allfds);
	FD_CLR(fd_client, &clientfds);
}

/*
 *	kio_fd_ready
 *
 *	Returns:	- fd_ready
 */
int
kio_fd_ready()
{
	return(fd_ready);
}

/*
 *	kio_shutdown
 *
 *	Function:	- cleanup and exit
 */
static void
kio_shutdown()
{
  pid_t pid;
  int status = 0;

  pid = fork();
 
  if (pid == 0) {
    /* child */

    /* GRRR....  Can't look at options in here - just use #define */
#if LAM_WANT_DEBUG
    execle(LAM_BINDIR "/tkill", "tkill", "-setsid", "-d", "-debug-output",
	   "syslog", NULL, environ);
#else
    execle(LAM_BINDIR "/tkill", "tkill", "-setsid", NULL, environ);
#endif

    /* Well, that didn't go well.  Parent already trying to clean up... */
    exit(1);
  }

	kkillall();
	shutdown(sd_kernel, 2);
	kio_cleanup();

	if (pid >= 0) {
	  /* Wait for tkill to get us, so that tkill must get scheduled... */
	  waitpid(pid, &status, 0);
	}

	exit(0);
}

/*
 *	kio_cleanup
 *
 *	Function:	- cleanup for fork() purposes
 */
void
kio_cleanup()
{
	close(sd_kernel);
}
