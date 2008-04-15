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
 * $Id: ssi_rpi_sysv_shm.c,v 1.3 2003/04/03 23:49:16 jsquyres Exp $
 *
 *	Function:	- sysv transport specific low-level routines
 */

#include <lam_config.h>

#include <assert.h>
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

#include <rpi_sysv.h>
#include <sysv_common.h>


/*
 * private variables
 *
 * Per connection semaphores 0 and 1 are used for cleanup control.
 * Semaphores 2, 3, 4 and 5 are used for transfer buffer locking as follows.
 *
 * 2 - server write lock, client read unlock
 * 3 - client read lock, server write unlock
 * 4 - client write lock, server read unlock
 * 5 - server read lock, client write unlock
 */
static struct sembuf op_lock[5] = {
  {3, -1, 0},
  {4, -1, 0},
  {5, -1, 0},
  {2, -1, 0},
  {0, -1, 0}
};

static struct sembuf op_trylock[4] = {
  {3, -1, IPC_NOWAIT},
  {4, -1, IPC_NOWAIT},
  {5, -1, IPC_NOWAIT},
  {2, -1, IPC_NOWAIT},
};


static struct sembuf op_unlock[5] = {
  {2, 1, 0},
  {5, 1, 0},
  {4, 1, 0},
  {3, 1, 0},
  {0, 1, 0},
};

/*
 * global semaphores
 */
static struct sembuf exit_lock = { 0, -1, 0 };
static struct sembuf exit_unlock = { 0, 1, 0 };
static struct sembuf exit_op = { 1, -1, IPC_NOWAIT };

static struct sembuf sop;

static union semun semctl_arg;


/*
 *	_shm_clientinit
 *
 *	Function:	- client side shared memory initialization
 *	Accepts:	- process
 *			- LAM msg containing server info
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_sysv_clientinit(struct lam_ssi_rpi_proc *ps, struct nmsg *msg)
{
  ps->cp_ppsem = (int) msg->nh_data[3];
/*
 * Initialize the semaphore operations.
 */
  ps->cp_lop = op_lock;
  ps->cp_top = op_trylock;
  ps->cp_uop = op_unlock;

  return (0);
}


/*
 *	_shm_serverinit
 *
 *	Function:	- server side shared memory initialization
 *	Accepts:	- process
 *			- LAM msg to fill with info for client
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_sysv_serverinit(struct lam_ssi_rpi_proc *ps, struct nmsg *msg)
{
  char objs[2][32];
  int semid;
/*
 * Initialize the semaphore operations.
 */
  ps->cp_lop = op_lock + 2;
  ps->cp_top = op_trylock + 2;
  ps->cp_uop = op_unlock + 2;
/*
 * Create the semaphores.
 */
  if ((semid = semget(IPC_PRIVATE, 6, 0600 | IPC_CREAT)) < 0) {
    errno = ESEMCREATE;
    return (LAMERROR);
  }
/*
 * Register id's for cleanup.
 */
  sprintf(objs[1], "%d", semid);
  lam_register_objects(1, 's', objs[1]);
/*
 * Initilize semaphores.
 */
  ps->cp_ppsem = semid;

  semctl_arg.val = 1;
  if (semctl(semid, 0, SETVAL, LAM_SEMUN_VAL(semctl_arg)) < 0)
    return (LAMERROR);
  semctl_arg.val = 2;
  if (semctl(semid, 1, SETVAL, LAM_SEMUN_VAL(semctl_arg)) < 0)
    return (LAMERROR);

  semctl_arg.val = 1;
  if (semctl(semid, 2, SETVAL, LAM_SEMUN_VAL(semctl_arg)) < 0)
    return (LAMERROR);
  semctl_arg.val = 0;
  if (semctl(semid, 3, SETVAL, LAM_SEMUN_VAL(semctl_arg)) < 0)
    return (LAMERROR);

  semctl_arg.val = 1;
  if (semctl(semid, 4, SETVAL, LAM_SEMUN_VAL(semctl_arg)) < 0)
    return (LAMERROR);
  semctl_arg.val = 0;
  if (semctl(semid, 5, SETVAL, LAM_SEMUN_VAL(semctl_arg)) < 0)
    return (LAMERROR);
/*
 * Set information to pass to client.
 */
  msg->nh_data[3] = (int4) semid;

  return (0);
}


/*
 *	lam_ssi_rpi_sysv_cleanup
 *
 *	Function:	- clean up a process's per conection shared memory
 *			  structures
 *	Accepts:	- process
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_sysv_cleanup(struct lam_ssi_rpi_proc *ps)
{
  char obj[32];
  int val;			/* cleanup counter */
/*
 * Check if the other side of the connection has cleaned up.
 */
  sop.sem_num = 1;
  sop.sem_op = -1;
  sop.sem_flg = IPC_NOWAIT;
  semctl_arg.val = 0;

  if (semop(ps->cp_ppsem, op_lock + 4, 1) < 0)
    return (LAMERROR);
  if (semop(ps->cp_ppsem, &sop, 1) < 0)
    return (LAMERROR);

  val = semctl(ps->cp_ppsem, 1, GETVAL, semctl_arg);
  if (val < 0)
    return (LAMERROR);

  if (semop(ps->cp_ppsem, op_unlock + 4, 1) < 0)
    return (LAMERROR);

  if (val == 0) {
/*
 * The other side of the shared area has already cleaned up.
 * Delete the per process pair semaphores and deregister.
 */
    semctl(ps->cp_ppsem, 0, IPC_RMID, semctl_arg);

    sprintf(obj, "%d", ps->cp_ppsem);
    lam_deregister_object('s', obj);
  }

  return (0);
}


/*
 *	lam_ssi_rpi_sysv_global_cleanup
 *
 *	Function:	- clean up a process's global shared memory structures
 *	Accepts:	- process
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_sysv_global_cleanup(struct lam_ssi_rpi_proc *ps)
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

  shmdt(lam_ssi_rpi_sysv_membase);

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


/*
 *	lam_ssi_rpi_sysv_readlock
 *
 *	Function:	- obtain a lock for reading from a process
 *	Accepts:	- process to read from
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_sysv_readlock(struct lam_ssi_rpi_proc *p)
{
  do {
    if (semop((p)->cp_ppsem, (p)->cp_lop, 1) == 0)
      return (0);
    else if (errno != EINTR)
      return (LAMERROR);
  } while (1);
}


/*
 *	lam_ssi_rpi_sysv_writelock
 *
 *	Function:	- obtain a lock for writing to a process
 *	Accepts:	- process to write to
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_sysv_writelock(struct lam_ssi_rpi_proc *p)
{
  do {
    if (semop((p)->cp_ppsem, (p)->cp_lop + 1, 1) == 0)
      return (0);
    else if (errno != EINTR)
      return (LAMERROR);
  } while (1);
}
