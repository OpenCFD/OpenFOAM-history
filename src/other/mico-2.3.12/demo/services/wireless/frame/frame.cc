#include <CORBA.h>
#include <coss/MobileTerminal.h>
#include "demo.h"
#include <functional>
#include <algorithm>
#include <unistd.h>

using namespace std;

static const string terminal_str = "\x04\x80\xd6\x0a\xa6\x08terminal";
static const CORBA::OctetSeq terminal_id(terminal_str.length(),
					 terminal_str.length(),
					 (CORBA::Octet *)terminal_str.c_str());

int
main (int argc, char *argv[])
{
    if (argc < 4) {
	cerr << "Usage: " << argv[0] << " <tername> <netname> <abs...>"
	     << endl;
	exit(1);
    }
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    CORBA::Object_var obj;
    vector<MobileTerminal::AccessBridge_ptr> abs;
    vector<string> addrs;
    char pwd[256];
    string directory = getcwd(pwd, sizeof pwd);

    for (int i = 3, port = 21100; i < argc; ++i, ++port) {
	string ab_name = "file://" + directory + "/ab"
	    + ((char)('0' + (i - 3))) + ".ref";
	cout << ab_name << endl;
	obj = orb->string_to_object(ab_name.c_str());
	abs.push_back(MobileTerminal::AccessBridge::_narrow(obj));
	char ab_addr[256];
	sprintf(ab_addr, "inet:%s:%d", argv[i], port);
	cout << ab_addr << endl;
	addrs.push_back(ab_addr);
    }
    string tb_name = "file://" + directory + "/tb.ref";
    cout << tb_name << endl;
    obj = orb->string_to_object(tb_name.c_str());
    MobileTerminal::TerminalBridge_var tb =
	MobileTerminal::TerminalBridge::_narrow(obj);
    char url[256];
    sprintf(url, "corbaloc::1.2@%s:34342/Demo", argv[1]);
    cout << url << endl;
    obj = orb->string_to_object(url);
    Demo_var terdemo = Demo::_narrow(obj);
    sprintf(url, "corbaloc::1.2@%s:34343/Demo", argv[2]);
    cout << url << endl;
    obj = orb->string_to_object(url);
    Demo_var netdemo = Demo::_narrow(obj);

    string line;
    for ( ; getline(cin, line); ) {
	cout << "Command: <" << line << ">" << endl;
	if (line.size() == 0 || line[0] == '#')
	    continue;
	string::iterator i = find_if(line.begin(), line.end(),
				     (int (*)(int))isspace);
	string command(line.begin(), i);
	if (command == "invoke") {
	    string::iterator j = find_if(i, line.end(),
					 not1(ptr_fun((int (*)(int))isspace)));
	    if (j == line.end())
		continue;
	    string::iterator k = find_if(j, line.end(), (int (*)(int))isspace);
	    string target(j, k);
	    j = find_if(k, line.end(), not1(ptr_fun((int (*)(int))isspace)));
	    if (j == line.end())
		continue;
	    string arg_str(j, line.end());
	    int arg = atoi(arg_str.c_str());
	    try {
		if (target == "net") {
		    cout << "Value: " << netdemo->date(arg) << endl;
		} else if (target == "term") {
		    cout << "Value: " << terdemo->date(arg) << endl;
		}
	    } catch (const CORBA::Exception &e) {
		cout << "Exception: " << e << endl;
	    }
	} else if (command == "delay") {
	    string::iterator j = find_if(i, line.end(),
					 not1(ptr_fun((int (*)(int))isspace)));
	    if (j == line.end())
		continue;
	    string delay_str(j, line.end());
	    int delay = atoi(delay_str.c_str());
	    sleep(delay);
	} else if (command == "handoff") {
	    string::iterator j = find_if(i, line.end(),
					 not1(ptr_fun((int (*)(int))isspace)));
	    if (j == line.end())
		continue;
	    string::iterator k = find_if(j, line.end(), (int (*)(int))isspace);
	    string target(j, k);
	    int from;
	    if (target == "net") {
		j = find_if(k, line.end(),
			    not1(ptr_fun((int (*)(int))isspace)));
		if (j == line.end())
		    continue;
		k = find_if(j, line.end(), (int (*)(int))isspace);
		string from_str(j, k);
		from = atoi(from_str.c_str());
	    }
	    j = find_if(k, line.end(), not1(ptr_fun((int (*)(int))isspace)));
	    if (j == line.end())
		continue;
	    string to_str(j, line.end());
	    int to = atoi(to_str.c_str());
	    if (target == "net") {
		if (from < 0 || from >= abs.size())
		    continue;
		if (to < 0 || to >= abs.size())
		    continue;
		try {
		    if (!abs[from]->terminal_attached(terminal_id))
			continue;
		    abs[from]->start_handoff(terminal_id, abs[to],
					     MobileTerminal::HandoffCallback::_nil());
		} catch (const CORBA::Exception &e) {
		    cout << "Exception: " << e << endl;
		}
	    } else if (target == "term") {
		if (to < 0 || to >= addrs.size())
		    continue;
		try {
	      	    tb->do_handoff(addrs[to].c_str());
		} catch (const CORBA::Exception &e) {
		    cout << "Exception: " << e << endl;
		}
	    }
	} else if (command == "recovery") {
	    string::iterator j = find_if(i, line.end(),
					 not1(ptr_fun((int (*)(int))isspace)));
	    if (j == line.end())
		continue;
	    string to_str(j, line.end());
	    int to = atoi(to_str.c_str());
	    if (to < 0 || to >= addrs.size())
		continue;
	    try {
		tb->release(FALSE);
		tb->connect(addrs[to].c_str());
	    } catch (const CORBA::Exception &e) {
		cout << "Exception: " << e << endl;
	    }
	} else if (command == "break") {
	    try {
		tb->release(FALSE);
	    } catch (const CORBA::Exception &e) {
		cout << "Exception: " << e << endl;
	    }
	}
    }
    return 0;
}
