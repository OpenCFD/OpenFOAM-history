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
 * $Id: init.c,v 6.80 2003/05/07 20:13:28 brbarret Exp $
 *
 *	Function:	- initialize the MPI session
 *			- the kitchen-sink of MPI
 *	Accepts:	- ptr to argc
 *			- ptr to argv
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <lam_config.h>

#include <mpi.h>
#include <blktype.h>
#include <mpisys.h>
#include <etc_misc.h>


/*
 * Create the whole MPI universe.  
 */
/*@
   MPI_Init - Initialize the MPI execution environment

Input Parameters:
+ pargc - Pointer to the number of arguments 
- pargv - Pointer to the argument vector

Notes:

MPI specifies no command-line arguments but does allow an MPI
implementation to make use of them.  LAM/MPI neither uses nor adds any
values to the 'argc' and 'argv' parameters.  As such, it is legal to
pass 'NULL' for both 'argc' and 'argv' in LAM/MPI.

Instead, LAM/MPI relies upon the 'mpirun' command to pass
meta-information between nodes in order to start MPI programs (of
course, the LAM daemons must have previously been launched with the
'lamboot' command).  As such, every rank in 'MPI_COMM_WORLD' will
receive the 'argc' and 'argv' that was specified with the 'mpirun'
command (either via the 'mpirun' command line or an app schema) as
soon as 'main' begins.  See the 'mpirun'(1) man page for more
information.

If 'mpirun' is `not` used to start MPI programs, the resulting process
will be rank 0 in 'MPI_COMM_WORLD', and 'MPI_COMM_WORLD' will have a
size of 1.  This is known as a "singleton" MPI.  It should be noted
that LAM daemons are still used for singleton MPI programs - 'lamboot'
must still have been successfully executed before running a singleton
process.

LAM/MPI takes care to ensure that the normal Unix process model of
execution is preserved: no extra threads or processes are forked from
the user''s process.  Instead, the LAM daemons are used for all
process management and meta-environment information.  Consequently,
LAM/MPI places no restriction on what may be invoked before
'MPI_INIT*' or after 'MPI_FINALIZE'; this is `not` a safe assumption
for those attempting to write portable MPI programs - see "Portability
Concerns", below.

MPI mandates that the same thread must call 'MPI_INIT' (or
'MPI_INIT_THREAD') and 'MPI_FINALIZE'.

Note that the Fortran binding for this routine has only the error
return argument ('MPI_INIT(ierror)').

Because the Fortran and C versions of 'MPI_INIT' are different, there
is a restriction on who can call 'MPI_INIT'.  The version (Fortran or
C) must match the main program.  That is, if the main program is in C,
then the C version of 'MPI_INIT' must be called.  If the main program
is in Fortran, the Fortran version must be called.

LAM/MPI uses the value of argv[0] to identify a process in many of the
user-level helper applications (mpitask and mpimsg, for example).
Fortran programs are generally identified as
"LAM_MPI_Fortran_program".  However, this name can be overridden for
Fortran programs by setting the environment variable
"LAM_MPI_PROCESS_NAME".

On exit from this routine, all processes will have a copy of the
argument list.  This is `not required` by the MPI standard, and truely
portable codes should not rely on it.  This is provided as a service
by this implementation (an MPI implementation is allowed to distribute
the command line arguments but is not required to).

Threading:

Applications using 'MPI_INIT' are effectively invoking
'MPI_INIT_THREAD' with a requested thread support of
'MPI_THREAD_SINGLE'.  However, this may be overridden with the
LAM_MPI_THREAD_LEVEL environment variable.  If set, this variable
replaces the default 'MPI_THREAD_SINGLE' value.  The following values
are allowed

0: Corresponds to 'MPI_THREAD_SINGLE'

1: Corresponds to 'MPI_THREAD_FUNNELED'

2: Corresponds to 'MPI_THREAD_SERIALIZED'

3: Corresponds to 'MPI_THREAD_MULTIPLE'

See MPI_Init_thread(3) for more information on thread level support in
LAM/MPI.

Predefined attributes:

LAM/MPI defines all required predefined attributes on
'MPI_COMM_WORLD'.  Some values are LAM-specific, and require
explanation.

'MPI_UNIVERSE_SIZE'

This is an MPI-required attribute.  It is set to an integer whose
value indicates how many CPUs LAM was booted with.  See bhost(5) and
lamboot(1) for more details on how to specify multiple CPUs per node.
Note that this may be larger than the number of CPUs in
'MPI_COMM_WORLD'.

'LAM_UNIVERSE_NCPUS'

This is a LAM-specific attribute -- it will not be defined in other
MPI implementations.  It is actually just a synonym for
'MPI_UNIVERSE_SIZE' -- it contains the number of CPUs in the current
LAM universe.  Note that this may be larger than the number of CPUs in
'MPI_COMM_WORLD'.

'LAM_UNIVERSE_NNODES'

This is a LAM-specific attribute -- it will not be defined in other
MPI implementations.  It indicates the total number of nodes in the
current LAM universe (which may be different from the total number of
CPUs).  Node that this may be larger than the number of nodes in
'MPI_COMM_WORLD'.

Signals Used:

The LAM implementation of MPI uses, by default, 'SIGUSR2'.  This may
be changed when LAM is compiled, however, with the '--with-signal'
command line switch to LAM''s 'configure' script.  Consult your system
administrator to see if they specified a different signal when LAM was
installed.

LAM/MPI does not catch any other signals in user code, by default.  If
a process terminates due to a signal, the mpirun will be notified of
this and will print out an appropriate error message and kill the rest
of the user MPI application.

This behavior can be overridden (mainly for historical reasons) with
the "-sigs" flag to 'mpirun'.  When "-sigs" is used, LAM/MPI will
effectively transfer the signal-handling code from mpirun to the user
program.  Signal handlers will be installed during 'MPI_INIT' (or
'MPI_INIT_THREAD') for the purpose of printing error messages before
invoking the next signal handler.  That is, LAM "chains" its signal
handler to be executed before the signal handler that was already set.

Therefore, it is safe for users to set their own signal handlers.  If
they wish the LAM signal handlers to be executed as well, users should
set their handlers before 'MPI_INIT*' is invoked. 

LAM/MPI catches the following signals

'SIGSEGV', 'SIGBUS', 'SIGFPE', 'SIGILL'

All other signals are unused by LAM/MPI, and will be passed to their
respective signal handlers.

Portability Concerns:

Portable MPI programs `cannot` assume the same process model that LAM
uses (i.e., essentially the same as POSIX).  MPI does not mandate
anything before 'MPI_INIT' (or 'MPI_INIT_THREAD'), nor anything after
'MPI_FINALIZE' executes.  Different MPI implementations make different
assumptions; some fork auxillary threads and/or processes to "help"
with the MPI run-time environment (this may interfere with the
constructors and destructors of global C++ objects, particularly in
the case where using atexit() or onexit(), for example).  As such, if
you are writing a portable MPI program, you cannot make the same
assumptions that LAM/MPI does.

In general, it is safest to call 'MPI_INIT' (or 'MPI_INIT_THREAD') as
soon as possible after 'main' begins, and call 'MPI_FINALIZE'
immediately before the program is supposed to end.  Consult the
documentation for each MPI implementation for their intialize and
finalize behavior.

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_INIT
.N MPI_ERR_OTHER

.seealso MPI_Init_thread, MPI_Finalize, lamboot, mpirun, lamhalt, lamssi

.N ACK
@*/
int MPI_Init(int *pargc, char ***pargv)
{
  int err;
  int provided;
  char *env;
  int requested = MPI_THREAD_SINGLE;

  lam_setfunc(BLKMPIINIT);

  /*
   * check for environment overrides for requested thread level.  If there
   * is, check to see that it is a valid/supported thread level.  If not,
   * default to MPI_THREAD_SINGLE.
   */

  if ((env = getenv("LAM_MPI_THREAD_LEVEL")) != NULL) {
    requested = atoi(env);
    if ((requested < MPI_THREAD_SINGLE) 
        || (requested > MPI_THREAD_SERIALIZED)) {
      show_help("mpi-init", "invalid-thread-level-env", NULL); 
      exit(LAM_EEXIT);
    }
  } 
    
  /* Call the back-end init function (shared between MPI_INIT and
     MPI_INIT_THREAD) */

  err = lam_mpi_init(pargc, pargv, requested, &provided, BLKMPIINIT);

  /* All done */

  lam_resetfunc(BLKMPIINIT);
  return err;
}
