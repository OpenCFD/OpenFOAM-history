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
#include <time.h>

//
// Implementation for interface UTO
//

//Constructor

UTO_impl::UTO_impl (TimeBase::TimeT a, TimeBase::InaccuracyT b, TimeBase::TdfT c)
{  
   timeV = a;
   inaccuracyV = b;
   tdfV = c;
}

//methods
TimeBase::TimeT UTO_impl::time()
{
  return timeV; 
}

TimeBase::InaccuracyT UTO_impl::inaccuracy()
{
  return inaccuracyV; 
}

TimeBase::TdfT UTO_impl::tdf()
{
  return tdfV; 
}

TimeBase::UtcT UTO_impl::utc_time()
{
   TimeBase::UtcT utcV;
   utcV.time = timeV;
   //OK, this is the first attempt
   utcV.inacclo = CORBA::UShort ((inaccuracyV <<32)>>32);
   utcV.inacchi = CORBA::UShort (inaccuracyV >> 32) & 0xFFFF;
   utcV.tdf = tdfV;
   
   return utcV; 
}


CosTime::UTO_ptr
UTO_impl::absolute_time()
{  

   TimeBase::TimeT atimeV = timeV + TimeBase::TimeT(getTime());
   TimeBase::TdfT  atdfV = getTimezone();
   TimeBase::TimeT ainaccurV = 10000000;
   ainaccurV *= INACCURACY;

   UTO_impl * utoAbs = new UTO_impl(atimeV, ainaccurV , atdfV);
   CosTime::UTO_ptr utoAbsP = utoAbs->_this();
   
   return utoAbsP; 
}


CosTime::TimeComparison
UTO_impl::compare_time(CosTime::ComparisonType comparison_type, CosTime::UTO_ptr uto )
{
   CosTime::TimeComparison retval;
   
   if (comparison_type == CosTime::IntervalC) {
      // compare two times including the error envelop

      long diffHigh = (timeV + inaccuracyV) - (uto->time() + uto->inaccuracy());
      long diffLow  = (timeV - inaccuracyV) - (uto->time() - uto->inaccuracy());
      //cout << diffHigh << " : " << diffLow << endl;
      if ((diffLow == 0) && (diffHigh == 0)) {
	 //TCEqualTo
	 retval = CosTime::TCEqualTo;
      } else if ((diffLow < 0) && (diffHigh < 0)) {
	 //TCLessThan
	 retval = CosTime::TCLessThan;
      } else if ((diffLow > 0) && (diffHigh > 0)) {
	 //TCGreaterThan
	 retval = CosTime::TCGreaterThan;
      } else  {
	 //TCIndetertimate
	 retval = CosTime::TCIndeterminate;
      }
      
   } else {
      // compare two base times
      
      long diff = timeV - uto->time();

      if (diff == 0 ) {
	 //TCEqualTo
	 retval = CosTime::TCEqualTo;
      } else if (diff< 0) {
	 //TCLessThan
	 retval = CosTime::TCLessThan;
      } else {
	 //TCGreaterThan
	 retval = CosTime::TCGreaterThan;
      }
   }

  return retval; 
}


CosTime::TIO_ptr
UTO_impl::time_to_interval( CosTime::UTO_ptr uto )
{

   TimeBase::TimeT interv_low, interv_high;
   
   if (timeV <= uto->time()) {
      interv_low = timeV;
      interv_high = uto->time();
   } else {
      interv_low = uto->time();
      interv_high = timeV;
   }
   
   TIO_impl * tio = new TIO_impl(interv_low,interv_high);      	 	 	 	 
   CosTime::TIO_ptr retval = tio->_this();

   return retval; 
}


CosTime::TIO_ptr
UTO_impl::interval()
{
   TimeBase::TimeT interv_low, interv_high;
   
   interv_low = timeV-inaccuracyV;
   interv_high = timeV+inaccuracyV;

   TIO_impl * tio = new TIO_impl(interv_low,interv_high);      	 	 	 	 
   CosTime::TIO_ptr retval = tio->_this();

   return retval; 
}


//Added 
void
UTO_impl::destroy()
{
   PortableServer::POA_var poa  = this->_default_POA();
   PortableServer::ObjectId_var oid = poa->servant_to_id(this);
   poa->deactivate_object(oid.in());
   
   delete this;
}
