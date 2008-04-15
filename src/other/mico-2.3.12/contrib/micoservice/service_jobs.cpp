/* Dynamic loaded CORBA Modules
 *  Copyright (c) 2002 Sorin Mustaca
 *
 *  For more information, visit the Personal Home Page at
 *  http://come.to/sorin-mustaca
 */
#include <include.h>
#include "service_jobs.h"
#include "util.h"

void job(int what,const char *srv)//!< what=0 =stop  , 1=start
{
	int running;
    char m_szServiceName[255];
	strcpy(m_szServiceName,srv);
	
	SetLastError(0);
	// Open the Service Control Manager
    SC_HANDLE hSCM = ::OpenSCManager(NULL, // local machine
		NULL, // ServicesActive database
		SC_MANAGER_ALL_ACCESS); // full access
    if (hSCM) 
	{
		
        // Try to open the service
        SC_HANDLE hService = ::OpenService(hSCM,
			m_szServiceName,
			SERVICE_ALL_ACCESS);
		SERVICE_STATUS ssStatus; 
        if (hService) 
		{	
			trace(("Modifying state of service %s",m_szServiceName));
    
			
			if (what==START) 
			{	
				trace(("starting service"));
				if (StartService(hService,0,NULL) == 0)
				{
					trace(("service error: not started"));
				}
			}
			else
				if (what==STOP)  
				{
					trace(("stopping service"));
					::ControlService(hService,SERVICE_CONTROL_STOP,&ssStatus);
				}
				
				 else
					 if (what == RESTART)
					 {
							trace(("restarting"));
							job(RESTART,srv);
							return;
					 }

		} else goto _exit_;
			
				DWORD dwOldCheckPoint; 
				DWORD dwStartTickCount;
				DWORD dwWaitTime;
				DWORD dwStatus;
				
				
				if (!QueryServiceStatus( 
					hService,   // handle to service 
					&ssStatus) )  // address of status information structure
				{
					trace(("QueryServiceStatus")); 
				}
				
				// Save the tick count and initial checkpoint.
				
				dwStartTickCount = GetTickCount();
				dwOldCheckPoint = ssStatus.dwCheckPoint;
				
				running =1;
				
			//	if (what == 1)
			//		running=(ssStatus.dwCurrentState == SERVICE_START_PENDING);
			//	else
			//		running=(ssStatus.dwCurrentState == SERVICE_STOP_PENDING);

				

				while (running) 
				{ 
					
				if (what == 1)
					running=(ssStatus.dwCurrentState == SERVICE_START_PENDING);
				else
					running=(ssStatus.dwCurrentState == SERVICE_STOP_PENDING);

					// Do not wait longer than the wait hint. A good interval is 
					// one tenth the wait hint, but no less than 1 second and no 
					// more than 10 seconds. 
					
					dwWaitTime = ssStatus.dwWaitHint / 10;
					
					if( dwWaitTime < 1000 )
						dwWaitTime = 1000;
					else 
						if ( dwWaitTime > 10000 )
							dwWaitTime = 10000;
					
					Sleep( dwWaitTime );
					
					// Check the status again. 
					
					if (!QueryServiceStatus( 
						hService,   // handle to service 
						&ssStatus) )  // address of structure
						break; 
					
					if ( ssStatus.dwCheckPoint > dwOldCheckPoint )
					{
						// The service is making progress.
						
						dwStartTickCount = GetTickCount();
						dwOldCheckPoint = ssStatus.dwCheckPoint;
					}
					else
					{
						if(GetTickCount()-dwStartTickCount > ssStatus.dwWaitHint)
						{
							// No progress made within the wait hint
							break;
						}
					}
				} 
				
				if (ssStatus.dwCurrentState == SERVICE_RUNNING) 
				{
					printf("StartService SUCCESS.\n"); 
					dwStatus = NO_ERROR;
				}
				else 
				{ 
					printf("\nService not started. \n");
					/*printf("  Current State: %d\n", ssStatus.dwCurrentState); 
					printf("  Exit Code: %d\n", ssStatus.dwWin32ExitCode); 
					printf("  Service Specific Exit Code: %d\n", 
						ssStatus.dwServiceSpecificExitCode); 
					printf("  Check Point: %d\n", ssStatus.dwCheckPoint); 
					printf("  Wait Hint: %d\n", ssStatus.dwWaitHint); 
					*/
					dwStatus = GetLastError();
				} 
				
				::CloseServiceHandle(hService);
		
		
	_exit_:
		::CloseServiceHandle(hSCM);
	}
}
