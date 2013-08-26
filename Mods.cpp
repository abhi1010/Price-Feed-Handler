#include <string>
#include <time.h>
#include <iostream>
#include <stdexcept>
#include <new>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
//#include <InitNamedThreads.h>
#include <stdio.h>

#include "ConfigManager.h"
//#include "Log.h"
#include "Module.h"
//#include "ModuleInitializer.h"
#include "ModuleManager.h"
#include "InitModule.h"



using std::string;
using std::cerr;
using std::invalid_argument;
using std::runtime_error;
using std::bad_alloc;
using std::set_unexpected;
using std::cout;


//Log*               logMain             = NULL;

int main(int argc, char **argv)
{
    // Log initialization...
    // InitModule<Log> initLog;
    // initLog.createModule (&moduleList, &cfgManager);

    ConfigManager cfgManager;
    ModuleManager::getInstance ().init(cfgManager);
    // initialize all objects
    ModuleManager::getInstance ().destroy();
}
