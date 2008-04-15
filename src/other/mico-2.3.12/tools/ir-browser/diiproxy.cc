/*
 *  MICO --- a free CORBA implementation
 *  Copyright (C) 1997 Kay Roemer & Arno Puder
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  Send comments and/or bug reports to:
 *                 mico@informatik.uni-frankfurt.de
 */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#ifndef _WIN32
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#endif
#include <stdio.h>
#include <string.h>
#ifdef _AIX
#include <strings.h>
#endif
#include <signal.h>
#include <time.h>
//#include <netdb.h>

#include <fstream.h>
#include <strstream.h>
#include <string>

#ifdef __CYGWIN32__
#ifdef HAVE_BYTEORDER_H
#include <byteorder.h>
#else
#include <asm/byteorder.h>
#endif
#endif

#include <CORBA.h>
#include <mico/os-net.h>
#include "accountI.h"
#include "browserI.h"
#include "parser.h"
#include <mico/template_impl.h>


static string thePeerAddr = "<server>";
static int theUserCount = 0;
static const int maxUserCount = 10;


static bool
denied (const string &s)
{
    return FALSE;
}


static void
setpeer (const sockaddr_in &sin)
{
    struct hostent *hent = gethostbyaddr ((char *)&sin.sin_addr.s_addr,
                                          sizeof (sin.sin_addr.s_addr),
                                          sin.sin_family);
    if (hent)
        thePeerAddr = hent->h_name;
    else
        thePeerAddr = inet_ntoa (sin.sin_addr);
}


static void
log (const string &s)
{
    time_t t = time (NULL);
    string timestr = ctime (&t);
    timestr = timestr.substr (0, timestr.length()-1);

    cout << "/* " << thePeerAddr << " [" << timestr << "]: */ " << s << endl;
}


static void
cleanup (int)
{
#ifndef _WIN32
    int stat, pid;

    while ((pid = waitpid (-1, &stat, WNOHANG)) > 0) {
        cout << "/* child " << pid << " exited */" << endl;
        --theUserCount;
    }
    signal( SIGCLD, cleanup );
#endif
}


static string
execute_request (const string &req, DII &dii)
{
    ostrstream ostr;
    ostr << req << ends;
    istrstream istr (ostr.str());
    ostr.rdbuf()->freeze (0);

    Parser parser (&dii, istr, "<stdin>");
    parser.parse();
    if (parser.errorOccured()) {
	string rep = "@error 1 : '";
	rep += parser.getErrorMsg();
	rep += "in line ";
	rep += parser.getErrorLine();
	rep += "'";
	return rep;
    }
    return dii.getAnswerString ();
}


static int
net_csend( int sock, const char *s )
{
    unsigned long size;
    int bytes_to_send;
    int len;

    bytes_to_send = strlen( s ) + 1;
    size = htonl( bytes_to_send );
    len = OSNet::sock_write( sock, &size, sizeof( unsigned long ) );
    if( len == -1 )
	/* return any error */
	return -1;

    if( len < sizeof( unsigned long ) ) {
	/* partial write? */
	errno = EIO;
	return -1;
    }

    len = OSNet::sock_write( sock, s, bytes_to_send );
    if( len == -1 )
	/* return any error */
	return -1;

    if( len < bytes_to_send ) {
	/* partial write? */
	errno = EIO;
	return -1;
    }
    return 0; /* success */
}



static int
net_read( int sock, char *buf, int buflen )
{
    int left;
    int len;
    char *p;

    left = buflen;
    p = buf;
    while( left > 0 ) {
	len = OSNet::sock_read( sock, p, left );
	if( len == -1 )
	    /* error occured */
	    return -1;
	if( len == 0 )
	    /* can't read any more */
	    break;
	
	left -= len;
	p += len;
    }
    return buflen - left;
}



static int
net_crecv( int sock, char **buf )
{
    unsigned long size;
    int len;
    unsigned long buflen;

    /* read the response header */
    len = net_read( sock, (char *) &size, sizeof( unsigned long ) );
    if( len == -1 )
	/* oops */
	return -1;
    if( len < sizeof( unsigned long ) ) {
	errno = EIO;
	return -1;
    }

    /* read buffer */
    buflen = ntohl( size );
    *buf = (char *) malloc( buflen );
    len = net_read( sock, (char *) *buf, buflen );
    if( len == -1 )
	return -1;
    if( len < buflen ) {
	errno = EIO;
	return -1;
    }
    return buflen;
}


static int
net_slisten( int port )
{
    int sock, new_sock;
    struct sockaddr_in sin, from;
    socket_size_t len;

    memset ( (char *) &sin, 0, sizeof(sin) );
    sin.sin_family = AF_INET;
    sin.sin_port = htons (port);

    sock = ::socket( AF_INET, SOCK_STREAM, 0 );
    if( sock < 0 )
	return -1;

    OSNet::sock_reuse (sock, TRUE);

    if( ::bind( sock, (sockaddr *) &sin, sizeof(sin) ) < 0 )
	return -1;

    listen( sock, 1 );
    while( 1 ) {
	len = sizeof( from );
	new_sock = ::accept( sock, (sockaddr *) &from, &len );
	if( new_sock < 0 ) {
	    if( errno == EINTR )
		continue;
	    return -1;
	}
	if (theUserCount > maxUserCount) {
	    net_csend (new_sock, "too many users. try again later");
	    close (new_sock);
	} else {
	    ++theUserCount;
#ifdef _WIN32
            setpeer (from);
            (void) OSNet::sock_close (sock);
            return new_sock;
#else
	    switch (fork ()) {
	    case -1: /* error? */
		return -1;
		
	    case 0: /* child */
		setpeer (from);
		(void) close( sock );
		return new_sock;
		
	    default: /* parent */
		signal( SIGCLD, cleanup );
		close( new_sock );
		break;
	    }
#endif
	}
    }
    /* NOTREACHED */
}




static void
net_server( int sock )
{
    char *java_host;
    int len, wlen, i;
    char *buf;
    char response[ 200 ];
    int rc;

    /* First string denotes the hostname of the Java client */
    len = net_crecv( sock, &java_host );
    if( len == -1 ) {
        log (string ("/* read on socket failed: ") +
             strerror (errno) + " */");
        exit( 1 );
    }

    DII dii;
    
    while( 1 ) { /* until an error occurs */
        /* read a request */
        len = net_crecv( sock, &buf );
 
        if( len == -1 ) {
            log (string("/* read on socket failed: ") +
                 strerror (errno) + " */");
            exit( 1 );
        }

        if (denied (buf)) {
            len = net_csend (sock, "@error 20 : 'Operation not permitted'");
            log (string(buf) + " /* -> denied */");
        } else {
	    string answer = execute_request (buf, dii);
            len = net_csend (sock, answer.c_str());
            log (string(buf) + " /* -> " + answer.c_str() + " */");
        }
        free( buf );
                
        if( len == -1 ) {
            log (string("/* write on socket failed: ") +
                 strerror (errno) +
                 " */");
            exit( 1 );
        }
    }
}


int
main( int argc, char *argv[] )
{
#ifndef _WIN32
    int sock;
    sock = net_slisten( 3399 );
#endif

    /*
     * every client gets its own impl, because net_slisten() fork()s and
     * return()s for each new client.
     */
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");
    CORBA::BOA_var boa = orb->BOA_init (argc, argv, "mico-local-boa");

#ifdef _WIN32
    int sock;
    sock = net_slisten( 3399 );
#endif

    new Account_impl;
    new IRBrowser_impl;
    net_server( sock );

    return 0;
}

