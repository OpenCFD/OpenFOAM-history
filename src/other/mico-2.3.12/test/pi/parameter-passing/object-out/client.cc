
#include "foo.h"
#include <fstream>


using namespace std;

int
main(int argc, char* argv[])
{
    try {
	CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
	ifstream in("bar.ref");
	string ref;
	in >> ref;
	CORBA::Object_var obj = orb->string_to_object(ref.c_str());
	Bar_var bar = Bar::_narrow(obj);
	assert(!CORBA::is_nil(bar));
	Foo_ptr foo = Foo::_nil();
	bar->create(foo);
	cout << "passed" << endl;
	//cout << orb->object_to_string(foo) << endl;
    } catch (...) {
	cout << "failed!" << endl;
    }
    return 0;
}
	
