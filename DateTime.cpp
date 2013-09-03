#include "DateTime.h"
#include "Log.h"


using namespace boost::gregorian;
using namespace boost::local_time;
using namespace boost::posix_time;


boost::local_time::tz_database   DateTime::mTzDb;

DateTime::DateTime()
{
    if (mTzDb.region_list().size() == 0)
    {
        BLOG ("TZ DB Size=" << mTzDb.region_list().size());
        mTzDb.load_from_file("date_time_zonespec.csv");
    }
    BLOG ("TZ DB Size=" << mTzDb.region_list().size());
}

DateTime::~DateTime()
{
}

local_date_time DateTime::now()
{
    time_zone_ptr tzSg = mTzDb.time_zone_from_region("Asia/Singapore");
    ptime curr_time_InSG(boost::posix_time::microsec_clock::local_time());
    date today = day_clock::local_day();

    local_date_time localTimeSg(today, curr_time_InSG.time_of_day(), tzSg, local_date_time::NOT_DATE_TIME_ON_ERROR);
    return localTimeSg;
}

local_date_time DateTime::timeInTz(local_date_time& timeInAnyTz, time_zone_ptr tzNew)
{
    local_date_time localTimeTokyo = timeInAnyTz.local_time_in(tzNew);
    return localTimeTokyo;
}
