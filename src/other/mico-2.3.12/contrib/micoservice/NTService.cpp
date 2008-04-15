/* Dynamic loaded CORBA Modules
 *  Copyright (c) 2002 Sorin Mustaca
 *
 *  For more information, visit the Personal Home Page at
 *  http://come.to/sorin-mustaca
 */
#include "include.h"
#include <windows.h>
#include <stdio.h>
#include "NTService.h"
#include "trace.h"
#include "util.h"
#include "service_jobs.h"
using namespace std;

// static variables
CNTService* CNTService::m_pThis = NULL;
BOOL CNTService::m_bIsRunning=TRUE;

CNTService::CNTService(const char* szServiceName)
{
    // copy the address of the current object so we can access it from
    // the static member callback functions. 
    // WARNING: This limits the application to only one CNTService object. 
    m_pThis = this;
    
    // Set the default service name and version
    strncpy(m_szServiceName, szServiceName, sizeof(m_szServiceName)-1);
    m_iMajorVersion = 1;
    m_iMinorVersion = 0;
    m_hEventSource = NULL;

    // set up the initial service status 
    m_hServiceStatus = NULL;
    m_Status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    m_Status.dwCurrentState = SERVICE_STOPPED;
    m_Status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    m_Status.dwWin32ExitCode = 0;
    m_Status.dwServiceSpecificExitCode = 0;
    m_Status.dwCheckPoint = 0;
    m_Status.dwWaitHint = 0;
    m_bIsRunning = FALSE;
	m_run =false;
}

CNTService::~CNTService()
{
    if (m_hEventSource) {
        ::DeregisterEventSource(m_hEventSource);
    }
}

////////////////////////////////////////////////////////////////////////////////////////
// Default command line argument parsing

// Returns TRUE if it found an arg it recognised, FALSE if not
// Note: processing some arguments causes output to stdout to be generated.
BOOL CNTService::ParseStandardArgs(int argc, char* argv[])
{
	trace(("argc=%d",argc));
    // See if we have any command line args we recognise
    if (argc < 2) 
		return FALSE;
	int k=2;

	 //argv[1] must ALWAYS be the service name

	if (argc==3 && argv[k][0]=='-' && argv[k][1]=='-')
	{
		argv[k]+=2;
		if (_stricmp(argv[k], "start") == 0)
		{
			job(START,m_szServiceName);
			return TRUE;
		}
		else if (_stricmp(argv[k], "stop") == 0)
			{
				job(STOP,m_szServiceName);
				return TRUE;
			}
			else if (_stricmp(argv[k], "restart") == 0)
				 {
					job(RESTART,m_szServiceName);
					return TRUE;
				}
				else if (_stricmp(argv[k], "run") == 0)
					 {
						trace(("Entering console mode"));
						m_run = true;//running as console
					 }
	}
	else if (argc==4 &&_stricmp(argv[k], "-i") == 0) 
	{
	    string name=argv[1];
		
		string cfgfile=argv[3];
        trace(("Request to install."));

        if (IsInstalled(name.c_str())) 
		{
            printf("%s is already installed\n", name.c_str());
        } 
		else 
		{
            // Try and install the copy that's running
            if (Install(cfgfile.c_str(),name.c_str())) 
			{
                printf("%s installed with configuration file %s\n", name.c_str(),cfgfile.c_str());
            } 
			else 
			{
                printf("%s failed to install. Error %d\n", name.c_str(), GetLastError());
            }
        }
        
		return TRUE; // say we processed the argument

    } 
	else if (argc==3 && _stricmp(argv[k], "-u") == 0) 
	{
		string name=argv[1];
		        
        trace(("Request to uninstall."));
        if (!IsInstalled(name.c_str())) 
		{
            printf("%s is not installed\n", name.c_str());
        } 
		else 
		{
            // Try and remove the copy that's installed
            if (Uninstall(name.c_str())) 
			{
                // Get the executable file path
                char szFilePath[_MAX_PATH];
                ::GetModuleFileName(NULL, szFilePath, sizeof(szFilePath));
                printf("%s removed. (You must delete the file (%s) yourself.)\n",
                       name.c_str(), szFilePath);
            } 
			else 
			{
                printf("Could not remove %s. Error %d\n", name.c_str(), GetLastError());
            }
        }
        return TRUE; // say we processed the argument
    
    }
         
    // Don't recognise the args
    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////
// Install/uninstall routines

// Test if the service is currently installed
BOOL CNTService::IsInstalled(const char* szServiceName)
{
    BOOL bResult = FALSE;

    // Open the Service Control Manager
    SC_HANDLE hSCM = ::OpenSCManager(NULL, // local machine
                                     NULL, // ServicesActive database
                                     SC_MANAGER_ALL_ACCESS); // full access
    if (hSCM) 
	{

        // Try to open the service
        SC_HANDLE hService = ::OpenService(hSCM,
                                           szServiceName,
                                           SERVICE_QUERY_CONFIG);
        if (hService) 
		{
            bResult = TRUE;
            ::CloseServiceHandle(hService);
        }

        ::CloseServiceHandle(hSCM);
    }
    
    return bResult;
}

BOOL CNTService::Install(const char* szCfgFile,const char* szServiceName)
{
    // Open the Service Control Manager
    SC_HANDLE hSCM = ::OpenSCManager(NULL, // local machine
                                     NULL, // ServicesActive database
                                     SC_MANAGER_ALL_ACCESS); // full access
    if (!hSCM) return FALSE;

    // Get the executable file path
    char szFilePath[_MAX_PATH];
    ::GetModuleFileName(NULL, szFilePath, sizeof(szFilePath));
	strcat(szFilePath," ");strcat(szFilePath,szServiceName);
	string name="MICO Service(";
	name+=szServiceName;
	name+=")";
		
	trace(("Creating service:%s with displayname:%s",szServiceName,name.c_str()));
    // Create the service
	
    SC_HANDLE hService = ::CreateService(hSCM,
                                         szServiceName,
                                         name.c_str(),  
                                         SERVICE_ALL_ACCESS,
                                         SERVICE_WIN32_OWN_PROCESS,
                                         SERVICE_AUTO_START,        // start condition
                                         SERVICE_ERROR_NORMAL,
                                         szFilePath,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL);
    if (!hService) {
        ::CloseServiceHandle(hSCM);
        return FALSE;
    }

    // make registry entries to support logging messages
    // Add the source name as a subkey under the Application
    // key in the EventLog service portion of the registry.
    char szKey[256];
    HKEY hKey = NULL;
    strcpy(szKey, "SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\");
    strcat(szKey, szServiceName);
    if (::RegCreateKey(HKEY_LOCAL_MACHINE, szKey, &hKey) != ERROR_SUCCESS) {
        ::CloseServiceHandle(hService);
        ::CloseServiceHandle(hSCM);
        return FALSE;
    }

    // Add the Event ID message-file name to the 'EventMessageFile' subkey.
    ::RegSetValueEx(hKey,
                    "EventMessageFile",
                    0,
                    REG_EXPAND_SZ, 
                    (CONST BYTE*)szFilePath,
                    strlen(szFilePath) + 1);     

    // Set the supported types flags.
    DWORD dwData = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE;
    ::RegSetValueEx(hKey,
                    "TypesSupported",
                    0,
                    REG_DWORD,
                    (CONST BYTE*)&dwData,
                     sizeof(DWORD));
    ::RegCloseKey(hKey);
    // tidy up
    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);
    
    LogEvent(EVENTLOG_INFORMATION_TYPE, EVMSG_INSTALLED, szServiceName);
    //writing the cfg file
	trace(("writing the cfg filename in registry:%s",szCfgFile));
	LONG    err;
    HKEY    hkey;
    DWORD    btype = REG_SZ, bsz = 260;
	err = RegOpenKeyEx(HKEY_LOCAL_MACHINE, ( string("SYSTEM\\CurrentControlSet\\Services\\")+string(szServiceName)).c_str(), 0, KEY_ALL_ACCESS, &hkey);
    if (err != ERROR_SUCCESS)
	{
		trace(("Can not open service's registry"));
		Uninstall(szServiceName);
		return FALSE;
	}

	bsz =strlen(szCfgFile) ;
	btype=REG_SZ ;
	err=RegSetValueEx(hkey, "Config", 0,btype,(BYTE*)szCfgFile, bsz);
	if (err != ERROR_SUCCESS)
	{
		Uninstall(szServiceName);
		RegCloseKey(hkey);
		return FALSE;
	}
				
	RegCloseKey(hkey);
	
    return TRUE;
}

BOOL CNTService::Uninstall(const char* szServiceName)
{
    // Open the Service Control Manager
    SC_HANDLE hSCM = ::OpenSCManager(NULL, // local machine
                                     NULL, // ServicesActive database
                                     SC_MANAGER_ALL_ACCESS); // full access
    if (!hSCM) return FALSE;

    BOOL bResult = FALSE;
    SC_HANDLE hService = ::OpenService(hSCM,
                                       szServiceName,
                                       DELETE);
    if (hService) {
        if (::DeleteService(hService)) {
            LogEvent(EVENTLOG_INFORMATION_TYPE, EVMSG_REMOVED, szServiceName);
            bResult = TRUE;
        } else {
            LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_NOTREMOVED, szServiceName);
        }
        ::CloseServiceHandle(hService);
    }
    
    ::CloseServiceHandle(hSCM);
    return bResult;
}

///////////////////////////////////////////////////////////////////////////////////////
// Logging functions

// This function makes an entry into the application event log
void CNTService::LogEvent(WORD wType, DWORD dwID,
                          const char* pszS1,
                          const char* pszS2,
                          const char* pszS3)
{
    const char* ps[3];
    ps[0] = pszS1;
    ps[1] = pszS2;
    ps[2] = pszS3;

    int iStr = 0;
    for (int i = 0; i < 3; i++) {
        if (ps[i] != NULL) iStr++;
    }
        
    // Check the event source has been registered and if
    // not then register it now
    if (!m_hEventSource) {
        m_hEventSource = ::RegisterEventSource(NULL,  // local machine
                                               m_szServiceName); // source name
    }

    if (m_hEventSource) {
        ::ReportEvent(m_hEventSource,
                      wType,
                      0,
                      dwID,
                      NULL, // sid
                      iStr,
                      0,
                      ps,
                      NULL);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Service startup and registration

BOOL CNTService::StartService()
{
    SERVICE_TABLE_ENTRY st[] = {
        {m_szServiceName, ServiceMain},
        {NULL, NULL}
    };

    BOOL b = ::StartServiceCtrlDispatcher(st);
    return b;
}

// static member function (callback)
void CNTService::ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
    // Get a pointer to the C++ object
    CNTService* pService = m_pThis;
    
    // Register the control request handler
    pService->m_Status.dwCurrentState = SERVICE_START_PENDING;
    pService->m_hServiceStatus = RegisterServiceCtrlHandler(pService->m_szServiceName,
                                                           Handler);
    if (pService->m_hServiceStatus == NULL) {
        pService->LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_CTRLHANDLERNOTINSTALLED);
        return;
    }

    // Start the initialisation
    if (pService->Initialize()) {

        // Do the real work. 
        // When the Run function returns, the service has stopped.
        pService->m_bIsRunning = TRUE;
        pService->m_Status.dwWin32ExitCode = 0;
        pService->m_Status.dwCheckPoint = 0;
        pService->m_Status.dwWaitHint = 0;
        pService->Run();
    }

    // Tell the service manager we are stopped
    pService->SetStatus(SERVICE_STOPPED);

}

///////////////////////////////////////////////////////////////////////////////////////////
// status functions

void CNTService::SetStatus(DWORD dwState)
{
    m_Status.dwCurrentState = dwState;
    ::SetServiceStatus(m_hServiceStatus, &m_Status);
}

///////////////////////////////////////////////////////////////////////////////////////////
// Service initialization

BOOL CNTService::Initialize()
{

    // Start the initialization
    SetStatus(SERVICE_START_PENDING);
    
    // Perform the actual initialization
    BOOL bResult = OnInit(); 
    
    // Set final state
    m_Status.dwWin32ExitCode = GetLastError();
    m_Status.dwCheckPoint = 0;
    m_Status.dwWaitHint = 0;
    if (!bResult) {
        LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_FAILEDINIT);
        SetStatus(SERVICE_STOPPED);
        return FALSE;    
    }
    
    LogEvent(EVENTLOG_INFORMATION_TYPE, EVMSG_STARTED);
    SetStatus(SERVICE_RUNNING);

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// main function to do the real work of the service

// This function performs the main work of the service. 
// When this function returns the service has stopped.
void CNTService::Run()
{

    while (m_bIsRunning) {
        Sleep(5000);
    }

    // nothing more to do
}

//////////////////////////////////////////////////////////////////////////////////////
// Control request handlers

// static member function (callback) to handle commands from the
// service control manager
void CNTService::Handler(DWORD dwOpcode)
{
    // Get a pointer to the object
    CNTService* pService = m_pThis;
    
    switch (dwOpcode) {
    case SERVICE_CONTROL_STOP: // 1
        pService->SetStatus(SERVICE_STOP_PENDING);
        pService->OnStop();
        pService->m_bIsRunning = FALSE;
        pService->LogEvent(EVENTLOG_INFORMATION_TYPE, EVMSG_STOPPED);
        break;

    case SERVICE_CONTROL_PAUSE: // 2
        pService->OnPause();
        break;

    case SERVICE_CONTROL_CONTINUE: // 3
        pService->OnContinue();
        break;

    case SERVICE_CONTROL_INTERROGATE: // 4
        pService->OnInterrogate();
        break;

    case SERVICE_CONTROL_SHUTDOWN: // 5
        pService->OnShutdown();
        break;

    default:
        if (dwOpcode >= SERVICE_CONTROL_USER) {
            if (!pService->OnUserControl(dwOpcode)) {
                pService->LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_BADREQUEST);
            }
        } else {
            pService->LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_BADREQUEST);
        }
        break;
    }

    // Report current status
    ::SetServiceStatus(pService->m_hServiceStatus, &pService->m_Status);
}
        
// Called when the service is first initialized
BOOL CNTService::OnInit()
{
	return TRUE;
}

// Called when the service control manager wants to stop the service
void CNTService::OnStop()
{
	if (TerminateProcess(piProcInfo.hProcess,0)==0)
	{
		error_string();
	}
	else 
		trace(("Process  terminated successfully !"));
}

// called when the service is interrogated
void CNTService::OnInterrogate()
{
}

// called when the service is paused
void CNTService::OnPause()
{
}

// called when the service is continued
void CNTService::OnContinue()
{
}

// called when the service is shut down
void CNTService::OnShutdown()
{
}

// called when the service gets a user control message
BOOL CNTService::OnUserControl(DWORD dwOpcode)
{
    return FALSE; // say not handled
}


