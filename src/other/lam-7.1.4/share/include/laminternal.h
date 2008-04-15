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
 *  $Id: laminternal.h,v 1.16 2003/11/04 02:18:46 pkambadu Exp $
 *
 *  Function: - prototypes for internally used LAM functions
 */

#ifndef _LAM_INTERNAL_H
#define _LAM_INTERNAL_H

#include <sys/types.h>
#include <sys/uio.h>

#include <lam_config.h>
#include <kreq.h>
#include <portable.h>
#include <tstdio.h>
#include <typical.h>

#ifdef __cplusplus
extern "C" {
#endif

char  *getworkdir __ARGS((void));
int getinetaddr __ARGS((char *, unsigned char *));

/* share/etc/kill.c */
int     lam_tmpdir_init __ARGS((char*, char*));
int     lam_tmpdir_init_opt __ARGS((char*, char*, int));
char    *lam_get_tmpdir __ARGS((void));
char    *lam_get_tmpdir_prefix __ARGS((void));
char    *lam_get_tmpdir_suffix __ARGS((void));
char    *lam_get_killfile __ARGS((void));
char    *lam_get_sockname __ARGS((void));
char    *lam_get_iosockname __ARGS((void));
int     lam_mktmpid __ARGS((int, char *, unint));
int     lam_mktmpid_stdin __ARGS((int, char *, unint));
int     lam_mktmpid_stdout __ARGS((int, char *, unint));
int     lam_mktmpid_stderr __ARGS((int, char *, unint));
void    lam_tmpdir_remove __ARGS((void));

int      microsleep __ARGS((unsigned int usec));

void    lam_deregister_object(int deltype, char *delobj);
#if __STDC__
void  lam_register_objects(int, ...);
#else
void  lam_register_objects();
#endif
void  lam_cleanup_objects(void);

typedef void (*lam_handler_func_t)();
lam_handler_func_t _lam_signal __ARGS((int signum, lam_handler_func_t handler));

int4  stoi __ARGS((unsigned char *));

void  nodespin_init __ARGS((char *));
void  nodespin_next __ARGS((int4));
void  nodespin_end __ARGS((void));

void  _cio_cleanup __ARGS((void));
void  _cio_bail __ARGS((int));
int _cipc_ksrfront __ARGS((struct kreq *, struct kreply *));
int _cipc_ksrback __ARGS((struct kreq *, struct kreply *));

off_t   lam_iomap_lseek __ARGS((unsigned int, int, off_t, int));
int   lam_iomap_open __ARGS((unsigned int, char *, int, int));
int lam_iomap_close __ARGS((unsigned int, int));
int   lam_iomap_read __ARGS((unsigned int, int, char *, int));
int   lam_iomap_write __ARGS((unsigned int, int, const char *, int));

void  _getbuf __ARGS((register TFILE *));
int _fmt __ARGS((TFILE *, char *, const char *, ...));

int  mread __ARGS((int, char *, int4));
int  mreadv __ARGS((int, struct iovec *, int));
int  mwrite __ARGS((int, const char *, int));
int  mwritev __ARGS((int, struct iovec *, int));

#ifdef __cplusplus
}
#endif

#endif  /* _LAM_INTERNAL_H */
