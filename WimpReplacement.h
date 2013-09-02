#pragma once


#include <string>
#include <iostream>

#include <boost/lambda/lambda.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/signals2/signal_base.hpp>
#include <boost/signals2/slot.hpp>
#include <boost/signals2/slot_base.hpp>

#include <boost/thread/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/atomic.hpp>
#include "WimpQueue.h"

using std::string;
using std::endl;
using std::cout;

#define BOOST_SIGNAL boost::signals2::signal<void (const std::string& publisherName, T& data)>

class WimpReplacement
{
public:
    WimpReplacement(void);
    ~WimpReplacement(void);

    void test();
};


// #########################################################################################################################################################




template <class T>
class InterfaceSubscriber
{
public:
    InterfaceSubscriber(const std::string& name)
        : mName         (name) {}

    virtual void onData (const std::string&   source, T& data)
    {
        cout << "InterfaceSubscriber::onData::BASE SHOULD BE IGNORED\n";
    }
    virtual ~InterfaceSubscriber() {}
protected:
    const std::string mName;
};

template <class T>
class InterfacePublisher
{
public:
    InterfacePublisher(const std::string& publisherName, WimpQueue<T>* wimpQ)
        : mPublisherName         (publisherName)
        , mSignalArgs            ()
        , mWimpQueue             (wimpQ)
        , mPublisher_threads     ()
    {
        setWimpQueue (mWimpQueue);
    }

    virtual ~InterfacePublisher()
    {
        mPublisher_threads.join_all();
    }
    void publish(T* data)
    {
        if (mWimpQueue)
        {
            cout << "Enqueueing data.......\n";
            while (!mWimpQueue->push(data));

            cout << "Enqueued data....... memlocation=" << data << "; data=" << *data << endl;
        }
        else
        {
            mSignalArgs(mPublisherName, *data);
        }
    }
    void thread_run_publish()
    {
        cout << "thread_run_publish. 1. \n";
        if (mWimpQueue)
        {
            T* data = new T();
            cout << "thread_run_publish. 2. \n";
            bool gotData = false;
            while(!false)
            {
                //cout << "thread_run_publish. 3. MemADDRESS= " << data << endl;
                data = mWimpQueue->pop(gotData);
                if (gotData && data)
                {
                    mSignalArgs(mPublisherName, *data);
                }

                cout << "thread_run_publish. 5. \n";
            }
        }
    }
    
    void addSubscription (InterfaceSubscriber<T>* subsc)
    {
        mSignalArgs.connect( std::bind (&InterfaceSubscriber<T>::onData , std::ref(*subsc), std::placeholders::_1, std::placeholders::_2));
    }

public:
    void setWimpQueue           (WimpQueue<T>* wimpQueue)
    {
        if (mWimpQueue != NULL || wimpQueue == NULL)
            return;
        mWimpQueue = wimpQueue;
        // if Async, create a new thread
        cout << "setWimpQueue. Creating new thread \n";
        mPublisher_threads.create_thread(std::bind (&InterfacePublisher::thread_run_publish, std::ref(*this)));
    }
protected:
    BOOST_SIGNAL                mSignalArgs;
    const std::string           mPublisherName;
    WimpQueue<T>*               mWimpQueue;
    boost::thread_group         mPublisher_threads;
};

// ################################################### SAMPLE CLASSES THAT CAN ACT LIKE MH AND LH ###################################################

/*
class MessageHandlerSubscriber : public InterfaceSubscriber<LineData>
{
public:
    MessageHandlerSubscriber (const std::string& subscName) : InterfaceSubscriber(subscName) {}
    void onData (const std::string&   source, LineData&        data) override
    {
        cout << mName << ":[" << source << "]Received Data of value: " << data.getSeqNum() << endl;
    }
};


class LineHandlerPublisher : public InterfacePublisher<LineData>
{
public:
    LineHandlerPublisher (const std::string& name, WimpQueue<LineData>* mWimpQ) : InterfacePublisher(name, mWimpQ) {}
};

*/
// ################################################### JUST AS SAMPLE ###################################################

class StringSubscriber : public InterfaceSubscriber<std::string>
{
public:
    StringSubscriber (const std::string& subscName) : InterfaceSubscriber<std::string>(subscName) {}
    void onData (const std::string&   source, std::string&        data) override
    {
        cout << mName << ":[" << source << "]Received string of value: " << data << endl;
    }
};


class StringPublisher : public InterfacePublisher<std::string>
{
public:
    StringPublisher (const std::string& name, WimpQueue<std::string>* mWimpQ) : InterfacePublisher(name, mWimpQ) {}
};

template <class Type>
class LockFreePublisher : public InterfacePublisher<Type>
{
public:
    LockFreePublisher (const std::string& name)
        : InterfacePublisher<Type> (name, NULL)
        , mWimpQueue        (new QueueLockFree<Type>())
    {
        this->setWimpQueue(mWimpQueue);
    }

    ~LockFreePublisher()
    {
        delete mWimpQueue;
    }

private:
    QueueLockFree<Type>* mWimpQueue;
};


/*
template <class T>
class _Delete_SimpleDataSubscriber : public InterfaceSubscriber<T>
{
public:
    _Delete_SimpleDataSubscriber (const std::string& subscName) : InterfaceSubscriber<std::string>(subscName) {}
    void onData (const std::string&   source, T&        data) override
    {
        //cout << mName << ":[" << source << "]Received value: " << data << endl;
    }
};
*/
