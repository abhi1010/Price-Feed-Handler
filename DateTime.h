
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


class DateTime
{
    public:
        DateTime();
        ~DateTime();

        static boost::local_time::local_date_time             now();
        boost::local_time::local_date_time             timeInTz (boost::local_time::local_date_time& timeInAnyTz,
        boost::local_time::time_zone_ptr tzNew);
    protected:
        static boost::local_time::tz_database                 mTzDb;
};
#endif
