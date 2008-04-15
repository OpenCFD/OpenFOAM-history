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
 *	Copyright 1997 The Ohio State University
 *	NJN
 *
 *	$Id: rfstat.c,v 6.10 2003/11/02 19:30:52 brbarret Exp $
 *
 *	Function:	- rffstat() and rfstat()
 *			- get file status
 */

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include <freq.h>
#include <net.h>
#include <portable.h>
#include <typical.h>

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
 * local functions
 */
/* convert to status structure */
static void		conv_stat(struct stat *status, uint4 *buf);

/*
 *	lam_rfstat
 *
 *	Function:	- get status on a named file
 *	Accepts:	- file name
 *			- ptr to status buffer
 *	Returns:	- 0 or ERROR
 */
int
lam_rfstat(CONST char *fname, char *status)
{
	int4		node;			/* server node ID */
	int4		len;
	uint4		buf[17];
	int		ret;

	fname = _fnparse(fname, &node);
	len = (int4) (strlen(fname) + 1);
	if (len > MAXNMSGLEN) len = MAXNMSGLEN;

	ret = _makefreq(node, FQSTAT, INT4_NIL, fname, len, (char *) buf,
		(int4) sizeof(buf));

	if (ret == 0) {
		conv_stat((struct stat *) status, buf);
	}

	return(ret);
}

/*
 *	lam_rffstat
 *
 *	Function:	- get status on a file descriptor
 *	Accepts:	- file descriptor handle
 *			- ptr to status buffer
 *	Returns:	- 0 or ERROR
 */
int
lam_rffstat(int fd, char *status)
{
	uint4		buf[17];
	int		ret;

	if (_fdvalid(fd)) return(LAMERROR);

	if ((fd <= 2) && (_ufd[fd].fu_node == NOTNODEID)) {
		_ufd[fd].fu_node = getorigin();
	}

	ret = _makefreq(_ufd[fd].fu_node, FQFSTAT, _ufd[fd].fu_tfd,
		(char *) 0, INT4_NIL, (char *) buf, (int4) sizeof(buf));

	if (ret == 0) {
		conv_stat((struct stat *) status, buf);
	}

	return(ret);
}


#if !LAM_STAT_MEMBERS_INT4
/*
 * stuff_buff
 *
 * Function:  - stuff a buffer with a quantity
 *            - zero-pads dest if dest is longer than src
 * Accepts:   - two buffers and corresponding sizes
 */
static void
stuff_buff(char *src, int src_size, char *dest, int dest_size)
{
  if (src_size >= dest_size)
    memcpy(dest, src, dest_size);
  else {
    memcpy(dest, src, src_size);
    memset(dest + src_size, 0, dest_size - src_size);
  }
}
#endif


/*
 *	conv_stat
 *
 *	Function:	- stores an int4 array into a status structure
 *	Accepts:	- ptr to stat structure
 *			- ptr to int4 array
 */
static void
conv_stat(struct stat *status, uint4 *buf)
{
#if LAM_STAT_MEMBERS_INT4
	status->st_dev = buf[0];
	status->st_ino = buf[1];
	status->st_mode = buf[2];
	status->st_nlink = buf[3];
	status->st_uid = buf[4];
	status->st_gid = buf[5];
	status->st_size = buf[6];
	status->st_atime = buf[7];
	status->st_mtime = buf[8];
	status->st_ctime = buf[9];
#else
  stuff_buff((char*) &buf[0], sizeof(uint4), 
	     (char*) &status->st_dev, sizeof(status->st_dev));
  stuff_buff((char*) &buf[1], sizeof(uint4), 
	     (char*) &status->st_ino, sizeof(status->st_ino));
  stuff_buff((char*) &buf[2], sizeof(uint4), 
	     (char*) &status->st_mode, sizeof(status->st_mode));
  stuff_buff((char*) &buf[3], sizeof(uint4), 
	     (char*) &status->st_nlink, sizeof(status->st_nlink));
  stuff_buff((char*) &buf[4], sizeof(uint4), 
	     (char*) &status->st_uid, sizeof(status->st_uid));
  stuff_buff((char*) &buf[5], sizeof(uint4), 
	     (char*) &status->st_gid, sizeof(status->st_gid));
  stuff_buff((char*) &buf[6], sizeof(uint4), 
	     (char*) &status->st_size, sizeof(status->st_size));
  stuff_buff((char*) &buf[7], sizeof(uint4), 
	     (char*) &status->st_atime, sizeof(status->st_atime));
  stuff_buff((char*) &buf[8], sizeof(uint4), 
	     (char*) &status->st_mtime, sizeof(status->st_mtime));
  stuff_buff((char*) &buf[9], sizeof(uint4), 
	     (char*) &status->st_ctime, sizeof(status->st_ctime));
#endif
}

/*
 * backwards compatibility
 */
int
rfstat(CONST char *fname, char *status)
{
  return(lam_rfstat(fname, status));
}

int
rffstat(int fd, char *status)
{
  return(lam_rffstat(fd, status)); 
}
