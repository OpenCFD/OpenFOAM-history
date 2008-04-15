// -*- c++ -*-
/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
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

#ifndef __mico_os_net_h__
#define __mico_os_net_h__

#if defined (_WIN32) || defined (_POCKET_PC)
#include <errno.h>
#if !defined (__MINGW32__) && !defined (_POCKET_PC)
#include <winsock2.h>
#include <limits>
#else
#include <winsock.h>
#endif

#ifndef EWOULDBLOCK
#define EWOULDBLOCK WSAEWOULDBLOCK
#endif

#ifndef ECONNRESET
#define ECONNRESET WSAECONNRESET
#endif

#ifndef ENOTCONN
#define ENOTCONN WSAENOTCONN
#endif

#ifndef ECONNREFUSED
#define ECONNREFUSED WSAECONNREFUSED
#endif

class OSNet {

#ifdef _DEBUG

  struct WSA_Number_Text_struct {
    int Number;
    char* Text;
  };

#define WSA_Number_Text_table_n 88
  static WSA_Number_Text_struct WSA_Number_Text_table[WSA_Number_Text_table_n];

  static void PrintWSAError (int n) 
  {
    int i;
    for( i=0; i < WSA_Number_Text_table_n; i++ ) {
      if (WSA_Number_Text_table[i].Number == n) {
	std::cout << "WSA: " << 
	  WSA_Number_Text_table[i].Number << " = " <<
	  WSA_Number_Text_table[i].Text << "\n";
	//break;
      }
    }
  }

#endif // _DEBUG

public:
    static void set_errno ()
    {
	int err = WSAGetLastError();
  
	switch (err) {
	case 0:
	    errno = 0;
	    break;

	case WSAEINTR:
	    errno = EINTR;
	    break;

	case WSAEWOULDBLOCK:
	    errno = EWOULDBLOCK; // EAGAIN
	    break;

	case WSAECONNRESET:
	    errno = ECONNRESET;
	    break;

	case WSAENOTCONN:
	    errno = ENOTCONN;
	    break;

	case WSAECONNREFUSED:
	    errno = ECONNREFUSED;
	    break;

        case WSAESHUTDOWN:
            errno = ECONNRESET;
            break;

	default:
#ifdef _DEBUG
	    std::cerr << "unhandled WSAGetLastError() result" << std::endl;
            PrintWSAError( err );
#endif
	    errno = -1;
	    break;
	}
    }

    static void sock_init ()
    {
	static bool winsock_init = false;

	if( !winsock_init ) {
	    WORD wVersionRequested;
	    WSADATA wsaData;
	    int err;
	
	    wVersionRequested = MAKEWORD( 2, 0 );
	    
	    err = WSAStartup( wVersionRequested, &wsaData );
	    assert( err == 0 );
	    winsock_init = true;
	}
    }

    static void sock_block (MICO_Long fd, MICO_Boolean on)
    {
	unsigned long para = on ? 0 /* NONBLOCK disabled */ : 1;
	ioctlsocket (fd,FIONBIO,&para);
    }

    static void sock_reuse (MICO_Long fd, MICO_Boolean on)
    {
	int _on = on;
	::setsockopt (fd, SOL_SOCKET, SO_REUSEADDR, (char *)&_on, sizeof(_on));
    }

    static void sock_broadcast (MICO_Long fd, MICO_Boolean on)
    {
	int _on = on;
	::setsockopt (fd, SOL_SOCKET, SO_BROADCAST, (char *)&_on, sizeof(_on));
    }

    static void sock_ndelay (MICO_Long fd, MICO_Boolean on)
    {
#ifdef TCP_NODELAY
	int _on = on;
	::setsockopt (fd, IPPROTO_TCP, TCP_NODELAY, (char *)&_on, sizeof(_on));
#endif
    }

    static MICO_Long sock_read (MICO_Long fd, void *buf, MICO_ULong count)
    {
	WSASetLastError(0);

	int ret = recv(fd,(char*)buf,count,0);
	if (ret != SOCKET_ERROR)
	    return ret;

	set_errno();

	return -1;
    }

    static MICO_Long sock_read_from (MICO_Long fd, void *buf,
				     MICO_ULong count,
				     struct sockaddr *sa,
				     MICO_ULong sa_count)
    {
	WSASetLastError(0);

	socket_size_t fromlen = sa_count;
	int ret = recvfrom(fd,(char*)buf,count,0,(socket_addr_t)sa,&fromlen);
	if (ret != SOCKET_ERROR)
	    return ret;

	set_errno();

	return -1;
    }

    static MICO_Long sock_write (MICO_Long fd, const void *buf,
				 MICO_ULong count)
    {
	WSASetLastError(0);

	int ret = send(fd,(char*)buf,count,0);
	if (ret != SOCKET_ERROR)
	    return ret;

	set_errno();

	return -1;
    }

    static MICO_Long sock_write_to (MICO_Long fd, const void *buf,
				    MICO_ULong count,
				    const struct sockaddr *sa,
				    MICO_ULong sa_count)
    {
	WSASetLastError(0);

	int ret = sendto(fd,(char*)buf,count,0,(socket_addr_t)sa,sa_count);
	if (ret != SOCKET_ERROR)
	    return ret;

	set_errno();

	return -1;
    }

    static MICO_Long sock_close (MICO_Long fd)
    {
	WSASetLastError(0);

	int ret = closesocket(fd);
	if (ret == 0)
	    return 0;

	set_errno();
	
	return -1;
    }

    static MICO_Long sock_shutdown (MICO_Long fd)
    {
	// Sorin Mustaca <msorin@gecadsoftware.com>: This is
	// incomplete: see MSDN/Platform SDK (shutdown) to see why.
	WSASetLastError(0);

	int ret = shutdown(fd,SD_SEND);
	if (ret == 0)
		return 0;
	set_errno();
	return -1;
    }
};

#else // _WIN32

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>

#include <sys/socket.h>
#ifdef HAVE_SYS_UN_H
#include <sys/un.h>
#endif
#include <netinet/in.h>
#include <arpa/inet.h>
#ifdef HAVE_NETINET_TCP_H
#include <netinet/tcp.h>
#endif
#include <netdb.h>
#include <signal.h>

#ifndef INADDR_LOOPBACK
#define INADDR_LOOPBACK 0x7F000001
#endif

#ifndef INADDR_ANY
#define INADDR_ANY 0x0
#endif

#ifndef SHUT_RDWR
#define SHUT_RDWR 2
#endif // SHUT_RDWR

#ifdef __CYGWIN32__
extern "C" int ioctl (int, int, void *);
#endif

class OSNet {
#ifdef __CYGWIN32__
    static MICO_Boolean sock_blocking (MICO_Long fd)
    {
	CORBA::Long flags = fcntl (fd, F_GETFL, 0);
	assert (flags != -1);
	return !!(flags & O_NONBLOCK);
    }

    static MICO_Boolean sock_readable (MICO_Long fd)
    {
	MICO_Long nbytes = 0;
	::ioctl (fd, FIONREAD, &nbytes);
	return nbytes > 0;
    }
#endif
public:
    static void set_errno ()
    {
    }

    static void sock_init ()
    {
        // when peer closes a unix socket we receive SIGPIPE ...
        signal (SIGPIPE, SIG_IGN);
    }

    static void sock_block (MICO_Long fd, MICO_Boolean on)
    {
	CORBA::Long flags = fcntl (fd, F_GETFL, 0);
	assert (flags != -1);
	flags = on ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
	fcntl (fd, F_SETFL, flags);
    }

    static void sock_reuse (MICO_Long fd, MICO_Boolean on)
    {
	int _on = on;
	::setsockopt (fd, SOL_SOCKET, SO_REUSEADDR, (char *)&_on, sizeof(_on));
    }

    static void sock_broadcast (MICO_Long fd, MICO_Boolean on)
    {
	int _on = on;
	::setsockopt (fd, SOL_SOCKET, SO_BROADCAST, (char *)&_on, sizeof(_on));
    }

    static void sock_ndelay (MICO_Long fd, MICO_Boolean on)
    {
#ifdef TCP_NODELAY
	int _on = on;
	::setsockopt (fd, IPPROTO_TCP, TCP_NODELAY, (char *)&_on, sizeof(_on));
#endif
    }

    static MICO_Long sock_read (MICO_Long fd, void *buf, MICO_ULong count)
    {
#ifdef __CYGWIN32__
	if (!sock_blocking (fd) && !sock_readable (fd)) {
	    errno = EWOULDBLOCK;
	    return -1;
	}
#endif
	return ::read (fd, buf, count);
    }

    static MICO_Long sock_read_from (MICO_Long fd, void *buf,
				     MICO_ULong count,
				     struct sockaddr *sa,
				     MICO_ULong sa_count)
    {
#ifdef __CYGWIN32__
	if (!sock_blocking (fd) && !sock_readable (fd)) {
	    errno = EWOULDBLOCK;
	    return -1;
	}
#endif
	socket_size_t fromlen = sa_count;
	return ::recvfrom (fd, (char *)buf, count, 0,
			   (socket_addr_t)sa, &fromlen);
    }

    static MICO_Long sock_write (MICO_Long fd, const void *buf,
				 MICO_ULong count)
    {
	return ::write (fd, buf, count);
    }

    static MICO_Long sock_write_to (MICO_Long fd, const void *buf,
				    MICO_ULong count,
				    const struct sockaddr *sa,
				    MICO_ULong sa_count)
    {
	return ::sendto (fd, (char *)buf, count, 0,
			 (socket_addr_t)sa, sa_count);
    }

    static MICO_Long sock_close (MICO_Long fd)
    {
	return ::close (fd);
    }

    static MICO_Long sock_shutdown (MICO_Long fd)
    {
	return ::shutdown (fd, SHUT_RDWR);
    }
};

#endif // _WIN32

#endif // __mico_os_net_h__
