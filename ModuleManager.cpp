
#include "ModuleManager.h"
#include "InitModule.h"
#include "Module.h"
#include "Log.h"
#include "Deleter.h"
#include "InitModuleInternal.h"

#include <map>
#include <sstream>
#include <algorithm>
#include <string>



using std::ostringstream;
using std::string;

typedef std::multimap<const uint32_t, InitModule*> InitModuleMap;
typedef std::map<string, const ModuleBase*>        ModuleMap;
typedef std::map<string, ModuleMap>                ModuleGroupMap; 

static const uint32_t INTERNAL_RESERVED_WEIGHT = 1000;

struct ModuleManager::ModuleManagerImpl
{
    ModuleManagerImpl  () 
    {
    }

    ~ModuleManagerImpl ()
    {
    }

    InitModuleMap  mInitModules;
    ModuleGroupMap mModules;
    
};

ModuleManager::ModuleManager ()
    : mImpl (new ModuleManagerImpl)
{
}

ModuleManager::~ModuleManager ()
{
}

ModuleManager& ModuleManager::getInstance ()
{
    static ModuleManager sModuleManager;
    return sModuleManager;
}

void ModuleManager::outputModuleList () const
{
    //Log::print (Log::INFO, "[ModuleManager] Loaded modules: ");

    ModuleGroupMap::iterator it  = mImpl->mModules.begin ();
    ModuleGroupMap::iterator end = mImpl->mModules.end ();

    for (; it != end; ++it)
    {
        ModuleMap& moduleMap = it->second;

        //Log::print (Log::INFO, "    Group:");

        ModuleMap::iterator itMod  = moduleMap.begin ();
        ModuleMap::iterator endMod = moduleMap.end ();

        for (; itMod != endMod; ++itMod)
        {
            const string& name   = itMod->first;

            //Log::print (Log::INFO, "               %s)", name.c_str ());
        }
    }
}

void ModuleManager::add  (InitModule* module,
                          uint32_t    weight)
{
    mImpl->mInitModules.insert (
        InitModuleMap::value_type (weight, module));
}

void ModuleManager::init (const ConfigManager& cfgManager)
{
    InitModuleMap::iterator it  = mImpl->mInitModules.begin ();
    InitModuleMap::iterator end = mImpl->mInitModules.end   ();

    //Log::print (Log::INIT, "[ModuleManager] %s", "Initializing all modules");

    try
    {
        for (; it != end; ++it)
        {
            InitModule* initModule = it->second;
            
            //Log::print(Log::DEBUG, "[ModuleManager] Initializing module %s", initModule->getName().c_str());

            if (!dynamic_cast<InitModuleInternal*> (initModule))
            {
                if (initModule->getWeight () <= INTERNAL_RESERVED_WEIGHT)
                {
                    std::ostringstream err;

                    err << "InitModule " << initModule->getName ()
                        << " has an illegal weight value "
                        << initModule->getWeight ()
                        << " (minimum: " 
                        << (INTERNAL_RESERVED_WEIGHT + 1)
                        << ")";

                    throw ModuleManagerException (err.str ());
                }
            }

            initModule->init (cfgManager);
        }
    }
    catch (const ModuleManagerException& ex)
    {
        destroy ();
        throw InitModuleException (ex.what ());
    }
    catch (const InitModuleException& ex)
    {
        destroy ();
        throw;
    }
}

void ModuleManager::destroy ()
{
    if (mImpl->mInitModules.empty ())
    {
        return;
    }
    
    std::cout << "[ModuleManager] Destroying all " << mImpl->mModules.size() << " modules " << std::endl;

    ModuleGroupMap::iterator itG  = mImpl->mModules.begin ();
    ModuleGroupMap::iterator endG = mImpl->mModules.end ();

    for (; itG != endG; ++itG)
    {
        ModuleMap& moduleMap = itG->second;

        foreachDeleter2nd (moduleMap);
    } 

    mImpl->mModules.clear ();

    InitModuleMap::reverse_iterator it  = mImpl->mInitModules.rbegin ();
    InitModuleMap::reverse_iterator end = mImpl->mInitModules.rend   ();

    //Log::print (Log::INIT, "[ModuleManager] Destroying all %u init modules", mImpl->mInitModules.size());

    int counter = 0;
    for (; it != end; ++it)
    {
        InitModule* initModule = it->second;
        
        if (!initModule)
        {
            //std::cout << "[ModuleManager] destroy module  NULL", ++counter, mImpl->mInitModules.size());
        }
        else
        {
            std::cout << "[ModuleManager] destroy module: " << 
                        initModule->getName().c_str() << std::endl;
        }

        initModule->destroy ();
    }
    
    mImpl->mInitModules.clear ();
}

void ModuleManager::addModule (
    const ModuleBase*  module,
    const string&      name,
    const string&      group)
{
    std::pair<ModuleGroupMap::iterator, bool> insertRet =
        mImpl->mModules.insert (
            ModuleGroupMap::value_type (group, ModuleMap ()));

    ModuleMap& moduleMap = (insertRet.first)->second;

    if (!insertRet.second)
    {
        // Module map existed, so we'll need to check
        // to see if we're colliding with an existing module
        ModuleMap::iterator it = moduleMap.find (name);

        if (moduleMap.end () != it)
        {
            std::ostringstream err;

            err << "Module " << name;

            if (group.empty ())
            {
                err << " (group: " << group << ")";
            }

            err << " already exists";

            throw ModuleManagerException (err.str ());
        }
    }

    moduleMap.insert (
        ModuleMap::value_type (name, module));
}

const ModuleBase* ModuleManager::lookupModule (
    const string& name,
    const string& group,
    bool          required) const
{
    std::pair<ModuleGroupMap::iterator, bool> insertRet =
        mImpl->mModules.insert (
            ModuleGroupMap::value_type (group, ModuleMap ()));

    ModuleMap& moduleMap = (insertRet.first)->second;

    ModuleMap::iterator it = moduleMap.find (name);
    if (moduleMap.end () == it)
    {
        if (!required)
        {
            return NULL;
        }

        std::ostringstream err;
        err << "Could not find module " << name;
        if (!group.empty ())
        {
            err << " (group: " << group << ")";
        }
        throw InitModuleException (err.str ());
    }

    return it->second;
}

