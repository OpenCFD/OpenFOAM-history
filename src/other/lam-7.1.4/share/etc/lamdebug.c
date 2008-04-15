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
 *	$Id: lamdebug.c,v 6.22 2003/05/05 17:45:56 jsquyres Exp $
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
#include <fcntl.h>
#include <unistd.h>

#include <lamdebug.h>
#include <laminternal.h>
#include <etc_misc.h>
#include <lamthreads.h>



/*
 * Internal data structures and helpers for the generalized
 * debugging output mechanism 
 */
typedef struct lam_debug_info {
  int ldi_used;
  int ldi_enabled;

  int ldi_syslog;
  int ldi_syslog_priority;
  char *ldi_syslog_ident;

  char *ldi_prefix;
  int ldi_prefix_len;

  int ldi_stdout;
  int ldi_stderr;

  int ldi_fd;
  char *ldi_file_suffix;
} lam_debug_info_t;

#define LAM_DEBUG_MAX_STREAMS 32


/*
 * Local state
 */
static int info_initialized = 0;
static lam_debug_info_t info[LAM_DEBUG_MAX_STREAMS];
static char *temp_str = 0;
static int temp_str_len = 0;
#if LAM_HAVE_THREADS
static lam_mutex_t mutex;
#endif


/*
 * Local functions
 */
static void init(void);


/*
 * lam_debug_open
 *
 * Opens a LAM debug stream.
 * Accepts:      - flag if syslog is to be used
 *               - string ident for syslog
 *               - flag if stdout is to be used
 *               - flag if stderr is to be used
 *               - flag if file is to be used
 *               - flag if file is to be appended to
 *               - string filename
 *               - max size of logfile (<= 0 for no max)
 * Returns:      - int identifier for stream or LAMERROR if none available
 */
int 
lam_debug_open(lam_debug_stream_info_t *lds)
{
  int i;
  int flags;
  char *filename;

  /* If we were compiled without --with-debug, and this is not
     lamopenlog() calling, just return -- don't do anything */

  if (lds->lds_fl_debug != 0 && LAM_WANT_DEBUG == 0)
    return LAMERROR;

  /* Setup */

  if (!info_initialized)
    init();

  /* Find an available stream, or return LAMERROR */

  for (i = 0; i < LAM_DEBUG_MAX_STREAMS; ++i)
    if (info[i].ldi_used == 0)
      break;
  if (i >= LAM_DEBUG_MAX_STREAMS)
    return LAMERROR;

  /* Got a stream -- now initialize it and open relevant outputs */

  info[i].ldi_used = 1;
  info[i].ldi_enabled = lds->lds_fl_debug ? LAM_WANT_DEBUG : 1;

  info[i].ldi_syslog = lds->lds_fl_syslog;
  if (lds->lds_fl_syslog == 1) {
    if (lds->lds_syslog_ident != NULL) {
      info[i].ldi_syslog_ident = strdup(lds->lds_syslog_ident);
      openlog(lds->lds_syslog_ident, LOG_PID, LOG_USER);
    } else {
      info[i].ldi_syslog_ident = NULL;
      openlog("lam", LOG_PID, LOG_USER);
    }
    info[i].ldi_syslog_priority = lds->lds_syslog_priority;
  }

  if (lds->lds_prefix != NULL) {
    info[i].ldi_prefix = strdup(lds->lds_prefix);
    info[i].ldi_prefix_len = strlen(lds->lds_prefix);
  } else {
    info[i].ldi_prefix = NULL;
    info[i].ldi_prefix_len = 0;
  }

  info[i].ldi_stdout = lds->lds_fl_stdout;
  info[i].ldi_stderr = lds->lds_fl_stderr;

  info[i].ldi_fd = -1;
  if (lds->lds_fl_file == 1) {

    /* Setup the filename and open flags */

    filename = lam_get_tmpdir();
    strcat(filename, "/lam-");
    if (lds->lds_file_suffix != NULL) {
      info[i].ldi_file_suffix = strdup(lds->lds_file_suffix);
      strcat(filename, lds->lds_file_suffix);
    } else {
      info[i].ldi_file_suffix = NULL;
      strcat(filename, "debug.txt");
    }
    flags = O_CREAT | O_RDWR;
    if (!lds->lds_fl_file_append)
      flags |= O_TRUNC;

    /* Actually open the file */

    info[i].ldi_fd = open(filename, flags, 0644);
    if (info[i].ldi_fd == -1) {
      info[i].ldi_used = 0;
      return LAMERROR;
    }

    /* Make the file be close-on-exec to prevent child inheritance
       problems */

    fcntl(info[i].ldi_fd, F_SETFD, 1);
    free(filename);
  }

  return i;
}


/*
 * lam_debug_switch
 *
 * Set and/or query the state of a stream (enabled/disabled)
 *
 * Accepts:      - debug stream ID
 *               - flag to enable (1), disable (0), or query only (-1)
 * Returns:      - state of the stream before the action
 */
int 
lam_debug_switch(int lam_debug_id, int fl_enable)
{
  int ret = LAMERROR;

  /* Setup */

  if (!info_initialized)
    init();

  if (lam_debug_id >= 0 && lam_debug_id < LAM_DEBUG_MAX_STREAMS) {
    ret = info[lam_debug_id].ldi_enabled;
    if (fl_enable != -1)
      info[lam_debug_id].ldi_enabled = fl_enable;
  }

  return ret;
}


/*
 * lam_debug
 *
 * Thin wrapper around lam_debug_output_low() -- convert the
 * "..."/va_alist to a va_list, and then call lam_debug_output_low()
 *
 * Accepts:      - debug stream ID
 *               - format of string to output (a la printf)
 *               - varargs data for the format (a la printf) 
 */
void 
#if LAM_WANT_PROTOS
lam_debug(int lam_debug_id, char *format, ...)
#else
lam_debug(lam_debug_id, format, va_alist)
int lam_debug_id;
char *format;
va_dcl
#endif
{
  va_list arglist;
#if __STDC__
  va_start(arglist, format);
#else
  va_start(arglist);
#endif
  lam_debug_output_low(lam_debug_id, format, arglist);
  va_end(arglist);
}


/*
 * lam_debug_output_low
 *
 * Do the actual output.  Take a va_list so that we can be called from
 * multiple different places, even functions that took "..." as input
 * arguments.
 *
 * Accepts:      - debug stream ID
 *               - format of string to output (a la printf)
 *               - va_list data for the format
 */
void 
lam_debug_output_low(int lam_debug_id, char *format, va_list arglist)
{
  int len, total_len, want_newline = 0;
  char *str;
  lam_debug_info_t *ldi;

  /* Setup */

  if (!info_initialized)
    init();

  /* If it's valid, used, and enabled, output */

  if (lam_debug_id >= 0 && lam_debug_id < LAM_DEBUG_MAX_STREAMS &&
      info[lam_debug_id].ldi_used == 1 && 
      info[lam_debug_id].ldi_enabled == 1) {
    ldi = &info[lam_debug_id];
    
    /* Make the formatted string */

#if LAM_HAVE_THREADS
    lam_mutex_lock(&mutex);
#endif
    str = lam_vsnprintf(format, arglist);
    total_len = len = strlen(str);
    if (str[len - 1] != '\n') {
      want_newline = 1;
      ++total_len;
    }
    if (ldi->ldi_prefix != NULL)
      total_len += strlen(ldi->ldi_prefix);
    if (temp_str_len < total_len + want_newline) {
      if (temp_str != NULL)
	free(temp_str);
      temp_str = malloc(total_len * 2);
      temp_str_len = total_len * 2;
    }
    if (ldi->ldi_prefix != NULL) {
      if (want_newline)
	snprintf(temp_str, temp_str_len, "%s%s\n", ldi->ldi_prefix, str);
      else
	snprintf(temp_str, temp_str_len, "%s%s", ldi->ldi_prefix, str);
    } else {
      if (want_newline) 
	snprintf(temp_str, temp_str_len, "%s\n", str);
      else
	snprintf(temp_str, temp_str_len, "%s", str);
    }

    /* Syslog output */

    if (ldi->ldi_syslog == 1)
      syslog(ldi->ldi_syslog_priority, str);

    /* stdout output */

    if (ldi->ldi_stdout) {
      printf(temp_str);
      fflush(stdout);
    }

    /* stderr output */

    if (ldi->ldi_stderr) {
      fprintf(stderr, temp_str);
      fflush(stderr);
    }

    /* File output */

    if (ldi->ldi_fd != -1)
      write(ldi->ldi_fd, temp_str, total_len);
#if LAM_HAVE_THREADS
    lam_mutex_unlock(&mutex);
#endif

    free(str);
  }  
}


/*
 * lam_debug_reopen_all
 *
 * Re-open all debug streams (in append mode)
 */
void
lam_debug_reopen_all(void)
{
  int i;
  lam_debug_stream_info_t lds;

  for (i = 0; i < LAM_DEBUG_MAX_STREAMS; ++i) {

    /* scan till we find ldi_used == 0, which is the end-marker */ 
    if (info[i].ldi_used == 0)
      break;

    /* 
     * set this to zero to ensure that lam_debug_open will return this same
     * index as the debug stream id 
     */
    info[i].ldi_used = 0;

    lds.lds_fl_debug = (LAM_WANT_DEBUG == info[i].ldi_enabled) ? 1 : 0;
    lds.lds_fl_syslog = info[i].ldi_syslog;
    lds.lds_syslog_priority = info[i].ldi_syslog_priority;
    lds.lds_syslog_ident = info[i].ldi_syslog_ident;
    lds.lds_prefix = info[i].ldi_prefix;
    lds.lds_fl_stdout = info[i].ldi_stdout;
    lds.lds_fl_stderr = info[i].ldi_stderr;
    lds.lds_fl_file = (-1 == info[i].ldi_fd) ? 0 : 1;
    /* open all streams in append mode */
    lds.lds_fl_file_append = 1;
    lds.lds_file_suffix = info[i].ldi_file_suffix;

    /* 
     * call lam_debug_open to open the stream. The return value is
     * guaranteed to be i.  So we can ignore it. 
     */
    lam_debug_open(&lds);
  }
    
  return;
}


/*
 * lam_debug_close
 *
 * Close a debug stream
 *
 * Accepts:      - debug stream ID
 */
void 
lam_debug_close(int lam_debug_id)
{
  int i;
  lam_debug_info_t *ldi;

  /* Setup */

  if (!info_initialized)
    init();

  /* If it's valid, used, enabled, and has an open file descriptor,
     close it */

  if (lam_debug_id >= 0 && lam_debug_id < LAM_DEBUG_MAX_STREAMS &&
      info[lam_debug_id].ldi_used == 1 && 
      info[lam_debug_id].ldi_enabled == 1) {

    ldi = &info[lam_debug_id];

    if (ldi->ldi_fd != -1)
      close(ldi->ldi_fd);
    ldi->ldi_used = 0;

    /* If we strduped a prefix, suffix, or syslog ident, free it */

    if (ldi->ldi_prefix != NULL)
      free(ldi->ldi_prefix);
    ldi->ldi_prefix = NULL;

    if (ldi->ldi_file_suffix != NULL)
      free(ldi->ldi_file_suffix);
    ldi->ldi_file_suffix = NULL;

    if (ldi->ldi_syslog_ident != NULL)
      free(ldi->ldi_syslog_ident);
    ldi->ldi_syslog_ident = NULL;
  }  

  /* If no one has the syslog open, we should close it */

  for (i = 0; i < LAM_DEBUG_MAX_STREAMS; ++i)
    if (info[i].ldi_used == 1 && info[i].ldi_syslog == 1)
      break;
  if (i >= LAM_DEBUG_MAX_STREAMS)
    closelog();

  /* Somewhat of a hack to free up the temp_str */

  if (temp_str != 0) {
    free(temp_str);
    temp_str = 0;
    temp_str_len = 0;
  }
}


/*
 * init
 *
 * Local function that is called once to initialize the array of debug
 * streams
 */
static void
init(void)
{
  int i;
  for (i = 0; i < LAM_DEBUG_MAX_STREAMS; ++i) {
    info[i].ldi_used = 0;
    info[i].ldi_enabled = 0;

    info[i].ldi_syslog = 0;
    info[i].ldi_fd = -1;
  }

  /* Initialize the mutex that protects the output */

#if LAM_HAVE_THREADS
  lam_mutex_init(&mutex);
#endif

  info_initialized = 1;
}
