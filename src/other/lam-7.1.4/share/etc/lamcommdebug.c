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
 *	$Id: lamcommdebug.c,v 6.8 2002/12/12 15:20:58 jsquyres Exp $
 * 
 *	Function:	- LAMD communication logging facility
 */

#include <lam_config.h>
#include <lamdebug.h>

#include <stdlib.h>

/*
 * Local state variable
 */
static int dlo_debug_id = -1;
static int dli_debug_id = -1;
static int local_debug_id = -1;
static int min_level = 0;


/*
 * lam_comm_debug_open
 *
 * Setup communication debugging with minimum message log level of level
 */
void
lam_comm_debug_open(int level, int where)
{
  lam_debug_stream_info_t lds = {
    1, 0, 0, NULL, NULL, 
    0, 0,
    1, 1, "bogus"
  };

  if ((where & LAM_COMM_DEBUG_DLO) && (dlo_debug_id == -1)) {
    lds.lds_file_suffix = "debug-dlo-log.txt";
    dlo_debug_id = lam_debug_open(&lds);
  }
  if ((where & LAM_COMM_DEBUG_DLI) && (dli_debug_id == -1)) {
    lds.lds_file_suffix = "debug-dli-log.txt";
    dli_debug_id = lam_debug_open(&lds);
  }
  if ((where & LAM_COMM_DEBUG_LOCAL) && (local_debug_id == -1)) {
    lds.lds_file_suffix = "debug-inter-log.txt";
    local_debug_id = lam_debug_open(&lds);
  }
}


/*
 * lam_comm_debug_local
 *
 * If we have opened (ie, we want to log), log message if level is 
 * high enough.  Logged to "local communication" file
 */
void
#if LAM_WANT_PROTOS
lam_comm_debug_local(int level, char *format, ...)
#else
lam_comm_debug_local(level, format, va_alist)
int level;
char *format;
va_dcl
#endif
{
  va_list arglist;

  if (local_debug_id != -1 && level >= min_level) {

    /* Invoke the underlying generalized debugging mechanism.  Convert
       the .../va_alist to a va_list and call
       lam_debug_output_low(). */

#if __STDC__
    va_start(arglist, format);
#else
    va_start(arglist);
#endif
    lam_debug_output_low(local_debug_id, format, arglist);
    va_end(arglist);
  }
}


/*
 * lam_comm_debug_dlo
 *
 * If we have opened (ie, we want to log), log message if level is 
 * high enough.  Logged to "dlo communication" file
 */
void
#if LAM_WANT_PROTOS
lam_comm_debug_dlo(int level, char *format, ...)
#else
lam_comm_debug_dlo(level, format, va_alist)
int level;
char *format;
va_dcl
#endif
{
  va_list arglist;

  if (dlo_debug_id != -1 && level >= min_level) {

    /* Invoke the underlying generalized debugging mechanism.  Convert
       the .../va_alist to a va_list and call
       lam_debug_output_low(). */

#if __STDC__
    va_start(arglist, format);
#else
    va_start(arglist);
#endif
    lam_debug_output_low(dlo_debug_id, format, arglist);
    va_end(arglist);
  }
}


/*
 * lam_comm_debug_dli
 *
 * If we have opened (ie, we want to log), log message if level is 
 * high enough.  Logged to "dli communication" file
 */
void
#if LAM_WANT_PROTOS
lam_comm_debug_dli(int level, char *format, ...)
#else
lam_comm_debug_dli(level, format, va_alist)
int level;
char *format;
va_dcl
#endif
{
  va_list arglist;

  if (dli_debug_id != -1 && level >= min_level) {

    /* Invoke the underlying generalized debugging mechanism.  Convert
       the .../va_alist to a va_list and call
       lam_debug_output_low(). */

#if __STDC__
    va_start(arglist, format);
#else
    va_start(arglist);
#endif
    lam_debug_output_low(dli_debug_id, format, arglist);
    va_end(arglist);
  }
}

/*
 * lam_comm_debug_close
 *
 * Closes the files, if any open
 */
void
lam_comm_debug_close()
{
  if (local_debug_id != -1) {
    lam_debug_close(local_debug_id);
    local_debug_id = -1;
  }
  if (dlo_debug_id != -1) {
    lam_debug_close(dlo_debug_id);
    dlo_debug_id = -1;
  }
  if (dli_debug_id != -1) {
    lam_debug_close(dli_debug_id);
    dli_debug_id = -1;
  }
}
