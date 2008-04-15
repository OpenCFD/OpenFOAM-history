//
// Corba TimeService (according to basic specs)
//
// Copyright (C) 1999 C. Zerbst
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// Send comments and/or bug reports to:
//                zerbst@tu-harburg.de
//

#include <CORBA-SMALL.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif
#include "TimeService_help.h"
#include "CosTime.h"
#include <coss/CosNaming.h>
#include <mico/util.h>
#include <time.h>
#include <sys/timeb.h>

//
//Helper functions, mainly wrappers around the time function
//
//
TimeBase::TimeT getTime() 
{
   
   //obtain actual time
   time_t etime;
   time( &etime);
   TimeBase::TimeT timeV = etime;
   //Offset from Unix Epoch to 15 October 1582
   TimeBase::TimeT tmp = EPOCH2GREGOR;
   tmp *= DAYLENGTH;
   timeV += tmp;
   //in 100 nano second steps
   timeV *= 1000000;
   
   return timeV;
}

//resolution is in miliseconds
TimeBase::TimeT getInaccuracy() 
{
   TimeBase::TimeT retval =1000000*INACCURACY;
   return retval;
}

TimeBase::TimeT getTimezone()
{
#if defined(HAVE_FTIME)
   //obtain timezone
   timeb  timestruct;
   ftime(&timestruct);
   
   TimeBase::TdfT displacementV = timestruct.timezone;
#elif defined(HAVE_GETTIMEOFDAY_TWOARGS)
   struct timeval tv;
   struct timezone tz;
   int r = gettimeofday (&tv, &tz);
   assert (r == 0);
   TimeBase::TdfT displacementV = tz.tz_minuteswest;
#else
   // where do we get the timezone from?
   assert (0);
#endif
   
   return displacementV;
}

//convert TimeBase::TimeT to time_t
time_t timeT2epoch(TimeBase::TimeT timeV) 
{
   time_t etime;

   //in 100 nano second steps
   timeV /= 1000000;
   //Offset from Unix Epoch to 15 October 1582
   TimeBase::TimeT tmp = EPOCH2GREGOR;
   tmp *= DAYLENGTH;
   timeV -= tmp;
   etime = timeV;

   return etime;
}
   
   


