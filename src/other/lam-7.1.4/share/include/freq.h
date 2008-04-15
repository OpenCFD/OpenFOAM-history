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
 *	$Id: freq.h,v 6.9 2003/11/02 19:30:52 brbarret Exp $
 *
 *	Function:	- filed request/reply templates and constants
 *
 */

#ifndef _FREQ
#define _FREQ

#include <lam_config.h>
#include <portable.h>
#include <sys/param.h>

/*
 * the bread and butter request codes
 */
#define FQOPEN		(0) 
#define FQCLOSE		(1) 
#define FQREAD		(2)
#define FQWRITE		(3) 
#define FQLSEEK		(4) 

/*
 * Trollius specific requests
 * FQIDLE and FQSTATE have the same # (dirty coding trick).
 */
#define FQSTATE		(5)		/* filed status */
#define FQRMFD		(6)		/* blow away one or all file descs */
#define FQINCR		(7)		/* increment usage counter */

/*
 * request for the enthusiasts
 */
#define FQACCESS	(8)
#define FQMKDIR		(9) 
#define FQUNLINK	(10) 
#define FQSYSTEM	(11) 
#define FQCHDIR		(12) 
#define FQRMDIR		(13) 
#define FQGETWD		(14) 
#define FQSTAT		(15) 
#define FQFSTAT		(16) 
#define FQDUP		(17)
#define FQTRUNC		(18) 
#define FQFTRUNC	(19)
#define FQOPENFD	(20)
#define FQPATHFIND	(21)
#define FQRENAME        (22)

/*
 * requests for F77
 */
#define FQF77READ	(23)
#define FQF77BACK	(24)
#define FQMAX		(25)		/* # of valid requests */

/*
 * Trollius specific open flags
 */
#define	LAM_O_RDONLY	((int4) 0x1)
#define	LAM_O_WRONLY	((int4) 0x2)
#define	LAM_O_RDWR	((int4) 0x4)

#define LAM_O_ACCMODE	(LAM_O_RDONLY | LAM_O_WRONLY | LAM_O_RDWR)

#define	LAM_O_APPEND	((int4) 0x10)
#define	LAM_O_CREAT	((int4) 0x20)
#define	LAM_O_TRUNC	((int4) 0x40)
#define	LAM_O_EXCL	((int4) 0x80)

#define	LAM_O_LOCK	((int4) 0x10000)
#define	LAM_O_REUSE	((int4) 0x20000)
#define	LAM_O_DRAW	((int4) 0x0)
#define	LAM_O_DINT4	((int4) 0x200000)
#define	LAM_O_DFLT4	((int4) 0x400000)
#define	LAM_O_DFLT8	((int4) 0x800000)
#define	LAM_O_1WAY	((int4) 0x1000000)
#define LAM_O_ACTIVE	((int4) 0x2000000)

#define LAM_O_DMSG	(LAM_O_DINT4 | LAM_O_DFLT4 | LAM_O_DFLT8)

#define LAM_O_FILED	(LAM_O_LOCK | LAM_O_REUSE | LAM_O_1WAY | LAM_O_DMSG)

/*
 * backward compatibility
 */
#define	O_LOCK		LAM_O_LOCK		
#define	O_REUSE		LAM_O_REUSE	
#define	O_1WAY		LAM_O_1WAY
#define	O_DINT		LAM_O_DINT4
#define	O_DFLT		LAM_O_DFLT4
#define	O_DDBL		LAM_O_DFLT8

/*
 * Trollius status flags
 */
#define FLOCK		LAM_O_LOCK
#define F1WAY		LAM_O_1WAY
#define FACTIVE		LAM_O_ACTIVE

/*
 * limits
 */
#define FUMAX		32		/* per process fd limit, min 3 */

/*
 * special values
 */
#define NOTFD		-1		/* not a file descriptor handle */

/*
 * client (user) file descriptor
 */
struct fclient {
	int4		fu_node;	/* server node ID */
	int4		fu_tfd;		/* filed descriptor */
	int4		fu_tflags;	/* open flags, 0 - unused */
};

/*
 * filed reply
 */
struct freply {
        int4		fr_errno;
        int4		fr_ret;
};

/*
 * filed request
 */
struct freq {
	int4		fq_req;
	int4		fq_src_node;
	int4		fq_src_event;
	int4		fq_param0;
#define	fq_tfd		fq_param0
#define fq_flags	fq_param0
#define fq_accmode	fq_param0
#define fq_trunclen	fq_param0
	int4		fq_param1;
#define	fq_mode		fq_param1
#define	fq_length	fq_param1
#define	fq_offset	fq_param1
#define	fq_newflags	fq_param1
	int4		fq_param2;
#define	fq_whence	fq_param2
#define	fq_fmt		fq_param2
};

struct fstate {
	int4		fs_tfd;		/* Trollius file descriptor */
	int4		fs_tflags;	/* Trollius peculiar flags */
	int4		fs_flow;	/* total data flow */
	int4		fs_count;	/* total data flow */
	int4		fs_src_node;	/* last client's node */
	int4		fs_src_event;	/* last client's node */
	char		fs_name[24];	/* file name */
};

struct fqrename {
  char source[MAXPATHLEN];
  char dest[MAXPATHLEN];
};


#ifdef __cplusplus
extern "C" {
#endif

extern int              lam_rf77back __ARGS((int fd, char fmt));
extern int4             lam_rf77read __ARGS((int fd, char *buf, int4 len, char fmt));
extern char		*lam_rfgetwd __ARGS((int4 node, char *dir));
extern int		lam_rfaccess __ARGS((const char *fname, int4 mode));
extern int		lam_rfchdir __ARGS((int4 node, const char *dir));
extern int		lam_rfclose __ARGS((int fd));
extern int		lam_rfdup __ARGS((int fd));
extern int		lam_rfdup2 __ARGS((int fd, int newfd));
extern int		lam_rffstat __ARGS((int fd, char *status));
extern int		lam_rfftruncate __ARGS((int fd, int4 len));
extern int		lam_rfincr __ARGS((int fd, int4 amount));
extern int		lam_rfmkdir __ARGS((const char *fname, int4 mode));
extern int		lam_rfopen __ARGS((const char *fname,
					int4 flags, int4 mode));
extern char *		lam_rfpathfind __ARGS((const char *fname, 
					       const char *cwd, int nodeid));
extern int		lam_rfrename __ARGS((const char *rffrom, 
					     const char *rfto));
extern int		lam_rfrmdir __ARGS((const char *fname));
extern int		lam_rfrmfd __ARGS((int4 node, int4 tfd));
extern int		lam_rfstat __ARGS((const char *fname, char *status));
extern int		lam_rfstate __ARGS((int node, struct fstate *tbl,
					int n));
extern int		lam_rfsystem __ARGS((int4 node, const char *cmd));
extern int		lam_rftruncate __ARGS((const char *fname, int4 len));
extern int		lam_rfunlink __ARGS((const char *fname));
extern int4		lam_rflseek __ARGS((int fd, int4 offset, int whence));
extern int4		lam_rfread __ARGS((int fd, char *buf, int4 len));
extern int4		lam_rfwrite __ARGS((int fd, const char *buf,
					int4 len));
extern int4		_lam_rfwrite __ARGS((int4 node, int4 tfd, int4 tflags,
					const char *buf, int4 len));
extern void		lam_rfatexit __ARGS((void));
extern int		lam_lfopenfd __ARGS((char *server));

  /*
   * Backward compatibility
   */
extern int		rfaccess __ARGS((const char *fname, int4 mode));
extern void		rfatexit __ARGS((void));
extern int		rfclose __ARGS((int fd));
extern int		rffstat __ARGS((int fd, char *status));
extern char		*rfgetwd __ARGS((int4 node, char *dir));
extern int		rfaccess __ARGS((const char *fname, int4 mode));
extern int		rfchdir __ARGS((int4 node, const char *dir));
extern int		rfdup __ARGS((int fd));
extern int		rfdup2 __ARGS((int fd, int newfd));
extern int4		rflseek __ARGS((int fd, int4 offset, int whence));
extern int		rfmkdir __ARGS((const char *fname, int4 mode));
extern int		rfftruncate __ARGS((int fd, int4 len));
extern int		rfincr __ARGS((int fd, int4 amount));
extern int		rfopen __ARGS((const char *fname,
					int4 flags, int4 mode));
extern int4		rfread __ARGS((int fd, char *buf, int4 len));
extern int		rfrmdir __ARGS((const char *fname));
extern int		rfrmfd __ARGS((int4 node, int4 tfd));
extern int		rfstat __ARGS((const char *fname, char *status));
extern int		rfsystem __ARGS((int4 node, const char *cmd));
extern int		rftruncate __ARGS((const char *fname, int4 len));
extern int4		rfwrite __ARGS((int fd, const char *buf,
					int4 len));
extern int		rfunlink __ARGS((const char *fname));

#ifdef __cplusplus
}
#endif

#endif
