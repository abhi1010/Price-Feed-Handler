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

        BLOG ("LineHandlerMulticast::Init");

        mLineHandlerMc = new LineHandlerMulticast("LineHandlerMulticast", "0.0.0.0", "239.255.0.1");

        //boost::thread threadReceiver(std::bind (&LineHandlerMulticast::connect, std::ref (mLineHandlerMc)));
        mLineHandlerMc->connect();

        /*
        struct timespec sleepTime, remTime;
        sleepTime.tv_sec = 10;
        std::cout << "LineHandlerMulticast::Connected 2. " << std::endl;
        nanosleep (&sleepTime, &remTime);

        std::cout << "LineHandlerMulticast::Connected 3. " << std::endl;

        sleepTime.tv_sec = 3;
        nanosleep (&sleepTime, &remTime);
        //mLineHandlerMc->connect();
         */

        BLOG ("Init Module:: connected");

        addModule (mLineHandlerMc, "LineHandlerMulticast");
    }

    void destroy ()
    {
    	BLOG ("initModuleLineHandlerMulticast:: Shutting down........ 1. " );
    	mLineHandlerMc->startShutDown();
    	BLOG ("initModuleLineHandlerMulticast:: Shutting down........ 2. " );
    	mLineHandlerMc->stopService();
    	BLOG ("initModuleLineHandlerMulticast:: Shutting down........ 3. " );
    	// Let's sleep for 1 sec maybe
    	struct timespec sleepTime, remTime;
		sleepTime.tv_sec = 1;
		BLOG ("Shutting down........ " );
		nanosleep (&sleepTime, &remTime);
		BLOG ("initModuleLineHandlerMulticast:: Shutting down........ 4. " );
        delete mLineHandlerMc;
    }
private:
    LineHandlerMulticast* mLineHandlerMc;
};


// Create the module.
initModuleLineHandlerMulticast initLineHandlerMc;
