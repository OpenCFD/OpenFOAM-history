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
 *	Function:	- F77 support
 */

#include <lam_config.h>

#include <stdlib.h>
#include <string.h>

#include <args.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>
#include <terror.h>
#include <typical.h>
#include <etc_misc.h>
#include <sfh.h>

/*
 * public functions
 */
char			*lam_F2C_string();
int			lam_F2C_argv();
void			lam_F2C_stat();
void			lam_C2F_string();
void			lam_C2F_stat();
int			lam_F_alloc_hdl();
int			lam_F_find_hdl();
int			lam_F_make_hdl();
void			lam_F_free_hdl();
void			mpi_null_copy_fn_(void);
void			mpi_null_delete_fn_(void);
void mpi_comm_null_copy_fn_(void);
void mpi_comm_null_delete_fn_(void);
void mpi_type_null_copy_fn_(void);
void mpi_type_null_delete_fn_(void);
void mpi_win_null_copy_fn_(void);
void mpi_win_null_delete_fn_(void);

/*
 * public variables
 */
int			lam_F_nhdl = 0;		/* number of handles in use */
int			lam_F_maxhdl = 0;	/* maximum handle in use */
int			lam_F_sizehdlarray = 0;	/* size of handle array */
void			**lam_F_handles = 0;	/* array of handles */
char			*lam_F_bottom;
void			*lam_F_argvnull;
void			*lam_F_argvsnull;
void			*lam_F_errorcodes_ignore;
void			*lam_F_status_ignore;
void			*lam_F_statuses_ignore;

/*
 * These variables are used to link against MPI F77 constants which
 * correspond to addresses, e.g. MPI_BOTTOM, and are implemented via common
 * blocks.  They must have the same size and alignment constraints as the
 * corresponding F77 common blocks.
 */
#if LAM_SIZEOF_LONG_DOUBLE == 16
LAM_LONG_DOUBLE		mpi_bottom_[1];
LAM_LONG_DOUBLE		mpi_argv_null_[1];
LAM_LONG_DOUBLE		mpi_argvs_null_[1];
LAM_LONG_DOUBLE		mpi_errcodes_ignore_[1];
LAM_LONG_DOUBLE		mpi_status_ignore_[1];
LAM_LONG_DOUBLE		mpi_statuses_ignore_[1];
#else
double			mpi_bottom_[2];
double			mpi_argv_null_[2];
double			mpi_argvs_null_[2];
double			mpi_errcodes_ignore_[2];
double			mpi_status_ignore_[2];
double			mpi_statuses_ignore_[2];
#endif

/*
 * Null functions.
 */
void mpi_null_copy_fn_(void) { }
void mpi_null_delete_fn_(void) { }
void mpi_comm_null_copy_fn_(void) { }
void mpi_comm_null_delete_fn_(void) { }
void mpi_type_null_copy_fn_(void) { }
void mpi_type_null_delete_fn_(void) { }
void mpi_win_null_copy_fn_(void) { }
void mpi_win_null_delete_fn_(void) { }

/*
 *	lam_F_alloc_hdl
 *
 *	Function:	- allocate entry in type lookup table
 *	Returns:	- table index or LAMERROR
 */
int
lam_F_alloc_hdl(void)
{
	int		i;
	unsigned	newsize;
	void		**p;
/*
 * If there is free space at the top of the table, allocate it.
 * Note that lam_F_maxhdl == (lam_F_nhdl - 1) from init.c:init_f77().
 */
	if (lam_F_maxhdl < (lam_F_sizehdlarray - 2)) {
		++lam_F_nhdl;
		return(++lam_F_maxhdl);
	}
/*
 * If there is room in the table, search for an empty entry.
 */
	if (lam_F_nhdl < (lam_F_sizehdlarray - 1)) {
		for (i = LAM_MAXF77PREDEF, p = lam_F_handles + LAM_MAXF77PREDEF;
		     i < lam_F_sizehdlarray; ++i, ++p) {

			if (*p == 0) {
				++lam_F_nhdl;
				return(i);
			}
		}

		errno = EIMPOSSIBLE;
		return(LAMERROR);
	}
/*
 * Double the table size and allocate entry from its top.
 */
	newsize = 2 * lam_F_sizehdlarray;
	p = (void **) realloc((char *) lam_F_handles,
				(unsigned) newsize * sizeof(void *));
	if (p == 0) return(LAMERROR);

	memset((char *) (p + lam_F_sizehdlarray), 0,
		lam_F_sizehdlarray * sizeof(void *));

	lam_F_handles = p;
	lam_F_sizehdlarray = newsize;

	++lam_F_nhdl;
	return(++lam_F_maxhdl);
}

/*
 *	lam_F_find_hdl
 *
 *	Function:	- locate given entry
 *	Accepts:	- C handle
 *	Returns:	- table index or LAMERROR
 */
int
lam_F_find_hdl(void *handle)
{
	int		i;
	int		rem;
	void		**p;

	rem = lam_F_nhdl;
	for (i = 0, p = lam_F_handles;
			(i < lam_F_sizehdlarray) && (rem > 0); ++i, ++p) {
		if (*p) {
			--rem;
			if (*p == handle) return(i);
		}
	}

	return(LAMERROR);
}

/*
 *	lam_F_free_hdl
 *
 *	Function:	- free entry in type lookup table
 *	Accepts:	- table index
 */
void
lam_F_free_hdl(int index)
{
	void		**p;

	if (index < LAM_MAXF77PREDEF) return;

	p = lam_F_handles + index;
	*p = 0;
	--lam_F_nhdl;

	if (index == lam_F_maxhdl) {
		for (--lam_F_maxhdl, --p;
				lam_F_maxhdl >= 0; --lam_F_maxhdl, --p) {
			if (*p) break;
		}
	}
}

/*
 *	lam_F_make_hdl
 *
 *	Function:	- create a type mapping
 *			- set error code appropriately
 *	Accepts:	- F77 handle (out)
 *			- error code (in-out)
 *			- C handle
 *	Returns:	- 1 success or 0 error
 */
int
lam_F_make_hdl(int *f77_handle, int *errcode, void *c_handle, int func)
{
	if ((*errcode == MPI_SUCCESS) && (c_handle != 0)) {

		if ((*f77_handle = lam_F_alloc_hdl()) < 0) {
			lam_setfunc_m(func);
			*errcode = lam_errfunc(MPI_COMM_WORLD, func,
					lam_mkerr(MPI_ERR_OTHER, errno));
			lam_resetfunc_m(func);
			*f77_handle = -1;
		} else {
			lam_F_handles[*f77_handle] = c_handle;
		}
	} else {
		*f77_handle = -1;
	}

	return(*f77_handle >= 0);
}

/*
 *	lam_C2F_stat
 *
 *	Function:	- copy MPI_Status from C to F77
 *	Accepts:	- C status
 *			- F77 status (out)
 */
void
lam_C2F_stat(MPI_Status *cstat, int *f77stat)
{
	*((MPI_Status *) f77stat) = *cstat;
}

/*
 *	lam_F2C_stat
 *
 *	Function:	- copy MPI_Status from F77 to C
 *	Accepts:	- F77 status
 *			- C status (out)
 */
void
lam_F2C_stat(int *f77stat, MPI_Status *cstat)
{
	*cstat = *((MPI_Status *) f77stat);
}

/*
 *	lam_F2C_string
 * 
 *	Function:	- creates a C string from an F77 string
 *	Accepts:	- F77 string
 *			- string length
 *	Returns:	- C string or NULL
 */
char *
lam_F2C_string(char *str, int len)
{
	char		*cstr;
	char		*end;
	int		i;
/*
 * Leading and trailing blanks are discarded.
 */
	end = str + len - 1;
	
	for (i = 0; (i < len) && (*str == ' '); i++, str++) ;

	if (i >= len) {
		len = 0;
	} else {
		for ( ; (end > str) && (*end == ' '); end--) ;

		len = end - str + 1;
	}
/*
 * Allocate space for the C string.
 */
	if ((cstr = malloc(len + 1)) == 0) {
		return((char *) 0);
	}
/*
 * Copy F77 string into C string and NULL terminate it.
 */
	if (len > 0) {
		lam_strncpy(cstr, str, len);
	}
	cstr[len] = 0;

	return(cstr);
}

/*
 *	lam_C2F_string
 * 
 *	Function:	- copy a C string into a F77 string
 *	Accepts:	- C string
 *			- F77 string (out)
 *			- F77 string length
 */
void
lam_C2F_string(char *cstr, char *f77str, int len)
{
	int		i;
	
	lam_strncpy(f77str, cstr, len);

	for (i = strlen(cstr); i < len; i++) {
		f77str[i] = ' ';
	}
}

/*
 *	lam_F2C_argv
 * 
 *	Function:	- creates a C argument vector from an F77 array 
 *			  of strings (terminated by a blank string)
 *	Accepts:	- F77 array of strings
 *			- length of strings
 *			- C argument vector (out)
 *	Returns:	- 0 or LAMERROR
 */
int
lam_F2C_argv(char *array, int len, char ***argv)
{
	int		argc = 0;
	char		*cstr;

	*argv = 0;
	
	while (1) {
		cstr = lam_F2C_string(array, len);
		if (cstr == 0) {
			sfh_argv_free(*argv);
			return(LAMERROR);
		}

		if (*cstr == 0) break;

		if (sfh_argv_add(&argc, argv, cstr)) {
                        sfh_argv_free(*argv);
			return(LAMERROR);
		}

		free(cstr);
		array += len;
	}

	return(0);
}
