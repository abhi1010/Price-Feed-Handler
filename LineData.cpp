#include "LineData.h"

LineData::LineData()
	: mData         (NULL)
	, mSeqNum       (0)
	, mDataLength	(0)
	, mLineTime		(boost::date_time::special_values::min_date_time)
{
}

LineData::LineData(const char* buffer, ulong len, boost::local_time::local_date_time localTime)
    : mData         (buffer)
    , mSeqNum       (0)
	, mDataLength	(len)
	, mLineTime		(localTime)
{
}

LineData::~LineData()
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
    mDataLength = 0;
    mSeqNum = 0;
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
