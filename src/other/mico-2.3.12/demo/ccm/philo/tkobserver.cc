/*
 *  Mico --- a free CORBA implementation
 *  Copyright Frank Pilhofer
 *
 *  This file is part of MicoCCM, written by Frank Pilhofer.
 *  The MICO CORBA Component Project was sponsored by Alcatel.
 */

#include <tcl.h>
#include <tk.h>
#include "philo.h"

/*
 * ----------------------------------------------------------------------
 * Tcl Code
 * ----------------------------------------------------------------------
 */

char startscript[] = "\
#\n\
# ----------------------------------------------------------------------\n\
#\n\
# Generated files here. Ignore this section (until the next --- line)\n\
#\n\
\n\
set alcatel \"\n\
R0lGODdhyAA2APIAAAAAAICAgP8AAP//AP///wAAAAAAAAAAACwAAAAAyAA2AAAD/ki63P4wykmp\n\
uOGKjHWuYCiOZGmeaDQIa8u+birPdG3fijZw67X7uKBwSJwJCC/kSnksOp/QKJKz8Xik2Kz25Oom\n\
t+CwuPHy8S7jtDqbbA/W8DjR2mnK73iZd5nv+0dnQH+DhBFHbYWJilNXi46DXY+SfSwfk5d3X5ib\n\
amicn2lvoKOkpaanqKmqq6ytrq+wsbKztLW2swABubu6vby/vsHAw8LFxMfGycjLys3Mz87R0Ly6\n\
ALvW1dfa2dzW297d2ODj4uXf5uHn6uns5Ovu7ejw8/L12tPS+fj7+v38//4C4vMljuCwgt8MCkPY\n\
TeEvhuMcSkxIsWHFiBchajzI/nFhR4gCQwLktUCkyZEnUyKL985ePAYt30FwSS9mugo1uUWgWQ4m\n\
y3QACYhkMHSmQAJBKejbubTkyI8KFWTsCHOjxwAOcjm1+hCAgokWdeEMGzECWYY+py70t2AkUX5p\n\
fSEN6LTo3H9t7QoF+HNc1b498+YE14Cnzao24W1NjG0x4GtpDWeD+5Zt3WmVUe4qbNetVL0hwZbl\n\
rJak4LOmkUIt7eBq14WXuRKMvVpnW9nnmpLWvVe35mAPOh81ivlyv8dEHQ/2KliycsnkiENv/HX5\n\
tp2MIzMGScxpZmnfn53WzBTvZ/N3h1cXWDvteNRYb9e2Rtv1RAm4G962n3D/u+v/vcSF0Vng4ceb\n\
ZeuhNIF6nqWHXm/89EXBdNr9VNh2tuE3nVbVbRjfXh7S19x20FTA227ICBhfcROg12BWJ6q4UkYV\n\
Isbfh1m1J8GNzCk14I89iqaRWQBeB4GQ1zgTHo4swtgkismEgOCLUC5TXjSPZWmdluK0tuWXIYaJ\n\
IZgx/WamSmieqWaa/syXH5Buxsnjm3TKWSR8d8I5J5tr9snnn36ixOWYYg5qKJmHFoqodYEC6mij\n\
kD4aTAIAOw==\n\
\"\n\
\n\
set fpx \"\n\
R0lGODdhaAA3APcAAAAAAAEBAQICAgMDAwQEBAUFBQYGBgcHBwgICAkJCQoKCgsLCwwMDA0NDQ4O\n\
Dg8PDxAQEBERERISEhMTExQUFBUVFRYWFhcXFxgYGBkZGRoaGhsbGxwcHB0dHR4eHh8fHyAgICEh\n\
ISIiIiMjIyQkJCUlJSYmJicnJygoKCkpKSoqKisrKywsLC0tLS4uLi8vLzAwMDExMTIyMjMzMzQ0\n\
NDU1NTY2Njc3Nzg4ODk5OTo6Ojs7Ozw8PD09PT4+Pj8/P0BAQEFBQUJCQkNDQ0REREVFRUZGRkdH\n\
R0hISElJSUpKSktLS0xMTE1NTU5OTk9PT1BQUFFRUVJSUlNTU1RUVFVVVVZWVldXV1hYWFlZWVpa\n\
WltbW1xcXF1dXV5eXl9fX2BgYGFhYWJiYmNjY2RkZGVlZWZmZmdnZ2hoaGlpaWpqamtra2xsbG1t\n\
bW5ubm9vb3BwcHFxcXJycnNzc3R0dHV1dXZ2dnd3d3h4eHl5eXp6ent7e3x8fH19fX5+fn9/f4CA\n\
gIGBgYKCgoODg4SEhIWFhYaGhoeHh4iIiImJiYqKiouLi4yMjI2NjY6Ojo+Pj5CQkJGRkZKSkpOT\n\
k5SUlJWVlZaWlpeXl5iYmJmZmZqampubm5ycnJ2dnZ6enp+fn6CgoKGhoaKioqOjo6SkpKWlpaam\n\
pqenp6ioqKmpqaqqqqurq6ysrK2tra6urq+vr7CwsLGxsbKysrOzs7S0tLW1tba2tre3t7i4uLm5\n\
ubq6uru7u7y8vL29vb6+vr+/v8DAwMHBwcLCwsPDw8TExMXFxcbGxsfHx8jIyMnJycrKysvLy8zM\n\
zM3Nzc7Ozs/Pz9DQ0NHR0dLS0tPT09TU1NXV1dbW1tfX19jY2NnZ2dra2tvb29zc3N3d3d7e3t/f\n\
3+Dg4OHh4eLi4uPj4+Tk5OXl5ebm5ufn5+jo6Onp6erq6uvr6+zs7O3t7e7u7u/v7/Dw8PHx8fLy\n\
8vPz8/T09PX19fb29vf39/j4+Pn5+fr6+vv7+/z8/P39/f7+/v///ywAAAAAaAA3AAAI/gD/CRxI\n\
sKDBgwgTKlzIsKHDhxAjSpxI8UQMHEGWTNkyxg0dP4QYTcrUqRMmSILonLmCBMeJCgAAKMBQQsaQ\n\
KF7U0Al0qFGiQXfQZEFio8SEmDEPPLjw4cQLHEKORMkS5gycO38GHVq0CBEhPnPSbHHyI0YICQEU\n\
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\n\
LgACEeCGOuoQAQEAOw==\n\
\"\n\
\n\
set forkimg \"\n\
R0lGODlhFAAaAOMAANTU1BsgHyYnJxQYF9fX1tfX1x4jJAAAABsfIKysrBATEwQFBbq6uhAUE9jY\n\
2NbW1SH+Dk1hZGUgd2l0aCBHSU1QACwAAAAAFAAaAAAEUxDISau9OOeAufYU+FGCuFGDiQnDpJ4E\n\
8HZAIWsVGBy4ZOg8ymGYE5mIOUQoKEG6VAqkUzYbLHjIgFYzbGi/s9D3EL5se0V0Wh1it92SMnzi\n\
mD8iADs=\n\
\"\n\
\n\
set nofork \"\n\
R0lGODlhFAAaAKUAAPewsNTU1BsgHzMEAyYnJxQYF0YFBdfX1tfX1/eurverqx4jJAAAAPevr9TT\n\
0+PDw/etrdnOzhsfIKysrNjPz/epqdvMzBATE9XS0gcAAAEBAe2MjPmenh8CAj8ODW4WFjcIB9jY\n\
2NbW1f//////////////////////////////////////////////////////////////////////\n\
/////////////////////////////////////////////yH+Dk1hZGUgd2l0aCBHSU1QACwAAAAA\n\
FAAaAAAGekCAMEAsGo3CJODIJCqVTeQT0KhGA9TGEKvdHpNaYzXJBDO7XqywEYUW3c2nOl2ekq/z\n\
hOKOVzcSdFEODxB/ABF9RBQAEAkNFRaJahAQexiJVVocWX2ZnlZXWaCZS218c6Vfp6ipTnBvcHJx\n\
fK91W6u2Q7i5ASGSqCJBADs=\n\
\"\n\
\n\
#\n\
# ----------------------------------------------------------------------\n\
# GUI Setup\n\
# ----------------------------------------------------------------------\n\
#\n\
\n\
wm title . \"Combat Dining Philosophers\"\n\
menu .menu -tearoff 0\n\
.menu add cascade -label \"File\" -menu .menu.file -underline 0\n\
menu .menu.file -tearoff 0\n\
.menu.file add command -label \"Exit\" -command \"exit\" -underline 0\n\
\n\
frame .title -relief raised -bd 2\n\
label .title.text -justify center -text \"Dining Philosophers Observer\" \\\n\
	-font -Adobe-Helvetica-Bold-R-Normal--*-180-*-*-*-*-*-*\n\
pack .title.text -fill both -padx 10 -pady 5 -side top\n\
pack .title -fill x -side top\n\
\n\
frame .logos -background \"#ffffff\"\n\
frame .logos.both -background \"#ffffff\"\n\
set fpxpic [image create photo -data $fpx]\n\
set alcatelpic [image create photo -data $alcatel]\n\
label .logos.both.alcatel -image $alcatelpic -background \"#ffffff\"\n\
label .logos.both.fpx -image $fpxpic -background \"#ffffff\"\n\
pack .logos.both.alcatel .logos.both.fpx -side left -padx 10 \\\n\
	-expand true -fill both\n\
pack .logos.both -anchor center -expand true -fill both\n\
pack .logos -side top -expand true -fill both -anchor center\n\
\n\
set emptypic [image create photo -width 20 -height 26]\n\
set forkpic [image create photo -data $forkimg]\n\
set noforkpic [image create photo -data $nofork]\n\
\n\
frame .philo\n\
for {set i 0} {$i < $maxphilosophers} {incr i} {\n\
    frame .philo.$i -relief sunken -bd 1\n\
    label .philo.$i.name -justify left -anchor w -width 18 \\\n\
	    -foreground \"#000000\"\n\
    label .philo.$i.left -justify center -anchor center -image $emptypic \\\n\
	    -width 20 -height 26\n\
    label .philo.$i.right -justify center -anchor center -image $emptypic \\\n\
	    -width 20 -height 26\n\
    canvas .philo.$i.hungry -height 26 -width 320\n\
    .philo.$i.hungry create rectangle 0 8 0 18 -tags hungryness \\\n\
	    -fill [.philo.$i.hungry cget -background] \\\n\
	    -outline [.philo.$i.hungry cget -background]\n\
    pack .philo.$i.name .philo.$i.left .philo.$i.right \\\n\
	    -side left -padx 2 -pady 2\n\
    pack .philo.$i.hungry -fill x -expand true\n\
    pack .philo.$i -fill x -side top -padx 2\n\
    frame .philo.filler-$i -relief raised -bd 1\n\
    pack .philo.filler-$i -side top -pady 1 -fill x\n\
}\n\
\n\
pack .philo -fill x\n\
\n\
frame .status -relief raised -bd 2\n\
label .status.left -justify left -anchor w -width 30\n\
label .status.right -justify right -anchor e -width 20\n\
pack .status.left -fill x -side left -expand true\n\
pack .status.right -side right\n\
pack .status -side bottom -fill x\n\
\n\
#\n\
# ----------------------------------------------------------------------\n\
# Update GUI - consume StatusInfo event\n\
# ----------------------------------------------------------------------\n\
#\n\
\n\
proc UpdateGUI {statusinfo} {\n\
    global philosophers maxphilosophers\n\
    global forkpic noforkpic\n\
\n\
    array set status $statusinfo\n\
\n\
    #\n\
    # New Philosopher? (first event from that philosopher)\n\
    #\n\
\n\
    if {![info exists philosophers($status(name))]} {\n\
	set philosophers($status(name)) $philosophers(count)\n\
	.philo.$philosophers($status(name)).name configure \\\n\
		-text $status(name)\n\
	incr philosophers(count)\n\
	.status.left configure -text \"$philosophers(count) Philosophers\"\n\
    }\n\
\n\
    #\n\
    # Update data for that Philosopher\n\
    #\n\
\n\
    set i $philosophers($status(name))\n\
\n\
    if {$status(has_left_fork)} {\n\
	.philo.$i.left configure -image $forkpic\n\
    } else {\n\
	.philo.$i.left configure -image $noforkpic\n\
    }\n\
    if {$status(has_right_fork)} {\n\
	.philo.$i.right configure -image $forkpic\n\
    } else {\n\
	.philo.$i.right configure -image $noforkpic\n\
    }\n\
\n\
    set hungryness [expr $status(ticks_since_last_meal) * 8]\n\
\n\
    switch $status(state) {\n\
	EATING -\n\
	THINKING {\n\
	    set color \"#008000\"\n\
	}\n\
	HUNGRY {\n\
	    set color \"#000000\"\n\
	}\n\
	STARVING {\n\
	    set color \"#FFFF00\"\n\
	}\n\
	default {\n\
	    set color \"#FF0000\"\n\
	}\n\
    }\n\
\n\
    .philo.$i.hungry coords hungryness 0 8 $hungryness 18\n\
    .philo.$i.hungry itemconfigure hungryness -fill $color\n\
}\n\
\n\
set philosophers(count) 0\n\
.status.left configure -text \"Passive\"\n\
\n\
";

/*
 * ----------------------------------------------------------------------\n\
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
 * Observer component
 * ----------------------------------------------------------------------
 */

class TkObserver_impl : 
  virtual public MyObserver,
  virtual public CORBA::DispatcherCallback
{
private:
  Tcl_Interp * myinterp;
  CORBA::Dispatcher * _disp;
  bool _active;

public:
  TkObserver_impl (Tcl_Interp * interp)
    {
      _active = false;
      myinterp = interp;
      CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
      _disp = orb->dispatcher ();
      _disp->tm_event (this, 100);
    }

  void set_session_context (Components::SessionContext_ptr)
    {
    }

  void ccm_activate ()
    {
      if (!_active) {
	_active = true;
	Tcl_Eval (myinterp, ".status.left configure -text \"Waiting for Events ...\"");
      }
    }

  void ccm_passivate ()
    {
      if (!_active) {
	return;
      }

      _active = false;
      Tcl_Eval (myinterp, ".status.left configure -text \"Passive\"");
    }

  void ccm_remove ()
    {
      if (_disp) {
	_disp->remove (this, CORBA::Dispatcher::Timer);
      }
      Tcl_DeleteInterp (myinterp);
    }

  void callback (CORBA::Dispatcher * d, CORBA::Dispatcher::Event e)
    {
      assert (e == CORBA::Dispatcher::Timer);
      _disp->tm_event (this, 100);
      while (Tcl_DoOneEvent (TCL_DONT_WAIT));
    }

  void push_info (DiningPhilosophers::StatusInfo * si)
    {
      Tcl_Obj *com, *cmd[2], *elements[10];

      elements[0] = Tcl_NewStringObj ("name", -1);
      elements[1] = Tcl_NewStringObj ((char *) si->name(), -1);
      elements[2] = Tcl_NewStringObj ("state", -1);

      switch (si->state()) {
      case DiningPhilosophers::EATING:
	elements[3] = Tcl_NewStringObj ("EATING", -1);
	break;

      case DiningPhilosophers::THINKING:
	elements[3] = Tcl_NewStringObj ("THINKING", -1);
	break;

      case DiningPhilosophers::HUNGRY:
	elements[3] = Tcl_NewStringObj ("HUNGRY", -1);
	break;

      case DiningPhilosophers::STARVING:
	elements[3] = Tcl_NewStringObj ("STARVING", -1);
	break;

      case DiningPhilosophers::DEAD:
	elements[3] = Tcl_NewStringObj ("DEAD", -1);
	break;
      }

      elements[4] = Tcl_NewStringObj ("ticks_since_last_meal", -1);
      elements[5] = Tcl_NewIntObj ((int) si->ticks_since_last_meal());
      elements[6] = Tcl_NewStringObj ("has_left_fork", -1);
      elements[7] = Tcl_NewBooleanObj (si->has_left_fork());
      elements[8] = Tcl_NewStringObj ("has_right_fork", -1);
      elements[9] = Tcl_NewBooleanObj (si->has_right_fork());

      cmd[0] = Tcl_NewStringObj ("UpdateGUI", -1);
      cmd[1] = Tcl_NewListObj (10, elements);
      com = Tcl_NewListObj (2, cmd);
      Tcl_IncrRefCount (com);

      Tcl_EvalObjv (myinterp, 2, cmd, TCL_EVAL_GLOBAL);

      Tcl_DecrRefCount (com);
    }
};

class TkObserverHome_impl : virtual public DiningPhilosophers::CCM_ObserverHome {
public:
  TkObserverHome_impl ()
    {
      Tcl_FindExecutable (0);
    }

  Components::EnterpriseComponent_ptr create ()
    {
      Tcl_Interp * myinterp = Tcl_CreateInterp ();

      if (Tcl_Init (myinterp) != TCL_OK) {
	assert (0);
      }

      if (Tk_Init (myinterp) != TCL_OK) {
	assert (0);
      }

      Tcl_SetVar (myinterp, "maxphilosophers", "7", 0);
      
      if (Tcl_GlobalEval (myinterp, startscript) != TCL_OK) {
	assert (0);
      }
      
      return new TkObserver_impl (myinterp);
    }
};

/*
 * ----------------------------------------------------------------------
 * Entry points
 * ----------------------------------------------------------------------
 */


extern "C" {
  Components::HomeExecutorBase_ptr
  create_DiningPhilosophers_ObserverHome (CORBA::ORB_ptr orb)
  {
    CORBA::ValueFactoryBase_var f = new StatusInfoFactory;
    orb->register_value_factory ("IDL:DiningPhilosophers/StatusInfo:1.0", f);

    return new TkObserverHome_impl ();
  }
}
