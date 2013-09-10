/*
 * InitStatCollector.cpp
 *
 *  Created on: Sep 6, 2013
 *      Author: dev
 */


#include "StatCollector.h"
#include "InitModule.h"
#include "InitModuleInternal.h"
#include "Log.h"
#include "LineHandler.h"
#include <iostream>
#include <string>

//#include <boost/thread.hpp>

using std::cout;
using std::endl;


class initModuleStatCollector : public InitModuleInternal
{
public:
    initModuleStatCollector()
        : InitModuleInternal ("StatCollector", 203)
    {
    }

    ~initModuleStatCollector()
    {
    }

    void init (
        const ConfigManager& cfgManager)
    {
    	BLOG ("StatCollector::Init");
    	LineHandler* lineHandler = NULL;
    	lookupModule(lineHandler, "LineHandler");
    	if (lineHandler)
    	{
    		BLOG ("StatCollector:: LH Name =" <<  lineHandler->getName());
    	}
    	else
    	{
    		BLOG ("StatCollector:: LH=null");
    	}


        mStatCollector = new StatCollector();
        lineHandler->addSubscription(mStatCollector);

        BLOG ("Init Module:: StatCollector initiated");

        addModule (mStatCollector, "StatCollector");
    }

    void destroy ()
    {
        delete mStatCollector;
    }
private:
    StatCollector* 		mStatCollector;
};

// Create the module.
initModuleStatCollector initStatCollector;
