
#ifndef ConfigManagerH
#define ConfigManagerH

#include <string>
#include <list>

#ifndef WOMBAT_CONFIG_NO_NAMESPACES
 using std::string;
 using std::list;
#endif

class ConfigFile;

struct ConfigManagerImpl;

//----------------------------------------------------------------------------
// Class:  ConfigManager
//----------------------------------------------------------------------------

class ConfigManager
{
  ConfigManager& operator= (const ConfigManager& copy); // No assignment

public:
  typedef list<string>           StringList;

  ConfigManager ();
  //ConfigManager (int           argc, char** const  argv);
  //ConfigManager (StringList&   argv);
  //ConfigManager (const ConfigManager& copy);
  virtual ~ConfigManager ();

  //---------- Utility methods----------------------------------------------
  typedef enum AltRootType
  {
    ALT_ROOT_OPTIONAL = 0,
    ALT_ROOT_REQUIRED = 1
  } AltRootType;

  /*
    //! addFile() adds a ConfigFile to the configuration tree to
    // search; may throw ConfigError.
    void       addFile(const ConfigFile* file);

    //! addFile() adds a node within the configuration tree to search;
    // may throw ConfigError.
    void       addConfigBase(const string&  altRootName,
                             AltRootType    optional);

    StringList getUnusedArgs();
    string     getNextArg(); // may throw ConfigError

    void       setArgList(int  argc, char** const  argv);
    void       setArgList(StringList&   argv);

    void           setClassName  (const string& className);
    void           setAppName    (const string& appName);
    void           setExeName    (const string& exeName);
    void           setVersion    (const string& version);
    void           setBinaryName (const string& binary);
    const string&  getClassName  () const;
    const string&  getAppName    () const;
    const string&  getExeName    () const;
    const string&  getVersion    () const;
    const string&  getBinaryName () const;

    static string  findFilePath (const string& filename);

  //---------- Lookup methods ----------------------------------------------
    string    getString (const string& key,
                         const string& cmdArg,
                         const string& def, 
                         bool          usedOk = false);
    long      getLong   (const string& key,
                         const string& cmdArg,
                         long          def, 
                         bool          usedOk = false);
    double    getDouble (const string& key,
                         const string& cmdArg,
                         double        def, 
                         bool          usedOk = false);
    bool      getBool   (const string& key,
                         const string& cmdArg,
                         bool          def, 
                         bool          usedOk = false);

    static const char* trueFalseStr (bool testValue);

    bool argExist (const string& arg,
                   bool          usedOk); // may throw (ConfigError);

protected:
    //---------- Arg protected members ---------------------------------------
    bool getArg   (const string& arg,
                   string&       result,
                   bool          usedOk); // may throw (ConfigError);
*/
}; // ConfigManager

#endif // ConfigManagerH
