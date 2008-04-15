/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2005 by The Mico Team
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  For more information, visit the MICO Home Page at
 *  http://www.mico.org/
 */

#ifdef FAST_PCH
#include "orb_pch.h"
#endif // FAST_PCH
#ifdef __COMO__
#pragma hdrstop
#endif // __COMO__

#ifndef FAST_PCH

#define MICO_CONF_INTERCEPT
#define MICO_CONF_IMR
#define MICO_CONF_IR
#define MICO_CONF_POA

#include <CORBA-SMALL.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif
#include <mico/util.h>
#include <mico/impl.h>
#include <mico/intercept.h>
#include <mico/ssl.h>
#include <mico/template_impl.h>

#ifdef USE_CSL2
#  include <mico/security/securitylevel2_impl.h>
#  include <mico/security/AuditIntercept.h>
#endif /* USE_CSL2  */

#ifdef USE_CSIV2
#include <mico/security/csiv2_impl.h>
#endif // USE_CSIV2

#endif // FAST_PCH

#ifdef HAVE_SSL

#ifdef HAVE_THREADS
#  ifdef HAVE_OPENSSL_SSL_H
// test if OpenSSL supports multi-threaded environment
#    define OPENSSL_THREAD_DEFINES
#    undef THREADS
#    undef OPENSSL_THREADS
#    include <openssl/opensslconf.h>
#    if !defined(THREADS) && !defined(OPENSSL_THREADS)
#      error You are trying to build multi-threaded MICO with single-threaded OpenSSL!
#    endif // THREADS
#    define USE_OPENSSL_THREAD_FUNCTIONS
#  else // HAVE_OPENSSL_SSL_H
#    error Can not test if your SSL lib was build with multi-threaded support
#  endif // HAVE_OPENSSL_SSL_H
#endif // HAVE_THREADS

#ifdef HAVE_OPENSSL_SSL_H
#include <openssl/err.h>
#endif // HAVE_OPENSSL_SSL_H


using namespace std;

// -ORBSSL* options
static MICOGetOpt::OptVec SSL_options;


/***************************** BIO methods ****************************/

extern "C" {
    typedef long (*ssl_long_t) ();
    typedef int (*ssl_int_t) ();
}

static int
mico_bio_new (BIO *b)
{
    b->init = 0;
    b->num = 0;
    b->ptr = 0;
    b->flags = 0;
    return 1;
}

static int
mico_bio_free (BIO *b)
{
    if (!b)
	return 0;
    if (b->shutdown) {
	if (b->init) {
	    CORBA::Transport *t = (CORBA::Transport *)b->ptr;	
	    assert (t);
	    t->close ();
	}
	b->init = 0;
	b->flags = 0;
    }
    return 1;
}

static int
mico_bio_read (BIO *b, char *out, int len)
{
    int ret = 0;
    if (out) {
	CORBA::Transport *t = (CORBA::Transport *)b->ptr;
	assert (t);
	ret = t->read (out, len);
	BIO_clear_retry_flags (b);
	if (ret <= 0) {
	    if (ret != len && !t->eof())
		BIO_set_retry_read (b);
	}
    }
    return ret;
}

static int
mico_bio_write (BIO *b, const char *in, int len)
{
    int ret;

    CORBA::Transport *t = (CORBA::Transport *)b->ptr;
    assert (t);

    ret = t->write (in, len);

    BIO_clear_retry_flags (b);
    if (ret <= 0) {
	if (ret != len && !t->eof())
	    BIO_set_retry_write (b);
    }
    return ret;
}

static int
mico_bio_puts (BIO *b, const char *out)
{
    return mico_bio_write (b, out, strlen (out));
}

static long
mico_bio_ctrl (BIO *b, int cmd, long num, void *ptr)
{
    long ret = 1;

    switch (cmd) {
    case BIO_CTRL_RESET:
	ret = 0;
	break;

    case BIO_CTRL_INFO:
	ret = 0;
	break;

    case BIO_CTRL_SET:
	b->ptr = ptr;
	b->num = 0;
	b->shutdown = (int)num;
	b->init = 1;
	break;

    case BIO_CTRL_GET:
	if (b->init) {
	    if (!ptr)
		ret = 0;
	    else
		*(char **)ptr = (char *)b->ptr;
	} else {
	    ret = -1;
	}
	break;

    case BIO_CTRL_GET_CLOSE:
	ret = b->shutdown;
	break;

    case BIO_CTRL_SET_CLOSE:
	b->shutdown = (int)num;
	break;

    case BIO_CTRL_PENDING:
    case BIO_CTRL_WPENDING:
	ret = 0;
	break;

    case BIO_CTRL_FLUSH:
    case BIO_CTRL_DUP:
	break;

    default:
	ret = 0;
	break;
    }
    return ret;
}


static BIO_METHOD mico_bio_methods = {
    BIO_TYPE_MEM, "mico_bio",
    (int (*)(BIO*, const char*, int))mico_bio_write,
    (int (*)(BIO*, char*, int))mico_bio_read,
    (int (*)(BIO*, const char*))mico_bio_puts,
    0, // mico_bio_gets
    (long (*)(BIO*, int, long int, void*))mico_bio_ctrl,
    (int (*)(BIO*))mico_bio_new,
    (int (*)(BIO*))mico_bio_free
};

static BIO_METHOD *BIO_mico ()
{
    return &mico_bio_methods;
}

//
// OpenSSL locking primitives
//
#ifdef USE_OPENSSL_THREAD_FUNCTIONS

static MICOMT::Mutex* S_ssl_mutex_array;
static long* S_ssl_mutex_count;

void
micomt_ssl_locking_callback(int mode, int type, const char* file, int line)
{
//      cerr << "thread: " << CRYPTO_thread_id() << " mode: "
//  	 << ((mode & CRYPTO_LOCK) ? "lock" : "unlock")
//  	 << "file: " << file << ":" << line << endl;
    if (mode & CRYPTO_LOCK) {
	S_ssl_mutex_array[type].lock();
	S_ssl_mutex_count[type]++;
    }
    else {
	S_ssl_mutex_array[type].unlock();
    }
}

MICOMT::Thread::ThreadID
micomt_ssl_thread_id_callback()
{
    return MICOMT::Thread::self();
}

#endif // USE_OPENSSL_THREAD_FUNCTIONS

/*************************** SSLAddress ****************************/


MICOSSL::SSLAddress::SSLAddress (CORBA::Address *a)
{
    // consume a
    _addr = a;
}

MICOSSL::SSLAddress::SSLAddress (const SSLAddress &a)
{
    _addr = a._addr->clone();
}

MICOSSL::SSLAddress &
MICOSSL::SSLAddress::operator= (const SSLAddress &a)
{
    delete _addr;
    _addr = a._addr->clone();
    return *this;
}

MICOSSL::SSLAddress::~SSLAddress ()
{
    delete _addr;
}

string
MICOSSL::SSLAddress::stringify () const
{
    return string("ssl:") + _addr->stringify();
}

const char *
MICOSSL::SSLAddress::proto () const
{
    return "ssl";
}

CORBA::Transport *
MICOSSL::SSLAddress::make_transport () const
{
    return new SSLTransport (this);
}

CORBA::TransportServer *
MICOSSL::SSLAddress::make_transport_server () const
{
    return new SSLTransportServer (this);
}

CORBA::IORProfile *
MICOSSL::SSLAddress::make_ior_profile (CORBA::Octet *key,
				       CORBA::ULong keylen,
				       const CORBA::MultiComponent &mc,
                                       CORBA::UShort version) const
{
    return new SSLProfile (key, keylen, *this, mc, version);
}

CORBA::Boolean
MICOSSL::SSLAddress::is_local () const
{
    return _addr->is_local();
}

CORBA::Boolean
MICOSSL::SSLAddress::is_here () const
{
    return _addr->is_here();
}

CORBA::Address *
MICOSSL::SSLAddress::clone () const
{
    return new SSLAddress (*this);
}

const CORBA::Address *
MICOSSL::SSLAddress::content () const
{
    return _addr;
}

void
MICOSSL::SSLAddress::content (CORBA::Address *a)
{
    delete _addr;
    // consume a
    _addr = a;
}

CORBA::Long
MICOSSL::SSLAddress::compare (const CORBA::Address &a) const
{
    CORBA::Long r = strcmp (proto(), a.proto());
    if (r)
	return r;
    const SSLAddress &he = (SSLAddress &)a;
    return _addr->compare (*he._addr);
}

CORBA::Boolean
MICOSSL::SSLAddress::operator== (const CORBA::Address &a) const
{
    return compare (a) == 0;
}

CORBA::Boolean
MICOSSL::SSLAddress::operator< (const CORBA::Address &a) const
{
    return compare (a) < 0;
}


/*************************** SSLAddressParser *************************/


MICOSSL::SSLAddressParser::SSLAddressParser ()
{
    CORBA::Address::register_parser (this);
}

MICOSSL::SSLAddressParser::~SSLAddressParser ()
{
    CORBA::Address::unregister_parser (this);
}

CORBA::Address *
MICOSSL::SSLAddressParser::parse (const char *str, const char *) const
{
    CORBA::Address *a = CORBA::Address::parse (str);
    if (!a)
	return 0;

    return new SSLAddress (a);
}

CORBA::Boolean
MICOSSL::SSLAddressParser::has_proto (const char *p) const
{
    return !strcmp ("ssl", p);
}

static MICOSSL::SSLAddressParser ssl_address_parser;


/************************** SSLTransport *****************************/


namespace MICOSSL {
  int SSLTransport::_ssl_verify_depth = 0;
  SSL_CTX *SSLTransport::_ssl_ctx = 0;
}

MICOSSL::SSLTransport::SSLTransport (const SSLAddress *a, CORBA::Transport *t)
{
    _closed = TRUE;
    _transp = t ? t : a->content()->make_transport();
    _local_addr = (SSLAddress *)a->clone();
    _peer_addr = (SSLAddress *)a->clone();

#ifdef USE_CSL2
    _ssl = 0;
    if (_ssl_ctx == NULL) {
	//if SecurityManager wasn't initialized
	CORBA::Boolean r = setup_ctx();
	assert (r);
    }
#else
    CORBA::Boolean r = setup_ctx();
    assert (r);
#endif /* USE_CSL2  */

    _bio = BIO_new (BIO_mico());
    assert (_bio);
    BIO_ctrl (_bio, BIO_CTRL_SET, 0, (char *)_transp);
    _ssl = SSL_new (_ssl_ctx);
    SSL_set_bio (_ssl, _bio, _bio);

    _rcb = _wcb = 0;
}

MICOSSL::SSLTransport::~SSLTransport ()
{
#ifdef HAVE_THREADS
    {
    MICOMT::AutoLock lock(_ssl_mutex);
#endif // HAVE_THREADS
    CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb", FALSE);
    if (!CORBA::is_nil(orb)) {
	_transp->rselect (orb->dispatcher(), 0);
	_transp->wselect (orb->dispatcher(), 0);
    }
    _rcb = _wcb = 0;
    if (!_closed) {
	this->close();
    }
//    BIO_ctrl (_ssl->rbio, BIO_CTRL_SET_CLOSE, 1, (char *)0);
    SSL_free (_ssl);
    
    // BIO is freed in SSL_free()
    delete _transp;
    delete _local_addr;
    delete _peer_addr;
#ifdef HAVE_THREADS
    }
#endif // HAVE_THREADS
}

#ifdef USE_CSL2
SSL*
MICOSSL::SSLTransport::get_ssl()
{
    return _ssl;
}
#endif /* USE_CSL2  */

int
MICOSSL::SSLTransport::ssl_verify_callback (int ok, X509_STORE_CTX *ctx)
{
    int error = X509_STORE_CTX_get_error (ctx);
    int depth = X509_STORE_CTX_get_error_depth (ctx);

    if (!ok) {
        if (MICO::Logger::IsLogged (MICO::Logger::Error)) {
	    MICO::Logger::Stream (MICO::Logger::Error)
		<< "SSL: verify error: "
		<< X509_verify_cert_error_string (error)
		<< endl;
	}

	// XXX
	if (_ssl_verify_depth <= depth) {
	    ok = 1;
	} else {
	    ok = 0;
	}
    }
    return ok;
}

CORBA::Boolean
MICOSSL::SSLTransport::setup_ctx ()
{
    if (_ssl_ctx)
	return TRUE;

    const char *cert = 0, *key = 0, *cafile = 0, *capath = 0, *cipher = 0;
    int verify = SSL_VERIFY_NONE;

    const MICOGetOpt::OptVec &o = SSL_options;
    for (MICOGetOpt::OptVec::const_iterator i = o.begin(); i != o.end(); ++i) {
	const string &arg = (*i).first;
	const string &val = (*i).second;
	if (arg == "-ORBSSLverify") {
	    verify = SSL_VERIFY_PEER;
	    _ssl_verify_depth = atoi ((char *)val.c_str());
	} else if (arg == "-ORBSSLcert") {
	    cert = val.c_str();
	} else if (arg == "-ORBSSLkey") {
	    key = val.c_str();
	} else if (arg == "-ORBSSLCApath") {
	    capath = val.c_str();
	} else if (arg == "-ORBSSLCAfile") {
	    cafile = val.c_str();
	} else if (arg == "-ORBSSLcipher") {
	    cipher = val.c_str();
	}
    }

    if (!cipher)
	cipher = getenv ("SSL_CIPHER");
    if (!cert)
	cert = "default.pem";
    if (!key)
	key = cert;

#ifdef USE_CSL2
    CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb", FALSE);
    CORBA::Object_var securitymanager = orb->resolve_initial_references
	("SecurityManager");
    assert (!CORBA::is_nil (securitymanager));

    SecurityLevel2::SecurityManager_var secman
	= SecurityLevel2::SecurityManager::_narrow(securitymanager);
    assert (!CORBA::is_nil (secman));

    Security::AuthenticationMethod our_method
	= (Security::AuthenticationMethod)SecurityLevel2::KeyCertCAPass;

    Security::SSLKeyCertCAPass method_struct;

    CORBA::Any* any_struct = secman -> get_method_data(our_method);
    *any_struct >>= method_struct;

    method_struct.key = key;
    method_struct.cert = cert;
    method_struct.CAfile = cafile;
    method_struct.CAdir = capath;
    method_struct.pass = CORBA::string_dup("");

    CORBA::Any* out_any_struct;
    out_any_struct = new CORBA::Any;
    *out_any_struct <<= method_struct;
    SecurityLevel2::PrincipalAuthenticator_ptr pa
	= secman -> principal_authenticator();

    const char* mechanism = "";
    const char* security_name = "ssl";
    Security::AttributeList privileges;
    SecurityLevel2::Credentials_ptr creds;
    CORBA::Any* continuation_data;
    CORBA::Any* auth_specific_data;

    try {
	pa->authenticate(our_method, mechanism, security_name, *out_any_struct,
			 privileges, creds, continuation_data,
			 auth_specific_data);
    }
    catch (...) {
	cout << "authentication failed" <<endl;
	delete out_any_struct;
	return 0;
    }
#else /* NOT USE_CSL2  */

    SSLeay_add_ssl_algorithms ();
    _ssl_ctx = SSL_CTX_new (SSLv23_method());

    if (cipher)
	SSL_CTX_set_cipher_list (_ssl_ctx, (char *)cipher);
    SSL_CTX_set_verify (_ssl_ctx, verify, ssl_verify_callback);

    SSL_CTX_set_client_CA_list (_ssl_ctx,
				SSL_load_client_CA_file ((char *)cafile));

    if (SSL_CTX_use_certificate_file (_ssl_ctx, (char *)cert,
				      SSL_FILETYPE_PEM) <= 0) {
        if (MICO::Logger::IsLogged (MICO::Logger::Error)) {
	  MICOMT::AutoDebugLock lock;
	  MICO::Logger::Stream (MICO::Logger::Error)
	    << "SSL: cannot set SSL certificate file: " << cert << endl;
	}
	return FALSE;
    }
    if (SSL_CTX_use_PrivateKey_file (_ssl_ctx, (char *)key,
				     SSL_FILETYPE_PEM) <= 0) {
        if (MICO::Logger::IsLogged (MICO::Logger::Error)) {
	  MICOMT::AutoDebugLock lock;
	  MICO::Logger::Stream (MICO::Logger::Error)
	    << "SSL: cannot set SSL private key file: " << key << endl;
	}
	return FALSE;
    }
    if (SSL_CTX_check_private_key (_ssl_ctx) <= 0) {
        if (MICO::Logger::IsLogged (MICO::Logger::Error)) {
	  MICOMT::AutoDebugLock lock;
	  MICO::Logger::Stream (MICO::Logger::Error)
	    << "SSL: bad private key" << endl;
	}
	return FALSE;
    }

    SSL_load_error_strings ();

    int ret = SSL_CTX_load_verify_locations (_ssl_ctx, (char *)cafile,
                                             (char *)capath);
    
    if ((!ret && (capath || cafile)) ||
	!SSL_CTX_set_default_verify_paths (_ssl_ctx)) {
        if (MICO::Logger::IsLogged (MICO::Logger::Error)) {
	  MICOMT::AutoDebugLock lock;
	  MICO::Logger::Stream (MICO::Logger::Error)
	    << "SSL: cannot set SSL verify locations" << endl;
	}
	return FALSE;
    }
#endif /* USE_CSL2  */

#ifdef USE_CSL2
//  X509 *peer = MICOSSL::SSLTransport::load_cert((char *)cert);
//  MICOSSL::SSLPrincipal principal(peer, "");
//  CORBA::Any* info = principal.get_property("ssl-x509-subject");
//  const char *s;
//  *info >>= s;
//  AuditServerInterceptor::_exec_principal_auth ((const char *)s);
#endif /* USE_CSL2  */

    return TRUE;
}

void
MICOSSL::SSLTransport::rselect (CORBA::Dispatcher *disp,
			     CORBA::TransportCallback *cb)
{
    _rcb = cb;
    _transp->rselect (disp, cb ? this : 0);
}

#ifdef USE_CSL2
//called from authenticate of PrincipalAuthenticator_impl
CORBA::Boolean
MICOSSL::SSLTransport::setup_ctx
(char* mechanism, Security::SSLKeyCertCAPass& method_struct)
{
    if (_ssl_ctx)
	return TRUE;
    CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb", FALSE);
    CORBA::Object_var policy_current_obj = orb->resolve_initial_references
	("PolicyCurrent");
    SecurityLevel2::PolicyCurrent_var policy_current
	= SecurityLevel2::PolicyCurrent::_narrow(policy_current_obj);
    assert (!CORBA::is_nil (policy_current));
    int verify = SSL_VERIFY_NONE;
    CORBA::PolicyTypeSeq policy_types;
    policy_types.length(1);
    policy_types[0] = Security::SecEstablishTrustPolicy;

    CORBA::PolicyList* policy_list
	= policy_current -> get_policy_overrides(policy_types);
    //MICOSL2::EstablishTrustPolicy_impl estab_tr_pol 
    // = (MICOSL2::EstablishTrustPolicy_impl)policy_list[0];

    CORBA::Policy_ptr policy = (*policy_list)[0];
    //MICO::Policy_impl policy = (*policy_list)[0];
    SecurityLevel2::EstablishTrustPolicy_ptr estab_tr_pol
	= SecurityLevel2::EstablishTrustPolicy::_narrow(policy);

    if ((estab_tr_pol ->trust()).trust_in_client)
	verify = SSL_VERIFY_PEER;
    if ((estab_tr_pol -> trust()).trust_in_target)
	verify = SSL_VERIFY_PEER| SSL_VERIFY_FAIL_IF_NO_PEER_CERT;

    if (verify == SSL_VERIFY_NONE)
	_ssl_verify_depth = 0;//9;

    const char *cert = 0, *key = 0, *cafile = 0, *capath = 0, *cipher = 0;
    cert = CORBA::string_dup(method_struct.cert);
    key = CORBA::string_dup(method_struct.key);
    cafile = CORBA::string_dup(method_struct.CAfile);
    capath = CORBA::string_dup(method_struct.CAdir);
    cipher = CORBA::string_dup(mechanism);

    if (*cert == 0)
	cert = 0;
    else if (access(cert,0) == -1 ) {
	cout << "Could not find " << cert << " file" << endl;
	mico_throw(CORBA::BAD_PARAM());
    }

    if (*key == 0)
	key = 0;
    else if (access(key,0) == -1) {
	cout << "Could not find " << key << " file" << endl;
	mico_throw(CORBA::BAD_PARAM());
    }

    if (*cafile == 0)
	cafile = cert;
    else if (access(cafile, 0) == -1) {
	cout << "Could not find " << cafile << " file" << endl;
	mico_throw(CORBA::BAD_PARAM());
    }

    if (*capath == 0)
	capath = 0;
    else if ( access(capath, 0) == -1) {
	cout << "Could not find " << capath << " file" << endl;
	mico_throw(CORBA::BAD_PARAM());
    }

    if (*cipher == 0)
	cipher = 0;

    if (!cipher)
	cipher = getenv ("SSL_CIPHER");
    if (!cert)
	cert = "default.pem";
    if (!key)
	key = cert;

    SSLeay_add_ssl_algorithms ();
    _ssl_ctx = SSL_CTX_new (SSLv23_method());

    if (cipher)
	SSL_CTX_set_cipher_list (_ssl_ctx, (char *)cipher);
    SSL_CTX_set_verify (_ssl_ctx, verify, ssl_verify_callback);

    SSL_CTX_set_client_CA_list (_ssl_ctx,
				SSL_load_client_CA_file ((char *)cafile));

    if (SSL_CTX_use_certificate_file (_ssl_ctx, (char *)cert,
				      SSL_FILETYPE_PEM) <= 0) {
	//	MICODebug::instance()->printer()
	//	    << "cannot set SSL certificate file: " << cert << endl;
	return FALSE;
    }
    if (SSL_CTX_use_PrivateKey_file (_ssl_ctx, (char *)key,
				     SSL_FILETYPE_PEM) <= 0) {
	//	MICODebug::instance()->printer()
	//	    << "cannot set SSL private key file: " << key << endl;
	return FALSE;
    }
    if (SSL_CTX_check_private_key (_ssl_ctx) <= 0) {
	//	MICODebug::instance()->printer()
	//	    << "bad private key" << endl;
	return FALSE;
    }

    SSL_load_error_strings ();

    int ret = SSL_CTX_load_verify_locations (_ssl_ctx, (char *)cafile,
					     (char *)capath);

    int default_ret =  SSL_CTX_set_default_verify_paths (_ssl_ctx);

    if ( ((!ret) && (capath || cafile)) || (!default_ret)) {
	//		MICODebug::instance()->printer()
	//		<< "cannot set SSL verify locations" << endl;
	return FALSE;
    }
    return TRUE;
}
// ###ras
X509* MICOSSL::SSLTransport::load_cert(const char *certfile)
{
    FILE *fp = fopen (certfile, "r");
    X509 *x509;

    if (!fp)
	return NULL;

    x509 = (X509 *)PEM_ASN1_read ((char *(*)())d2i_X509,
				  PEM_STRING_X509,
				  fp, NULL, NULL,NULL);

    if (x509 == NULL) {
	//ERR_print_errors_fp (stderr);
	return NULL;
    }

    fclose (fp);
    return x509;
}
#endif /* USE_CSL2  */

void
MICOSSL::SSLTransport::wselect (CORBA::Dispatcher *disp,
			     CORBA::TransportCallback *cb)
{
    _wcb = cb;
    _transp->wselect (disp, cb ? this : 0);
}

void
MICOSSL::SSLTransport::callback (CORBA::Transport *, CORBA::TransportCallback::Event ev)
{
    switch (ev) {
    case CORBA::TransportCallback::Read:
	_rcb->callback (this, ev);
	break;

    case CORBA::TransportCallback::Write:
	_wcb->callback (this, ev);
	break;

    case CORBA::TransportCallback::Remove:
	if (_rcb)
	    _rcb->callback (this, ev);
	if (_wcb)
	    _wcb->callback (this, ev);
	_rcb = _wcb = 0;
	break;
    }
}

CORBA::Boolean
MICOSSL::SSLTransport::bind (const CORBA::Address *a)
{
    assert (!strcmp (a->proto(), "ssl"));
    SSLAddress *sa = (SSLAddress *)a;

    CORBA::Boolean r = _transp->bind (sa->content());
    if (!r) {
	_err = _transp->errormsg();
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
MICOSSL::SSLTransport::connect (const CORBA::Address *a)
{
    assert (!strcmp (a->proto(), "ssl"));
    SSLAddress *sa = (SSLAddress *)a;

    CORBA::Boolean blocking = _transp->isblocking();
    _transp->block (TRUE);
    CORBA::Boolean r = _transp->connect (sa->content());
    if (!r) {
	_err = _transp->errormsg();
        _transp->block (blocking);
	return FALSE;
    }
#if SSLEAY_VERSION_NUMBER >= 0x0900
    SSL_set_connect_state (_ssl);
#endif
    int i = SSL_connect (_ssl);
    _transp->block (blocking);
    if (i <= 0) {
	_err = "SSL active connection setup failed";
	return FALSE;
    }
    _closed = FALSE;
    return TRUE;
}

CORBA::Boolean
MICOSSL::SSLTransport::accept ()
{
    CORBA::Boolean blocking = _transp->isblocking();
    _transp->block (TRUE);
    int i = SSL_accept (_ssl);
    _transp->block (blocking);
    if (i <= 0) {
	_err = "SSL passive connection setup failed";
	return FALSE;
    }
    _closed = FALSE;
    return TRUE;
}

void
MICOSSL::SSLTransport::close ()
{
#ifdef HAVE_THREADS
    MICOMT::AutoLock lock(_ssl_mutex);
#endif // HAVE_THREADS
    // kcg: we need to block to correctly complete SSL_shutdown
    // otherwise we will need to use select to watch its completion
    _transp->block(TRUE);
    if (SSL_shutdown(_ssl) == 0) {
	// bi-direction SSL shutdown is not yet completed
	long ret = SSL_shutdown(_ssl);
	// ret value should be either 1 in case of success
	// or 0 in case of already closed connection
	// (i.e. client exists before closing connection properly)
	assert(ret == 1 || ret == 0);
    }
    _transp->close ();
    _closed = TRUE;
}

void
MICOSSL::SSLTransport::block (CORBA::Boolean doblock)
{
    _transp->block (doblock);
}

CORBA::Boolean
MICOSSL::SSLTransport::isblocking ()
{
    return _transp->isblocking();
}

CORBA::Boolean
MICOSSL::SSLTransport::isreadable ()
{
    // XXX if transport is readable that doesnt mean that SSL_read()
    // can read something ...
    return _transp->isreadable();
}

CORBA::Long
MICOSSL::SSLTransport::read (void *_b, CORBA::Long len)
{
#ifdef HAVE_THREADS
    // We can't lock while using blocking transport (blocking is used only
    // by thread per connection concurrency model) because it block on read
    // and it can lead to deadlock ie. reader will be faster then writer,
    // it will lock _ssl_mutex and then block on read without chance to unlock
    // since writter doesn't write it's request
    if (!this->isblocking())
	_ssl_mutex.lock();
#endif // HAVE_THREADS
    int i = SSL_read (_ssl, (char *)_b, len);
    if (i < 0)
	_err = _transp->errormsg();
    if (i == 0) {
	// maybe clean shutdown?
	if (SSL_get_shutdown(_ssl) == SSL_RECEIVED_SHUTDOWN) {
	    // we have received close notify alert from the peer so we will
	    // reply to it by calling SSL_shutdown
	    long ret = SSL_shutdown(_ssl);
	    assert(ret == 1);
	}
    }
#ifdef HAVE_THREADS
    // see comment above
    if (!this->isblocking())
	_ssl_mutex.unlock();
#endif // HAVE_THREADS
    return i;
}

CORBA::Long
MICOSSL::SSLTransport::write (const void *_b, CORBA::Long len)
{
#ifdef HAVE_THREADS
    MICOMT::AutoLock lock(_ssl_mutex);
#endif // HAVE_THREADS
    int i = SSL_write (_ssl, (char *)_b, len);
    if (i < 0) {
	cerr << "SSL error handling" << endl;
	CORBA::Long error = SSL_get_error(_ssl, i);
	cerr << "SSL error: " << error << endl;
	if (error == SSL_ERROR_WANT_WRITE) {
	    cerr << "SSL_ERROR_WANT_WRITE" << endl;
	}
	ERR_print_errors_fp(stderr);
	assert(0);
    }
    if (i < 0)
	_err = _transp->errormsg();
    return i;
}

const CORBA::Address *
MICOSSL::SSLTransport::addr ()
{
    _local_addr->content (_transp->addr()->clone());
    return _local_addr;
}

const CORBA::Address *
MICOSSL::SSLTransport::peer ()
{
    _peer_addr->content (_transp->peer()->clone());
    return _peer_addr;
}

CORBA::Boolean
MICOSSL::SSLTransport::bad () const
{
    return _err.length() > 0;
}

CORBA::Boolean
MICOSSL::SSLTransport::eof () const
{
    return (SSL_get_shutdown(_ssl) == (SSL_RECEIVED_SHUTDOWN|SSL_SENT_SHUTDOWN))
	||_transp->eof();
}

string
MICOSSL::SSLTransport::errormsg () const
{
    return _err;
}

CORBA::Principal_ptr
MICOSSL::SSLTransport::get_principal ()
{
    return new SSLPrincipal (SSL_get_peer_certificate (_ssl),
			     SSL_get_cipher (_ssl), this);
}


/************************ SSLTransportServer **************************/


MICOSSL::SSLTransportServer::SSLTransportServer (const SSLAddress *a)
{
    CORBA::TransportServer* t_serv = a->content()->make_transport_server();
    _server = dynamic_cast<MICO::SocketTransportServer*>(t_serv);
    assert(_server != NULL);
    _local_addr = (SSLAddress *)a->clone();
    _acb = 0;
}

MICOSSL::SSLTransportServer::~SSLTransportServer ()
{
    CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb", FALSE);
    _server->aselect (orb->dispatcher(), 0);
    _acb = 0;

    delete _server;
    delete _local_addr;
}

void
MICOSSL::SSLTransportServer::listen()
{
    _server->listen();
}

void
MICOSSL::SSLTransportServer::aselect (CORBA::Dispatcher *disp,
				   CORBA::TransportServerCallback *cb)
{
#ifdef HAVE_THREADS
    if (MICO::MTManager::thread_per_connection())
	MICO::SocketTransportServer::aselect(disp, cb);
#endif // HAVE_THREADS
    _acb = cb;
    _server->aselect (disp, cb ? this : 0);
}

void
MICOSSL::SSLTransportServer::callback (CORBA::TransportServer *, CORBA::TransportServerCallback::Event ev)
{
    switch (ev) {
    case CORBA::TransportServerCallback::Accept:
	_acb->callback (this, ev);
	break;

    case CORBA::TransportServerCallback::Remove:
	_acb->callback (this, ev);
	_acb = 0;
	break;
    }
}

CORBA::Boolean
MICOSSL::SSLTransportServer::bind (const CORBA::Address *a)
{
    assert (!strcmp (a->proto(), "ssl"));
    SSLAddress *sa = (SSLAddress *)a;

    if (!_server->bind (sa->content())) {
        _err = _server->errormsg();
        return FALSE;
    }
    return TRUE;
}

void
MICOSSL::SSLTransportServer::close ()
{
    _server->close ();
}

void
MICOSSL::SSLTransportServer::block (CORBA::Boolean doblock)
{
    _server->block (doblock);
}

CORBA::Boolean
MICOSSL::SSLTransportServer::isblocking ()
{
    return _server->isblocking();
}

CORBA::Transport *
MICOSSL::SSLTransportServer::accept ()
{
    CORBA::Transport *t = _server->accept();
    if (!t)
        return 0;
    SSLTransport *st = new SSLTransport (_local_addr, t);
    st->accept ();
    return st;
}

const CORBA::Address *
MICOSSL::SSLTransportServer::addr ()
{
    _local_addr->content (_server->addr()->clone());
    return _local_addr;
}

CORBA::Boolean
MICOSSL::SSLTransportServer::bad () const
{
    return _err.length() > 0;
}

string
MICOSSL::SSLTransportServer::errormsg () const
{
    return _err;
}


/****************************** SSLProfile *****************************/

/*
 * the next function is the only one that has to be changed
 * to add support for SSL over a new low level transport.
 */

static CORBA::IORProfile::ProfileId
tag_to_ssltag (CORBA::IORProfile::ProfileId tag)
{
    switch (tag) {
    case CORBA::IORProfile::TAG_INTERNET_IOP:
        return CORBA::IORProfile::TAG_SSL_INTERNET_IOP;
    case CORBA::IORProfile::TAG_UNIX_IOP:
        return CORBA::IORProfile::TAG_SSL_UNIX_IOP;
    case CORBA::IORProfile::TAG_UDP_IOP:
        return CORBA::IORProfile::TAG_SSL_UDP_IOP;
    default:
        assert (0);
    }
    return 0;
}

MICOSSL::SSLProfile::SSLProfile (CORBA::Octet *o, CORBA::ULong l,
				 const SSLAddress &ia,
				 const CORBA::MultiComponent &mc,
                                 CORBA::UShort version)
    : _myaddr (ia)
{
    // copy port from IIOP profile to SSLComponent
    CORBA::UShort port = 0;
    if (strcmp (_myaddr.content()->proto(), "inet") == 0)
	port = ((MICO::InetAddress *)_myaddr.content())->port();

    CORBA::MultiComponent mc2 (mc);
    mc2.add_component (new SSLComponent (port));
    _prof = ia.content()->make_ior_profile (o, l, mc2, version);
}

MICOSSL::SSLProfile::SSLProfile (CORBA::IORProfile *ior, const SSLAddress &ia)
    : _myaddr (ia)
{
    _prof = ior;
    CORBA::Boolean port_assigned = FALSE;
#ifdef USE_CSIV2
    // we need to assign CSIv2 component port to profile's address' port
    // which is as spec said 0 at the moment
    CORBA::MultiComponent* comps = ior->components();
    if (comps->component(CSIIOP::TAG_CSI_SEC_MECH_LIST)) {
	CSIv2::Component* c = NULL;
	c = dynamic_cast<CSIv2::Component*>
	    (comps->component(CSIIOP::TAG_CSI_SEC_MECH_LIST));
	assert(c != NULL);
	CSIIOP::CompoundSecMechList* clist = c->mech_list();
	if (clist->mechanism_list[0].transport_mech.tag == CSIIOP::
	    TAG_TLS_SEC_TRANS) {
	    MICO::InetAddress* i_addr = dynamic_cast<MICO::InetAddress*>
		(const_cast<CORBA::Address*>(_myaddr.content()));
	    try {
		CORBA::ORB_var orb = CORBA::ORB_instance
		    ("mico-local-orb", FALSE);
		CORBA::Object_var obj = orb->resolve_initial_references
		    ("CSIv2SecurityManager");
		CSIv2::SecurityManager_var sec_man
		    = CSIv2::SecurityManager::_narrow(obj);
		assert(!CORBA::is_nil(sec_man));
		if (sec_man->csiv2())
		    if (MICO::Logger::IsLogged(MICO::Logger::Security))
			MICO::Logger::Stream(MICO::Logger::Security)
			    << "CSIv2 enabled" << endl;
		IOP::TaggedComponent trans_mech_comp
		    = clist->mechanism_list[0].transport_mech;
		IOP::Codec_ptr codec = sec_man->codec();
		CORBA::Any* t_any = new CORBA::Any;
		CSIIOP::TLS_SEC_TRANS trans_mech;
		(*t_any) <<= trans_mech;
		t_any = codec->decode_value
		    (trans_mech_comp.component_data, t_any->type());
		(*t_any) >>= trans_mech;
		if (MICO::Logger::IsLogged(MICO::Logger::Security))
		    MICO::Logger::Stream(MICO::Logger::Security)
			<< "reassigning port "
			<< trans_mech.addresses[0].port
			<< " from CSIv2 comp into IIOP profile" << endl;
		i_addr->port(trans_mech.addresses[0].port);
		port_assigned = TRUE;
	    } catch (CORBA::Exception& ex) {
		if (MICO::Logger::IsLogged(MICO::Logger::Security))
		    MICO::Logger::Stream(MICO::Logger::Security)
			<< "catch ex: " << ex._repoid() << endl;
	    }
	}
    }
#endif // USE_CSIV2
    // use port from SSLComponent instead of port from IIOP profile ...
    if ((!port_assigned)
	&& (strcmp (_myaddr.content()->proto(), "inet") == 0)) {
	assert (ior->id() == CORBA::IORProfile::TAG_INTERNET_IOP);
	CORBA::MultiComponent *mc = ior->components();
	CORBA::Component *c =
	    mc->component (CORBA::Component::TAG_SSL_SEC_TRANS);
	assert (c);
	CORBA::UShort port =  ((SSLComponent *)c)->port();
	((MICO::InetAddress *)_myaddr.content())->port (port);
    }
}

MICOSSL::SSLProfile::SSLProfile (const SSLProfile &ip)
    : _myaddr (ip._myaddr)
{
    _prof = ip._prof->clone();
}

MICOSSL::SSLProfile::~SSLProfile ()
{
    delete _prof;
}

MICOSSL::SSLProfile &
MICOSSL::SSLProfile::operator= (const SSLProfile &ip)
{
    if (this != &ip) {
        delete _prof;
        _prof = ip._prof->clone();
        _myaddr = ip._myaddr;
    }
    return *this;
}

void
MICOSSL::SSLProfile::encode (CORBA::DataEncoder &ec) const
{
#ifdef USE_CSIV2
    // here we have the only one chance to 
    // tweak port in TAG_CSI_SEC_MECH_LIST
    // component
    CORBA::Address* t_a = const_cast<CORBA::Address*>
	(_prof->addr());
    //MICO::InetAddress* iaddr = dynamic_cast<MICO::InetAddress*>
    //(_prof->addr());
    MICO::InetAddress* iaddr = dynamic_cast<MICO::InetAddress*>
	(t_a);
    assert(iaddr != NULL);
    if (iaddr->port() != 0) {
	// the first call of this method => we need to change CSIv2 component
	// and set port in profile to 0
	CORBA::MultiComponent* comps = _prof->components();
	CORBA::Component* sec_comp = NULL;
	sec_comp = comps->component(CSIIOP::TAG_CSI_SEC_MECH_LIST);
	if (sec_comp != NULL) {
	    if (MICO::Logger::IsLogged(MICO::Logger::Security))
		MICO::Logger::Stream(MICO::Logger::Security)
		    << "Found CSI::TAG_CSI_SEC_MECH_LIST component while encoding"
		    << " SSL profile" << endl;
	    CSIv2::Component* csiv2_comp = dynamic_cast<CSIv2::Component*>
		(sec_comp);
	    if (csiv2_comp != NULL) {
		//cerr << "CSIv2 component sucesfully downcasted" << endl;
		try {
		    CORBA::ORB_var orb = CORBA::ORB_instance
			("mico-local-orb", FALSE);
		    CORBA::Object_var obj = orb->resolve_initial_references
			("CSIv2SecurityManager");
		    CSIv2::SecurityManager_var sec_man
			= CSIv2::SecurityManager::_narrow(obj);
		    assert(!CORBA::is_nil(sec_man));
		    //if (sec_man->csiv2())
		    //cerr << "CSIv2 enabled" << endl;
		    CSIIOP::CompoundSecMechList* clist
			= csiv2_comp->mech_list();
		    CSIIOP::TransportAddress addr;
		    addr.host_name = iaddr->host();
		    addr.port = iaddr->port();
		    iaddr->port(0);
		    if (MICO::Logger::IsLogged(MICO::Logger::Security))
			MICO::Logger::Stream(MICO::Logger::Security)
			    << "addr: " << addr.host_name << ":"
			    << addr.port << endl;
		    CSIIOP::TransportAddressList addr_list;
		    addr_list.length(1);
		    addr_list[0] = addr;
		    CSIIOP::TLS_SEC_TRANS trans;
		    MICOSSL::SSLComponent* ssl_comp = NULL;
		    ssl_comp = dynamic_cast<MICOSSL::SSLComponent*>
			(comps->component(CORBA::Component::TAG_SSL_SEC_TRANS));
		    if (ssl_comp != NULL) {
			if (MICO::Logger::IsLogged(MICO::Logger::Security))
			    MICO::Logger::Stream(MICO::Logger::Security)
				<< "getting support/requires from SSL component"
				<< endl;
			trans.target_supports = ssl_comp->target_supports();
			trans.target_requires = ssl_comp->target_requires();
		    }
		    else {
			trans.target_supports = 102;
			trans.target_requires = 70;
		    }
		    comps->del_component(ssl_comp);
		    trans.addresses = addr_list;
		    IOP::TaggedComponent comp;
		    comp.tag = CSIIOP::TAG_TLS_SEC_TRANS;
		    IOP::Codec_ptr codec = sec_man->codec();
		    CORBA::Any t_any;
		    t_any <<= trans;
		    CORBA::OctetSeq* data = codec->encode_value(t_any);
		    if (MICO::Logger::IsLogged(MICO::Logger::Security)) {
			MICO::Logger::Stream(MICO::Logger::Security)
			    << "encoded trans: ";
			for (CORBA::ULong i=0; i<data->length(); i++)
			    MICO::Logger::Stream(MICO::Logger::Security)
				<< (unsigned short)(*data)[i] << " ";
			MICO::Logger::Stream(MICO::Logger::Security)
			    << endl;
		    }
		    comp.component_data = (*data);
		    CSIIOP::AssociationOptions opt = 0;
		    for (CORBA::ULong i=0;
			 i<clist->mechanism_list.length();
			 i++) {
			clist->mechanism_list[i].transport_mech = comp;
			opt = trans.target_requires;
			if (clist->mechanism_list[i].as_context_mech.target_supports != 0)
			    opt = opt|clist->mechanism_list[i].as_context_mech.target_requires;
			if (clist->mechanism_list[i].sas_context_mech.target_supports != 0)
			    opt = opt|clist->mechanism_list[i].sas_context_mech.target_requires;
			clist->mechanism_list[i].target_requires = opt;
		    }
		} catch (CORBA::ORB::InvalidName_catch&) {
		    cerr << "CSIv2 disabled" << endl;
		    assert(0);
		}
	    }
	    else {
		cerr << "unsuccessfull downcasting :-((" << endl;
		assert(0);
	    }
	}
    }
#endif
    _prof->encode (ec);
}

const CORBA::Address *
MICOSSL::SSLProfile::addr () const
{
    return &_myaddr;
}

MICOSSL::SSLProfile::ProfileId
MICOSSL::SSLProfile::id () const
{
    return tag_to_ssltag (_prof->id());
}

MICOSSL::SSLProfile::ProfileId
MICOSSL::SSLProfile::encode_id () const
{
    return _prof->id();
}

void
MICOSSL::SSLProfile::objectkey (CORBA::Octet *o, CORBA::Long l)
{
    _prof->objectkey (o, l);
}

const CORBA::Octet *
MICOSSL::SSLProfile::objectkey (CORBA::Long &l) const
{
    return _prof->objectkey (l);
}

CORBA::Boolean
MICOSSL::SSLProfile::reachable ()
{
    return _prof->reachable ();
}

void
MICOSSL::SSLProfile::print (ostream &o) const
{
    o << "SSL ";
    _prof->print (o);
}

CORBA::MultiComponent*
MICOSSL::SSLProfile::components()
{
    assert(_prof != NULL);
    return _prof->components();
}

CORBA::IORProfile *
MICOSSL::SSLProfile::clone () const
{
    return new SSLProfile (*this);
}

CORBA::Long
MICOSSL::SSLProfile::compare (const CORBA::IORProfile &p) const
{
    if (p.id() != id())
	return (CORBA::Long)id() - (CORBA::Long)p.id();

    const SSLProfile &sp = (const SSLProfile &)p;

    CORBA::Long r = _myaddr.compare (sp._myaddr);
    if (r)
	return r;

    return _prof->compare (*sp._prof);
}

CORBA::Boolean
MICOSSL::SSLProfile::operator== (const CORBA::IORProfile &p) const
{
    return compare (p) == 0;
}

CORBA::Boolean
MICOSSL::SSLProfile::operator< (const CORBA::IORProfile &p) const
{
    return compare (p) < 0;
}

CORBA::UShort
MICOSSL::SSLProfile::iiop_version() const
{
    MICO::IIOPProfile* iiop_prof = dynamic_cast<MICO::IIOPProfile*>(this->_prof);
    if (iiop_prof != NULL) {
	return iiop_prof->iiop_version();
    }
    return 0;
}

/*************************** SSLProfileDecoder *************************/


/*
 * unfortunately SSL does not have its own IOR profile, but is
 * part of the IIOP profile. Therefore we cannot use our own
 * profile decoder (the below code) but have to insert special
 * checks into the IIOP profile decoder code. Except this
 * unavoidable uglyness the SSL code is completely independent
 * from the rest of MICO.
 */

#if 0
MICOSSL::SSLProfileDecoder::SSLProfileDecoder ()
{
    CORBA::IORProfile::register_decoder (this);
}

MICOSSL::SSLProfileDecoder::~SSLProfileDecoder ()
{
    CORBA::IORProfile::unregister_decoder (this);
}

CORBA::IORProfile *
MICOSSL::SSLProfileDecoder::decode (CORBA::DataDecoder &dc, ProfileId pid,
				    CORBA::ULong len) const
{
    ProfileId orig_pid = ssltag_to_tag (pid);
    CORBA::IORProfile *ior =
        CORBA::IORProfile::decode_body (dc, orig_pid, len);
    if (!ior)
        return 0;

    SSLAddress sa (ior->addr()->clone());
    return new SSLProfile (ior, sa);
}

CORBA::Boolean
MICOSSL::SSLProfileDecoder::has_id (MICOSSL::SSLProfile::ProfileId id) const
{
    return supported_ssltag (id);
}

static MICOSSL::SSLProfileDecoder ssl_ior_decoder;
#endif

/***************************** SSLPrincipal **************************/


MICOSSL::SSLPrincipal::SSLPrincipal (X509 *peer, const char *cipher,
                                     CORBA::DataDecoder &dc,
                                     CORBA::Transport *t)
    : CORBA::Principal (dc, t), _peer (peer),
      _ssl_cipher (cipher ? cipher : "")
{
}

MICOSSL::SSLPrincipal::SSLPrincipal (X509 *peer, const char *cipher,
                                     CORBA::Transport *t)
    : CORBA::Principal (t), _peer (peer), _ssl_cipher (cipher ? cipher : "")
{
}

MICOSSL::SSLPrincipal::~SSLPrincipal ()
{
    if (_peer)
	X509_free (_peer);
}

CORBA::Principal::PropertyNameList_ptr
MICOSSL::SSLPrincipal::list_properties ()
{
    PropertyNameList_ptr pl = CORBA::Principal::list_properties ();
    CORBA::ULong olen = pl->length();
    pl->length (olen+3);

    (*pl)[olen++] = (const char *)"ssl-x509-subject";
    (*pl)[olen++] = (const char *)"ssl-x509-issuer";
    (*pl)[olen++] = (const char *)"ssl-cipher";

    return pl;
}

#ifdef USE_CSL2
CORBA::Boolean MICOSSL::SSLPrincipal::check(string& str)
{
    ASN1_UTCTIME* time = X509_get_notAfter (_peer);

    str = (char *)time->data;
    int before_ret = X509_cmp_current_time(X509_get_notBefore (_peer));
    int after_ret = X509_cmp_current_time(time);

    if ((before_ret < 0) && (after_ret > 0))
	return TRUE;
    return FALSE;
}
#endif /* USE_CSL2  */

string
MICOSSL::SSLPrincipal::get_x509_entry (X509_NAME *name, const char *entry)
{
    const char *cp = strchr (entry, ':');
    if (!cp) {
	char *cp = X509_NAME_oneline (name, NULL, 0);
	string s = cp;
	::free (cp);
	return s;
    }
    int nid = OBJ_txt2nid ((char *)cp+1);
    if (nid == NID_undef)
	return string();

    char buf[1000];
    if (X509_NAME_get_text_by_NID (name, nid, buf, sizeof (buf)) < 0)
	return string();

    return string (buf);
}


#ifdef USE_CSL2
CORBA::Any*
MICOSSL::SSLPrincipal::get_attribute(unsigned long fam, unsigned long attr)
{
    switch(fam) {
    case 0: {
	return this->get_property("ssl-x509-subject");
    }
    case 1:
    	switch(attr) {
	case 1:
	    break; // never used
	case 2:
	    return this->get_property("ssl-x509-subject");
	case 3:
	    return this->get_property("ssl-x509-subject:OU");
	case 4:
	    return this->get_property("ssl-x509-subject:O");
	default: {
		mico_throw(::CORBA::BAD_PARAM	());
				/*
				  CORBA::Any *ret = new CORBA::Any;
				  *ret <<= "";
				  return ret;*/
	    }
	}
    case 10:
	switch(attr) {
	case 1:
	    return this->get_property("ssl-x509-subject");
	case 2:
	    return this->get_property("ssl-x509-issuer");
	case 3:
	    return this->get_property("ssl-cipher");
	case 4:
	    return this->get_property("ssl-x509-subject:CN");
	case 5:
	    return this->get_property("ssl-x509-subject:C");
	case 6:
	    return this->get_property("ssl-x509-subject:L");
	case 7:
	    return this->get_property("ssl-x509-subject:ST");
	case 8:
	    return this->get_property("ssl-x509-subject:O");
	case 9:
	    return this->get_property("ssl-x509-subject:OU");
	case 10:
	    return this->get_property("ssl-x509-issuer:CN");
	case 11:
	    return this->get_property("ssl-x509-issuer:C");
	case 12:
	    return this->get_property("ssl-x509-issuer:L");
	case 13:
	    return this->get_property("ssl-x509-issuer:ST");
	case 14:
	    return this->get_property("ssl-x509-issuer:O");
	case 15:
	    return this-> get_property("ssl-x509-issuer:OU");
	}
	break;
    case 11:
	switch(attr) {
	case 1:
	    return this->get_property("auth-method");
	case 2:
	    return this->get_property("peer-address");
	}
    default:
	// Raise appropriate exception
	mico_throw(::CORBA::BAD_PARAM());
	break;
    }
    // never reached
    return NULL;
}
#endif /* USE_CSL2  */


CORBA::Any*
MICOSSL::SSLPrincipal::get_property (const char *prop_name)
{
    if (!strcmp ("auth-method", prop_name)) {
	CORBA::Any *a = new CORBA::Any;
	*a <<= "ssl";
	return a;
    }
    if (!strncmp ("ssl-x509-subject", prop_name, strlen("ssl-x509-subject"))) {
	CORBA::Any *a = new CORBA::Any;
	if (_peer) {
	    string s = get_x509_entry (X509_get_subject_name (_peer),
				       prop_name);
	    *a <<= s.c_str();
	} else {
	    *a <<= "";
	}
	return a;
    }
    if (!strncmp ("ssl-x509-issuer", prop_name, strlen ("ssl-x509-issuer"))) {
	CORBA::Any *a = new CORBA::Any;
	if (_peer) {
	    string s = get_x509_entry (X509_get_issuer_name (_peer),
				       prop_name);
	    *a <<= s.c_str();
	} else {
	    *a <<= "";
	}
	return a;
    }
    if (!strcmp ("ssl-cipher", prop_name)) {
	CORBA::Any *a = new CORBA::Any;
	*a <<= _ssl_cipher.c_str();
	return a;
    }
    return CORBA::Principal::get_property (prop_name);
}


/***************************** SSLComponent **************************/

#if 0
MICOSSL::SSLComponent::SSLComponent (CORBA::UShort port)
    : _port (port)
{
    // this is a guess, dont know how to setup these correctly
    _target_supports =
        Security::Integrity|
        Security::Confidentiality|
        Security::EstablishTrustInTarget|
        Security::EstablishTrustInClient;
    _target_requires = 0;
}
#endif
MICOSSL::SSLComponent::SSLComponent (
    CORBA::UShort port,
    Security::AssociationOptions target_supports,
    Security::AssociationOptions target_requires)
    : _target_supports (target_supports),
      _target_requires (target_requires),
      _port (port)
{
}

MICOSSL::SSLComponent::~SSLComponent ()
{
}

void
MICOSSL::SSLComponent::encode (CORBA::DataEncoder &ec) const
{
    ec.struct_begin();
    {
	// MICO_SSL_VB_COMPAT ...
	if (sizeof (Security::AssociationOptions) == sizeof (CORBA::ULong)) {
	    ec.put_ulong ((CORBA::ULong&)_target_supports);
	    ec.put_ulong ((CORBA::ULong&)_target_requires);
	} else {
	    ec.put_ushort ((CORBA::UShort&)_target_supports);
	    ec.put_ushort ((CORBA::UShort&)_target_requires);
	}
	ec.put_ushort (_port);
    }
    ec.struct_end();
}

CORBA::Component::ComponentId
MICOSSL::SSLComponent::id () const
{
    return TAG_SSL_SEC_TRANS;
}

void
MICOSSL::SSLComponent::print (ostream &o) const
{
  o << " SSL:"
    << " supports 0x" << hex << _target_supports
    << " requires 0x" << hex << _target_requires
    << " port " << dec << _port << endl;
}

CORBA::Component *
MICOSSL::SSLComponent::clone () const
{
    return new SSLComponent (*this);
}

CORBA::Long
MICOSSL::SSLComponent::compare (const CORBA::Component &c) const
{
    if (id() != c.id())
	return (CORBA::Long)id() - (CORBA::Long)c.id();

    const SSLComponent &sc = (const SSLComponent &)c;

    CORBA::Long r = (CORBA::Long)_port - (CORBA::Long)sc._port;
    if (r)
	return r;

    r = (CORBA::Long)_target_supports - (CORBA::Long)sc._target_supports;
    if (r)
	return r;

    return (CORBA::Long)_target_requires - (CORBA::Long)sc._target_requires;
}

CORBA::Boolean
MICOSSL::SSLComponent::operator== (const CORBA::Component &c) const
{
    return compare (c) == 0;
}

CORBA::Boolean
MICOSSL::SSLComponent::operator< (const CORBA::Component &c) const
{
    return compare (c) < 0;
}


/************************** SSLComponentDecoder ************************/


MICOSSL::SSLComponentDecoder::SSLComponentDecoder ()
{
    CORBA::Component::register_decoder (this);
}

MICOSSL::SSLComponentDecoder::~SSLComponentDecoder ()
{
    CORBA::Component::unregister_decoder (this);
}

#define check(exp) if (!(exp)) return 0;

CORBA::Component *
MICOSSL::SSLComponentDecoder::decode (CORBA::DataDecoder &dc,
				      ComponentId id, CORBA::ULong len) const
{
    Security::AssociationOptions target_supports, target_requires;
    CORBA::UShort port;

    check (dc.struct_begin ());
    {
        // MICO_SSL_VB_COMPAT ...
	if (sizeof (Security::AssociationOptions) == sizeof (CORBA::ULong)) {
	    check (dc.get_ulong ((CORBA::ULong&)target_supports));
	    check (dc.get_ulong ((CORBA::ULong&)target_requires));
	} else {
	    check (dc.get_ushort ((CORBA::UShort&)target_supports));
	    check (dc.get_ushort ((CORBA::UShort&)target_requires));
	}
	check (dc.get_ushort (port));
    }
    check (dc.struct_end ());

    return new SSLComponent (port, target_supports, target_requires);
}

#undef check

CORBA::Boolean
MICOSSL::SSLComponentDecoder::has_id (ComponentId id) const
{
    return id == CORBA::Component::TAG_SSL_SEC_TRANS;
}

static MICOSSL::SSLComponentDecoder ssl_component_decoder;


/***************************** Initialization **************************/


static class SSLInit : public Interceptor::InitInterceptor {
public:
    SSLInit ()
        : Interceptor::InitInterceptor(0)
    {
    }
    Interceptor::Status initialize (CORBA::ORB_ptr orb,
				    const char *orbid,
				    int &argc, char *argv[])
    {
	MICOGetOpt::OptMap opts;
	opts["-ORBSSLverify"]     = "arg-expected";
	opts["-ORBSSLcert"]       = "arg-expected";
	opts["-ORBSSLkey"]        = "arg-expected";
	opts["-ORBSSLCApath"]     = "arg-expected";
	opts["-ORBSSLCAfile"]     = "arg-expected";
	opts["-ORBSSLcipher"]     = "arg-expected";

	MICOGetOpt opt_parser (opts);
	CORBA::Boolean r = opt_parser.parse (orb->rcfile(), TRUE);
	assert (r);
	r = opt_parser.parse (argc, argv, TRUE);
	assert (r);
	SSL_options = opt_parser.opts ();

	return Interceptor::INVOKE_CONTINUE;
    }
} ssl_init;

void
MICOSSL::_init ()
{
  // Is it really needed? E.g. for some obsolete compilers?
  // In that case, please uncomment following line
  //ssl_init;
#ifdef USE_OPENSSL_THREAD_FUNCTIONS
  S_ssl_mutex_array = new MICOMT::Mutex[CRYPTO_num_locks()];
  S_ssl_mutex_count = new long[CRYPTO_num_locks()];
  for(int i=0; i<CRYPTO_num_locks(); i++)
      S_ssl_mutex_count[i] = 0;
  CRYPTO_set_locking_callback((void (*)(int, int, const char*, int))micomt_ssl_locking_callback);
  CRYPTO_set_id_callback((unsigned long (*)())micomt_ssl_thread_id_callback);
#endif // USE_OPENSSL_THREAD_FUNCTIONS
}

#endif // HAVE_SSL
