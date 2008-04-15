/* -*- c -*-
 *
 * LAM configuation header file.
 * See the top-level LICENSE file for license and copyright
 * information.
 *
 * $Id: lam_config_bottom.h,v 1.6 2003/04/09 17:37:12 brbarret Exp $
 *
 * Function: - Bottom of lam_config.h.  Rather than have an enormous
 * AH_BOTTOM in configure.in, put all the hardwired values that need
 * to be in <lam_config.h> here in this file, and simply #include it
 * at the bottom of <lam_config.h>.
 */

#ifndef _LAM_CONFIG_BOTTOM_H
#define _LAM_CONFIG_BOTTOM_H

#define LAM_HAVE_THREADS (LAM_HAVE_SOLARIS_THREADS || LAM_HAVE_POSIX_THREADS)

/*
 * Some compilers do not define __STDC__ correctly (e.g., Solaris
 * workshop, at least version 4.2).  As such, the LAM header files
 * will not include full ANSI prototyping, which is a real pain for
 * users.  We can override that here -- set to 1 if you want full
 * ANSI prototypes.
 */
#if __STDC__ || defined(c_plusplus) || defined(__cplusplus) || LAM_CONFIG_FORCE_PROTOS
#define LAM_WANT_PROTOS                  1
#else
#define LAM_WANT_PROTOS                  0
#endif

/*
 * Use to select if definitely want to zero out memory before using it.
 * This affects lower layers of message passing API, where purify, bcheck,
 * and other memory-checking debuggers will sometimes complain about
 * "read from uninitialized" and the like, when the code actually shows
 * that it is ok.  That is, LAM will function correctly with or without
 * this flag -- this flag only affects what purify/bcheck/etc. will report.
 * Note that there will be slightly more overhead in message passing if
 * this flag is enabled.
 */
#if LAM_DISINFECT
#include <string.h>
#define LAM_ZERO_ME(a)                   memset(&(a), 0, sizeof((a)));
#else
#define LAM_ZERO_ME(a)
#endif

/* 
 * This used to be part of mpi2c++/acinclude.h.  These things can
 * safely be hardwired -- they are not determined by configure
 * anymore.
 */
#define _MPIPP_USEEXCEPTIONS_ 1
#define _MPIPP_DEBUG_         0

#define MPI2CPP_HAVE_BOOL     1 

#ifndef MPI2CPP_SIZEOF_INT
#define MPI2CPP_SIZEOF_INT 4
#endif

#ifndef MPI2CPP_SIZEOF_BOOL_T
#define MPI2CPP_SIZEOF_BOOL_T 1
#endif

#if MPI2CPP_SIZEOF_INT != MPI2CPP_SIZEOF_BOOL_T
#define _MPIPP_BOOL_NE_INT_   1
#else
#define _MPIPP_BOOL_NE_INT_   0
#endif

/*
 * LAM has (the proper) MPI_ERR_PENDING, so set this to 0
 */
#define MPI2CPP_HAVE_PENDING  0

/*
 * LAS has MPI_GET_VERSION, so set this to 1
 */
#define MPI2CPP_HAVE_MPI_GET_VERSION  1

/*
 * LAM has both of these, so set them to 1
 */
#define MPI2CPP_HAVE_STATUS_IGNORE    1
#define MPI2CPP_HAVE_STATUSES_IGNORE  1

/*
 * LAM and the fortran datatypes
 */
#define MPI2CPP_FORTRAN 1
#define MPI2CPP_ALL_OPTIONAL_FORTRAN 1
#define MPI2CPP_SOME_OPTIONAL_FORTRAN 0
#define MPI2CPP_OPTIONAL_C 1

#if LAM_WANT_PROFILE
#define _REAL_MPI_ PMPI
#else
#define _REAL_MPI_ MPI
#endif

/*
 * Assuming all compilers have "namespace" these days
 */
#define _MPIPP_USENAMESPACE_  1
#define _MPIPP_STATIC_
#define _MPIPP_EXTERN_ extern

/*
 * Assuming all compilers have "bool" these days
 */
#if defined(c_plusplus) || defined(__cplusplus)
typedef bool MPI2CPP_BOOL_T;
#define MPI2CPP_FALSE false
#define MPI2CPP_TRUE true
#endif

/*
 * Architecture/OS's that will need flags in the test suite to ignore
 * certain tests.  They're largely ignored now, but they have to be
 * set or g++ -Wundef will complain loudly.
 */
#define MPI2CPP_LAM63 0
#define MPI2CPP_LAM631 0
#define MPI2CPP_LAM632 0
#define MPI2CPP_LAM633 0
#define MPI2CPP_LAM64 0
#define MPI2CPP_LAMUNKNOWN 0
#define MPI2CPP_LAM 1

#define MPI2CPP_MPICH120 0
#define MPI2CPP_MPICH121 0
#define MPI2CPP_MPICHUNKNOWN 0
#define MPI2CPP_MPICH 0
 
#define MPI2CPP_IBM21014 0
#define MPI2CPP_IBM21015 0
#define MPI2CPP_IBM21016 0
#define MPI2CPP_IBM21017 0
#define MPI2CPP_IBM21018 0
#define MPI2CPP_IBM2_3_0_0 0
#define MPI2CPP_IBM2_4_0_0 0
#define MPI2CPP_IBMUNKNOWN 0
#define MPI2CPP_IBM_SP 0
 
#define MPI2CPP_SGI20 0
#define MPI2CPP_SGI30 0
#define MPI2CPP_SGI31 0
#define MPI2CPP_SGI32 0
#define MPI2CPP_SGIUNKNOWN 0

#define MPI2CPP_HPUX0102 0
#define MPI2CPP_HPUX0103 0
#define MPI2CPP_HPUX0105 0
#define MPI2CPP_HPUXUNKNOWN 0
#define MPI2CPP_HPUX_OS 0

#define MPI2CPP_CRAY 0
#define MPI2CPP_CRAY1104 0
#define MPI2CPP_CRAYUNKNOWN 0

#define MPI2CPP_G_PLUS_PLUS 0

/*
 * Can virtual functions return derived class instead
 * of base class?
 *
 * We seem to have lost this test somewhere along the line...
 */
#define MPI2CPP_VIRTUAL_FUNC_RET 1


/*
 * Some combinations for simplicity
 */
#define LAM_HAVE_PTYS (LAM_HAVE_BSD_PTYS || LAM_HAVE_SYSV_PTYS)

/*
 * Grumble.  AC_C_BIGENDIAN #defines WORDS_BIGENDIAN to 1 if we're on
 * a big endian machine.  But it leaves it undefined if we're on a
 * little endian machine.  This causes "gcc -Wundef" to complain when
 * we use "#if WORDS_BIGENDIAN".  So #define it here to be 0 if it
 * isn't already defined.  
 */
#ifndef WORDS_BIGENDIAN
#define LAM_WORDS_BIGENDIAN              0
#else
#define LAM_WORDS_BIGENDIAN              1
#endif

/*
 * In some places, we need to swap the endian (particularly in IMPI)
 */
#if LAM_WANT_IMPI
#if LAM_WORDS_BIGENDIAN
#define LAM_SWAP_ENDIAN(x, y)
#else
#define LAM_SWAP_ENDIAN(x, y) lam_swap_endian(x, y)
#endif
#endif

/*
 * Some compilers complain when sizeof(long double) == sizeof(double)
 * Example: gcc/linux 2.95.2 on alpha (64 bit)
 */
#if LAM_SIZEOF_DOUBLE == LAM_SIZEOF_LONG_DOUBLE
typedef double LAM_LONG_DOUBLE;
#else
typedef long double LAM_LONG_DOUBLE;
#endif


/*
 * If we don't have the gethostname proto, make it
 */
#if !LAM_HAVE_PROTO_gethostname
#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif
int gethostname(char *name, int namelen);
#if defined(c_plusplus) || defined(__cplusplus)
}
#endif
#endif

#if !LAM_HAVE_UNION_SEMUN
union semun {
        int             val;            /* value for SETVAL */
        struct semid_ds *buf;           /* buffer for IPC_STAT & IPC_SET */
        unsigned short  *array;         /* array for GETALL & SETALL */
};
#endif

/*
 * Macro to use when passing the val argument of semun into semctl().
 *
 * Should be used as:
 *   union semun semctl_arg;
 *   semctl_arg.val = 1;
 *   semctl(semid, 0, SETVAL, LAM_SEMUN_VAL(semctl_arg);
 *
 * Needed because Darwin 8 misinterprets the POSIX standard about how
 * union semun should be used.  The standard in question is here:
 *
 *  http://www.opengroup.org/onlinepubs/009695399/functions/semctl.html
 *
 * Now, everyone else in the world seems to think that the text would
 * require you to call semctl (as used above) as:
 *   semctl(semid, 0, SETVAL, semctl_arg);
 *
 * ie, you always pass a union semun as definied in the standard to
 * semctl, regardless of which field you are actually going to use.
 * However, Apple seems to misunderstand the text about variadic
 * functions and takes the fourth argument to be of type whatever is
 * appropriate for the command called.  So for SETVAL, they assume
 * that rather than passing in an union semun, you are passing in an
 * integer.  This all works fine without issue in 32bit land because
 * sizeof(int) == sizeof(void*).  However, it doesn't work so well in
 * 64bit land because sizeof(int) is less than sizeof(void*), so
 * Darwin is looking for val in the wrong place.  They assume you
 * called semctl like this:
 *    semctl(semid, 0, SETVAL, semctl_arg.val);
 *
 * The macro below is used to figure out which to use and go with that
 * option.  It should ONLY be used when the val member of the union
 * semun is needed.  For other members, do not use this macro.
 *
 * For the record, on 64 bit big endian machines, we're tested Linux,
 * AIX, Solaris, and Irix and they all interpret the standard the
 * non-Darwin way.
 */
#if LAM_UNION_SEMUN_VAL_DIRECT
#define LAM_SEMUN_VAL(a) a.val
#else
#define LAM_SEMUN_VAL(a) a
#endif

/* 
 * Define as 1 if broken.
 * Can't find an OS that's broken these days, but let's keep it here
 * just to be safe.
 */
#define LAM_BROKEN_SET_INET_SO_BUFSIZES 0
#define LAM_BROKEN_SET_UNIX_SO_BUFSIZES 0

/*
 * GER define
 *
 * per-process-pair minimum envelope slots
 */
#define MPI_GER                         8

/*
 * See if we can use "const" or not
 */
#define CONST                           const

/* 
 * Define as 1 if floating point is IEEE.
 */
#define HAVE_IEEE_FP                    1

/*
 * Provide a cast to go from an int to a (void*), or empty if
 * sizeof(void*) == sizeof(int).  This is necessary on arches where we
 * need to first promote an int up to an integer type of the same size
 * as (void*), and then cast that to (void*), eg., Alpha Linux.
 *
 * Weird logic here so that we avoid the cast whenever possible -- so
 * we end up duplicating the "no cast" case.  For example, if we only
 * had to no cast case at the end, if we are on a architecture where
 * sizeof(int) == sizeof(long) == sizeof(void*) (e.g., Solaris 2.6),
 * we'll always end up casting through long.  Icky.  
 */
#if LAM_SIZEOF_INT == LAM_SIZEOF_VOID_P
#define LAM_INT_TO_VOID_P
#elif LAM_SIZEOF_VOID_P == LAM_SIZEOF_LONG
#define LAM_INT_TO_VOID_P (long)
#elif LAM_SIZEOF_VOID_P == LAM_SIZEOF_LONG_LONG
#define LAM_INT_TO_VOID_P (long long)
#else
#define LAM_INT_TO_VOID_P
#endif 


/*
 * prototypes
 */
#ifndef __ARGS
#if LAM_WANT_PROTOS
#define __ARGS(a)	a
#else
#define __ARGS(a)	()
#endif
#endif

#endif	/* _LAM_CONFIG_BOTTOM_H */
