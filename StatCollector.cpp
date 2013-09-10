/*
 * StatCollector.cpp
 *
 *  Created on: Sep 5, 2013
 *      Author: dev
 */

#include "StatCollector.h"
#include "LineData.h"
#include "Log.h"
#include <cmath>

using boost::timer::cpu_timer;
using boost::timer::cpu_times;
using boost::timer::nanosecond_type;


StatCollector::StatCollector()
	: InterfaceSubscriber<LineData>("StatCollector")
	, mCounter 			(0)
	, mFirstTimeStamp	(special_values::not_a_date_time)
	, mTimer 			()
	, mLastElapsed		()
{
	mTimer.stop();
	nanosecond_type const twenty_seconds(20 * 1000000000LL);
	nanosecond_type last(0);
}

StatCollector::~StatCollector()
{
}

void StatCollector::onData (const std::string&   source, LineData& data)
{
	BLOG ( "StatCollector::onData:: Received Data");
	this->collectData(data);
}

void StatCollector::collectData(LineData& data)
{
	++mCounter;
	//cpu_times const elapsed_times(mTimer.elapsed());
	//nanosecond_type const elapsed(elapsed_times.system + elapsed_times.user);

	if (mFirstTimeStamp.is_not_a_date_time())
	{
		mTimer.start();
		mFirstTimeStamp = data.getLineTime();
	}
	mLastElapsed = mTimer.elapsed();
	BLOG ("StatCollector::collectData(): Num Received # " << mCounter << "; mFirstTimeStamp=" << mFirstTimeStamp);
	BLOG ("StatCollector::collectData(): Elapsed = " << mLastElapsed.wall << "; system=" << mLastElapsed.user);
}
