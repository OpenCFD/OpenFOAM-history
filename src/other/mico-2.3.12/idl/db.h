/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
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
 *  For more information, visit the MICO Home Page at
 *  http://www.mico.org/
 */

#ifndef __DB_H__
#define __DB_H__

class ParseNode;

class DB
{
private:
  CORBA::Container_var _con;
  
  typedef std::map<std::string, std::string, std::less<std::string> >
  RepoidFnameMap;
  RepoidFnameMap _repoid_fname_map;

  std::string _toplevel_fname;

  class PseudoRepoId {
  private: 
    std::string _full_id;
    std::string _contained_id;
    std::string _prefix;
    std::string _suffix;
  public:
    PseudoRepoId();
    ~PseudoRepoId();
    void set_full_id( std::string str );
    std::string get_full_id();
    void add_prefix( std::string str );
    std::string get_prefix();
    void add_suffix( std::string str );
    std::string get_suffix();
    void set_contained_id( std::string str);
    std::string get_contained_id();
    bool contains_id();
  };

  typedef std::pair<std::string, PseudoRepoId> IdToPseudoIdMapEntry;
  typedef std::multimap<std::string, 
                   PseudoRepoId, std::less<std::string> > IdToPseudoIdMap;
  IdToPseudoIdMap _id_to_pseudo_id_map;

  typedef std::map<std::string, bool, std::less<std::string> > NameImplicitMap;
  NameImplicitMap _name_implicit_map;

  typedef std::vector<std::vector<std::string> > VecVecString;
  
  VecVecString    _current_scope;
  std::vector<std::string>  _current_prefix;
  std::vector<std::string>  _version_name;
  std::vector<std::string>  _version;
  std::vector<std::string>  _id_name;
  std::vector<std::string>  _id;
  std::vector<std::string>  _prefix_name;
  std::vector<std::string>  _prefix;

  void gen_pseudo_repoid( CORBA::IDLType_ptr t, PseudoRepoId &pseudo_id );

public:
  DB();
  ~DB();

  std::string gen_pseudo_repoid( CORBA::IDLType_ptr t );

  void register_anonym_type( ParseNode* node, CORBA::IDLType_ptr t );
  void register_repoid( ParseNode* node, const std::string &id );
  void register_repoid( const std::string &id, const std::string &fname );

  bool is_repoid_defined( const std::string& id ) const;
  std::string get_fname_for_repoid( const std::string& id );
  std::string get_toplevel_fname () const;
  void set_toplevel_fname (const char *);

  void enter_scope( const std::string& name );
  void leave_scope();
  void enter_include_file();
  void leave_include_file();
  void switch_prefix( const std::string& name );
  std::string get_current_scope() const;
  std::string get_current_prefix() const;
  std::string get_prefix(const char *) const;
  void set_prefix( const std::string& name, const std::string& prefix );
  void set_repo_version( const std::string& name, const std::string& version );
  void set_repo_id( const std::string& name, const std::string& id );
  void set_repoids( CORBA::Repository_ptr repo );

  void add_forward_dcl( const char* scoped_name );
  void remove_forward_dcl( const char* scoped_name );
  bool is_in_forward_dcl( const char* scoped_name );

  std::set<std::string, std::less<std::string> > _forward_dcl;

  void add_implicit (const char * scoped_name);
  bool is_implicit (const char * scoped_name);
  void clear_implicits ();
};


#endif


