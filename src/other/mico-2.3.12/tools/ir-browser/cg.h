// -*- c++ -*-
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

#ifndef __CG_H__
#define __CG_H__

#include <CORBA.h>

#include <stdlib.h>
#include <iostream.h>
#include <string>
#include <vector>


#define CONCEPT_NODE 1
#define RELATION_NODE 2


class Node 
{
protected:
  string name;
  string quote (const string &s);
public:
  Node();
  virtual ~Node();
  string &getName();
  void setName (const string &);
  virtual int isA() = 0;
  virtual int operator==( Node &node ) = 0;
  virtual void print( ostream &ostr ) = 0;
};


class RelationNode : public Node
{
public:
  RelationNode( const string &aName );
  RelationNode( RelationNode *node );
  ~RelationNode();
  int isA();
  int operator==( Node &node );
  void print( ostream &ostr );
};


typedef vector<string> SetOfString;

extern SetOfString emptyStringSet;


class ConceptNode : public Node
{
private:
  SetOfString  inst;
  long         value;
  bool         has_value;
public:
  ConceptNode( const string &aName, SetOfString &theInst = emptyStringSet );
  ConceptNode( ConceptNode *node );
  ~ConceptNode();
  int isA();
  int operator==( Node &node );
  SetOfString &getInstances();
  void setInstances( SetOfString &new_inst );
  void setValue( long aValue );
  long getValue();
  bool hasValue() const
  {
      return has_value;
  }
  void print( ostream &ostr );
};



class ConceptualGraph
{
private:
  Node            *node;
  ConceptualGraph *width;
  ConceptualGraph *depth;
public:
  ConceptualGraph( Node *aNode, ConceptualGraph *theWidth = NULL,
		   ConceptualGraph *theDepth = NULL );
  ~ConceptualGraph();
  void setNode( Node *aNode );
  Node *getNode();
  void setWidth( ConceptualGraph *theWidth );
  void setDepth( ConceptualGraph *theDepth );
  ConceptualGraph *getWidth();
  ConceptualGraph *getDepth();
  void print( ostream &ostr );
  void prettyPrint( ostream &ostr, int incr );
  bool operator== (ConceptualGraph &);
  void subtract (ConceptualGraph &);
  
private:
  void doPrettyPrint( ostream &ostr, int indent, int incr );
  int printSpaces( ostream &ostr, int indent, int incr );
  
  long magic;
  enum {CGMAGIC = 0x31415927L};
  
  void validate()   { magic = CGMAGIC; }
  void invalidate() { magic = 0L; }
  void sanity();
};


Node *CopyNode( Node *node );
ConceptualGraph *CopyGraph( ConceptualGraph *node );


#endif
