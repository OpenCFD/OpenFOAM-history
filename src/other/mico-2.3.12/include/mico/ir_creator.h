/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
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
 *  For more information, visit the MICO Home Page at
 *  http://www.mico.org/
 */

#ifndef __IR_CREATOR_H__
#define __IR_CREATOR_H__

#define MICO_IR_EXC_DUPLICATE_NAME   100
#define MICO_IR_EXC_DUPLICATE_REPOID 101
#define MICO_IR_EXC_DUPLICATE_LABEL  102
#define MICO_IR_EXC_AMBIGUOUS_NAME   103
#define MICO_IR_EXC_OVERLOADING      104
#define MICO_IR_EXC_BAD_RECURSION    105
#define MICO_IR_EXC_BAD_ONEWAY       106
#define MICO_IR_EXC_DUPLICATE_MEMBER 107
#define MICO_IR_EXC_BAD_UNION        108
#define MICO_IR_EXC_BAD_TYPE         109

namespace MICO {

  CORBA::Repository * create_interface_repository (CORBA::ORB_ptr,
						   CORBA::Boolean = 0);

}

#endif
