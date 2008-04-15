/*
 * Copyright (c) 2001-2004 The Trustees of Indiana University.  
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
 * $Id: etc_misc.h,v 1.28 2003/07/05 21:53:40 jsquyres Exp $
 *
 *	Function:	- some random C declarations for internal LAM functions
 */

#ifndef _LAM_ETC_MISC_H_
#define _LAM_ETC_MISC_H_

#include <lam_config.h>

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#include <portable.h>
#include <net.h>        /* for struct nmsg */

#ifdef __cplusplus
extern "C" {
#endif


/*
 * share/etc/atos.c
 */
short int atos(char *s);


/*
 * share/etc/combine.c
 */
void cbn_add(int4 flags, int4 length, char *msg1, char *msg2);
void cbn_and(int4 flags, int4 length, char *msg1, char *msg2);
void cbn_err(int4 flags, int4 length, char *msg1, char *msg2);
void cbn_max(int4 flags, int4 length, char *msg1, char *msg2);
void cbn_min(int4 flags, int4 length, char *msg1, char *msg2);
void cbn_mul(int4 flags, int4 length, char *msg1, char *msg2);
void cbn_or(int4 flags, int4 length, char *msg1, char *msg2);
void cbn_xor(int4 flags, int4 length, char *msg1, char *msg2);
int4 cbn_cat(char *buf1, int4 len1, int4 nfull, char *buf2, int4 len2);


/*
 * share/etc/f2cstring.c
 */
char *f2cstring(char *str, int len);


/*
 * share/etc/f2cstring.c
 */
int lam_pty_open(int *fdm, int *fds);


/*
 * share/etc/ldtimer.c
 */
int4 ldtimer(void);


/*
 * share/etc/memucmp.c
 */
int memucmp(unsigned char *m1, unsigned char *m2, int4 nbytes);


/*
 * share/etc/show_version.c
 */
void lam_show_version(int verbose);


/*
 * share/etc/few.c
 */
int _lam_few(char *argv[]);

/*
 * share/etc/fork.c
 */
typedef pid_t (*lam_fork_func_t)(void);
extern lam_fork_func_t lam_fork;

/*
 * share/etc/lam_getpid.c
 */
void lam_register_pid(pid_t);
pid_t lam_getpid(void);
void lam_reset_pid_cache(void);

/*
 * share/etc/lamunix.c
 */
void _lam_atexit(void (*)());


/*
 * share/etc/bootsockio.c
 */
int readsockint4(int sock, int4 *pdata);
int writesockint4(int sock, int4 data);
int readcltcoord(int sock, int *pid, int4 *pstrport, int4 *pudpport);
int writecltcoord(int sock, int4 id, int4 strport, int4 udpport);
int readcltnbr(int sock, int4 *plink, unsigned char *phostaddr,
	       int4 *pport, int4 *node_type, int4 *pnum_cpus);
int writecltnbr(int sock, int4 link, int is_hostname,
		unsigned char *hostaddr_or_name, int4 port, 
		int4 node_type, int4 num_cpus);

/*
 * share/etc/nodespin.c
 */
void nodespin_init(char *msg);
void nodespin_next(int4 node);
void nodespin_end(void);

/*
 * share/etc/prime.c
 */
int is_prime(int4 n);
int4 next_prime(int4 n);

/*
 * share/etc/nmsgconv.c
 */
void _ltot_sysnmsg(struct nmsg *);
void _ttol_sysnmsg(struct nmsg *);
void _ltot_usrnmsg(struct nmsg *);
void _ttol_usrnmsg(struct nmsg *);

/*
 * share/etc/show_help.c
 */
void show_help(const char *program, const char *topic, ...);
void show_help_file(const char *filename, 
		    const char *program, const char *topic, ...);

/*
 * share/etc/snprintf.c
 */
#if !LAM_HAVE_SNPRINTF
#define snprintf lam_snprintf
#elif !LAM_HAVE_PROTO_snprintf
int snprintf(char *str, int len, char *fmt, ...);
#endif
int lam_snprintf(char *str, int len, char *fmt, ...);

/*
 * share/etc/vsprintf.c
 */
#if __STDC__
#include <stdarg.h>
#else
#ifdef __cplusplus
#include <stdarg.h>
#else
#include <varargs.h>
#endif
#endif
char *lam_vsnprintf __ARGS((char *format, va_list arglist));

/*
 * share/etc/mempool.c
 */

typedef void *(lam_mp_malloc_fn_t)(size_t size);
typedef void (lam_mp_free_fn_t)(void *base);

#define LAM_DEFAULT_MP_POOL_SIZE 32
#define LAM_DEFAULT_MP_INCR_SIZE 32

int lam_mp_init(int size, lam_mp_malloc_fn_t *mfn, lam_mp_free_fn_t *ffn, 
		int initial_pool_size, int increment_size, int want_disinfect);
void *lam_mp_malloc(int num);
void lam_mp_free(int num, void *instance);
int lam_mp_finalize(int num);

/*
 * share/etc/strncpy.c
 */

char *lam_strncpy(char *dest, const char *src, int len);

/*
 * share/etc/ncpu.c
 */
long lam_get_ncpus();

#ifdef __cplusplus
}
#endif

#endif
