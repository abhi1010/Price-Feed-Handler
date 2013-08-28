#include "InitModule.h"
#include "InitModuleInternal.h"
#include "Log.h"


class initLogModule : public InitModuleInternal
{
public:
    initLogModule ()
        : InitModuleInternal    ("LogModule", 200)
        , mLog                  (nullptr)
    {
        
    }

    ~initLogModule ()
    {
    }
    
    void init (
        const ConfigManager& cfgManager)
    {
        std::cout << "LogModule::Init" 
            << std::endl;

        mLog = new Log;
        mLog->initialize();

        addModule (mLog, "Log");
        std::cout << "LogModule::End\n";
    }

    void destroy ()
    {
        delete mLog;
    }

private:
    Log* mLog;
};

initLogModule initLogMod;
