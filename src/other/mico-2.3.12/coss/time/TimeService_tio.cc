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
// Implementation for interface TIO
//

//Constructor

TIO_impl::TIO_impl (TimeBase::TimeT a, TimeBase::TimeT b)
{  
   lower_boundV = a;
   upper_boundV = b;
}

// methods

TimeBase::IntervalT TIO_impl::time_interval()
{
   TimeBase::IntervalT retval;
   
   retval.lower_bound = lower_boundV;
   retval.upper_bound = upper_boundV;   

   return retval; 
}


CosTime::OverlapType
TIO_impl::spans( CosTime::UTO_ptr time, CosTime::TIO_out overlap )
{
   CosTime::OverlapType retval;
  
   //Calculate the overlap type and intervall
   long lower_boundW = time->time() - time->inaccuracy();
   long diffLow  = lower_boundV - lower_boundW;
   long upper_boundW = time->time() + time->inaccuracy();   
   long diffHigh = upper_boundV - upper_boundW;
   
   //cout << diffLow << " " << diffHigh << endl;
   TimeBase::TimeT interv_low  = 0;
   TimeBase::TimeT interv_high = 0;   
   
   if (diffLow <= 0  && diffHigh >= 0) {
      //OTContainer
      retval = CosTime::OTContainer;
      interv_low = lower_boundW;
      interv_high = upper_boundW;
   } else if (diffLow >=  0 && diffHigh <= 0) {
      //OTContained
      retval = CosTime::OTContained;
      interv_low = lower_boundV;
      interv_high = upper_boundV;
   } else {
      
      if ((lower_boundV > lower_boundW) && (lower_boundV < upper_boundW)) {
	 //OTOverlap
	 retval = CosTime::OTOverlap;
	 interv_low = lower_boundV;
	 interv_high = upper_boundW;
      } else if ((upper_boundV > lower_boundW) && (upper_boundV < upper_boundW)) {
	 //OTOverlap
	 retval = CosTime::OTOverlap;
	 interv_low = lower_boundW;
	 interv_high = upper_boundV;
      } else if ((lower_boundV > lower_boundW) && (lower_boundV > upper_boundW)) {
	 //OTNoOverlap
	 retval = CosTime::OTNoOverlap;
	 interv_low = upper_boundW;
	 interv_high = lower_boundV;
      } else {
	 //OTNoOverlap	   
	 retval = CosTime::OTNoOverlap;
	 interv_low = upper_boundV;
	 interv_high = lower_boundW;
      }
   } 
   
   TIO_impl * tioO = new TIO_impl(interv_low,interv_high);      	 	 	 	 
   overlap = tioO->_this();
   
   return retval; 
}


CosTime::OverlapType
TIO_impl::overlaps( CosTime::TIO_ptr interval, CosTime::TIO_out overlap )
{
   CosTime::OverlapType retval;

   //Calculate the overlap type and intervall
   long lower_boundW = interval->time_interval().lower_bound;
   long diffLow  = lower_boundV - lower_boundW;
   long upper_boundW = interval->time_interval().upper_bound;
   long diffHigh = upper_boundV - upper_boundW;
   
   TimeBase::TimeT interv_low  = 0;
   TimeBase::TimeT interv_high = 0;   
   
   if (diffLow <= 0  && diffHigh >= 0) {
      //OTContainer
      retval = CosTime::OTContainer;
      interv_low = lower_boundW;
      interv_high = upper_boundW;
   } else if (diffLow >=  0 && diffHigh <= 0) {
      //OTContained
      retval = CosTime::OTContained;
      interv_low = lower_boundV;
      interv_high = upper_boundV;
   } else {
      
      if ((lower_boundV > lower_boundW) && (lower_boundV < upper_boundW)) {
	 //OTOverlap
	 retval = CosTime::OTOverlap;
	 interv_low = lower_boundV;
	 interv_high = upper_boundW;
      } else if ((upper_boundV > lower_boundW) && (upper_boundV < upper_boundW)) {
	 //OTOverlap
	 retval = CosTime::OTOverlap;
	 interv_low = lower_boundW;
	 interv_high = upper_boundV;
      } else if ((lower_boundV > lower_boundW) && (lower_boundV > upper_boundW)) {
	 //OTNoOverlap	    
	 retval = CosTime::OTNoOverlap;
	 interv_low = upper_boundW;
	 interv_high = lower_boundV;
      } else {
	 //OTNoOverlap	    	    
	 retval = CosTime::OTNoOverlap;
	 interv_low = upper_boundV;
	 interv_high = lower_boundW;
      }
   } 
   
   TIO_impl * tioO = new TIO_impl(interv_low,interv_high);      	 	 	 	 
   overlap = tioO->_this();
   
   return retval; 
}


CosTime::UTO_ptr
TIO_impl::time()
{

   UTO_impl * utoAbs = new UTO_impl((upper_boundV+lower_boundV)/2,(upper_boundV-lower_boundV)/2 , 0);
   CosTime::UTO_ptr retval = utoAbs->_this();

  return retval; 
}

//Added 
void
TIO_impl::destroy()
{   
   PortableServer::POA_var poa  = this->_default_POA();
   PortableServer::ObjectId_var oid = poa->servant_to_id(this);
   poa->deactivate_object(oid.in());
   
   delete this;

}

