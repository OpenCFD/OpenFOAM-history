
#include <CORBA.h>
#include <mico/codec_impl.h>
//#include <mico/CSI.h>


using namespace std;

CORBA::OctetSeq*
string2octet_seq(string __str)
{
    CORBA::OctetSeq* __seq = new CORBA::OctetSeq;
    __seq->length(__str.length());
    for (int __i=0; __i<__str.length(); __i++)
	(*__seq)[__i] = (CORBA::Octet)__str[__i];
    return __seq;
}

int
main(int __argc, char** __argv)
{
    CORBA::ORB_ptr __orb = CORBA::ORB_init(__argc, __argv, "mico-local-orb");
    cout << "resolving security manager..." << flush;
    CORBA::Object_var __obj = __orb->resolve_initial_references("CSIv2SecurityManager");
    assert(!CORBA::is_nil(__obj));
    CSIv2::SecurityManager_ptr __sec_man = CSIv2::SecurityManager::_narrow(__obj);
    assert(!CORBA::is_nil(__sec_man));
    cout << "ok." << endl;
    cout << "resolving codec factory..." << flush;
    __obj = __orb->resolve_initial_references("CodecFactory");
    assert(!CORBA::is_nil(__obj));
    IOP::CodecFactory_ptr __factory = IOP::CodecFactory::_narrow(__obj);
    assert(!CORBA::is_nil(__factory));
    cout << "ok." << endl;
    cout <<"creating codec..." << flush;
    IOP::Encoding __encoding;
    __encoding.format = IOP::ENCODING_CDR_ENCAPS;
    __encoding.major_version = 1;
    __encoding.minor_version = 0;
    IOP::Codec_ptr __codec  = IOP::Codec::_duplicate(__factory->create_codec(__encoding));
    assert(!CORBA::is_nil(__codec));
    cout << "ok." << endl;

    CORBA::Any __any;
    CORBA::DataEncoder* __encoder = new MICO::CDREncoder;
    CORBA::Buffer* __buf;

    //
    // GSSUP::InitialContextToken encoding test
    //

    GSSUP::InitialContextToken __init_token;
    CORBA::OctetSeq* __name = string2octet_seq(string("karel"));
    CORBA::OctetSeq* __passwd = string2octet_seq(string("cobalt"));
    __init_token.username.length(__name->length());
    __init_token.password.length(__passwd->length());
    __init_token.target_name.length(0);
    for (int __i=0; __i<__name->length(); __i++)
	__init_token.username[__i] = (*__name)[__i];
    for (int __i=0; __i<__passwd->length(); __i++)
	__init_token.password[__i] = (*__passwd)[__i];
    delete __name;
    delete __passwd;
    cout << "encoding gssup initial context token by codec..." << flush;
    __any <<= __init_token;
    CORBA::OctetSeq* __d1 = __codec->encode_value(__any);
    cout << "ok." << endl;
    cout << "encoding gssup initial context token by generated marshaller..." << flush;
    _marshaller_GSSUP_InitialContextToken->marshal(*__encoder, (void*)&__init_token);
    __buf = __encoder->buffer();
    __buf->rseek_beg(0);
    CORBA::OctetSeq __d2;
    __d2.length(__buf->length());
    CORBA::Octet* __tbuf = __buf->buffer();
    for (int __i=0; __i<__d2.length(); __i++)
	__d2[__i] = __tbuf[__i];
    cout << "ok." << endl;
    cout << "comparing encoded sequences...";
    if (__d1->length() == __d2.length()) {
	for (int __i=0; __i<__d2.length(); __i++) {
	    if (__d2[__i] != (*__d1)[__i])
		cout << "fail on " << __i << ". octet! FAIL!" << endl;
	}
	cout << "ok." << endl;
    }
    else {
	cout << "fail on length. FAIL!" << endl;
    }
    delete __d1;
    delete __encoder;
    __buf = NULL;
    __encoder = new MICO::CDREncoder;

    cout << "encoding establish context msg by codec..." << flush;
    CSI::EstablishContext __msg;
    __msg.client_context_id = 0;
    __msg.authorization_token = 0;
    //__msg.identity_token = 0;
    //__msg.client_authentication_token = *__token;
    CSI::SASContextBody __body;
    __body.establish_msg(__msg);    
    __any <<= __body;
    __d1 = __codec->encode_value(__any);
    cout << "ok." << endl;
    cout << "encoding establish context msg by generated marshaller..." << flush;
    assert(__encoder != NULL);
    _marshaller_CSI_SASContextBody->marshal(*__encoder, (void*)&__body);
    __buf = __encoder->buffer();
    __buf->rseek_beg(0);
    __d2.length(__buf->length());
    __tbuf = __buf->buffer();
    for (int __i=0; __i<__d2.length(); __i++)
	__d2[__i] = __tbuf[__i];
    cout << "ok." << endl;
    cout << "comparing encoded sequences...";
    if (__d1->length() == __d2.length()) {
	for (int __i=0; __i<__d2.length(); __i++) {
	    if (__d2[__i] != (*__d1)[__i])
		cout << "fail on " << __i << ". octet! FAIL!" << endl;
	}
	cout << "ok." << endl;
    }
    else {
	cout << "fail on length. FAIL!" << endl;
    }
}
