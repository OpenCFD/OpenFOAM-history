#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#ifdef _WIN32
#include <mico/config-win32.h>
#else // _WIN32
#include <mico/config.h>
#endif // _WIN32

#ifdef HAVE_THREADS
#include <CORBA.h>
#include <mico/os-thread.h>

static MICOMT::Mutex _S_memcheck_mutex;
#endif

#ifdef USE_MEMTRACE

#include <mico/memtrace.h>

void *operator new (size_t sz)
{
#ifdef HAVE_THREADS
  MICOMT::AutoLock __lock (_S_memcheck_mutex);
#endif
  return MemTrace_Malloc (sz);
}

void *operator new[] (size_t sz)
{
#ifdef HAVE_THREADS
  MICOMT::AutoLock __lock (_S_memcheck_mutex);
#endif
  return MemTrace_Malloc (sz);
}

void operator delete (void *_p)
{
#ifdef HAVE_THREADS
  MICOMT::AutoLock __lock (_S_memcheck_mutex);
#endif
  if (!_p) {
    return;
  }
  MemTrace_Free (_p);
}

void operator delete[] (void *_p)
{
#ifdef HAVE_THREADS
  MICOMT::AutoLock __lock (_S_memcheck_mutex);
#endif
  if (!_p) {
    return;
  }
  MemTrace_Free (_p);
}

#else

#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif


using namespace std;

/*
 * Simple memchecker -- outputs some stats
 */

static long _memory = 0;
static long _memory_max = 0;

static struct Report {
  ~Report ()
  {
    cerr << "memstats: "
	 << _memory_max << " bytes max; "
	 << _memory << " bytes not freed" << endl;
  }
} __report;

void *operator new (size_t sz)
{
#ifdef HAVE_THREADS
    MICOMT::AutoLock __lock (_S_memcheck_mutex);
#endif
    long *p = (long *)::malloc (sz+2*sizeof(long));
    assert (p);
    assert (p[0] != 0x27182818 && p[0] != 0x27182819);
    p[0] = 0x27182818;
    p[1] = sz;
    memset (p+2, 0x66, sz);

    _memory += sz;
    if (_memory > _memory_max)
      _memory_max = _memory;

    return p+2;
}

void *operator new[] (size_t sz)
{
#ifdef HAVE_THREADS
    MICOMT::AutoLock __lock (_S_memcheck_mutex);
#endif
    long *p = (long *)::malloc (sz+2*sizeof(long));
    assert (p);
    assert (p[0] != 0x27182818 && p[0] != 0x27182819);
    p[0] = 0x27182819;
    p[1] = sz;
    memset (p+2, 0x66, sz);

    _memory += sz;
    if (_memory > _memory_max)
      _memory_max = _memory;

    return p+2;
}

void operator delete (void *_p)
{
#ifdef HAVE_THREADS
    MICOMT::AutoLock __lock (_S_memcheck_mutex);
#endif
    if (!_p)
	return;
    long *p = (long *)_p-2;
    size_t sz = p[1];
    assert (p[0] == 0x27182818);
    memset (p, 0x55, sz+2*sizeof(long));
    ::free (p);

    _memory -= sz;
}

void operator delete[] (void *_p)
{
#ifdef HAVE_THREADS
    MICOMT::AutoLock __lock (_S_memcheck_mutex);
#endif
    if (!_p)
	return;
    long *p = (long *)_p-2;
    size_t sz = p[1];
    assert (p[0] == 0x27182819);
    memset (p, 0x55, sz+2*sizeof(long));
    ::free (p);

    _memory -= sz;
}

#endif
