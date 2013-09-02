#include "LineHandlerMulticast.h"
#include "Log.h"

#include <string>
#include <sstream>
#include <iostream>

// the third parameter of from_string() should be
// one of std::hex, std::dec or std::oct
template <class T>
bool from_string(T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&))
{
    std::istringstream iss(s);
            return !(iss >> f >> t).fail();
}


LineHandlerMulticast::LineHandlerMulticast(const std::string& strName,
                                           const std::string& listen_address,
                                           const std::string& multicast_address)
        : LineHandler       (strName)
        , mIoService        ()
        , mSocket           (mIoService)
        , mListenAddress    (boost::asio::ip::address::from_string(listen_address))
        , mMulticastAddress (boost::asio::ip::address::from_string(multicast_address))
{
}

LineHandlerMulticast::~LineHandlerMulticast(void)
{
    setShuttingDown();
    if (connected())
    {
        terminate();
        disconnect();
    }
}

void LineHandlerMulticast::connect()
{
    // Create the socket so that multiple may be bound to the same address.
    boost::asio::ip::udp::endpoint listen_endpoint(mListenAddress, 30001);
    mSocket.open(listen_endpoint.protocol());
    mSocket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
    mSocket.bind(listen_endpoint);

    // Join the multicast group.
    mSocket.set_option(boost::asio::ip::multicast::join_group(mMulticastAddress));

    mSocket.async_receive_from(
        boost::asio::buffer(mReceivedDataBuffer, MAX_DATA_LEN), mSenderEndpoint,
        boost::bind(&LineHandlerMulticast::handle_receive_from, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));

    boost::thread threadIOService (boost::bind ( &LineHandlerMulticast::runService, this));
}
void LineHandlerMulticast::runService()
{
    mIoService.run();
}
void LineHandlerMulticast::stopService()
{
    mIoService.stop();
}

void LineHandlerMulticast::handle_receive_from(const boost::system::error_code& error,
    size_t bytes_recvd)
{
    static const string FUNC_NAME = "handle_receive_from::";
    BLOG (FUNC_NAME);
    struct timespec recvTime;
    struct timespec realTime;

    if (!error && !mShuttingDown)
    {
        BLOG (FUNC_NAME << "Received Data:" << mReceivedDataBuffer << "; Bytes Received=" <<  bytes_recvd );

        static int numRecv = 0;
        static long totalTime = 0;
        clock_gettime(CLOCK_MONOTONIC_RAW, &recvTime);

        std::stringstream ss;
        ss << mReceivedDataBuffer;
        std::string sendStr = ss.str();
        long sendTvSec, sendTvNsec;
        std::size_t indexTimeSeparator = sendStr.find(":");
        std::size_t indexMsgSeparator = sendStr.find("|");
        std::string sendStrTvSec, sendStrTvNsec;
        if (indexTimeSeparator != std::string::npos)
        {
            ++numRecv;
            sendStrTvSec = sendStr.substr (0, indexTimeSeparator);
            sendStrTvNsec = sendStr.substr (indexTimeSeparator + 1, indexMsgSeparator - indexTimeSeparator);
            BLOG (FUNC_NAME << "indexTimeSeparator is pos " << indexTimeSeparator<< " ,indexMsgSeparator is pos " << indexMsgSeparator );
            from_string (sendTvSec, sendStrTvSec, std::dec);
            from_string (sendTvNsec, sendStrTvNsec, std::dec);
            BLOG (FUNC_NAME << "Send Time was "<< sendStrTvSec << ":" << sendStrTvNsec );
            BLOG (FUNC_NAME << "Receive Time was " << recvTime.tv_sec << ":" << recvTime.tv_nsec );
            if (recvTime.tv_sec - sendTvSec == 0)
            {
                totalTime += recvTime.tv_nsec - sendTvNsec;
                BLOG (FUNC_NAME << "Time taken was "<<recvTime.tv_nsec - sendTvNsec<<" Made of receive time:"
                    << recvTime.tv_nsec << " and sendTime:" << sendTvNsec );
            }
            else
            {
                BLOG (FUNC_NAME << "Time taken was "<< (recvTime.tv_nsec+1E9) - sendTvNsec );
            }
        }

        mSocket.async_receive_from ( boost::asio::buffer(mReceivedDataBuffer, MAX_DATA_LEN), mSenderEndpoint,
                boost::bind(&LineHandlerMulticast::handle_receive_from, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }

    else
    {
        BLOG ("######################################################################");
    }
}

// ########################## LINEHANDLER Functions

void LineHandlerMulticast::terminate ()
{
    mSocket.close();
}
void LineHandlerMulticast::disconnect ()
{
}
bool LineHandlerMulticast::connected () const
{
    BLOG ("socket.is_open?: " << mSocket.is_open() );
    return mSocket.is_open();
}
// ########################## LINEHANDLER Functions

