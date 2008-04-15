/*
 *  Mico --- a free CORBA implementation
 *  Copyright Frank Pilhofer
 *
 *  This file is part of MicoCCM, written by Frank Pilhofer.
 *  The MICO CORBA Component Project was sponsored by Alcatel.
 */

/*
 * At the moment, all components for philo.idl are implemented in this file
 */

#include "philo.h"

/*
 * ----------------------------------------------------------------------
 * Helpers to introduce some randomness
 * ----------------------------------------------------------------------
 */

#include <mico/os-misc.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif


using namespace std;

void MySrand ()
{
  static bool initialized = false;

  if (!initialized) {
    OSMisc::TimeVal tv = OSMisc::gettime ();
    CORBA::Long pid = OSMisc::getpid ();
    srand ((unsigned int) (tv.tv_usec + pid) % 991);
    initialized = true;
  }
}

int MyRand ()
{
  MySrand ();
  return (int) rand ();
}

/*
 * ----------------------------------------------------------------------
 * StatusInfo event type
 * ----------------------------------------------------------------------
 */

class StatusInfo_impl :
  virtual public OBV_DiningPhilosophers::StatusInfo,
  virtual public CORBA::DefaultValueRefCountBase
{
public:
  StatusInfo_impl ();
};

StatusInfo_impl::StatusInfo_impl ()
{
}

class StatusInfoFactory : public CORBA::ValueFactoryBase
{
public:
  CORBA::ValueBase * create_for_unmarshal ();
};

CORBA::ValueBase *
StatusInfoFactory::create_for_unmarshal ()
{
  return new StatusInfo_impl;
}

/*
 * ----------------------------------------------------------------------
 * Fork component
 * ----------------------------------------------------------------------
 */


class Fork_impl : virtual public MyFork {
private:
  bool _inuse;

public:
  Fork_impl ()
    {
      _inuse = false;
    }

  void get ()
    {
      if (_inuse) {
	throw DiningPhilosophers::InUse ();
      }

      _inuse = true;
    }

  void release ()
    {
      assert (_inuse);
      _inuse = false;
    }

  DiningPhilosophers::CCM_Fork_ptr get_the_fork ()
    {
      return DiningPhilosophers::CCM_Fork::_duplicate (this);
    }
};

class ForkHome_impl : virtual public DiningPhilosophers::CCM_ForkHome {
public:
  Components::EnterpriseComponent_ptr create ()
    {
      return new Fork_impl;
    }
};


/*
 * ----------------------------------------------------------------------
 * Philosopher component
 * ----------------------------------------------------------------------
 */


class Philosopher_impl :
  virtual public MyPhilosopher,
  virtual public CORBA::DispatcherCallback
{
private:
  CORBA::Dispatcher * _disp;
  CORBA::ULong _state;
  CORBA::ULong _speed;
  bool _have_left_fork;
  bool _have_right_fork;
  bool _active;
  CORBA::String_var _name;
  DiningPhilosophers::CCM_Philosopher_Context_var _ctx;

public:
  Philosopher_impl ()
    {
      _active = false;
      _disp = 0;
      _state = 0;
      _have_left_fork = false;
      _have_right_fork = false;
      _name = CORBA::string_dup ("unnamed");
      // random metabolic rate
      _speed = 1500 + (MyRand() % 13) * 300;
    }

  void name (const char * newname)
    {
      _name = CORBA::string_dup (newname);
    }

  char * name ()
    {
      return CORBA::string_dup (_name.in());
    }

  void push_state ()
    {
      DiningPhilosophers::StatusInfo_var si = new StatusInfo_impl;
      si->name (_name.in());

      if (_have_left_fork && _have_right_fork) {
	si->state (DiningPhilosophers::EATING);
      }
      else if (_state < 3) {
	si->state (DiningPhilosophers::THINKING);
      }
      else if (_state < 10) {
	si->state (DiningPhilosophers::HUNGRY);
      }
      else if (_state < 40) {
	si->state (DiningPhilosophers::STARVING);
      }
      else {
	si->state (DiningPhilosophers::DEAD);
      }

      si->ticks_since_last_meal (_state);
      si->has_left_fork (_have_left_fork);
      si->has_right_fork (_have_right_fork);

      _ctx->push_info (si.in());
    }

  void set_session_context (Components::SessionContext_ptr ctx)
    {
      _ctx = DiningPhilosophers::CCM_Philosopher_Context::_narrow (ctx);
      assert (!CORBA::is_nil (_ctx));
    }

  void ccm_activate ()
    {
      if (!_active) {
	CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
	_disp = orb->dispatcher ();
	_disp->tm_event (this, _speed);

	cout << _name << " is active, speed is "
	     << _speed << endl;

	_active = true;
      }
    }

  void ccm_passivate ()
    {
      if (!_active) {
	return;
      }

      _active = false;

      if (_disp) {
	_disp->remove (this, CORBA::Dispatcher::Timer);
      }

      cout << _name << " is inactive." << endl;

      if (_have_left_fork) {
	try {
	  DiningPhilosophers::Fork_var _left = _ctx->get_connection_left ();
	  _left->release ();
	  _have_left_fork = false;
	} catch (...) {
	};
      }

      if (_have_right_fork) {
	try {
	  DiningPhilosophers::Fork_var _right = _ctx->get_connection_right ();
	  _right->release ();
	  _have_right_fork = false;
	} catch (...) {
	}
      }

      push_state ();
    }

  void ccm_remove ()
    {
    }

  void callback (CORBA::Dispatcher * d, CORBA::Dispatcher::Event e)
    {
      assert (e == CORBA::Dispatcher::Timer);

      _disp->tm_event (this, _speed);

      if (_have_left_fork && _have_right_fork) {
	/*
	 * Eating
	 */
	if (_state > 3) {
	  _state -= 3;
	  push_state ();
	  return;
	}
	_state = 0;

	try {
	  DiningPhilosophers::Fork_var _left = _ctx->get_connection_left ();
	  _left->release ();
	  _have_left_fork = false;
	  DiningPhilosophers::Fork_var _right = _ctx->get_connection_right ();
	  _right->release ();
	  _have_right_fork = false;
	} catch (...) {
	}
	cout << _name << " has eaten." << endl;
	push_state ();
	return;
      }

      _state++;

      if (_state < 3) {
	push_state ();
	return;
      }

      if (_state == 3) {
	cout << _name << " is becoming hungry." << endl;
      }
      else if (_state >= 10) {
	cout << _name << " is starving." << endl;
      }

      DiningPhilosophers::Fork_var _left = _ctx->get_connection_left ();

      if (!_have_left_fork && !CORBA::is_nil (_left)) {
	try {
	  _left->get ();
	  _have_left_fork = true;
	}
	catch (...) {
	}

	if (_have_left_fork) {
	  cout << _name << " has got the left fork." << endl;
	  push_state ();
	  return;
	}
	else {
	  // cout << _name << " did not get the left fork." << endl;
	}
      }

      DiningPhilosophers::Fork_var _right = _ctx->get_connection_right ();

      if (!_have_right_fork && !CORBA::is_nil (_right)) {
	try {
	  _right->get ();
	  _have_right_fork = true;
	}
	catch (...) {
	}

	if (_have_right_fork) {
	  cout << _name << " has got the right fork." << endl;
	  push_state ();
	  return;
	}
	else {
	  // cout << _name << " did not get the right fork." << endl;
	}
      }

      if (((_have_left_fork && !_have_right_fork) ||
	   (!_have_left_fork && _have_right_fork)) && ((rand() % 3) == 0)) {
	if (_have_left_fork) {
	  try {
	    _left->release ();
	    _have_left_fork = false;
	    cout << "Deadlock prevention! " << _name
		 << " drops the left fork." << endl;
	  }
	  catch (...) {
	  }
	}
	if (_have_right_fork) {
	  try {
	    _right->release ();
	    _have_right_fork = false;
	    cout << "Deadlock prevention! " << _name
		 << " drops the right fork." << endl;
	  }
	  catch (...) {
	  }
	}
      }

      push_state ();
    }
};

class PhilosopherHome_impl : virtual public DiningPhilosophers::CCM_PhilosopherHome {
public:
  Components::EnterpriseComponent_ptr create ()
    {
      return new Philosopher_impl;
    }

  Components::EnterpriseComponent_ptr _cxx_new (const char * thename)
    {
      Philosopher_impl * np = new Philosopher_impl;
      np->name (thename);
      return np;
    }
};


/*
 * ----------------------------------------------------------------------
 * Observer component
 * ----------------------------------------------------------------------
 */

class Observer_impl : virtual public DiningPhilosophers::CCM_Observer {
public:
  Observer_impl ()
    {
    }

  void push_info (DiningPhilosophers::StatusInfo * si)
    {
      printf ("%-20s: ", si->name ());

      switch (si->state()) {
      case DiningPhilosophers::EATING:   printf ("Eating     "); break;
      case DiningPhilosophers::THINKING: printf ("Thinking   "); break;
      case DiningPhilosophers::HUNGRY:   printf ("Hungry     "); break;
      case DiningPhilosophers::STARVING: printf ("Starving   "); break;
      case DiningPhilosophers::DEAD:     printf ("Dead       "); break;
      }

      if (si->has_left_fork()) {
	printf ("* ");
      }
      else {
	printf ("  ");
      }

      if (si->has_right_fork()) {
	printf ("* ");
      }
      else {
	printf ("  ");
      }

      for (CORBA::ULong i=0; i<si->ticks_since_last_meal(); i++) {
	printf (".");
      }

      printf ("\n");
    }
};

class ObserverHome_impl : virtual public DiningPhilosophers::CCM_ObserverHome {
public:
  Components::EnterpriseComponent_ptr create ()
    {
      return new Observer_impl;
    }
};

/*
 * ----------------------------------------------------------------------
 * Entry points
 * ----------------------------------------------------------------------
 */


extern "C" {
  Components::HomeExecutorBase_ptr
  create_DiningPhilosophers_ForkHome ()
  {
    return new ForkHome_impl;
  }

  Components::HomeExecutorBase_ptr 
  create_DiningPhilosophers_PhilosopherHome ()
  {
    return new PhilosopherHome_impl;
  }

  Components::HomeExecutorBase_ptr
  create_DiningPhilosophers_ObserverHome (CORBA::ORB_ptr orb)
  {
    CORBA::ValueFactoryBase_var f = new StatusInfoFactory;
    orb->register_value_factory ("IDL:DiningPhilosophers/StatusInfo:1.0", f);
    return new ObserverHome_impl;
  }
}

