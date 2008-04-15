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

#ifndef __imr_imr_impl_h__
#define __imr_imr_impl_h__

class ImplementationDef_impl : public POA_CORBA::ImplementationDef {
    typedef CORBA::ImplementationDef::ActivationMode ActivationMode;
    typedef CORBA::ImplementationDef::ObjectInfoList ObjectInfoList;
    ActivationMode _mode;
    ObjectInfoList _objs;
    CORBA::String_var _name;
    CORBA::String_var _command;

    void put_str (std::string &, const std::string &);
    std::string get_str (std::string &);
public:
    ImplementationDef_impl (const char *);
    ImplementationDef_impl (ActivationMode, const ObjectInfoList &objs,
			    const char *name, const char *command);
    ~ImplementationDef_impl ();

    void mode (ActivationMode);
    ActivationMode mode ();

    void objs (const ObjectInfoList &objs);
    ObjectInfoList *objs ();

    char *name ();

    void command (const char *);
    char *command ();

    char *tostring ();
};


class ImplRepository_impl : public POA_CORBA::ImplRepository {
    typedef CORBA::ImplementationDef::ActivationMode ActivationMode;
    typedef CORBA::ImplementationDef::ObjectInfoList ObjectInfoList;
    typedef CORBA::ImplementationDef::ObjectTag ObjectTag;
    typedef std::list<CORBA::ImplementationDef_ptr> ListImplDef;
    typedef CORBA::ImplRepository::ImplDefSeq ImplDefSeq;
    ListImplDef defs;
public:
    ImplRepository_impl ();
    ~ImplRepository_impl ();

    CORBA::ImplementationDef_ptr restore (const char *);
    CORBA::ImplementationDef_ptr create (ActivationMode,
					 const ObjectInfoList &repoids,
					 const char *name, const char *cmd);
    void destroy (CORBA::ImplementationDef_ptr);
    ImplDefSeq *find_by_name (const char *);
    ImplDefSeq *find_by_repoid (const char *);
    ImplDefSeq *find_by_repoid_tag (const char *, const ObjectTag &);
    ImplDefSeq *find_all ();
};

#endif // __imr_imr_impl_h__
