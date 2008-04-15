/*
 *  MICO --- a free CORBA implementation
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
#include "accountI.h"
#include <mico/template_impl.h>

Account_impl::Account_impl()
{
    _balance = 0;
}

void
Account_impl::set_holder (const Holder &h)
{
    _holder = h;
}

Account::Holder *
Account_impl::get_holder ()
{
    return new Account::Holder (_holder);
}

void
Account_impl::deposit (CORBA::ULong amount)
{
    _balance += amount;
}

void
Account_impl::withdraw (CORBA::ULong amount)
{
    _balance -= amount;
}

CORBA::Long
Account_impl::balance ()
{
    return _balance;
}

