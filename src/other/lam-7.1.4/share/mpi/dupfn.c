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
 * $Id: dupfn.c,v 6.7 2003/06/02 04:03:30 brbarret Exp $
 *
 *	Function:	- pre-defined simple attribute copy functions
 *	Accepts:	- communicator/datatytpe/window
 *			- key value
 *			- extra state info
 *			- in attribute value
 *			- out attribute value (out)
 *			- flag (out)
 *	Returns:	- MPI_SUCCESS
 */

#include <mpi.h>
#include <mpisys.h>

/*@

MPI_DUP_FN - Built-in MPI function to simple-mindedly copy
communicator attributes.

Input Parameters:
+ comm - communicator (handle)
. key - key value (integer)
. extra - extra state
. attrin - in value for attribute

Output Parameters:
. attrout - out value for attribute
- flag - 1 if attribute copied successfully, 0 otherwise (logical)

.N Wrapper MPI_COMM_DUP_FN

Additional:

Note that the C typedefs 'MPI_Copy_function' and
'MPI_MPI_Delete_function' are also deprecated, in favor of their new
names - 'MPI_Comm_copy_function', 'MPI_Comm_delete_function'.

Additionally, the MPI constants 'MPI_NULL_COPY_FN' and
'MPI_NULL_DELETE_FN' are also deprecated in favor of their new names -
'MPI_COMM_NULL_COPY_FN' and 'MPI_COMM_NULL_DELETE_FN'.

.N ACK
@*/
int MPI_DUP_FN(MPI_Comm comm, int key, void *extra, 
	       void *attrin, void *attrout, int *flag)
{
	*flag = 1;
	*((void **) attrout) = attrin;

	return(MPI_SUCCESS);
}

/*@

MPI_COMM_DUP_FN - Built-in MPI function to simple-mindedly copy
communicator attributes.

Input Parameters:
+ comm - communicator (handle)
. key - key value (integer)
. extra - extra state
. attrin - in value for attribute

Output Parameters:
. attrout - out value for attribute
- flag - 1 if attribute copied successfully, 0 otherwise (logical)

Notes:

This function blindly byte-copies attributes on communicators.  It is
the default atrribute-copying function for communicators.  This
function will always return 'MPI_SUCCESS', and 'flag' will always be
set to 1.

The actual copy is performed by the code

.vb
	*((void **) attrout) = attrin;
.ve

If this simple assignment is not sufficient for your attributes, you
will need to reassign the attribute-copying function on relevant
keyvals on communicators with the 'MPI_Comm_keyval_create' function.

.N fortran

.seealso: MPI_Comm_keyval_create

.N ACK
@*/
int MPI_COMM_DUP_FN(MPI_Comm comm, int key, void *extra, 
		    void *attrin, void *attrout, int *flag)
{
	*flag = 1;
	*((void **) attrout) = attrin;

	return(MPI_SUCCESS);
}

/*@

MPI_TYPE_DUP_FN - Built-in MPI function to simple-mindedly copy
datatype attributes.

Input Parameters:
+ dtype - communicator (handle)
. key - key value (integer)
. extra - extra state
. attrin - in value for attribute

Output Parameters:
. attrout - out value for attribute
- flag - 1 if attribute copied successfully, 0 otherwise (logical)

Notes:

This function blindly byte-copies attributes on datatypes.  It is the
default atrribute-copying function for datatypes.  This function will
always return 'MPI_SUCCESS', and 'flag' will always be set to 1.

The actual copy is performed by the code

.vb
	*((void **) attrout) = attrin;
.ve

If this simple assignment is not sufficient for your attributes, you
will need to reassign the attribute-copying function on relevant
keyvals on datatypes with the 'MPI_Type_keyval_create' function.

.N fortran

.seealso: MPI_Type_keyval_create

.N ACK
@*/
int MPI_TYPE_DUP_FN(MPI_Datatype dtype, int key, 
		    void *extra, void *attrin, void *attrout, 
		    int *flag)
{
	*flag = 1;
	*((void **) attrout) = attrin;

	return(MPI_SUCCESS);
}

/*@

MPI_WIN_DUP_FN - Built-in MPI function to simple-mindedly copy window
attributes.

Input Parameters:
+ win - window (handle)
. key - key value (integer)
. extra - extra state
. attrin - in value for attribute

Output Parameters:
. attrout - out value for attribute
- flag - 1 if attribute copied successfully, 0 otherwise (logical)

Notes:

This function blindly byte-copies attributes on windows.  It is the
default atrribute-copying function for windows.  This function will
always return 'MPI_SUCCESS', and 'flag' will always be set to 1.

The actual copy is performed by the code

.vb
	*((void **) attrout) = attrin;
.ve

If this simple assignment is not sufficient for your attributes, you
will need to reassign the attribute-copying function on relevant
keyvals on windows with the 'MPI_Win_keyval_create' function.

.N fortran

.seealso: MPI_Win_keyval_create

.N ACK
@*/
int MPI_WIN_DUP_FN(MPI_Win win, int key, void *extra, 
		   void *attrin, void *attrout, int *flag)
{
	*flag = 1;
	*((void **) attrout) = attrin;

	return(MPI_SUCCESS);
}
