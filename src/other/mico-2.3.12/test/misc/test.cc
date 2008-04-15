#include <iostream.h>
#include <ministl/string>

class foo {
public:
	enum color { red, blue };
	class except {
	public:
		except (const string &m) : msg(m) {}
		string msg;
	};
	foo () throw (except);
	foo (color) throw (except);
	foo (unsigned long) throw (except);
};

foo::foo ()
	throw (except)
{
	throw except ("boing!");
}

foo::foo (color c)
	throw (except)
{
	cout << "foo(color)" << endl;
	throw except ("boing!");
}

foo::foo (unsigned long)
	throw (except)
{
	cout << "foo(ulong)" << endl;
	throw except ("boing!");
}

typedef foo bar;

int main ()
{
	try {
		bar x (bar::red);
	} catch (bar::except &e) {
		cout << "exception: " << e.msg << endl;
	}
	return 0;
}
