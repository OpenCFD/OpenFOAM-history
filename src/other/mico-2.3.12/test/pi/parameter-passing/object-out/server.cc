
#include "foo.h"
#include <fstream>


using namespace std;

class Foo_impl
    : public virtual POA_Foo
{
};

class Bar_impl
    : public virtual POA_Bar
{
    void
    create(Foo_out value);
};

void
Bar_impl::create(Foo_out value)
{
    Foo_impl* foo = new Foo_impl;
    value = foo->_this();
}

int
main(int argc, char* argv[])
{
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);
    PortableServer::POAManager_var mgr = poa->the_POAManager();
    Bar_impl* bar = new Bar_impl;
    Bar_ptr ref = bar->_this();
    ofstream out("bar.ref");
    out << orb->object_to_string(ref) << endl;
    out.close();
    mgr->activate();
    orb->run();
    return 0;
}
