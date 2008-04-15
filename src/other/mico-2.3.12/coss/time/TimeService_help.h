//
//helper functions
//

//Difference from begin of gregors calender to UNIX  epoch in days,
//thanks to Laffoon, Mark
#define EPOCH2GREGOR  141427

//Length of a day in seconds, simple guess
#define DAYLENGTH 86400

//inaccuracy of the internal clock in seconds
#define INACCURACY 1

//use the secure time? Only if your system clock is very accurate!
//#define USESECURE 

#include "CosTime.h"

//get best approx of time
TimeBase::TimeT getTime();

//get inaccuracy
TimeBase::TimeT getInaccuracy();

//get timezone
TimeBase::TimeT getTimezone();

//convert TimeBase::TimeT to time_t
time_t timeT2epoch(TimeBase::TimeT);
