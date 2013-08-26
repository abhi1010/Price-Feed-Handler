#ifndef ModuleH__
#define ModuleH__

#include <stdexcept>
#include <typeinfo>


class InitModule;

template <typename T>
class Module;

class ModuleBase
{
public:
    virtual ~ModuleBase() {}

    template <typename T>
    T* get () const
    {
        if (typeid(T) != mTypeInfo)
        {
            throw std::bad_cast ();
        }

        return (static_cast<const Module<T>*>(this)->get ());
    }

    const std::type_info& getTypeInfo () const
        { return mTypeInfo; }

protected:
    ModuleBase (const std::type_info& info)
        : mTypeInfo (info)
    {
    }

private:
    ModuleBase ();

    const std::type_info& mTypeInfo;

};

template <typename T>
class Module : public ModuleBase
{
public:
    T* get () const { return mObject; }

private:
    Module (T* obj)
        : ModuleBase (typeid(T)) 
        , mObject (obj)
    {
    }
    
    T* mObject;

    friend class InitModule;

};


#endif // ModuleH__
