#include "bench.h"
#ifdef _WIN32
#include <iostream>
#else
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS
#endif // _WIN32

#ifdef USE_POA
class Bench_impl3 : virtual public POA_Bench {
public:
    Bench_var _bench;
    CORBA::Long _level;
public:
    void f ()
    {
    }
    void sync ()
    {
    }
    void g ()
    {
	if (--_level > 0)
	    _bench->g();
    }
    void connect (Bench_ptr b, CORBA::Long l)
    {
	_bench = Bench::_duplicate (b);
	_level = l;
    }
};
#endif

