/*
 * Copyright (c) 2001-2002 The Trustees of Indiana University.  
 *                         All rights reserved.
 * Copyright (c) 1998-2001 University of Notre Dame. 
 *                         All rights reserved.
 * Copyright (c) 1994-1998 The Ohio State University.  
 *                         All rights reserved.
 * 
 * This file is part of the LAM/MPI software package.  For license
 * information, see the LICENSE file in the top level directory of the
 * LAM/MPI source distribution.
 * 
 * $HEADER$
 *
 *	$Id: events.h,v 6.12 2002/10/09 20:57:23 brbarret Exp $
 *
 *	Function:	- system events
 */

#ifndef _LAM_EVENTS_H
#define _LAM_EVENTS_H

/* These all used to be of the form 0x800000nn.  This created a
   problem on some 64 bit architectures where 0x80... wasn't
   necessarily a negative number.  So they were changed to negative
   numbers, outright (e.g., (-7)).  However, the 0x8000 numbers may
   not have intended to be negative numbers in the first place (i.e.,
   a misunderstanding on my part) -- just high valued positive numbers
   (i.e., higher than the range of (pid_t) so as not to cause clashes
   with programs that used getpid() as the event number).

   But events are signed, so they were effectively negative numbers on
   32 bit systems, and therefore caused potential clashes with
   -getpid() and the system-level events (-getpid() is used as the
   event in many places in LAM).  For example, Solaris 7/8 allows PIDs
   to be as low as 4, which could clash with EVBOOTD.

   Below talks about this problem in a little more detail.

   ------

   We've discovered an ancillary problem -- since (-getpid()) is
   frequently used as the *_event field for nsends and the like,
   processes with really low PID's will get mixed up with these
   values.  End result: things like mpirun can hang (e.g., if mpirun
   has a PID of 8, messages that should have gone to mpirun went to
   the loadd instead).

   A simple solution would seem to be to use (getpid()) everywhere.
   However, Nick pointed out (msg01574.php on the Llamas mail archive)
   that historically, negative numbers were used for system events,
   and positive numbers were used for user events.  Granted, there are
   probably precious few programs out there that are using LAM calls,
   but we have seen some traffic about it on the list recently (late
   2000 / early 2001).

   Raja suggested (msg01576.php on the Llamas mail archive) that we
   redefine this a little: negative events are still reserved for
   system events, but *also* reserve 0x40000000 and above for system
   events.  This prevents us from ever clashing with internal services
   (because we'll make all the constants below be 0x40000000 and
   above), and [mostly] preserves the positive events for user events.

   Granted, this *can* break some user LAM (i.e., most likely non-MPI)
   programs out there.  Particularly for people who are using getpid()
   for the event on architectures where the PID can be very large --
   larger than 0x40000000 (e.g., AIX 4.3.3, IRIX 6.5).  With some
   testing, we determined the following (as of these versions -- no
   guarantee for future versions):

   OS             Min PID   Max PID
   Solaris 7/8    4         29999
   Linux 2.2.x    300       32767
   IRIX 6.5       ? ***     MAX_INT
   AIX 4.3.3      ? ***     MAX_INT
   OpenBSD        6         32767

   Nick also suggested having an additional flag that specifies
   whether it is a system event or a user event, and avoiding this
   whole mess.  This is actually a bigger change than it sounds --
   we'd have to go set this flag everywhere in LAM that we use
   nsend/nrecv (or any of nsend/nrecv's friends).  This is a lot of
   places, but it is a clean solution and doesn't have the
   restrictions that are listed above.

   More to the point, if I were a user, I don't know how I would
   specify a unique (and dynamic) event other than the PID unless some
   kind of event server were also used (which would be a real pain).
   But then again, as Raja said, "if the LAM API compatibility breaks
   in the forest and no app is there to notice it, did it really
   break?"
 */

#define LAM_BASE_EVENT      0x40000000

#ifndef	NOTEVENT
#define	NOTEVENT	(LAM_BASE_EVENT + 2)
#endif

/*
 * server events
 */
#define EVROUTER	(LAM_BASE_EVENT + 3)
#define EVBOOTD		(LAM_BASE_EVENT + 4)
#define EVDLOINIT	(LAM_BASE_EVENT + 5)
#define EVDLI		(LAM_BASE_EVENT + 6)
#define EVFILED		(LAM_BASE_EVENT + 7)
#define EVLOADD		(LAM_BASE_EVENT + 8)
#define EVBFORWARD	(LAM_BASE_EVENT + 9)
#define EVKENYAD	(LAM_BASE_EVENT + 10)
#define EVIOD		(LAM_BASE_EVENT + 11)
#define EVECHOD		(LAM_BASE_EVENT + 12)
#define EVBUFFERD	(LAM_BASE_EVENT + 13)
#define EVFLATD	        (LAM_BASE_EVENT + 14)
#define EVTRACED	(LAM_BASE_EVENT + 15)
#define EVHALTD		(LAM_BASE_EVENT + 16)

/*
 * special local layer events
 */
#define EVPAUSE		(LAM_BASE_EVENT + 17)
#define EVSTOP		(LAM_BASE_EVENT + 18)
#define EVDL0		(LAM_BASE_EVENT + 19)

/*
 * optional server events
 */
#define EVMTVD          (LAM_BASE_EVENT + 20)  /* This is required by XMTV */
#define EVLEDD          (LAM_BASE_EVENT + 21)  /* This is required by XLED */

/*
 * optional tools
 */
#define EVIPCDIAG	(LAM_BASE_EVENT + 22)
#define EVXMPI          (LAM_BASE_EVENT + 23)  /* Also required by XMTV */

/*
 * New pseudo-daemon, but don't want to displace all the old events 
 */
#define EVVERSIOND      (LAM_BASE_EVENT + 24)

#endif /* _LAM_EVENTS_H */
