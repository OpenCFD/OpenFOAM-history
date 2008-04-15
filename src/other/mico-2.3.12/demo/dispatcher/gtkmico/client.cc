#include <mico/gtkmico.h>
#include <gtk/gtk.h>
#include <CORBA-SMALL.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif
#include "hello.h"


using namespace std;

HelloWorld_var hello_client;

void
return_cb (GtkWidget *w, gpointer data)
{
    gchar *arg;

    arg = gtk_entry_get_text (GTK_ENTRY(w));

    hello_client->hello (arg);
}

void
quit_cb (GtkWidget *w, gpointer data)
{
    exit (0);
}

int
main (int argc, char *argv[])
{
    /*
     * MICO initialization
     */
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");
    CORBA::BOA_var boa = orb->BOA_init (argc, argv, "mico-local-boa");

    /*
     * GTK initialization
     */

    gtk_init (&argc, &argv);

    /*
     * Use GTK dispatcher
     */
    orb->dispatcher (new GtkDispatcher ());

    /*
     * MICO stuff
     */
    CORBA::Object_var obj = orb->bind ("IDL:HelloWorld:1.0");
    if (CORBA::is_nil (obj)) {
	cout << "cannot bind to " << argv[1] << endl;
	return 1;
    }
    hello_client = HelloWorld::_narrow (obj);


    /*
     * GTK stuff
     */
    GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    
    GtkWidget *vbox = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (window), vbox);
    gtk_widget_show (vbox);

    // entry

    GtkWidget *entry = gtk_entry_new ();
    gtk_box_pack_start (GTK_BOX (vbox), entry, FALSE, FALSE, 0);
    gtk_widget_show (entry);

    gtk_signal_connect (GTK_OBJECT (entry), "activate",
			GTK_SIGNAL_FUNC (return_cb), NULL);
    
    // quit button

    GtkWidget *button = gtk_button_new_with_label("quit");
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);
    gtk_widget_show (button);

    gtk_signal_connect (GTK_OBJECT (button), "clicked",
			GTK_SIGNAL_FUNC (quit_cb), NULL);
    
    gtk_widget_show (window);

    gtk_main();

    return 0;
}


