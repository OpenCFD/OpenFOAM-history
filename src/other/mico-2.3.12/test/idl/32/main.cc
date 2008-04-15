
#include "test.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS


using namespace std;

// local interface implementation as described
// by ptc/03-03-09

class LocalTest_impl
    : public LocalTest,
      public CORBA::LocalObject
{
public:
    virtual ~LocalTest_impl()
    { cout << "~LocalTest_impl()" << endl; }

    void
    op()
    {}
};

int
main(int argc, char* argv[])
{
  // ORB initialization
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
  LocalTest_impl* impl = new LocalTest_impl;
  {
      assert(impl->_refcount_value() == 1);
      LocalTest_var test = LocalTest::_duplicate(impl);
      assert(impl->_refcount_value() == 2);
      LocalTest_var test2 = test;
      assert(impl->_refcount_value() == 3);
      LocalTest_ptr test3 = LocalTest::_duplicate(test2);
      assert(impl->_refcount_value() == 4);
      CORBA::release(test3);
      assert(impl->_refcount_value() == 3);
      impl->_add_ref();
      assert(impl->_refcount_value() == 4);
      LocalTest_impl* i2 = impl;
      i2->_remove_ref();
      assert(impl->_refcount_value() == 3);
      i2 = NULL;
  }
  assert(impl->_refcount_value() == 1);
  try {
      impl->_get_interface();
      assert(0);
  } catch (CORBA::NO_IMPLEMENT&) {
  } catch (CORBA::Exception&) {
      assert(0);
  }
  try {
      impl->_get_domain_managers();
      assert(0);
  } catch (CORBA::NO_IMPLEMENT&) {
  } catch (CORBA::Exception&) {
      assert(0);
  }
  try {
      impl->_get_policy(0);
      assert(0);
  } catch (CORBA::NO_IMPLEMENT&) {
  } catch (CORBA::Exception&) {
      assert(0);
  }
  try {
      CORBA::PolicyList pols;
      pols.length(0);
      impl->_set_policy_overrides(pols, CORBA::ADD_OVERRIDE);
      assert(0);
  } catch (CORBA::NO_IMPLEMENT&) {
  } catch (CORBA::Exception&) {
      assert(0);
  }
  CORBA::Boolean result = TRUE;
  result = impl->_non_existent();
  assert(!result);
  result = (impl->_hash(ULONG_MAX) == impl->_hash(ULONG_MAX));
  assert(result);
  result = FALSE;
  result = impl->_is_equivalent(impl);
  assert(result);
  impl->_remove_ref();
  cout << "return" << endl;
  return 0;
}
