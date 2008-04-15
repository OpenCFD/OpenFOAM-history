
#include <CORBA.h>
#include <qlabel.h>
#include <mico/qtmico.h>
#include "hello.h"

// Implementation of the Hello class //////////////////

QLabel* h;
CApp* app;

class Hello_impl
    : virtual public POA_HelloWorld
{
public:
    void
    hello(const char *s)
    {
	// kcg: we don't need to lock CApp here, because it's already locked,
	// I only don't know where :-( I guess QT internals while working
	// with QT dispatcher (i.e. QNotifier)
	h->setText(s);
#ifdef HAVE_THREADS
	// but we need to wake up gui thread to render set text
	app->wakeUpGuiThread();
#endif // HAVE_THREADS
    }
};

int
main(int argc, char **argv)
{
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);
    PortableServer::POAManager_var mgr = poa->the_POAManager();

    app = new CApp(argc,argv);

    orb->dispatcher(app->get_dispatcher());

    // some qt pow wow ///////////////////////////////////////////////////

    h = new QLabel(NULL);

    h->setCaption("QT meets MICO (hello server)");
    h->setFont(QFont("times",32,QFont::Bold));       	// default font
    h->setBackgroundColor(QLabel::white);			// default bg color
    h->setAlignment(QLabel::AlignCenter);			// default alignment
    h->setFixedSize(300,50);
    app->setMainWidget(h);
    h->show();

    // Create Server Object /////////////////////////////////////////////

    Hello_impl* hello = new Hello_impl();
    hello->_this();

    mgr->activate();

    while(!app->finished()) {
	orb->perform_work();
    }
    orb->shutdown(TRUE);
    return 0;
}
