
#ifndef DATETIME_H_
#define DATETIME_H_

#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/local_time/local_time.hpp"
#include "boost/date_time/local_time/tz_database.hpp"
//#include "boost/locale.hpp"
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/lock_algorithms.hpp>
#include <boost/thread/thread_only.hpp>
//#include <boost/chrono/chrono_io.hpp>

using namespace boost::local_time;

class DateTime
{
    public:
        DateTime();
        ~DateTime();

        static local_date_time             now();
        local_date_time             timeInTz (local_date_time& timeInAnyTz,
        time_zone_ptr tzNew);
    protected:
        static tz_database                 mTzDb;
};
#endif
