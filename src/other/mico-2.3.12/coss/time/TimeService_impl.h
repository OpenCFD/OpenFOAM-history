

#ifndef __TIMESERVICE_IMPL_H__
#define __TIMESERVICE_IMPL_H__

#include "CosTime.h"
#include <CORBA.h>

// Implementation for interface UTO
class UTO_impl : virtual public POA_CosTime::UTO
{
   
   static CORBA::Object orb;
   
 public:
   
   UTO_impl (TimeBase::TimeT, TimeBase::InaccuracyT, TimeBase::TdfT);
   
   TimeBase::TimeT time();
   TimeBase::InaccuracyT inaccuracy();
   TimeBase::TdfT tdf();
   TimeBase::UtcT utc_time();
   
   CosTime::UTO_ptr absolute_time();

   CosTime::TimeComparison compare_time( CosTime::ComparisonType comparison_type, CosTime::UTO_ptr uto );
   
    CosTime::TIO_ptr time_to_interval( CosTime::UTO_ptr uto );
   
   CosTime::TIO_ptr interval();
   void destroy();

   
 private:
   TimeBase::TimeT timeV;
   TimeBase::InaccuracyT inaccuracyV;
   TimeBase::TdfT tdfV;
   
};


// Implementation for interface TIO
class TIO_impl : virtual public POA_CosTime::TIO
{
   
   static CORBA::Object orb;
   
  public:
   
   TIO_impl (TimeBase::TimeT a, TimeBase::TimeT b);
   
   TimeBase::IntervalT time_interval();

   CosTime::OverlapType spans( CosTime::UTO_ptr time, CosTime::TIO_out overlap );

    CosTime::OverlapType overlaps( CosTime::TIO_ptr interval, CosTime::TIO_out overlap );

    CosTime::UTO_ptr time();
   
   void destroy();
   
 private:
   TimeBase::TimeT lower_boundV;
   TimeBase::TimeT upper_boundV;
};


// Implementation for interface TimeService
class TimeService_impl : virtual public POA_CosTime::TimeService
{
  public:

    CosTime::UTO_ptr universal_time();

    CosTime::UTO_ptr secure_universal_time();

    CosTime::UTO_ptr new_universal_time( TimeBase::TimeT time, TimeBase::InaccuracyT inaccuracy, TimeBase::TdfT tdf );

    CosTime::UTO_ptr uto_from_utc( const TimeBase::UtcT& utc );

    CosTime::TIO_ptr new_interval( TimeBase::TimeT lower, TimeBase::TimeT upper );
};


#endif
