/* Dynamic loaded CORBA Modules
 *  Copyright (c) 2002 Sorin Mustaca
 *
 *  For more information, visit the Personal Home Page at
 *  http://come.to/sorin-mustaca
 */

// ntservice.h
//
// Definitions for CNTService
//

#ifndef _NTSERVICE_H_
#define _NTSERVICE_H_

#include "ntservmsg.h" // Event message ids

#define SERVICE_CONTROL_USER 128

class CNTService
{
public:
    CNTService(const char* szServiceName);
    virtual ~CNTService();
    BOOL ParseStandardArgs(int argc, char* argv[]);
    BOOL IsInstalled(const char* szServiceName);
    BOOL Install(const char* cfgfile,const char* szServiceName);
    BOOL Uninstall(const char* szServiceName);
    void LogEvent(WORD wType, DWORD dwID,
                  const char* pszS1 = NULL,
                  const char* pszS2 = NULL,
                  const char* pszS3 = NULL);
    BOOL StartService();
    void SetStatus(DWORD dwState);
    BOOL Initialize();
    virtual void Run();
	virtual BOOL OnInit();
    virtual void OnStop();
    virtual void OnInterrogate();
    virtual void OnPause();
    virtual void OnContinue();
    virtual void OnShutdown();
    virtual BOOL OnUserControl(DWORD dwOpcode);
    
    
    // static member functions
    static void WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
    static void WINAPI Handler(DWORD dwOpcode);

    // data members
    char m_szServiceName[64];
    int m_iMajorVersion;
    int m_iMinorVersion;
    SERVICE_STATUS_HANDLE m_hServiceStatus;
    SERVICE_STATUS m_Status;
    static BOOL m_bIsRunning;

    // static data
    static CNTService* m_pThis; // nasty hack to get object ptr
	PROCESS_INFORMATION piProcInfo; 
	bool		m_run;
private:
    HANDLE m_hEventSource;
};

#endif // _NTSERVICE_H_
