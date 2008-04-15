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
 * $Id: mpil_trace.c,v 6.5 2002/12/11 19:15:13 jsquyres Exp $
 *
 *	Function:	- toggles tracing status
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>

/*
 *	MPIL_Trace_on
 *
 *	Function:	- enables tracing; begins segment
 */
/*@

MPIL_Trace_on - LAM/MPI-specific function to enable run-time tracing

Notes:

These functions give the application the flexibility to generate
traces only during certain interesting phases of the application''s
execution.  This technique can considerably reduce the size of the
trace files and burden of displaying them.
 
Both functions are collective over the 'MPI_COMM_WORLD' communicator.
In typical usage, the '-toff' option of 'mpirun'(1) would be used to
enable tracing, but start with the runtime switch in the off position.
At the beginning of an interesting phase of program execution,
'MPIL_Trace_on' would be called.  'MPIL_Trace_off' would be called
after the interesting phase.  Tracing can be turned on and off many
times.  Each period of tracing eventually forms a trace segment in the
trace file extracted by 'lamtrace'(1).  If the on/off functions are
never used and tracing is enabled with the '-ton' option of
'mpirun'(1), a single trace segment is produced.
 
The on/off functions have no effect if tracing is not enabled by
'mpirun'(1) with either the '-ton' or '-toff' switches.  Thus, an
application can be littered with these functions but run without trace
collection and very little additional overhead due to the no-operation
function calls.
 
This is a LAM/MPI-specific function and is intended mainly for
debugging.  If this function is used, it should be used in conjunction
with the 'LAM_MPI' C preprocessor macro

.vb
  #if LAM_MPI
     MPIL_Trace_on();
  #endif
.ve

Limitations:

After the volume of generated traces exceeds a preset limit, the
oldest traces are discarded in favour of new traces.  Avoiding
discarded traces is further incentive to use 'MPIL_Trace_on' and
'MPIL_Trace_off'.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_OTHER

.seealso: MPI_Trace_off, mpirun
@*/
int MPIL_Trace_on(void)
{
	int		err;			/* error code */
	int		trmodesave;		/* save trace mode */
/*
 * This is a NOP if trace mode is off.
 */
	if ( ! (_kio.ki_rtf & RTF_TRACE)) return(MPI_SUCCESS);

	lam_initerr();
	lam_setfunc(BLKMPILTRACEON);
/*
 * Synchronize. We don't want to trace it though!
 */
	trmodesave = _kio.ki_rtf & RTF_TRSWITCH;
	_kio.ki_rtf &= ~RTF_TRSWITCH;
	
	err = MPI_Barrier(MPI_COMM_WORLD);

	_kio.ki_rtf |= trmodesave;
	
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPILTRACEON, err));
	}
/*
 * Turn tracing on.
 */
	if (lam_tr_on() < 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPILTRACEON,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	}
	
	lam_resetfunc(BLKMPILTRACEON);
	return(MPI_SUCCESS);
}

/*
 *	MPIL_Trace_off
 *
 *	Function:	- disables tracing; ends segment
 */
/*@

MPIL_Trace_off - LAM/MPI-specific function to disable run-time tracing

Notes:

These functions give the application the flexibility to generate
traces only during certain interesting phases of the application''s
execution.  This technique can considerably reduce the size of the
trace files and burden of displaying them.
 
Both functions are collective over the 'MPI_COMM_WORLD' communicator.
In typical usage, the '-toff' option of 'mpirun'(1) would be used to
enable tracing, but start with the runtime switch in the off position.
At the beginning of an interesting phase of program execution,
'MPIL_Trace_on' would be called.  'MPIL_Trace_off' would be called
after the interesting phase.  Tracing can be turned on and off many
times.  Each period of tracing eventually forms a trace segment in the
trace file extracted by 'lamtrace'(1).  If the on/off functions are
never used and tracing is enabled with the '-ton' option of
'mpirun'(1), a single trace segment is produced.
 
The on/off functions have no effect if tracing is not enabled by
'mpirun'(1) with either the '-ton' or '-toff' switches.  Thus, an
application can be littered with these functions but run without trace
collection and very little additional overhead due to the no-operation
function calls.
 
This is a LAM/MPI-specific function and is intended mainly for
debugging.  If this function is used, it should be used in conjunction
with the 'LAM_MPI' C preprocessor macro

.vb
  #if LAM_MPI
     MPIL_Trace_off();
  #endif
.ve

Limitations:

After the volume of generated traces exceeds a preset limit, the
oldest traces are discarded in favour of new traces.  Avoiding
discarded traces is further incentive to use 'MPIL_Trace_on' and
'MPIL_Trace_off'.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_OTHER

.seealso: MPI_Trace_on, mpirun
@*/
int MPIL_Trace_off(void)
{
	int		err;			/* error code */
	int		trmodesave;		/* save trace mode */
/*
 * This is a NOP if trace mode is off.
 */
	if ( ! (_kio.ki_rtf & RTF_TRACE)) return(MPI_SUCCESS);
	
	lam_initerr();
	lam_setfunc(BLKMPILTRACEOFF);
/*
 * Synchronize. We don't want to trace it though!
 */
	trmodesave = _kio.ki_rtf & RTF_TRSWITCH;
	_kio.ki_rtf &= ~RTF_TRSWITCH;

	err = MPI_Barrier(MPI_COMM_WORLD);

	_kio.ki_rtf |= trmodesave;

	if (err != MPI_SUCCESS) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPILTRACEOFF, err));
	}
/*
 * Turn tracing off.
 */
	if (lam_tr_off() < 0) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPILTRACEOFF,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	}
	
	lam_resetfunc(BLKMPILTRACEOFF);
	return(MPI_SUCCESS);
}
