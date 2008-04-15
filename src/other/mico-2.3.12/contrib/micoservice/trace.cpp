/* Dynamic loaded CORBA Modules
 *  Copyright (c) 2002 Sorin Mustaca
 *
 *  For more information, visit the Personal Home Page at
 *  http://come.to/sorin-mustaca
 */
#include <include.h>
#include "trace.h"

#ifdef DCM_TRACE

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "criticalsection.h"

static FILE *trace_file=NULL;
static char LOGFILE [1024];
static char trace_buffer[4096]="";
static CriticalSection trace_lock;
static KILL_TRACE killtrace;//to close the trace system

void init_trace(int argc,char **argv)
{
	time_t now=time(0);
	_snprintf(LOGFILE,1023,"%s.log",argv[0]);
	trace_file=fopen(LOGFILE,"wt");//not appending
	if (trace_file) 
	{
		fprintf(trace_file,"Log file for %s initialized at %s",argv[0],ctime(&now));
	}
	fflush(trace_file);
}

void end_trace()
{
	if (trace_file) 
	{   
		time_t now=time(0);
		fprintf(trace_file,"\nClosing trace at %s",ctime(&now));
		fclose(trace_file);
	}
}

void lock_trace()
{
	trace_lock.Enter();
}

void unlock_trace()
{
	trace_lock.Leave();
}

void scope(const char* file,int line,const char* function)
{
	if(function!=NULL)
		sprintf(trace_buffer, "%s:%d:[%s](%lu): ",file,line,function, GetCurrentThreadId());
	else
		sprintf(trace_buffer, "%s:%d(%lu): ",file,line, GetCurrentThreadId());
};

void __show(char *pszFormat,...) 
{	
	va_list arglist;
	va_start(arglist, pszFormat);
	_vsnprintf(&trace_buffer[strlen(trace_buffer)],4000,pszFormat, arglist);
	va_end(arglist);
	strcat(trace_buffer, "\n");
	
	if (trace_file)
	{
		fprintf(trace_file,"%s",trace_buffer);
		fflush(trace_file);
	}
	
	fprintf(stdout,"%s",trace_buffer);

#ifdef _WIN32	
	OutputDebugString(trace_buffer);
#endif
}


#else

void init_trace(int argc,char **argv) {}
void end_trace(){}

#endif