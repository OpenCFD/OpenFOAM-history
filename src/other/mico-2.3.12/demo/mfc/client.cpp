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

#include "client.h"



CClientDlg::CClientDlg(Account_ptr account,
                       CWnd* pParent /*=NULL*/)
	: CDialog(CClientDlg::IDD, pParent)
{
  m_account = Account::_duplicate( account );
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_CURR_BALANCE, m_curr_balance);
  DDX_Control(pDX, IDC_AMOUNT, m_amount);
}


BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	ON_BN_CLICKED(IDC_DEPOSIT, OnDeposit)
	ON_BN_CLICKED(IDC_WITHDRAW, OnWithdraw)
END_MESSAGE_MAP()


BOOL CClientDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  updateBalance();
  return TRUE;
}


CORBA::Long CClientDlg::getAmount()
{
  CString amount;
  m_amount.GetWindowText( amount );
  return atol( amount );
}

void CClientDlg::updateBalance()
{
  CORBA::Long balance;

  balance = m_account->balance();

  char buf[ 50 ];
  sprintf( buf, "%d", balance );
  m_curr_balance.SetWindowText( buf );
}

void CClientDlg::OnDeposit() 
{
  m_account->deposit( getAmount() );
  updateBalance();
}

void CClientDlg::OnWithdraw() 
{
  m_account->withdraw( getAmount() );
  updateBalance();
}

BEGIN_MESSAGE_MAP(CClientApp, CWinApp)
END_MESSAGE_MAP()


CClientApp::CClientApp()
{
}

BOOL CClientApp::InitInstance()
{
  if (!AfxSocketInit()) {
    AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
    return FALSE;
  }

  int i = 0;
  CORBA::ORB_var orb = CORBA::ORB_init( i, NULL, "mico-local-orb" );
  if( CORBA::is_nil( orb ) ) {
    AfxMessageBox( IDP_ORB_INIT_FAILED );
    return FALSE;
  }

  CORBA::Object_var obj = orb->bind( "IDL:Account:1.0", "inet:127.0.0.1:5555" );
  if( CORBA::is_nil( obj ) ) {
    AfxMessageBox( IDP_ACCOUNT_INIT_FAILED );
    return FALSE;
  }

  Account_var account = Account::_narrow( obj );
  if( CORBA::is_nil( account ) ) {
    AfxMessageBox( IDP_ACCOUNT_INIT_FAILED );
    return FALSE;
  }

#ifdef _AFXDLL
  Enable3dControls();
#else
  Enable3dControlsStatic();
#endif

  CClientDlg dlg( account );
  m_pMainWnd = &dlg;
  dlg.DoModal();
  
  return FALSE;
}


CClientApp theApp;
