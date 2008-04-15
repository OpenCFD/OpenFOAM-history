/* Dynamic loaded CORBA Modules
 *  Copyright (c) 2002 Sorin Mustaca
 *
 *  For more information, visit the Personal Home Page at
 *  http://come.to/sorin-mustaca
 */
#ifndef __TRACE_H_
	#define __TRACE_H_


void end_trace();
void init_trace(int argc,char **argv);
	
#ifdef DCM_TRACE

class KILL_TRACE
{   
public:
	KILL_TRACE(){}
	~KILL_TRACE()
	{
		end_trace();	
	}
};

 void __show(char *pszFormat,...) ;
 void scope(const char* file,int line,const char* function);
 void unlock_trace();
 void lock_trace();

	#ifndef __FUNCTION__
		#define __FUNCTION__ "?"
	#endif

	#define trace(x)\
		 do{lock_trace();\
			scope(__FILE__,__LINE__,__FUNCTION__);\
			__show x;\
			unlock_trace();\
		}while(0);
#else//DCMTRACE
	#define trace(x) {;}
	
#endif//dcmtrace

	
#endif//__TRACE_H_     	
