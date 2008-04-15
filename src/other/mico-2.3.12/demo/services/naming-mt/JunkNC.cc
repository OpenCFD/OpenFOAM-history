
#include "JunkNC.h"

#include <stdlib.h>
#include <iostream>
#include <sstream>
using std::cerr;
using std::cout;
using std::endl;


JunkNC::
JunkNC(JunkFactory_ptr jf, 
       CosNaming::NamingContext_ptr root_nc,
       int                          client_num) :
           junk_factory_(JunkFactory::_duplicate(jf) ),
           root_nc_(CosNaming::NamingContext::_duplicate(root_nc) ),
           client_num_(client_num)
{
    nc_add_  = 0;
    nc_remove_ = 0;
}

bool
JunkNC::
traverse(CosNaming::Name&             cname, 
         CosNaming::NamingContext_ptr nc, 
         int                          level,
         int                          liter,
         bool&                        empty)
{
    empty = false;
    int len = cname.length();
    CosNaming::BindingIterator_var it;
    CosNaming::BindingList_var     bl;
    //cerr << "JunkNC::traverse:list -" << client_num_ << "-" << liter 
    //                                      << "-" << level << "-";
    //for ( int k = 0; k < cname.length(); k++) cerr << cname[k].id.in() << " ";
    //cerr << endl;
    try {
        nc->list(100, bl, it);
    } catch ( const CORBA::OBJECT_NOT_EXIST& cne) {
        cerr << "JunkNC::traverse-list -" << client_num_ << "-" << liter <<
                       "-" << level << "- OBJECT_NOT_EXIST-continuing" << endl;
        cerr << "                      -" << client_num_ << "-  ";
        for ( CORBA::ULong k = 0; k < cname.length(); k++) 
                                             cerr << cname[k].id.in() << " ";
        cerr << endl;
        return true;
    } catch ( const CORBA::SystemException& se) {
        cerr << "JunkNC::traverse-list-Unknown system exception" << endl;
        cerr << se << endl;
        return false;
    }
    if ( bl->length() == 0 ) empty = true;

    for ( CORBA::ULong i = 0; i < bl->length(); i++) {
        cname.length(len + 1);
        cname[len].id   = CORBA::string_dup((*bl)[i].binding_name[0].id);
        cname[len].kind = CORBA::string_dup((*bl)[i].binding_name[0].kind);
        CORBA::Object_var obj;
        try {
            obj = root_nc_->resolve(cname);
        } catch ( const CosNaming::NamingContext::NotFound& nf) {
            cerr << "JunkNC::traverse      -" << client_num_ << "-" << liter << 
                                                      "- NotFound" << endl;
            cerr << "                      -" << client_num_ << "-  ";
            for ( CORBA::ULong k = 0; k < cname.length(); k++) 
                                           cerr << cname[k].id.in() << " ";
            cerr << endl;
            continue;
        } catch ( const CosNaming::NamingContext::CannotProceed& cp) {
            cerr << "JunkNC::traverse      -" << client_num_ <<
                                                 "-  CannotProceed" << endl;
            return false;
        } catch ( const CosNaming::NamingContext::InvalidName& cp) {
            cerr << "JunkNC::traverse      -" << client_num_ <<
                                                 "-  InvalidName" << endl;
            return false;
        } catch ( const CORBA::OBJECT_NOT_EXIST& one) {
            cerr << "JunkNC::traverse      -" << client_num_ <<
                                   "-  OBJECT_NOT_EXIST-continuing" << endl;
            return true;
        } catch ( const CORBA::TRANSIENT& te) {
            cerr << "JunkNC::traverse      -" << client_num_ <<
                                                 "-  TRANSIENT" << endl;
            return false;
        } catch ( const CORBA::SystemException& te) {
            cerr << "JunkNC::traverse      -" << client_num_ <<
                                          "-  Unknown system exception" << endl;
            cerr << te << endl;
            return false;
        }
        if ( bl[i].binding_type ==CosNaming::ncontext) {
            CosNaming::NamingContext_var nc_t = 
                                      CosNaming::NamingContext::_narrow(obj);
            bool empty;
            if ( !traverse(cname, nc_t.in(), level + 1, liter, empty ) ) 
                                                                return false;
            if ( empty ) {
                int jsw = (int)( 5.0 * rand() /(RAND_MAX + 1.0) );
                if ( jsw <= 3 ) {
                    if ( !remove_nc( nc_t, cname, liter) ) return false;
                } else if ( jsw <= 4  && level < 9) {
                    if ( !add_nc(nc, level, liter) ) return false;
                    empty = false;
                }
            }
        }
        cname.length(len);    // restore cname
    }
    int jsw = (int) (3.0 * ::rand() / (RAND_MAX + 1.0) ) ;
    if ( jsw == 0 && bl->length() < 3 && level < 9) {
        if ( !add_nc(nc, level, liter) ) return false;
        empty = false;
    }
    return true;
}

bool
JunkNC::
remove_nc(CosNaming::NamingContext_ptr nc_t,
          CosNaming::Name&             name,
          int                          liter)
{
    //cerr << "JunkNC::remove_nc     -" << client_num_ << "-" << liter << "-"
    //                                       << " " << "-";
    //for ( int k = 0; k < name.length(); k++) cerr << name[k].id.in() << " ";
    //cerr << endl;
    try {
        nc_t->destroy();
        nc_remove_++;
    } catch ( const CosNaming::NamingContext::NotEmpty& ne ) {
        cerr << "JunkNC::remove_nc     -" << client_num_ <<
                             "-NotEmpty-Maybe someone put something in it"
                                                      << endl;
        cerr << "                      -" << client_num_ << "-  ";
        for ( CORBA::ULong k = 0; k < name.length(); k++) 
                                              cerr << name[k].id.in() << " ";
        cerr << endl;
        return true;
    } catch ( const CORBA::OBJECT_NOT_EXIST& one ) {
        cerr << "JunkNC::remove_nc     -" << client_num_ <<
                                "-OBJECT_NOT_EXIST, someone removed it" << endl;
        cerr << "                      -" << client_num_ << "-  ";
        for ( CORBA::ULong k = 0; k < name.length(); k++) 
                                     cerr << name[k].id.in() << " ";
        cerr << endl;
        return true;
    } catch ( const CORBA::SystemException& te) {
        cerr << "JunkNC::remove_nc      -" << client_num_ << 
                                        "-Unknown system exception-2" << endl;
        cerr << te << endl;
        return false;
    }
    try {
        root_nc_->unbind(name);
    } catch ( const CosNaming::NamingContext::NotFound& nf) {
        cerr << "JunkNC::remove_nc      -" << client_num_ <<
                             "-NotFound-Maybe someone got it" << endl;
        return true;
    } catch ( const CosNaming::NamingContext::CannotProceed& cp) {
        cerr << "JunkNC::remove_nc      -" << client_num_ <<
                             "-CannotProceed" << endl;
        return false;
    } catch ( const CosNaming::NamingContext::InvalidName& cp) {
        cerr << "JunkNC::remove_nc      -" << client_num_ <<
                             "-InvalidName" << endl;
        return false;
    }
    return true;
}

bool
JunkNC::
add_nc(CosNaming::NamingContext_ptr nc, int level, int liter)
{
    int m;
    try {
        m = junk_factory_->junk_count()->nc_count();
    } catch ( const CORBA::SystemException& te) {
        cerr << "JunkNC::add_nc         -" << client_num_ <<
                                        "-Unknown system exception-0" << endl;
        cerr << te << endl;
        return false;
    }
    std::ostringstream buf;
    buf << "nc_" << level << "_" << m;
    //cerr << "JunkNC::add_nc        -" << client_num_ << "-" << liter 
    //                     << "-" << level << "-" << buf.str() << endl;
    CosNaming::Name name;
    name.length(1);
    name[0].id    = CORBA::string_dup(buf.str().c_str() );
    name[0].kind  = CORBA::string_dup("");
    try {
        nc->bind_new_context(name);
        nc_add_++;
    } catch ( const CosNaming::NamingContext::AlreadyBound& ab) {
        cerr << "JunkNC::add_nc         -" << client_num_ <<
                                "-AlreadyBound-shouldn't happen" << endl;
        return false;
    } catch ( const CORBA::OBJECT_NOT_EXIST& one ) {
        cerr << "JunkNC::add_nc        -" << client_num_ << "-" << liter <<
                              "- OBJECT_NOT_EXIST, someone removed it" << endl;
    } catch ( const CORBA::SystemException& te) {
        cerr << "JunkNC::add_nc         -" << client_num_ << 
                                        "-Unknown system exception-2" << endl;
        cerr << te << endl;
        return false;
    }
    return true;
}

void
JunkNC::
report(int client_num)
{
    cerr << "JunkNc::report-" << client_num << 
           "**********************************" 
           ", nc_add = " << nc_add_ << ", nc_remove = " << nc_remove_ << endl;
}
