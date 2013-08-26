#include "TestModule.h"
#include "InitModule.h"
#include "InitModuleInternal.h"
#include "Log.h"

#include <iostream>
#include <string>

using std::cout;
using std::endl;


class initModuleTestModule : public InitModuleInternal
{
public:
    initModuleTestModule ()
        : InitModuleInternal ("TestModule", 201)
    {
        
    }

    ~initModuleTestModule ()
    {
    }
    
    void init (
        const ConfigManager& cfgManager)
    {
        std::cout << "TestModule::Init" 
            << std::endl;
        BLOG ("Log record InitModuleInternal");

        mTestModule = new TestModule;

        addModule (mTestModule, "TestModule");
    }

    void destroy ()
    {
        delete mTestModule;
    }
private:
    TestModule* mTestModule;
};


// Create the module.
initModuleTestModule initTestMod;
