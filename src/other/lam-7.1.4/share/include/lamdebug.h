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
 *	Function:	- general debugging support
 */

#ifndef LAMDEBUG_H_
#define LAMDEBUG_H_

#include <lam_config.h>

#if __STDC__
#include <stdarg.h>
#else
#ifdef __cplusplus
#include <stdarg.h>
#else
#include <varargs.h>
#endif
#endif /* #if __STDC__ */


/*
 * Struct for holding / passing around info about opening debug streams
 */
typedef struct lam_debug_stream_info {
  int lds_fl_debug;
  int lds_fl_syslog;
  int lds_syslog_priority;
  char *lds_syslog_ident;
  char *lds_prefix;
  int lds_fl_stdout;
  int lds_fl_stderr;
  int lds_fl_file;
  int lds_fl_file_append;
  char *lds_file_suffix;
} lam_debug_stream_info_t;


#ifdef __cplusplus
extern "C" {
#endif

  /*
   * support for run-time debug mode
   *
   * DO NOT USE IN LAM LIBRARY (anything under share)
   * not all applications in LAM have the fl_* flags declared
   * or in use.
   */
#define DBUG		if (fl_debug) printf
#define VERBOSE		if (fl_verbose) printf
  
  /*
   * Simple syslog debugging support
   *
   * Also outputs to /tmp/<blah>/lam-debug-log.txt
   *
   * See share/etc/lamlog.c
   */
  void lamopenlog(char *ident);
  void lamlog(char *format, ...);
  void lamcloselog(void);

  /*
   * More generalized debugging output mechanism
   *
   * See share/etc/lamdebug.c
   */
  int lam_debug_open(lam_debug_stream_info_t *lds);
  int lam_debug_switch(int lam_debug_id, int fl_enable);
  void lam_debug(int lam_debug_id, char *format, ...);
  void lam_debug_output_low(int lam_debug_id, char *format, va_list arglist);
  void lam_debug_reopen_all(void);
  void lam_debug_close(int lam_debug_id);

  /*
   * LAM daemon-based communication debugging
   *
   * See share/etc/lamcommdebug.c
   */
#define LAM_COMM_DEBUG_ALL   0x0FFFFFFF
#define LAM_COMM_DEBUG_DLI   0x00000001
#define LAM_COMM_DEBUG_DLO   0x00000002
#define LAM_COMM_DEBUG_LOCAL 0x00000004

  void lam_comm_debug_open(int level, int where);
  void lam_comm_debug_local(int level, char *format, ...);
  void lam_comm_debug_dlo(int level, char *format, ...);
  void lam_comm_debug_dli(int level, char *format, ...);
  void lam_comm_debug_close(void);

  /*
   * Wrapper macros for speed-sensitive debugging
   *
   * Only enabled if LAM is compiled with --with-debug
   */
#if LAM_WANT_DEBUG
#define lam_debug_cond(a) lam_debug a ;
#define lamlog_cond(a) lamlog a ;
#define lam_comm_debug_local_cond(a) lam_comm_debug_local a ;
#define lam_comm_debug_dlo_cond(a) lam_comm_debug_dlo a ;
#define lam_comm_debug_dli_cond(a) lam_comm_debug_dli a ;

#else /* #if LAM_WANT_DEBUG */

#define lam_debug_cond(a)
#define lamlog_cond(a)
#define lam_comm_debug_local_cond(a)
#define lam_comm_debug_dlo_cond(a)
#define lam_comm_debug_dli_cond(a)

#endif /* #if LAM_WANT_DEBUG */

#ifdef __cplusplus
}
#endif

#endif /* #ifdef LAMDEBUG_H_ */
