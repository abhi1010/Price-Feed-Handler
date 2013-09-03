#include "Log.h"
#include <stdio.h>
#include <stdarg.h>

#include <boost/log/utility/setup/console.hpp>


#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>


#include <boost/log/attributes/timer.hpp>
#include <boost/log/attributes/named_scope.hpp>

#include <boost/log/support/date_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/barrier.hpp>

using boost::shared_ptr;

Log::Log()
{
}
Log::~Log()
{

    std::cout << "LOG:: ~ called" << std::endl;
    // Flush all buffered records
    mSink->stop();
    mSink->flush();
}


bool Log::initialize()
{
    try
    {
        // Open a rotating text file
        shared_ptr< std::ostream > strm(new std::ofstream("test.log"));
        if (!strm->good())
            throw std::runtime_error("Failed to open a text log file");

        /*
        // Enable putting the output on console
        boost::log::add_console_log(std::clog, keywords::format = expr::stream
            << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%H:%M:%S.%f")
            << " --> " << expr::message );
        */

        boost::shared_ptr< sink_t_h > asyncSink (new sink_t_h(
                                            boost::make_shared< backend_t_h >(),
            // We'll apply record ordering to ensure that records from different threads go sequentially in the file
            keywords::order = logging::make_attr_ordering("TimeStamp", std::less< unsigned int >())));

        mSink = asyncSink;

        mSink->locked_backend()->add_stream(strm);

        mSink->set_formatter
        (
            expr::format("%1%:[%2%] %3%")
                % expr::attr< boost::posix_time::ptime >("TimeStamp")
                % expr::attr< boost::thread::id >("ThreadID")
                % expr::smessage
        );


        // Add it to the core
        logging::core::get()->add_sink(mSink);

        // Add some attributes too
        logging::core::get()->add_global_attribute("TimeStamp", attrs::local_clock());

        // TODO: Not sure why this is not showing up yet
        // Here we go. First, identfy the thread.
        logging::core::get()->add_global_attribute("ThreadID", attrs::current_thread_id());

        // Now, do some logging
        BLOG ("Log record Log.cpp");

        return true;
    }
    catch (std::exception& e)
    {
        std::cout << "FAILURE: " << e.what() << std::endl;
        return false;
    }
}

