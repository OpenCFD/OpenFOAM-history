/* -*- Mode: C; c-basic-offset:4 ; -*- */
/* 
 *   $Id: darrayf.c,v 1.3 2003/03/25 22:41:21 nsanghvi Exp $    
 *
 *   Copyright (C) 1997 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "mpio.h"
#include "adio.h"


#if defined(MPIO_BUILD_PROFILING) || defined(HAVE_WEAK_SYMBOLS)
#ifdef FORTRANCAPS
#define mpi_type_create_darray_ PMPI_TYPE_CREATE_DARRAY
#elif defined(FORTRANDOUBLEUNDERSCORE)
#define mpi_type_create_darray_ pmpi_type_create_darray__
#elif !defined(FORTRANUNDERSCORE)
#if defined(HPUX) || defined(SPPUX)
#pragma _HP_SECONDARY_DEF pmpi_type_create_darray pmpi_type_create_darray_
#endif
#define mpi_type_create_darray_ pmpi_type_create_darray
#else
#if defined(HPUX) || defined(SPPUX)
#pragma _HP_SECONDARY_DEF pmpi_type_create_darray_ pmpi_type_create_darray
#endif
#define mpi_type_create_darray_ pmpi_type_create_darray_
#endif

#if defined(HAVE_WEAK_SYMBOLS)
#if defined(HAVE_PRAGMA_WEAK)
#if defined(FORTRANCAPS)
#pragma weak MPI_TYPE_CREATE_DARRAY = PMPI_TYPE_CREATE_DARRAY
#elif defined(FORTRANDOUBLEUNDERSCORE)
#pragma weak mpi_type_create_darray__ = pmpi_type_create_darray__
#elif !defined(FORTRANUNDERSCORE)
#pragma weak mpi_type_create_darray = pmpi_type_create_darray
#else
#pragma weak mpi_type_create_darray_ = pmpi_type_create_darray_
#endif

#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#if defined(FORTRANCAPS)
#pragma _HP_SECONDARY_DEF PMPI_TYPE_CREATE_DARRAY MPI_TYPE_CREATE_DARRAY
#elif defined(FORTRANDOUBLEUNDERSCORE)
#pragma _HP_SECONDARY_DEF pmpi_type_create_darray__ mpi_type_create_darray__
#elif !defined(FORTRANUNDERSCORE)
#pragma _HP_SECONDARY_DEF pmpi_type_create_darray mpi_type_create_darray
#else
#pragma _HP_SECONDARY_DEF pmpi_type_create_darray_ mpi_type_create_darray_
#endif

#elif defined(HAVE_PRAGMA_CRI_DUP)
#if defined(FORTRANCAPS)
#pragma _CRI duplicate MPI_TYPE_CREATE_DARRAY as PMPI_TYPE_CREATE_DARRAY
#elif defined(FORTRANDOUBLEUNDERSCORE)
#pragma _CRI duplicate mpi_type_create_darray__ as pmpi_type_create_darray__
#elif !defined(FORTRANUNDERSCORE)
#pragma _CRI duplicate mpi_type_create_darray as pmpi_type_create_darray
#else
#pragma _CRI duplicate mpi_type_create_darray_ as pmpi_type_create_darray_
#endif

/* end of weak pragmas */
#endif
/* Include mapping from MPI->PMPI */
#include "mpioprof.h"
#endif

#else

#ifdef FORTRANCAPS
#define mpi_type_create_darray_ MPI_TYPE_CREATE_DARRAY
#elif defined(FORTRANDOUBLEUNDERSCORE)
#define mpi_type_create_darray_ mpi_type_create_darray__
#elif !defined(FORTRANUNDERSCORE)
#if defined(HPUX) || defined(SPPUX)
#pragma _HP_SECONDARY_DEF mpi_type_create_darray mpi_type_create_darray_
#endif
#define mpi_type_create_darray_ mpi_type_create_darray
#else
#if defined(HPUX) || defined(SPPUX)
#pragma _HP_SECONDARY_DEF mpi_type_create_darray_ mpi_type_create_darray
#endif
#endif
#endif

#ifdef MPIHP
void mpi_type_create_darray_(int *size,int *rank,int *ndims,
                           int *array_of_gsizes,int *array_of_distribs,
                           int *array_of_dargs,int *array_of_psizes,
                           int *order, MPI_Fint *oldtype,
                           MPI_Fint *newtype, int *ierr )
{
    MPI_Datatype oldtype_c, newtype_c;

    oldtype_c = MPI_Type_f2c(*oldtype);

    *ierr = MPI_Type_create_darray(*size,*rank,*ndims,array_of_gsizes,array_of_distribs,array_of_dargs,array_of_psizes,*order,oldtype_c,&newtype_c);

    *newtype = MPI_Type_c2f(newtype_c);
}

#else

void mpi_type_create_darray_(int *size,int *rank,int *ndims,
                         int *array_of_gsizes,int *array_of_distribs,
                           int *array_of_dargs,int *array_of_psizes,
                           int *order,MPI_Datatype *oldtype,
                           MPI_Datatype *newtype, int *ierr )
{
    *ierr = MPI_Type_create_darray(*size,*rank,*ndims,array_of_gsizes,array_of_distribs,array_of_dargs,array_of_psizes,*order,*oldtype,newtype);
}
#endif
