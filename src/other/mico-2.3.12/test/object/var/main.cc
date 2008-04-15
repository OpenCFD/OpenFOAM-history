
#include <CORBA.h>

using namespace CORBA;
using namespace std;

int
main(int argc, char* argv[])
{
    ORB_var orb = ORB_init(argc, argv);
    Object_var obj = new Object;
    assert(obj->_refcnt() == 1);
    Object_var obj2 = obj; // implicit duplicate
    assert(obj->_refcnt() == 2);
    obj2 = Object::_nil(); // release
    assert(obj->_refcnt() == 1);
    obj2 = Object::_duplicate(obj); // explicit duplicate
    assert(obj->_refcnt() == 2);
    obj2 = Object::_nil(); // release
    assert(obj->_refcnt() == 1);
    // testing assignments of the same object
    obj = obj;
    assert(obj->_refcnt() == 1);
    obj = obj;
    assert(obj->_refcnt() == 1);
    obj = Object::_duplicate(obj);
    assert(obj->_refcnt() == 1);
    obj = Object::_duplicate(obj);
    assert(obj->_refcnt() == 1);
    cout << "All tests passed" << endl;
    return 0;
}
