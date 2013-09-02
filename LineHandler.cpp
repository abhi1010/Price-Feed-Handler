#include "LineHandler.h"
#include <limits>


LineHandler::LineHandler(const std::string& str)
    : LockFreePublisher     (str)
    , mName                 (str)
    , mShuttingDown         (false)
{
}

void LineHandler::setShuttingDown()
{
    mShuttingDown = true;
}

LineHandler::~LineHandler(void)
{
}


/** getName() gets the "name" of this LineHandler. */
const string& LineHandler::getName      () const
{
    return mName;
}

/** Get the message count of this feed handler. */
ulong LineHandler::getMsgCount () const
{
    return mMsgCount;
}

/** Set the message count of this feed handler. */
void LineHandler::addMsgCount (size_t count)
{
    mMsgCount += count;
}

/** Get the LineStatus of this LineHandler. */
LineStatus LineHandler::getLineStatus () const
{
    return LineStatus::OFF;
}
