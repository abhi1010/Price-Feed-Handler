#ifndef LogCppH__
#define LogCppH__




#define BOOST_LOG_DYN_LINK 1

#include <stdexcept>
#include <string>
#include <iostream>
#include <fstream>
#include <functional>
#include <boost/ref.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/barrier.hpp>

#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/utility/empty_deleter.hpp>
#include <boost/log/utility/record_ordering.hpp>

namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

using boost::shared_ptr;

#define BLOG(E) BOOST_LOG(test_lg::get()) << E;

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(test_lg, boost::log::sources::logger_mt)

class Log
{
public:
    Log();
    ~Log();

    bool initialize();
    
typedef sinks::text_ostream_backend backend_t_h;
typedef sinks::asynchronous_sink<
    backend_t_h,
    sinks::unbounded_ordering_queue<
        logging::attribute_value_ordering< unsigned int, std::less< unsigned int > >
    >
> sink_t_h;

typedef sinks::synchronous_sink< sinks::text_ostream_backend > text_sink;

private:
    boost::shared_ptr< sink_t_h > mSink;
    //boost::shared_ptr< text_sink > pSink;
};
#endif // LogCppH__
