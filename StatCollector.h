/*
 * StatCollector.h
 *
 *  Created on: Sep 5, 2013
 *      Author: dev
 */

#ifndef STATCOLLECTOR_H_
#define STATCOLLECTOR_H_

#include "stdio.h"
#include "string"
#include "DateTime.h"
#include "WimpReplacement.h"
#include <boost/timer/timer.hpp>

class LineData;

// Collects all details about LineData objects.
/* The kind of details that we want to save here for reference:
 * - How many LineData Objects == How many multicast packets received
 * - How much time did it take from the first packet to the last packet to arrive + average for them
 * -
 */
class StatCollector : public InterfaceSubscriber<LineData>
{


public:
	StatCollector();
	~StatCollector();

	void 	collectData(LineData& data);
	void 	onData (const std::string&   source, LineData& data) override;

private:
	unsigned long 			mCounter;

	// Timestamp of the first LineData received
	local_date_time 		mFirstTimeStamp;

	// For internal bookkeeping
	boost::timer::cpu_timer 				mTimer;

	// How much time has elapsed since the first LineData was received?
	boost::timer::cpu_times				mLastElapsed;
};

#endif /* STATCOLLECTOR_H_ */


