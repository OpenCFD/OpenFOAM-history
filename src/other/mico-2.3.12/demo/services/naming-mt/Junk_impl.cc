

#include "Junk_impl.h"

#include <fstream>
using std::cout;
using std::cerr;
using std::endl;

ORB_holder* ORB_holder::instance_ = 0;

Junk_impl::
Junk_impl(int is_new, const std::string& tag, PortableServer::POA_ptr junk_poa)
{
    tag_       = tag;
    junk_poa_  = PortableServer::POA::_duplicate(junk_poa);
    statefile_ = "db/" + tag;
    removed_   = false;
}

Junk_impl::
Junk_impl(const std::string& tag, PortableServer::POA_ptr junk_poa)
{
    tag_       = tag;
    junk_poa_  = PortableServer::POA::_duplicate(junk_poa);
    statefile_ = "db/" + tag;
    removed_   = false;

    std::ifstream in( statefile_.c_str() );
    if ( !in.rdbuf()->is_open())
                       throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_YES);
    restore(in);
    in.close();
}

Junk_impl::
~Junk_impl()
{
    if ( removed_ ) {
        ::remove(statefile_.c_str() );
    };
}

char*
Junk_impl::
tag()
{
    return CORBA::string_dup(tag_.c_str() );
}

void
Junk_impl::
remove()
{
    //cout << "Junk_impl::remove- " << tag_ << endl;
    removed_ = true;
    PortableServer::ObjectId_var oid =
                PortableServer::string_to_ObjectId(tag_.c_str() );
    junk_poa_->deactivate_object(oid);
}

void
Junk_impl::
saveState(const char* object_id)
{
    std::ofstream out (statefile_.c_str() );
    save(out);
    out.close();
}
                                                                                
void
Junk_impl::
save (std::ostream& ostrm)
{
}
                                                                                
void
Junk_impl::
restore (std::istream& istrm)
{
}

JunkFactory_impl::
JunkFactory_impl(PortableServer::POA_ptr root_poa)
{
    CORBA::PolicyList pl;
    pl.length(3);
    pl[0] = root_poa->create_lifespan_policy(
                                     PortableServer::PERSISTENT);
    pl[1] = root_poa->create_id_assignment_policy(
                                      PortableServer::USER_ID);
    pl[2] = root_poa->create_request_processing_policy(
                                      PortableServer::USE_SERVANT_MANAGER);
    junk_poa_ = root_poa->create_POA("Junk_POA",
                                     PortableServer::POAManager::_nil(),
                                     pl);
    PortableServer::POAManager_var junk_mgr = junk_poa_->the_POAManager();
    JunkActivator* junk_activator = new JunkActivator;
    PortableServer::ServantManager_var jaref = junk_activator->_this();
    junk_poa_->set_servant_manager( jaref);
                                                                                
    PortableServer::ObjectId_var oid = root_poa->activate_object(this);
    std::ofstream of ("junk_factory.ior");
    CORBA::Object_var ref = root_poa->id_to_reference( oid.in() );
    of << ORB_holder::instance()->orb()->object_to_string( ref) << endl;
    of.close();

    PortableServer::POA_ptr junk_count_poa = 
                          root_poa->create_POA("JunkCount_POA", junk_mgr, pl);
    JunkCountActivator* jc_act = new JunkCountActivator;
    PortableServer::ServantManager_var jc_ref = jc_act->_this();
    junk_count_poa->set_servant_manager( jc_ref);

    std::string jc_tag = "junk_counts";
    JunkCount_impl* jc_impl = new JunkCount_impl(jc_tag, junk_count_poa);
    oid = PortableServer::string_to_ObjectId(jc_tag.c_str() );
    CORBA::Object_ptr obj;
    try {
        junk_count_poa->activate_object_with_id( oid, jc_impl);
        obj = junk_count_poa->id_to_reference(oid);
    } catch ( const PortableServer::POA::ServantAlreadyActive& saa) {
        cerr << "JunkFactory_impl::ctor-JunkCount servant already active" 
                                                                      << endl;
        assert(0);
    } catch ( const PortableServer::POA::ObjectAlreadyActive& saa) {
        cerr << "JunkFactory_impl::ctor-JunkCount object already active" 
                                                                      << endl;
        assert(0);
    }
    cout << "JunkFactory_impl::ctor-JunkCount activated" << endl;
                                                                                
    junk_count_ref_ = JunkCount::_narrow(obj);

}

PortableServer::POAManager_ptr
JunkFactory_impl::poa_mgr()
{
    return junk_poa_->the_POAManager();
}

Junk_ptr
JunkFactory_impl::create( const char* tag)
{
    Junk_impl* junk_impl = new Junk_impl(true, tag, junk_poa_);
    PortableServer::ObjectId_var oid =
                     PortableServer::string_to_ObjectId(tag );
    CORBA::Object_ptr obj;
    try {
        junk_poa_->activate_object_with_id( oid, junk_impl);
        obj = junk_poa_->id_to_reference(oid);
    } catch ( const PortableServer::POA::ServantAlreadyActive& saa) {
        throw CannotCreateJunk("ServantAlreadyActive", tag );
    } catch ( const PortableServer::POA::ObjectAlreadyActive& saa) {
        throw CannotCreateJunk("ObjectAlreadyActive", tag );
    }
                                                                                
    Junk_ptr junk_ref = Junk::_narrow(obj);
    return junk_ref;
}

JunkCount_ptr
JunkFactory_impl::junk_count()
{
    return JunkCount::_narrow(junk_count_ref_);
}

PortableServer::Servant
JunkActivator::
incarnate(const PortableServer::ObjectId& oid,
          PortableServer::POA_ptr    poa)
{
    CORBA::String_var oid_string;
    try {
        oid_string = PortableServer::ObjectId_to_string(oid);
    } catch ( const CORBA::BAD_PARAM&) {
        throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_YES);
    }
                                                                                
    PortableServer::Servant server = 0;
    server = new Junk_impl(oid_string.in(), poa);
    return server;
}
                                                                                
void
JunkActivator::
etherealize(const PortableServer::ObjectId& oid,
            PortableServer::POA_ptr         poa,
            PortableServer::Servant         servant,
            CORBA::Boolean                  cleanup_in_progress,
            CORBA::Boolean                  remaining_activations)
{
    if ( !remaining_activations  ) {
        CORBA::String_var oid_string;
        oid_string = PortableServer::ObjectId_to_string(oid);
        Junk_impl* target = dynamic_cast<Junk_impl*>(servant);
        target->saveState(oid_string.in());
        delete servant;
    }
}

ORB_holder::
ORB_holder()
{
    orb_ = CORBA::ORB::_nil();
}

ORB_holder*
ORB_holder::
instance()
{
    if ( !instance_ ) instance_ = new ORB_holder();
    return instance_;
}

void 
ORB_holder::
orb(CORBA::ORB_ptr orb)
{
    orb_ = CORBA::ORB::_duplicate(orb);
}

CORBA::ORB_ptr
ORB_holder::
orb()
{
    if ( CORBA::is_nil(orb_) ) {
        std::cerr << "ORB_holder::orb() - orb unassigned - "
                                "programming error" << std::endl;
        assert(0);
    }
    return CORBA::ORB::_duplicate(orb_);
}

JunkCount_impl::
JunkCount_impl(const std::string& oid_s, PortableServer::POA_ptr poa)
{
    oid_s_     = oid_s;
    poa_       = PortableServer::POA::_duplicate(poa);
    statefile_ = "db/" + oid_s;
    removed_   = false;

    std::ifstream in( statefile_.c_str() );
    if (!in ) cout << "JunkCount_impl::ctor-cannot open " << statefile_ << endl;

    if ( in.rdbuf()->is_open() ) {
        restore (in);
        in.close();
    } else {
        nc_count_ = 0;
        obj_count_ = 0;
    }
}

JunkCount_impl::
~JunkCount_impl()
{
    if ( removed_ ) {
        ::remove(statefile_.c_str() );
    };
}

CORBA::Long
JunkCount_impl::
nc_count()
{
    MICOMT::AutoLock l(count_lock_);
    nc_count_++;
    return nc_count_;
}

CORBA::Long
JunkCount_impl::
obj_count()
{
    MICOMT::AutoLock l(count_lock_);
    obj_count_++;
    return obj_count_;
}

void
JunkCount_impl::
remove()
{
    removed_ = true;
    PortableServer::ObjectId_var oid =
                PortableServer::string_to_ObjectId(oid_s_.c_str() );
    poa_->deactivate_object(oid);
}

void
JunkCount_impl::
saveState(const char* object_id)
{
    std::ofstream out (statefile_.c_str() );
    save(out);
    out.close();
}
                                                                                
void
JunkCount_impl::
save (std::ostream& ostrm)
{
    ostrm << "JunkCount_persistent_store" << endl;
    ostrm << "nc_count:     " << nc_count_ << endl;
    ostrm << "obj_count:    " << obj_count_ << endl;
}
                                                                                
void
JunkCount_impl::
restore (std::istream& istrm)
{
    std::string tmpstr;
    istrm >> tmpstr;
    if ( tmpstr != "JunkCount_persistent_store" ) {
        cerr << "JunkCount_impl::restore-not correct persistent store" << endl;
        assert(0);
    }
    istrm >> tmpstr >> nc_count_;
    istrm >> tmpstr >> obj_count_;
}

PortableServer::Servant
JunkCountActivator::
incarnate(const PortableServer::ObjectId& oid,
          PortableServer::POA_ptr    poa)
{
    CORBA::String_var oid_string;
    try {
        oid_string = PortableServer::ObjectId_to_string(oid);
    } catch ( const CORBA::BAD_PARAM&) {
        throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_YES);
    }
                                                                                
    PortableServer::Servant server = 0;
    server = new JunkCount_impl(oid_string.in(), poa);
    return server;
}
                                                                                
void
JunkCountActivator::
etherealize(const PortableServer::ObjectId& oid,
            PortableServer::POA_ptr         poa,
            PortableServer::Servant         servant,
            CORBA::Boolean                  cleanup_in_progress,
            CORBA::Boolean                  remaining_activations)
{
    if ( !remaining_activations  ) {
        CORBA::String_var oid_string;
        oid_string = PortableServer::ObjectId_to_string(oid);
        JunkCount_impl* target = dynamic_cast<JunkCount_impl*>(servant);
        target->saveState(oid_string.in());
        delete servant;
    }
}
