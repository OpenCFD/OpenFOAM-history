/*
 *  MICOStream - a simple stream service
 *  Copyright (C) 1998 Christian Becker
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

#define MICO_CONF_IMR
#include <CORBA-SMALL.h>
#include <coss/stream_impl.h>
#include <mico/impl.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif


using namespace std;

StreamTransport::StreamTransport()
{
  writing = false;
  _t = NULL;
}

StreamTransport::~StreamTransport()
{
  if (_t){
    _t->close();
    delete _t;
  }
}
MICOStream::StreamData *
StreamTransport::read()
{

  while (_buffer.size()==0) // lousy busy waits
    _orbnc()->dispatcher()->run(FALSE);

  MICOStream::StreamData *ret = new MICOStream::StreamData(_buffer.front());
  _buffer.erase(_buffer.begin());

  return (ret);
}

MICOStream::StreamData *
StreamTransport::poll()
{
  MICOStream::StreamData *ret;

  if (_buffer.size()){
    ret = new MICOStream::StreamData(_buffer.front());
    _buffer.erase(_buffer.begin());
    return (ret);
  }
  else{
    ret = new MICOStream::StreamData;
    ret->length(0);
    return (ret);
  }
}

void
StreamTransport::receive(const MICOStream::StreamData& chunk)
{
   // exactly one sequence, contents appended
    if (_buffer.size()){
      MICOStream::StreamData *actData = new MICOStream::StreamData(_buffer[0]);
      int i=actData->length();
      /*
      actData->length(_buffer[0].length() + chunk.length());
      for (int j=0 ; j<chunk.length(); i++, j++)
	(*actData)[i] = chunk[j];
      */

      memcpy(&((*actData)[i]), &(chunk[0]), chunk.length());

      _buffer.erase(_buffer.begin());
      _buffer.push_back(*actData);
    }else
      _buffer.push_back(chunk);
}


void
StreamTransport::write(const MICOStream::StreamData& chunk)
{
 
  write_record *rec = new write_record;

  rec->data = (MICOStream::StreamData *) &chunk;
  rec->do_free = false;
  rec->is_ready = false;
  rec->pos = 0;

  write_buffer.push_back(rec);

  callback(_t, CORBA::TransportCallback::Write);

  while (!rec->is_ready)
    _orbnc()->dispatcher()->run(FALSE);

  delete rec;

}

void
StreamTransport::write_async(const MICOStream::StreamData& chunk)
{
  write_record *rec = new write_record;
  
  rec->data = new MICOStream::StreamData;
  rec->data->length(chunk.length());
  rec->pos = 0;

  memcpy(&((*rec->data)[0]), &(chunk[0]), chunk.length());
  rec->do_free = TRUE;

  write_buffer.push_back(rec);
  
  callback(_t, CORBA::TransportCallback::Write);

}
void
StreamTransport::callback(CORBA::Transport *d, CORBA::TransportCallback::Event e)
{

  if (e == CORBA::TransportCallback::Read){

    CORBA::Long _len = 4096;    
    newChunk.length(4096);
    assert (_t);
    _len = _t->read((void *)&(newChunk[0]), _len);

    if (_len > 0){
      newChunk.length(_len);
      receive(newChunk);
    }else if ( (_len == 0) && ( _t->eof())) {
      _t->rselect(_orbnc()->dispatcher(), NULL);
    }else if ( (_len < 0) && (!disconnecting) ){
      assert(0);  // exception...
    }
     
  }else if (e == CORBA::TransportCallback::Write){
    if (write_buffer.size()){
      write_record *actrec;
      actrec = write_buffer.front();

      CORBA::Octet *_b;
      int _len;

      _b = (CORBA::Octet *) &((*actrec->data)[0]); // first el in sequence
      _b += actrec->pos;

      _len = _t->write( (void *) _b, actrec->data->length() - actrec->pos);
      assert(_len >=0 ); // execptions preferred ;-)

      if (_len == (int)actrec->data->length() - actrec->pos){
	// completely written
	if (actrec->do_free){      // asynch write: free record and data
	  write_buffer.pop_front();
	  delete actrec->data;
	  delete actrec;
	}else{   // synch write: destroy list entry, mark record written
	  actrec->is_ready = TRUE;
	  write_buffer.pop_front(); // synch. write: caller keeps pointer aliased
	}
	if (write_buffer.empty()){ // nothing more to write
	  writing = FALSE;
	  _t->wselect(_orbnc()->dispatcher(),NULL);
	}else{
          writing = TRUE;
	  _t->wselect(_orbnc()->dispatcher(), this);
	}
      }else{ 
	actrec->pos += _len;
	writing = TRUE;
	_t->wselect(_orbnc()->dispatcher(), this);
      }
    }else{ // write buffer empty - nothing to write
      writing = FALSE;
      _t->wselect(_orbnc()->dispatcher(), NULL);
    }
  }
}

// Active CEP

ActiveCEP_impl::ActiveCEP_impl()
{
  _t = NULL;
  newChunk.length(4096);
  // bool disconnecting = FALSE;
}

ActiveCEP_impl::~ActiveCEP_impl()
{
}

void
ActiveCEP_impl::connect(const char * bind_addr)
{
  CORBA::Address *addr = CORBA::Address::parse (bind_addr);
  if (!addr) {    
    mico_throw(MICOStream::NotBound ());
  }

  _t = addr->make_transport();
  if (!_t->connect(addr)){
    mico_throw(MICOStream::NotConnected ());
  }
  _t->block (FALSE);
  _t->rselect (_orbnc()->dispatcher(), this);

  delete addr;
}

void
ActiveCEP_impl::disconnect()
{
  if (_t){
    disconnecting = TRUE;
    _t->rselect(_orbnc()->dispatcher(), NULL);
    _t->wselect(_orbnc()->dispatcher(), NULL);

    _t->close();
  }

  while (_buffer.size())
  {
    _buffer.pop_back();
  }
}




// Passive CEP


PassiveCEP_impl::PassiveCEP_impl()
{
  _bind_addr = NULL;
  newChunk.length(4096);
}

PassiveCEP_impl::~PassiveCEP_impl()
{
  if (_bind_addr)
    CORBA::string_free (_bind_addr);
}

char *
PassiveCEP_impl::bind_addr(void)
{
  if (_bind_addr)
    return (CORBA::string_dup(_bind_addr));
  assert(0);
  return 0;
}

void
PassiveCEP_impl::bind(const char *bind_addr)
{
  CORBA::Address *addr = CORBA::Address::parse (bind_addr);
  if (!addr) {
    string s = "inet:";
    s += MICO::InetAddress::hostname();
    s += ":0";
    addr = CORBA::Address::parse (s.c_str());
    assert (addr);
  }

  _ts = addr->make_transport_server();
  if (!_ts->bind(addr)){
    mico_throw(MICOStream::NotBound ());
  }
  _ts->block (FALSE);
  _ts->aselect (_orbnc()->dispatcher(), this);

  _bind_addr = CORBA::string_dup (_ts->addr()->stringify().c_str());
  delete addr;
}

void
PassiveCEP_impl::disconnect()
{
  if (_t){
    _t->close();
    _t->rselect(_orbnc()->dispatcher(), NULL);
    _t->wselect(_orbnc()->dispatcher(), NULL);

    delete _t;
  }
  if (_ts){
    _ts->aselect(_orbnc()->dispatcher(), NULL);
    _ts->close();
    delete _ts;
  }

  while (_buffer.size())
  {
    _buffer.pop_back();
  }
}

void
PassiveCEP_impl::callback(CORBA::TransportServer *d, CORBA::TransportServerCallback::Event e)
{
  if ( e == CORBA::TransportServerCallback::Accept){
    _t = _ts->accept();
    _t->block (FALSE);
    _t->rselect (_orbnc()->dispatcher(), this);
  }
}
