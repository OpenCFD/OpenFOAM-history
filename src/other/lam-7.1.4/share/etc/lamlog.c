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
 *	$Id: lamlog.c,v 6.14 2003/04/18 11:00:37 jsquyres Exp $
 * 
 *	Function:	- thin wrappers around syslog functionality
 */

#include <lam_config.h>

#include <stdio.h>
#if __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <lamdebug.h>
#include <laminternal.h>
#include <etc_misc.h>


/*
 * Local state variable
 */
static int debug_id = -1;


/*
 * lamopenlog
 *
 * Open the syslog and set fl_syslog to 1 so that future calls to
 * lamlog will go to the syslog.
 */
void
lamopenlog(char *ident)
{
  lam_debug_stream_info_t lds = {
    0, 1, LOG_INFO, NULL, NULL, 0, 0,
    1, 1, "debug-log.txt"
  };
  lds.lds_syslog_ident = ident;

  /* Open a debug stream, 'cause some OS's don't let normal users see
     the syslog.  This is now all implemented via the generalized
     debugging mechanism.  */

  if (debug_id == -1)
    debug_id = lam_debug_open(&lds);
}


/*
 * lamlog
 *
 * Send something to the syslog if fl_syslog is 1
 */
void
#if LAM_WANT_PROTOS
lamlog(char *format, ...)
#else
lamlog(format, va_alist)
char *format;
va_dcl
#endif
{
  va_list arglist;

  if (debug_id != -1) {

    /* Invoke the underlying generalized debugging mechanism.  Convert
       the .../va_alist to a va_list and call
       lam_debug_output_low(). */

#if __STDC__
    va_start(arglist, format);
#else
    va_start(arglist);
#endif
    lam_debug_output_low(debug_id, format, arglist);
    va_end(arglist);
  }
}


/*
 * lamcloselog
 *
 * Closes the syslog, if it was open
 */
void
lamcloselog()
{
  if (debug_id != -1) {
    lam_debug_close(debug_id);
    debug_id = -1;
  }
}
