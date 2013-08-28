#pragma once

#include "LineHandler.h"
#include <boost/asio.hpp>
#include "boost/bind.hpp"

#define MAX_DATA_LEN 64000

class LineHandlerMulticast : public LineHandler
{
public:
    LineHandlerMulticast(const std::string& strName, 
                            const std::string& listen_address,
                            const std::string& multicast_address);

    virtual ~LineHandlerMulticast();
    
    void handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd);


    // ################### Functions from LineHandler
    
    virtual void       terminate ();
    virtual void       connect ();
    virtual void       disconnect ();
    virtual bool       connected () const;
    virtual void       read (int maxMsgs = 0) {}

    void runService();
    void stopService();

    /** check() is provided to allow subclasses of the LineHandler *
     *  class to verify that the data recevied can be passed to the
     *  Wimp<LineData>, This is primarily provided so that subclasses
     *  like LineHandlerSoup can strip off any SOUP-level messages, such
     *  as passwords. LineHandler::Check always returns "true". */
    virtual bool       check (LineData& data) {return true;}

    /** save() informs the LineHandler that the data should be written
     *  to the recovery file (if active/possible). */
    virtual void       save (LineData& data) {}

    /** bounceLine() reports a server disconnect and attempts to
     *  reconnect. */
    virtual void       bounceLine (
        const string&           reason,
        LineStatus  status = LineStatus::DOWN,
        bool                    isError = true) {}


private:
    boost::asio::io_service mIoService;
    boost::asio::ip::udp::socket mSocket;
    boost::asio::ip::udp::endpoint mSenderEndpoint;
    char mReceivedDataBuffer[MAX_DATA_LEN];
    const boost::asio::ip::address mListenAddress;
    const boost::asio::ip::address mMulticastAddress;
};


