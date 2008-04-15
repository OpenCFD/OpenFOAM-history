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
 * $Id: handles.c,v 1.5 2002/12/11 19:15:11 jsquyres Exp $
 *
 *	Function:	- handle conversion C<->Fortran 
 */

#include <mpi.h>
#include <mpisys.h>
#include <MPISYSF.h>
#include <rpisys.h>


/*
 *	MPI_Comm_f2c
 *
 *	Function:	- communicator handle conversion Fortran->C
 *	Accepts:	- Fortran communicator handle 
 *	Returns:	- C communicator handle
 */
/*@
   MPI_Comm_f2c - Convert a communicator handle from its Fortran representation to its C representation

Input Parameter:
. f_handle - Fortran communicator handle

Return Value:
. comm - C communicator handle

.N fortran

.N CONVERSION_FUNCTIONS

.seealso MPI_Comm_c2f(3)

.N WEB
@*/
MPI_Comm
MPI_Comm_f2c(MPI_Fint f_handle)
{
	return(GETHDL(f_handle));
}

/*
 *	MPI_Comm_c2f
 *
 *	Function:	- communicator handle conversion C->Fortran
 *	Accepts:	- C communicator handle 
 *	Returns:	- Fortran communicator handle
 */
/*@
   MPI_Comm_c2f - Convert a communicator handle from its C representation to its Fortran representation

Input Parameter:
. comm - C communicator handle

Return Value:
. f_handle - Fortran communicator handle

.N fortran

.N CONVERSION_FUNCTIONS

.seealso MPI_Comm_c2f(3)

.N WEB
@*/
MPI_Fint
MPI_Comm_c2f(MPI_Comm comm)
{
	int		f77_handle;

        if (MPI_COMM_NULL == comm) {
            return (MPI_Fint) -1;
        }

	if (comm->c_f77handle < 0) {
		if ((f77_handle = lam_F_alloc_hdl()) >= 0) {
			lam_F_handles[f77_handle] = comm;
			comm->c_f77handle = f77_handle;
		}
	}

	return((MPI_Fint) comm->c_f77handle);
}

/*
 *	MPI_Group_f2c
 *
 *	Function:	- group handle conversion Fortran->C
 *	Accepts:	- Fortran group handle 
 *	Returns:	- C group handle
 */
/*@
   MPI_Group_f2c - Convert a group handle from its Fortran representation to its C representation

Input Parameter:
. f_handle - Fortran group handle

Return Value:
. group - C group handle

.N fortran

.N CONVERSION_FUNCTIONS

.seealso MPI_Group_c2f(3)

.N WEB
@*/
MPI_Group
MPI_Group_f2c(MPI_Fint f_handle)
{
	return(GETHDL(f_handle));
}

/*
 *	MPI_Group_c2f
 *
 *	Function:	- group handle conversion C->Fortran
 *	Accepts:	- C group handle 
 *	Returns:	- Fortran group handle
 */
/*@
   MPI_Group_c2f - Convert a group handle from its C representation to its Fortran representation

Input Parameter:
. group - C group handle

Return Value:
. f_handle - Fortran group handle

.N fortran

.N CONVERSION_FUNCTIONS

.seealso MPI_Group_c2f(3)

.N WEB
@*/
MPI_Fint
MPI_Group_c2f(MPI_Group group)
{
	int		f77_handle;

        if (MPI_GROUP_NULL == group) {
            return (MPI_Fint) -1;
        }

	if (group->g_f77handle < 0) {
		if ((f77_handle = lam_F_alloc_hdl()) >= 0) {
			lam_F_handles[f77_handle] = group;
			group->g_f77handle = f77_handle;
		}
	}

	return((MPI_Fint) group->g_f77handle);
}

/*
 *	MPI_Type_f2c
 *
 *	Function:	- datatype handle conversion Fortran->C
 *	Accepts:	- Fortran datatype handle 
 *	Returns:	- C datatype handle
 */
/*@
   MPI_Type_f2c - Convert a datatype handle from its Fortran representation to its C representation

Input Parameter:
. f_handle - Fortran datatype handle

Return Value:
. datatype - C datatype handle

.N fortran

.N CONVERSION_FUNCTIONS

.seealso MPI_Type_c2f(3)

.N WEB
@*/
MPI_Datatype
MPI_Type_f2c(MPI_Fint f_handle)
{
	return(GETHDL(f_handle));
}

/*
 *	MPI_Type_c2f
 *
 *	Function:	- datatype handle conversion C->Fortran
 *	Accepts:	- C datatype handle 
 *	Returns:	- Fortran datatype handle
 */
/*@
   MPI_Type_c2f - Convert a datatype handle from its C representation to its Fortran representation

Input Parameter:
. datatype - C datatype handle

Return Value:
. f_handle - Fortran datatype handle

.N fortran

.N CONVERSION_FUNCTIONS

.seealso MPI_Type_c2f(3)

.N WEB
@*/
MPI_Fint
MPI_Type_c2f(MPI_Datatype datatype)
{
	int		f77_handle;

        if (MPI_DATATYPE_NULL == datatype) {
            return (MPI_Fint) -1;
        }

	if (datatype->dt_f77handle < 0) {
		if ((f77_handle = lam_F_alloc_hdl()) >= 0) {
			lam_F_handles[f77_handle] = datatype;
			datatype->dt_f77handle = f77_handle;
		}
	}

	return((MPI_Fint) datatype->dt_f77handle);
}

/*
 *	MPI_Request_f2c
 *
 *	Function:	- request handle conversion Fortran->C
 *	Accepts:	- Fortran request handle 
 *	Returns:	- C request handle
 */
/*@
   MPI_Request_f2c - Convert a request handle from its Fortran representation to its C representation

Input Parameter:
. f_handle - Fortran request handle

Return Value:
. request - C request handle

.N fortran

.N CONVERSION_FUNCTIONS

.seealso MPI_Comm_c2f(3)

.N WEB
@*/
MPI_Request
MPI_Request_f2c(MPI_Fint f_handle)
{
	return(GETHDL(f_handle));
}

/*
 *	MPI_Request_c2f
 *
 *	Function:	- request handle conversion C->Fortran
 *	Accepts:	- C request handle 
 *	Returns:	- Fortran request handle
 */
/*@
   MPI_Request_c2f - Convert a request handle from its C representation to its Fortran representation

Input Parameter:
. request - C request handle

Return Value:
. f_handle - Fortran request handle

.N fortran

.N CONVERSION_FUNCTIONS

.seealso MPI_Request_c2f(3)

.N WEB
@*/
MPI_Fint
MPI_Request_c2f(MPI_Request request)
{
	int		f77_handle;

        if (MPI_REQUEST_NULL == request) {
            return (MPI_Fint) -1;
        }

	if (request->rq_f77handle < 0) {
		if ((f77_handle = lam_F_alloc_hdl()) >= 0) {
			lam_F_handles[f77_handle] = request;
			request->rq_f77handle = f77_handle;
		}
	}

	return((MPI_Fint) request->rq_f77handle);
}

/*
 *	MPI_Op_f2c
 *
 *	Function:	- operation handle conversion Fortran->C
 *	Accepts:	- Fortran operation handle 
 *	Returns:	- C operation handle
 */
/*@
   MPI_Op_f2c - Convert an op handler from its Fortran representation to its C representation

Input Parameter:
. f_handle - Fortran op handle

Return Value:
. op - C op handle

.N fortran

.N CONVERSION_FUNCTIONS

.seealso MPI_Op_c2f(3)

.N WEB
@*/
MPI_Op
MPI_Op_f2c(MPI_Fint f_handle)
{
	return(GETHDL(f_handle));
}

/*
 *	MPI_Op_c2f
 *
 *	Function:	- operation handle conversion C->Fortran
 *	Accepts:	- C operation handle 
 *	Returns:	- Fortran operation handle
 */
/*@
   MPI_Op_c2f - Convert an op handle from its C representation to its Fortran representation

Input Parameter:
. op - C op handle

Return Value:
. f_handle - Fortran op handle

.N fortran

.N CONVERSION_FUNCTIONS

.seealso MPI_Op_c2f(3)

.N WEB
@*/
MPI_Fint
MPI_Op_c2f(MPI_Op op)
{
	int		f77_handle;

        if (MPI_OP_NULL == op) {
            return (MPI_Fint) -1;
        }

	if (op->op_f77handle < 0) {
		if ((f77_handle = lam_F_alloc_hdl()) >= 0) {
			lam_F_handles[f77_handle] = op;
			op->op_f77handle = f77_handle;
		}
	}

	return((MPI_Fint) op->op_f77handle);
}

/*
 *	MPI_Info_f2c
 *
 *	Function:	- info handle conversion Fortran->C
 *	Accepts:	- Fortran info handle 
 *	Returns:	- C info handle
 */
/*@
   MPI_Info_f2c - Convert a info handle from its Fortran representation to its C representation

Input Parameter:
. f_handle - Fortran info handle

Return Value:
. info - C info handle

.N fortran

.N CONVERSION_FUNCTIONS

.seealso MPI_Info_c2f(3)

.N WEB
@*/
MPI_Info
MPI_Info_f2c(MPI_Fint f_handle)
{
	return(GETHDL(f_handle));
}

/*
 *	MPI_Info_c2f
 *
 *	Function:	- info handle conversion C->Fortran
 *	Accepts:	- C info handle 
 *	Returns:	- Fortran info handle
 */
/*@
   MPI_Info_c2f - Convert an info handle from its C representation to its Fortran representation

Input Parameter:
. info - C info handle

Return Value:
. f_handle - Fortran info handle

.N fortran

.N CONVERSION_FUNCTIONS

.seealso MPI_Info_c2f(3)

.N WEB
@*/
MPI_Fint
MPI_Info_c2f(MPI_Info info)
{
	int		f77_handle;

        if (MPI_INFO_NULL == info) {
            return (MPI_Fint) -1;
        }

	if (info->info_f77handle < 0) {
		if ((f77_handle = lam_F_alloc_hdl()) >= 0) {
			lam_F_handles[f77_handle] = info;
			info->info_f77handle = f77_handle;
		}
	}

	return((MPI_Fint) info->info_f77handle);
}

/*
 *	MPI_Win_f2c
 *
 *	Function:	- window handle conversion Fortran->C
 *	Accepts:	- Fortran window handle 
 *	Returns:	- C window handle
 */
/*@
   MPI_Win_f2c - Convert a window handle from its Fortran representation to its C representation

Input Parameter:
. f_handle - Fortran window handle

Return Value:
. win - C window handle

.N fortran

.N CONVERSION_FUNCTIONS

.seealso MPI_Comm_Win(3)

.N WEB
@*/
MPI_Win
MPI_Win_f2c(MPI_Fint f_handle)
{
	return(GETHDL(f_handle));
}

/*
 *	MPI_Win_c2f
 *
 *	Function:	- window handle conversion C->Fortran
 *	Accepts:	- C window handle 
 *	Returns:	- Fortran window handle
 */
/*@
   MPI_Win_c2f - Convert a window handle from its C representation to its Fortran representation

Input Parameter:
. win - C window handle

Return Value:
. f_handle - Fortran window handle

.N fortran

.N CONVERSION_FUNCTIONS

.seealso MPI_Win_c2f(3)

.N WEB
@*/
MPI_Fint
MPI_Win_c2f(MPI_Win win)
{
	int		f77_handle;

        if (MPI_WIN_NULL == win) {
            return (MPI_Fint) -1;
        }

	if (win->w_f77handle < 0) {
		if ((f77_handle = lam_F_alloc_hdl()) >= 0) {
			lam_F_handles[f77_handle] = win;
			win->w_f77handle = f77_handle;
		}
	}

	return((MPI_Fint) win->w_f77handle);
}


/*
 *	MPI_Status_f2c
 *
 *	Function:	- status conversion Fortran->C
 *	Accepts:	- Fortran status
 *			- C status (out)
 *	Returns:	- MPI_SUCCESS
 */
/*@
   MPI_Status_f2c - Convert a status from its Fortran representation to its C representation

Input Parameter:
. f_status - Fortran status

Output Parameter:
. c_status - C status

.N fortran

.N Errors
.N MPI_SUCCESS

.seealso MPI_Status_c2f(3)

.N WEB
@*/
int
MPI_Status_f2c(MPI_Fint *f_status, MPI_Status *c_status)
{
	*c_status = *((MPI_Status *) f_status);
	return(MPI_SUCCESS);
}


/*
 *	MPI_Status_c2f
 *
 *	Function:	- status conversion C->Fortran
 *	Accepts:	- pointer C status
 *			- Fortran status (out)
 *	Returns:	- MPI_SUCCESS
 */
/*@
   MPI_Status_c2f - Convert a status from its C representation to its Fortran representation

Input Parameter:
. c_status - C status

Return Value:
. f_status - Fortran status

.N fortran

.N CONVERSION_FUNCTIONS

.seealso MPI_Status_c2f(3)

.N WEB
@*/
int
MPI_Status_c2f(MPI_Status *c_status, MPI_Fint *f_status)
{
	*((MPI_Status *) f_status) = *c_status;
	return(MPI_SUCCESS);
}


/*
 *	MPI_Errhandler_f2c
 *      This actually isn't an official MPI function, but ROMIO uses it
 *
 *	Function:	- communicator handle conversion Fortran->C
 *	Accepts:	- Fortran communicator handle 
 *	Returns:	- C communicator handle
 */
/*@
   MPI_Errhandler_f2c - Convert a error handler from its Fortran representation to its C representation

Input Parameter:
. f_handle - Fortran error handler

Return Value:
. errh - C error handler

Description:

This is a non-standard MPI routine.  It is mainly here because ROMIO
needs it.

.N fortran

.N CONVERSION_FUNCTIONS

.seealso MPI_Errhandler_c2f(3)

.N WEB
@*/
MPI_Errhandler
MPI_Errhandler_f2c(MPI_Fint f_handle)
{
	return(GETHDL(f_handle));
}

/*
 *	MPI_Errhandler_c2f
 *      This actually isn't an official MPI function, but ROMIO uses it
 *
 *	Function:	- communicator handle conversion C->Fortran
 *	Accepts:	- C communicator handle 
 *	Returns:	- Fortran communicator handle
 */
/*@
   MPI_Errhandler_c2f - Convert an error handler from its C representation to its Fortran representation

Input Parameter:
. err - C error handler

Return Value:
. f_handle - Fortran error handler

Description:

This is a non-standard MPI routine.  It is mainly here because ROMIO
needs it.

.N fortran

.N CONVERSION_FUNCTIONS

.seealso MPI_Errhandler_c2f(3)

.N WEB
@*/
MPI_Fint
MPI_Errhandler_c2f(MPI_Errhandler err)
{
	int		f77_handle;

        if (MPI_ERRHANDLER_NULL == err) {
            return (MPI_Fint) -1;
        }

	if (err->eh_f77handle < 0) {
		if ((f77_handle = lam_F_alloc_hdl()) >= 0) {
			lam_F_handles[f77_handle] = err;
			err->eh_f77handle = f77_handle;
		}
	}

	return((MPI_Fint) err->eh_f77handle);
}

