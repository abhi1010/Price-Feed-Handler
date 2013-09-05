#pragma once

#include <string>
#include <iostream>
#include "LineData.h"
#include "WimpReplacement.h"
using std::string;

enum class LineStatus
{
    OFF      = 0,     // monitoring is off for this line (or set of lines)
    UP       = 1,     // both lines OK (if two lines)
    PART     = 2,     // one line OK, one down (if two lines)
    DOWN     = 3,     // both lines down (if two lines)
    UNKNOWN  = 4,     // status cannot be determined
    WAIT     = 5,     // status has not yet been determined
    DISABLED = 6      // monitoring is completely disabled for this
                      // line (or lines); OFF is temporary.
}; // LineStatus


class LineHandler : public LockFreePublisher<LineData>
{
public:
    LineHandler(const std::string& str);

    virtual ~LineHandler();

    void setShuttingDown();
    /*
    inline bool isShuttingDown() const
    {
        return mShuttingDown;
    }*/

    /** terminate() ends all LineHandler activity. */
    virtual void       terminate () = 0;

    /** connect() attempts to connect to the "line" (i.e. network,
     * device, file, etc.). */
    virtual void       connect () = 0;

    /** disconnect() attempts to disconnect the "line". */
    virtual void       disconnect () = 0;

    /** connected() returns whether this LineHandler is "connected"
     *  ("connected" can mean to a server, to the network, or to a
     *  file). */
    virtual bool       connected () const = 0;


    /** read() is an abstract method that must be overridden by
     * concrete subclasses of LineHandler. The method should read up
     * to maxMsgs more messages or as many messages as possible if
     * maxMsgs is zero). */
    virtual void       read (int maxMsgs = 0) = 0;

    /** check() is provided to allow subclasses of the LineHandler *
     *  class to verify that the data recevied can be passed to the
     *  Wimp<LineData>, This is primarily provided so that subclasses
     *  like LineHandlerSoup can strip off any SOUP-level messages, such
     *  as passwords. LineHandler::Check always returns "true". */
    virtual bool       check (LineData& data) = 0;

    /** save() informs the LineHandler that the data should be written
     *  to the recovery file (if active/possible). */
    virtual void       save (LineData& data) = 0;

    /** bounceLine() reports a server disconnect and attempts to
     *  reconnect. */
    virtual void       bounceLine (
        const string&           reason,
        LineStatus  status = LineStatus::DOWN,
        bool                    isError = true) = 0;


    /** getName() gets the "name" of this LineHandler. */
    const string& getName      () const;

    /** Get the message count of this feed handler. */
    ulong      getMsgCount () const;

    /** Set the message count of this feed handler. */
    void          addMsgCount (size_t count) ;

    /** Get the LineStatus of this LineHandler. */
    LineStatus getLineStatus () const;

protected:
    ulong           mMsgCount;
    std::string     mName;
    bool            mShuttingDown;

};


