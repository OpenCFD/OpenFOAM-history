#define MICO_CONF_IMR
#include <CORBA-SMALL.h>
#ifdef HAVE_QLINEEDIT_H
#include <qlineedit.h>
#else
#include <qlined.h>
#endif
#include "hello.h"
#include <mico/qtmico.h>


using namespace std;

HelloWorld_var hello_client;

class HelloClientWidget
    : public QLineEdit
{
    Q_OBJECT
public:
    HelloClientWidget(QWidget* parent = NULL, const char* name = NULL)
	: QLineEdit(parent, name)
    {
	connect(this, SIGNAL(returnPressed()),
		this, SLOT(send()));
    }
    ~HelloClientWidget() 
    {
    }
private slots:
    void send()
    {
	hello_client->hello(this->text());
	//this->setText("");
    }
};

int main (int argc, char *argv[])
{
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    CApp app(argc,argv);

    // MICO stuff
    CORBA::Object_var obj = orb->bind("IDL:HelloWorld:1.0");
    if (CORBA::is_nil(obj)) {
	cout << "cannot bind to HelloWorld server" << endl;
	return 1;
    }
    hello_client = HelloWorld::_narrow(obj);
    HelloClientWidget h;
    app.setMainWidget(&h);
    h.setFont(QFont("times",32,QFont::Bold));		// default font
    h.setBackgroundColor(QLineEdit::white);			// default bg color
    h.setFixedSize(300,50);
    h.show();
    app.exec();
    return 0;
}

#include "client.moc"
