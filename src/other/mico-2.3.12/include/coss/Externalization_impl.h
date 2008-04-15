/*
 *  Externaization Service for MICO
 *  Copyright (C) 1999 Dmitry V. Sedov
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
 *  or to my private e-mail:
 *                 sedov@postman.simcb.ru
 */

#ifndef __CosExternalization_impl_h__
#define __CosExternalization_impl_h__

#include <coss/CosExternalization.h>

class Stream_impl : virtual public POA_CosExternalization::Stream,
		    virtual public POA_CosLifeCycle::LifeCycleObject
{
    CORBA::String_var filename_;
    CosLifeCycle::Key factory_id_;
    std::istream* ifstream_;
    std::ostream* ofstream_;
    CORBA::Boolean context;
    
public:
    Stream_impl(const char* filename = NULL);
    ~Stream_impl();

// Function From LifeCycleObject Interface
    virtual CosLifeCycle::LifeCycleObject_ptr copy(CosLifeCycle::FactoryFinder_ptr there,
                                                   const CosLifeCycle::Criteria& the_criteria);

    virtual void move(CosLifeCycle::FactoryFinder_ptr there,
                      const CosLifeCycle::Criteria& the_criteria);

    virtual void remove();
//  **********************************************

    virtual void externalize(CosStream::Streamable_ptr theObject);

    virtual CosStream::Streamable_ptr internalize(CosLifeCycle::FactoryFinder_ptr there);

    virtual void begin_context();

    virtual void end_context();

    virtual void flush();
};

class StreamFactory_impl : virtual public POA_CosExternalization::StreamFactory
{
public:
    StreamFactory_impl();
    virtual CosExternalization::Stream_ptr create();
};

class FileStreamFactory_impl : virtual public POA_CosExternalization::FileStreamFactory
{
public:
    FileStreamFactory_impl();
    virtual CosExternalization::Stream_ptr create(const char* theFileName);
};

#endif __CosExternalization_impl_h__
