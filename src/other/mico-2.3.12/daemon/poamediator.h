// -*- c++ -*-
/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (C) 1998 Frank Pilhofer
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
 *  For more information, visit the MICO Home Page at
 *  http://www.mico.org/
 */


#ifndef __poamediator_h__
#define __poamediator_h__

#include <CORBA.h>
#include <mico/impl.h>

class POAMediatorImpl : virtual public CORBA::ObjectAdapter,
			virtual public POA_CORBA::POAMediator,
			virtual public CORBA::ORBCallback,
			virtual public MICO::ProcessCallback
{
public:
  // must be public for Sun CC
  enum ServerState {
    Inactive,
    Started,
    Active,
    Holding,
    Stopped,
    Failed
  };

private:
  struct SvInf {
    SvInf();
    ServerState pstate;
    CORBA::IOR ior;
    MICO::UnixProcess * proc;
    CORBA::Long failed;
  };

  typedef std::map<std::string, SvInf, std::less<std::string> > MapSvInf;
  typedef std::map<CORBA::ORBMsgId, CORBA::ORBMsgId, std::less<CORBA::ORBMsgId> > MapIdId;

  MapSvInf svmap;
  MapIdId requests;
  CORBA::ORB_var orb;
  std::string myior;
  CORBA::Boolean forward;

  CORBA::ImplRepository_var imr;
  MICO::RequestQueue invqueue;

  MICOMT::Mutex _M_global_lock;
  CORBA::Object_var my_ref_;
public:
  POAMediatorImpl (CORBA::ORB_ptr, CORBA::Boolean forward = FALSE);
  ~POAMediatorImpl ();

  /*
   * POAMediator stuff
   */

  char * create_impl (const char *, const char *);
  void activate_impl (const char *);
  void deactivate_impl (const char *);

  CORBA::Boolean force_activation (CORBA::ImplementationDef_ptr);
  CORBA::Boolean hold (CORBA::ImplementationDef_ptr);
  CORBA::Boolean stop (CORBA::ImplementationDef_ptr);
  CORBA::Boolean _cxx_continue (CORBA::ImplementationDef_ptr);

  /*
   * Helper
   */

  CORBA::Boolean create_server (const char *);
  void set_own_ref(CORBA::Object_ptr obj);

  /*
   * ObjectAdapter interface
   */

  const char *get_oaid () const;
  CORBA::Boolean has_object (CORBA::Object_ptr);
  CORBA::Boolean is_local () const;
    
#ifdef USE_CSL2
  CORBA::Principal_ptr get_principal (CORBA::Object_ptr);
#endif /* USE_CSL2  */
  CORBA::Boolean invoke (CORBA::ORBMsgId, CORBA::Object_ptr,
			 CORBA::ORBRequest *,
			 CORBA::Principal_ptr,
			 CORBA::Boolean response_exp = TRUE);
  CORBA::Boolean bind (CORBA::ORBMsgId, const char *repoid,
		       const CORBA::ORB::ObjectTag &,
		       CORBA::Address *addr);
  CORBA::Boolean locate (CORBA::ORBMsgId, CORBA::Object_ptr);
  CORBA::Object_ptr skeleton (CORBA::Object_ptr);
  void cancel (CORBA::ORBMsgId);
  void shutdown (CORBA::Boolean wait_for_completion);
    
  void answer_invoke (CORBA::ORBMsgId, CORBA::Object_ptr,
		      CORBA::ORBRequest *,
		      CORBA::InvokeStatus);

  /*
   * Callbacks (ORB, Process)
   */

  CORBA::Boolean waitfor (CORBA::ORB_ptr, CORBA::ORBMsgId,  CORBA::ORBCallback::Event, CORBA::Long);
  void notify (CORBA::ORB_ptr, CORBA::ORBMsgId, CORBA::ORBCallback::Event);

  void callback (MICO::Process *, MICO::ProcessCallback::Event);

  /*
   * Shutdown
   */

  void shutdown_server ();
};

#endif
