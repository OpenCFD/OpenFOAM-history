/* Dynamic loaded CORBA Modules
 *  Copyright (c) 2002 Sorin Mustaca
 *  For more information, visit the Personal Home Page at
 *  http://come.to/sorin-mustaca
 */

// myservice.cpp
#include "include.h"
#include "myservice.h"
using namespace std;

CMyService::CMyService(const char* name)
	:CNTService(name)
{
	m_iStartParam = 0;
	m_iIncParam = 1;
	m_iState = m_iStartParam;
}

BOOL CMyService::OnInit()
{
	// Read the registry parameters
    // Try opening the registry key:
    // HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\<AppName>\Parameters
    HKEY hkey;
	char szKey[1024];
	strcpy(szKey, "SYSTEM\\CurrentControlSet\\Services\\");
	strcat(szKey, m_szServiceName);
	strcat(szKey, "\\Parameters");
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		szKey,
		0,
		KEY_QUERY_VALUE,
		&hkey) == ERROR_SUCCESS) {
        // Yes we are installed
        DWORD dwType = 0;
        DWORD dwSize = sizeof(m_iStartParam);
        RegQueryValueEx(hkey,
			"Start",
			NULL,
			&dwType,
			(BYTE*)&m_iStartParam,
			&dwSize);
        dwSize = sizeof(m_iIncParam);
        RegQueryValueEx(hkey,
			"Inc",
			NULL,
			&dwType,
			(BYTE*)&m_iIncParam,
			&dwSize);
        RegCloseKey(hkey);
    }
	
	// Set the initial state
	m_iState = m_iStartParam;
	
	return TRUE;
}

bool CMyService::readCfg(std::string& path)
{
//reading registry path
	LONG    err;
    HKEY    hkey;
    DWORD    btype = REG_SZ, bsz = 260;
	trace(("readcfg:"));
	string  srv=string("SYSTEM\\CurrentControlSet\\Services\\")+string(m_szServiceName);
	err = RegOpenKeyEx(HKEY_LOCAL_MACHINE, srv.c_str(), 0, KEY_READ, &hkey);
    if (err != ERROR_SUCCESS)
	{
		trace(("Can not open service's registry:%s",srv.c_str()));
		return false;
	}

	char szCfgFile[256]="";
	bsz =256;
	btype=REG_SZ ;
	err=RegQueryValueEx(hkey, "Config", 0,&btype,(BYTE*)szCfgFile, &bsz);
	if (err != ERROR_SUCCESS)
	{
		RegCloseKey(hkey);
		return FALSE;
	}
					
	RegCloseKey(hkey);
	path=szCfgFile;
	return true;
}

int CMyService::start_console()
{
	
		string cfgpath;
		char   szFilePath[_MAX_PATH];
	    char 	params[256]="";
	    string s="Can not read configuration file :";
			
		if(readCfg(cfgpath)==false)
		{   
			return 0;
		}
		
		s+=cfgpath;
		
		FILE *f=fopen(cfgpath.c_str(),"rt");
		if (f==NULL)
		{	
			trace(("%s",s.c_str()));
			LogEvent(EVENTLOG_ERROR_TYPE,EVMSG_FAILEDINIT,s.c_str());
			return 0;
		}
		else
		{	
			fgets(params,256,f);
			if(params[strlen(params)-1]=='\n')
				params[strlen(params)-1]=0;

			fclose(f);
			trace(("read from cfg file : %s",params));
		}
		
		m_bIsRunning=false;
		string	 cmdln;
		
		//cmdln=s;
		cmdln+=params;
		trace(("command line is:%s",cmdln.c_str()));
		
		//	PROCESS_INFORMATION piProcInfo;  - is in base class, so that we can terminate the program at OnStop event
		
		STARTUPINFO siStartInfo; 
		
		// Set up members of the PROCESS_INFORMATION structure. 
		
		ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );
		
		// Set up members of the STARTUPINFO structure. 
		
		ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
		siStartInfo.cb = sizeof(STARTUPINFO); 

		trace(("Creating process "));
		
		if (CreateProcess(NULL,const_cast<char*>(cmdln.c_str()),NULL,NULL,FALSE,
						m_run?CREATE_NEW_CONSOLE:CREATE_NO_WINDOW,NULL,NULL,&siStartInfo,&piProcInfo) 
			)
		{
			trace(("Process spawned successfully !"));
		}
		else
		{
				trace(("Process can not be spawned ! Exiting !"));
				return 0;
		}
					
		m_bIsRunning = !m_run;//if we are in console mode, this program must exit
		return 1;

}

void CMyService::Run()
{		
		if(start_console()==0)
		return ;

		while (m_bIsRunning)
		{
			WaitForSingleObject(piProcInfo.hProcess,INFINITE);
		}
		
		trace(("Done !"));	
}

// Process user control requests
BOOL CMyService::OnUserControl(DWORD dwOpcode)
{
    switch (dwOpcode) {
    case SERVICE_CONTROL_USER + 0:
		
        // Save the current status in the registry
        SaveStatus();
        return TRUE;
		
    default:
        break;
    }
    return FALSE; // say not handled
}

// Save the current status in the registry
void CMyService::SaveStatus()
{
    // Try opening the registry key:
    // HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\<AppName>\...
    HKEY hkey = NULL;
	char szKey[1024];
	strcpy(szKey, "SYSTEM\\CurrentControlSet\\Services\\");
	strcat(szKey, m_szServiceName);
	strcat(szKey, "\\Status");
    DWORD dwDisp;
	DWORD dwErr;
    dwErr = RegCreateKeyEx(	HKEY_LOCAL_MACHINE,
		szKey,
		0,
		"",
		REG_OPTION_NON_VOLATILE,
		KEY_WRITE,
		NULL,
		&hkey,
		&dwDisp);
	if (dwErr != ERROR_SUCCESS) {
		trace(("Failed to create Status key (%lu)", dwErr));
		return;
	}	
	
    // Set the registry values
    RegSetValueEx(hkey,
		"Current",
		0,
		REG_DWORD,
		(BYTE*)&m_iState,
		sizeof(m_iState));
	
	
    // Finished with key
    RegCloseKey(hkey);
	
}
