/* Dynamic loaded CORBA Modules
 *  Copyright (c) 2002 Sorin Mustaca
 *
 *  For more information, visit the Personal Home Page at
 *  http://come.to/sorin-mustaca
 */

// myservice.h

#include "ntservice.h"
#include "NTServApp.h"
#include <string>

class CMyService 
	: public CNTService
{
public:
	CMyService(const char* name);
	virtual BOOL OnInit();
    virtual void Run();
    virtual BOOL OnUserControl(DWORD dwOpcode);
	bool readCfg(std::string& path);
    void SaveStatus();

	// Control parameters
	int m_iStartParam;
	int m_iIncParam;
	int start_console();
	// Current state
	int m_iState;
};
