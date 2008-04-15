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
 *	Ohio Trollius
 *	Copyright 1995 The Ohio State University
 *	GDB
 *
 *	$Id: rfsimplex.c,v 6.6 2003/11/02 19:30:51 brbarret Exp $
 *
 *	Function:	- simple, basic, filed requests
 */

#include <errno.h>
#include <string.h>

#include <freq.h>
#include <net.h>
#include <portable.h>
#include <typical.h>
#include <t_types.h>

#define MYNULL		((char *) 0)

#ifndef MAXPATHLEN
#define MAXPATHLEN	1024
#endif

/*
 * external variables
 */
extern struct fclient	_ufd[FUMAX];	/* client file descriptor table */

/*
 * external functions
 */
extern int		_fdvalid(int);	/* check validity of file desc */
extern int4		_makefreq();	/* make simple file request */
extern char		*_fnparse();	/* parse nodeid from file name */

/*
 *	lam_rfmkdir
 *
 *	Function:	- make a new directory
 *	Accepts:	- ptr to extended file name
 *			- access mode for directory
 *	Returns:	- 0 or ERROR
 */
int
lam_rfmkdir(fname, mode)

CONST char		*fname;
int4			mode;

{
	int4		node;			/* server node ID */
	int4		len;			/* length of file name */

	fname = _fnparse(fname, &node);
	len = (int4) (strlen(fname) + 1);
	if (len > MAXNMSGLEN) len = MAXNMSGLEN;
	return((int) _makefreq(node, FQMKDIR, mode, fname, len, MYNULL,
		INT4_NIL));
}

/*
 *	lam_rfaccess
 *
 *	Function:	- check accessibility of a file
 *	Accepts:	- ptr to extended file name
 *			- access mode sought
 *	Returns:	- 0 or ERROR
 */
int
lam_rfaccess(fname, mode)

CONST char		*fname;
int4			mode;

{
	int4		node;			/* server node ID */
	int4		len;			/* length of file name */

	fname = _fnparse(fname, &node);
	len = (int4) (strlen(fname) + 1);
	if (len > MAXNMSGLEN) len = MAXNMSGLEN;
	return((int) _makefreq(node, FQACCESS, mode, fname, len, MYNULL,
		INT4_NIL));
}

/*
 *	lam_rfchdir
 *
 *	Function:	- change the current working directory
 *	Accepts:	- nodeid of filed
 *			- new working directory
 *	Returns:	- 0 or ERROR
 */
int
lam_rfchdir(node, newdir)

int4			node;
CONST char		*newdir;

{
	int4		len;

	len = (int4) (strlen(newdir) + 1);
	if (len > MAXNMSGLEN) len = MAXNMSGLEN;
	return((int) _makefreq(node, FQCHDIR, INT4_NIL, newdir, len, MYNULL,
		INT4_NIL));
}

/*
 *	lam_rfgetwd
 *
 *	Function:	- get the current working directory
 *	Accepts:	- nodeid of filed
 *			- caller's buffer for working directory
 *	Returns:	- 0 or ERROR
 */
char *
lam_rfgetwd(node, currdir)

int4			node;
char			*currdir;

{
	if (_makefreq(node, FQGETWD, INT4_NIL, MYNULL, INT4_NIL,
		currdir, (int4) MAXPATHLEN) == ERROR) {

		return(MYNULL);
	} else {
		return(currdir);
	}
}

/*
 *	lam_rfrmfd
 *
 *	Function:	- remove a filed descriptor
 *	Accepts:	- nodeid of filed
 *			- file descriptor handle
 *	Returns:	- 0 or ERROR
 */
int
lam_rfrmfd(node, tfd)

int4			node;
int4			tfd;

{
	return((int) _makefreq(node, FQRMFD, tfd, MYNULL, INT4_NIL, MYNULL,
		INT4_NIL));
}

/*
 *	lam_rfsystem
 *
 *	Function:	- issue a shell command
 *	Accepts:	- node ID of filed
 *			- shell command
 *	Returns:	- exit status
 */
int
lam_rfsystem(node, cmd)

int4			node;
CONST char		*cmd;

{
	int4		len;

	len = (int4) (strlen(cmd) + 1);
	if (len > MAXNMSGLEN) len = MAXNMSGLEN;
	return((int) _makefreq(node, FQSYSTEM, INT4_NIL, cmd, len, MYNULL,
		INT4_NIL));
}

/*
 *	lam_rfunlink
 *
 *	Function:	- remove a file
 *	Accepts:	- extended file name
 *	Returns:	- 0 or ERROR
 */
int
lam_rfunlink(fname)

CONST char		*fname;

{
	int4		node;			/* server node ID */
	int4		len;

	fname = _fnparse(fname, &node);
	len = (int4) (strlen(fname) + 1);
	if (len > MAXNMSGLEN) len = MAXNMSGLEN;
	return((int) _makefreq(node, FQUNLINK, INT4_NIL, fname, len, MYNULL,
		INT4_NIL));
}

/*
 *	lam_rfrmdir
 *
 *	Function:	- remove a directory
 *	Accepts:	- pathname of directory
 *	Returns:	- 0 or ERROR
 */
int
lam_rfrmdir(fname)

CONST char		*fname;

{
	int4		node;			/* server node ID */
	int4		len;

	fname = _fnparse(fname, &node);
	len = (int4) (strlen(fname) + 1);
	if (len > MAXNMSGLEN) len = MAXNMSGLEN;
	return((int) _makefreq(node, FQRMDIR, INT4_NIL, fname, len, MYNULL,
		INT4_NIL));
}

/*
 *	lam_rfftruncate
 *
 *	Function:	- set an open file to a specified length
 *	Accepts:	- file descriptor handle
 *			- new file length
 *	Returns:	- 0 or ERROR
 */
int
lam_rfftruncate(fd, len)

int			fd;
int4			len;

{
	if (_fdvalid(fd) == ERROR) {
		return(ERROR);
	}

	if ((fd <= 2) && (_ufd[fd].fu_node == NOTNODEID)) {
		_ufd[fd].fu_node = getorigin();
	}

	len = ltot(len);
	return((int) _makefreq(_ufd[fd].fu_node, FQFTRUNC, _ufd[fd].fu_tfd,
		(char *) &len, (int4) sizeof(int4), MYNULL, INT4_NIL));
}

/*
 *	lam_rftruncate
 *
 *	Function:	- set a named file to a specified length
 *	Accepts:	- file name
 *			- new file length
 *	Returns:	- 0 or ERROR
 */
int
lam_rftruncate(fname, len)

CONST char		*fname;
int4			len;

{
	int4		node;			/* server node ID */
	int4		lfname;			/* length of fname */

	fname = _fnparse(fname, &node);
	lfname = (int4) (strlen(fname) + 1);
	if (lfname > MAXNMSGLEN) lfname = MAXNMSGLEN;
	return((int) _makefreq(node, FQTRUNC, len, fname, lfname, MYNULL,
		INT4_NIL));
}

/*
 *	lam_rfincr
 *
 *	Function:	- increment usage count of a file descriptor
 *	Accepts:	- file descriptor handle
 *			- increment amount
 *	Returns:	- 0 or ERROR
 */
int
lam_rfincr(fd, amount)

int			fd;
int4			amount;

{
	if (_fdvalid(fd) == ERROR) {
		return(ERROR);
	}

	if ((fd <= 2) && (_ufd[fd].fu_node == NOTNODEID)) {
		_ufd[fd].fu_node = getorigin();
	}

	amount = ltot(amount);
	return((int) _makefreq(_ufd[fd].fu_node, FQINCR, _ufd[fd].fu_tfd,
		(char *) &amount, (int4) sizeof(int4), MYNULL, INT4_NIL));
}

/*
 *	lam_rfdup
 *
 *	Function:	- duplicate file descriptor
 *	Accepts:	- existing open file descriptor
 *	Returns:	- 0 or ERROR
 */
int
lam_rfdup(ofd)

int			ofd;

{
	int		nfd;			/* new file descriptor */

	if (_fdvalid(ofd) == ERROR) {
		return(ERROR);
	}
/*
 * Find slot in client fd table.
 */
	for (nfd = 3; (nfd < FUMAX) && (_ufd[nfd].fu_tflags != 0); ++nfd) {
	}

	if (nfd >= FUMAX) {
		errno = ENFILE;
		return(ERROR);
	}

	if ((ofd <= 2) && (_ufd[ofd].fu_node == NOTNODEID)) {
		_ufd[ofd].fu_node = getorigin();
	}

	_ufd[nfd].fu_tfd = _makefreq(_ufd[ofd].fu_node, FQDUP,
		_ufd[ofd].fu_tfd, MYNULL, INT4_NIL, MYNULL, INT4_NIL);

	if (_ufd[nfd].fu_tfd == ERROR) {
		return(ERROR);
	} else {
		_ufd[nfd].fu_tflags = _ufd[ofd].fu_tflags;
		_ufd[nfd].fu_node = _ufd[ofd].fu_node;
		return(nfd);
	}
}

/*
 *	lam_rfdup2
 *
 *	Function:	- duplicate file descriptor to specific handle
 *	Accepts:	- existing open file descriptor
 *			- new file descriptor
 *	Returns:	- 0 or ERROR
 */
int
lam_rfdup2(ofd, nfd)

int			ofd;
int			nfd;

{
	if (_fdvalid(ofd) == ERROR) {
		return(ERROR);
	}

	if ((nfd < 0) || (nfd >= FUMAX)) {
		errno = EBADF;
		return(ERROR);
	}

	if (_ufd[nfd].fu_tflags != 0) {

		if (lam_rfclose(nfd) == ERROR) {
			return(ERROR);
		}
	}

	if ((ofd <= 2) && (_ufd[ofd].fu_node == NOTNODEID)) {
		_ufd[ofd].fu_node = getorigin();
	}

	_ufd[nfd].fu_tfd = _makefreq(_ufd[ofd].fu_node, FQDUP,
		_ufd[ofd].fu_tfd, MYNULL, INT4_NIL, MYNULL, INT4_NIL);

	if (_ufd[nfd].fu_tfd == ERROR) {
		return(ERROR);
	} else {
		_ufd[nfd].fu_tflags = _ufd[ofd].fu_tflags;
		_ufd[nfd].fu_node = _ufd[ofd].fu_node;
		return(nfd);
	}
}

/*
 * backwards compatibility
 */
int
rfmkdir(CONST char *fname, int4 mode)
{
  return(lam_rfmkdir(fname, mode)); 
}

int
rfaccess(CONST char *fname, int4 mode)
{
  return(lam_rfaccess(fname, mode)); 
}

int
rfchdir(int4 node, CONST char *newdir)
{
  return(lam_rfchdir(node, newdir)); 
}

char *
rfgetwd(int4 node, char *currdir)
{
  return(lam_rfgetwd(node, currdir)); 
}

int
rfrmfd(int4 node, int4 tfd)
{
  return(lam_rfrmfd(node, tfd)); 
}

int
rfsystem(int4 node, CONST char *cmd)
{
  return(lam_rfsystem(node, cmd)); 
}

int
rfunlink(CONST char *fname)
{
  return(lam_rfunlink(fname)); 
}

int
rfrmdir(CONST char *fname)
{ 
  return(lam_rfrmdir(fname)); 
}

int
rfftruncate(int fd, int4 len)
{ 
  return(lam_rfftruncate(fd, len)); 
}

int
rftruncate(CONST char *fname, int4 len)
{ 
  return(lam_rftruncate(fname, len)); 
}

int
rfincr(int fd, int4 amount)
{
  return(lam_rfincr(fd, amount)); 
}

int
rfdup(int ofd)
{ 
  return(lam_rfdup(ofd)); 
}

int
rfdup2(int ofd, int nfd)
{
  return(lam_rfdup2(ofd, nfd));
}
