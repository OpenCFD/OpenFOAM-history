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
 * $Id: finalize.c,v 6.40 2004/03/02 03:43:29 jsquyres Exp $
 *
 *	Function:	- terminate the MPI session
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <lam_config.h>

#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>
#include <lam-ssi-rpi.h>
#include <rpisys.h>
#include <terror.h>
#include <kreq.h>
#include <trreq.h>
#include <typical.h>
#include <t_types.h>
#include <laminternal.h>
#include <lammpithreads.h>
#include <etc_misc.h>
#if LAM_WANT_IMPI
#include <impi.h>
#endif
#include <lam-ssi-rpi.h>
#include <lam-ssi-cr.h>
#include <lam-ssi-coll.h>


/*
 * local functions
 */
static int		waitall(void);
static void		free_comm_self(void);
static void		free_comms(void);
static void		free_dtype(void);
static void		free_env(void);
static void		free_errhdl(void);
static void		free_f77(void);
static void		free_op(void);
static void		free_rdtype(void);


/*@
   MPI_Finalize - Terminates MPI execution environment

Notes:

All processes must call this routine before exiting.  The number of
processes running `after` this routine is called is undefined; it is
best not to perform much more than a 'return rc' after calling
'MPI_Finalize'.

MPI mandates that the same thread invoke 'MPI_Init' (or
'MPI_Init_thread') and 'MPI_Finalize'; if a different thread invokes
'MPI_Finalize', 'MPI_ERR_OTHER' will be returned, and MPI will not be
finalized.

.N fortran

.N Errors
.N MPI_ERR_INTERN
. MPI_ERR_OTHER - Returned if a different thread attempts to finalize
MPI than the thread that initialized MPI.

.seealso MPI_Init, MPI_Init_thread

.N ACK
@*/
int 
MPI_Finalize(void)
{
	int		err;			/* error code */
	
	lam_initerr();
	lam_setfunc(BLKMPIFINAL);
/*
 * Ensure that if we started with MPI_Init_thread that this is the
 * same thread that invoked MPI_Init_thread.
 */
	if (lam_mpi_thread_level > MPI_THREAD_SINGLE && 
	    !lam_thread_compare(lam_thread_self(), lam_mpi_main_thread))
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIFINAL, 
				   lam_mkerr(MPI_ERR_ARG, EINVAL)));

	/* Per MPI-2:4.8, MPI_COMM_SELF must be freed first, before
	   anything else in MPI_FINALIZE, which allows user-specified
	   functions to be run at finalize time before MPI has really
	   shut down. */

	free_comm_self();

	LAM_TRACE(lam_tr_cffstart(BLKMPIFINAL));
/*
 * Block until all pending requests are done.
 */
	err = waitall();
	if (err != MPI_SUCCESS)
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIFINAL, err));
	
        /*
         * If C/R has been enabled and a cr module has been selected, call
         * MPI_Barrier.
         */
        if (lam_ssi_crmpi.lscrma_finalize != NULL)
	  MPI_Barrier(MPI_COMM_WORLD);
	
/*
 * IMPI finalization
 */
#if LAM_WANT_IMPI
	if (_kio.ki_rtf & RTF_IMPI) {
	  if (IMPI_Finalize() != 0)
	    return(lam_errfunc(MPI_COMM_WORLD, BLKMPIFINAL, err));
	}
#endif

/*
 * Cleanup.
 */
	lam_nukepublished();
	lam_ports_free();
	lam_free_onesided();

	free_env();
	free_rdtype();
	free_comms();
	free_op();
	free_dtype();
	free_errhdl();
	free_f77();

	/* Close the collective SSI modules */

	if (lam_ssi_coll_base_close() != 0)
	  return(lam_errfunc(MPI_COMM_NULL, BLKMPIFINAL,
			     lam_mkerr(MPI_ERR_INTERN, errno)));

	/* Finalize RPI SSI modules */

	if (RPI_FINALIZE_ALLPROCS())
	  return(lam_errfunc(MPI_COMM_NULL, BLKMPIFINAL,
			     lam_mkerr(MPI_ERR_INTERN, errno)));

        /* Finalize the active crmpi SSI module */

        if (lam_ssi_crmpi.lscrma_finalize != NULL &&
            lam_ssi_crmpi.lscrma_finalize() != 0)
          return(lam_errfunc(MPI_COMM_NULL, BLKMPIFINAL,
                             lam_mkerr(MPI_ERR_INTERN, errno)));

	LAM_TRACE(lam_tr_cffend(BLKMPIFINAL, -1, 0, 0, 0));

	lam_tr_off();
	lam_nuketrace();

	if (_kio.ki_rtf & RTF_MPISIGS)
	  lam_mpi_reset_sighandlers();

	/* Shut down all MPI SSI kinds */
	/* MPI SSI kind: RPI */

	lam_ssi_rpi_base_close();

        /* MPI SSI kind: CRMPI */

        lam_ssi_crmpi_base_close();

        /* Shut down SSI itself */

        lam_ssi_base_close();

        /* Kill all the procs */

 	lam_nukeprocs();

        /* Free all memory pool stuff */

        lam_mp_finalize(-1);

	/* Free the copy of command line arguments */

	ao_free(lam_mpi_ao);

	/* Final shutdown */

	lam_flfinal = 1;
	lam_kexit(0);

	lam_resetfunc(BLKMPIFINAL);
	lam_nukefunc();
	lam_updown_finalize();

	if (lam_mpi_thread_level > MPI_THREAD_SINGLE)
	  lam_mpi_thread_finalize();

	/* Signal TotalView DLL that comms have changed (vanished)*/
	++lam_tv_comm_seqnum;

	return(MPI_SUCCESS);
}


/*
 *	waitall
 *
 *	Function:	- wait till all pending requests are done
 *			- window handler requests are ingored
 *	Returns:	- MPI_SUCCESS or error code
 */
static int
waitall(void)
{
	int		err;			/* error code */
	int		nreqs;			/* # pending requests */
	MPI_Request	*reqs;			/* request array */
	int		i;

	nreqs = _mpi_req_getn();
	if (nreqs == 0) return(MPI_SUCCESS);

	reqs = (MPI_Request *) malloc((unsigned) nreqs * sizeof(MPI_Request));
	if (reqs == 0) {
		return(lam_mkerr(MPI_ERR_OTHER, errno));
	}

	_mpi_req_get(nreqs, reqs);
/*
 * Ignore window handler requests.
 */
	for (i = 0; i < nreqs; i++) {
	    if (reqs[i]->rq_hdlr == lam_osd_handler) {
		reqs[i] = MPI_REQUEST_NULL;
	    }
	}

	/* Must use LAM_MPI_C_STATUSES_IGNORE as MPISYSF #undefs
	   MPI_STATUSES_IGNORE due to name clash */
	err = MPI_Waitall(nreqs, reqs, LAM_MPI_C_STATUSES_IGNORE);

	free((char *) reqs);
	return(err);
}


/*
 *	free_errhdl
 *
 *	Function:	- deallocate pre-defined error handles
 */
static void
free_errhdl(void)
{
/*
 * Nothing to free.
 */
}


/*
 *	free_comm_self
 *
 *	Function:	- deallocate pre-defined MPI_COMM_SELF
 */
static void
free_comm_self(void)
{
        MPI_Errhandler err;
	int key;
	struct _attr *p;

	/* First, free all keys so that we invoke any user handlers on
	   attributes on COMM_SELF.  Ignore any errors in the
	   process. */

	if (MPI_COMM_SELF->c_keys) {
		p = (struct _attr *) ah_top(MPI_COMM_SELF->c_keys);
		while (p) {
			key = p->a_key;
			MPI_Comm_delete_attr(MPI_COMM_SELF, key);
			MPI_Comm_free_keyval(&key);

			p = (struct _attr *) ah_next(MPI_COMM_SELF->c_keys,
						     (void *) p);
		}
		ah_free(MPI_COMM_SELF->c_keys);
	}

	/* Tell the collective SSI module that this communicator is
	   gone.  It's too late to worry about errors. */

	lam_ssi_coll_base_finalize_comm(MPI_COMM_SELF);

	/* Free the error handler, if a user error handler was defined
 	   on it (otherwise, we have a memory leak) */

	err = MPI_COMM_SELF->c_errhdl;
	if (err != 0 && err != MPI_ERRHANDLER_NULL && 
	    err != MPI_ERRORS_ARE_FATAL && err != MPI_ERRORS_RETURN)
	  MPI_Errhandler_free(&MPI_COMM_SELF->c_errhdl);

	/* Free the group and context ID */

	lam_rmcid(MPI_COMM_SELF->c_contextid);
	free((char *) MPI_COMM_SELF->c_group);
}


/*
 *	free_comms
 *
 *	Function:	- deallocate all other pre-defined communicators
 */
static void
free_comms(void)
{
        MPI_Errhandler err;
	MPI_Comm *comm;

	/* Go through the comm list and let the coll SSI modules
	   finalize each of them.  MPI_COMM_WORLD and lam_comm_parent
	   will have additional shutdown actions, but all other
	   user-allocated communicators will just be freed.  It's too
	   late to worry about errors, so don't check the return
	   status of the finalize API call.  Note that this already
	   happened on MPI_COMM_SELF (due to special MPI-2:4.8
	   ordering of freeing attributes on MPI_COMM_SELF), so be
	   sure to skip it here. */

	for (comm = al_top(lam_comms); comm != NULL; 
	     comm = al_next(lam_comms, comm))
	  if (*comm != MPI_COMM_SELF)
	    lam_ssi_coll_base_finalize_comm(*comm);

/*
 * Free the "parent" communicator if any.
 */
	if (lam_comm_parent != MPI_COMM_NULL)
	  lam_comm_free(lam_comm_parent);

/*
 * Free the "world" communicator.
 * Free the error handler, if a user error handler was defined on it
 * (otherwise, we have a memory leak)
 */
	err = MPI_COMM_WORLD->c_errhdl;
	if (err != 0 && err != MPI_ERRHANDLER_NULL && 
	    err != MPI_ERRORS_ARE_FATAL && err != MPI_ERRORS_RETURN)
	  MPI_Errhandler_free(&MPI_COMM_WORLD->c_errhdl);
	lam_rmcid(MPI_COMM_WORLD->c_contextid);
	free((char *) MPI_COMM_WORLD->c_group);
	ah_free(MPI_COMM_WORLD->c_keys);

	al_free(lam_comms);

	lam_nukecids();
}


/*
 *	free_dtype
 *
 *	Function:	- deallocate basic (intrinsic) datatypes
 */
static void
free_dtype(void)
{
/*
 * Nothing to free.
 */
}


/*
 *	free_rdtype
 *
 *	Function:	- deallocate reduction datatypes
 */
static void
free_rdtype(void)
{
	lam_type_free(MPI_2INT);
	lam_type_free(MPI_FLOAT_INT);
	lam_type_free(MPI_DOUBLE_INT);
	lam_type_free(MPI_LONG_DOUBLE_INT);
	lam_type_free(MPI_LONG_INT);
	lam_type_free(MPI_SHORT_INT);
	lam_type_free(MPI_F_2INTEGER);
	lam_type_free(MPI_F_2REAL);
	lam_type_free(MPI_F_2DOUBLE_PRECISION);
}


/*
 *	free_op
 *
 *	Function:	- deallocate intrinsic reduction operations
 */
static void
free_op(void)
{
/*
 * Nothing to free.
 */
}


/*
 *	free_env
 *
 *	Function:	- deallocate environment attributes
 */
static void
free_env(void)
{
	MPI_Attr_delete(MPI_COMM_WORLD, MPI_TAG_UB);
	MPI_Attr_delete(MPI_COMM_WORLD, MPI_HOST);
	MPI_Attr_delete(MPI_COMM_WORLD, MPI_IO);
	MPI_Attr_delete(MPI_COMM_WORLD, MPI_WTIME_IS_GLOBAL);
	MPI_Attr_delete(MPI_COMM_WORLD, MPI_UNIVERSE_SIZE);
	if (lam_appnum >= 0) {
	    MPI_Attr_delete(MPI_COMM_WORLD, MPI_APPNUM);
	}

	lam_nukekeys();
}


/*
 *	free_f77
 *
 *	Function:	- deallocate f77 support structures
 */
static void
free_f77(void)
{
	free((char *) lam_F_handles);
}
