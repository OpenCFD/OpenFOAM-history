/*
 * Implementations for our valuetypes. Note the inheritance for
 * BinaryTree_impl, which is both a valuetype and a servant.
 */

#ifndef __TREE_IMPL_H__
#define __TREE_IMPL_H__

#include "tree.h"

class TreeNode_impl :
  virtual public OBV_TreeNode,
  virtual public CORBA::DefaultValueRefCountBase
{
public:
  TreeNode_impl ();
  TreeNode_impl (Key *, Node *);
};

class BinaryTree_impl :
  virtual public POA_BinaryTree,
  virtual public OBV_BinaryTree,
  virtual public PortableServer::ValueRefCountBase
{
public:
  BinaryTree_impl ();
  void insert (Key * k, Node * n);
  Node * find (Key * k);
};

class StringKey_impl :
  virtual public OBV_StringKey,
  virtual public CORBA::DefaultValueRefCountBase
{
public:
  StringKey_impl ();
  StringKey_impl (const char *);
  CORBA::Boolean equal (Key * other);
  CORBA::Boolean less_than (Key * other);
};

class Name_impl :
  virtual public OBV_Name,
  virtual public CORBA::DefaultValueRefCountBase
{
public:
  Name_impl ();
  Name_impl (const char *);
};

class NameValue_impl :
  virtual public OBV_NameValue,
  virtual public Name_impl,
  virtual public CORBA::DefaultValueRefCountBase
{
public:
  NameValue_impl ();
  NameValue_impl (const char *, const char *);
};

/*
 * Factories
 */

class TreeNode_Factory : public CORBA::ValueFactoryBase
{
public:
  CORBA::ValueBase * create_for_unmarshal ()
  {
    return new TreeNode_impl;
  }
};

class BinaryTree_Factory : public CORBA::ValueFactoryBase
{
public:
  CORBA::ValueBase * create_for_unmarshal ()
  {
    return new BinaryTree_impl;
  }
};

class StringKey_Factory : public CORBA::ValueFactoryBase
{
public:
  CORBA::ValueBase * create_for_unmarshal ()
  {
    return new StringKey_impl;
  }
};

class Name_Factory : public CORBA::ValueFactoryBase
{
public:
  CORBA::ValueBase * create_for_unmarshal ()
  {
    return new Name_impl;
  }
};

class NameValue_Factory : public CORBA::ValueFactoryBase
{
public:
  CORBA::ValueBase * create_for_unmarshal ()
  {
    return new NameValue_impl;
  }
};

#endif
