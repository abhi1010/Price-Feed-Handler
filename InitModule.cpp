
#include "ModuleManager.h"
#include "InitModule.h"
#include "InitModuleInternal.h"

#include <string>


using std::string;

const string sDefaultGroup = "";

typedef struct InitModule::InitModuleImpl
{
    InitModuleImpl (const string& name,
                    uint32_t      weight)
        : mName   (name)
        , mWeight (weight)
    {
    }

    ~InitModuleImpl ()
    {
    }

    string   mName;
    uint32_t mWeight;
}
InitModuleImpl;

InitModule::InitModule (const string& name,
                        uint32_t      weight)
    : mImpl (new InitModuleImpl (name, weight))
{
    ModuleManager& moduleManager = ModuleManager::getInstance ();

    try
    {
        moduleManager.add (this, weight);
    }
    catch (const ModuleManagerException& ex)
    {
        throw InitModuleException (ex.what ());
    }
}

InitModule::~InitModule ()
{
}

const std::string& InitModule::getDefaultGroup ()
{
    return sDefaultGroup;
}

const std::string& InitModule::getName () const
{
    return mImpl->mName;
}

uint32_t InitModule::getWeight () const
{
    return mImpl->mWeight;
}

void InitModule::addModuleBase (
    const ModuleBase*  module,
    const std::string& name,
    const std::string& group)
{
    ModuleManager& moduleManager = ModuleManager::getInstance ();

    try
    {
        moduleManager.addModule (module, name, group);
    }
    catch (const ModuleManagerException& ex)
    {
        throw InitModuleException (ex.what ());
    }
}

const ModuleBase* InitModule::lookupModuleBase (
    const string& name,
    const string& group,
    bool          required) const
{
    ModuleManager& moduleManager = ModuleManager::getInstance ();

    const ModuleBase* module = NULL;

    try
    {
        module = moduleManager.lookupModule (name, group, required);
    }
    catch (const ModuleManagerException& ex)
    {
        throw InitModuleException (ex.what ());
    }

    return module;
}

