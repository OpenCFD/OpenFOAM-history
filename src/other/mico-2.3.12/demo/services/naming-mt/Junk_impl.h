

#ifndef JUNK_IMPL_HH
#define JUNK_IMPL_HH

#include "Junk.h"

#include <iostream>
#include <string>

class ORB_holder
{
    public:
        static ORB_holder* instance();
        CORBA::ORB_ptr     orb();
        void               orb(CORBA::ORB_ptr orb);
    protected:
        ORB_holder();
    private:
        CORBA::ORB_var     orb_;
        static ORB_holder* instance_;
};
 
class Junk_impl : virtual public POA_Junk
{
    friend class JunkActivator;
    public:
        Junk_impl( int is_new, const std::string&, PortableServer::POA_ptr);
        Junk_impl(             const std::string&, PortableServer::POA_ptr);
        ~Junk_impl();

        char*     tag();
        void      remove();
    private:
        void saveState(const char* object_id);
        void save   (std::ostream& ostrm);
        void restore(std::istream& istrm);

        std::string             tag_;
        std::string             statefile_;
        PortableServer::POA_ptr junk_poa_;
        bool                    removed_;
};

class JunkFactory_impl : virtual public POA_JunkFactory
{
    public:
        JunkFactory_impl(PortableServer::POA_ptr);
    
        ::Junk_ptr                     create( const char* tag);
        ::JunkCount_ptr                junk_count();

        PortableServer::POAManager_ptr poa_mgr();
    private:
        PortableServer::POA_var        junk_poa_;
        JunkCount_var                  junk_count_ref_;
};

class JunkActivator : virtual public POA_PortableServer::ServantActivator
{
    public:
        PortableServer::Servant incarnate(const PortableServer::ObjectId&,
                                          PortableServer::POA_ptr);
        void etherealize (const PortableServer::ObjectId&,
                          PortableServer::POA_ptr,
                          PortableServer::Servant,
                          CORBA::Boolean,
                          CORBA::Boolean);
};
 
class JunkCount_impl : virtual public POA_JunkCount
{
    friend class JunkCountActivator;
    public:
        JunkCount_impl(const std::string&, PortableServer::POA_ptr);
        ~JunkCount_impl();

        CORBA::Long  nc_count();
        CORBA::Long  obj_count();
        void         remove();
    private:
        void saveState(const char* object_id);
        void save   (std::ostream& ostrm);
        void restore(std::istream& istrm);

        int                     nc_count_;
        int                     obj_count_;
        std::string             oid_s_;
        std::string             statefile_;
        PortableServer::POA_ptr poa_;
        bool                    removed_;
        MICOMT::Mutex           count_lock_;
};

class JunkCountActivator : virtual public POA_PortableServer::ServantActivator
{
    public:
        PortableServer::Servant incarnate(const PortableServer::ObjectId&,
                                          PortableServer::POA_ptr);
        void etherealize (const PortableServer::ObjectId&,
                          PortableServer::POA_ptr,
                          PortableServer::Servant,
                          CORBA::Boolean,
                          CORBA::Boolean);
};
#endif

