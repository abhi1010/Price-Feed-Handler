#ifndef InitModuleH__
#define InitModuleH__

#include "ApiException.h"
#include "Module.h"

#include <stdint.h>
#include <string>
#include <memory>
#include <sstream>
#include <typeinfo>


/**
 * The ModuleManager/InitModule sub-system is used for serialising the creation and
 * destruction of modules in the FHSDK.  A weight value is assigned to each InitModule
 * to specify the order of its construction/destruction relative to others.  For an
 * InitModule with a weight of 20, it will be initialisated before an InitModule with
 * a weight of 30, and vice-versa for destruction.  Only internal FHSDK InitModules
 * may have a weight below or equal-to 1000.  Please use a weight higher than this.
 *
 * @defgroup ModuleManager
 */

class ModuleList;
class ConfigManager;

class InitModuleException : public ApiException
{
public:
    InitModuleException (const std::string& msg)
        : ApiException (msg)
    {}
};

class InitModule
{
public:
    virtual ~InitModule ();

    static const std::string& getDefaultGroup ();

    /**
     * @return The name for the InitModule (defined on construction).
     */
    const std::string& getName   () const;

    /**
     * @return The weighting for the InitModule (defined on construction).
     */
    uint32_t           getWeight () const;

    /**
     * Called by the ModuleManager during FHSDK initialisation
     * in an order defined by the InitModule weight (lower = earlier).
     *
     * @param cfgManager The ConfigManager instance for the FHSDK.
     */
    virtual void init (
        const ConfigManager& cfgManager) = 0;

    /**
     * Called by the ModuleManager during FHSDK destruction/shutdown
     * in an order defined by the InitModule weight (lower = later).
     */
    virtual void destroy () = 0;

protected:
    /**
     * InitModule constructor - Adds the InitModule into ModuleManager
     * using the provided @name and @weight.
     *
     * @param name   The name of the InitModule.
     * @param weight The weight of the InitModule (lower = init earlier).
     */
    InitModule (const std::string& name,
                uint32_t           weight);

    /**
     * Add a named module into ModuleManager.  This makes the module
     * referencable by lookup for other InitModules.  The same module
     * name may be used in different groups/namespaces, but otherwise
     * names are unique per group/namespace.
     *
     * @param obj   The module to be stored (must be allocated on the heap).
     * @param name  The name of the module (used by lookup).
     * @param group The group/namespace for the module. 
     */
    template <typename T>
    void addModule (
        T*                 obj,
        const std::string& name,
        const std::string& group = getDefaultGroup ());

    /**
     * Lookup an existing module from the ModuleManager by name (and
     * optionally group/namespace).  If the module is @required then
     * an exception will be thrown if the module cannot be found. An
     * exception will also be thrown if the module is of a different
     * type than the one requested.
     *
     * @param[out] obj      The pointer to be populated if the module if found.
     * @param      name     The name of the module (used by lookup).
     * @param      group    The group/namespace for the module. 
     * @param      required Determines whether or not the module must exist.
     */
    template <typename T>
    void lookupModule (
        T*&                obj,
        const std::string& name,
        const std::string& group = getDefaultGroup (),
        bool               required = true) const;

    /*
     * Alias for lookupModule (pointer argument variant) with the
     * required parameter set as true.
     */
    template <typename T>
    void lookupRequiredModule (
        T*&                obj,
        const std::string& name,
        const std::string& group = getDefaultGroup ()) const;

    /*
     * Alias for lookupModule (pointer argument variant) with the
     * required parameter set as false.
     */
    template <typename T>
    void lookupOptionalModule (
        T*&                obj,
        const std::string& name,
        const std::string& group = getDefaultGroup ()) const;

    /**
     * Lookup an existing module from the ModuleManager by name (and
     * optionally group/namespace).  If the module is @required then
     * an exception will be thrown if the module cannot be found. An
     * exception will also be thrown if the module is of a different
     * type than the one requested.
     *
     * @param name     The name of the module (used by lookup).
     * @param group    The group/namespace for the module. 
     * @param required Determines whether or not the module must exist.
     *
     * @return The pointer to the module if found.
     */
    template <typename T>
    T* lookupModule (
        const std::string& name,
        const std::string& group = getDefaultGroup (),
        bool               required = true) const;

    /*
     * Alias for lookupModule (return pointer variant) with the
     * required parameter set as true.
     */
    template <typename T>
    T* lookupRequiredModule (
        const std::string& name,
        const std::string& group = getDefaultGroup ()) const;

    /*
     * Alias for lookupModule (return pointer variant) with the
     * required parameter set as false.
     */
    template <typename T>
    T* lookupOptionalModule (
        const std::string& name,
        const std::string& group = getDefaultGroup ()) const;

private:
    void addModuleBase (
        const ModuleBase*  module,
        const std::string& name,
        const std::string& group);

    const ModuleBase* lookupModuleBase (
        const std::string& name,
        const std::string& group,
        bool               required) const;

    struct InitModuleImpl;
    std::auto_ptr<InitModuleImpl> mImpl;

};

template <typename T>
inline void InitModule::addModule (
    T*                 obj,
    const std::string& name,
    const std::string& group)
{
    Module<T>* module = new Module<T> (obj);

    try
    {
        addModuleBase (module, name, group);
    }
    catch (const InitModuleException& ex)
    {
        delete module;
        throw;
    }
}

template <typename T>
inline void InitModule::lookupModule (
    T*&                obj,
    const std::string& name,
    const std::string& group,
    bool               required) const
{
    const ModuleBase* module = lookupModuleBase (name, group, required);

    if (module)
    {
        try
        {
            obj = module->get<T> ();
        }
        catch (const std::bad_cast& ex)
        {
            obj = NULL;

            std::ostringstream err;

            err << "Module " << name
                << " is type "
                << module->getTypeInfo ().name ()
                << ", not type "
                << typeid(T).name ();

            throw InitModuleException (err.str());
        }
    }
    else
    {
        obj = NULL;
    }
}

template <typename T>
inline void InitModule::lookupRequiredModule (
    T*&                obj,
    const std::string& name,
    const std::string& group) const
{
    lookupModule (obj, name, group, true);
}

template <typename T>
inline void InitModule::lookupOptionalModule (
    T*&                obj,
    const std::string& name,
    const std::string& group) const
{
    lookupModule (obj, name, group, false);
}

template <typename T>
inline T* InitModule::lookupModule (
    const std::string& name,
    const std::string& group,
    bool               required) const
{
    T* obj = NULL;

    lookupModule (obj, name, group, required);

    return obj;
}

template <typename T>
inline T* InitModule::lookupRequiredModule (
    const std::string& name,
    const std::string& group) const
{
    T* obj = NULL;

    lookupModule (obj, name, group, true);

    return obj;
}

template <typename T>
inline T* InitModule::lookupOptionalModule (
    const std::string& name,
    const std::string& group) const
{
    T* obj = NULL;

    lookupModule (obj, name, group, false);

    return obj;
}


#endif /* InitModuleH__ */
