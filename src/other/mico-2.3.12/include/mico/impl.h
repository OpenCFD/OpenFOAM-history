// -*- c++ -*-
/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2005 by The Mico Team
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  For more information, visit the MICO Home Page at
 *  http://www.mico.org/
 */

#ifndef __mico_impl_h__
#define __mico_impl_h__

#if defined(_WIN32) && !defined(__MINGW32__)
#include <mico/config-win32.h>
#else
#include <mico/config.h>
#endif

#ifdef HAVE_THREADS
#include <mico/mtdebug.h>
#endif

#include <mico/util.h>

#if defined(_WIN32) && !defined(__MINGW32__)
#include <sys/timeb.h>
#include <winsock2.h>
#elif defined (_WIN32) && defined(__MINGW32__)
#include <sys/timeb.h>
#include <winsock.h>
#elif defined (_POCKET_PC)
//#include <sys/timeb.h>
#include <winsock.h>
#else
#include <sys/time.h>
#include <sys/socket.h>
#endif

#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#ifdef HAVE_POLL_H
#include <poll.h>
#endif // HAVE_POLL_H

/*
 * MICO Namespace
 */

#include <mico/fast_array.h>

#ifdef HAVE_THREADS
#include <mico/message.h>
#include <mico/operation.h>
#include <mico/mt_manager.h>
#include <mico/operation_impl.h>
#endif

#include <mico/shlib.h>
#include <mico/process.h>
#include <mico/address_impl.h>
#include <mico/ior_impl.h>
#include <mico/select_dispatcher.h>
#ifdef HAVE_POLL_H
#include <mico/poll_dispatcher.h>
#endif // HAVE_POLL_H
#include <mico/transport_impl.h>
#include <mico/transport/tcp.h>
#include <mico/transport/udp.h>
#include <mico/transport/unix.h>
#include <mico/shlib_impl.h>
#include <mico/process_impl.h>
#include <mico/codec_impl.h>
#include <mico/codeset_impl.h>
#include <mico/orb_impl.h>
#ifdef HAVE_THREADS
#include <mico/mt_dispatcher.h>
#endif

#include <mico/iop.h>
#include <mico/policy_impl.h>

#endif // __mico_impl_h__
