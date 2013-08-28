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
    std::cout << "LOG:: ~ 1. called" << std::endl;
    mSink->flush();
    std::cout << "LOG:: ~ 2. called" << std::endl;
}


bool Log::initialize()
{
    try
    {
        // Open a rotating text file
        shared_ptr< std::ostream > strm(new std::ofstream("test.log"));
        if (!strm->good())
            throw std::runtime_error("Failed to open a text log file");

        // Create a text file sink

        /*
        typedef sinks::text_ostream_backend backend_t;
        typedef sinks::asynchronous_sink<
            backend_t,
            sinks::unbounded_ordering_queue<
                logging::attribute_value_ordering< char, unsigned int, std::less< unsigned int > >
            >
        > sink_t;
*/


        boost::log::add_console_log(std::clog, keywords::format = expr::stream
            << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%H:%M:%S.%f")
            << " --> " << expr::message );




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

        // How about adding stdout?
        //shared_ptr< std::ostream > pStream(&std::clog, logging::empty_deleter());
        //mSink->locked_backend()->add_stream(pStream);

        // Add some attributes too
        logging::core::get()->add_global_attribute("TimeStamp", attrs::local_clock());
        //logging::core::get()->add_global_attribute("RecordID", attrs::counter< unsigned int >());

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


/*
void Log::useFile(const std::string& file)
{
    //log_useFile(file.c_str());
}

void Log::useStdout()
{
    //log_useStdout();
}

void Log::setLevel(Level level)
{
    //log_setLevel((logLevel)level);
}

Log::Level Log::getLevel()
{
    return Log::Level::INFO;
    //return (Log::Level)log_getLevel();
}

void Log::print(Level level, const std::string& format, ...)
{
    char buffer[512];
    va_list args;
    va_start (args, format);
    vsprintf (buffer, format.c_str(), args);
    perror (buffer);
    va_end (args);
    
    va_list ap;
    va_start (ap, format);
    log_printVa((logLevel)level, format.c_str(), ap);
    va_end (ap);
}

void Log::setMamaLevel(const std::string& level)
{
    //log_setMamaLevel(level.c_str());
}

void Log::setPrintInit(bool print)
{
    //log_setPrintInit(print ? 1 : 0);
}

void Log::rollFile()
{
    //log_rollFile();
}

Log::Level Log::stringToLevel(const std::string& level)
{
    return (Log::Level::INFO);
    //return (Log::Level)log_strToLevel(level.c_str());
}

const char* Log::levelToString(Log::Level level)
{
    return "Info";
    //return log_levelToString((logLevel)level);
}
*/
// Log with streams. Procedure to implement it:
//
// 0. Log must become a class with public default ctor, and private copy-ctor and operator=
//
// 1. Add this class as private internal class to Log
//private:
//    class Stream
//    {
//    public:
//        Stream(Log::Level level, const std::string& component);
//        Stream(const Stream& stream);
//        ~Stream();
//
//    private:
//        Log::Level mLevel;
//        std::ostringstream mMessage;
//        
//    private:
//        friend class Log;
//        template <typename T>
//        friend const Stream& operator<<(const Stream& logStream, const T& value);
//    };
//
// 2. Add the following method to Log class
//    Stream getStream(Level level = INFO, const std::string& component = "");
//
// 3. Add as many handy methods to the Log to write to a specific level
//    Stream error(const std::string& component = "");
//    Stream warning(const std::string& component = "");
//    Stream info(const std::string& component = "");
// ... and implement them like this
//    Log::Stream Log::error(const std::string& component)
//    {
//        return getStream(ERROR, component);
//    }
//
// 4. Uncomment the following code and then log messages as follows:
// Log log;
// log.warning("MyComponent") << "var1: " << var1;
//
//    Log::Stream Log::getStream(Log::Level level, const std::string& component)
//    {
//        return Stream(level, component);
//    }
//
//Log::Stream(Log::LogLevel level, const std::string& component)
//    : mLevel(level)
//{   // Mama log does not print the level
//    mMessage
//        // << log_levelToString((logLevel)level)
//        << " [" << component << "] ";
//}
//Log::Stream::~Stream()
//{
//    // mMessage.str() returns a temporary object... check if the following is fine
//    log_print((logLevel)mLevel, "%s", mMessage.str().c_str());
//}
//
//Log::Stream::Stream(const Log::Stream& stream)
//    : mLevel(stream.mLevel)
//    , mMessage(stream.mMessage.str())
//{
//}
//
//template <typename T>
//const Log::Stream& operator<<(const Log::Stream& logStream, const T& value)
//{
//    const_cast<Log::Stream&>(logStream).mMessage << value;
//    return logStream;
//}

