#include <iostream.h>

class A {
public:
    virtual void *ptr () { return (void *)this; }
    virtual void foo () { cout << "A" << endl; }
};
class B : virtual public A {
public:
    virtual void *ptr () { return (void *)this; }
    virtual void foo () { cout << "B" << endl; }
};

int
main ()
{
  A *a = new B ();
  a->foo ();
  B *b = (B *)a->ptr();
  b->foo ();
}
