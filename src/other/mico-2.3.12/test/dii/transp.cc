#include <CORBA.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <string>
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <string.h>
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <mico/impl.h>
#include <mico/template_impl.h>


using namespace std;

struct Sender : public CORBA::TransportCallback {
    CORBA::Buffer buf;

    Sender (CORBA::Dispatcher *disp, CORBA::Address *addr, const string &s)
    {
	MICO::CDREncoder ec (&buf, FALSE);
	((CORBA::DataEncoder &)ec).put_string (s);
        CORBA::Transport *t = addr->make_transport();
        if (!t->connect (addr)) {
	    cerr << "connect: " << t->errormsg() << endl;
	    exit (1);
	}
        t->wselect (disp, this);
	t->block (FALSE);
    }

    virtual void callback (CORBA::Transport *t, Event e)
    {
        switch (e) {
        case Write:
            if (t->write (buf, buf.length()) < 0) {
		cerr << "write: " << t->errormsg() << endl;
		exit (1);
	    }
            if (buf.length() == 0) {
                t->close ();
            }
            break;

        case Remove:
            cout << "Sender removed" << endl;
            break;

        default:
            assert (0);
        }
    }
};

struct Receiver : public CORBA::TransportCallback {
    CORBA::Buffer buf;

    Receiver (CORBA::Dispatcher *disp, CORBA::Transport *t)
    {
        t->rselect (disp, this);
	t->block (FALSE);
    }

    virtual void callback (CORBA::Transport *t, Event e)
    {
        switch (e) {
        case Read:
            while (t->read (buf, 100) > 0)
                ;
	    if (t->bad()) {
		cerr << "read: " << t->errormsg() << endl;
		exit (1);
	    }
            if (buf.length() > 0) {
                MICO::CDRDecoder dc (&buf, FALSE);
                string s;
                CORBA::Boolean r = ((CORBA::DataDecoder &)dc).get_string_stl (s);
		assert (r);
                cout << "peer says: " << s << endl;
            }
	    break;

        case Remove:
            cout << "Receiver removed" << endl;
            break;

        default:
            assert (0);
        }
    }
};

struct Server : public CORBA::TransportServerCallback {
    CORBA::Dispatcher *dispatcher;

    Server (CORBA::Dispatcher *disp, CORBA::Address *addr)
    {
        CORBA::TransportServer *t = addr->make_transport_server ();
        if (!t->bind (addr)) {
	    cerr << "bind: " << t->errormsg() << endl;
	    exit (1);
	}
        t->aselect (disp, this);
	t->block (FALSE);
        dispatcher = disp;
    }

    virtual void callback (CORBA::TransportServer *t, Event e)
    {
        switch (e) {
        case Accept: {
            CORBA::Transport *nt = t->accept ();
	    if (!nt) {
		cerr << "accept: " << t->errormsg() << endl;
		exit (1);
	    }
	    t->block (FALSE);
            cout << "connect from: " << nt->peer()->stringify() << endl;
            (void)new Receiver (dispatcher, nt);
            break;
        }
        case Remove:
            cout << "Server removed" << endl;
            break;

        default:
            assert (0);
        }
    }
};

struct Timeout : public CORBA::DispatcherCallback {
    Timeout (CORBA::Dispatcher *disp, long tmout)
    {
	disp->tm_event (this, tmout);
    }
    void callback (CORBA::Dispatcher *disp, Event e)
    {
	assert (e == CORBA::Dispatcher::Timer);
	cout << "shutting down" << endl;
	exit (0);
    }
};

int
main (int argc, char *argv[])
{
    assert (argc == 2);

    MICO::SelectDispatcher disp;
    MICO::InetAddress sva (MICO::InetAddress::hostname().c_str(), 9999,
			   MICO::InetAddress::DGRAM);

    if (string("server") == argv[1]) {
        Server srv (&disp, &sva);
        disp.run();
    } else {
        Sender snd1 (&disp, &sva, "hello1");
        Sender snd2 (&disp, &sva, "hello2");
        Sender snd3 (&disp, &sva, "hello3");
        Sender snd4 (&disp, &sva, "hello4");
        Sender snd5 (&disp, &sva, "hello5");
	Timeout t (&disp, 10*1000L);
        disp.run();
    }
    return 0;
}
