#include "LineHandlerMulticast.h"
#include "InitModule.h"
#include "InitModuleInternal.h"
#include "Log.h"

#include <iostream>
#include <string>

#include <boost/thread.hpp>

using std::cout;
using std::endl;


class initModuleLineHandlerMulticast : public InitModuleInternal
{
public:
    initModuleLineHandlerMulticast()
        : InitModuleInternal ("LineHandlerMulticast", 202)
    {
        
    }

    ~initModuleLineHandlerMulticast()
    {
    }
    
    void init (
        const ConfigManager& cfgManager)
    {
        std::cout << "LineHandlerMulticast::Init " << std::endl;
        BLOG ("LineHandlerMulticast::Init");

        mLineHandlerMc = new LineHandlerMulticast("LineHandlerMulticast", "0.0.0.0", "239.255.0.1");

        //boost::thread threadReceiver(std::bind (&LineHandlerMulticast::connect, std::ref (mLineHandlerMc)));
        mLineHandlerMc->connect();
        std::cout << "LineHandlerMulticast::Connected 1. " << std::endl;
        struct timespec sleepTime, remTime;
        sleepTime.tv_sec = 10;
        std::cout << "LineHandlerMulticast::Connected 2. " << std::endl;
        nanosleep (&sleepTime, &remTime);

        std::cout << "LineHandlerMulticast::Connected 3. " << std::endl;
        mLineHandlerMc->setShuttingDown();

        sleepTime.tv_sec = 3;
        nanosleep (&sleepTime, &remTime);
        //mLineHandlerMc->connect();
        std::cout << "LineHandlerMulticast::Connected 4." << std::endl;
        BLOG ("Init Module:: connected");
        mLineHandlerMc->stopService();
        std::cout << "LineHandlerMulticast::Connected 5." << std::endl;

        addModule (mLineHandlerMc, "LineHandlerMulticast");
    }

    void destroy ()
    {
        delete mLineHandlerMc;
    }
private:
    LineHandlerMulticast* mLineHandlerMc;
};


// Create the module.
initModuleLineHandlerMulticast initLineHandlerMc;
