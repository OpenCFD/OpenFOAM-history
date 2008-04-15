/*
 *  MICOStream --- a simple stream service for MICO
 *  Copyright (C) 1998 Christian Becker
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

#ifndef __MICO_STREAM__
#define __MICO_STREAM__

#include <coss/stream.h>

struct write_record
{
  MICOStream::StreamData  *data;    // data to be written
  int                     pos;      // actual position
  bool                    do_free;  // callback frees data
  bool                    is_ready; // if not, set true on transmission
};

class StreamTransport : public virtual MICOStream::Transport_skel,
			public virtual CORBA::TransportCallback
{
 protected:
    std::vector<MICOStream::StreamData> _buffer;
    CORBA::Transport *_t;
    std::list<write_record *> write_buffer;
    bool writing;
    MICOStream::StreamData newChunk;
    bool disconnecting;
 public:
    StreamTransport();
    virtual ~StreamTransport();
    virtual MICOStream::StreamData* read();
    virtual MICOStream::StreamData* poll();
    virtual void receive( const MICOStream::StreamData& chunk );
    virtual void write( const MICOStream::StreamData& chunk );
    virtual void write_async( const MICOStream::StreamData& chunk );
    virtual void callback (CORBA::Transport *, CORBA::TransportCallback::Event);
};


class ActiveCEP_impl : public virtual StreamTransport,
		       public virtual CORBA::TransportCallback,
                       public virtual MICOStream::ActiveCEP_skel
{
 private:

 public:
    ActiveCEP_impl();
    virtual ~ActiveCEP_impl();

    virtual void connect( const char* bind_addr );
    virtual void disconnect();


};

class PassiveCEP_impl : public virtual StreamTransport,
			public CORBA::TransportServerCallback,
		
                        public virtual MICOStream::PassiveCEP_skel
{
 protected:
    CORBA::TransportServer *_ts;
    char *_bind_addr;


 public:
    PassiveCEP_impl();
    virtual ~PassiveCEP_impl();

    virtual char* bind_addr();
    virtual void bind( const char* bind_addr );
    virtual void disconnect();

    virtual void callback (CORBA::TransportServer *, CORBA::TransportServerCallback::Event);
};

#endif
