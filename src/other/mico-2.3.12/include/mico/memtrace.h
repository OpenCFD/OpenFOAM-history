/*
 * ----------------------------------------------------------------------
 *
 * Memory Tracer 
 *
 * Print a backtrace for each allocated chunk of memory.
 *
 * (w) 1999 Frank Pilhofer
 *
 * ----------------------------------------------------------------------
 */

#ifndef __MEMTRACE_H__
#define __MEMTRACE_H__

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MEMTRACE_REPORT_ON_EXIT        (1<<0)
#define MEMTRACE_FULL_FILENAMES        (1<<1)
#define MEMTRACE_NO_INVALIDATE_MEMORY  (1<<2)

/*
 * Admin interface
 */

void MemTrace_Init   (const char *, int);
void MemTrace_Report (FILE *);
void MemTrace_Flush  (void);

/*
 * Print a backtrace from here
 */

void MemTrace_SelfTrace (FILE *);

/*
 * malloc() and free() replacements
 */

void * MemTrace_Malloc   (size_t);
void   MemTrace_Free     (void *);

#define malloc(__size) MemTrace_Malloc (__size)
#define free(__pointer) MemTrace_Free (__pointer)

#ifdef __cplusplus
}
#endif

#endif
