/* Dynamic loaded CORBA Modules
 *  Copyright (c) 2002 Sorin Mustaca
 *
 *  For more information, visit the Personal Home Page at
 *  http://come.to/sorin-mustaca
 */

#ifndef _SERVICE_JOBS_H
#define _SERVICE_JOBS_H

enum service_actions
{ 
STOP=0,
START,
RESTART
};

void job(int what,const char *srv);//!< what=0 =stop  , 1=start 2=restart

#endif



