
/* whether or not --enable-mini-stl is used */
#undef HAVE_MINI_STL

/* whether CORBASec Level 2 is used */
#undef USE_CSL2

/* whether CSIv2 is used */
#undef USE_CSIV2

/* whether anything wireless is built */
#undef USE_WIRELESS

/* whether we have byteorder big endian */
#undef HAVE_BYTEORDER_BE

/* whether we have byteorder little endian */
#undef HAVE_BYTEORDER_LE

/* whether we have a prototype for gethostname */
#undef HAVE_GETHOSTNAME_PROTO

/* whether we have a prototype for finite */
#undef HAVE_FINITE_PROTO

/* whether we have a prototype for strerror */
#undef HAVE_STRERROR_PROTO

/* whether we have a prototype for ftime */
#undef HAVE_FTIME_PROTO

/* whether C++ compiler supports exception handling */
#undef HAVE_EXCEPTS

/* whether C++ compiler supports exceptions in shared libs */
#undef HAVE_SHARED_EXCEPTS

/* whether C++ compiler supports standard exception handling */
#undef HAVE_STD_EH

/* whether to built ORB with exception support */
#undef HAVE_ORB_EXCEPTS

/* whether we need to explicitely call global ctors in dlopen()ed module */
#undef HAVE_EXPLICIT_CTORS

/* whether we have a size limit on .o files (like hpux) */
#undef HAVE_SIZE_LIMIT

/* whether we have IEEE compliant floating point types */
#undef HAVE_IEEE_FP

/* whether or not class foo { friend void f <> (); } works */
#undef HAVE_GUIDING_DECLS

/* whether or not namespaces work */
#undef HAVE_NAMESPACE

/* type of size argument to socket calls */
#undef socket_size_t

/* type of sockaddr argument to socket calls */
#undef socket_addr_t

/* type of fd_set arguments to select calls */
#undef select_addr_t

/* whether gettimeofday has two args */
#undef HAVE_GETTIMEOFDAY_TWOARGS

/* whether or not he have SSLeay */
#undef HAVE_SSL

/* whether or not he have Postgresql */
#undef HAVE_PGSQL

/* whether or not we have Bluetooth */
#undef HAVE_BLUETOOTH

/* whether or not bool is supported by C++ compiler */
#undef HAVE_BOOL_TYPE

/* whether or not typename is supported by C++ compiler */
#undef HAVE_TYPENAME
#undef typename

/* whether or not scanf ("%Lf", ...) works */
#undef HAVE_SCANF_LF

/* whether or not printf ("%Lf", ...) works */
#undef HAVE_PRINTF_LF

/* whether the system supports dynamic module loading */
#undef HAVE_DYNAMIC

/* whether c++ compiler supports const overload of conversion operators */
#undef HAVE_CONST_OVERLOAD

/* whether virtual methods inherited from two or more base class must be
   overridden */
#undef HAVE_EXPLICIT_METHOD_OVERRIDE

/* whether the compiler fails to automatically generate working
   copy constructors, assignment operators and destructors on structs
   with template members */
#undef HAVE_EXPLICIT_STRUCT_OPS

/* whether we can use the advanced memchecker */
#undef USE_MEMTRACE

/* whether a derived class can override a typedef in the base class */
#undef HAVE_TYPEDEF_OVERLOAD

/* Define if you have the <poll.h> header file.  */
#undef HAVE_POLL_H

/* Define if you have the <pth.h> header file.  */
#undef HAVE_PTH_H

/* Define if you have pthread support */
#undef HAVE_PTHREADS

/* Define if you have DCE thread support */
#undef HAVE_DCE_THREADS

/* Define if you have Solaris thread support */
#undef HAVE_SOLARIS_THREADS

/* Define if you have GNU Pth thread support */
#undef HAVE_PTH_THREADS

/* Define if you want thread support at all */
#undef HAVE_THREADS

/* whether to build a Minimum CORBA ORB */
#undef HAVE_MINIMUM_CORBA

/* whether we support dynamic_cast<> */
#undef HAVE_DYNAMIC_CAST

/* whether to build CCM-specific parts */
#undef USE_CCM

/* path information */
#undef ABSEXECDIR
#undef ABSSHRDDIR

/* whether ANSI C++ header files are present */
#undef HAVE_ANSI_CPLUSPLUS_HEADERS
