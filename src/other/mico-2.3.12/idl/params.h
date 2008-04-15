/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
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

#ifndef __PARAMS_H__
#define __PARAMS_H__


class IDLParam 
{
public:
  std::string         cpp;
  std::string         base_dir;
  std::string         cpp_options;
  std::string         name;
  std::string         ifdef_prefix;
  std::string         file;
  std::string         cpp_suffix;
  std::string         hh_prefix;
  std::string         hh_suffix;
  std::string         wsdl_map;
  std::string         repo_id;
  std::vector<std::string> inc_paths;
  bool           use_quotes;
  bool           feed_ir;
  bool           feed_included_defs;
  bool           codegen_cpp;
  bool           cpp_impl;
  bool           cpp_skel;
  bool           codegen_idl;
  bool           codegen_midl;
  bool           codegen_wsdl;
  bool           emit_repoids;
  bool           query_server_for_narrow;
  bool           no_exceptions;
  bool           missing_nscorba;
  bool           support_id;
  bool           wsi_wsdl;
  bool           pseudo;
  bool           boa;
  bool           poa;
  bool           any;
  bool           typecode;
  bool           poa_ties;
  bool           poa_stubs;
  bool           windows_dll;
  bool           windows_dll_with_export;
  bool           mico_core;
  bool           gen_included_defs;
  bool           gen_full_dispatcher;
  bool           throw_decl;
  bool           idl3toidl2;
  bool           no_path_in_include;
  bool           reflection;
  bool           vc_sequence_reference_bug_workaround;
  std::map<std::string, std::string, std::less<std::string> > i_prefixes;
  std::map<std::string, std::string, std::less<std::string> > i_postfixes;
  std::string	output_dir;
  std::string	skel_suffix;
  std::string   dll_def_prefix;
  std::string   tc_export;
  std::string   static_tc_export;
  std::string   class_export;
  std::string   operator_export;
  
public:
  IDLParam( int argc, char *argv[] );
  IDLParam();

  bool cpp_only();

  std::string rewrite_include_path(std::string fname) const;

private:
  int    _argc;
  char** _argv;

  bool _cpp_only;
  
  void set_defaults();
  void parse_params();
  void check_params();
  void usage();
  void config();
};


#endif
