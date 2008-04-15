#include <iostream>
#include <climits>
#include <cstdlib>
#include <ctime>

using namespace std;

int
main (int argc, char *argv[])
{
    long seed = 0, rounds = 0;

    if (argc == 2) {
	seed = atoi(argv[1]);
    } else if (argc > 2) {
	seed = atoi(argv[1]);
	rounds = atoi(argv[2]);
    }
    if (seed <= 0)
	seed = time(NULL);
    if (rounds <= 0)
	rounds = LONG_MAX;
    srand(seed);

    int limits[] = { 5 /* invoke */, 13 /* delay */, 16 /* handoff */,
		     18 /* recovery */ };
    int last = limits[3];
    int abno = 2;

    for (long i = 0; i < rounds; ++i) {
	int choice = rand() / (RAND_MAX / last + 1);
	if (choice < limits[0]) {
	    cout << "invoke ";
	    int which = rand() / (RAND_MAX / 2 + 1);
	    if (which == 0)
		cout << "net ";
	    else
		cout << "term ";
	    int delay = rand() / (RAND_MAX / 20 + 1);
	    delay -= 5;
	    if (delay < 0)
		delay = 0;
	    cout << delay << endl;
	} else if (choice < limits[1]) {
	    cout << "delay ";
	    int delay = rand() / (RAND_MAX / 10 + 1);
	    cout << delay + 1 << endl;
	} else if (choice < limits[2]) {
	    cout << "handoff ";
	    int which = rand() / (RAND_MAX / 2 + 1);
	    if (which == 0) {
		cout << "net ";
		int from = rand() / (RAND_MAX / abno + 1);
		cout << from << " ";
	    } else
		cout << "term ";
	    int to = rand() / (RAND_MAX / abno + 1);
	    cout << to << endl;
	} else if (choice < limits[3]) {
	    cout << "recovery ";
	    int to = rand() / (RAND_MAX / abno + 1);
	    cout << to << endl;
	}
    }

    return 0;
}
