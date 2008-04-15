#include <CORBA.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <string>
#include <iostream>
#include <fstream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <string.h>
#include <iostream.h>
#include <fstream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <mico/template_impl.h>
#include <mico/throw.h>


using namespace std;

// variable length struct
struct SV {
    CORBA::String_var s;
};
typedef TVarVar<SV> SV_var;
typedef TVarOut<SV> SV_out;


class __stc_SV : public CORBA::StaticTypeInfo {
    typedef SV T;
public:
    StaticValueType create () const
    { return (StaticValueType)new T; }
    StaticValueType copy (const StaticValueType v) const
    { return (StaticValueType)new T (*(T *)v); }
    void assign (StaticValueType d, const StaticValueType s) const
    { *(T *)d = *(T *)s; }
    void free (StaticValueType v) const
    { delete (T *)v; }
    CORBA::Boolean demarshal (CORBA::DataDecoder &dc, StaticValueType v) const
    {
	return
	    dc.struct_begin() &&
	    CORBA::_stc_string->demarshal (dc, &((T *)v)->s.inout()) &&
	    dc.struct_end();
    }
    void marshal (CORBA::DataEncoder &ec, StaticValueType v) const
    {
	ec.struct_begin();
	CORBA::_stc_string->marshal (ec, &((T *)v)->s.inout());
	ec.struct_end();
    }
};

CORBA::StaticTypeInfo *_stc_SV = new __stc_SV;



// fixed length struct
struct SF {
    CORBA::Long l;
};
typedef TFixVar<SF> SF_var;
typedef SF&         SF_out;


class __stc_SF : public CORBA::StaticTypeInfo {
    typedef SF T;
public:
    StaticValueType create () const
    { return (StaticValueType)new T; }
    StaticValueType copy (const StaticValueType v) const
    { return (StaticValueType)new T (*(T *)v); }
    void assign (StaticValueType d, const StaticValueType s) const
    { *(T *)d = *(T *)s; }
    void free (StaticValueType v) const
    { delete (T *)v; }
    CORBA::Boolean demarshal (CORBA::DataDecoder &dc, StaticValueType v) const
    {
	return
	    dc.struct_begin() &&
	    CORBA::_stc_long->demarshal (dc, &((T *)v)->l) &&
	    dc.struct_end();
    }
    void marshal (CORBA::DataEncoder &ec, StaticValueType v) const
    {
	ec.struct_begin();
	CORBA::_stc_long->marshal (ec, &((T *)v)->l);
	ec.struct_end();
    }
};

CORBA::StaticTypeInfo *_stc_SF = new __stc_SF;



CORBA::Long
add_stub (CORBA::Object_ptr obj, CORBA::Long x, CORBA::Long y, CORBA::String_out str,
	  CORBA::Context_ptr ctx)
{
    CORBA::Long res;

    // create StaticAny's for arguments and result
    CORBA::StaticAny x_any (CORBA::_stc_long, &x);
    CORBA::StaticAny y_any (CORBA::_stc_long, &y);
    CORBA::StaticAny res_any (CORBA::_stc_long, &res);
    CORBA::StaticAny str_any (CORBA::_stc_string, &str);


    // create request
    CORBA::StaticRequest req (obj, "add");

    // set context stuff
    CORBA::ContextList_var clist = new CORBA::ContextList;
    clist->add ("a*");

    req.set_context (ctx);
    req.set_context_list (clist);

    // register arguments with StaticRequest
    req.add_in_arg (&x_any);
    req.add_in_arg (&y_any);
    req.add_out_arg (&str_any);
    req.set_result (&res_any);

    /*
     * invoke it
     *
     * exception handling is the same as for the DII, except that
     * you have to pass a StaticTypeInfo to
     * UnknownUserException::exception() instead of a TypeCode.
     */
    req.invoke ();
#ifdef HAVE_EXCEPTIONS
    if (req.exception()) {
	CORBA::Exception *ex = req.exception();
	CORBA::UnknownUserException *uuex =
	    CORBA::UnknownUserException::_downcast (ex);
	if (uuex) {
	    const char *repoid = uuex->_except_repoid();
	    /*
	     * // for each user exception ...
	     * if (!strcmp (repoid, "IDL:Except:1.0")) {
	     *     CORBA::StaticAny &sa = uuex->exception (_stc_Except);
	     *     mico_throw (*(Except *)sa.value());
	     * }
	     */
	    mico_throw (CORBA::UNKNOWN());
	} else {
	    mico_throw (*ex);
	}
    }
#else
    if (req.exception())
      CORBA::Exception::_throw_failed (req.exception());
#endif

    /*
     * no need to write back result and out args from StaticAny
     * to real arguments/result, StaticAny did it for you...
     */
    return res;
}

SF
fixed_stub (CORBA::Object_ptr obj, const SF &in, SF &inout, SF_out out)
{
    CORBA::StaticAny _in (_stc_SF, &in);
    CORBA::StaticAny _inout (_stc_SF, &inout);
    CORBA::StaticAny _out (_stc_SF, &out);
    SF _res;
    CORBA::StaticAny __res (_stc_SF, &_res);

    CORBA::StaticRequest req (obj, "fixed");
    req.add_in_arg (&_in);
    req.add_inout_arg (&_inout);
    req.add_out_arg (&_out);
    req.set_result (&__res);

    req.invoke();

#ifdef HAVE_EXCEPTIONS
    if (req.exception()) {
	CORBA::Exception *ex = req.exception();
	CORBA::UnknownUserException *uuex =
	    CORBA::UnknownUserException::_downcast (ex);
	if (uuex) {
	    const char *repoid = uuex->_except_repoid();
	    mico_throw (CORBA::UNKNOWN());
	} else {
	    mico_throw (*ex);
	}
    }
#else
    if (req.exception())
      CORBA::Exception::_throw_failed (req.exception());
#endif
    return _res;
}

SV *
variable_stub (CORBA::Object_ptr obj, const SV &in, SV &inout, SV_out out)
{
    CORBA::StaticAny _in (_stc_SV, &in);
    CORBA::StaticAny _inout (_stc_SV, &inout);
    /*
     * let the StaticAny allocate result and out params
     * of variable length data types by not passing a second argument
     */
    CORBA::StaticAny _out (_stc_SV);
    CORBA::StaticAny __res (_stc_SV);

    CORBA::StaticRequest req (obj, "variable");
    req.add_in_arg (&_in);
    req.add_inout_arg (&_inout);
    req.add_out_arg (&_out);
    req.set_result (&__res);

    req.invoke();

#ifdef HAVE_EXCEPTIONS
    if (req.exception()) {
	CORBA::Exception *ex = req.exception();
	CORBA::UnknownUserException *uuex =
	    CORBA::UnknownUserException::_downcast (ex);
	if (uuex) {
	    const char *repoid = uuex->_except_repoid();
	    mico_throw (CORBA::UNKNOWN());
	} else {
	    mico_throw (*ex);
	}
    }
#else
    if (req.exception())
      CORBA::Exception::_throw_failed (req.exception());
#endif
    /*
     * get the newly allocated out param and result from StaticAny.
     * by calling _retn() we take over responsibility for the allocated
     * storage.
     */
    out = (SV *)_out._retn();
    return (SV *)__res._retn();
}

int
main (int argc, char *argv[])
{
    CORBA::ORB_ptr orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

    CORBA::Object_var obj = orb->bind ("IDL:Calc:1.0");
    if (CORBA::is_nil (obj)) {
	cerr << "cannot bind to diiserver" << endl;
	cerr << "use -ORBBindAddr options to specify default bind addresses"
	     << endl;
	return 1;
    }


    // call add()
    {
	CORBA::Any any;
	CORBA::Context_var ctx, ctx2;
	orb->get_default_context (ctx);

	ctx->create_child ("child", ctx2);

	any <<= "aa";
	ctx->set_one_value ("aa", any);

	any <<= "ab";
	ctx->set_one_value ("ab", any);

	any <<= "bb";
	ctx->set_one_value ("bb", any);

	any <<= "aa-child";
	ctx2->set_one_value ("aa", any);

	CORBA::String_var str;
	CORBA::Long res;
	res = add_stub (obj, 1, 2, str, ctx2);

	cout << "result: " << res << endl;
	cout << str.in() << endl;
    }

    // call fixed()
    {
	SF in, out, inout, _res;
	in.l = 1;
	inout.l = 2;

	_res = fixed_stub (obj, in, inout, out);

	cout << inout.l << endl << out.l << endl << _res.l << endl << endl;
    }

    // call variable()
    {
	SV in, inout;
	SV_var out, _res;
	in.s = (const char *)"foo";
	inout.s = (const char *)"bar";

	_res = variable_stub (obj, in, inout, out);

	cout << inout.s.in() << endl << out->s.in() << endl
             << _res->s.in() << endl << endl;
    }

    return 0;
}

