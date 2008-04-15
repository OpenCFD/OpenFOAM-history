/*
 *  MICO --- a free CORBA implementation
 *  Copyright (C) 1997-98 Kay Roemer & Arno Puder
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  Send comments and/or bug reports to:
 *                 mico@informatik.uni-frankfurt.de
 */


#include <afxwin.h>
#include <afxext.h>
#include <afxcmn.h>
#include <afxsock.h>
#include <stdlib.h>

#include "server.h"


UINT main_Thread( LPVOID pParam );


/*
 * Account implementation
 */

class Account_impl : virtual public Account_skel {
private:
  CORBA::Long _current_balance;
  CServerDlg *_dlg;

  void updateBalance()
    {
      _dlg->updateBalance( _current_balance );
    }

public:
  Account_impl ( CServerDlg *dlg )
    : Account_skel ()
    {
      _current_balance = 0;
      _dlg = dlg;
      updateBalance();
    }

  void deposit( CORBA::ULong amount )
    {
      _current_balance += amount;
      updateBalance();
    }
  
  void withdraw( CORBA::ULong amount )
    {
      _current_balance -= amount;
      updateBalance();
    }
  
  CORBA::Long balance()
    {
      return _current_balance;
    }

};



CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
{
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_CURR_BALANCE, m_curr_balance);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
END_MESSAGE_MAP()


BOOL CServerDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  AfxBeginThread( main_Thread, this );

  return TRUE;
}

void CServerDlg::updateBalance( CORBA::Long balance )
{
  char buf[ 50 ];
  sprintf( buf, "%d", balance );
  m_curr_balance.SetWindowText( buf );
}


BEGIN_MESSAGE_MAP(CServerApp, CWinApp)
END_MESSAGE_MAP()


CServerApp::CServerApp()
{
}

BOOL CServerApp::InitInstance()
{
  if (!AfxSocketInit())	{
    AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
    return FALSE;
  }

#ifdef _AFXDLL
  Enable3dControls();
#else
  Enable3dControlsStatic();
#endif

  CServerDlg dlg;
  m_pMainWnd = &dlg;
  dlg.DoModal();

  return FALSE;
}


UINT main_Thread( LPVOID pParam )
{
  int argc = 0;
  static char *argv[] = { "main_Thread", "-ORBIIOPAddr",
			  "inet:127.0.0.1:5555",
			  NULL };
  while( argv[ argc ] )
    argc++;

  CServerDlg *dlg = (CServerDlg*) pParam;

  CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );
  if( CORBA::is_nil( orb ) ) {
    AfxMessageBox( IDP_ORB_INIT_FAILED );
    return FALSE;
  }

  CORBA::BOA_var boa = orb->BOA_init( argc, argv, "mico-local-boa" );
  if( CORBA::is_nil( boa ) ) {
    AfxMessageBox( IDP_ORB_INIT_FAILED );
    return FALSE;
  }

  Account_impl* server = new Account_impl( dlg );

  boa->impl_is_ready( CORBA::ImplementationDef::_nil() );
  orb->run();
  CORBA::release( server );
  return 0;
}


CServerApp theApp;

