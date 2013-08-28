#pragma once

#include <iostream>
#include <stdint.h>

typedef unsigned long ulong;

class LineData
{
public:
    LineData(void);
    virtual ~LineData(void);

    /** Get the next sequence number. */
    virtual ulong  getNextSeqNum() const { return (getSeqNum() + 1); }

    /** Get the sequence number. */
    virtual ulong getSeqNum   () const;

    /** Get the begining of the raw data. */
    virtual const char*   getData     () const;

    /** Get the length of the raw data. */
    virtual ulong      getDataLen  () const;

    /** Update the sequence number. */
    virtual void  setSeqNum (ulong  seqNum);

    // Concrete, overridable methods:
    /** Clear this object. */
    virtual void  clear ();

    /** Update the location and length of the raw data. */
    virtual void  setData (const char*  data,
                           ulong     len);

    /** Update the length of the raw data. */
    virtual void  setDataLen (ulong  len);

private: 
    const char* mData;
    ulong       mSeqNum;
    ulong       mDataLength;
};


