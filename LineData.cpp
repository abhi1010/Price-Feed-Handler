#include "LineData.h"


LineData::LineData(void)
    : mData         (NULL)
    , mSeqNum       (0)
{
}


LineData::~LineData(void)
{
}

/** Get the sequence number. */
ulong LineData::getSeqNum   () const
{
    return mSeqNum;
}


/** Get the begining of the raw data. */
const char* LineData::getData     () const
{
    return mData;
}

/** Get the length of the raw data. */
ulong LineData::getDataLen  () const
{
    return mDataLength;
}


/** Update the sequence number. */
void LineData::setSeqNum (ulong  seqNum)
{
    mSeqNum = seqNum;
}

// Concrete, overridable methods:
/** Clear this object. */
void LineData::clear ()
{
    mData = NULL;
}

/** Update the location and length of the raw data. */
void LineData::setData (const char*  data, ulong len)
{
    mData = data;
    mDataLength = len;
}

/** Update the length of the raw data. */
void LineData::setDataLen  (ulong dataLen)
{
    mDataLength = dataLen;
}
