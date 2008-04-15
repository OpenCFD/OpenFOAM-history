


#ifndef JUNKNC_HH
#define JUNKNC_HH

#include "Junk.h"

#include "coss/CosNaming.h"

#include <string>

class JunkNC 
{
    public:
         JunkNC(JunkFactory_ptr junk_factory,
                CosNaming::NamingContext_ptr root_nc,
                int                          client_num);
         ~JunkNC() {}

         bool traverse( CosNaming::Name&             name, 
                        CosNaming::NamingContext_ptr nc,
                        int                          level,
                        int                          liter,
                        bool&                        empty);
         bool remove_nc(CosNaming::NamingContext_ptr nc_t,
                        CosNaming::Name&             name,
                        int                          liter);
         bool add_nc   (CosNaming::NamingContext_ptr nc,
                        int                          level,
                        int                          liter);
         void report(int client_num);
    private:
        int                          nc_add_;
        int                          nc_remove_;
        JunkFactory_var              junk_factory_;
        CosNaming::NamingContext_var root_nc_;
        int                          client_num_;
};

#endif
