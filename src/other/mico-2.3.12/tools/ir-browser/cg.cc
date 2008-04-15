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

#include <assert.h>
#include <ctype.h>
#include "cg.h"


SetOfString emptyStringSet;


Node::Node()
  : name( "" )
{
}

Node::~Node()
{
}

string
Node::quote (const string &s)
{
    if (s.length() == 0)
	return "''";
    int i;
    for (i = 0; i < s.length(); ++i) {
	if (!isalnum (s[i]) && s[i] != '_')
	    break;
    }
    if (i == s.length())
	return s;
    return string("'") + s + string("'");
}

string &Node::getName()
{
  return name;
}


void
Node::setName (const string &s)
{
  name = s;
}


RelationNode::RelationNode( const string &aName )
{
  name = aName;
}


RelationNode::RelationNode( RelationNode *node )
{
  name = node->name;
}


RelationNode::~RelationNode()
{
}


int RelationNode::isA()
{
  return RELATION_NODE;
}

int RelationNode::operator==( Node &node )
{
  return ( node.isA() == RELATION_NODE ) && 
    ( name == ((RelationNode&) node).name );
}

void RelationNode::print( ostream &ostr )
{
  ostr << "(" << quote (name) << ")";
}


ConceptNode::ConceptNode( const string &aName, SetOfString &theInst )
{
  name = aName;
  inst = theInst;
  value = 0;
  has_value = false;
}


ConceptNode::ConceptNode( ConceptNode *node )
{
  name = node->name;
  inst = node->inst;
  value = node->getValue();
  has_value = node->hasValue();
}


ConceptNode::~ConceptNode()
{
}


SetOfString &ConceptNode::getInstances()
{
  return inst;
}


void ConceptNode::setInstances( SetOfString &new_inst )
{
  inst = new_inst;
}


void ConceptNode::setValue( long aValue )
{
  value = aValue;
  has_value = true;
}


long ConceptNode::getValue()
{
  return value;
}


void ConceptNode::print( ostream &ostr )
{
  ostr << "[" << quote (name);
  if( inst.size() != 0 ) {
    ostr << ":";
    int first = 1;
    SetOfString::const_iterator i;
    for (i = inst.begin(); i != inst.end(); ++i) {
      if( first )
	first = 0;
      else
	ostr << ", ";
      ostr << quote (*i);
    }
  } else {
    if( has_value )
      ostr << ":" << value;
  }
  ostr << "]";
}


int ConceptNode::isA()
{
  return CONCEPT_NODE;
}


int ConceptNode::operator==( Node &node )
{
  if (node.isA () != CONCEPT_NODE)
    return false;
  
  ConceptNode &cnode = (ConceptNode&)node;
  if (!(name == cnode.name))
      return false;
  if (has_value != cnode.has_value)
      return false;
  if (has_value)
      return value == cnode.value;
  else
      return inst == cnode.inst;
}


ConceptualGraph::ConceptualGraph( Node *aNode, ConceptualGraph *theWidth, 
                                  ConceptualGraph *theDepth )
{
  validate();
  node = aNode;
  width = theWidth;
  depth = theDepth;
}

ConceptualGraph::~ConceptualGraph()
{
  invalidate();
  assert( node != NULL );
  delete node;
  if( width )
    delete width;
  if( depth )
    delete depth;
}

void ConceptualGraph::sanity()
{
#if 1
  assert( magic == CGMAGIC );
#endif
}

void ConceptualGraph::setNode( Node *aNode )
{
  sanity();
  if( node )
    delete node;
  node = aNode;
}


Node *ConceptualGraph::getNode()
{
  sanity();
  return node;
}


void ConceptualGraph::setWidth( ConceptualGraph *theWidth )
{
  sanity();
  if( width )
    delete width;
  width = theWidth;
}

void ConceptualGraph::setDepth( ConceptualGraph *theDepth )
{
  sanity();
  if( depth )
    delete depth;
  depth = theDepth;
}


ConceptualGraph *ConceptualGraph::getWidth()
{
  sanity();
  return width;
}


ConceptualGraph *ConceptualGraph::getDepth()
{
  sanity();
  return depth;
}


void ConceptualGraph::print( ostream &ostr )
{
  sanity();
  node->print( ostr );
  if( width ) {
    if( width->getDepth() )
      ostr << "-";
    ostr << "->";
    width->print( ostr );
  }
  if( width && width->getDepth() )
    ostr << ".";
  if( depth ) {
    ostr << ",->";
    depth->print( ostr );
  }
}



void ConceptualGraph::prettyPrint( ostream &ostr, int incr )
{
  doPrettyPrint( ostr, 0, incr );
}



void ConceptualGraph::doPrettyPrint( ostream &ostr, int indent, int incr )
{
  node->print( ostr );
  if( width != NULL ) {
    if( width->getDepth() != NULL ) {
      ostr << "-";
      indent = printSpaces( ostr, indent, incr );
    }
    ostr << "->";
    width->doPrettyPrint( ostr, indent, incr );
  }
  int new_incr = 0;
  if( width != NULL && width->getDepth() != NULL ) {
    ostr << ".";
    new_incr = -incr;
  }
  if( depth != NULL ) {
    ostr << ",";
    indent = printSpaces( ostr, indent, new_incr );
    ostr << "->";
    depth->doPrettyPrint( ostr, indent, incr );
  }
}



int ConceptualGraph::printSpaces( ostream &ostr, int indent, int incr )
{
  if( indent == -1 )
    return indent;
  indent += incr;
  int num = indent;
  ostr << "\n";
  while( num-- != 0 )
    ostr << " ";
  return indent;
}


bool ConceptualGraph::operator==( ConceptualGraph &cg2 )
{
  if (!(*getNode() == *cg2.getNode()))
    return false;

  vector<ConceptualGraph *> childs;
  for (ConceptualGraph *p = cg2.getWidth(); p; p = p->getDepth())
    childs.push_back (p);

  for (ConceptualGraph *p1 = getWidth(); p1; p1 = p1->getDepth()) {
    bool found = false;
    vector<ConceptualGraph *>::iterator i;
    for (i = childs.begin(); i != childs.end(); ++i) {
      if (**i == *p1) {
	childs.erase (i);
	found = true;
	break;
      }
    }
    if (!found)
      return false;
  }
  return childs.size() == 0;
}


void ConceptualGraph::subtract( ConceptualGraph &cg2 )
{
  for (ConceptualGraph *p2 = cg2.getWidth(); p2; p2 = p2->getDepth()) {
    ConceptualGraph **parent, *p1;
    for (parent = &width, p1 = *parent; p1; p1 = *parent) {
      if (*p1 == *p2) {
	*parent = p1->depth;
	p1->depth = NULL;
	delete p1;
      } else {
	parent = &p1->depth;
      }
    }
  }
}

		
Node *CopyNode( Node *node )
{
  assert( node->isA() == CONCEPT_NODE || node->isA() == RELATION_NODE );
  if( node->isA() == CONCEPT_NODE ) 
    return new ConceptNode( (ConceptNode *) node );
  else
    return new RelationNode( (RelationNode *) node );
}


ConceptualGraph *CopyGraph( ConceptualGraph *node )
{
  if( node == NULL )
    return NULL;
  ConceptualGraph *width = CopyGraph( node->getWidth() );
  ConceptualGraph *depth = CopyGraph( node->getDepth() );
  Node *graph_node = CopyNode( node->getNode() );
  ConceptualGraph *new_node = new ConceptualGraph( graph_node,
						   width, depth );
  return new_node;
}
