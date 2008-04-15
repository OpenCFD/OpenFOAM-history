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
 * $Id: ssi_rpi_usysv_shm.c,v 1.3 2003/04/03 23:50:58 jsquyres Exp $
 *
 *	Function:	- usysv transport low-level routines
 */

#include <lam_config.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <terror.h>
#include <typical.h>
#include <t_types.h>
#include <laminternal.h>

#include <rpi_usysv.h>
#include <usysv_common.h>


/*
 * # loops before usleep()
 */
int lam_ssi_rpi_usysv_lock_poll_read = READPOLLCOUNT;
int lam_ssi_rpi_usysv_lock_poll_write = WRITEPOLLCOUNT;


/*
 * private variables
 */
static struct sembuf exit_lock = { 0, -1, 0 };
static struct sembuf exit_unlock = { 0, 1, 0 };
static struct sembuf exit_op = { 1, -1, IPC_NOWAIT };


static union semun semctl_arg;

/*
 *	lam_ssi_rpi_usysv_cleanup
 *
 *	Function:	- clean up a process's shared memory structures
 *	Accepts:	- process
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_usysv_cleanup(struct lam_ssi_rpi_proc *ps)
{
  char obj[32];
  int val;			/* cleanup counter */

  if (semop(ps->cp_sem, &exit_lock, 1) < 0)
    return (LAMERROR);
  if (semop(ps->cp_sem, &exit_op, 1) < 0)
    return (LAMERROR);

  semctl_arg.val = 0;
  val = semctl(ps->cp_sem, 1, GETVAL, semctl_arg);
  if (val < 0)
    return (LAMERROR);

  shmdt(lam_ssi_rpi_usysv_membase);

  if (semop(ps->cp_sem, &exit_unlock, 1) < 0)
    return (LAMERROR);

  if (val == 0) {
/*
 * The other side of the shared area has already cleaned up so
 * we can delete the semaphores and deregister the shared structures.
 */
    semctl(ps->cp_sem, 0, IPC_RMID, semctl_arg);
    shmctl(ps->cp_shm, IPC_RMID, (struct shmid_ds *) 0);

    sprintf(obj, "%d", ps->cp_sem);
    lam_deregister_object('s', obj);
    sprintf(obj, "%d", ps->cp_shm);
    lam_deregister_object('m', obj);
  }

  return (0);
}
