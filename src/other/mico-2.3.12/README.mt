

RELEASE NOTES for MICO/MT 2.3.6

This is mainly stabilisation release thought it brings some new
features. The main effort in the past was in I/O domain where Andreas
tried to achieve the most parallelism rate as could be done. So we saw
separate reader/writer threads for each connection, thread pool for
decoding messages and orb thread pool. It was great achieve and
Andreas did great work.

Since Andreas couldn't finish his work with integration of MICO/MT
into main MICO I decided to help the project a little and fix some
bugs. During this time I tried to compare MICO/MT framework with MT
frameworks of other open-source orbs around the world from the point
of performance and scalability and I found some areas where MICO/MT
lags behind. The main problem is that Andreas's achievement of the
most parallel I/O framework goes against achievement to make MICO/MT
more scalable. Simply I think that it's not right to allocate 4000 OS
threads for serving 2000 clients because not all people have such
*big* machine. I decided to slightly change MICO/MT internals while
using all good stuff which Andreas wrote e.g. channels, operations
etc. and slightly limit the number of used threads. I implemented two
concurrency models: thread pool and thread per connection (in reality
I implemented another model: thread per request, but I considered it
useless and so it's not debugged and developed any more). 

Thread pool concurrency model
-----------------------------
This model is on by default so you don't need to use any option to
switch it on. In case that you are running your servers/clients with
other concurrency model by default (by selecting it in micorc file),
you can use option '-ORBThreadPool' for switching it on. This model
uses constant number of threads for serving all connections. This is
possible to change number of threads by using '-ORBRequestLimit
<number>'. By default MICO/MT uses four threads in thread pool.
In case that your server (code) is not thread aware you can
use '1' for number of threads and then MICO/MT will run in single
threaded mode. Don't be afraid that you will see two server threads
running, the first one is server's main thread which is working in
select loop and sending incomming message to the request thread pool
in which you have selected number of threads. The second one is the
only one thread (as you selected by -ORBRequestLimit 1) in request
thread pool which will make all up-calls to the POA(s) and
servant(s). For this model there is available unlimited number of
incomming connections. If you would like to limit number of incomming
connections, you can use '-ORBConnLimit <number>' parameter.

Thread per connection concurrency model
---------------------------------------
This model uses one thread per each connection in tcp blocking
mode. It's slightly faster than thread pool model above but it's a
little bit scalable. To switch it on please use
'-ORBThreadPerConnection' parameter. By default there are ten
connection available, to increase this number use '-ORBConnLimit
<number>' parameter.


What's new
----------

 - MICO 2.3.6 source tree merged in
 - implemented two concurrency models: thread pool and thread per connection
 - thread pool concurrency model doesn't use separate reader/writer threads for each incoming connection
   any more as the old implementation does
 - it's possible to build sources without thread support
 - configure provides options for selecting thread package
 - ported to FreeBSD and OpenBSD

Fixes in this release
---------------------
 
 - fixed oneway invocations
 - fixed SSL support
 - several fixes in IIOP/GIOP/Connection management - e.g. GIOP
   crashing while sending random data to the server
   and deadlock while throwing SYS exception on the client side
 - fixed POA for usage with servant manager
 - compilation fixes in micod (it still doesn't work well but compiles
   without error)
 - naming service was changed to be MT ready - e.g. it's POAs
   have thread policy with single thread value

Tested on
---------

 Platforms: x86: GNU/Linux, FreeBSD, OpenBSD (MICO/MT compiles well on
                 it, but due to bad c++ behavior on this platform many
                 examples which run well somewhere else crash) 
            sparc64: GNU/Linux, Solaris 8
            s390: GNU/Linux

 Thread package: only POSIX thread support is tested (and developed)

 Examples: generic obv/abstract obv/date obv/tree poa/hello-1
           poa/hello-2 poa/inherit poa/account-1 poa/account-2
           run well on every platform
 Example: ssl tested only on x86 GNU/Linux - runs well
 Example: obv/tricky broken on bigendian machines (sparc64 and s390)
 Example: poa/account-3 broken on every platform due to error in micod
          and IIOP/GIOP while working with forwarded messages

 Note that examples were started in default MICO/MT mode and so each
run with thread pool concurrency model.




---------------------------------------------------------------------




RELEASE NOTES for MICO/mt - version 2000-01-10
----------------------------------------------

This is the second alpha release of MICO-mt.
It represents a step on the long way to a
full multi-threaded MICO. Don't expect it
to work completely or flawless.

------------------- WARNING -----------------------------

  PLEASE don't send "it won't work"-style bug-reports.
         those will be redirected >/dev/null
 
         If it's broken - fix it !

         or provide sufficient info, 
     so that we can hunt the problem down. 

---------------------------------------------------------

This release has been tested on HP/UX 11 using HP's aCC and
Linux 2.3.25 using GCC-2.95.2 and GLIBC 2.1
It's not known if this release will work with
other compilers and/or lib-c versions.

Features:

- pthread, dce-thread, pththreads, solaris-threads support
- multi-threaded I/O (one accept() thread per Socket, one read() and one
  write() thread per Socket)
- limited support for multi-threaded CORBA invocations

Known Problems / Limitations:

- nameservices may be completly broken (Andy was there)
- configure does only superficial tests for pthreads
- thread support only for pthread draft 10 and LinuxThreads
- pth and solaris thread support is only theoretic
- the ORB, BOA and POA might not be completely thread safe at the moment
- POA thread policies are not yet honored
- micod is broken
- CORBA Services are not thread safe
- ORB shutdown is broken
- tons of other unknown bugs  ;-)

NEW DEBUG OPTIONS
-----------------

The mt version uses a lot of debugcode at the moment.
Much of this will go away once we are sure that the bulk of changes is
stable enough. To be able to control debugging better the -ORBDebugLevel
options is now a bit mask and we have introduced a -ORBDebugModule options
being also a bit mask.

ORBDebugLevel:

1 - simple debug output
2 - trace function calls
4 - dump
8 - profiling outdump

ORBDebugModule

1   - Global
2   - Support
4   - Thread
8   - ORB
16  - POA
32  - GIOP
64  - IMR
128 - IR

HOW TO COMPILE
--------------

MICO/mt needs a thread-safe STL implementation. We have not yet verified
if MICO's mini-stl is thread safe or not.
To be safe, you should use your system supplied STL.

To build MICO/mt for Linux do:

	./configure --enable-threads
	make

To build MICO/mt for HP/UX 11 do (for csh):

	setenv CC 'aCC -D_INCLUDE_LONGLONG'
	setenv CXX aCC
	./configure --enable-threads
	make

The HP/UX 11.0 configuration uses/relies upon the system supplied
Rogue Wave STL.

Have fun - but don't get caught!


    Andreas
