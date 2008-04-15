#define LAM_MPI_EXTRA 0
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
 * $Id: mpietc.c,v 6.9 2003/08/22 02:33:18 brbarret Exp $
 *
 *	Function:	- misc. shared MPI-related functions
 *			- used by libmpi and MPI tools
 */

#include <app_mgmt.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>

#if LAM_MPI_EXTRA
#define lam_pt2coll lam_mpi_pt2coll
#define lam_coll2pt lam_mpi_coll2pt
#define lam_pt2impicoll lam_mpi_pt2impicoll
#define lam_impicoll2pt lam_mpi_impicoll2pt
#define lam_pt2impidatasync lam_mpi_pt2impidatasync
#define lam_pt2impisyncack lam_mpi_pt2impisyncack
#define lam_mkcoll lam_mpi_mkcoll
#define lam_mkpt lam_mpi_mkpt
#define lam_mkimpicoll lam_mpi_mkimpicoll
#define lam_unsyncsql lam_mpi_unsyncsql
#define lam_rank_in_group lam_mpi_rank_in_group
#endif

/*
 * macros
 */
#define FLIPCID(x)	(-((x) + 1))

/*
 *	lam_pt2coll
 *
 *	Function:	- change context ID from pt2pt to collective
 *	Accepts:	- pt2pt context ID
 *	Returns:	- collective context ID
 */
int
lam_pt2coll(int cid)
{
  if (cid >= 0) 
    cid = FLIPCID(cid);
  return(cid);
}

/*
 *	lam_coll2pt
 *
 *	Function:	- change context ID from collective to pt2pt
 *	Accepts:	- collective context ID
 *	Returns:	- pt2pt context ID
 */
int
lam_coll2pt(int cid)
{
  if (cid < 0) 
    cid = FLIPCID(cid);
  return(cid);
}

#if LAM_WANT_IMPI
/*
 *	lam_pt2impicoll
 *
 *	Function:	- change context ID from pt2pt to impi collective
 *	Accepts:	- pt2pt context ID
 *	Returns:	- impi collective context ID
 */
int
lam_pt2impicoll(int cid)
{
  /* Per IMPI section 4.3, the context ID for IMPI collectives is the
      pt2pt ID + 1 */

  if (cid >= 0 && cid % 3 == 0)
    cid++;
  return(cid);
}

/*
 *	lam_impicoll2pt
 *
 *	Function:	- change context ID from impi collective to pt2pt
 *	Accepts:	- impi collective context ID
 *	Returns:	- pt2pt context ID
 */
int
lam_impicoll2pt(int cid)
{
  /* Per IMPI section 4.3, the context ID for IMPI collectives is the
      pt2pt ID + 1 */

  if (cid > 0 && cid % 3 == 1)
    cid--;
  return(cid);
}


/*
 *	lam_pt2impidatasync
 *
 *	Function:	- change context ID from pt2pt to impi datasync
 *	Accepts:	- pt2pt context ID
 *	Returns:	- impi datasync context ID
 */
int
lam_pt2impidatasync(int cid)
{
  if (cid >= 0 && cid % 3 == 0)
    cid += 2;

  return(cid);
}


/*
 *	lam_pt2impidatasync
 *
 *	Function:	- change context ID from pt2pt to impi syncack
 *	Accepts:	- pt2pt context ID
 *	Returns:	- impi syncack context ID
 */
int
lam_pt2impisyncack(int cid)
{
  if (cid >= 0 && cid % 3 == 0) {
    cid += 2;
    cid = FLIPCID(cid);
  }

  return(cid);
}


#endif

/*
 *	lam_mkcoll
 *
 *	Function:	- switch communicator to collective context ID
 *	Accepts:	- communicator
 */
void
lam_mkcoll(MPI_Comm comm)
{
  if (comm) 
    comm->c_contextid = lam_pt2coll(comm->c_contextid);
}

/*
 *	lam_mkpt
 *
 *	Function:	- switch communicator to pt2pt context ID
 *	Accepts:	- communicator
 */
void
lam_mkpt(MPI_Comm comm)
{
  /* To preserve naming schemes, we have to do a little extra work in
     the IMPI case -- figure out which collective context ID scheme
     was used, and then do the opposite of it to return it to pt2pt */

  if (comm) {
#if LAM_WANT_IMPI
    if (comm->c_shadow != NULL)
      comm->c_contextid = lam_impicoll2pt(comm->c_contextid);
    else
      comm->c_contextid = lam_coll2pt(comm->c_contextid);
#else
    comm->c_contextid = lam_coll2pt(comm->c_contextid);
#endif
  }
}

#if LAM_WANT_IMPI
/*
 *	lam_mkimpicoll
 *
 *	Function:	- switch communicator to impi collective context ID
 *	Accepts:	- communicator
 */
void
lam_mkimpicoll(MPI_Comm comm)
{
  if (comm) 
    comm->c_contextid = lam_pt2impicoll(comm->c_contextid);
}
#endif

/*
 *	lam_unsyncsql
 *
 *	Function:	- extract SQL sync. information
 *	Accepts:	- event
 *			- type
 *			- tag (out)
 *			- context ID (out)
 *			- ACK flag (out)
 *			- destination index (out)
 *			- source node (out)
 *			- source index (out)
 */
void
lam_unsyncsql(int event, int type, int *ptag, int *pcid, int *pack,
	      int *pdidx, int *psnode, int *psidx)
{
  *pdidx = event & 0x7FFF;
  *pack = event & 0x10000;
  *pcid = (event >> 18) & 0xFFF;
  if (*pcid & 0x800) 
    *pcid |= 0xFFFFF000;
  
  *ptag = type & 0xFFFF;
  if (*ptag & 0x8000) 
    *ptag |= 0xFFFF0000;
  
  type >>= 16;
  *psnode = type & 0x3FF;
  *psidx = (type >> 10) & 0x3F;
  
  if (*psnode == 0x3FF) 
    *psnode = MPI_ANY_SOURCE;
  if (*psidx == 0x3F) 
    *psidx = MPI_ANY_SOURCE;
}

/*
 *	lam_rank_in_group
 *
 *	Function:	- find rank of process in a group
 *	Accepts:	- group
 *			- process
 *	Returns:	- rank in group else MPI_UNDEFINED
 */
int
lam_rank_in_group(MPI_Group group, struct _proc *proc)
{
  int		i;
  struct _gps	*gps;
  
  gps = &proc->p_gps;
  
  for (i = 0; i < group->g_nprocs; i++) {
    if (LAM_FULL_GPSCMP(&group->g_procs[i]->p_gps, gps) == 0) {
      return(i);
    }
  }
  
  return(MPI_UNDEFINED);
}
