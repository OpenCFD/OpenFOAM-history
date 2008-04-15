/*
 * Copyright (c) 2001-2006 The Trustees of Indiana University.  
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
 * $Id: lamprocs.c,v 6.17 2003/04/28 21:48:39 jsquyres Exp $
 *
 *	Function:	- manage MPI processes information
 */

#include <lam_config.h>

#include <stdlib.h>

#include <all_list.h>
#include <app_mgmt.h>
#include <mpi.h>
#include <mpisys.h>
#include <lam-ssi-rpi.h>
#include <rpisys.h>
#include <typical.h>


/*
 * global variables
 */
struct _proc **lam_procs = 0;
int lam_num_procs = 0;


/*
 * static variables
 */
static LIST		*plist = 0;		/* list of processes */
static struct _proc	*curproc = 0;		/* current process iterator */
static int              max_procs = 0;
static const int        init_procs = 16;


/*
 * private helper functions
 */
static int cache_procs(void);
static int expand_cache(void);
static int qsort_compare(const void *a, const void *b);


/*
 *	lam_proccmp
 *
 *	Function:	- compare two process entries
 *	Accepts:	- ptr 2 process entries
 *	Returns:	- -1/0/+1 comparison
 */
int
lam_proccmp(struct _proc *p1, struct _proc *p2)
{
	return(LAM_FULL_GPSCMP(&(p1->p_gps), &(p2->p_gps)));
}


/*
 *	lam_procsort
 *
 *	Function:	- sort an array of pointers to procs
 *	Accepts:	- array of proc pointers, length of array
 *	Returns:	- Nothing
 */
void
lam_procsort(struct _proc **procs, int nprocs)
{
  qsort(procs, nprocs, sizeof(struct _proc *), qsort_compare);
}


/*
 *	lam_procadd
 *
 *	Function:	- add process to the list if not already present
 *	Accepts:	- process GPS
 *	Returns:	- process or NULL
 */
struct _proc *
lam_procadd(struct _gps *gps)
{
	struct _proc	pelem;			/* process element */
	struct _proc	*p;			/* favourite pointer */
/*
 * Initialize the list if necessary.
 */
	if (plist == 0) {
		plist = al_init(sizeof(struct _proc), lam_proccmp);
		if (plist == 0) return(0);
	}
/*
 * Check if the process is in the list.
 */
	LAM_ZERO_ME(pelem);
	pelem.p_gps = *gps;
	p = al_find(plist, &pelem);

	if (p) {
		return(p);
	}
/*
 * The process is not present so initialize and insert it.  The reference count
 * will be incremented upon addition to a group.
 */
	pelem.p_mode = 0;
	pelem.p_ger_nsnd = 0;
	pelem.p_refcount = 0;
	pelem.p_rpi = 0;
	pelem.p_num_buf_env = 0;
	
	if ((p = al_insert(plist, &pelem)) == 0)
	  return NULL;

	if (cache_procs() != 0)
	  return NULL;

	return p;
}


/*
 *	lam_procfree
 *
 *	Function:	- free a process
 *	Accepts:	- process
 *	Returns:	- 0 or LAMERROR
 */
int
lam_procfree(struct _proc *p)
{
	if (RPI_FINALIZE_PROC(p)) {
		return(LAMERROR);
	}

	if (lam_procrm(p) != 0)
	  return LAMERROR;

	return cache_procs();
}


/*
 *	lam_procfind
 *
 *	Function:	- locate process in the list
 *	Accepts:	- process GPS
 *	Returns:	- process or NULL
 */
struct _proc *
lam_procfind(struct _gps *gps)
{
	struct _proc	pelem;			/* process element */
	struct _proc	*p = 0;

	if (plist) {
		pelem.p_gps = *gps;
		p = (struct _proc *) al_find(plist, &pelem);
	}

	return(p);
}


/*
 *	lam_procrm
 *
 *	Function:	- remove a process from the list
 *	Accepts:	- ptr to process
 *	Returns:	- 0 or LAMERROR
 */
int
lam_procrm(struct _proc *proc)
{
  if (plist != NULL && al_delete(plist, proc) != 0)
    return LAMERROR;

  return cache_procs();
}


/*
 *	lam_topproc
 *
 *	Function:	- reset process list iterator and get top element
 *	Returns:	- top process in list
 */
struct _proc *
lam_topproc(void)
{
	curproc = (plist) ? al_top(plist) : NULL;
	return(curproc);
}


/*
 *	lam_nextproc
 *
 *	Function:	- get next process in list
 *			- this should NOT be called if processes have been
 *			  removed from the list since
 *			  the last call to this function or lam_topproc
 *	Returns:	- next process in list or 0
 */
struct _proc *
lam_nextproc(void)
{
	if (curproc) curproc = al_next(plist, curproc);

	return(curproc);
}


/*
 *	lam_nprocs
 *
 *	Function:	- size of process list
 *	Returns:	- number of processes in process list
 */
int
lam_nprocs(void)
{
	return ((plist) ? al_count(plist) : 0);
}


/*
 *	lam_nukeprocs
 *
 *	Function:	- delete the process list
 */
void
lam_nukeprocs(void)
{
	LIST		*tmp;

	tmp = plist;
/*
 * The process list is set to null and we free the temporary to avoid a
 * nasty recursion when a signal occurs in al_free().  
 */
	plist = 0;
	curproc = 0;

	if (tmp) {
		(void) al_free(tmp);
	}
	if (lam_procs != NULL)
	  free(lam_procs);
	lam_num_procs = 0;
}


/*
 * Private helper function to fill in the lam_procs cache array from
 * the real list
 */
static int 
cache_procs(void)
{
  struct _proc *p, *save;
  int max = lam_nprocs();

  /* Ensure that the cache array is large enough */

  while (max > max_procs)
    if (expand_cache() != 0)
      return LAMERROR;

  /* Now fill it.  Save curproc before calling topproc()/nextproc() in
     case some outter function is in the middle of iterating over
     nextproc(). */

  save = curproc;
  for (lam_num_procs = 0, p = lam_topproc(); p != NULL; 
       ++lam_num_procs, p = lam_nextproc())
    lam_procs[lam_num_procs] = p;
  curproc = save;

  return 0;
}


/*
 * Private helper function to increase the storage of the lam_procs
 * array.
 */
static int 
expand_cache(void)
{
#if LAM_WANT_DEBUG
  int i;
#endif

  /* If this is the first time, alloc init_procs */

  if (max_procs == 0) {
    lam_procs = malloc(sizeof(struct _proc**) * init_procs);
    if (lam_procs == NULL)
      return LAMERROR;

#if LAM_WANT_DEBUG
    for (i = 0; i < init_procs; ++i)
      lam_procs[i] = NULL;
#endif
    max_procs = init_procs;
    lam_num_procs = 0;
  } 

  /* Otherwise, realloc more than we had before */

  else {
    max_procs += init_procs;
    lam_procs = realloc(lam_procs, sizeof(struct _proc**) * max_procs);
    if (lam_procs == NULL)
      return LAMERROR;

#if LAM_WANT_DEBUG
    for (i = lam_num_procs; i < max_procs; ++i)
      lam_procs[i] = NULL;
#endif
  }

  return 0;
}


/*
 *	qsort_compare
 *
 *	Function:	- compare two process entries
 *	Accepts:	- ptr 2 process entries
 *	Returns:	- -1/0/+1 comparison
 */
static int
qsort_compare(const void *a, const void *b)
{
  /* This is confusing enough -- it's not worth the extra optimization
     to try to put this all inline in a single LAM_FULL_GPSCMP
     statement and avoid the extra temporaries.  Keep them as separate
     temp variables so that you can have a hope of understanding the
     typecasting that's going on here. */

  const struct _proc **aa = (const struct _proc **) a;
  const struct _proc **bb = (const struct _proc **) b;

  return(LAM_FULL_GPSCMP(&(aa[0]->p_gps), 
			 &(bb[0]->p_gps)));
}
