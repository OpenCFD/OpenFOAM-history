#include <map>

typedef map<long, long, less<long> > MapLL;

class foo {
public:
	MapLL m;
	foo ();
	foo (const foo &);
	~foo ();
	foo &operator= (const foo &);
	int operator== (const foo &) const;
};

int
main ()
{
	foo x;
	return 0;
}
