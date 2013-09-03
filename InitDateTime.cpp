#include "DateTime.h"
#include "InitModule.h"
#include "InitModuleInternal.h"
#include "Log.h"

#include <iostream>
#include <string>

using std::cout;
using std::endl;


class initModuleDateTime : public InitModuleInternal
{
public:
    initModuleDateTime ()
        : InitModuleInternal ("DateTime", 11)
    {
        
    }

    ~initModuleDateTime ()
    {
    }
    
    void init (
        const ConfigManager& cfgManager)
    {
        std::cout << "DateTime::Init" 
            << std::endl;
        BLOG ("Log record InitModuleInternal");

        mDateTime = new DateTime;

        addModule (mDateTime, "DateTime");
    }

    void destroy ()
    {
        delete mDateTime;
    }
private:
    DateTime* mDateTime;
};


// Create the module.
initModuleDateTime initDateTime;
