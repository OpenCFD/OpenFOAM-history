
- merged with MICO 2.3.7

version 2.3.6

- added configure into cvs
- fixed compilation of single-threaded build
- changed configure.in to prefer POSIX threads than Solaris threads on Solaris
- fixed POA_impl::perform_invoke for orec handling while using servant manager
- made ObjectActivationLock recursive
- MT debug code was switched off
- added posix4 library for solaris build
- fixed GIOPConn crashing bug number #437748
- added -D_THREAD_SAFE param for compilation with gcc on *BSD
- added global dep (or .depend) rule, it makes .depend file in all
  prgdirs. It's possible to use -j make option after creation of .depend
  files
- fixed managing of GIOPConn's active refs on client side (IIOPProxy)
  - it causes bug #439533
- changed thread-pool concurrency model, it doesn't use separate accept thread
  for incoming connections any more
- made global debug mutex not recursive
- renamed swap16 to swap_16 because of name clash with OpenBSD's swap16 macro
- fixed time service for compilation on *BSD
- removed old gcc related params from configure
- added checking for pthread's symbols in libc_r (needs for FreeBSD, OpenBSD)
- added semaphore implementation based on mutex & condvar (needs for OpenBSD)
  it is used on OSes which doesn't provide POSIX implementation and semaphore.h
- fixed ssl support
- removed _tpm static value from global MICO:: namespace into MTManager class
- implemented disabling of connection checking with using `-ORBConnLimit 0',
  connection checking is disabled by default for thread-pool concurrency model
- fixed compilation of final build
- fixed --enable-threads configure option
- fixed server scalability problem with thread-pool concurrency model
  which uses separate reader/writer threads for each GIOP connection
- added initialization of MT stuff into main ORB init
- implemented priority handling in GNU Pth thread support
- fixed data key handling in GNU Pth thread support
- fixed GNU Pth support for compilation
- changed configure to disable MICO parts which are not MT ready
- added support for GNU Pth thread package into configure
- changed configure for better handling of thread package configuration
- changed optimization from '-O' to '-O2'
- merged with MICO 2.3.6
- implemented thread-per-request thread model. Please use `-ORBThreadPerRequest' option
  to turn it on.
- implemented thread-per-connection thread model. Please use `-ORBThreadPerConnection'
  option to turn it on.
- changed methods' names do_CloseConnection -> close_connection,
  do_InputReady -> input_ready, do_InputReadyCallback -> input_ready_callback,
  set_InvokeHint -> set_invoke_hint, get_InvokeHint -> get_invoke_hint,
  set_RequestHint -> set_request_hint, get_RequestHint -> get_request_hint,
  activeDeref -> active_deref, activeRef -> active_ref, sendORBMsg -> send_orb_msg,
  acquireConnection -> acquire_connection, releaseConnection -> release_connection,
  connectionLimit -> connection_limit, requestLimit -> request_limit,
  getThreadPoolManager -> get_thread_pool_manager, StateChange -> state_change,
  State -> state
- fixed naming service for use in multi-threading environment
- fixed compilation of TimeService with using threads
- changed to use memtrace for STL
- added malloc/free macros into memtrace tool
- changed to use orb request cache for ST build
- fixed mem-leak in method ORB::new_msgid
- fixed threading stuff for compiling without DEBUG_NAMES
- restored debuging in orb/os-thread/* and include/mico/os-thread/* files
- added usage of AutoDebugLock into all debug code
- added AutoDebugLock class which uses __mtdebug_(un)lock methods for locking/unlocking
  Logger streams
- removed global mutex from Logger class
- changed methods' names getThreadPool -> get_thread_pool, setThreadPool -> set_thread_pool,
  get_OP_id -> get_op_id, set_OP_id -> set_op_id, get_nextOP -> get_next_op,
  set_nextOP -> set_next_op, Info -> info, Thread -> thread, initializeShutdown ->
  initialize_shutdown, finalizeShutdown -> finalize_shutdown, registerInputMC ->
  register_input_mc, getInputMC -> get_input_mc, initShutdown -> init_shutdown,
  getThreadPool -> get_thread_pool, setThreadPool -> set_thread_pool, setOperation ->
  set_operation, registerOperation -> register_operation, deregisterOperation ->
  deregister_operation, getOperation -> get_operation, setThreadPoolManager ->
  set_thread_pool_manager
- chaged classes names _namedObject -> NamedObject, passiveOperation -> PassiveOperation,
  activeOperation -> ActiveOperation, activeMsgQueue -> ActiveMsgQueue,
  passiveMsgQueue -> PassiveMsgQueue, directMsgConnector -> DirectMsgConnector
- added global mutex for debug streams into Logger class
- added remains debug ''modes'' into Logger class
- added simple producer/consumer demo
- added simple multi-threaded callback demo
- changed single-threaded build to use ''empty'' MT classes instead of many #ifdefs
- added ''empty'' MT classes for single-threaded build
- first attempt to fix build of MICOMT without thread support
- changed classes names in MICOMT namespace: auto_Lock -> AutoLock, rwLock -> RWLock, 
  rwLocked -> RWLocked, auto_rdLock -> AutoRDLock, auto_wrLock -> AutoWRLock
- daemon and ir dirs added for generating documentation
- merged with MICO CVS from Jul 23 2001
- first attempt to fix micod - only for POA based servers now
- first attempt to fix ORB::new_msgid for skiping MsgIds of pending invocations
- first attempt to fix one way calls
- fixed two mem-leaks
- fixed memcheck.cc for MT
- merged with MICO CVS from Jul 3 2001
