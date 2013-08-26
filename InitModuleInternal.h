#ifndef InitModuleInternalH__
#define InitModuleInternalH__

#include "InitModule.h"


static const uint32_t INIT_WEIGHT_INTERNAL_COMMS      = 10;
static const uint32_t INIT_WEIGHT_STATS               = 15;
static const uint32_t INIT_WEIGHT_WAM_SUBSCRIBER      = 20;
static const uint32_t INIT_WEIGHT_CONFIGMANAGER       = 22;
static const uint32_t INIT_WEIGHT_LOG                 = 25;
static const uint32_t INIT_WEIGHT_TRANSPORT_FACTORY   = 30;
static const uint32_t INIT_WEIGHT_FT_MEMBER           = 40;
static const uint32_t INIT_WEIGHT_DATA_RESPONDER      = 45;
static const uint32_t INIT_WEIGHT_PUBLISHER           = 50;
static const uint32_t INIT_WEIGHT_SYMBOL_CACHE        = 60;
static const uint32_t INIT_WEIGHT_RECAP_HANDLER       = 100;
static const uint32_t INIT_WEIGHT_CONFLATION          = 400;
static const uint32_t INIT_WEIGHT_SYMBOL_LIST_MANAGER = 800;

class InitModuleInternal : public InitModule
{
public:
    InitModuleInternal (const std::string& name,
                        uint32_t           weight)
        : InitModule (name, weight)
    {
    }

    ~InitModuleInternal ()
    {
    }

};


#endif /* InitModuleInternalH__ */
