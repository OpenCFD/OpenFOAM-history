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
 * $Id: lamreduce.c,v 6.14 2002/12/16 16:23:31 jsquyres Exp $
 *
 *	Function:	- all predefined reduce operations
 *			- min, max, sum, product
 *			- and, or, xor (bitwise and logical)
 *			- minloc, maxloc, replace
 */

#include <string.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <typical.h>

/*
 * local structures
 */
#if LAM_SIZEOF_LONG_DOUBLE
struct ldblcplex { long double r; long double i; };
#else
struct ldblcplex { double r; double i; };
#endif
struct dblcplex { double r; double i; };
struct complex { float r; float i; };
struct fltflt { float v; float k; };
struct fltint { float v; int k; };
struct dbldbl { double v; double k; };
struct dblint { double v; int k; };
struct intint { int v; int k; };
struct longint { long v; int k; };
struct shortint { short v; int k; };
#if LAM_SIZEOF_LONG_DOUBLE
struct ldblint { LAM_LONG_DOUBLE v; int k; };
#else
struct ldblint { double v; int k; };
#endif

void
lam_max(void *invec, void *inoutvec, int *len, MPI_Datatype *datatype)
{
	int		i;
	int		*piin, *piout;
	long		*plin, *plout;
	short		*psin, *psout;
	unsigned short	*pusin, *pusout;
	unsigned	*puin, *puout;
	unsigned long	*pulin, *pulout;
	float		*pfin, *pfout;
	double		*pdin, *pdout;
#if LAM_SIZEOF_LONG_DOUBLE
	LAM_LONG_DOUBLE	*pldin, *pldout;
#else
	double		*pldin, *pldout;
#endif
#if LAM_SIZEOF_LONG_LONG
	long long	*pllin, *pllout;
	unsigned long long *pullin, *pullout;
#else
	long		*pllin, *pllout;
	unsigned long 	*pullin, *pullout;
#endif
#if LAM_SIZEOF_FORTRAN_INTEGER1
        LAM_FORTRAN_C_TYPE_INTEGER1 *pfint1in, *pfint1out;
#endif
#if LAM_SIZEOF_FORTRAN_INTEGER2
        LAM_FORTRAN_C_TYPE_INTEGER2 *pfint2in, *pfint2out;
#endif
#if LAM_SIZEOF_FORTRAN_INTEGER4
        LAM_FORTRAN_C_TYPE_INTEGER4 *pfint4in, *pfint4out;
#endif
#if LAM_SIZEOF_FORTRAN_INTEGER8
        LAM_FORTRAN_C_TYPE_INTEGER8 *pfint8in, *pfint8out;
#endif
#if LAM_SIZEOF_FORTRAN_REAL4 
        LAM_FORTRAN_C_TYPE_REAL4 *pfr4in, *pfr4out;
#endif
#if LAM_SIZEOF_FORTRAN_REAL8 
        LAM_FORTRAN_C_TYPE_REAL8 *pfr8in, *pfr8out;
#endif
#if LAM_SIZEOF_FORTRAN_REAL16 
        LAM_FORTRAN_C_TYPE_REAL16 *pfr16in, *pfr16out;
#endif

	if (*datatype == MPI_INT || *datatype == MPI_F_INTEGER) {
		piin = (int *) invec;
		piout = (int *) inoutvec;
		for (i = 0; i < *len; ++i, ++piin, ++piout) {
			if (*piin > *piout) {
				*piout = *piin;
			}
		}
		return;
	}

	if (*datatype == MPI_LONG) {
		plin = (long *) invec;
		plout = (long *) inoutvec;
		for (i = 0; i < *len; ++i, ++plin, ++plout) {
			if (*plin > *plout) {
				*plout = *plin;
			}
		}
		return;
	}

	if (*datatype == MPI_SHORT) {
		psin = (short *) invec;
		psout = (short *) inoutvec;
		for (i = 0; i < *len; ++i, ++psin, ++psout) {
			if (*psin > *psout) {
				*psout = *psin;
			}
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_SHORT) {
		pusin = (unsigned short *) invec;
		pusout = (unsigned short *) inoutvec;
		for (i = 0; i < *len; ++i, ++pusin, ++pusout) {
			if (*pusin > *pusout) {
				*pusout = *pusin;
			}
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED) {
		puin = (unsigned *) invec;
		puout = (unsigned *) inoutvec;
		for (i = 0; i < *len; ++i, ++puin, ++puout) {
			if (*puin > *puout) {
				*puout = *puin;
			}
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_LONG) {
		pulin = (unsigned long *) invec;
		pulout = (unsigned long *) inoutvec;
		for (i = 0; i < *len; ++i, ++pulin, ++pulout) {
			if (*pulin > *pulout) {
				*pulout = *pulin;
			}
		}
		return;
	}

	if (*datatype == MPI_FLOAT || *datatype == MPI_F_REAL) {
		pfin = (float *) invec;
		pfout = (float *) inoutvec;
		for (i = 0; i < *len; ++i, ++pfin, ++pfout) {
			if (*pfin > *pfout) {
				*pfout = *pfin;
			}
		}
		return;
	}

	if (*datatype == MPI_DOUBLE || *datatype == MPI_F_DOUBLE_PRECISION) {
		pdin = (double *) invec;
		pdout = (double *) inoutvec;
		for (i = 0; i < *len; ++i, ++pdin, ++pdout) {
			if (*pdin > *pdout) {
				*pdout = *pdin;
			}
		}
		return;
	}

	if (*datatype == MPI_LONG_DOUBLE) {
#if LAM_SIZEOF_LONG_DOUBLE
		pldin = (LAM_LONG_DOUBLE *) invec;
		pldout = (LAM_LONG_DOUBLE *) inoutvec;
#else
		pldin = (double *) invec;
		pldout = (double *) inoutvec;
#endif
		for (i = 0; i < *len; ++i, ++pldin, ++pldout) {
			if (*pldin > *pldout) {
				*pldout = *pldin;
			}
		}
		return;
	}

	if (*datatype == MPI_LONG_LONG_INT) {
#if LAM_SIZEOF_LONG_LONG
		pllin = (long long *) invec;
		pllout = (long long *) inoutvec;
#else
		pllin = (long *) invec;
		pllout = (long *) inoutvec;
#endif
		for (i = 0; i < *len; ++i, ++pllin, ++pllout) {
			if (*pllin > *pllout) {
				*pllout = *pllin;
			}
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_LONG_LONG) {
#if LAM_SIZEOF_LONG_LONG
		pullin = (unsigned long long *) invec;
		pullout = (unsigned long long *) inoutvec;
#else
		pullin = (unsigned long *) invec;
		pullout = (unsigned long *) inoutvec;
#endif
		for (i = 0; i < *len; ++i, ++pullin, ++pullout) {
			if (*pullin > *pullout) {
				*pullout = *pullin;
			}
		}
		return;
	}

#if LAM_SIZEOF_FORTRAN_INTEGER1
        if (*datatype == MPI_F_INTEGER1) {
            pfint1in = (LAM_FORTRAN_C_TYPE_INTEGER1 *) invec;
            pfint1out = (LAM_FORTRAN_C_TYPE_INTEGER1 *) inoutvec;
            for (i = 0; i < *len; ++i, ++pfint1in, ++pfint1out) {
                if (*pfint1in > *pfint1out) {
                    *pfint1out = *pfint1in;
                }
            }
            return;
        }
#endif

#if LAM_SIZEOF_FORTRAN_INTEGER2
        if (*datatype == MPI_F_INTEGER2) {
            pfint2in = (LAM_FORTRAN_C_TYPE_INTEGER2 *) invec;
            pfint2out = (LAM_FORTRAN_C_TYPE_INTEGER2 *) inoutvec;
            for (i = 0; i < *len; ++i, ++pfint2in, ++pfint2out) {
                if (*pfint2in > *pfint2out) {
                    *pfint2out = *pfint2in;
                }
            }
            return;
        }
#endif

#if LAM_SIZEOF_FORTRAN_INTEGER4
        if (*datatype == MPI_F_INTEGER4) {
            pfint4in = (LAM_FORTRAN_C_TYPE_INTEGER4 *) invec;
            pfint4out = (LAM_FORTRAN_C_TYPE_INTEGER4 *) inoutvec;
            for (i = 0; i < *len; ++i, ++pfint4in, ++pfint4out) {
                if (*pfint4in > *pfint4out) {
                    *pfint4out = *pfint4in;
                }
            }
            return;
        }
#endif

#if LAM_SIZEOF_FORTRAN_INTEGER8
        if (*datatype == MPI_F_INTEGER8) {
            pfint8in = (LAM_FORTRAN_C_TYPE_INTEGER8 *) invec;
            pfint8out = (LAM_FORTRAN_C_TYPE_INTEGER8 *) inoutvec;
            for (i = 0; i < *len; ++i, ++pfint8in, ++pfint8out) {
                if (*pfint8in > *pfint8out) {
                    *pfint8out = *pfint8in;
                }
            }
            return;
        }
#endif

#if LAM_SIZEOF_FORTRAN_REAL4
	if (*datatype == MPI_F_REAL4) {
		pfr4in = (LAM_FORTRAN_C_TYPE_REAL4 *) invec;
		pfr4out = (LAM_FORTRAN_C_TYPE_REAL4 *) inoutvec;
		for (i = 0; i < *len; ++i, ++pfr4in, ++pfr4out) {
			if (*pfr4in > *pfr4out) {
				*pfr4out = *pfr4in;
			}
		}
		return;
	}
#endif

#if LAM_SIZEOF_FORTRAN_REAL8
	if (*datatype == MPI_F_REAL8) {
		pfr8in = (LAM_FORTRAN_C_TYPE_REAL8 *) invec;
		pfr8out = (LAM_FORTRAN_C_TYPE_REAL8 *) inoutvec;
		for (i = 0; i < *len; ++i, ++pfr8in, ++pfr8out) {
			if (*pfr8in > *pfr8out) {
				*pfr8out = *pfr8in;
			}
		}
		return;
	}
#endif

#if LAM_SIZEOF_FORTRAN_REAL16
	if (*datatype == MPI_F_REAL16) {
		pfr16in = (LAM_FORTRAN_C_TYPE_REAL16 *) invec;
		pfr16out = (LAM_FORTRAN_C_TYPE_REAL16 *) inoutvec;
		for (i = 0; i < *len; ++i, ++pfr16in, ++pfr16out) {
			if (*pfr16in > *pfr16out) {
				*pfr16out = *pfr16in;
			}
		}
		return;
	}
#endif

	(void) lam_errfunc(MPI_COMM_WORLD,
			   lam_getfunc(), lam_mkerr(MPI_ERR_OP, EINVAL));
}

void
lam_min(void *invec, void *inoutvec, int *len, MPI_Datatype *datatype)
{
	int		i;
	int		*piin, *piout;
	long		*plin, *plout;
	short		*psin, *psout;
	unsigned short	*pusin, *pusout;
	unsigned	*puin, *puout;
	unsigned long	*pulin, *pulout;
	float		*pfin, *pfout;
	double		*pdin, *pdout;
#if LAM_SIZEOF_LONG_DOUBLE
	LAM_LONG_DOUBLE	*pldin, *pldout;
#else
	double		*pldin, *pldout;
#endif
#if LAM_SIZEOF_LONG_LONG
	long long	*pllin, *pllout;
	unsigned long long *pullin, *pullout;
#else
	long		*pllin, *pllout;
	unsigned long 	*pullin, *pullout;
#endif
#if LAM_SIZEOF_FORTRAN_INTEGER1
        LAM_FORTRAN_C_TYPE_INTEGER1 *pfint1in, *pfint1out;
#endif
#if LAM_SIZEOF_FORTRAN_INTEGER2
        LAM_FORTRAN_C_TYPE_INTEGER2 *pfint2in, *pfint2out;
#endif
#if LAM_SIZEOF_FORTRAN_INTEGER4
        LAM_FORTRAN_C_TYPE_INTEGER4 *pfint4in, *pfint4out;
#endif
#if LAM_SIZEOF_FORTRAN_INTEGER8
        LAM_FORTRAN_C_TYPE_INTEGER8 *pfint8in, *pfint8out;
#endif
#if LAM_SIZEOF_FORTRAN_REAL4 
        LAM_FORTRAN_C_TYPE_REAL4 *pfr4in, *pfr4out;
#endif
#if LAM_SIZEOF_FORTRAN_REAL8 
        LAM_FORTRAN_C_TYPE_REAL8 *pfr8in, *pfr8out;
#endif
#if LAM_SIZEOF_FORTRAN_REAL16 
        LAM_FORTRAN_C_TYPE_REAL16 *pfr16in, *pfr16out;
#endif
	
	if (*datatype == MPI_INT || *datatype == MPI_F_INTEGER) {
		piin = (int *) invec;
		piout = (int *) inoutvec;
		for (i = 0; i < *len; ++i, ++piin, ++piout) {
			if (*piin < *piout) {
				*piout = *piin;
			}
		}
		return;
	}

	if (*datatype == MPI_LONG) {
		plin = (long *) invec;
		plout = (long *) inoutvec;
		for (i = 0; i < *len; ++i, ++plin, ++plout) {
			if (*plin < *plout) {
				*plout = *plin;
			}
		}
		return;
	}

	if (*datatype == MPI_SHORT) {
		psin = (short *) invec;
		psout = (short *) inoutvec;
		for (i = 0; i < *len; ++i, ++psin, ++psout) {
			if (*psin < *psout) {
				*psout = *psin;
			}
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_SHORT) {
		pusin = (unsigned short *) invec;
		pusout = (unsigned short *) inoutvec;
		for (i = 0; i < *len; ++i, ++pusin, ++pusout) {
			if (*pusin < *pusout) {
				*pusout = *pusin;
			}
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED) {
		puin = (unsigned *) invec;
		puout = (unsigned *) inoutvec;
		for (i = 0; i < *len; ++i, ++puin, ++puout) {
			if (*puin < *puout) {
				*puout = *puin;
			}
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_LONG) {
		pulin = (unsigned long *) invec;
		pulout = (unsigned long *) inoutvec;
		for (i = 0; i < *len; ++i, ++pulin, ++pulout) {
			if (*pulin < *pulout) {
				*pulout = *pulin;
			}
		}
		return;
	}

	if (*datatype == MPI_FLOAT || *datatype == MPI_F_REAL) {
		pfin = (float *) invec;
		pfout = (float *) inoutvec;
		for (i = 0; i < *len; ++i, ++pfin, ++pfout) {
			if (*pfin < *pfout) {
				*pfout = *pfin;
			}
		}
		return;
	}

	if (*datatype == MPI_DOUBLE || *datatype == MPI_F_DOUBLE_PRECISION) {
		pdin = (double *) invec;
		pdout = (double *) inoutvec;
		for (i = 0; i < *len; ++i, ++pdin, ++pdout) {
			if (*pdin < *pdout) {
				*pdout = *pdin;
			}
		}
		return;
	}

	if (*datatype == MPI_LONG_DOUBLE) {
#if LAM_SIZEOF_LONG_DOUBLE
		pldin = (LAM_LONG_DOUBLE *) invec;
		pldout = (LAM_LONG_DOUBLE *) inoutvec;
#else
		pldin = (double *) invec;
		pldout = (double *) inoutvec;
#endif
		for (i = 0; i < *len; ++i, ++pldin, ++pldout) {
			if (*pldin < *pldout) {
				*pldout = *pldin;
			}
		}
		return;
	}

	if (*datatype == MPI_LONG_LONG_INT) {
#if LAM_SIZEOF_LONG_LONG
		pllin = (long long *) invec;
		pllout = (long long *) inoutvec;
#else
		pllin = (long *) invec;
		pllout = (long *) inoutvec;
#endif
		for (i = 0; i < *len; ++i, ++pllin, ++pllout) {
			if (*pllin < *pllout) {
				*pllout = *pllin;
			}
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_LONG_LONG) {
#if LAM_SIZEOF_LONG_LONG
		pullin = (unsigned long long *) invec;
		pullout = (unsigned long long *) inoutvec;
#else
		pullin = (unsigned long *) invec;
		pullout = (unsigned long *) inoutvec;
#endif
		for (i = 0; i < *len; ++i, ++pullin, ++pullout) {
			if (*pullin < *pullout) {
				*pullout = *pullin;
			}
		}
		return;
	}

#if LAM_SIZEOF_FORTRAN_INTEGER1
        if (*datatype == MPI_F_INTEGER1) {
            pfint1in = (LAM_FORTRAN_C_TYPE_INTEGER1 *) invec;
            pfint1out = (LAM_FORTRAN_C_TYPE_INTEGER1 *) inoutvec;
            for (i = 0; i < *len; ++i, ++pfint1in, ++pfint1out) {
                if (*pfint1in < *pfint1out) {
                    *pfint1out = *pfint1in;
                }
            }
            return;
        }
#endif

#if LAM_SIZEOF_FORTRAN_INTEGER2
        if (*datatype == MPI_F_INTEGER2) {
            pfint2in = (LAM_FORTRAN_C_TYPE_INTEGER2 *) invec;
            pfint2out = (LAM_FORTRAN_C_TYPE_INTEGER2 *) inoutvec;
            for (i = 0; i < *len; ++i, ++pfint2in, ++pfint2out) {
                if (*pfint2in < *pfint2out) {
                    *pfint2out = *pfint2in;
                }
            }
            return;
        }
#endif

#if LAM_SIZEOF_FORTRAN_INTEGER4
        if (*datatype == MPI_F_INTEGER4) {
            pfint4in = (LAM_FORTRAN_C_TYPE_INTEGER4 *) invec;
            pfint4out = (LAM_FORTRAN_C_TYPE_INTEGER4 *) inoutvec;
            for (i = 0; i < *len; ++i, ++pfint4in, ++pfint4out) {
                if (*pfint4in < *pfint4out) {
                    *pfint4out = *pfint4in;
                }
            }
            return;
        }
#endif

#if LAM_SIZEOF_FORTRAN_INTEGER8
        if (*datatype == MPI_F_INTEGER8) {
            pfint8in = (LAM_FORTRAN_C_TYPE_INTEGER8 *) invec;
            pfint8out = (LAM_FORTRAN_C_TYPE_INTEGER8 *) inoutvec;
            for (i = 0; i < *len; ++i, ++pfint8in, ++pfint8out) {
                if (*pfint8in < *pfint8out) {
                    *pfint8out = *pfint8in;
                }
            }
            return;
        }
#endif

#if LAM_SIZEOF_FORTRAN_REAL4
	if (*datatype == MPI_F_REAL4) {
		pfr4in = (LAM_FORTRAN_C_TYPE_REAL4 *) invec;
		pfr4out = (LAM_FORTRAN_C_TYPE_REAL4 *) inoutvec;
		for (i = 0; i < *len; ++i, ++pfr4in, ++pfr4out) {
			if (*pfr4in < *pfr4out) {
				*pfr4out = *pfr4in;
			}
		}
		return;
	}
#endif

#if LAM_SIZEOF_FORTRAN_REAL8
	if (*datatype == MPI_F_REAL8) {
		pfr8in = (LAM_FORTRAN_C_TYPE_REAL8 *) invec;
		pfr8out = (LAM_FORTRAN_C_TYPE_REAL8 *) inoutvec;
		for (i = 0; i < *len; ++i, ++pfr8in, ++pfr8out) {
			if (*pfr8in < *pfr8out) {
				*pfr8out = *pfr8in;
			}
		}
		return;
	}
#endif

#if LAM_SIZEOF_FORTRAN_REAL16
	if (*datatype == MPI_F_REAL16) {
		pfr16in = (LAM_FORTRAN_C_TYPE_REAL16 *) invec;
		pfr16out = (LAM_FORTRAN_C_TYPE_REAL16 *) inoutvec;
		for (i = 0; i < *len; ++i, ++pfr16in, ++pfr16out) {
			if (*pfr16in < *pfr16out) {
				*pfr16out = *pfr16in;
			}
		}
		return;
	}
#endif

	(void) lam_errfunc(MPI_COMM_WORLD,
			   lam_getfunc(), lam_mkerr(MPI_ERR_OP, EINVAL));
}

void
lam_sum(void *invec, void *inoutvec, int *len, MPI_Datatype *datatype)
{
	int		i;
	int		*piin, *piout;
	long		*plin, *plout;
	short		*psin, *psout;
	unsigned short	*pusin, *pusout;
	unsigned	*puin, *puout;
	unsigned long	*pulin, *pulout;
	float		*pfin, *pfout;
	double		*pdin, *pdout;
	struct complex	*pxin, *pxout;
	struct dblcplex	*pdxin, *pdxout;
	struct ldblcplex *pldxin, *pldxout;
#if LAM_SIZEOF_LONG_DOUBLE
	LAM_LONG_DOUBLE	*pldin, *pldout;
#else
	double		*pldin, *pldout;
#endif
#if LAM_SIZEOF_LONG_LONG
	long long	*pllin, *pllout;
	unsigned long long *pullin, *pullout;
#else
	long		*pllin, *pllout;
	unsigned long 	*pullin, *pullout;
#endif
#if LAM_SIZEOF_FORTRAN_INTEGER1
        LAM_FORTRAN_C_TYPE_INTEGER1 *pfint1in, *pfint1out;
#endif
#if LAM_SIZEOF_FORTRAN_INTEGER2
        LAM_FORTRAN_C_TYPE_INTEGER2 *pfint2in, *pfint2out;
#endif
#if LAM_SIZEOF_FORTRAN_INTEGER4
        LAM_FORTRAN_C_TYPE_INTEGER4 *pfint4in, *pfint4out;
#endif
#if LAM_SIZEOF_FORTRAN_INTEGER8
        LAM_FORTRAN_C_TYPE_INTEGER8 *pfint8in, *pfint8out;
#endif
#if LAM_SIZEOF_FORTRAN_REAL4 
        LAM_FORTRAN_C_TYPE_REAL4 *pfr4in, *pfr4out;
#endif
#if LAM_SIZEOF_FORTRAN_REAL8 
        LAM_FORTRAN_C_TYPE_REAL8 *pfr8in, *pfr8out;
#endif
#if LAM_SIZEOF_FORTRAN_REAL16 
        LAM_FORTRAN_C_TYPE_REAL16 *pfr16in, *pfr16out;
#endif

	if (*datatype == MPI_INT || *datatype == MPI_F_INTEGER) {
		piin = (int *) invec;
		piout = (int *) inoutvec;
		for (i = 0; i < *len; ++i, ++piin, ++piout) {
			*piout += *piin;
		}
		return;
	}

	if (*datatype == MPI_LONG) {
		plin = (long *) invec;
		plout = (long *) inoutvec;
		for (i = 0; i < *len; ++i, ++plin, ++plout) {
			*plout += *plin;
		}
		return;
	}

	if (*datatype == MPI_SHORT) {
		psin = (short *) invec;
		psout = (short *) inoutvec;
		for (i = 0; i < *len; ++i, ++psin, ++psout) {
			*psout += *psin;
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_SHORT) {
		pusin = (unsigned short *) invec;
		pusout = (unsigned short *) inoutvec;
		for (i = 0; i < *len; ++i, ++pusin, ++pusout) {
			*pusout += *pusin;
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED) {
		puin = (unsigned *) invec;
		puout = (unsigned *) inoutvec;
		for (i = 0; i < *len; ++i, ++puin, ++puout) {
			*puout += *puin;
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_LONG) {
		pulin = (unsigned long *) invec;
		pulout = (unsigned long *) inoutvec;
		for (i = 0; i < *len; ++i, ++pulin, ++pulout) {
			*pulout += *pulin;
		}
		return;
	}

	if (*datatype == MPI_FLOAT  || *datatype == MPI_F_REAL) {
		pfin = (float *) invec;
		pfout = (float *) inoutvec;
		for (i = 0; i < *len; ++i, ++pfin, ++pfout) {
			*pfout += *pfin;
		}
		return;
	}

	if (*datatype == MPI_F_COMPLEX || *datatype == MPI_CXX_COMPLEX) {
		pxin = (struct complex *) invec;
		pxout = (struct complex *) inoutvec;
		for (i = 0; i < *len; ++i, ++pxin, ++pxout) {
			pxout->r += pxin->r;
			pxout->i += pxin->i;
		}
		return;
	}

	if (*datatype == MPI_F_DOUBLE_COMPLEX ||
	    *datatype == MPI_CXX_DOUBLE_COMPLEX) {
		pdxin = (struct dblcplex *) invec;
		pdxout = (struct dblcplex *) inoutvec;
		for (i = 0; i < *len; ++i, ++pdxin, ++pdxout) {
			pdxout->r += pdxin->r;
			pdxout->i += pdxin->i;
		}
		return;
	}

	if (*datatype == MPI_CXX_LONG_DOUBLE_COMPLEX) {
		pldxin = (struct ldblcplex *) invec;
		pldxout = (struct ldblcplex *) inoutvec;
		for (i = 0; i < *len; ++i, ++pldxin, ++pldxout) {
			pldxout->r += pldxin->r;
			pldxout->i += pldxin->i;
		}
		return;
	}

	if (*datatype == MPI_DOUBLE || *datatype == MPI_F_DOUBLE_PRECISION) {
		pdin = (double *) invec;
		pdout = (double *) inoutvec;
		for (i = 0; i < *len; ++i, ++pdin, ++pdout) {
			*pdout += *pdin;
		}
		return;
	}

	if (*datatype == MPI_LONG_DOUBLE) {
#if LAM_SIZEOF_LONG_DOUBLE
		pldin = (LAM_LONG_DOUBLE *) invec;
		pldout = (LAM_LONG_DOUBLE *) inoutvec;
#else
		pldin = (double *) invec;
		pldout = (double *) inoutvec;
#endif
		for (i = 0; i < *len; ++i, ++pldin, ++pldout) {
			*pldout += *pldin;
		}
		return;
	}

	if (*datatype == MPI_LONG_LONG_INT) {
#if LAM_SIZEOF_LONG_LONG
		pllin = (long long *) invec;
		pllout = (long long *) inoutvec;
#else
		pllin = (long *) invec;
		pllout = (long *) inoutvec;
#endif
		for (i = 0; i < *len; ++i, ++pllin, ++pllout) {
			*pllout += *pllin;
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_LONG_LONG) {
#if LAM_SIZEOF_LONG_LONG
		pullin = (unsigned long long *) invec;
		pullout = (unsigned long long *) inoutvec;
#else
		pullin = (unsigned long *) invec;
		pullout = (unsigned long *) inoutvec;
#endif
		for (i = 0; i < *len; ++i, ++pullin, ++pullout) {
			*pullout += *pullin;
		}
		return;
	}

#if LAM_SIZEOF_FORTRAN_INTEGER1
        if (*datatype == MPI_F_INTEGER1) {
            pfint1in = (LAM_FORTRAN_C_TYPE_INTEGER1 *) invec;
            pfint1out = (LAM_FORTRAN_C_TYPE_INTEGER1 *) inoutvec;
            for (i = 0; i < *len; ++i, ++pfint1in, ++pfint1out) {
                *pfint1out += *pfint1in;
            }
            return;
        }
#endif

#if LAM_SIZEOF_FORTRAN_INTEGER2
        if (*datatype == MPI_F_INTEGER2) {
            pfint2in = (LAM_FORTRAN_C_TYPE_INTEGER2 *) invec;
            pfint2out = (LAM_FORTRAN_C_TYPE_INTEGER2 *) inoutvec;
            for (i = 0; i < *len; ++i, ++pfint2in, ++pfint2out) {
                *pfint2out += *pfint2in;
            }
            return;
        }
#endif

#if LAM_SIZEOF_FORTRAN_INTEGER4
        if (*datatype == MPI_F_INTEGER4) {
            pfint4in = (LAM_FORTRAN_C_TYPE_INTEGER4 *) invec;
            pfint4out = (LAM_FORTRAN_C_TYPE_INTEGER4 *) inoutvec;
            for (i = 0; i < *len; ++i, ++pfint4in, ++pfint4out) {
                *pfint4out += *pfint4in;
            }
            return;
        }
#endif

#if LAM_SIZEOF_FORTRAN_INTEGER8
        if (*datatype == MPI_F_INTEGER8) {
            pfint8in = (LAM_FORTRAN_C_TYPE_INTEGER8 *) invec;
            pfint8out = (LAM_FORTRAN_C_TYPE_INTEGER8 *) inoutvec;
            for (i = 0; i < *len; ++i, ++pfint8in, ++pfint8out) {
                *pfint8out += *pfint8in;
            }
            return;
        }
#endif

#if LAM_SIZEOF_FORTRAN_REAL4
	if (*datatype == MPI_F_REAL4) {
		pfr4in = (LAM_FORTRAN_C_TYPE_REAL4 *) invec;
		pfr4out = (LAM_FORTRAN_C_TYPE_REAL4 *) inoutvec;
		for (i = 0; i < *len; ++i, ++pfr4in, ++pfr4out) {
			*pfr4out += *pfr4in;
		}
		return;
	}
#endif

#if LAM_SIZEOF_FORTRAN_REAL8
	if (*datatype == MPI_F_REAL8) {
		pfr8in = (LAM_FORTRAN_C_TYPE_REAL8 *) invec;
		pfr8out = (LAM_FORTRAN_C_TYPE_REAL8 *) inoutvec;
		for (i = 0; i < *len; ++i, ++pfr8in, ++pfr8out) {
			*pfr8out += *pfr8in;
		}
		return;
	}
#endif

#if LAM_SIZEOF_FORTRAN_REAL16
	if (*datatype == MPI_F_REAL16) {
		pfr16in = (LAM_FORTRAN_C_TYPE_REAL16 *) invec;
		pfr16out = (LAM_FORTRAN_C_TYPE_REAL16 *) inoutvec;
		for (i = 0; i < *len; ++i, ++pfr16in, ++pfr16out) {
			*pfr16out += *pfr16in;
		}
		return;
	}
#endif

	(void) lam_errfunc(MPI_COMM_WORLD,
			   lam_getfunc(), lam_mkerr(MPI_ERR_OP, EINVAL));
}

void
lam_prod(void *invec, void *inoutvec, int *len, MPI_Datatype *datatype)
{
	int		i;
	int		*piin, *piout;
	long		*plin, *plout;
	short		*psin, *psout;
	unsigned short	*pusin, *pusout;
	unsigned	*puin, *puout;
	unsigned long	*pulin, *pulout;
	float		*pfin, *pfout;
	float		xr, xi;
	double		dxr, dxi;
	double		*pdin, *pdout;
	struct complex	*pxin, *pxout;
	struct dblcplex	*pdxin, *pdxout;
	struct ldblcplex *pldxin, *pldxout;
#if LAM_SIZEOF_LONG_DOUBLE
	LAM_LONG_DOUBLE	*pldin, *pldout;
	LAM_LONG_DOUBLE ldxr, ldxi;
#else
	double		*pldin, *pldout;
	double          ldxr, ldxi;
#endif
#if LAM_SIZEOF_LONG_LONG
	long long	*pllin, *pllout;
	unsigned long long *pullin, *pullout;
#else
	long		*pllin, *pllout;
	unsigned long 	*pullin, *pullout;
#endif
#if LAM_SIZEOF_FORTRAN_INTEGER1
        LAM_FORTRAN_C_TYPE_INTEGER1 *pfint1in, *pfint1out;
#endif
#if LAM_SIZEOF_FORTRAN_INTEGER2
        LAM_FORTRAN_C_TYPE_INTEGER2 *pfint2in, *pfint2out;
#endif
#if LAM_SIZEOF_FORTRAN_INTEGER4
        LAM_FORTRAN_C_TYPE_INTEGER4 *pfint4in, *pfint4out;
#endif
#if LAM_SIZEOF_FORTRAN_INTEGER8
        LAM_FORTRAN_C_TYPE_INTEGER8 *pfint8in, *pfint8out;
#endif
#if LAM_SIZEOF_FORTRAN_REAL4 
        LAM_FORTRAN_C_TYPE_REAL4 *pfr4in, *pfr4out;
#endif
#if LAM_SIZEOF_FORTRAN_REAL8 
        LAM_FORTRAN_C_TYPE_REAL8 *pfr8in, *pfr8out;
#endif
#if LAM_SIZEOF_FORTRAN_REAL16 
        LAM_FORTRAN_C_TYPE_REAL16 *pfr16in, *pfr16out;
#endif

	if (*datatype == MPI_INT || *datatype == MPI_F_INTEGER) {
		piin = (int *) invec;
		piout = (int *) inoutvec;
		for (i = 0; i < *len; ++i, ++piin, ++piout) {
			*piout *= *piin;
		}
		return;
	}

	if (*datatype == MPI_LONG) {
		plin = (long *) invec;
		plout = (long *) inoutvec;
		for (i = 0; i < *len; ++i, ++plin, ++plout) {
			*plout *= *plin;
		}
		return;
	}

	if (*datatype == MPI_SHORT) {
		psin = (short *) invec;
		psout = (short *) inoutvec;
		for (i = 0; i < *len; ++i, ++psin, ++psout) {
			*psout *= *psin;
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_SHORT) {
		pusin = (unsigned short *) invec;
		pusout = (unsigned short *) inoutvec;
		for (i = 0; i < *len; ++i, ++pusin, ++pusout) {
			*pusout *= *pusin;
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED) {
		puin = (unsigned *) invec;
		puout = (unsigned *) inoutvec;
		for (i = 0; i < *len; ++i, ++puin, ++puout) {
			*puout *= *puin;
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_LONG) {
		pulin = (unsigned long *) invec;
		pulout = (unsigned long *) inoutvec;
		for (i = 0; i < *len; ++i, ++pulin, ++pulout) {
			*pulout *= *pulin;
		}
		return;
	}

	if (*datatype == MPI_FLOAT || *datatype == MPI_F_REAL) {
		pfin = (float *) invec;
		pfout = (float *) inoutvec;
		for (i = 0; i < *len; ++i, ++pfin, ++pfout) {
			*pfout *= *pfin;
		}
		return;
	}

	if (*datatype == MPI_F_COMPLEX ||
	    *datatype == MPI_CXX_COMPLEX) {
		pxin = (struct complex *) invec;
		pxout = (struct complex *) inoutvec;
		for (i = 0; i < *len; ++i, ++pxin, ++pxout) {
			xr = pxout->r;
			xi = pxout->i;
			pxout->r = (xr * pxin->r) - (xi * pxin->i);
			pxout->i = (xr * pxin->i) + (xi * pxin->r);
		}
		return;
	}

	if (*datatype == MPI_F_DOUBLE_COMPLEX ||
	    *datatype == MPI_CXX_DOUBLE_COMPLEX) {
		pdxin = (struct dblcplex *) invec;
		pdxout = (struct dblcplex *) inoutvec;
		for (i = 0; i < *len; ++i, ++pdxin, ++pdxout) {
			dxr = pdxout->r;
			dxi = pdxout->i;
			pdxout->r = (dxr * pdxin->r) - (dxi * pdxin->i);
			pdxout->i = (dxr * pdxin->i) + (dxi * pdxin->r);
		}
		return;
	}

	if (*datatype == MPI_CXX_LONG_DOUBLE_COMPLEX) {
	  pldxin = (struct ldblcplex *) invec;
	  pldxout = (struct ldblcplex *) inoutvec;
	  for (i = 0; i < *len; ++i, ++pldxin, ++pldxout) {
	    ldxr = pldxout->r;
	    ldxi = pldxout->i;
	    pldxout->r = (ldxr * pldxin->r) - (ldxi * pldxin->i);
	    pldxout->i = (ldxr * pldxin->i) + (ldxi * pldxin->r);
	  }
	  return;
	}

	if (*datatype == MPI_DOUBLE || *datatype == MPI_F_DOUBLE_PRECISION) {
		pdin = (double *) invec;
		pdout = (double *) inoutvec;
		for (i = 0; i < *len; ++i, ++pdin, ++pdout) {
			*pdout *= *pdin;
		}
		return;
	}

	if (*datatype == MPI_LONG_DOUBLE) {
#if LAM_SIZEOF_LONG_DOUBLE
		pldin = (LAM_LONG_DOUBLE *) invec;
		pldout = (LAM_LONG_DOUBLE *) inoutvec;
#else
		pldin = (double *) invec;
		pldout = (double *) inoutvec;
#endif
		for (i = 0; i < *len; ++i, ++pldin, ++pldout) {
			*pldout *= *pldin;
		}
		return;
	}

	if (*datatype == MPI_LONG_LONG_INT) {
#if LAM_SIZEOF_LONG_LONG
		pllin = (long long *) invec;
		pllout = (long long *) inoutvec;
#else
		pllin = (long *) invec;
		pllout = (long *) inoutvec;
#endif
		for (i = 0; i < *len; ++i, ++pllin, ++pllout) {
			*pllout *= *pllin;
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_LONG_LONG) {
#if LAM_SIZEOF_LONG_LONG
		pullin = (unsigned long long *) invec;
		pullout = (unsigned long long *) inoutvec;
#else
		pullin = (unsigned long *) invec;
		pullout = (unsigned long *) inoutvec;
#endif
		for (i = 0; i < *len; ++i, ++pullin, ++pullout) {
			*pullout *= *pullin;
		}
		return;
	}

#if LAM_SIZEOF_FORTRAN_INTEGER1
        if (*datatype == MPI_F_INTEGER1) {
            pfint1in = (LAM_FORTRAN_C_TYPE_INTEGER1 *) invec;
            pfint1out = (LAM_FORTRAN_C_TYPE_INTEGER1 *) inoutvec;
            for (i = 0; i < *len; ++i, ++pfint1in, ++pfint1out) {
                *pfint1out *= *pfint1in;
            }
            return;
        }
#endif

#if LAM_SIZEOF_FORTRAN_INTEGER2
        if (*datatype == MPI_F_INTEGER2) {
            pfint2in = (LAM_FORTRAN_C_TYPE_INTEGER2 *) invec;
            pfint2out = (LAM_FORTRAN_C_TYPE_INTEGER2 *) inoutvec;
            for (i = 0; i < *len; ++i, ++pfint2in, ++pfint2out) {
                *pfint2out *= *pfint2in;
            }
            return;
        }
#endif

#if LAM_SIZEOF_FORTRAN_INTEGER4
        if (*datatype == MPI_F_INTEGER4) {
            pfint4in = (LAM_FORTRAN_C_TYPE_INTEGER4 *) invec;
            pfint4out = (LAM_FORTRAN_C_TYPE_INTEGER4 *) inoutvec;
            for (i = 0; i < *len; ++i, ++pfint4in, ++pfint4out) {
                *pfint4out *= *pfint4in;
            }
            return;
        }
#endif

#if LAM_SIZEOF_FORTRAN_INTEGER8
        if (*datatype == MPI_F_INTEGER8) {
            pfint8in = (LAM_FORTRAN_C_TYPE_INTEGER8 *) invec;
            pfint8out = (LAM_FORTRAN_C_TYPE_INTEGER8 *) inoutvec;
            for (i = 0; i < *len; ++i, ++pfint8in, ++pfint8out) {
                *pfint8out *= *pfint8in;
            }
            return;
        }
#endif

#if LAM_SIZEOF_FORTRAN_REAL4
	if (*datatype == MPI_F_REAL4) {
		pfr4in = (LAM_FORTRAN_C_TYPE_REAL4 *) invec;
		pfr4out = (LAM_FORTRAN_C_TYPE_REAL4 *) inoutvec;
		for (i = 0; i < *len; ++i, ++pfr4in, ++pfr4out) {
			*pfr4out *= *pfr4in;
		}
		return;
	}
#endif

#if LAM_SIZEOF_FORTRAN_REAL8
	if (*datatype == MPI_F_REAL8) {
		pfr8in = (LAM_FORTRAN_C_TYPE_REAL8 *) invec;
		pfr8out = (LAM_FORTRAN_C_TYPE_REAL8 *) inoutvec;
		for (i = 0; i < *len; ++i, ++pfr8in, ++pfr8out) {
			*pfr8out *= *pfr8in;
		}
		return;
	}
#endif

#if LAM_SIZEOF_FORTRAN_REAL16
	if (*datatype == MPI_F_REAL16) {
		pfr16in = (LAM_FORTRAN_C_TYPE_REAL16 *) invec;
		pfr16out = (LAM_FORTRAN_C_TYPE_REAL16 *) inoutvec;
		for (i = 0; i < *len; ++i, ++pfr16in, ++pfr16out) {
			*pfr16out *= *pfr16in;
		}
		return;
	}
#endif

	(void) lam_errfunc(MPI_COMM_WORLD,
			   lam_getfunc(), lam_mkerr(MPI_ERR_OP, EINVAL));
}


/*
 * Logical And
 *
 * Supported by:
 *   C integer: MPI_INT, MPI_LONG, MPI_SHORT, MPI_UNSIGNED_SHORT,
 *              MPI_UNSIGNED, MPI_UNSIGNED_LONG
 *   Logical:   MPI_LOGICAL
 *   Special C++: MPI_CXX_BOOL
 */
void
lam_land(void *invec, void *inoutvec, int *len, MPI_Datatype *datatype)
{
	int		i;
	int		*piin, *piout;
	long		*plin, *plout;
	short		*psin, *psout;
	unsigned short	*pusin, *pusout;
	unsigned	*puin, *puout;
	unsigned long	*pulin, *pulout;
	LAM_CXX_BOOL_TYPE *pboolin, *pboolout;
#if LAM_SIZEOF_LONG_LONG
	long long	*pllin, *pllout;
	unsigned long long *pullin, *pullout;
#else
	long		*pllin, *pllout;
	unsigned long 	*pullin, *pullout;
#endif

	if ((*datatype == MPI_INT) || (*datatype == MPI_F_LOGICAL)
			|| (*datatype == MPI_F_INTEGER)) {
		piin = (int *) invec;
		piout = (int *) inoutvec;
		for (i = 0; i < *len; ++i, ++piin, ++piout) {
			*piout = *piout && *piin;
		}
		return;
	}

	if (*datatype == MPI_LONG) {
		plin = (long *) invec;
		plout = (long *) inoutvec;
		for (i = 0; i < *len; ++i, ++plin, ++plout) {
			*plout = *plout && *plin;
		}
		return;
	}

	if (*datatype == MPI_SHORT) {
		psin = (short *) invec;
		psout = (short *) inoutvec;
		for (i = 0; i < *len; ++i, ++psin, ++psout) {
			*psout = *psout && *psin;
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_SHORT) {
		pusin = (unsigned short *) invec;
		pusout = (unsigned short *) inoutvec;
		for (i = 0; i < *len; ++i, ++pusin, ++pusout) {
			*pusout = *pusout && *pusin;
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED) {
		puin = (unsigned *) invec;
		puout = (unsigned *) inoutvec;
		for (i = 0; i < *len; ++i, ++puin, ++puout) {
			*puout = *puout && *puin;
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_LONG) {
		pulin = (unsigned long *) invec;
		pulout = (unsigned long *) inoutvec;
		for (i = 0; i < *len; ++i, ++pulin, ++pulout) {
			*pulout = *pulout && *pulin;
		}
		return;
	}

	if (*datatype == MPI_LONG_LONG_INT) {
#if LAM_SIZEOF_LONG_LONG
		pllin = (long long *) invec;
		pllout = (long long *) inoutvec;
#else
		pllin = (long *) invec;
		pllout = (long *) inoutvec;
#endif
		for (i = 0; i < *len; ++i, ++pllin, ++pllout) {
			*pllout = *pllout && *pllin;
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_LONG_LONG) {
#if LAM_SIZEOF_LONG_LONG
		pullin = (unsigned long long *) invec;
		pullout = (unsigned long long *) inoutvec;
#else
		pullin = (unsigned long *) invec;
		pullout = (unsigned long *) inoutvec;
#endif
		for (i = 0; i < *len; ++i, ++pullin, ++pullout) {
			*pullout = *pullout && *pullin;	
		}
		return;
	}

	if (*datatype == MPI_CXX_BOOL) {
	  pboolin = (LAM_CXX_BOOL_TYPE *) invec;
	  pboolout = (LAM_CXX_BOOL_TYPE *) inoutvec;

	  for (i = 0 ; i < *len ; ++i, ++pboolin, ++pboolout) {
	    *pboolout = *pboolout && *pboolin;
	  }
	  return;
	}

	(void) lam_errfunc(MPI_COMM_WORLD,
			   lam_getfunc(), lam_mkerr(MPI_ERR_OP, EINVAL));
}


/*
 * Logical Or
 *
 * Supported by:
 *   C integer: MPI_INT, MPI_LONG, MPI_SHORT, MPI_UNSIGNED_SHORT,
 *              MPI_UNSIGNED, MPI_UNSIGNED_LONG
 *   Logical:   MPI_LOGICAL
 *   Special C++: MPI_CXX_BOOL
 */
void
lam_lor(void *invec, void *inoutvec, int *len, MPI_Datatype *datatype)
{
	int		i;
	int		*piin, *piout;
	long		*plin, *plout;
	short		*psin, *psout;
	unsigned short	*pusin, *pusout;
	unsigned	*puin, *puout;
	unsigned long	*pulin, *pulout;
	LAM_CXX_BOOL_TYPE *pboolin, *pboolout;
#if LAM_SIZEOF_LONG_LONG
	long long	*pllin, *pllout;
	unsigned long long *pullin, *pullout;
#else
	long		*pllin, *pllout;
	unsigned long 	*pullin, *pullout;
#endif

	if ((*datatype == MPI_INT) || (*datatype == MPI_F_LOGICAL)
			|| (*datatype == MPI_F_INTEGER)) {
		piin = (int *) invec;
		piout = (int *) inoutvec;
		for (i = 0; i < *len; ++i, ++piin, ++piout) {
			*piout = *piout || *piin;
		}
		return;
	}

	if (*datatype == MPI_LONG) {
		plin = (long *) invec;
		plout = (long *) inoutvec;
		for (i = 0; i < *len; ++i, ++plin, ++plout) {
			*plout = *plout || *plin;
		}
		return;
	}

	if (*datatype == MPI_SHORT) {
		psin = (short *) invec;
		psout = (short *) inoutvec;
		for (i = 0; i < *len; ++i, ++psin, ++psout) {
			*psout = *psout || *psin;
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_SHORT) {
		pusin = (unsigned short *) invec;
		pusout = (unsigned short *) inoutvec;
		for (i = 0; i < *len; ++i, ++pusin, ++pusout) {
			*pusout = *pusout || *pusin;
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED) {
		puin = (unsigned *) invec;
		puout = (unsigned *) inoutvec;
		for (i = 0; i < *len; ++i, ++puin, ++puout) {
			*puout = *puout || *puin;
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_LONG) {
		pulin = (unsigned long *) invec;
		pulout = (unsigned long *) inoutvec;
		for (i = 0; i < *len; ++i, ++pulin, ++pulout) {
			*pulout = *pulout || *pulin;
		}
		return;
	}

	if (*datatype == MPI_LONG_LONG_INT) {
#if LAM_SIZEOF_LONG_LONG
		pllin = (long long *) invec;
		pllout = (long long *) inoutvec;
#else
		pllin = (long *) invec;
		pllout = (long *) inoutvec;
#endif
		for (i = 0; i < *len; ++i, ++pllin, ++pllout) {
			*pllout = *pllout || *pllin;
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_LONG_LONG) {
#if LAM_SIZEOF_LONG_LONG
		pullin = (unsigned long long *) invec;
		pullout = (unsigned long long *) inoutvec;
#else
		pullin = (unsigned long *) invec;
		pullout = (unsigned long *) inoutvec;
#endif
		for (i = 0; i < *len; ++i, ++pullin, ++pullout) {
			*pullout = *pullout || *pullin;	
		}
		return;
	}

	if (*datatype == MPI_CXX_BOOL) {
	  pboolin = (LAM_CXX_BOOL_TYPE *) invec;
	  pboolout = (LAM_CXX_BOOL_TYPE *) inoutvec;

	  for (i = 0 ; i < *len ; ++i, ++pboolin, ++pboolout) {
	    *pboolout = *pboolout || *pboolin;
	  }
	  return;
	}

	(void) lam_errfunc(MPI_COMM_WORLD,
			   lam_getfunc(), lam_mkerr(MPI_ERR_OP, EINVAL));
}


/*
 * Logical Or
 *
 * Supported by:
 *   C integer: MPI_INT, MPI_LONG, MPI_SHORT, MPI_UNSIGNED_SHORT,
 *              MPI_UNSIGNED, MPI_UNSIGNED_LONG
 *   Logical:   MPI_LOGICAL
 *   Special C++: MPI_CXX_BOOL
 */
void
lam_lxor(void *invec, void *inoutvec, int *len, MPI_Datatype *datatype)
{
	int		i;
	int		*piin, *piout;
	long		*plin, *plout;
	short		*psin, *psout;
	unsigned short	*pusin, *pusout;
	unsigned	*puin, *puout;
	unsigned long	*pulin, *pulout;
	LAM_CXX_BOOL_TYPE *pboolin, *pboolout;
#if LAM_SIZEOF_LONG_LONG
	long long	*pllin, *pllout;
	unsigned long long *pullin, *pullout;
#else
	long		*pllin, *pllout;
	unsigned long 	*pullin, *pullout;
#endif

	if ((*datatype == MPI_INT) || (*datatype == MPI_F_LOGICAL)
			|| (*datatype == MPI_F_INTEGER)) {
		piin = (int *) invec;
		piout = (int *) inoutvec;
		for (i = 0; i < *len; ++i, ++piin, ++piout) {
			*piout = (*piout || *piin) && !(*piout && *piin);
		}
		return;
	}

	if (*datatype == MPI_LONG) {
		plin = (long *) invec;
		plout = (long *) inoutvec;
		for (i = 0; i < *len; ++i, ++plin, ++plout) {
			*plout = (*plout || *plin) && !(*plout && *plin);
		}
		return;
	}

	if (*datatype == MPI_SHORT) {
		psin = (short *) invec;
		psout = (short *) inoutvec;
		for (i = 0; i < *len; ++i, ++psin, ++psout) {
			*psout = (*psout || *psin) && !(*psout && *psin);
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_SHORT) {
		pusin = (unsigned short *) invec;
		pusout = (unsigned short *) inoutvec;
		for (i = 0; i < *len; ++i, ++pusin, ++pusout) {
			*pusout = (*pusout || *pusin) && !(*pusout && *pusin);
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED) {
		puin = (unsigned *) invec;
		puout = (unsigned *) inoutvec;
		for (i = 0; i < *len; ++i, ++puin, ++puout) {
			*puout = (*puout || *puin) && !(*puout && *puin);
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_LONG) {
		pulin = (unsigned long *) invec;
		pulout = (unsigned long *) inoutvec;
		for (i = 0; i < *len; ++i, ++pulin, ++pulout) {
			*pulout = (*pulout || *pulin) && !(*pulout && *pulin);
		}
		return;
	}

	if (*datatype == MPI_LONG_LONG_INT) {
#if LAM_SIZEOF_LONG_LONG
		pllin = (long long *) invec;
		pllout = (long long *) inoutvec;
#else
		pllin = (long *) invec;
		pllout = (long *) inoutvec;
#endif
		for (i = 0; i < *len; ++i, ++pllin, ++pllout) {
			*pllout = (*pllout || *pllin) && !(*pllout && *pllin);
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_LONG_LONG) {
#if LAM_SIZEOF_LONG_LONG
		pullin = (unsigned long long *) invec;
		pullout = (unsigned long long *) inoutvec;
#else
		pullin = (unsigned long *) invec;
		pullout = (unsigned long *) inoutvec;
#endif
		for (i = 0; i < *len; ++i, ++pullin, ++pullout) {
			*pullout = (*pullout || *pullin)
					&& !(*pullout && *pullin);
		}
		return;
	}

	if (*datatype == MPI_CXX_BOOL) {
	  pboolin = (LAM_CXX_BOOL_TYPE *) invec;
	  pboolout = (LAM_CXX_BOOL_TYPE *) inoutvec;

	  for (i = 0 ; i < *len ; ++i, ++pboolin, ++pboolout) {
	    *pboolout = (*pboolout || *pboolin) &&
	      !(*pboolout && *pboolin);
	  }
	  return;
	}

	(void) lam_errfunc(MPI_COMM_WORLD,
			   lam_getfunc(), lam_mkerr(MPI_ERR_OP, EINVAL));
}

void
lam_band(void *invec, void *inoutvec, int *len, MPI_Datatype *datatype)
{
	int		i;
	int		*piin, *piout;
	long		*plin, *plout;
	short		*psin, *psout;
	unsigned short	*pusin, *pusout;
	unsigned	*puin, *puout;
	unsigned long	*pulin, *pulout;
	unsigned char	*pucin, *pucout;
#if LAM_SIZEOF_LONG_LONG
	long long	*pllin, *pllout;
	unsigned long long *pullin, *pullout;
#else
	long		*pllin, *pllout;
	unsigned long 	*pullin, *pullout;
#endif
#if LAM_SIZEOF_FORTRAN_INTEGER1
        LAM_FORTRAN_C_TYPE_INTEGER1 *pfint1in, *pfint1out;
#endif
#if LAM_SIZEOF_FORTRAN_INTEGER2
        LAM_FORTRAN_C_TYPE_INTEGER2 *pfint2in, *pfint2out;
#endif
#if LAM_SIZEOF_FORTRAN_INTEGER4
        LAM_FORTRAN_C_TYPE_INTEGER4 *pfint4in, *pfint4out;
#endif
#if LAM_SIZEOF_FORTRAN_INTEGER8
        LAM_FORTRAN_C_TYPE_INTEGER8 *pfint8in, *pfint8out;
#endif

	if ((*datatype == MPI_INT) || (*datatype == MPI_F_LOGICAL)
			|| (*datatype == MPI_F_INTEGER)) {
		piin = (int *) invec;
		piout = (int *) inoutvec;
		for (i = 0; i < *len; ++i, ++piin, ++piout) {
			*piout &= *piin;
		}
		return;
	}

	if (*datatype == MPI_LONG) {
		plin = (long *) invec;
		plout = (long *) inoutvec;
		for (i = 0; i < *len; ++i, ++plin, ++plout) {
			*plout &= *plin;
		}
		return;
	}

	if (*datatype == MPI_SHORT) {
		psin = (short *) invec;
		psout = (short *) inoutvec;
		for (i = 0; i < *len; ++i, ++psin, ++psout) {
			*psout &= *psin;
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_SHORT) {
		pusin = (unsigned short *) invec;
		pusout = (unsigned short *) inoutvec;
		for (i = 0; i < *len; ++i, ++pusin, ++pusout) {
			*pusout &= *pusin;
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED) {
		puin = (unsigned *) invec;
		puout = (unsigned *) inoutvec;
		for (i = 0; i < *len; ++i, ++puin, ++puout) {
			*puout &= *puin;
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_LONG) {
		pulin = (unsigned long *) invec;
		pulout = (unsigned long *) inoutvec;
		for (i = 0; i < *len; ++i, ++pulin, ++pulout) {
			*pulout &= *pulin;
		}
		return;
	}

	if (*datatype == MPI_BYTE) {
		pucin = (unsigned char *) invec;
		pucout = (unsigned char *) inoutvec;
		for (i = 0; i < *len; ++i, ++pucin, ++pucout) {
			*pucout &= *pucin;
		}
		return;
	}

	if (*datatype == MPI_LONG_LONG_INT) {
#if LAM_SIZEOF_LONG_LONG
		pllin = (long long *) invec;
		pllout = (long long *) inoutvec;
#else
		pllin = (long *) invec;
		pllout = (long *) inoutvec;
#endif
		for (i = 0; i < *len; ++i, ++pllin, ++pllout) {
			*pllout &= *pllin;
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_LONG_LONG) {
#if LAM_SIZEOF_LONG_LONG
		pullin = (unsigned long long *) invec;
		pullout = (unsigned long long *) inoutvec;
#else
		pullin = (unsigned long *) invec;
		pullout = (unsigned long *) inoutvec;
#endif
		for (i = 0; i < *len; ++i, ++pullin, ++pullout) {
			*pullout &= *pullin;
		}
		return;
	}

#if LAM_SIZEOF_FORTRAN_INTEGER1
        if (*datatype == MPI_F_INTEGER1) {
            pfint1in = (LAM_FORTRAN_C_TYPE_INTEGER1 *) invec;
            pfint1out = (LAM_FORTRAN_C_TYPE_INTEGER1 *) inoutvec;
            for (i = 0; i < *len; ++i, ++pfint1in, ++pfint1out) {
                *pfint1out &= *pfint1in;
            }
            return;
        }
#endif

#if LAM_SIZEOF_FORTRAN_INTEGER2
        if (*datatype == MPI_F_INTEGER2) {
            pfint2in = (LAM_FORTRAN_C_TYPE_INTEGER2 *) invec;
            pfint2out = (LAM_FORTRAN_C_TYPE_INTEGER2 *) inoutvec;
            for (i = 0; i < *len; ++i, ++pfint2in, ++pfint2out) {
                *pfint2out &= *pfint2in;
            }
            return;
        }
#endif

#if LAM_SIZEOF_FORTRAN_INTEGER4
        if (*datatype == MPI_F_INTEGER4) {
            pfint4in = (LAM_FORTRAN_C_TYPE_INTEGER4 *) invec;
            pfint4out = (LAM_FORTRAN_C_TYPE_INTEGER4 *) inoutvec;
            for (i = 0; i < *len; ++i, ++pfint4in, ++pfint4out) {
                *pfint4out &= *pfint4in;
            }
            return;
        }
#endif

#if LAM_SIZEOF_FORTRAN_INTEGER8
        if (*datatype == MPI_F_INTEGER8) {
            pfint8in = (LAM_FORTRAN_C_TYPE_INTEGER8 *) invec;
            pfint8out = (LAM_FORTRAN_C_TYPE_INTEGER8 *) inoutvec;
            for (i = 0; i < *len; ++i, ++pfint8in, ++pfint8out) {
                *pfint8out &= *pfint8in;
            }
            return;
        }
#endif

	(void) lam_errfunc(MPI_COMM_WORLD,
			   lam_getfunc(), lam_mkerr(MPI_ERR_OP, EINVAL));
}

void
lam_bor(void *invec, void *inoutvec, int *len, MPI_Datatype *datatype)
{
	int		i;
	int		*piin, *piout;
	long		*plin, *plout;
	short		*psin, *psout;
	unsigned short	*pusin, *pusout;
	unsigned	*puin, *puout;
	unsigned long	*pulin, *pulout;
	unsigned char	*pucin, *pucout;
#if LAM_SIZEOF_LONG_LONG
	long long	*pllin, *pllout;
	unsigned long long *pullin, *pullout;
#else
	long		*pllin, *pllout;
	unsigned long 	*pullin, *pullout;
#endif
#if LAM_SIZEOF_FORTRAN_INTEGER1
        LAM_FORTRAN_C_TYPE_INTEGER1 *pfint1in, *pfint1out;
#endif
#if LAM_SIZEOF_FORTRAN_INTEGER2
        LAM_FORTRAN_C_TYPE_INTEGER2 *pfint2in, *pfint2out;
#endif
#if LAM_SIZEOF_FORTRAN_INTEGER4
        LAM_FORTRAN_C_TYPE_INTEGER4 *pfint4in, *pfint4out;
#endif
#if LAM_SIZEOF_FORTRAN_INTEGER8
        LAM_FORTRAN_C_TYPE_INTEGER8 *pfint8in, *pfint8out;
#endif

	if ((*datatype == MPI_INT) || (*datatype == MPI_F_LOGICAL)
			|| (*datatype == MPI_F_INTEGER)) {
		piin = (int *) invec;
		piout = (int *) inoutvec;
		for (i = 0; i < *len; ++i, ++piin, ++piout) {
			*piout |= *piin;
		}
		return;
	}

	if (*datatype == MPI_LONG) {
		plin = (long *) invec;
		plout = (long *) inoutvec;
		for (i = 0; i < *len; ++i, ++plin, ++plout) {
			*plout |= *plin;
		}
		return;
	}

	if (*datatype == MPI_SHORT) {
		psin = (short *) invec;
		psout = (short *) inoutvec;
		for (i = 0; i < *len; ++i, ++psin, ++psout) {
			*psout |= *psin;
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_SHORT) {
		pusin = (unsigned short *) invec;
		pusout = (unsigned short *) inoutvec;
		for (i = 0; i < *len; ++i, ++pusin, ++pusout) {
			*pusout |= *pusin;
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED) {
		puin = (unsigned *) invec;
		puout = (unsigned *) inoutvec;
		for (i = 0; i < *len; ++i, ++puin, ++puout) {
			*puout |= *puin;
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_LONG) {
		pulin = (unsigned long *) invec;
		pulout = (unsigned long *) inoutvec;
		for (i = 0; i < *len; ++i, ++pulin, ++pulout) {
			*pulout |= *pulin;
		}
		return;
	}

	if (*datatype == MPI_BYTE) {
		pucin = (unsigned char *) invec;
		pucout = (unsigned char *) inoutvec;
		for (i = 0; i < *len; ++i, ++pucin, ++pucout) {
			*pucout |= *pucin;
		}
		return;
	}

	if (*datatype == MPI_LONG_LONG_INT) {
#if LAM_SIZEOF_LONG_LONG
		pllin = (long long *) invec;
		pllout = (long long *) inoutvec;
#else
		pllin = (long *) invec;
		pllout = (long *) inoutvec;
#endif
		for (i = 0; i < *len; ++i, ++pllin, ++pllout) {
			*pllout |= *pllin;
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_LONG_LONG) {
#if LAM_SIZEOF_LONG_LONG
		pullin = (unsigned long long *) invec;
		pullout = (unsigned long long *) inoutvec;
#else
		pullin = (unsigned long *) invec;
		pullout = (unsigned long *) inoutvec;
#endif
		for (i = 0; i < *len; ++i, ++pullin, ++pullout) {
			*pullout |= *pullin;
		}
		return;
	}

#if LAM_SIZEOF_FORTRAN_INTEGER1
        if (*datatype == MPI_F_INTEGER1) {
            pfint1in = (LAM_FORTRAN_C_TYPE_INTEGER1 *) invec;
            pfint1out = (LAM_FORTRAN_C_TYPE_INTEGER1 *) inoutvec;
            for (i = 0; i < *len; ++i, ++pfint1in, ++pfint1out) {
                *pfint1out |= *pfint1in;
            }
            return;
        }
#endif

#if LAM_SIZEOF_FORTRAN_INTEGER2
        if (*datatype == MPI_F_INTEGER2) {
            pfint2in = (LAM_FORTRAN_C_TYPE_INTEGER2 *) invec;
            pfint2out = (LAM_FORTRAN_C_TYPE_INTEGER2 *) inoutvec;
            for (i = 0; i < *len; ++i, ++pfint2in, ++pfint2out) {
                *pfint2out |= *pfint2in;
            }
            return;
        }
#endif

#if LAM_SIZEOF_FORTRAN_INTEGER4
        if (*datatype == MPI_F_INTEGER4) {
            pfint4in = (LAM_FORTRAN_C_TYPE_INTEGER4 *) invec;
            pfint4out = (LAM_FORTRAN_C_TYPE_INTEGER4 *) inoutvec;
            for (i = 0; i < *len; ++i, ++pfint4in, ++pfint4out) {
                *pfint4out |= *pfint4in;
            }
            return;
        }
#endif

#if LAM_SIZEOF_FORTRAN_INTEGER8
        if (*datatype == MPI_F_INTEGER8) {
            pfint8in = (LAM_FORTRAN_C_TYPE_INTEGER8 *) invec;
            pfint8out = (LAM_FORTRAN_C_TYPE_INTEGER8 *) inoutvec;
            for (i = 0; i < *len; ++i, ++pfint8in, ++pfint8out) {
                *pfint8out |= *pfint8in;
            }
            return;
        }
#endif

	(void) lam_errfunc(MPI_COMM_WORLD,
			   lam_getfunc(), lam_mkerr(MPI_ERR_OP, EINVAL));
}

void
lam_bxor(void *invec, void *inoutvec, int *len, MPI_Datatype *datatype)
{
	int		i;
	int		*piin, *piout;
	long		*plin, *plout;
	short		*psin, *psout;
	unsigned short	*pusin, *pusout;
	unsigned	*puin, *puout;
	unsigned long	*pulin, *pulout;
	unsigned char	*pucin, *pucout;
#if LAM_SIZEOF_LONG_LONG
	long long	*pllin, *pllout;
	unsigned long long *pullin, *pullout;
#else
	long		*pllin, *pllout;
	unsigned long 	*pullin, *pullout;
#endif
#if LAM_SIZEOF_FORTRAN_INTEGER1
        LAM_FORTRAN_C_TYPE_INTEGER1 *pfint1in, *pfint1out;
#endif
#if LAM_SIZEOF_FORTRAN_INTEGER2
        LAM_FORTRAN_C_TYPE_INTEGER2 *pfint2in, *pfint2out;
#endif
#if LAM_SIZEOF_FORTRAN_INTEGER4
        LAM_FORTRAN_C_TYPE_INTEGER4 *pfint4in, *pfint4out;
#endif
#if LAM_SIZEOF_FORTRAN_INTEGER8
        LAM_FORTRAN_C_TYPE_INTEGER8 *pfint8in, *pfint8out;
#endif

	if ((*datatype == MPI_INT) || (*datatype == MPI_F_LOGICAL)
			|| (*datatype == MPI_F_INTEGER)) {
		piin = (int *) invec;
		piout = (int *) inoutvec;
		for (i = 0; i < *len; ++i, ++piin, ++piout) {
			*piout ^= *piin;
		}
		return;
	}

	if (*datatype == MPI_LONG) {
		plin = (long *) invec;
		plout = (long *) inoutvec;
		for (i = 0; i < *len; ++i, ++plin, ++plout) {
			*plout ^= *plin;
		}
		return;
	}

	if (*datatype == MPI_SHORT) {
		psin = (short *) invec;
		psout = (short *) inoutvec;
		for (i = 0; i < *len; ++i, ++psin, ++psout) {
			*psout ^= *psin;
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_SHORT) {
		pusin = (unsigned short *) invec;
		pusout = (unsigned short *) inoutvec;
		for (i = 0; i < *len; ++i, ++pusin, ++pusout) {
			*pusout ^= *pusin;
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED) {
		puin = (unsigned *) invec;
		puout = (unsigned *) inoutvec;
		for (i = 0; i < *len; ++i, ++puin, ++puout) {
			*puout ^= *puin;
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_LONG) {
		pulin = (unsigned long *) invec;
		pulout = (unsigned long *) inoutvec;
		for (i = 0; i < *len; ++i, ++pulin, ++pulout) {
			*pulout ^= *pulin;
		}
		return;
	}

	if (*datatype == MPI_BYTE) {
		pucin = (unsigned char *) invec;
		pucout = (unsigned char *) inoutvec;
		for (i = 0; i < *len; ++i, ++pucin, ++pucout) {
			*pucout ^= *pucin;
		}
		return;
	}

	if (*datatype == MPI_LONG_LONG_INT) {
#if LAM_SIZEOF_LONG_LONG
		pllin = (long long *) invec;
		pllout = (long long *) inoutvec;
#else
		pllin = (long *) invec;
		pllout = (long *) inoutvec;
#endif
		for (i = 0; i < *len; ++i, ++pllin, ++pllout) {
			*pllout ^= *pllin;
		}
		return;
	}

	if (*datatype == MPI_UNSIGNED_LONG_LONG) {
#if LAM_SIZEOF_LONG_LONG
		pullin = (unsigned long long *) invec;
		pullout = (unsigned long long *) inoutvec;
#else
		pullin = (unsigned long *) invec;
		pullout = (unsigned long *) inoutvec;
#endif
		for (i = 0; i < *len; ++i, ++pullin, ++pullout) {
			*pullout ^= *pullin;
		}
		return;
	}

#if LAM_SIZEOF_FORTRAN_INTEGER1
        if (*datatype == MPI_F_INTEGER1) {
            pfint1in = (LAM_FORTRAN_C_TYPE_INTEGER1 *) invec;
            pfint1out = (LAM_FORTRAN_C_TYPE_INTEGER1 *) inoutvec;
            for (i = 0; i < *len; ++i, ++pfint1in, ++pfint1out) {
                *pfint1out ^= *pfint1in;
            }
            return;
        }
#endif

#if LAM_SIZEOF_FORTRAN_INTEGER2
        if (*datatype == MPI_F_INTEGER2) {
            pfint2in = (LAM_FORTRAN_C_TYPE_INTEGER2 *) invec;
            pfint2out = (LAM_FORTRAN_C_TYPE_INTEGER2 *) inoutvec;
            for (i = 0; i < *len; ++i, ++pfint2in, ++pfint2out) {
                *pfint2out ^= *pfint2in;
            }
            return;
        }
#endif

#if LAM_SIZEOF_FORTRAN_INTEGER4
        if (*datatype == MPI_F_INTEGER4) {
            pfint4in = (LAM_FORTRAN_C_TYPE_INTEGER4 *) invec;
            pfint4out = (LAM_FORTRAN_C_TYPE_INTEGER4 *) inoutvec;
            for (i = 0; i < *len; ++i, ++pfint4in, ++pfint4out) {
                *pfint4out ^= *pfint4in;
            }
            return;
        }
#endif

#if LAM_SIZEOF_FORTRAN_INTEGER8
        if (*datatype == MPI_F_INTEGER8) {
            pfint8in = (LAM_FORTRAN_C_TYPE_INTEGER8 *) invec;
            pfint8out = (LAM_FORTRAN_C_TYPE_INTEGER8 *) inoutvec;
            for (i = 0; i < *len; ++i, ++pfint8in, ++pfint8out) {
                *pfint8out ^= *pfint8in;
            }
            return;
        }
#endif

	(void) lam_errfunc(MPI_COMM_WORLD,
			   lam_getfunc(), lam_mkerr(MPI_ERR_OP, EINVAL));
}

void
lam_maxloc(void *invec, void *inoutvec, int *len, MPI_Datatype *datatype)
{
	int		i;
	struct fltflt	*pffin, *pffout;
	struct fltint	*pfiin, *pfiout;
	struct dbldbl	*pddin, *pddout;
	struct dblint	*pdiin, *pdiout;
	struct ldblint	*pldiin, *pldiout;
	struct intint	*piiin, *piiout;
	struct longint	*pliin, *pliout;
	struct shortint	*psiin, *psiout;

	if (*datatype == MPI_F_2REAL) {
		pffin = (struct fltflt *) invec;
		pffout = (struct fltflt *) inoutvec;
		for (i = 0; i < *len; ++i, ++pffin, ++pffout) {
			if (pffin->v > pffout->v) {
				pffout->v = pffin->v;
				pffout->k = pffin->k;
			}
			else if (pffin->v == pffout->v) {
				pffout->k = LAM_min(pffin->k, pffout->k);
			}
		}
		return;
	}

	if (*datatype == MPI_FLOAT_INT) {
		pfiin = (struct fltint *) invec;
		pfiout = (struct fltint *) inoutvec;
		for (i = 0; i < *len; ++i, ++pfiin, ++pfiout) {
			if (pfiin->v > pfiout->v) {
				pfiout->v = pfiin->v;
				pfiout->k = pfiin->k;
			}
			else if (pfiin->v == pfiout->v) {
				pfiout->k = LAM_min(pfiin->k, pfiout->k);
			}
		}
		return;
	}

	if (*datatype == MPI_F_2DOUBLE_PRECISION) {
		pddin = (struct dbldbl *) invec;
		pddout = (struct dbldbl *) inoutvec;
		for (i = 0; i < *len; ++i, ++pddin, ++pddout) {
			if (pddin->v > pddout->v) {
				pddout->v = pddin->v;
				pddout->k = pddin->k;
			}
			else if (pddin->v == pddout->v) {
				pddout->k = LAM_min(pddin->k, pddout->k);
			}
		}
		return;
	}

	if (*datatype == MPI_DOUBLE_INT) {
		pdiin = (struct dblint *) invec;
		pdiout = (struct dblint *) inoutvec;
		for (i = 0; i < *len; ++i, ++pdiin, ++pdiout) {
			if (pdiin->v > pdiout->v) {
				pdiout->v = pdiin->v;
				pdiout->k = pdiin->k;
			}
			else if (pdiin->v == pdiout->v) {
				pdiout->k = LAM_min(pdiin->k, pdiout->k);
			}
		}
		return;
	}

	if (*datatype == MPI_LONG_DOUBLE_INT) {
		pldiin = (struct ldblint *) invec;
		pldiout = (struct ldblint *) inoutvec;
		for (i = 0; i < *len; ++i, ++pldiin, ++pldiout) {
			if (pldiin->v > pldiout->v) {
				pldiout->v = pldiin->v;
				pldiout->k = pldiin->k;
			}
			else if (pldiin->v == pldiout->v) {
				pldiout->k = LAM_min(pldiin->k, pldiout->k);
			}
		}
		return;
	}

	if (*datatype == MPI_2INT || *datatype == MPI_F_2INTEGER) {
		piiin = (struct intint *) invec;
		piiout = (struct intint *) inoutvec;
		for (i = 0; i < *len; ++i, ++piiin, ++piiout) {
			if (piiin->v > piiout->v) {
				piiout->v = piiin->v;
				piiout->k = piiin->k;
			}
			else if (piiin->v == piiout->v) {
				piiout->k = LAM_min(piiin->k, piiout->k);
			}
		}
		return;
	}

	if (*datatype == MPI_LONG_INT) {
		pliin = (struct longint *) invec;
		pliout = (struct longint *) inoutvec;
		for (i = 0; i < *len; ++i, ++pliin, ++pliout) {
			if (pliin->v > pliout->v) {
				pliout->v = pliin->v;
				pliout->k = pliin->k;
			}
			else if (pliin->v == pliout->v) {
				pliout->k = LAM_min(pliin->k, pliout->k);
			}
		}
		return;
	}
	
	if (*datatype == MPI_SHORT_INT) {
		psiin = (struct shortint *) invec;
		psiout = (struct shortint *) inoutvec;
		for (i = 0; i < *len; ++i, ++psiin, ++psiout) {
			if (psiin->v > psiout->v) {
				psiout->v = psiin->v;
				psiout->k = psiin->k;
			}
			else if (psiin->v == psiout->v) {
				psiout->k = LAM_min(psiin->k, psiout->k);
			}
		}
		return;
	}

	(void) lam_errfunc(MPI_COMM_WORLD,
			   lam_getfunc(), lam_mkerr(MPI_ERR_OP, EINVAL));
}


void
lam_minloc(void *invec, void *inoutvec, int *len, MPI_Datatype *datatype)
{
	int		i;
	struct fltflt	*pffin, *pffout;
	struct fltint	*pfiin, *pfiout;
	struct dbldbl	*pddin, *pddout;
	struct dblint	*pdiin, *pdiout;
	struct ldblint	*pldiin, *pldiout;
	struct intint	*piiin, *piiout;
	struct longint	*pliin, *pliout;
	struct shortint	*psiin, *psiout;

	if (*datatype == MPI_F_2REAL) {
		pffin = (struct fltflt *) invec;
		pffout = (struct fltflt *) inoutvec;
		for (i = 0; i < *len; ++i, ++pffin, ++pffout) {
			if (pffin->v < pffout->v) {
				pffout->v = pffin->v;
				pffout->k = pffin->k;
			}
			else if (pffin->v == pffout->v) {
				pffout->k = LAM_min(pffin->k, pffout->k);
			}
		}
		return;
	}

	if (*datatype == MPI_FLOAT_INT) {
		pfiin = (struct fltint *) invec;
		pfiout = (struct fltint *) inoutvec;
		for (i = 0; i < *len; ++i, ++pfiin, ++pfiout) {
			if (pfiin->v < pfiout->v) {
				pfiout->v = pfiin->v;
				pfiout->k = pfiin->k;
			}
			else if (pfiin->v == pfiout->v) {
				pfiout->k = LAM_min(pfiin->k, pfiout->k);
			}
		}
		return;
	}

	if (*datatype == MPI_F_2DOUBLE_PRECISION) {
		pddin = (struct dbldbl *) invec;
		pddout = (struct dbldbl *) inoutvec;
		for (i = 0; i < *len; ++i, ++pddin, ++pddout) {
			if (pddin->v < pddout->v) {
				pddout->v = pddin->v;
				pddout->k = pddin->k;
			}
			else if (pddin->v == pddout->v) {
				pddout->k = LAM_min(pddin->k, pddout->k);
			}
		}
		return;
	}

	if (*datatype == MPI_DOUBLE_INT) {
		pdiin = (struct dblint *) invec;
		pdiout = (struct dblint *) inoutvec;
		for (i = 0; i < *len; ++i, ++pdiin, ++pdiout) {
			if (pdiin->v < pdiout->v) {
				pdiout->v = pdiin->v;
				pdiout->k = pdiin->k;
			}
			else if (pdiin->v == pdiout->v) {
				pdiout->k = LAM_min(pdiin->k, pdiout->k);
			}
		}
		return;
	}

	if (*datatype == MPI_LONG_DOUBLE_INT) {
		pldiin = (struct ldblint *) invec;
		pldiout = (struct ldblint *) inoutvec;
		for (i = 0; i < *len; ++i, ++pldiin, ++pldiout) {
			if (pldiin->v < pldiout->v) {
				pldiout->v = pldiin->v;
				pldiout->k = pldiin->k;
			}
			else if (pldiin->v == pldiout->v) {
				pldiout->k = LAM_min(pldiin->k, pldiout->k);
			}
		}
		return;
	}

	if (*datatype == MPI_2INT || *datatype == MPI_F_2INTEGER) {
		piiin = (struct intint *) invec;
		piiout = (struct intint *) inoutvec;
		for (i = 0; i < *len; ++i, ++piiin, ++piiout) {
			if (piiin->v < piiout->v) {
				piiout->v = piiin->v;
				piiout->k = piiin->k;
			}
			else if (piiin->v == piiout->v) {
				piiout->k = LAM_min(piiin->k, piiout->k);
			}
		}
		return;
	}

	if (*datatype == MPI_LONG_INT) {
		pliin = (struct longint *) invec;
		pliout = (struct longint *) inoutvec;
		for (i = 0; i < *len; ++i, ++pliin, ++pliout) {
			if (pliin->v < pliout->v) {
				pliout->v = pliin->v;
				pliout->k = pliin->k;
			}
			else if (pliin->v == pliout->v) {
				pliout->k = LAM_min(pliin->k, pliout->k);
			}
		}
		return;
	}

	if (*datatype == MPI_SHORT_INT) {
		psiin = (struct shortint *) invec;
		psiout = (struct shortint *) inoutvec;
		for (i = 0; i < *len; ++i, ++psiin, ++psiout) {
			if (psiin->v < psiout->v) {
				psiout->v = psiin->v;
				psiout->k = psiin->k;
			}
			else if (psiin->v == psiout->v) {
				psiout->k = LAM_min(psiin->k, psiout->k);
			}
		}
		return;
	}

	(void) lam_errfunc(MPI_COMM_WORLD,
			   lam_getfunc(), lam_mkerr(MPI_ERR_OP, EINVAL));
}

void
lam_replace(void *invec, void *inoutvec, int *len, MPI_Datatype *datatype)
{
	if (*datatype == MPI_INT
		|| *datatype == MPI_F_INTEGER
		|| *datatype == MPI_F_INTEGER1
		|| *datatype == MPI_F_INTEGER2
		|| *datatype == MPI_F_INTEGER4
		|| *datatype == MPI_F_INTEGER8
		|| *datatype == MPI_LONG
		|| *datatype == MPI_BYTE
		|| *datatype == MPI_UNSIGNED_CHAR
		|| *datatype == MPI_SHORT
		|| *datatype == MPI_UNSIGNED_SHORT
		|| *datatype == MPI_UNSIGNED
		|| *datatype == MPI_UNSIGNED_LONG
		|| *datatype == MPI_FLOAT
		|| *datatype == MPI_F_REAL
		|| *datatype == MPI_F_REAL4
		|| *datatype == MPI_F_REAL8
		|| *datatype == MPI_F_REAL16
		|| *datatype == MPI_DOUBLE
		|| *datatype == MPI_F_DOUBLE_PRECISION
		|| *datatype == MPI_LONG_DOUBLE
		|| *datatype == MPI_LONG_LONG_INT
		|| *datatype == MPI_UNSIGNED_LONG_LONG) {

		lam_memcpy((char *) inoutvec,
			   (char *) invec, *len * (*datatype)->dt_size);

		return;
	}

	(void) lam_errfunc(MPI_COMM_WORLD,
			   lam_getfunc(), lam_mkerr(MPI_ERR_OP, EINVAL));
}
