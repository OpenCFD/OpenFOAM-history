/* Dynamic loaded CORBA Modules
 *  Copyright (c) 2002 Sorin Mustaca
 *
 *  For more information, visit the Personal Home Page at
 *  http://come.to/sorin-mustaca
 */

// NTService.cpp
// 
// This is the main program file containing the entry point.
#include "include.h"
#include "NTServApp.h"
#include "myservice.h"
#include "trace.h"

int main(int argc, char* argv[])
{
    // Create the service object
	init_trace(argc,argv);
    std::string name="MICO Service";
	
	if(argc>=2)
	{   
		name+="(";
		name+=argv[1];
		name+=")";
	}
	
	trace(("Service name:%s",name.c_str()));
	CMyService MyService(argv[1]);
    
    // Parse for standard arguments (install, uninstall etc.)
    if (!MyService.ParseStandardArgs(argc, argv)) 
	{
        if(MyService.m_run)//trying to start console
		{
			MyService.OnInit();
			trace(("Entering in console mode"));
			if(MyService.start_console())
				return 0;
		}
		else
			MyService.StartService();
	}

    // When we get here, the service has been stopped
    return MyService.m_Status.dwWin32ExitCode;
}
