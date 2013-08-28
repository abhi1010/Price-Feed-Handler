#pragma once

#include "LineData.h"
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
        std::cout << "InterfaceSubscriber::onData::BASE SHOULD BE IGNORED\n";
    }
    virtual ~InterfaceSubscriber() {}
protected:
    const std::string mName;
};

template <class T>
class InterfacePublisher
{
public:
    InterfacePublisher(const std::string& publisherName, bool isAsync)
        : mPublisherName         (publisherName)
        , mIsAsyncPublisher      (isAsync)
        //, mQueue                 (128)
        //, mShuttingDown          (false)
        , mPublisher_threads      ()
    {
        // if Async, create a new thread
        if (isAsync)
        {
            mPublisher_threads.create_thread(std::bind (&InterfacePublisher::thread_run_publish, std::ref(*this)));
        }
    }

    virtual ~InterfacePublisher()
    {
        mPublisher_threads.join_all();
        //mShuttingDown = true;
    }
    void publish(T& data)
    {
        if (mIsAsyncPublisher)
        {
            std::cout << "Enqueueing data.......\n";
            //while (!mQueue.push(&data));
        }
        else
            mSignalArgs(mPublisherName, data);
    }
    void thread_run_publish()
    {
        /*
        T* data;
        while(!mShuttingDown)
        {
            while (!mQueue.pop(data));
            mSignalArgs(mPublisherName, *data);
        }*/
    }
    
    void addSubscription (InterfaceSubscriber<T>* subsc)
    {
        mSignalArgs.connect( std::bind (&InterfaceSubscriber<T>::onData , std::ref(*subsc), std::placeholders::_1, std::placeholders::_2));
    }

protected:
    boost::signals2::signal<void (const std::string& publisherName, T& data)> mSignalArgs;
    const std::string           mPublisherName;
    const bool                  mIsAsyncPublisher;
    //boost::lockfree::queue<T*>  mQueue;
    //std::atomic<bool>         mShuttingDown;
    boost::thread_group         mPublisher_threads;
};

// ################################################### SAMPLE CLASSES THAT CAN ACT LIKE MH AND LH ###################################################

class MessageHandlerSubscriber : public InterfaceSubscriber<LineData>
{
public:
    MessageHandlerSubscriber (const std::string& subscName) : InterfaceSubscriber(subscName) {}
    void onData (const std::string&   source, LineData&        data) override
    {
        std::cout << mName << ":[" << source << "]Received Data of value: " << data.getSeqNum() << std::endl;
    }
};


class LineHandlerPublisher : public InterfacePublisher<LineData>
{
public:
    LineHandlerPublisher (const std::string& name, bool isAsync) : InterfacePublisher(name, isAsync) {}
};

