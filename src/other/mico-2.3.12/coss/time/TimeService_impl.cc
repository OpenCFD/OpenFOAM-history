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

#include "TimeService_impl.h"
#include "TimeService_help.h" 

// Implementation for interface TimeService

CosTime::UTO_ptr
TimeService_impl::universal_time()
{
   
   TimeBase::TimeT timeV = timeV + TimeBase::TimeT(getTime());
   TimeBase::TimeT inaccuracyV = 10000000;
   inaccuracyV *= INACCURACY;
   TimeBase::TdfT  tdfV = getTimezone();

   UTO_impl * utoAbs = new UTO_impl(timeV, inaccuracyV, tdfV);
   CosTime::UTO_ptr retval = utoAbs->_this();

   return retval; 
}


CosTime::UTO_ptr
TimeService_impl::secure_universal_time()
{
   CosTime::UTO_ptr retval;

#ifdef USESECURE
   

   TimeBase::TimeT timeV = timeV + TimeBase::TimeT(getTime());
   TimeBase::TimeT inaccuracyV = INACCURACY;
   TimeBase::TdfT  tdfV = getTimezone();

   UTO_impl * utoAbs = new UTO_impl(timeV, inaccuracyV, tdfV);
   retval = utoAbs->_this();

#else   

   mico_throw(CosTime::TimeUnavailable());
   
#endif
   
   return retval; 
}


CosTime::UTO_ptr
TimeService_impl::new_universal_time( TimeBase::TimeT a, TimeBase::InaccuracyT b, TimeBase::TdfT c )

{
   
   UTO_impl * utoAbs = new UTO_impl(a, b , c);
   CosTime::UTO_ptr retval = utoAbs->_this();

   return retval; 
}


CosTime::UTO_ptr
TimeService_impl::uto_from_utc( const TimeBase::UtcT& utc )
{
   TimeBase::TimeT a = utc.time;
   TimeBase::TimeT b = utc.inacclo + (utc.inacchi << 4);
   TimeBase::TdfT  c = utc.tdf;
   UTO_impl * utoAbs = new UTO_impl(a, b, c);
   CosTime::UTO_ptr retval = utoAbs->_this();

   return retval; 
}


CosTime::TIO_ptr
TimeService_impl::new_interval( TimeBase::TimeT lower, TimeBase::TimeT upper )
{
   
   TIO_impl * tioO = new TIO_impl(lower,upper);      	 	 	 	 
   CosTime::TIO_ptr retval = tioO->_this();

   return retval; 
}

