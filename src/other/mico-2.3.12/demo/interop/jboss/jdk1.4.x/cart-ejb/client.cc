#include <string>
#include <CORBA.h>
#include <coss/CosNaming.h>
#include "examples/CartHome.h"

#define MAXLEN 1024


using namespace std;

/*
 * Valuetype implementation for BookException
 */

class BookException_impl : 
  virtual public OBV_examples::BookException,
  virtual public CORBA::DefaultValueRefCountBase
{
private:
  static const char* _repository_id;

  static const char* _get_repository_id()
  {
    vector<string> repository_ids;
    CORBA::Boolean is_chunked;
    examples::BookException_var instance = 
      examples::BookException::_downcast(new BookException_impl());
    instance->_get_marshal_info(repository_ids, is_chunked); // MICO-specific
    assert(repository_ids.size() > 0);
    return strdup(repository_ids[0].c_str());
  }

public:
  static const char* repository_id()
  {
    return _repository_id;
  }

  static BookException_impl* _downcast(CORBA::ValueBase* vb)
  {
    void* p;

    // here we use a MICO-specific helper for safe downcasting
    if (vb && ((p = vb->_narrow_helper("LOCAL:BookException_impl")))) {
      return (BookException_impl*) p;
    }
    return 0;
  }

  // MICO-specific helper for safe downcasting
  virtual void* _narrow_helper(const char* repoid)
  {
    void* p;
    if (strcmp(repoid, "LOCAL:BookException_impl") == 0) {
      return (void*) this;
    }
    if ((p = examples::BookException::_narrow_helper(repoid)) != NULL) {
      return p;
    }
    return NULL;
  }

  // Gets the message associated with the exception
  ::CORBA::WStringValue* getMessage() 
  {
    // invoke protected method inherited from java::lang::Throwable
    return detailMessage();
  }

  BookException_impl() 
  {
  }

};

const char *BookException_impl::_repository_id = 
  BookException_impl::_get_repository_id();

/*
 * Valuetype implementation for StackTraceElement
 */

class StackTraceElement_impl : 
  virtual public OBV_java::lang::StackTraceElement,
  virtual public CORBA::DefaultValueRefCountBase
{
private:
  static const char* _repository_id;

  static const char* _get_repository_id()
  {
    vector<string> repository_ids;
    CORBA::Boolean is_chunked;
    java::lang::StackTraceElement_var instance = 
      java::lang::StackTraceElement::_downcast(new StackTraceElement_impl());
    instance->_get_marshal_info(repository_ids, is_chunked); // MICO-specific
    assert(repository_ids.size() > 0);
    return strdup(repository_ids[0].c_str());
  }

public:
  static const char* repository_id()
  {
    return _repository_id;
  }

  static StackTraceElement_impl* _downcast(CORBA::ValueBase* vb)
  {
    void* p;

    // here we use a MICO-specific helper for safe downcasting
    if (vb && ((p = vb->_narrow_helper("LOCAL:StackTraceElement_impl")))) {
      return (StackTraceElement_impl*) p;
    }
    return 0;
  }

  // MICO-specific helper for safe downcasting
  virtual void* _narrow_helper(const char* repoid)
  {
    void* p;
    if (strcmp(repoid, "LOCAL:StackTraceElement_impl") == 0) {
      return (void*) this;
    }
    if ((p = java::lang::StackTraceElement::_narrow_helper(repoid)) != NULL) {
      return p;
    }
    return NULL;
  }

  StackTraceElement_impl() 
  {
  }

};

const char * StackTraceElement_impl::_repository_id = 
   StackTraceElement_impl::_get_repository_id();

/*
 * Valuetype factory for BookException
 */

class BookException_Factory : public CORBA::ValueFactoryBase
{
public:
  CORBA::ValueBase* create_for_unmarshal()
  {
    return new BookException_impl;
  }
};

/*
 * Valuetype factory for StackTraceElement
 */

class StackTraceElement_Factory : public CORBA::ValueFactoryBase
{
public:
  CORBA::ValueBase* create_for_unmarshal()
  {
    return new StackTraceElement_impl;
  }
};

/*
 * Client mainline
 */

int main(int argc, char* argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

  // Register value factory

  CORBA::ValueFactoryBase_var vf = new BookException_Factory;
  orb->register_value_factory(BookException_impl::repository_id(), vf);
  vf = new StackTraceElement_Factory;
  orb->register_value_factory(StackTraceElement_impl::repository_id(), vf);

  // Acquire a reference to the Naming Service
  
  CORBA::Object_var nsobj =
    orb->resolve_initial_references("NameService");

  CosNaming::NamingContext_var nc = 
    CosNaming::NamingContext::_narrow(nsobj);

  if (CORBA::is_nil(nc)) {
    cerr << "Oops, I cannot access the Naming Service!" << endl;
    exit(1);
  }

  // Locate IOR for 'examples/MyCart'

  CosNaming::Name name;
  name.length(2);
  name[0].id = CORBA::string_dup("examples");
  name[0].kind = CORBA::string_dup("");
  name[1].id = CORBA::string_dup("MyCart");
  name[1].kind = CORBA::string_dup("");

  CORBA::Object_var obj;

  cout << "Looking up examples/MyCart..." << flush;

  try {
    obj = nc->resolve(name);
  }
  catch (CosNaming::NamingContext::NotFound &exc) {
    cout << "NotFound exception." << endl;
    exit(1);
  }
  catch (CosNaming::NamingContext::CannotProceed &exc) {
    cout << "CannotProceed exception." << endl;
    exit(1);
  }
  catch (CosNaming::NamingContext::InvalidName &exc) {
    cout << "InvalidName exception." << endl;
    exit(1);
  }

  cout << " Done." << endl;

  assert(!CORBA::is_nil(obj));

  // Got an IOR. It should be an examples::CartHome IOR.

  examples::CartHome_var home = 
     examples::CartHome::_narrow(obj);
  assert(!CORBA::is_nil(home));
  
  // Use home reference to create a cart instance.

  CORBA::WStringValue_var person = new CORBA::WStringValue(L"Duke DeEarl");
  CORBA::WStringValue_var id = new CORBA::WStringValue(L"123");
  examples::Cart_var shoppingCart = 
      home->create__CORBA_WStringValue__CORBA_WStringValue(person, id);
         
  assert(!CORBA::is_nil(shoppingCart));

  // Add some books to the cart.

  CORBA::WStringValue_var title 
      = new CORBA::WStringValue(L"The Martian Chronicles");
  shoppingCart->addBook(title);

  title = new CORBA::WStringValue(L"2001 A Space Odyssey");
  shoppingCart->addBook(title);

  title = new CORBA::WStringValue(L"The Left Hand of Darkness");
  shoppingCart->addBook(title);

  ::org::omg::boxedRMI::CORBA::seq1_WStringValue_var bookList = 
        new ::org::omg::boxedRMI::CORBA::seq1_WStringValue();

  // List cart contents.

  bookList = shoppingCart->contents();

  cout << "Cart contents:" << endl;
  for (unsigned j = 0; j < bookList->length(); j++) {
    char title_str[MAXLEN];

    title = (*bookList)[j]; 
    if (wcstombs(title_str, title->_value(), MAXLEN) != (size_t)(-1)) {
      cout << "\t" << title_str << endl;
    }
  }

  // Try to remove from the cart a book that is not there.

  try {
    title = new CORBA::WStringValue(L"Alice in Wonderland");
    shoppingCart->removeBook(title);
  }
  catch (examples::BookEx &exc) {
    cout << "Caught expected exception examples::BookEx:" << endl;
    examples::BookException_var value = exc.value;
    BookException_impl* bookException = BookException_impl::_downcast(value);
    char msg_str[MAXLEN];
    if (wcstombs(msg_str, 
                 bookException->getMessage()->_value(), 
                 MAXLEN) != (size_t)(-1)) {
      cout << "\t" << msg_str << endl;
    }
    exit(1);
  }

  // Destroy cart.

  shoppingCart->remove();

  return 0;
}
