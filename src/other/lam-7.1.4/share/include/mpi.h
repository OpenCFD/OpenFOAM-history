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
 *	$Id: mpi.h,v 6.41 2004/03/09 16:49:39 adharurk Exp $
 *
 *	Function:	- MPI header file
 */

#ifndef _MPI_H
#define _MPI_H

#include <lam_config.h>

/*
 * Just in case you need it.  :-)
 */
#define LAM_MPI                 1

/*
 * MPI version
 */
#define MPI_VERSION		1
#define MPI_SUBVERSION		2

/*
 * typedefs
 */
typedef struct _status		MPI_Status;
typedef struct _group		*MPI_Group;
typedef struct _comm		*MPI_Comm;
typedef struct _dtype		*MPI_Datatype;
typedef struct _req		*MPI_Request;
typedef struct _op		*MPI_Op;
typedef struct _errhdl		*MPI_Errhandler;
typedef struct _info		*MPI_Info;
typedef struct _window		*MPI_Win;
typedef void			*MPI_Buffer;
typedef long			MPI_Aint;
typedef int			MPI_Fint;

/*
 * user typedefs
 */
#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif
#if __STDC__ || defined(c_plusplus) || defined(__cplusplus)
typedef int	(MPI_Copy_function)(MPI_Comm, int, void *,
					void *, void *, int *);
typedef int	(MPI_Delete_function)(MPI_Comm, int, void *, void *);
typedef void	(MPI_Comm_errhandler_fn)(MPI_Comm *, int *, ...);
typedef void	(MPI_Win_errhandler_fn)(MPI_Win *, int *, ...);
typedef void	(MPI_Handler_function)(MPI_Comm *, int *, ...);
typedef void	(MPI_User_function)(void *, void *, int *, MPI_Datatype *);
typedef int	(MPI_Comm_copy_attr_function)(MPI_Comm, int, void *,
						void *, void *, int *);
typedef int	(MPI_Comm_delete_attr_function)(MPI_Comm, int, void *, void *);
typedef int	(MPI_Type_copy_attr_function)(MPI_Datatype, int, void *,
						void *, void *, int *);
typedef int	(MPI_Type_delete_attr_function)(MPI_Datatype, int,
    						void *, void *);
typedef int	(MPI_Win_copy_attr_function)(MPI_Win, int, void *,
						void *, void *, int *);
typedef int	(MPI_Win_delete_attr_function)(MPI_Win, int, void *, void *);
#else
typedef int	(MPI_Copy_function)();
typedef int	(MPI_Delete_function)();
typedef void	(MPI_Comm_errhandler_fn)();
typedef void	(MPI_Win_errhandler_fn)();
typedef void	(MPI_Handler_function)();
typedef void	(MPI_User_function)();
typedef int	(MPI_Comm_copy_attr_function)();
typedef int	(MPI_Comm_delete_attr_function)();
typedef int	(MPI_Type_copy_attr_function)();
typedef int	(MPI_Type_delete_attr_function)();
typedef int	(MPI_Win_copy_attr_function)();
typedef int	(MPI_Win_delete_attr_function)();
#endif
#if defined(c_plusplus) || defined(__cplusplus)
}
#endif

/*
 * misc. constants
 */
#define MPI_SUCCESS		0		/* no errors */
#define MPI_ANY_SOURCE		-1		/* match any source rank */
#define MPI_PROC_NULL		-2		/* rank of null process */
#define MPI_CANCEL_SOURCE	-3		/* successful cancel */
#define MPI_ROOT                -4
#define MPI_ANY_TAG		-1		/* match any message tag */
#define MPI_GER_TAG		-2		/* used for GER protocol */
#define MPI_MERGE_TAG		-3		/* used to merge inter-comm */
#define MPI_MAX_PROCESSOR_NAME	256		/* max proc. name length */
#define MPI_MAX_ERROR_STRING	256		/* max error message length */
#define MPI_MAX_OBJECT_NAME	64		/* max object name length */
#define MPI_UNDEFINED		-32766		/* undefined stuff */
#define MPI_CART		1		/* cartesian topology */
#define MPI_GRAPH		2		/* graph topology */
#define MPI_KEYVAL_INVALID	-1		/* invalid key value */

/*
 * one-sided constants
 */
#define MPI_MODE_NOCHECK             0
#define MPI_MODE_NOPRECEDE           0
#define MPI_MODE_NOPUT               0
#define MPI_MODE_NOSTORE             0
#define MPI_MODE_NOSUCCEED           0

/*
 * Predefined attribute keyvals
 */
#define MPI_TAG_UB		0		/* predefined attribute key */
#define MPI_HOST		1		/* predefined attribute key */
#define MPI_IO			2		/* predefined attribute key */
#define MPI_WTIME_IS_GLOBAL	3		/* predefined attribute key */
#define MPI_UNIVERSE_SIZE	4		/* predefined attribute key */
#define MPI_APPNUM		5		/* predefined attribute key */
#define MPI_WIN_BASE		6		/* predefined attribute key */
#define MPI_WIN_SIZE		7		/* predefined attribute key */
#define MPI_WIN_DISP_UNIT	8		/* predefined attribute key */

/* 
 * Even though these four are IMPI attributes, they need to be there
 * for all MPI jobs
 */
#define IMPI_CLIENT_SIZE        9               /* predefined attribute key */
#define IMPI_CLIENT_COLOR       10              /* predefined attribute key */
#define IMPI_HOST_SIZE          11              /* predefined attribute key */
#define IMPI_HOST_COLOR         12              /* predefined attribute key */

/*
 * Predefined attribute keyvals, but LAM-specific
 */
#define LAM_UNIVERSE_NCPUS	13		/* predefined attribute key */
#define LAM_UNIVERSE_NNODES	14		/* predefined attribute key */

/*
 * Predefined attribute keyvals, but LAM/MPI-specific
 */
#define LAM_MPI_SSI_COLL                    15
#define LAM_MPI_SSI_COLL_CROSSOVER          16
#define LAM_MPI_SSI_COLL_ASSOCIATIVE        17
#define LAM_MPI_SSI_COLL_REDUCE_CROSSOVER   18
#define LAM_MPI_SSI_COLL_SHMEM_MESSAGE_POOL_SIZE 19
#define LAM_MPI_SSI_COLL_SHMEM_NUM_SEGMENTS 20


#define MPI_BOTTOM		((void *) 0)	/* base reference address */
#define MPI_BSEND_OVERHEAD	40		/* size of bsend header + ptr */
#define MPI_MAX_INFO_KEY	36		/* max info key length */
#define MPI_MAX_INFO_VAL	256		/* max info value length */
#define MPI_ARGV_NULL		((char **) 0)	/* NULL argument vector */
#define MPI_ARGVS_NULL		((char ***) 0)	/* NULL argument vectors */
#define MPI_ERRCODES_IGNORE	((int *) 0)	/* don't return error codes */
#define MPI_MAX_PORT_NAME       36              /* max port name length */
#define MPI_MAX_NAME_LEN	MPI_MAX_PORT_NAME /* max port name length */
#define MPI_ORDER_C		0		/* C row major order */
#define MPI_ORDER_FORTRAN	1		/* Fortran column major order */
#define MPI_DISTRIBUTE_BLOCK	0		/* block distribution */
#define MPI_DISTRIBUTE_CYCLIC	1		/* cyclic distribution */
#define MPI_DISTRIBUTE_NONE     2		/* not distributed */
#define MPI_DISTRIBUTE_DFLT_DARG (-1)		/* default distribution arg */

/*
 * error classes
 */
#define MPI_ERR_BUFFER		1	/* invalid buffer pointer */
#define MPI_ERR_COUNT		2	/* invalid count argument */
#define MPI_ERR_TYPE		3	/* invalid datatype argument */
#define MPI_ERR_TAG		4	/* invalid tag argument */
#define MPI_ERR_COMM		5	/* invalid communicator */
#define MPI_ERR_RANK		6	/* invalid rank */
#define MPI_ERR_REQUEST		7	/* invalid request handle */
#define MPI_ERR_ROOT		8	/* invalid root */
#define MPI_ERR_GROUP		9	/* invalid group */
#define MPI_ERR_OP		10	/* invalid operation */
#define MPI_ERR_TOPOLOGY	11	/* invalid topology */
#define MPI_ERR_DIMS		12	/* invalid dimension argument */
#define MPI_ERR_ARG		13	/* invalid argument */
#define MPI_ERR_UNKNOWN		14	/* unknown error */
#define MPI_ERR_TRUNCATE	15	/* message truncated on receive */
#define MPI_ERR_OTHER		16	/* LAM error */
#define MPI_ERR_INTERN		17	/* internal MPI error */
#define MPI_ERR_IN_STATUS	18	/* error code is in status */
#define MPI_ERR_PENDING		19	/* pending request */
#define MPI_ERR_SYSRESOURCE	20	/* out of system resources */
#define MPI_ERR_LOCALDEAD	21	/* process in local group is dead */
#define MPI_ERR_REMOTEDEAD	22	/* process in remote group is dead */
#define MPI_ERR_VALUE		23	/* truncated info value */
#define MPI_ERR_FLAGS		24	/* mismatched run-time flags */
#define MPI_ERR_SERVICE		25	/* name publishing service error */
#define MPI_ERR_NAME		26	/* name not published */
#define MPI_ERR_SPAWN		27	/* error while spawning processes */
#define MPI_ERR_KEYVAL		28	/* invalid key value */
#define MPI_ERR_INFO_NOKEY	29	/* no such info key */
#define MPI_ERR_WIN		30	/* invalid window */
#define MPI_ERR_EPOCH		31	/* mismatched one-sided synch. */
#define MPI_ERR_TYPENOTSUP	32	/* operation not supported on type */
#define MPI_ERR_INFO_KEY	33	/* invalid info key */
#define MPI_ERR_INFO_VALUE	34	/* invalid info value */
#define MPI_ERR_NO_MEM		35	/* no memory left */
#define MPI_ERR_BASE		36	/* invalid base pointer value */
#define MPI_ERR_LASTCODE	37	/* last error code */

/*
 * Comparison results.  Don't change the order of these, the group
 * comparison functions rely on it.  
 */
#define MPI_IDENT		1
#define MPI_CONGRUENT		2
#define MPI_SIMILAR		3
#define MPI_UNEQUAL		4

/*
 * NULL handles
 */

/* These need to be here - see note near #undefs in MPISYSF.h */
#define LAM_MPI_C_GROUP_NULL	((MPI_Group) 0)
#define LAM_MPI_C_COMM_NULL	((MPI_Comm) 0)
#define LAM_MPI_C_DATATYPE_NULL	((MPI_Datatype) 0)
#define LAM_MPI_C_REQUEST_NULL	((MPI_Request) 0)
#define LAM_MPI_C_OP_NULL	((MPI_Op) 0)
#define LAM_MPI_C_ERRHANDLER_NULL	((MPI_Errhandler) 0)
#define LAM_MPI_C_NULL_COPY_FN	        ((MPI_Copy_function *) 0)
#define LAM_MPI_C_NULL_DELETE_FN	((MPI_Delete_function *) 0)
#define LAM_MPI_C_COMM_NULL_COPY_FN	((MPI_Comm_copy_attr_function *) 0)
#define LAM_MPI_C_COMM_NULL_DELETE_FN	((MPI_Comm_delete_attr_function *) 0)
#define LAM_MPI_C_TYPE_NULL_COPY_FN	((MPI_Type_copy_attr_function *) 0)
#define LAM_MPI_C_TYPE_NULL_DELETE_FN	((MPI_Type_delete_attr_function *) 0)
#define LAM_MPI_C_WIN_NULL_COPY_FN	((MPI_Win_copy_attr_function *) 0)
#define LAM_MPI_C_WIN_NULL_DELETE_FN	((MPI_Win_delete_attr_function *) 0)
#define LAM_MPI_C_INFO_NULL	((MPI_Info) 0)
#define LAM_MPI_C_WIN_NULL	((MPI_Win) 0)

#define LAM_MPI_C_STATUSES_IGNORE	((MPI_Status *) 0)
#define LAM_MPI_C_STATUS_IGNORE	((MPI_Status *) 0)

#define MPI_GROUP_NULL          LAM_MPI_C_GROUP_NULL
#define MPI_COMM_NULL           LAM_MPI_C_COMM_NULL
#define MPI_DATATYPE_NULL       LAM_MPI_C_DATATYPE_NULL
#define MPI_REQUEST_NULL        LAM_MPI_C_REQUEST_NULL
#define MPI_OP_NULL             LAM_MPI_C_OP_NULL
#define MPI_ERRHANDLER_NULL     LAM_MPI_C_ERRHANDLER_NULL
#define MPI_NULL_COPY_FN        LAM_MPI_C_NULL_COPY_FN
#define MPI_NULL_DELETE_FN      LAM_MPI_C_NULL_DELETE_FN
#define MPI_COMM_NULL_COPY_FN   LAM_MPI_C_COMM_NULL_COPY_FN
#define MPI_COMM_NULL_DELETE_FN LAM_MPI_C_COMM_NULL_DELETE_FN
#define MPI_TYPE_NULL_COPY_FN   LAM_MPI_C_TYPE_NULL_COPY_FN
#define MPI_TYPE_NULL_DELETE_FN LAM_MPI_C_TYPE_NULL_DELETE_FN
#define MPI_WIN_NULL_COPY_FN    LAM_MPI_C_WIN_NULL_COPY_FN
#define MPI_WIN_NULL_DELETE_FN  LAM_MPI_C_WIN_NULL_DELETE_FN
#define MPI_INFO_NULL           LAM_MPI_C_INFO_NULL
#define MPI_WIN_NULL            LAM_MPI_C_WIN_NULL

#define MPI_STATUSES_IGNORE     LAM_MPI_C_STATUSES_IGNORE
#define MPI_STATUS_IGNORE       LAM_MPI_C_STATUS_IGNORE


/*
 * Datatype combiners.
 */
#define MPI_COMBINER_NAMED		0
#define MPI_COMBINER_DUP		1
#define MPI_COMBINER_CONTIGUOUS		2
#define MPI_COMBINER_VECTOR		3
#define MPI_COMBINER_HVECTOR_INTEGER	4
#define MPI_COMBINER_HVECTOR		5
#define MPI_COMBINER_INDEXED		6
#define MPI_COMBINER_HINDEXED_INTEGER	7
#define MPI_COMBINER_HINDEXED		8
#define MPI_COMBINER_INDEXED_BLOCK	9
#define MPI_COMBINER_STRUCT_INTEGER	10
#define MPI_COMBINER_STRUCT		11
#define MPI_COMBINER_SUBARRAY		12
#define MPI_COMBINER_DARRAY		13
#define MPI_COMBINER_F90_REAL		14
#define MPI_COMBINER_F90_COMPLEX	15
#define MPI_COMBINER_F90_INTEGER	16
#define MPI_COMBINER_RESIZED		17

/*
 * MPI_Init_thread constants
 */
#define MPI_THREAD_SINGLE		0
#define MPI_THREAD_FUNNELED		1
#define MPI_THREAD_SERIALIZED		2
#define MPI_THREAD_MULTIPLE		3

/*
 * Transparent MPI data type.
 */
struct _status { 
	int		MPI_SOURCE;
	int		MPI_TAG;
	int		MPI_ERROR;
	int		st_length;		/* message length */
};

/*
 * external variables
 */
extern struct _comm		lam_mpi_comm_world;
extern struct _comm		lam_mpi_comm_self;

extern struct _group		lam_mpi_group_empty;

extern struct _op		lam_mpi_max, lam_mpi_min;
extern struct _op		lam_mpi_sum, lam_mpi_prod;
extern struct _op		lam_mpi_land, lam_mpi_band;
extern struct _op		lam_mpi_lor, lam_mpi_bor;
extern struct _op		lam_mpi_lxor, lam_mpi_bxor;
extern struct _op		lam_mpi_maxloc, lam_mpi_minloc;
extern struct _op		lam_mpi_replace;

extern struct _dtype		lam_mpi_char, lam_mpi_byte;
extern struct _dtype		lam_mpi_int, lam_mpi_logic;
extern struct _dtype		lam_mpi_short, lam_mpi_long;
extern struct _dtype		lam_mpi_float, lam_mpi_double;
extern struct _dtype		lam_mpi_long_double;
extern struct _dtype		lam_mpi_cplex, lam_mpi_packed;
extern struct _dtype		lam_mpi_unsigned_char;
extern struct _dtype		lam_mpi_unsigned_short;
extern struct _dtype		lam_mpi_unsigned;
extern struct _dtype		lam_mpi_unsigned_long;
extern struct _dtype		lam_mpi_ub, lam_mpi_lb;
extern struct _dtype		lam_mpi_float_int, lam_mpi_double_int;
extern struct _dtype		lam_mpi_long_int, lam_mpi_2int;
extern struct _dtype		lam_mpi_short_int, lam_mpi_dblcplex;
extern struct _dtype		lam_mpi_integer, lam_mpi_real;
extern struct _dtype            lam_mpi_integer1, lam_mpi_integer2;
extern struct _dtype            lam_mpi_integer4, lam_mpi_integer8;
extern struct _dtype            lam_mpi_real4, lam_mpi_real8, lam_mpi_real16;
extern struct _dtype		lam_mpi_dblprec, lam_mpi_character;
extern struct _dtype		lam_mpi_2real, lam_mpi_2dblprec;
extern struct _dtype		lam_mpi_2integer, lam_mpi_longdbl_int;
extern struct _dtype		lam_mpi_wchar, lam_mpi_long_long_int;
extern struct _dtype		lam_mpi_unsigned_long_long;
extern struct _dtype            lam_mpi_cxx_cplex, lam_mpi_cxx_dblcplex;
extern struct _dtype            lam_mpi_cxx_ldblcplex;
extern struct _dtype            lam_mpi_cxx_bool;

extern struct _errhdl		lam_mpi_errors_are_fatal;
extern struct _errhdl		lam_mpi_errors_return;

extern MPI_Fint			*MPI_F_STATUS_IGNORE;
extern MPI_Fint			*MPI_F_STATUSES_IGNORE;

/*
 * MPI predefined handles
 */
#define MPI_COMM_WORLD		((MPI_Comm) &lam_mpi_comm_world)
#define MPI_COMM_SELF		((MPI_Comm) &lam_mpi_comm_self)

#define MPI_GROUP_EMPTY		((MPI_Group) &lam_mpi_group_empty)

#define MPI_MAX			((MPI_Op) &lam_mpi_max)
#define MPI_MIN			((MPI_Op) &lam_mpi_min)
#define MPI_SUM			((MPI_Op) &lam_mpi_sum)
#define MPI_PROD		((MPI_Op) &lam_mpi_prod)
#define MPI_LAND		((MPI_Op) &lam_mpi_land)
#define MPI_BAND		((MPI_Op) &lam_mpi_band)
#define MPI_LOR			((MPI_Op) &lam_mpi_lor)
#define MPI_BOR			((MPI_Op) &lam_mpi_bor)
#define MPI_LXOR		((MPI_Op) &lam_mpi_lxor)
#define MPI_BXOR		((MPI_Op) &lam_mpi_bxor)
#define MPI_MAXLOC		((MPI_Op) &lam_mpi_maxloc)
#define MPI_MINLOC		((MPI_Op) &lam_mpi_minloc)
#define MPI_REPLACE		((MPI_Op) &lam_mpi_replace)

#define MPI_BYTE		((MPI_Datatype) &lam_mpi_byte)
#define MPI_PACKED		((MPI_Datatype) &lam_mpi_packed)
#define MPI_CHAR		((MPI_Datatype) &lam_mpi_char)
#define MPI_SHORT		((MPI_Datatype) &lam_mpi_short)
#define MPI_INT			((MPI_Datatype) &lam_mpi_int)
#define MPI_LONG		((MPI_Datatype) &lam_mpi_long)
#define MPI_FLOAT		((MPI_Datatype) &lam_mpi_float)
#define MPI_DOUBLE		((MPI_Datatype) &lam_mpi_double)
#define MPI_LONG_DOUBLE		((MPI_Datatype) &lam_mpi_long_double)
#define MPI_UNSIGNED_CHAR	((MPI_Datatype) &lam_mpi_unsigned_char)
#define MPI_UNSIGNED_SHORT	((MPI_Datatype) &lam_mpi_unsigned_short)
#define MPI_UNSIGNED_LONG	((MPI_Datatype) &lam_mpi_unsigned_long)
#define MPI_UNSIGNED		((MPI_Datatype) &lam_mpi_unsigned)
#define MPI_FLOAT_INT		((MPI_Datatype) &lam_mpi_float_int)
#define MPI_DOUBLE_INT		((MPI_Datatype) &lam_mpi_double_int)
#define MPI_LONG_DOUBLE_INT	((MPI_Datatype) &lam_mpi_longdbl_int)
#define MPI_LONG_INT		((MPI_Datatype) &lam_mpi_long_int)
#define MPI_SHORT_INT		((MPI_Datatype) &lam_mpi_short_int)
#define MPI_2INT		((MPI_Datatype) &lam_mpi_2int)
#define MPI_UB			((MPI_Datatype) &lam_mpi_ub)
#define MPI_LB			((MPI_Datatype) &lam_mpi_lb)
#define MPI_WCHAR		((MPI_Datatype) &lam_mpi_wchar)
#define MPI_LONG_LONG		((MPI_Datatype) &lam_mpi_long_long_int)
#define MPI_LONG_LONG_INT	((MPI_Datatype) &lam_mpi_long_long_int)
#define MPI_UNSIGNED_LONG_LONG	((MPI_Datatype) &lam_mpi_unsigned_long_long)

#define MPI_INTEGER             ((MPI_Datatype) &lam_mpi_integer)
#define MPI_INTEGER1            ((MPI_Datatype) &lam_mpi_integer1)
#define MPI_INTEGER2            ((MPI_Datatype) &lam_mpi_integer2)
#define MPI_INTEGER4            ((MPI_Datatype) &lam_mpi_integer4)
#define MPI_INTEGER8            ((MPI_Datatype) &lam_mpi_integer8)
#define MPI_INTEGER16           ((MPI_Datatype) &lam_mpi_integer16)
#define MPI_REAL                ((MPI_Datatype) &lam_mpi_real)
#define MPI_REAL4               ((MPI_Datatype) &lam_mpi_real4)
#define MPI_REAL8               ((MPI_Datatype) &lam_mpi_real8)
#define MPI_REAL16              ((MPI_Datatype) &lam_mpi_real16)
#define MPI_COMPLEX8            ((MPI_Datatype) &lam_mpi_complex8)
#define MPI_COMPLEX16           ((MPI_Datatype) &lam_mpi_complex16)
#define MPI_COMPLEX32           ((MPI_Datatype) &lam_mpi_complex32)
#define MPI_DOUBLE_PRECISION    ((MPI_Datatype) &lam_mpi_dblprec)
#define MPI_CHARACTER           ((MPI_Datatype) &lam_mpi_character)
#define MPI_LOGICAL             ((MPI_Datatype) &lam_mpi_logic)
#define MPI_COMPLEX             ((MPI_Datatype) &lam_mpi_cplex)
#define MPI_DOUBLE_COMPLEX      ((MPI_Datatype) &lam_mpi_dblcplex)
#define MPI_2INTEGER            ((MPI_Datatype) &lam_mpi_2integer)
#define MPI_2REAL               ((MPI_Datatype) &lam_mpi_2real)
#define MPI_2DOUBLE_PRECISION   ((MPI_Datatype) &lam_mpi_2dblprec)

#define MPI_ERRORS_ARE_FATAL	((MPI_Errhandler) &lam_mpi_errors_are_fatal)
#define MPI_ERRORS_RETURN	((MPI_Errhandler) &lam_mpi_errors_return)

#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

extern int	MPI_DUP_FN __ARGS((MPI_Comm, int, void *,
    					void *, void *, int *));
extern int	MPI_COMM_DUP_FN __ARGS((MPI_Comm, int, void *,
    					void *, void *, int *));
extern int	MPI_TYPE_DUP_FN __ARGS((MPI_Datatype, int, void *,
    					void *, void *, int *));
extern int	MPI_WIN_DUP_FN __ARGS((MPI_Win, int, void *,
    					void *, void *, int *));
extern int MPI_Abort __ARGS((MPI_Comm, int));
extern int MPI_Accumulate __ARGS((void *, int, MPI_Datatype, int, MPI_Aint, 
				  int, MPI_Datatype, MPI_Op, MPI_Win));
extern int MPI_Address __ARGS((void *, MPI_Aint *));
extern int MPI_Allgather __ARGS((void *, int, MPI_Datatype, void *, int, 
				 MPI_Datatype, MPI_Comm));
extern int MPI_Allgatherv __ARGS((void *, int, MPI_Datatype, void *, int *, 
				  int *, MPI_Datatype, MPI_Comm));
extern int MPI_Alloc_mem __ARGS((MPI_Aint size, MPI_Info info, 
				 void *baseptr));
extern int MPI_Allreduce __ARGS((void *, void *, int, MPI_Datatype, 
				 MPI_Op, MPI_Comm));
extern int MPI_Alltoall __ARGS((void *, int, MPI_Datatype, void *, int, 
				MPI_Datatype, MPI_Comm));
extern int MPI_Alltoallv __ARGS((void *, int *, int *, MPI_Datatype, void *, 
				 int *, int *, MPI_Datatype, MPI_Comm));
extern int MPI_Alltoallw __ARGS((void *, int *, int *, MPI_Datatype *, void *,
				 int *, int *, MPI_Datatype *, MPI_Comm));

extern int MPI_Attr_delete __ARGS((MPI_Comm, int));
extern int MPI_Attr_get __ARGS((MPI_Comm, int, void *, int *));
extern int MPI_Attr_put __ARGS((MPI_Comm, int, void *));
extern int MPI_Barrier __ARGS((MPI_Comm));
extern int MPI_Bcast __ARGS((void *, int, MPI_Datatype, int, MPI_Comm));
extern int MPI_Bsend __ARGS((void *, int, MPI_Datatype, int, int, MPI_Comm));
extern int MPI_Bsend_init __ARGS((void *, int, MPI_Datatype, int, int, 
				  MPI_Comm, MPI_Request *));
extern int MPI_Buffer_attach __ARGS((void *, int));
extern int MPI_Buffer_detach __ARGS((void *, int *));
extern int MPI_Cancel __ARGS((MPI_Request *));
extern int MPI_Cart_coords __ARGS((MPI_Comm, int, int, int *));
extern int MPI_Cart_create __ARGS((MPI_Comm, int, int *, int *, int, 
				   MPI_Comm *));
extern int MPI_Cartdim_get __ARGS((MPI_Comm, int *));
extern int MPI_Cart_get __ARGS((MPI_Comm, int, int *, int *, int *));
extern int MPI_Cart_map __ARGS((MPI_Comm, int, int *, int *, int *));
extern int MPI_Cart_rank __ARGS((MPI_Comm, int *, int *));
extern int MPI_Cart_shift __ARGS((MPI_Comm, int, int, int *, int *));
extern int MPI_Cart_sub __ARGS((MPI_Comm, int *, MPI_Comm *));
extern int MPI_Close_port __ARGS((char *));
extern MPI_Fint MPI_Comm_c2f __ARGS((MPI_Comm));
extern int MPI_Comm_accept __ARGS((char *, MPI_Info, int, MPI_Comm, 
				   MPI_Comm *));
extern int MPI_Comm_compare __ARGS((MPI_Comm, MPI_Comm, int *));
extern int MPI_Comm_connect __ARGS((char *, MPI_Info, int, MPI_Comm, 
				    MPI_Comm *));
extern int MPI_Comm_create __ARGS((MPI_Comm, MPI_Group, MPI_Comm *));
extern int MPI_Comm_create_errhandler __ARGS((MPI_Comm_errhandler_fn *, 
					      MPI_Errhandler *));
extern int MPI_Comm_create_keyval __ARGS((MPI_Comm_copy_attr_function *, 
					  MPI_Comm_delete_attr_function *, 
					  int *, void *));
extern int MPI_Comm_delete_attr __ARGS((MPI_Comm, int));
extern int MPI_Comm_disconnect __ARGS((MPI_Comm *));
extern int MPI_Comm_dup __ARGS((MPI_Comm, MPI_Comm *));
extern MPI_Comm MPI_Comm_f2c __ARGS((MPI_Fint));
extern int MPI_Comm_free __ARGS((MPI_Comm *));
extern int MPI_Comm_free_keyval __ARGS((int *));
extern int MPI_Comm_get_attr __ARGS((MPI_Comm, int, void *, int *));
extern int MPI_Comm_get_errhandler __ARGS((MPI_Comm, MPI_Errhandler *));
extern int MPI_Comm_get_name __ARGS((MPI_Comm, char *, int *));
extern int MPI_Comm_get_parent __ARGS((MPI_Comm *));
extern int MPI_Comm_group __ARGS((MPI_Comm, MPI_Group *));
extern int MPI_Comm_join __ARGS((int, MPI_Comm *));
extern int MPI_Comm_rank __ARGS((MPI_Comm, int *));
extern int MPI_Comm_remote_group __ARGS((MPI_Comm, MPI_Group *));
extern int MPI_Comm_remote_size __ARGS((MPI_Comm, int *));
extern int MPI_Comm_set_attr __ARGS((MPI_Comm, int, void *));
extern int MPI_Comm_set_errhandler __ARGS((MPI_Comm, MPI_Errhandler));
extern int MPI_Comm_set_name __ARGS((MPI_Comm, char *));
extern int MPI_Comm_size __ARGS((MPI_Comm, int *));
extern int MPI_Comm_spawn __ARGS((char *, char **, int, MPI_Info, int, 
				  MPI_Comm, MPI_Comm *, int *));
extern int MPI_Comm_spawn_multiple __ARGS((int, char **, char ***, int *, 
					   MPI_Info *, int, MPI_Comm, 
					   MPI_Comm *, int *));
extern int MPI_Comm_split __ARGS((MPI_Comm, int, int, MPI_Comm *));
extern int MPI_Comm_test_inter __ARGS((MPI_Comm, int *));
extern int MPI_Dims_create __ARGS((int, int, int *));
extern MPI_Fint MPI_Errhandler_c2f __ARGS((MPI_Errhandler err));
extern int MPI_Errhandler_create __ARGS((MPI_Handler_function *, 
					 MPI_Errhandler *));
extern MPI_Errhandler MPI_Errhandler_f2c __ARGS((MPI_Fint f_handle));
extern int MPI_Errhandler_free __ARGS((MPI_Errhandler *));
extern int MPI_Errhandler_get __ARGS((MPI_Comm, MPI_Errhandler *));
extern int MPI_Errhandler_set __ARGS((MPI_Comm, MPI_Errhandler));
extern int MPI_Error_class __ARGS((int, int *));
extern int MPI_Error_string __ARGS((int, char *, int *));
extern int MPI_Exscan __ARGS((void *, void *, int, MPI_Datatype, MPI_Op,
			      MPI_Comm));
extern int MPI_Finalize __ARGS((void));
extern int MPI_Finalized __ARGS((int *flag));
extern int MPI_Free_mem __ARGS((void *base));
extern int MPI_Gather __ARGS((void *, int, MPI_Datatype, void *, int, 
			      MPI_Datatype, int, MPI_Comm));
extern int MPI_Gatherv __ARGS((void *, int, MPI_Datatype, void *, int *, 
			       int *, MPI_Datatype, int, MPI_Comm));
extern int MPI_Get_address __ARGS((void *, MPI_Aint *));
extern int MPI_Get __ARGS((void *, int, MPI_Datatype, int, MPI_Aint, int, 
			   MPI_Datatype, MPI_Win));
extern int MPI_Get_count __ARGS((MPI_Status *, MPI_Datatype, int *));
extern int MPI_Get_elements __ARGS((MPI_Status *, MPI_Datatype, int *));
extern int MPI_Get_processor_name __ARGS((char *, int *));
extern int MPI_Get_version __ARGS((int *, int *));
extern int MPI_Graph_create __ARGS((MPI_Comm, int, int *, int *, int, 
				    MPI_Comm *));
extern int MPI_Graphdims_get __ARGS((MPI_Comm, int *, int *));
extern int MPI_Graph_get __ARGS((MPI_Comm, int, int, int *, int *));
extern int MPI_Graph_map __ARGS((MPI_Comm, int, int *, int *, int *));
extern int MPI_Graph_neighbors __ARGS((MPI_Comm, int, int, int *));
extern int MPI_Graph_neighbors_count __ARGS((MPI_Comm, int, int *));
extern MPI_Fint MPI_Group_c2f __ARGS((MPI_Group));
extern int MPI_Group_compare __ARGS((MPI_Group, MPI_Group, int *));
extern int MPI_Group_difference __ARGS((MPI_Group, MPI_Group, MPI_Group *));
extern int MPI_Group_excl __ARGS((MPI_Group, int, int *, MPI_Group *));
extern MPI_Group MPI_Group_f2c __ARGS((MPI_Fint));
extern int MPI_Group_free __ARGS((MPI_Group *));
extern int MPI_Group_incl __ARGS((MPI_Group, int, int *, MPI_Group *));
extern int MPI_Group_intersection __ARGS((MPI_Group, MPI_Group, MPI_Group *));
extern int MPI_Group_range_excl __ARGS((MPI_Group, int, int [][3], 
					MPI_Group *));
extern int MPI_Group_range_incl __ARGS((MPI_Group, int, int [][3], 
					MPI_Group *));
extern int MPI_Group_rank __ARGS((MPI_Group, int *));
extern int MPI_Group_size __ARGS((MPI_Group, int *));
extern int MPI_Group_translate_ranks __ARGS((MPI_Group, int, int *, 
					     MPI_Group, int *));
extern int MPI_Group_union __ARGS((MPI_Group, MPI_Group, MPI_Group *));
extern int MPI_Ibsend __ARGS((void *, int, MPI_Datatype, int, int, MPI_Comm, 
			      MPI_Request *));
extern MPI_Fint MPI_Info_c2f __ARGS((MPI_Info));
extern int MPI_Info_create __ARGS((MPI_Info *));
extern int MPI_Info_delete __ARGS((MPI_Info, char *));
extern int MPI_Info_dup __ARGS((MPI_Info, MPI_Info *));
extern MPI_Info MPI_Info_f2c __ARGS((MPI_Fint));
extern int MPI_Info_free __ARGS((MPI_Info *));
extern int MPI_Info_get __ARGS((MPI_Info, char *, int, char *, int *));
extern int MPI_Info_get_nkeys __ARGS((MPI_Info, int *));
extern int MPI_Info_get_nthkey __ARGS((MPI_Info, int, char *));
extern int MPI_Info_get_valuelen __ARGS((MPI_Info, char *, int *, int *));
extern int MPI_Info_set __ARGS((MPI_Info, char *, char *));
extern int MPI_Init __ARGS((int *, char ***));
extern int MPI_Initialized __ARGS((int *));
extern int MPI_Init_thread __ARGS((int *, char ***, int, int *));
extern int MPI_Intercomm_create __ARGS((MPI_Comm, int, MPI_Comm, int, int, 
					MPI_Comm *));
extern int MPI_Intercomm_merge __ARGS((MPI_Comm, int, MPI_Comm *));
extern int MPI_Iprobe __ARGS((int, int, MPI_Comm, int *, MPI_Status *));
extern int MPI_Irecv __ARGS((void *, int, MPI_Datatype, int, int, MPI_Comm, 
			     MPI_Request *));
extern int MPI_Irsend __ARGS((void *, int, MPI_Datatype, int, int, MPI_Comm, 
			      MPI_Request *));
extern int MPI_Isend __ARGS((void *, int, MPI_Datatype, int, int, MPI_Comm, 
			     MPI_Request *));
extern int MPI_Issend __ARGS((void *, int, MPI_Datatype, int, int, MPI_Comm, 
			      MPI_Request *));
extern int MPI_Is_thread_main __ARGS((int *));
extern int MPI_Keyval_create __ARGS((MPI_Copy_function *, 
				     MPI_Delete_function *, int *, void *));
extern int MPI_Keyval_free __ARGS((int *));
extern int MPI_Lookup_name __ARGS((char *, MPI_Info, char *));
extern MPI_Fint MPI_Op_c2f __ARGS((MPI_Op op)); 
extern int MPI_Op_create __ARGS((MPI_User_function *, int, MPI_Op *));
extern int MPI_Open_port __ARGS((MPI_Info, char *));
extern MPI_Op MPI_Op_f2c __ARGS((MPI_Fint f_handle));
extern int MPI_Op_free __ARGS((MPI_Op *));
extern int MPI_Pack __ARGS((void *, int, MPI_Datatype, void *, int, int *, 
			    MPI_Comm));
extern int MPI_Pack_size __ARGS((int, MPI_Datatype, MPI_Comm, int *));
extern int MPI_Pcontrol __ARGS((int level, ...));
extern int MPI_Probe __ARGS((int, int, MPI_Comm, MPI_Status *));
extern int MPI_Publish_name __ARGS((char *, MPI_Info, char *));
extern int MPI_Put __ARGS((void *, int, MPI_Datatype, int, MPI_Aint, int, 
			   MPI_Datatype, MPI_Win));
extern int MPI_Query_thread __ARGS((int *));
extern int MPI_Recv __ARGS((void *, int, MPI_Datatype, int, int, MPI_Comm, 
			    MPI_Status *));
extern int MPI_Recv_init __ARGS((void *, int, MPI_Datatype, int, int, 
				 MPI_Comm, MPI_Request *));
extern int MPI_Reduce __ARGS((void *, void *, int, MPI_Datatype, MPI_Op, int, 
			      MPI_Comm));
extern int MPI_Reduce_scatter __ARGS((void *, void *, int *, MPI_Datatype, 
				      MPI_Op, MPI_Comm));
extern MPI_Fint MPI_Request_c2f __ARGS((MPI_Request));
extern MPI_Request MPI_Request_f2c __ARGS((MPI_Fint));
extern int MPI_Request_free __ARGS((MPI_Request *));
extern int MPI_Rsend __ARGS((void *, int, MPI_Datatype, int, int, MPI_Comm));
extern int MPI_Rsend_init __ARGS((void *, int, MPI_Datatype, int, int, 
				  MPI_Comm, MPI_Request *));
extern int MPI_Scan __ARGS((void *, void *, int, MPI_Datatype, MPI_Op, 
			    MPI_Comm));
extern int MPI_Scatter __ARGS((void *, int, MPI_Datatype, void *, int, 
			       MPI_Datatype, int, MPI_Comm));
extern int MPI_Scatterv __ARGS((void *, int *, int *, MPI_Datatype, 
				void *, int, MPI_Datatype, int, MPI_Comm));
extern int MPI_Send __ARGS((void *, int, MPI_Datatype, int, int, MPI_Comm));
extern int MPI_Send_init __ARGS((void *, int, MPI_Datatype, int, int, 
				 MPI_Comm, MPI_Request *));
extern int MPI_Sendrecv __ARGS((void *, int, MPI_Datatype, int, int, void *, 
				int, MPI_Datatype, int, int, MPI_Comm, 
				MPI_Status *));
extern int MPI_Sendrecv_replace __ARGS((void *, int, MPI_Datatype, int, int, 
					int, int, MPI_Comm, MPI_Status *));
extern int MPI_Ssend __ARGS((void *, int, MPI_Datatype, int, int, MPI_Comm));
extern int MPI_Ssend_init __ARGS((void *, int, MPI_Datatype, int, int, 
				  MPI_Comm, MPI_Request *));
extern int MPI_Startall __ARGS((int, MPI_Request *));
extern int MPI_Start __ARGS((MPI_Request *));
extern int MPI_Status_c2f __ARGS((MPI_Status *, MPI_Fint *));
extern int MPI_Status_f2c __ARGS((MPI_Fint *, MPI_Status *));
extern int MPI_Testall __ARGS((int, MPI_Request *, int *, MPI_Status *));
extern int MPI_Testany __ARGS((int, MPI_Request *, int *, int *, 
			       MPI_Status *));
extern int MPI_Test __ARGS((MPI_Request *, int *, MPI_Status *));
extern int MPI_Test_cancelled __ARGS((MPI_Status *, int *));
extern int MPI_Testsome __ARGS((int, MPI_Request *, int *, int *, 
				MPI_Status *));
extern int MPI_Topo_test __ARGS((MPI_Comm, int *));
extern MPI_Fint MPI_Type_c2f __ARGS((MPI_Datatype));
extern int MPI_Type_commit __ARGS((MPI_Datatype *));
extern int MPI_Type_contiguous __ARGS((int, MPI_Datatype, MPI_Datatype *));
extern int MPI_Type_create_darray __ARGS((int, int, int, int *, int *, int *, 
					  int*, int, MPI_Datatype, 
					  MPI_Datatype *));
extern int MPI_Type_create_hindexed __ARGS((int, int *, MPI_Aint *, 
					    MPI_Datatype, MPI_Datatype *));
extern int MPI_Type_create_hvector __ARGS((int, int, MPI_Aint, MPI_Datatype, 
					   MPI_Datatype *));
extern int MPI_Type_create_keyval __ARGS((MPI_Type_copy_attr_function *, 
					  MPI_Type_delete_attr_function *, 
					  int *, void *));
extern int MPI_Type_create_resized __ARGS((MPI_Datatype, MPI_Aint, MPI_Aint, 
					   MPI_Datatype *));
extern int MPI_Type_create_struct __ARGS((int, int *, MPI_Aint *, 
					  MPI_Datatype *, MPI_Datatype *));
extern int MPI_Type_create_subarray __ARGS((int, int *, int *, int *, int, 
					    MPI_Datatype, MPI_Datatype *));
extern int MPI_Type_delete_attr __ARGS((MPI_Datatype, int));
extern int MPI_Type_dup __ARGS((MPI_Datatype, MPI_Datatype *));
extern int MPI_Type_extent __ARGS((MPI_Datatype, MPI_Aint *));
extern MPI_Datatype MPI_Type_f2c __ARGS((MPI_Fint));
extern int MPI_Type_free __ARGS((MPI_Datatype *));
extern int MPI_Type_free_keyval __ARGS((int *));
extern int MPI_Type_get_attr __ARGS((MPI_Datatype, int, void *, int *));
extern int MPI_Type_get_contents __ARGS((MPI_Datatype, int, int, int, int *, 
					 MPI_Aint *, MPI_Datatype *));
extern int MPI_Type_get_envelope __ARGS((MPI_Datatype, int *, int *, int *, 
					 int *));
extern int MPI_Type_get_extent __ARGS((MPI_Datatype, MPI_Aint *, MPI_Aint *));
extern int MPI_Type_get_name __ARGS((MPI_Datatype, char *, int *));
extern int MPI_Type_get_true_extent __ARGS((MPI_Datatype, MPI_Aint *, 
					    MPI_Aint *));
extern int MPI_Type_hindexed __ARGS((int, int *, MPI_Aint *, MPI_Datatype, 
				     MPI_Datatype *));
extern int MPI_Type_hvector __ARGS((int, int, MPI_Aint, MPI_Datatype, 
				    MPI_Datatype *));
extern int MPI_Type_indexed __ARGS((int, int *, int *, MPI_Datatype, 
				    MPI_Datatype *));
extern int MPI_Type_lb __ARGS((MPI_Datatype, MPI_Aint *));
extern int MPI_Type_set_attr __ARGS((MPI_Datatype, int, void *));
extern int MPI_Type_set_name __ARGS((MPI_Datatype, char *));
extern int MPI_Type_size __ARGS((MPI_Datatype, int *));
extern int MPI_Type_struct __ARGS((int, int *, MPI_Aint *, MPI_Datatype *, 
				   MPI_Datatype *));
extern int MPI_Type_ub __ARGS((MPI_Datatype, MPI_Aint *));
extern int MPI_Type_vector __ARGS((int, int, int, MPI_Datatype, 
				   MPI_Datatype *));
extern int MPI_Unpack __ARGS((void *, int, int *, void *, int, MPI_Datatype, 
			      MPI_Comm));
extern int MPI_Unpublish_name __ARGS((char *, MPI_Info, char *));
extern int MPI_Waitall __ARGS((int, MPI_Request *, MPI_Status *));
extern int MPI_Waitany __ARGS((int, MPI_Request *, int *, MPI_Status *));
extern int MPI_Wait __ARGS((MPI_Request *, MPI_Status *));
extern int MPI_Waitsome __ARGS((int, MPI_Request *, int *, int *, 
				MPI_Status *));
extern MPI_Fint MPI_Win_c2f __ARGS((MPI_Win));
extern int MPI_Win_call_errhandler __ARGS((MPI_Win, int));
extern int MPI_Win_complete __ARGS((MPI_Win));
extern int MPI_Win_create __ARGS((void *, MPI_Aint, int, MPI_Info, MPI_Comm, 
				  MPI_Win *));
extern int MPI_Win_create_errhandler __ARGS((MPI_Win_errhandler_fn *, 
					     MPI_Errhandler *));
extern int MPI_Win_create_keyval __ARGS((MPI_Win_copy_attr_function *, 
					 MPI_Win_delete_attr_function *, 
					 int *, void *));
extern int MPI_Win_delete_attr __ARGS((MPI_Win, int));
extern MPI_Win MPI_Win_f2c __ARGS((MPI_Fint));
extern int MPI_Win_fence __ARGS((int, MPI_Win));
extern int MPI_Win_free __ARGS((MPI_Win *));
extern int MPI_Win_free_keyval __ARGS((int *));
extern int MPI_Win_get_attr __ARGS((MPI_Win, int, void *, int *));
extern int MPI_Win_get_errhandler __ARGS((MPI_Win, MPI_Errhandler *));
extern int MPI_Win_get_group __ARGS((MPI_Win, MPI_Group *));
extern int MPI_Win_get_name __ARGS((MPI_Win, char *, int *));
extern int MPI_Win_lock __ARGS((int, int, int, MPI_Win));
extern int MPI_Win_post __ARGS((MPI_Group, int, MPI_Win));
extern int MPI_Win_set_attr __ARGS((MPI_Win, int, void *));
extern int MPI_Win_set_errhandler __ARGS((MPI_Win, MPI_Errhandler));
extern int MPI_Win_set_name __ARGS((MPI_Win, char *));
extern int MPI_Win_start __ARGS((MPI_Group, int, MPI_Win));
extern int MPI_Win_test __ARGS((MPI_Win, int *));
extern int MPI_Win_unlock __ARGS((int, MPI_Win));
extern int MPI_Win_wait __ARGS((MPI_Win));
extern double MPI_Wtick __ARGS((void));
extern double MPI_Wtime __ARGS((void));

/*
 * profiling prototypes
 */
extern int PMPI_Abort __ARGS((MPI_Comm, int));
extern int PMPI_Accumulate __ARGS((void *, int, MPI_Datatype, int, MPI_Aint, 
				   int, MPI_Datatype, MPI_Op, MPI_Win));
extern int PMPI_Address __ARGS((void *, MPI_Aint *));
extern int PMPI_Allgather __ARGS((void *, int, MPI_Datatype, void *, int, 
				  MPI_Datatype, MPI_Comm));
extern int PMPI_Allgatherv __ARGS((void *, int, MPI_Datatype, void *, int *, 
				   int *, MPI_Datatype, MPI_Comm));
extern int PMPI_Alloc_mem __ARGS((MPI_Aint size, MPI_Info info, 
				  void *baseptr));
extern int PMPI_Allreduce __ARGS((void *, void *, int, MPI_Datatype, MPI_Op, 
				  MPI_Comm));
extern int PMPI_Alltoall __ARGS((void *, int, MPI_Datatype, void *, int, 
				 MPI_Datatype, MPI_Comm));
extern int PMPI_Alltoallv __ARGS((void *, int *, int *, MPI_Datatype, void *, 
				  int *, int *, MPI_Datatype, MPI_Comm));
extern int PMPI_Alltoallw __ARGS((void *, int *, int *, MPI_Datatype *, void *,
				  int *, int *, MPI_Datatype *, MPI_Comm));
extern int PMPI_Attr_delete __ARGS((MPI_Comm, int));
extern int PMPI_Attr_get __ARGS((MPI_Comm, int, void *, int *));
extern int PMPI_Attr_put __ARGS((MPI_Comm, int, void *));
extern int PMPI_Barrier __ARGS((MPI_Comm));
extern int PMPI_Bcast __ARGS((void *, int, MPI_Datatype, int, MPI_Comm));
extern int PMPI_Bsend __ARGS((void *, int, MPI_Datatype, int, int, MPI_Comm));
extern int PMPI_Bsend_init __ARGS((void *, int, MPI_Datatype, int, int, 
				   MPI_Comm, MPI_Request *));
extern int PMPI_Buffer_attach __ARGS((void *, int));
extern int PMPI_Buffer_detach __ARGS((void *, int *));
extern int PMPI_Cancel __ARGS((MPI_Request *));
extern int PMPI_Cart_coords __ARGS((MPI_Comm, int, int, int *));
extern int PMPI_Cart_create __ARGS((MPI_Comm, int, int *, int *, int, 
				    MPI_Comm *));
extern int PMPI_Cartdim_get __ARGS((MPI_Comm, int *));
extern int PMPI_Cart_get __ARGS((MPI_Comm, int, int *, int *, int *));
extern int PMPI_Cart_map __ARGS((MPI_Comm, int, int *, int *, int *));
extern int PMPI_Cart_rank __ARGS((MPI_Comm, int *, int *));
extern int PMPI_Cart_shift __ARGS((MPI_Comm, int, int, int *, int *));
extern int PMPI_Cart_sub __ARGS((MPI_Comm, int *, MPI_Comm *));
extern int PMPI_Close_port __ARGS((char *));
extern MPI_Fint PMPI_Comm_c2f __ARGS((MPI_Comm));
extern int PMPI_Comm_accept __ARGS((char *, MPI_Info, int, MPI_Comm, 
				    MPI_Comm *));
extern int PMPI_Comm_compare __ARGS((MPI_Comm, MPI_Comm, int *));
extern int PMPI_Comm_connect __ARGS((char *, MPI_Info, int, MPI_Comm, 
				     MPI_Comm *));
extern int PMPI_Comm_create __ARGS((MPI_Comm, MPI_Group, MPI_Comm *));
extern int PMPI_Comm_create_errhandler __ARGS((MPI_Comm_errhandler_fn *, 
					       MPI_Errhandler *));
extern int PMPI_Comm_create_keyval __ARGS((MPI_Comm_copy_attr_function *, 
					   MPI_Comm_delete_attr_function *, 
					   int *, void *));
extern int PMPI_Comm_delete_attr __ARGS((MPI_Comm, int));
extern int PMPI_Comm_disconnect __ARGS((MPI_Comm *));
extern int PMPI_Comm_dup __ARGS((MPI_Comm, MPI_Comm *));
extern MPI_Comm PMPI_Comm_f2c __ARGS((MPI_Fint));
extern int PMPI_Comm_free __ARGS((MPI_Comm *));
extern int PMPI_Comm_free_keyval __ARGS((int *));
extern int PMPI_Comm_get_attr __ARGS((MPI_Comm, int, void *, int *));
extern int PMPI_Comm_get_errhandler __ARGS((MPI_Comm, MPI_Errhandler *));
extern int PMPI_Comm_get_name __ARGS((MPI_Comm, char *, int *));
extern int PMPI_Comm_get_parent __ARGS((MPI_Comm *));
extern int PMPI_Comm_group __ARGS((MPI_Comm, MPI_Group *));
extern int PMPI_Comm_join __ARGS((int, MPI_Comm *));
extern int PMPI_Comm_rank __ARGS((MPI_Comm, int *));
extern int PMPI_Comm_remote_group __ARGS((MPI_Comm, MPI_Group *));
extern int PMPI_Comm_remote_size __ARGS((MPI_Comm, int *));
extern int PMPI_Comm_set_attr __ARGS((MPI_Comm, int, void *));
extern int PMPI_Comm_set_errhandler __ARGS((MPI_Comm, MPI_Errhandler));
extern int PMPI_Comm_set_name __ARGS((MPI_Comm, char *));
extern int PMPI_Comm_size __ARGS((MPI_Comm, int *));
extern int PMPI_Comm_spawn __ARGS((char *, char **, int, MPI_Info, int, 
				   MPI_Comm, MPI_Comm *, int *));
extern int PMPI_Comm_spawn_multiple __ARGS((int, char **, char ***, int *, 
					    MPI_Info *, int, MPI_Comm, 
					    MPI_Comm *, int *));
extern int PMPI_Comm_split __ARGS((MPI_Comm, int, int, MPI_Comm *));
extern int PMPI_Comm_test_inter __ARGS((MPI_Comm, int *));
extern int PMPI_Dims_create __ARGS((int, int, int *));
extern MPI_Fint PMPI_Errhandler_c2f __ARGS((MPI_Errhandler err));
extern int PMPI_Errhandler_create __ARGS((MPI_Handler_function *, 
					  MPI_Errhandler *));
extern MPI_Errhandler PMPI_Errhandler_f2c __ARGS((MPI_Fint f_handle));
extern int PMPI_Errhandler_free __ARGS((MPI_Errhandler *));
extern int PMPI_Errhandler_get __ARGS((MPI_Comm, MPI_Errhandler *));
extern int PMPI_Errhandler_set __ARGS((MPI_Comm, MPI_Errhandler));
extern int PMPI_Error_class __ARGS((int, int *));
extern int PMPI_Error_string __ARGS((int, char *, int *));
extern int PMPI_Exscan __ARGS((void *, void *, int, MPI_Datatype, MPI_Op,
			       MPI_Comm));
extern int PMPI_Finalize __ARGS((void));
extern int PMPI_Finalized __ARGS((int *flag));
extern int PMPI_Free_mem __ARGS((void *base));
extern int PMPI_Gather __ARGS((void *, int, MPI_Datatype, void *, int, 
			       MPI_Datatype, int, MPI_Comm));
extern int PMPI_Gatherv __ARGS((void *, int, MPI_Datatype, void *, int *, 
				int *, MPI_Datatype, int, MPI_Comm));
extern int PMPI_Get_address __ARGS((void *, MPI_Aint *));
extern int PMPI_Get __ARGS((void *, int, MPI_Datatype, int, MPI_Aint, int, 
			    MPI_Datatype, MPI_Win));
extern int PMPI_Get_count __ARGS((MPI_Status *, MPI_Datatype, int *));
extern int PMPI_Get_elements __ARGS((MPI_Status *, MPI_Datatype, int *));
extern int PMPI_Get_processor_name __ARGS((char *, int *));
extern int PMPI_Get_version __ARGS((int *, int *));
extern int PMPI_Graph_create __ARGS((MPI_Comm, int, int *, int *, int, 
				     MPI_Comm *));
extern int PMPI_Graphdims_get __ARGS((MPI_Comm, int *, int *));
extern int PMPI_Graph_get __ARGS((MPI_Comm, int, int, int *, int *));
extern int PMPI_Graph_map __ARGS((MPI_Comm, int, int *, int *, int *));
extern int PMPI_Graph_neighbors __ARGS((MPI_Comm, int, int, int *));
extern int PMPI_Graph_neighbors_count __ARGS((MPI_Comm, int, int *));
extern MPI_Fint PMPI_Group_c2f __ARGS((MPI_Group));
extern int PMPI_Group_compare __ARGS((MPI_Group, MPI_Group, int *));
extern int PMPI_Group_difference __ARGS((MPI_Group, MPI_Group, MPI_Group *));
extern int PMPI_Group_excl __ARGS((MPI_Group, int, int *, MPI_Group *));
extern MPI_Group PMPI_Group_f2c __ARGS((MPI_Fint));
extern int PMPI_Group_free __ARGS((MPI_Group *));
extern int PMPI_Group_incl __ARGS((MPI_Group, int, int *, MPI_Group *));
extern int PMPI_Group_intersection __ARGS((MPI_Group, MPI_Group, MPI_Group *));
extern int PMPI_Group_range_excl __ARGS((MPI_Group, int, int [][3], 
					 MPI_Group *));
extern int PMPI_Group_range_incl __ARGS((MPI_Group, int, int [][3], 
					 MPI_Group *));
extern int PMPI_Group_rank __ARGS((MPI_Group, int *));
extern int PMPI_Group_size __ARGS((MPI_Group, int *));
extern int PMPI_Group_translate_ranks __ARGS((MPI_Group, int, int *, 
					      MPI_Group, int *));
extern int PMPI_Group_union __ARGS((MPI_Group, MPI_Group, MPI_Group *));
extern int PMPI_Ibsend __ARGS((void *, int, MPI_Datatype, int, int, 
			       MPI_Comm, MPI_Request *));
extern MPI_Fint PMPI_Info_c2f __ARGS((MPI_Info));
extern int PMPI_Info_create __ARGS((MPI_Info *));
extern int PMPI_Info_delete __ARGS((MPI_Info, char *));
extern int PMPI_Info_dup __ARGS((MPI_Info, MPI_Info *));
extern MPI_Info PMPI_Info_f2c __ARGS((MPI_Fint));
extern int PMPI_Info_free __ARGS((MPI_Info *));
extern int PMPI_Info_get __ARGS((MPI_Info, char *, int, char *, int *));
extern int PMPI_Info_get_nkeys __ARGS((MPI_Info, int *));
extern int PMPI_Info_get_nthkey __ARGS((MPI_Info, int, char *));
extern int PMPI_Info_get_valuelen __ARGS((MPI_Info, char *, int *, int *));
extern int PMPI_Info_set __ARGS((MPI_Info, char *, char *));
extern int PMPI_Init __ARGS((int *, char ***));
extern int PMPI_Initialized __ARGS((int *));
extern int PMPI_Init_thread __ARGS((int *, char ***, int, int *));
extern int PMPI_Intercomm_create __ARGS((MPI_Comm, int, MPI_Comm, int, int, 
					 MPI_Comm *));
extern int PMPI_Intercomm_merge __ARGS((MPI_Comm, int, MPI_Comm *));
extern int PMPI_Iprobe __ARGS((int, int, MPI_Comm, int *, MPI_Status *));
extern int PMPI_Irecv __ARGS((void *, int, MPI_Datatype, int, int, MPI_Comm, 
			      MPI_Request *));
extern int PMPI_Irsend __ARGS((void *, int, MPI_Datatype, int, int, MPI_Comm, 
			       MPI_Request *));
extern int PMPI_Isend __ARGS((void *, int, MPI_Datatype, int, int, MPI_Comm, 
			      MPI_Request *));
extern int PMPI_Issend __ARGS((void *, int, MPI_Datatype, int, int, MPI_Comm, 
			       MPI_Request *));
extern int PMPI_Is_thread_main __ARGS((int *));
extern int PMPI_Keyval_create __ARGS((MPI_Copy_function *, 
				      MPI_Delete_function *, int *, void *));
extern int PMPI_Keyval_free __ARGS((int *));
extern int PMPI_Lookup_name __ARGS((char *, MPI_Info, char *));
extern MPI_Fint PMPI_Op_c2f __ARGS((MPI_Op op));
extern int PMPI_Op_create __ARGS((MPI_User_function *, int, MPI_Op *));
extern int PMPI_Open_port __ARGS((MPI_Info, char *));
extern MPI_Op PMPI_Op_f2c __ARGS((MPI_Fint f_handle));
extern int PMPI_Op_free __ARGS((MPI_Op *));
extern int PMPI_Pack __ARGS((void *, int, MPI_Datatype, void *, int, int *, 
			     MPI_Comm));
extern int PMPI_Pack_size __ARGS((int, MPI_Datatype, MPI_Comm, int *));
extern int PMPI_Pcontrol __ARGS((int level, ...));
extern int PMPI_Probe __ARGS((int, int, MPI_Comm, MPI_Status *));
extern int PMPI_Publish_name __ARGS((char *, MPI_Info, char *));
extern int PMPI_Put __ARGS((void *, int, MPI_Datatype, int, MPI_Aint, int, 
			    MPI_Datatype, MPI_Win));
extern int PMPI_Query_thread __ARGS((int *));
extern int PMPI_Recv __ARGS((void *, int, MPI_Datatype, int, int, MPI_Comm, 
			     MPI_Status *));
extern int PMPI_Recv_init __ARGS((void *, int, MPI_Datatype, int, int, 
				  MPI_Comm, MPI_Request *));
extern int PMPI_Reduce __ARGS((void *, void *, int, MPI_Datatype, MPI_Op, 
			       int, MPI_Comm));
extern int PMPI_Reduce_scatter __ARGS((void *, void *, int *, MPI_Datatype, 
				       MPI_Op, MPI_Comm));
extern MPI_Fint PMPI_Request_c2f __ARGS((MPI_Request));
extern MPI_Request PMPI_Request_f2c __ARGS((MPI_Fint));
extern int PMPI_Request_free __ARGS((MPI_Request *));
extern int PMPI_Rsend __ARGS((void *, int, MPI_Datatype, int, int, MPI_Comm));
extern int PMPI_Rsend_init __ARGS((void *, int, MPI_Datatype, int, int, 
				   MPI_Comm, MPI_Request *));
extern int PMPI_Scan __ARGS((void *, void *, int, MPI_Datatype, MPI_Op, 
			     MPI_Comm));
extern int PMPI_Scatter __ARGS((void *, int, MPI_Datatype, void *, int, 
				MPI_Datatype, int, MPI_Comm));
extern int PMPI_Scatterv __ARGS((void *, int *, int *, MPI_Datatype, void *, 
				 int, MPI_Datatype, int, MPI_Comm));
extern int PMPI_Send __ARGS((void *, int, MPI_Datatype, int, int, MPI_Comm));
extern int PMPI_Send_init __ARGS((void *, int, MPI_Datatype, int, int, 
				  MPI_Comm, MPI_Request *));
extern int PMPI_Sendrecv __ARGS((void *, int, MPI_Datatype, int, int, void *, 
				 int, MPI_Datatype, int, int, MPI_Comm, 
				 MPI_Status *));
extern int PMPI_Sendrecv_replace __ARGS((void *, int, MPI_Datatype, int, int, 
					 int, int, MPI_Comm, MPI_Status *));
extern int PMPI_Ssend __ARGS((void *, int, MPI_Datatype, int, int, MPI_Comm));
extern int PMPI_Ssend_init __ARGS((void *, int, MPI_Datatype, int, int, 
				   MPI_Comm, MPI_Request *));
extern int PMPI_Startall __ARGS((int, MPI_Request *));
extern int PMPI_Start __ARGS((MPI_Request *));
extern int PMPI_Status_c2f __ARGS((MPI_Status *, MPI_Fint *));
extern int PMPI_Status_f2c __ARGS((MPI_Fint *, MPI_Status *));
extern int PMPI_Testall __ARGS((int, MPI_Request *, int *, MPI_Status *));
extern int PMPI_Testany __ARGS((int, MPI_Request *, int *, int *, 
				MPI_Status *));
extern int PMPI_Test __ARGS((MPI_Request *, int *, MPI_Status *));
extern int PMPI_Test_cancelled __ARGS((MPI_Status *, int *));
extern int PMPI_Testsome __ARGS((int, MPI_Request *, int *, int *, 
				 MPI_Status *));
extern int PMPI_Topo_test __ARGS((MPI_Comm, int *));
extern MPI_Fint PMPI_Type_c2f __ARGS((MPI_Datatype));
extern int PMPI_Type_commit __ARGS((MPI_Datatype *));
extern int PMPI_Type_contiguous __ARGS((int, MPI_Datatype, MPI_Datatype *));
extern int PMPI_Type_create_darray __ARGS((int, int, int, int *, int *, 
					   int *, int*, int, MPI_Datatype, 
					   MPI_Datatype *));
extern int PMPI_Type_create_hindexed __ARGS((int, int *, MPI_Aint *, 
					     MPI_Datatype, MPI_Datatype *));
extern int PMPI_Type_create_hvector __ARGS((int, int, MPI_Aint, MPI_Datatype, 
					    MPI_Datatype *));
extern int PMPI_Type_create_keyval __ARGS((MPI_Type_copy_attr_function *, 
					   MPI_Type_delete_attr_function *, 
					   int *, void *));
extern int PMPI_Type_create_resized __ARGS((MPI_Datatype, MPI_Aint, MPI_Aint, 
					    MPI_Datatype *));
extern int PMPI_Type_create_struct __ARGS((int, int *, MPI_Aint *, 
					   MPI_Datatype *, MPI_Datatype *));
extern int PMPI_Type_create_subarray __ARGS((int, int *, int *, int *, int, 
					     MPI_Datatype, MPI_Datatype *));
extern int PMPI_Type_delete_attr __ARGS((MPI_Datatype, int));
extern int PMPI_Type_dup __ARGS((MPI_Datatype, MPI_Datatype *));
extern int PMPI_Type_extent __ARGS((MPI_Datatype, MPI_Aint *));
extern MPI_Datatype PMPI_Type_f2c __ARGS((MPI_Fint));
extern int PMPI_Type_free __ARGS((MPI_Datatype *));
extern int PMPI_Type_free_keyval __ARGS((int *));
extern int PMPI_Type_get_attr __ARGS((MPI_Datatype, int, void *, int *));
extern int PMPI_Type_get_contents __ARGS((MPI_Datatype, int, int, int, int *, 
					  MPI_Aint *, MPI_Datatype *));
extern int PMPI_Type_get_envelope __ARGS((MPI_Datatype, int *, int *, int *, 
					  int *));
extern int PMPI_Type_get_extent __ARGS((MPI_Datatype, MPI_Aint *, MPI_Aint *));
extern int PMPI_Type_get_name __ARGS((MPI_Datatype, char *, int *));
extern int PMPI_Type_get_true_extent __ARGS((MPI_Datatype, MPI_Aint *, 
					     MPI_Aint *));
extern int PMPI_Type_hindexed __ARGS((int, int *, MPI_Aint *, MPI_Datatype, 
				      MPI_Datatype *));
extern int PMPI_Type_hvector __ARGS((int, int, MPI_Aint, MPI_Datatype, 
				     MPI_Datatype *));
extern int PMPI_Type_indexed __ARGS((int, int *, int *, MPI_Datatype, 
				     MPI_Datatype *));
extern int PMPI_Type_lb __ARGS((MPI_Datatype, MPI_Aint *));
extern int PMPI_Type_set_attr __ARGS((MPI_Datatype, int, void *));
extern int PMPI_Type_set_name __ARGS((MPI_Datatype, char *));
extern int PMPI_Type_size __ARGS((MPI_Datatype, int *));
extern int PMPI_Type_struct __ARGS((int, int *, MPI_Aint *, MPI_Datatype *, 
				    MPI_Datatype *));
extern int PMPI_Type_ub __ARGS((MPI_Datatype, MPI_Aint *));
extern int PMPI_Type_vector __ARGS((int, int, int, MPI_Datatype, 
				    MPI_Datatype *));
extern int PMPI_Unpack __ARGS((void *, int, int *, void *, int, MPI_Datatype, 
			       MPI_Comm));
extern int PMPI_Unpublish_name __ARGS((char *, MPI_Info, char *));
extern int PMPI_Waitall __ARGS((int, MPI_Request *, MPI_Status *));
extern int PMPI_Waitany __ARGS((int, MPI_Request *, int *, MPI_Status *));
extern int PMPI_Wait __ARGS((MPI_Request *, MPI_Status *));
extern int PMPI_Waitsome __ARGS((int, MPI_Request *, int *, int *, 
				 MPI_Status *));
extern MPI_Fint PMPI_Win_c2f __ARGS((MPI_Win));
extern int PMPI_Win_call_errhandler __ARGS((MPI_Win, int));
extern int PMPI_Win_complete __ARGS((MPI_Win));
extern int PMPI_Win_create __ARGS((void *, MPI_Aint, int, MPI_Info, MPI_Comm, 
				   MPI_Win *));
extern int PMPI_Win_create_errhandler __ARGS((MPI_Win_errhandler_fn *, 
					      MPI_Errhandler *));
extern int PMPI_Win_create_keyval __ARGS((MPI_Win_copy_attr_function *, 
					  MPI_Win_delete_attr_function *, 
					  int *, void *));
extern int PMPI_Win_delete_attr __ARGS((MPI_Win, int));
extern int PMPI_Win_fence __ARGS((int, MPI_Win));
extern MPI_Win PMPI_Win_f2c __ARGS((MPI_Fint));
extern int PMPI_Win_free __ARGS((MPI_Win *));
extern int PMPI_Win_free_keyval __ARGS((int *));
extern int PMPI_Win_get_attr __ARGS((MPI_Win, int, void *, int *));
extern int PMPI_Win_get_errhandler __ARGS((MPI_Win, MPI_Errhandler *));
extern int PMPI_Win_get_group __ARGS((MPI_Win, MPI_Group *));
extern int PMPI_Win_get_name __ARGS((MPI_Win, char *, int *));
extern int PMPI_Win_lock __ARGS((int, int, int, MPI_Win));
extern int PMPI_Win_post __ARGS((MPI_Group, int, MPI_Win));
extern int PMPI_Win_set_attr __ARGS((MPI_Win, int, void *));
extern int PMPI_Win_set_errhandler __ARGS((MPI_Win, MPI_Errhandler));
extern int PMPI_Win_set_name __ARGS((MPI_Win, char *));
extern int PMPI_Win_start __ARGS((MPI_Group, int, MPI_Win));
extern int PMPI_Win_test __ARGS((MPI_Win, int *));
extern int PMPI_Win_unlock __ARGS((int, MPI_Win));
extern int PMPI_Win_wait __ARGS((MPI_Win));
extern double PMPI_Wtick __ARGS((void));
extern double PMPI_Wtime __ARGS((void));

/*
 * LAM/MPI extensions
 */
extern int MPIL_Spawn __ARGS((MPI_Comm, char *, int, MPI_Comm *));
extern int MPIL_Comm_parent __ARGS((MPI_Comm *));
extern int MPIL_Universe_size __ARGS((int *));
extern int MPIL_Comm_id __ARGS((MPI_Comm, int *));
extern int MPIL_Comm_gps __ARGS((MPI_Comm, int, int *, int *));
extern int MPIL_Type_id __ARGS((MPI_Datatype, int *));
extern int MPIL_Trace_on __ARGS((void));
extern int MPIL_Trace_off __ARGS((void));
extern int MPIL_Signal __ARGS((MPI_Comm, int, int));
extern int MPIL_Request_set_name __ARGS((MPI_Request, char *));
extern char *MPIL_Request_get_name __ARGS((MPI_Request));

extern int PMPIL_Spawn __ARGS((MPI_Comm, char *, int, MPI_Comm *));
extern int PMPIL_Comm_parent __ARGS((MPI_Comm *));
extern int PMPIL_Universe_size __ARGS((int *));
extern int PMPIL_Comm_id __ARGS((MPI_Comm, int *));
extern int PMPIL_Comm_gps __ARGS((MPI_Comm, int, int *, int *));
extern int PMPIL_Type_id __ARGS((MPI_Datatype, int *));
extern int PMPIL_Trace_on __ARGS((void));
extern int PMPIL_Trace_off __ARGS((void));
extern int PMPIL_Signal __ARGS((MPI_Comm, int, int));
extern int PMPIL_Request_set_name __ARGS((MPI_Request, char *));
extern char *PMPIL_Request_get_name __ARGS((MPI_Request));

extern int XMPI_Buoy __ARGS((const char* name));
extern int XMPI_Coloron __ARGS((int red, int green, int blue));
extern int XMPI_Coloroff __ARGS((void));

/*
 * backward compatibility
 */
extern int MPI_Comm_get_contextid __ARGS((MPI_Comm, int *));
extern int PMPI_Comm_get_contextid __ARGS((MPI_Comm, int *));


/*
 * Put this in because if LAM_BUILDING is not defined (and it's only
 * defined while LAM is building itself), it can result in a warning
 * if you compile with "gcc -Wundef".  Hence, we ensure that
 * LAM_BUILDING is always defined.
 */
#ifndef LAM_BUILDING
#define LAM_BUILDING 0
#endif


/*
 * Conditional ROMIO support
 * Careful not to include it while we're building LAM (because it won't 
 * exist yet) 
 */
#if LAM_WANT_ROMIO & !LAM_BUILDING
#include <mpio.h>
#endif

#if defined(c_plusplus) || defined(__cplusplus)
}

/*
 * Conditional MPI 2 C++ bindings support
 * Careful not to include it while we're building LAM (because it won't 
 * exist yet) 
 */
#if LAM_WANT_MPI2CPP & !LAM_BUILDING
#include <mpicxx.h>
#endif
#endif

/*
 * This function should only be used if you are using external memory
 * management.  Please read the LAM/MPI Installation Guide for more
 * details.
 */
#include <stdlib.h>
void lam_handle_free(void *buf, size_t length);


#endif	/* _MPI_H */
