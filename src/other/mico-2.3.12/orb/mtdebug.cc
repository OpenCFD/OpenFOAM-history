/* -*- mode: c++; c-basic-offset: 4; -*-
 *
 * Copyright (C) 1999 Andreas Schultz
 *
 */

#ifdef FAST_PCH
#include "orb_pch.h"
#endif // FAST_PCH
#ifdef __COMO__
#pragma hdrstop
#endif // __COMO__

#ifndef FAST_PCH

#define MICO_CONF_IMR
#include <CORBA-SMALL.h>
#include <mico/impl.h>

#endif // FAST_PCH


#ifdef DEBUG_NAMES
NamedObject::NamedObject () {
    
    _name = new char [strlen ("<UnNamed Object>") + 1];
    strcpy (_name, "<UnNamed Object>");
}

NamedObject::~NamedObject () {

    if (_name)
        delete [] _name;
}

//
// get/set this objects's Name (if assigned)
//
const char *
NamedObject::name (const char *new_name) {
 
    if (new_name)
    {
        if (_name)
            delete [] _name;

        _name = new char [ strlen (new_name) + 1 ];
        strcpy (_name, new_name);
    }
 
    return _name;
}

#endif // DEBUG_NAMES

MICOMT::AutoDebugLock::AutoDebugLock()
{
    __mtdebug_lock();
}

MICOMT::AutoDebugLock::~AutoDebugLock()
{
    __mtdebug_unlock();
}

#ifndef HAVE_THREADS

void __mtdebug_init()
{ 
}

void __mtdebug_lock()
{
}

void __mtdebug_unlock()
{
}

#endif
