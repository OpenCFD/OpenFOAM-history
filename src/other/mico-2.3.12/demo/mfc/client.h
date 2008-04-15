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


#ifndef __CLIENT_H__
#define __CLIENT_H__


#include "client-res.h"
#include "account.h"



class CClientDlg : public CDialog
{
public:
  CClientDlg( Account_ptr account, CWnd* pParent = NULL);

  enum { IDD = IDD_CLIENT_DIALOG };

  Account_var m_account;
  
  CEdit	m_curr_balance;
  CEdit	m_amount;

protected:
  virtual void DoDataExchange(CDataExchange* pDX);

protected:
  virtual BOOL OnInitDialog();
  afx_msg void OnCancelMode();
  afx_msg void OnDeposit();
  afx_msg void OnWithdraw();
  DECLARE_MESSAGE_MAP()

private:
  CORBA::Long getAmount();
  void updateBalance();
};


class CClientApp : public CWinApp
{
public:
  CClientApp();

public:
  virtual BOOL InitInstance();

  DECLARE_MESSAGE_MAP()
};

#endif
