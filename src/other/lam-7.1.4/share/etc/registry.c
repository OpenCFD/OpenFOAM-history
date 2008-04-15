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
 * $Id: registry.c,v 1.14 2003/05/31 19:39:55 brbarret Exp $
 *
 *	Function:	- object registry
 *			- mechanism for registering semi-persistent
 *			  objects for cleanup
 *			- objects are registered with their type
 *				m	SYSV shared memory segment
 *				s	SYSV semaphore set
 *				f	temporary file
 *
 */

#include <lam_config.h>

#if __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif
#include <errno.h>
#include <fcntl.h>
#include <lamdebug.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#if LAM_HAVE_SYSV_SHMEM
#include <sys/ipc.h>
#include <sys/shm.h>
#endif
#if LAM_HAVE_SYSV_SEMAPHORES
#include <sys/sem.h>
#endif 
#include <unistd.h>

#include <terror.h>
#include <typical.h>
#include <args.h>
#include <laminternal.h>
#include <etc_misc.h>

#ifndef MAXPATHLEN
#define MAXPATHLEN	1024
#endif


/*
 * local constants
 */
#define	FORMBUFLEN	(MAXPATHLEN + 16)	/* size of formatting buffer */


/*
 * private functions
 */
static char		*lam_reg_filename(void);
static char		*lam_reg_tmpname(void);
static char		*lam_reg_lockname(void);
static void		clean_warning(char *msg);
static void		del_warning(char *msg);
static int              set_names(char *msg);
static int              lock_registry(char *name);
static void             unlock_registry(void);
static int              filelock(int fd, int cmd, int type, off_t offset, 
				 int whence, off_t len);
static void             free_filenames(void);


/*
 * private variables
 */
static FILE		*f = 0;			/* registry file */
static FILE		*tmpf = 0;		/* temporary file */
static char		*reg_fname = 0;		/* registry filename */
static char		*reg_tmpname = 0;	/* temporary filename */
static char		*reg_lockname = 0;	/* registry lock filename */
static char		*reg_mylockname = 0;	/* registry lock filename */
static int		lockfd = -1;		/* lock file descriptor */
static int              oexcl_works = 0;        /* does O_EXCL work? */

#if LAM_HAVE_SYSV_SEMAPHORES

static	union semun	semctl_arg;

#endif /* LAM_HAVE_SYSV_SEMAPHORES */


/*
 *	lam_register_objects
 *
 *	Function:	- add objects to registry
 *	Accepts:	- number of objects
 *			- variable length list of (int, char *) pairs
 *			  <obj type> <obj name>, <obj type> <obj name>, ...
 *			  type is 'm' => shared memory, 's' => semaphore set
 *			  	  'f' => file 
 */
#if __STDC__
void
lam_register_objects(int nobjs, ...)
#else
void
lam_register_objects(nobjs, va_alist)
int			nobjs;
va_dcl
#endif
{
	char		buf[FORMBUFLEN];	/* formatting buffer */
	char		type;			/* object type */
	char		*obj;			/* object name */
	int		fd;			/* registry file descriptor */
	int		i;
	va_list		arglist;

#if __STDC__
	va_start(arglist, nobjs);
#else
	va_start(arglist);
#endif

	if (set_names("lam_register_objects") == LAMERROR)
	  return;
	if (lock_registry("lam_register_objects") == LAMERROR)
	  return;
/*
 * Open the registry file and add the objects.
 */
	fd = open(reg_fname, O_WRONLY | O_APPEND | O_CREAT, 0600);

	if (fd < 0) {
	  unlock_registry();
	  return;
	}

	for (i = 0; i < nobjs; i++) {
		type = (char) va_arg(arglist, int);
		obj = va_arg(arglist, char *);

		switch (type) {
		case 'm':
		case 's':
		case 'f':
			sprintf(buf, "%c %s\n", type, obj);
			write(fd, buf, strlen(buf));
			break;
		default:
			close(fd);
			unlock_registry();
			va_end(arglist);
			return;
		}
	}

	close(fd);
	unlock_registry();
        free_filenames();
	va_end(arglist);
}


/*
 *	lam_deregister_object
 *
 *	Function:	- deregister object from registry file
 *			- does not delete the object, this should be done
 *			  before calling this function
 *	Accepts:	- type of the object to deregister
 *			- name of the object to deregister
 */
void
lam_deregister_object(int deltype, char *delobj)
{
    struct stat		fstatus;		/* registry file status */
    int			fd;			/* registry file descriptor */
    char		obj[MAXPATHLEN];	/* object name */
    char		type;			/* object type */

    if (set_names("lam_deregister_object") == LAMERROR)
      return;
    if (lock_registry("lam_deregister_object") == LAMERROR)
      return;
/*
 * Open the registry and temporary files.
 */
    if ((f = fopen(reg_fname, "r")) == 0) {
	del_warning("lam_deregister_object (fopen)");
	return;
    }

    fd = fileno(f);
    if (fstat(fd, &fstatus) == -1) {
	del_warning("lam_deregister_object (fstat)");
	return;
    }

    if (fstatus.st_size == 0) {
	fclose(f);
	f = 0;
	unlock_registry();
	return;
    }

    if ((tmpf = fopen(reg_tmpname, "w")) == 0) {
	del_warning("lam_deregister_object (fopen)");
	return;
    }

    while (fscanf(f, "%c %s\n", &type, obj) == 2) {
	if (deltype != type || strcmp(obj, delobj) != 0) {
	    fprintf(tmpf, "%c %s\n", type, obj);
	}
    }
/*
 * Replace registry file with temporary file.
 */
    fclose(tmpf);
    tmpf = 0;
    fclose(f);
    f = 0;

    if (rename(reg_tmpname, reg_fname)) {
	del_warning("lam_deregister_object (rename)");
	return;
    }

    unlock_registry();
    free_filenames();
}


/*
 *	lam_cleanup_objects
 *
 *	Function:	- clean all registered objects
 *	Returns:	- 0 or LAMERROR
 */
void
lam_cleanup_objects(void)
{
    int			sysvid;			/* SYSV IPC shared id */
    char		type;			/* shared structure type */
    char		obj[MAXPATHLEN];	/* object name */

    if (set_names("lam_cleanup_objects") == LAMERROR)
      return;
    if (lock_registry("lam_cleanup_objects") == LAMERROR)
      return;
/*
 * Open the registry file.
 */
    if ((f = fopen(reg_fname, "r")) == 0) {
	if (errno != ENOENT) {
	    clean_warning("lam_cleanup_objects (fopen)");
	}
    } else {
      while (fscanf(f, "%c %s\n", &type, obj) == 2) {

	/* Don't check for errors here as structures may not exist. */

	switch (type) {
#if LAM_HAVE_SYSV_SHMEM
	case 'm':
	    sysvid = atoi(obj);
	    shmctl(sysvid, IPC_RMID, (struct shmid_ds *) 0);
	    break;
#endif
#if LAM_HAVE_SYSV_SEMAPHORES
	case 's':
	    sysvid = atoi(obj);
	    semctl(sysvid, 0, IPC_RMID, semctl_arg);
	    break;
#endif
	case 'f':
	    unlink(obj);
	    break;
	default:
	    break;
	}
      }

      fclose(f);
      f = 0;

      unlink(reg_fname);
    }

    unlock_registry();
    free_filenames();
}


/*
 *	lam_reg_filename
 *
 *	Function:	- forms shared structure registry filename
 *			- allocates space
 *	Returns:	- registry filename
 */
static char *
lam_reg_filename(void)
{
    	char		*fname;
	
	fname = lam_get_tmpdir();
	if (fname == 0) return(0);

	strcat(fname, "/lam-registry");
	return(fname);
}

/*
 *	lam_reg_filename
 *
 *	Function:	- forms temporary registry filename
 *			- allocates space
 *	Returns:	- temporary registry filename
 */
static char *
lam_reg_tmpname(void)
{
    	char		*fname;

	fname = lam_get_tmpdir();
	if (fname == 0) return(0);

	strcat(fname, "/lam-registry-tmp");
	return(fname);
}


/*
 *	lam_reg_lockname
 *
 *	Function:	- forms registry lock filename
 *			- allocates space
 *	Returns:	- registry lock filename
 */
static char *
lam_reg_lockname(void)
{
    	char		*fname;

	fname = lam_get_tmpdir();
	if (fname == 0) return(0);

	strcat(fname, "/lam-registry-lock");
	return(fname);
}


/*
 *	clean_warning
 *
 *	Function:	- print warning of error while cleaning and
 *			  close work files
 *			- errno contains the error code
 *	Accepts:	- message
 */
static void
clean_warning(char *msg)
{
	char		errmsg[256];		/* error message */

	lam_errorstr(errmsg, sizeof(errmsg));
	fprintf(stderr,
		"%s: Warning: %s: deleting shared memory structures\n",
		msg, reg_fname);
	fprintf(stderr,
		"%s: Warning: \"%s\"\n",
		msg, errmsg);
	fprintf(stderr, 
		"%s: Warning: Manual cleanup may be necessary. Try ipcrm.\n",
		msg);

	if (f) {
		fclose(f);
		f = 0;
	}
	if (tmpf) {
		fclose(tmpf);
		tmpf = 0;
		unlink(reg_tmpname);
	}
}


/*
 *	del_warning
 *
 *	Function:	- print warning of error while deleting an id from
 *			  the registry file and then close work files
 *			- errno contains the error code
 *	Accepts:	- message
 */
static void
del_warning(char *msg)
{
    char		errmsg[128];		/* error message */

    lam_errorstr(errmsg, sizeof(errmsg));
    if (errno) {
	fprintf(stderr,
		"%s: Warning: %s: deleting shared memory id's from registry.\n",
		msg, errmsg);
	fprintf(stderr,
		"%s: Warning: \"%s\"\n",
		msg, reg_fname);
    } else {
	fprintf(stderr, "%s\n", msg);
    }

    if (f) {
	fclose(f);
	f = 0;
    }
    if (tmpf) {
	fclose(tmpf);
	tmpf = 0;
	unlink(reg_tmpname);
    }
}


/*
 *	set_names
 *
 *	Function:	- setup various filenames in static char*'s
 *	Accepts:	- name of calling function
 *	Returns:	- 0 or LAMERROR
 */
static int
set_names(char *name)
{
  int fd;

  if (reg_fname == 0) {
    reg_fname = lam_reg_filename();
    if (reg_fname == 0) {
      fprintf(stderr, "%s: Problem obtaining registry filename\n", name);
      return LAMERROR;
    }
    
    reg_tmpname = lam_reg_tmpname();
    if (reg_tmpname == 0) {
      fprintf(stderr, "%s: Problem obtaining temporary filename\n", name);
      return LAMERROR;
    }
    
    reg_lockname = lam_reg_lockname();
    if (reg_lockname == 0) {
      fprintf(stderr, "%s: Problem obtaining registry lock filename\n", name);
      return LAMERROR;
    }

    /* Test to see if we can make the lockfile exclusively -- this may
       fail if the lockfile is on NFS.  Make a unique filename and try
       to open it with O_EXCL.  If we fail (note that we can't get
       EEXIST, because this is a unique filename, so there's no
       possibility that some other process ahs already created it),
       then O_EXCL won't work.  See lock_registry(), below, for more
       info on this. */

    reg_mylockname = malloc(MAXPATHLEN);
    snprintf(reg_mylockname, MAXPATHLEN - 1, "%s.%d", 
	     reg_lockname, lam_getpid());
    fd = open(reg_mylockname, O_RDWR | O_CREAT | O_EXCL, 0600);
    if (fd == -1) {
      oexcl_works = 0;
    } else {
      oexcl_works = 1;
      close(fd);
    }

    /* we need to unlink the filename regardless of whether the open
       succeeded or not -- some broken filesystems (cough cough linux
       NFS cough cough) will leave a broken entry in the directory
       listing even if the create failed.  <sigh> */
    
    unlink(reg_mylockname);
  }

  return 0;
}


/*
 *	lock_registry
 *
 *	Function:	- open the registry lockfile atomicly
 *	Accepts:	- name of invoking function
 *	Returns:	- 0 or LAMERROR
 */
static int
lock_registry(char *name)
{
  struct stat statbuf;

  /* This gets complicated.  If the lockfile is on NFS, open(2) (on
     linux, at least) says that O_EXCL is broken.  Hence, we have to
     do something different to assure atomicity. */

  /* Standard method -- open lock file atomicly with O_EXCL */

  if (oexcl_works == 1) {
    lockfd = open(reg_lockname, O_RDWR | O_CREAT | O_EXCL, 0600);
    
    if (lockfd == -1) {
      return LAMERROR;
    }
  }

  /* Alternative locking method using link() and stat().  Ideas behind
     this came from the linux open(2) man page.  Make a new file that
     is unique to this process (remember that it's already in a
     directory that is specific to this host -- so using a PID in the
     filename is sufficient to guarantee uniqueness), and try to hard
     link it to the well-known lock name.  If the hard link succeeds,
     or if the link count from a stat() of the unique file goes to 2,
     we have successfully locked the registry.  Note that NFS can
     sometimes incorrectly report a failure of link() -- this is why
     we check stat() as well. */

  else {
    lockfd = open(reg_mylockname, O_RDWR | O_CREAT, 0600);
    
    if (lockfd == -1 && errno == EEXIST)
      lockfd = open(reg_mylockname, O_RDWR, 0600);

    if (lockfd == -1)
      return LAMERROR;

    link(reg_mylockname, reg_lockname);
    if (stat(reg_mylockname, &statbuf) == -1) {
      close(lockfd);
      unlink(reg_mylockname);
      return LAMERROR;
    }

    if (statbuf.st_nlink != 2) {
      close(lockfd);
      unlink(reg_mylockname);
      return LAMERROR;
    }
  }  

  if (filelock(lockfd, F_SETLKW, F_WRLCK,
	       (off_t) 0, SEEK_SET, (off_t) 0) == -1) {
    close(lockfd);
    lockfd = -1;
    if (oexcl_works == 0)
      unlink(reg_mylockname);
    unlink(reg_lockname);
    return LAMERROR;
  }
  
  return 0;
}


/*
 *	unlock_registry
 *
 *	Function:	- remove our lock on the registry
 */
static void
unlock_registry(void)
{
  if (lockfd != -1)
    close(lockfd);
  lockfd = -1;

  if (oexcl_works == 0)
    unlink(reg_mylockname);
  unlink(reg_lockname);
}


/*
 *	filelock
 *
 *	Function:	- lock file via fcntl
 *	Accepts:	- file descriptor to lock
 *			- fcntl locking cmd
 *			- type of lock
 *			- relative offset of lock
 *			- starting offset of lock
 *			- length of lock
 *	Returns:	- fcntl return value
 */
static int
filelock(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
	struct flock	lock;

	lock.l_type = type;
	lock.l_start = offset;
	lock.l_whence = whence;
	lock.l_len = len;

	return(fcntl(fd, cmd, &lock));
}


/*
 *	free_filenames
 *
 *	Function:	- free the filenames allocated by the session 
 *                        directory API
 *	Accepts:	- void
 *	Returns:	- void
 */
static void
free_filenames(void)
{

    if (reg_fname) {
	free(reg_fname);
	reg_fname = 0;
    }
    if (reg_lockname) {
	free(reg_lockname);
	reg_lockname = 0;
    }
    if (reg_mylockname) {
	free(reg_mylockname);
	reg_mylockname = 0;
    }
    if (reg_tmpname) {
	free(reg_tmpname);
	reg_tmpname = 0;
    }
}
