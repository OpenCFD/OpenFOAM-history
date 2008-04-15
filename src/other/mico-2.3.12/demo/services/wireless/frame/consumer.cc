#include <CORBA.h>
#include <mico/util.h>
#include <coss/CosNaming.h>
#include <coss/CosEventComm.h>
#include <coss/CosEventChannelAdmin.h>
#include <coss/MobileTerminalNotification.h>

using namespace std;

int
main (int argc, char *argv[])
{
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    CORBA::Object_var obj = orb->resolve_initial_references("NameService");
    CosNaming::NamingContext_var ns = CosNaming::NamingContext::_narrow(obj);

    assert(argc >= 2);
    CosNaming::Name name;
    name.length(2);
    name[0].id = CORBA::string_dup("MobilitySupport");
    name[1].id = CORBA::string_dup(argv[1]);
    obj = ns->resolve(name);
    CosEventChannelAdmin::EventChannel_var chan =
	CosEventChannelAdmin::EventChannel::_narrow(obj);
    CosEventChannelAdmin::ConsumerAdmin_var consumer = chan->for_consumers();
    CosEventChannelAdmin::ProxyPullSupplier_var supplier =
	consumer->obtain_pull_supplier();

    CORBA::String_var time_str = CORBA::string_alloc(20);
    time_t ev_time;

    try {
	for ( ; ; ) {
	    CORBA::Any_var event = supplier->pull();
	    CORBA::TypeCode_var tc = event->type();
	    time(&ev_time);
	    struct tm *evrec = localtime(&ev_time);
	    strftime(time_str, 20, "%Y-%m-%dT%H:%M:%S", evrec);
	    cout << "*** Mobility Event at " << time_str << endl;
	    if (tc->equal(MobileTerminalNotification::_tc_HandoffDepartureEvent)) {
		MobileTerminalNotification::HandoffDepartureEvent net_dep;
		event >>= net_dep;
		CORBA::String_var tid =
		    mico_url_encode(net_dep.terminal_id.get_buffer(),
				    net_dep.terminal_id.length());
		CORBA::String_var new_ab;
		if (!CORBA::is_nil(net_dep.new_access_bridge)) {
		    CORBA::IORProfile *prof =
			net_dep.new_access_bridge->_ior()->profile();
		    CORBA::Long keylen;
		    const CORBA::Octet *key = prof->objectkey(keylen);
		    new_ab = mico_url_encode(key, keylen);
		} else {
		    new_ab = CORBA::string_dup("(NONE)");
		}
		cout << "*** Terminal Departure" << endl;
		cout << "    Departing terminal: " << tid << endl;
		cout << "    New Access Bridge:  " << new_ab << endl;
	    } else if (tc->equal(MobileTerminalNotification::_tc_HandoffArrivalEvent)) {
		MobileTerminalNotification::HandoffArrivalEvent net_arr;
		event >>= net_arr;
		CORBA::String_var tid =
		    mico_url_encode(net_arr.terminal_id.get_buffer(),
				    net_arr.terminal_id.length());
		CORBA::String_var old_ab;
		if (!CORBA::is_nil(net_arr.old_access_bridge)) {
		    CORBA::IORProfile *prof =
			net_arr.old_access_bridge->_ior()->profile();
		    CORBA::Long keylen;
		    const CORBA::Octet *key = prof->objectkey(keylen);
		    old_ab = mico_url_encode(key, keylen);
		} else {
		    old_ab = CORBA::string_dup("(NONE)");
		}
		cout << "*** Terminal Arrival" << endl;
		cout << "    Arriving terminal: " << tid << endl;
		cout << "    Old Access Bridge: " << old_ab << endl;
	    } else if (tc->equal(MobileTerminalNotification::_tc_AccessDropoutEvent)) {
		MobileTerminalNotification::AccessDropoutEvent net_drop;
		event >>= net_drop;
		CORBA::String_var tid =
		    mico_url_encode(net_drop.terminal_id.get_buffer(),
				    net_drop.terminal_id.length());
		cout << "*** Terminal Dropout" << endl;
		cout << "    Terminal identifier: " << tid << endl;
	    } else if (tc->equal(MobileTerminalNotification::_tc_AccessRecoveryEvent)) {
		MobileTerminalNotification::AccessRecoveryEvent net_rec;
		event >>= net_rec;
		CORBA::String_var tid =
		    mico_url_encode(net_rec.terminal_id.get_buffer(),
				    net_rec.terminal_id.length());
		cout << "*** Terminal Recovery" << endl;
		cout << "    Terminal identifier: " << tid << endl;
	    } else if (tc->equal(MobileTerminalNotification::_tc_TerminalHandoffEvent)) {
		MobileTerminalNotification::TerminalHandoffEvent term_hoff;
		event >>= term_hoff;
		CORBA::String_var new_ab;
		if (!CORBA::is_nil(term_hoff.new_access_bridge)) {
		    CORBA::IORProfile *prof =
			term_hoff.new_access_bridge->_ior()->profile();
		    CORBA::Long keylen;
		    const CORBA::Octet *key = prof->objectkey(keylen);
		    new_ab = mico_url_encode(key, keylen);
		} else {
		    new_ab = CORBA::string_dup("(NONE)");
		}
		cout << "*** Access Handoff" << endl;
		cout << "    New Access Bridge: " << new_ab << endl;
	    } else if (tc->equal(MobileTerminalNotification::_tc_TerminalDropoutEvent)) {
		MobileTerminalNotification::TerminalDropoutEvent term_drop;
		event >>= term_drop;
		CORBA::String_var tid =
		    mico_url_encode(term_drop.terminal_id.get_buffer(),
				    term_drop.terminal_id.length());
		cout << "*** Access Dropout" << endl;
		cout << "    Terminal identifier: " << tid << endl;
	    } else if (tc->equal(MobileTerminalNotification::_tc_TerminalRecoveryEvent)) {
		MobileTerminalNotification::TerminalRecoveryEvent term_rec;
		event >>= term_rec;
		CORBA::String_var tid =
		    mico_url_encode(term_rec.terminal_id.get_buffer(),
				    term_rec.terminal_id.length());
		cout << "*** Access Recovery" << endl;
		cout << "    Terminal identifier: " << tid << endl;
	    } else {
		cout << "*** Unknown Event" << endl;
	    }
	    cout << endl;
	}
    } catch (const CORBA::Exception &e) {
	cerr << "Got exception: " << e << endl;
	return 1;
    }

    return 0;
}
