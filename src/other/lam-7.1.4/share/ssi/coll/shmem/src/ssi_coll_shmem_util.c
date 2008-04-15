/*
 * Copyright (c) 2001-2004 The Trustees of Indiana University.  
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
 * $Id: ssi_coll_shmem_util.c,v 1.3 2004/03/09 16:04:52 adharurk Exp $
 *
 *	Function:	- shmem collective routines
 */

#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

#include <lam_config.h>
#if LAM_WANT_PROFILE
#define LAM_PROFILELIB 1
#endif
#include <lam-ssi-coll-shmem-config.h>

#include <lam-ssi-coll.h>
#include <lam-ssi-coll-shmem.h>
#include <mpisys.h>
#include <rpisys.h>

#include <args.h>
#include <dl_inet.h>
#include <laminternal.h>
#include <net.h>
#include <rreq.h>
#include <typical.h>
#include <t_types.h>
#include <terror.h>
#include <etc_misc.h>

/* Handling the shared memory -- START */

#if LAM_HAVE_UNION_SEMUN
typedef union semun lam_semun_t;
#else
typedef union {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
} lam_semun_t;
#endif

static struct sembuf decrement_op = { 0, -1, IPC_NOWAIT };
static struct sembuf shm_lock = { 1, -1, 0 };
static struct sembuf shm_unlock = { 1, 1, 0 };

static void shmlock(int);
static void shmunlock(int);

/* Creates a shared memory area and returns the id to calling process */
int
lam_ssi_coll_shmem_create_area(int shmsize, 
			       char** lam_devices_shmem_membase_ptr,
			       int* shmem_id, int* sem_id, int num_procs)
{
  int semid, shmid;
  char objs[2][32];
  lam_semun_t semctl_arg;

  /* Create the shared memory segment. Get the shared semaphores too */

  shmid = shmget(IPC_PRIVATE, shmsize, 0600 | IPC_CREAT);
  if (shmid < 0) {
    errno = ESHMCREATE; 
    return (LAMERROR);
  }

  *lam_devices_shmem_membase_ptr = (char *) shmat(shmid, (char *) 0, 0);
  if (*lam_devices_shmem_membase_ptr == (char *) -1) {
    shmctl(shmid, IPC_RMID, (struct shmid_ds *) 0);
    return (LAMERROR);
  }

  /* Zero out the memory */
  memset(*lam_devices_shmem_membase_ptr, -1, shmsize);

  /*
   * Create the semaphores.
   */
  if ((semid = semget(IPC_PRIVATE, 2, 0600 | IPC_CREAT)) < 0) {
    shmctl(shmid, IPC_RMID, (struct shmid_ds *) 0);
    errno = ESEMCREATE;
    return (LAMERROR);
  }

  /*
   * Register id's for cleanup.
   */
  snprintf(objs[0], sizeof(objs[0]), "%d", shmid);
  objs[0][sizeof(objs[0]) - 1] = '\0';
  snprintf(objs[1], sizeof(objs[1]), "%d", semid);
  objs[1][sizeof(objs[1]) - 1] = '\0';
  lam_register_objects(2, 'm', objs[0], 's', objs[1]);

  semctl_arg.val = num_procs;
  if (semctl(semid, 0, SETVAL, LAM_SEMUN_VAL(semctl_arg)) < 0)
    return (LAMERROR);
  semctl_arg.val = 1;
  if (semctl(semid, 1, SETVAL, LAM_SEMUN_VAL(semctl_arg)) < 0)
    return (LAMERROR);

  /* Pass shared memory id and semaphore id to the calling process */

  *shmem_id = shmid;
  *sem_id = semid;

  return (0);
}

/* Attaches the shared memory area to the calling process space */
int
lam_ssi_coll_shmem_attach_area(int shmem_id, int sem_id,
			       char** lam_devices_shmem_membase_ptr)
{
  lam_semun_t semctl_arg;

  *lam_devices_shmem_membase_ptr = (char *) shmat(shmem_id, (char *) 0, 0);
  if (*lam_devices_shmem_membase_ptr == (char *) -1) {
    shmctl(shmem_id, IPC_RMID, (struct shmid_ds *) 0);
    semctl_arg.val = 0;
    semctl(shmem_id, 0, IPC_RMID, semctl_arg);
    return (LAMERROR);
  }
  return (0);
}

/* Frees the shared memory and semaphore pool. Whoever finishes last is 
   responsible for deleting the shared resources. */
int
lam_ssi_coll_shmem_free_area(int shmem_id, int sem_id, 
			     char* lam_devices_shmem_membase_ptr)
{
  char obj[32];
  lam_semun_t semctl_arg;

  /* decrement the semaphore. If it reaches to 0 then delete shared
     memory segment */
  shmlock(sem_id);

  if (semop(sem_id, &decrement_op, 1) < 0)
    return (LAMERROR);
  
  semctl_arg.val = 0;
  if (shmdt(lam_devices_shmem_membase_ptr) != 0)
    return LAMERROR;
  if (semctl(sem_id, 0, GETVAL, semctl_arg) == 0) { 
    shmunlock(sem_id);

    semctl_arg.val = 0;
    semctl(sem_id, 0, IPC_RMID, semctl_arg);
    shmctl(shmem_id, IPC_RMID, (struct shmid_ds *) 0);

    sprintf(obj, "%d", shmem_id);
    lam_deregister_object('s', obj);
    sprintf(obj, "%d", sem_id);
    lam_deregister_object('m', obj);
  } else {
    shmunlock(sem_id);
  }

  return 0;
}

/*
 *	shmlock
 *
 *	Function:	- lock the shared memory area for deletion
 */

static void
shmlock(int semaphores)
{
  do {
    if (semop(semaphores, &shm_lock, 1) == 0) {
      return;
    } else {
    }
  } while (1);
}


/*
 *	shmunlock
 *
 *	Function:	- unlock the shared memory area for deletion
 */
static void
shmunlock(int semaphores)
{
  semop(semaphores, &shm_unlock, 1);
}

/* Handling the shared memory -- END */


/* Queries lamd and gets number of processors that were specified in the
   hostfile.  Minumum of this number and actual number of available processors
   returned by sysconf is used */

int 
lam_ssi_coll_shmem_get_num_cpu() 
{
  int4 node;
  int n_index, n_flags, argc, cpus, onln_cpus;
  int *pnodes;
  char **argv1; 
  struct route r;
  static int4 nlinks;
  static struct dolink	*links;

  if (ldogetlinks(&links, &nlinks))
    return LAMERROR;

  pnodes = (int4 *) malloc((unsigned) (nlinks * sizeof(int4)));
  if (pnodes == 0) 
    return LAMERROR;
  
  if (getnodes(pnodes, nlinks, 0, NT_CAST)) 
    return LAMERROR;

  argc = 2;
  argv1 = (char**) malloc(2 * sizeof(char*));
  argv1[0] = (char*) malloc(2 * sizeof(char));
  strcpy(argv1[0],"a");
  argv1[1] = (char*) malloc(sizeof(char) * 2);
  strcpy(argv1[1],"h");
  if (nid_parse(&argc, argv1) != 0)
    return LAMERROR;

  nid_get(&n_index, &node, &n_flags);
  if (n_index != 0) {
    free(argv1[0]);
    free(argv1[1]);
    free(argv1);
    free(pnodes);
    free(links);
    nid_free();
    return LAMERROR;
  }
  else {
    if (node == LOCAL)
      node = getnodeid();
    r.r_nodeid = node;
    getrent(&r);
    cpus = r.r_ncpus;
  }
  if ((onln_cpus = lam_get_ncpus()) < cpus)
    cpus = onln_cpus;
  
  free(argv1[0]);
  free(argv1[1]);
  free(argv1);
  free(pnodes);
  free(links);
  nid_free();
  return cpus;
}

/* Returns the segment number (0 .. NUM_SEGMENTS-1) that will be used during
   the collective operation, at the same time increment the segment number */
int
lam_ssi_coll_shmem_increment_segment_in_use(lam_ssi_coll_data_t * lcd) {
  int seg;
  seg = lcd->lcd_segment_in_use;
  lcd->lcd_segment_in_use = (lcd->lcd_segment_in_use + 1) 
    % lcd->lcd_num_segments;
  return seg;
}


/* 
   The basic scheme used is as follows:
   Non-root processes set a flag corresponding to their rank.
   Root processes wait till all non-root processes arrive at barrier.
   Root then signals non-root processes.
   Non-root processes wait for the signal from the root.
   The segment_number indicates that the barrier is being done for that
   particular segment. 
   For performance:
   Different sets of flags are used for signalling root and non-root
   processes. This avoids memory contention a little when barrier is called
   frequently.
 */

/*
 * Accepts:                     - MPI_Comm 
 *                              - segment_number for which the barrier is 
 *                                being called
 *                              - rank of the process calling barrier
 * Returns:                     - MPI_SUCCESS or error code
 */

int 
lam_ssi_coll_shmem_comm_barrier(MPI_Comm comm, int segment_number,
				int myrank) 
{
  int i;
  long count;
  lam_ssi_coll_data_t *lcd = comm->c_ssi_coll_data;

  if (myrank == 0) {
    /* If I am root then wait till other process arrive at the barrier. If it
       is found that this is case of oversubscription (number of processes
       overrule the number of processors) then make use of lam_yield() so 
       that the process yields control if it is not doing any fruitful work
       and just spiining over a shared flag. */

    count = 0;
    while (1) {
      for (i = 1; i < lcd->lcd_num_procs; i++)
	if (lcd->lcd_barrier[segment_number][i * NUM_HOPS] != i)
	  break;
      count++;
      if (i != lcd->lcd_num_procs) {
	if ((count > 100) && (lcd->lcd_num_procs > lcd->lcd_num_cpus)) {
	  lam_yield();
	  count = 0;
	}
      } else 
	break;
    }

    /* Root signals non-root processes indicating that the root has aslo
       entered in the barrier */

    for (i = 1; i < lcd->lcd_num_procs; i++) {
      /* Set the flag back to its default value */
      lcd->lcd_barrier[segment_number][i * NUM_HOPS] = -1;

      lcd->lcd_collective_track[segment_number][i * NUM_HOPS] = 1;
    }
    
 
  } else {
    /* Non-root processes sets a flag indicating that they have arrived at
       the barrier */
    
    lcd->lcd_barrier[segment_number][myrank * NUM_HOPS] = myrank;
    count = 0;

    /* Wait till I get signal from the root */
    while (1) {
      if (lcd->lcd_collective_track[segment_number][myrank * NUM_HOPS] == 1)
	break;
      count++;
      if ((count > 100) && (lcd->lcd_num_procs > lcd->lcd_num_cpus)) {
	lam_yield();
	count = 0;
      } 
    }
    
    /* Set the flag back to its default value */
    lcd->lcd_collective_track[segment_number][myrank * NUM_HOPS] = -1;
    
  }
  
  return MPI_SUCCESS;
}

/* Similar to the comm_barrier except that it is used to signal non-root
   processes.  Root exits immediately after setting shared flags while 
   non-root processes wait till they get this signal from root.  This
   function is mainly used during rooted operations where root distributed the 
   data
*/

/*
 * Function:                    - signal_by_root
 * Accepts:                     - MPI_Comm 
 *                              - segment_number for which the barrier is 
 *                                being called
 *                              - root (rank of the root process)
 *                              - rank of the process calling barrier
 * Returns:                     - MPI_SUCCESS or error code
 */

int
lam_ssi_coll_shmem_signal_by_root(MPI_Comm comm, int segment_number, 
				  int root, int myrank) 
{
  int i, count;
  lam_ssi_coll_data_t *lcd = comm->c_ssi_coll_data;

  if (myrank == root) {
    for (i = 0; i < lcd->lcd_num_procs; i++) {
      if (i != root) 
	lcd->lcd_barrier[segment_number][i * NUM_HOPS] = 1;
    }
  } else {
    
    count = 0;
    while (1) {
      if (lcd->lcd_barrier[segment_number][myrank * NUM_HOPS] == 1)
	break;
      count++;
      if ((count > 100) && (lcd->lcd_num_procs > lcd->lcd_num_cpus)) {
	lam_yield();
	count = 0;
      } 
    }
    lcd->lcd_barrier[segment_number][myrank * NUM_HOPS] = -1;
    
  }
  
  return MPI_SUCCESS;
}

/*
  Similar to the comm_barrier except that root waits till it is signalled
  by all the non-root processes. Non root processes exit immediately once they
  set appropriate shared flag 
*/

/*
 * Function:                    - signal_by_non_roots
 * Accepts:                     - MPI_Comm 
 *                              - segment_number for which the barrier is 
 *                                being called
 *                              - root (rank of the root process)
 *                              - rank of the process calling barrier
 * Returns:                     - MPI_SUCCESS or error code
 */

int 
lam_ssi_coll_shmem_signal_by_non_roots(MPI_Comm comm, int segment_number,
				      int root, int myrank) 
{

  int i, count;
  lam_ssi_coll_data_t *lcd = comm->c_ssi_coll_data;

  if (myrank == root) {
    count = 0;
    while (1) {
      for (i = 0; i < lcd->lcd_num_procs; i++) {
	if (i != root) {
	  if (lcd->lcd_barrier[segment_number][i * NUM_HOPS] != 1)
	    break;
	}
      }
      count++;
      if (i != lcd->lcd_num_procs) {
	if ((count > 100) && (lcd->lcd_num_procs > lcd->lcd_num_cpus)) {
	  lam_yield();
	  count = 0;
	} 
      } else {
	break;
      }
    }
    for (i = 0; i < lcd->lcd_num_procs; ++i) 
      if (i != root) 
	lcd->lcd_barrier[segment_number][i * NUM_HOPS] = -1;
  } else {
    lcd->lcd_barrier[segment_number][myrank * NUM_HOPS] = 1;
  }
  
  return MPI_SUCCESS;
}
