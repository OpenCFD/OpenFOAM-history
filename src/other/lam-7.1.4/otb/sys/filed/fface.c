/*
 * Copyright (c) 2001-2002 The Trustees of Indiana University.  
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
 *	$Id: fface.c,v 6.10 2003/06/30 05:19:30 brbarret Exp $
 * 
 *	Function:	- abstracts native filesystem interface
 *			- Trollius remote file access follows the UNIX I/O
 *			  standard assuming a 32 bit interface (int4).
 *			- In case the standard UNIX calls are not available,
 *			  they may be emulated in this module.
 *			- UNIX version
 */

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <stdio.h>

#include "filed.h"
#include <freq.h>
#include <net.h>
#include <typical.h>

/*
 * local functions
 */
static void		conv_stat();
static int4		conv_flags();

/*
 * UNIX version - boring, so just blurt them out.
 * Here we know that int4 is int.
 * In fmyopen(), convert LAM flags to their UNIX equivalent.
 */

int4
fmyopen(path, flags, mode)

char			*path;
int4			flags;
int4			mode;

{
	int4		fd;

	fd = open(path, conv_flags(flags) | O_NOCTTY, mode);
	return(fd);
}

int4 fmyclose(fd) int4 fd;
{ return(close(fd)); }

int4 fmyread(fd, buf, nbytes) int4 fd; char *buf; int4 nbytes;
{ return(read(fd, buf, nbytes)); }

int4 fmywrite(fd, buf, nbytes) int4 fd; char *buf; int4 nbytes;
{ return(write(fd, buf, nbytes)); }

int4 fmylseek(fd, offset, whence) int4 fd; int4 offset; int4 whence;
{ return(lseek(fd, (off_t) offset, whence)); }

int4 fmychdir(newdir) char *newdir;
{ return(chdir(newdir)); }

char *fmygetwd(dir) char *dir;
#if LAM_HAVE_GETCWD
{ return(getcwd(dir, MAXNMSGLEN)); }
#else
{ return(getwd(dir)); }
#endif

int4 fmyunlink(name) char *name;
{ return(unlink(name)); }

int4 fmyrmdir(dir) char *dir;
{ return(rmdir(dir)); }

int4 fmysystem(cmd) char *cmd;
{ return(system(cmd)); }

int4 fmymkdir(path, mode) char *path; int4 mode;
{ return(mkdir(path, mode)); }

int4 fmyaccess(path, mode) char *path; int4 mode;
{ return(access(path, mode)); }

int4 
fmyrename(CONST char *from, CONST char *to)
{
  return rename(from, to);
}

#ifndef SCO

int4 fmytrunc(path, len) char *path; int4 len;
{ return(truncate(path, (off_t) len)); }

int4 fmyftrunc(fd, len) int4 fd; int4 len;
{ return(ftruncate(fd, (off_t) len)); }

#else /* SCO */

/*
 * Simulate truncate() by opening the file.
 */
int4 fmytrunc(path, len) char *path; int4 len;
{
	int4		fd;

	fd = open(path, O_WRONLY, 0);
	if (fd < 0) {
		return((int4) ERROR);
	}

	if (chsize(fd, (off_t) len)) {
		return((int4) ERROR);
	}

	return((int4) close(fd));
}

int4 fmyftrunc(fd, len) int4 fd; int4 len;
{ return(chsize(fd, (off_t) len)); }

#endif /* SCO */

/*
 *	fmyinit
 *
 *	Function:	- machine dependent initialization
 *			- in UNIX case, take care of process group/
 *			  controlling terminal headaches
 *	Returns:	- 0 or ERROR
 */
int
fmyinit()

{
/*
 * Create a new session.  This detaches our controlling terminal
 * so we don't get TTIN and TTOU signals when reading/writing the tty.
 * We also get out of the tty's process group so that we don't
 * catch special character signals from it.
 *
 * In the future, we will have worker processes to deal with ttys
 * (and all weird and/or slow devices) so we won't have this hassle.
 */
/*
 * I thought I vaguely understood this subject.  Current testing reveals
 * that neither of the above two problems exists if I fail to call setsid().
 * It is clearly documented that I cannot make this call if I am already
 * a session leader and I want to be able to run lamd from the shell.
 *
 *	if (setsid() < 0) {
 *		return(ERROR);
 *	}
 */
	return(0);
}

/*
 *	fmystat
 */
int4
fmystat(path, buf)

char			*path;
struct stat		*buf;

{
	struct stat	status;
	int4		ret;

	ret = stat(path, &status);
	conv_stat(&status, (unsigned int *) buf);
	return(ret);
}

/*
 *	fmyfstat
 */
int4
fmyfstat(fd, buf)

int4			fd;
struct stat		*buf;

{
	struct stat	status;
	int4		ret;

	ret = fstat(fd, &status);
	conv_stat(&status, (unsigned int *) buf);
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
stuff_buff(char *dest, int dest_size, char *src, int src_size)
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
 *	Function:	- stores status structure in an int4 array
 *	Accepts:	- ptr to stat structure
 *			- ptr to int4 array
 */
static void
conv_stat(status, buf)

struct stat		*status;
uint4			*buf;

{
#if LAM_STAT_MEMBERS_INT4
	buf[0] = (uint4) status->st_dev;
	buf[1] = (uint4) status->st_ino;
	buf[2] = (uint4) status->st_mode;
	buf[3] = (uint4) status->st_nlink;
	buf[4] = (uint4) status->st_uid;
	buf[5] = (uint4) status->st_gid;
	buf[6] = (uint4) status->st_size;
	buf[7] = (uint4) status->st_atime;
	buf[8] = (uint4) status->st_mtime;
	buf[9] = (uint4) status->st_ctime;
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
 *	conv_flags
 *
 *	Function:	- convert LAM flags to UNIX open() flags
 *	Accepts:	- LAM flags
 *	Returns:	- UNIX flags
 */
static int4
conv_flags(lflags)

int4			lflags;

{
	int4		uflags;

	uflags = 0;

	if (lflags & LAM_O_RDONLY) { uflags |= O_RDONLY; }
	if (lflags & LAM_O_WRONLY) { uflags |= O_WRONLY; }
	if (lflags & LAM_O_RDWR) { uflags |= O_RDWR; }
	if (lflags & LAM_O_APPEND) { uflags |= O_APPEND; }
	if (lflags & LAM_O_CREAT) { uflags |= O_CREAT; }
	if (lflags & LAM_O_EXCL) { uflags |= O_EXCL; }
	if (lflags & LAM_O_TRUNC) { uflags |= O_TRUNC; }

	return(uflags);
}
