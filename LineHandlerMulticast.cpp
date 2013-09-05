#include "LineHandlerMulticast.h"
#include "Log.h"
#include "DateTime.h"
#include <boost/move/move.hpp>

#include <string>
#include <sstream>
#include <iostream>

using namespace std;

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
        boost::bind(&LineHandlerMulticast::handleReceiveData, this,
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
void LineHandlerMulticast::handleReceiveData(const boost::system::error_code& error,
    size_t bytes_recvd)
{
    if (!error && !mShuttingDown)
    {
        // TODO: Get the time as the first thing - boost date time
        boost::local_time::local_date_time now = DateTime::now();

        cout << "Time.Now=" << now << endl;

        LineData * lineData = new LineData(std::move(mReceivedDataBuffer), bytes_recvd, std::move(now));
        //LineData * lineData = new LineData((mReceivedDataBuffer), bytes_recvd, (now));
        printDataDetails (lineData);
        publish(lineData);
        mSocket.async_receive_from ( boost::asio::buffer(mReceivedDataBuffer, MAX_DATA_LEN), mSenderEndpoint,
                boost::bind(&LineHandlerMulticast::handleReceiveData, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
    else
    {
        BLOG ("######################################################################");
    }
}

void LineHandlerMulticast::printDataDetails (LineData * lineData)
{

    struct timespec recvTime;
    clock_gettime(CLOCK_MONOTONIC_RAW, &recvTime);

    //LineData* data = new LineData (std::move(mReceivedDataBuffer));
    static const string FUNC_NAME = "printDataDetails::";
    BLOG (FUNC_NAME << "Entry");
    struct timespec realTime;

    BLOG (FUNC_NAME << "Received Data:" << lineData->getData() << "; Bytes Received=" <<  lineData->getDataLen() );
    BLOG (FUNC_NAME << "Time Received=" << lineData->getLineTime());

    static int numRecv = 0;
    static long totalTime = 0;

    std::stringstream ss;
    ss << lineData->getData();
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

