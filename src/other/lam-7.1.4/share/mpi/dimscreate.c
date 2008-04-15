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
 * $Id: dimscreate.c,v 6.6 2003/11/02 21:13:10 brbarret Exp $
 *
 *	Function:	- select "best" grid dimensions
 *			- "Greed is good, greed works!" (Gecko)
 *	Accepts:	- # of processes
 *			- # of dimensions
 *			- array of dimension sizes
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>
#include <typical.h>

/*
 * static functions
 */
static int		assignprocs(int ndim, int nfactor, int *pfacts, 
                                    int *counts, int **pdims);
static int		getfactors(int num, int nprime, int *primes, int **pcounts);
static int		getprimes(int num, int *pnprime, int **pprimes);

/*@

MPI_Dims_create - Creates a division of processors in a cartesian grid

Input Parameters:
+ nprocs - number of nodes in a grid (integer) 
- ndims - number of cartesian dimensions (integer) 

In/Out Parameter:   
. dims - integer array of size 'ndims' specifying the number of nodes
in each dimension

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_DIMS
.N MPI_ERR_ARG
.N MPI_ERR_EXHAUSTED

.N ACK
@*/
int MPI_Dims_create(int nprocs, int ndims, int *dims)
{
	int		i;
	int		freeprocs;
	int		freedims;
	int		nprimes;
	int		*primes;
	int		*factors;
	int		*procs;
	int		*p;

	lam_initerr();
	lam_setfunc(BLKMPIDIMSCREATE);
/*
 * Check the arguments.
 */
	if (ndims < 1) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIDIMSCREATE, lam_mkerr(MPI_ERR_DIMS, EINVAL)));
	}

	if ((nprocs <= 0) || (dims == 0)) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIDIMSCREATE, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Get # of free-to-be-assigned processes and # of free dimensions.
 */
	freeprocs = nprocs;
	freedims = 0;
	for (i = 0, p = dims; i < ndims; ++i,++p) {
		if (*p == 0) {
			++freedims;
		} else if ((*p < 0) || ((nprocs % *p) != 0)) {
			return(lam_errfunc(MPI_COMM_WORLD, BLKMPIDIMSCREATE,
					lam_mkerr(MPI_ERR_DIMS, EINVAL)));
		} else {
			freeprocs /= *p;
		}
	}

	if (freedims == 0) {
		if (freeprocs == 1) {
			lam_resetfunc(BLKMPIDIMSCREATE);
			return(MPI_SUCCESS);
		}

		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIDIMSCREATE,
				lam_mkerr(MPI_ERR_DIMS, EINVAL)));
	}

	if (freeprocs < 1) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIDIMSCREATE, lam_mkerr(MPI_ERR_DIMS, EINVAL)));
	}
	else if (freeprocs == 1) {
		for (i = 0; i < ndims; ++i, ++dims) {
			if (*dims == 0) {
				*dims = 1;
			}
		}

		lam_resetfunc(BLKMPIDIMSCREATE);
		return(MPI_SUCCESS);
	}
/*
 * Compute the relevant prime numbers for factoring.
 */
	if (getprimes(freeprocs, &nprimes, &primes)) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIDIMSCREATE, lam_mkerr(MPI_ERR_OTHER, errno)));
	}
/*
 * Factor the number of free processes.
 */
	if (getfactors(freeprocs, nprimes, primes, &factors)) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIDIMSCREATE, lam_mkerr(MPI_ERR_OTHER, errno)));
	}
/*
 * Assign free processes to free dimensions.
 */
	if (assignprocs(freedims, nprimes, primes, factors, &procs)) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIDIMSCREATE, lam_mkerr(MPI_ERR_OTHER, errno)));
	}
/*
 * Return assignment results.
 */
	p = procs;
	for (i = 0; i < ndims; ++i, ++dims) {
		if (*dims == 0) {
			*dims = *p++;
		}
	}

	free((char *) primes);
	free((char *) factors);
	free((char *) procs);

	lam_resetfunc(BLKMPIDIMSCREATE);
	return(MPI_SUCCESS);
}

/*
 *	assignprocs
 *
 *	Function:	- assign processes to dimensions
 *			- get "best-balanced" grid
 *			- greedy bin-packing algorithm used
 *			- sort dimensions in decreasing order
 *			- dimensions array dynamically allocated
 *	Accepts:	- # of dimensions
 *			- # of prime factors
 *			- array of prime factors
 *			- array of factor counts
 *			- ptr to array of dimensions (returned value)
 *	Returns:	- 0 or ERROR
 */
static int
assignprocs(int ndim, int nfactor, int *pfacts, int *counts, int **pdims)
{
	int		*bins;
	int		i, j;
	int		n;
	int		f;
	int		*p;
	int		*pmin;

	if (ndim <= 0) {
		errno = EINVAL;
		return(ERROR);
	}
/*
 * Allocate and initialize the bins.
 */
	bins = (int *) malloc((unsigned) ndim * sizeof(int));
	if (bins == 0) {
		return(ERROR);
	}
	*pdims = bins;

	for (i = 0, p = bins; i < ndim; ++i, ++p) {
		*p = 1;
	}
/*
 * Loop assigning factors from the highest to the lowest.
 */
	for (j = nfactor - 1; j >= 0; --j) {

		f = pfacts[j];
		for (n = counts[j]; n > 0; --n) {
/*
 * Assign a factor to the smallest bin.
 */
			pmin = bins;
			for (i = 1, p = pmin + 1; i < ndim; ++i, ++p) {
				if (*p < *pmin) {
					pmin = p;
				}
			}

			*pmin *= f;
		}
	}
/*
 * Sort dimensions in decreasing order (O(n^2) for now).
 */
	for (i = 0, pmin = bins; i < ndim - 1; ++i, ++pmin) {
		for (j = i + 1, p = pmin + 1; j < ndim; ++j, ++p) {
			if (*p > *pmin) {
				n = *p;
				*p = *pmin;
				*pmin = n;
			}
		}
	}

	return(0);
}

/*
 *	getfactors
 *
 *	Function:	- factorize a number
 *	Accepts:	- number
 *			- # of primes
 *			- array of primes
 *			- ptr to array of counts (returned value)
 *	Returns:	- 0 or ERROR
 */
static int
getfactors(int num, int nprime, int *primes, int **pcounts)
{
	int		*counts;
	int		i;
	int		*p;
	int		*c;

	if (nprime <= 0) {
		errno = EINVAL;
		return(ERROR);
	}
/*
 * Allocate the factor counts array.
 */
	counts = (int *) malloc((unsigned) nprime * sizeof(int));
	if (counts == 0) {
		return(ERROR);
	}
	*pcounts = counts;
/*
 * Loop over the prime numbers.
 */
	i = nprime - 1;
	p = primes + i;
	c = counts + i;

	for (; i >= 0; --i, --p, --c) {
		*c = 0;
		while ((num % *p) == 0) {
			++(*c);
			num /= *p;
		}
	}

	if (num != 1) {
		errno = EIMPOSSIBLE;
		return(ERROR);
	}

	return(0);
}

/*
 *	getprimes
 *
 *	Function:	- get primes smaller than number
 *			- array of primes dynamically allocated
 *	Accepts:	- number
 *			- ptr # of primes (returned value)
 *			- ptr array of primes (returned values)
 *	Returns:	- 0 or ERROR
 */
static int
getprimes(int num, int *pnprime, int **pprimes)
{
	int		i, j;
	int		n;
	int		size;
	int		*primes;
/*
 * Allocate the array of primes.
 */
	size = (num / 2) + 1;
	primes = (int *) malloc((unsigned) size * sizeof(int));
	if (primes == 0) {
		return(ERROR);
	}
	*pprimes = primes;
/*
 * Find the prime numbers.
 */
	i = 0;
	primes[i++] = 2;

	for (n = 3; n <= num; n += 2) {
		for (j = 1; j < i; ++j) {
			if ((n % primes[j]) == 0) {
				break;
			}
		}

		if (j == i) {
			if (i >= size) {
				errno = EIMPOSSIBLE;
				return(ERROR);
			}

			primes[i++] = n;
		}
	}

	*pnprime = i;
	return(0);
}
