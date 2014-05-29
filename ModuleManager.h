#ifndef ModuleManagerCppH__
#define ModuleManagerCppH__

#include "ApiException.h"
#include <string>
#include <memory>


class ModuleBase;
class InitModule;
class ConfigManager;

class ModuleManagerException : public ApiException
{
public:
    ModuleManagerException (const std::string& msg)
        : ApiException (msg)
    {}
};

/**
 * ModuleManager is a statically allocated class for managing modules
 *
 * Modules are given a weighting, which determines the order of their
 * initialization, with priority given to the lowest numbers.
 */
class ModuleManager
{
public:
    /**
     * Get a reference to the static instance of ModuleManager.
     *
     * @return A reference to the static instance of ModuleManager.
     */
    static ModuleManager& getInstance ();

    /**
     * Print the module list to log. List is in initialisation
     * order according to assigned weights.
     */
    void outputModuleList () const;

    /**
     * Add an InitModule to the list.
     *
     * @param module A pointer to the InitModule for the object.
     * @param weight The weighting to insert the module at.
     */
    void add              (InitModule* module,
                           uint32_t    weight);

    /**
     * Initialises all InitModules in order by weight.
     *
     * @param cfgManager A reference to the ConfigManager.
     */
    void init             (const ConfigManager&  cfgManager);

    /**
     * Destroy all InitModules in reverse order to creation.
     */
    void destroy          ();

    void addModule (
        const ModuleBase*  module,
        const std::string& name,
        const std::string& group);

    const ModuleBase* lookupModule (
        const std::string& name,
        const std::string& group,
        bool               required) const;

protected:
    ModuleManager  ();
    ~ModuleManager ();

private:
    struct ModuleManagerImpl;
    std::auto_ptr <ModuleManagerImpl> mImpl;
};



#endif /* ModuleManagerCppH__ */
