/*
 *  MICO --- a CORBA 2.0 implementation
 *  Copyright (C) 1997 Kay Roemer & Arno Puder
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  Send comments and/or bug reports to:
 *                 mico@informatik.uni-frankfurt.de
 */

import java.awt.*;
import java.awt.event.*;
import Account;


public class ClientApplet extends java.applet.Applet
                          implements ActionListener
{

  private TextField _currBalance, _amount;
  private Button _deposit, _withdraw;
  private Account _account;

  public void init() {

    // This GUI uses a 3 by 2 grid of widgets.
    setLayout( new GridLayout( 3, 2 ) );

    // Add the six widgets.
    add( "Center", new Label( "Current balance" ) );
    add( _currBalance = new TextField() );
    add( new Label( "Amount" ) );
    add( _amount = new TextField() );
    add( _deposit = new Button( "Deposit" ) );
    add( _withdraw = new Button( "Withdraw" ) );
    _currBalance.setEditable( false );

    // Register with ActionListener
    _deposit.addActionListener( this );
    _withdraw.addActionListener( this );

    // Initialize the ORB (using the Applet).
    org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init( this, null );

    // Locate the account object.
    String ref = getParameter( "IOR" );
    org.omg.CORBA.Object obj = orb.string_to_object( ref );
    _account = AccountHelper.narrow( obj );
    updateBalance();
  }

  private void updateBalance() {
    int balance = _account.balance();
    _currBalance.setText( Long.toString( balance ) );
  }

  public void actionPerformed( ActionEvent ev ) {
    int amount;
    try {
      amount = (int) Long.parseLong( _amount.getText() );
      if( ev.getSource() == _deposit ) {
	_account.deposit( amount );
      } else {
	_account.withdraw( amount );
      };
      updateBalance();
    } catch( NumberFormatException ex ) {
    }
  }

}
