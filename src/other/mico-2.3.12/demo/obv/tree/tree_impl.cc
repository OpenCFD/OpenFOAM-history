/*
 * Implementations for our valuetypes.
 */


#include "tree.h"
#include "tree_impl.h"

TreeNode_impl::TreeNode_impl ()
{
}

TreeNode_impl::TreeNode_impl (Key * k, Node * n)
{
  key (k);
  data (n);
}

BinaryTree_impl::BinaryTree_impl ()
{
}

void
BinaryTree_impl::insert (Key * k, Node * n)
{
  TreeNode * where = Root();

  if (!where) {
    TreeNode_var nn = new TreeNode_impl (k, n);
    Root (nn);
    return;
  }

  while (42) {
    if (where->key()->less_than (k)) {
      if (where->left() == 0) {
	TreeNode_var nn = new TreeNode_impl (k, n);
	where->left (nn);
	break;
      }
      else {
	where = where->left();
      }
    }
    else {
      if (where->right() == 0) {
	TreeNode_var nn = new TreeNode_impl (k, n);
	where->right (nn);
	break;
      }
      else {
	where = where->right();
      }
    }
  }
}

Node *
BinaryTree_impl::find (Key * k)
{
  TreeNode * where = Root();

  while (where) {
    if (where->key()->equal (k)) {
      CORBA::add_ref (where->data());
      return where->data();
    }
    if (where->key()->less_than (k)) {
      where = where->left();
    }
    else {
      where = where->right();
    }
  }

  return 0;
}

StringKey_impl::StringKey_impl ()
{
}

StringKey_impl::StringKey_impl (const char * str)
  : OBV_StringKey (str)
{
}

CORBA::Boolean
StringKey_impl::equal (Key * other)
{
  StringKey * sk = StringKey::_downcast (other);
  assert (sk != 0);
  return (strcmp (key(), sk->key()) == 0);
}

CORBA::Boolean
StringKey_impl::less_than (Key * other)
{
  StringKey * sk = StringKey::_downcast (other);
  assert (sk != 0);
  return (strcmp (key(), sk->key()) < 0);
}

Name_impl::Name_impl ()
{
}

Name_impl::Name_impl (const char * str)
  : OBV_Name (str)
{
}

NameValue_impl::NameValue_impl ()
{
}

NameValue_impl::NameValue_impl (const char * n, const char * v)
  : OBV_NameValue (n, v), Name_impl (n)
{
}
