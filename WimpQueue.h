
#include <boost/thread/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/atomic.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/signals2/signal_base.hpp>
#include <boost/signals2/slot.hpp>
#include <boost/signals2/slot_base.hpp>

#include "Log.h"

#define MAX_QUEUE_SIZE 10000

template <class T>
class WimpQueue
{
public:
    WimpQueue()
    {}
    virtual ~WimpQueue() {}

    virtual bool    push(T* data) = 0;
    virtual T*      pop(bool& success) = 0;
};


template <class T>
class QueueLockFree : public WimpQueue<T>
{
public:
    QueueLockFree();
    virtual ~QueueLockFree();

    bool    push(T* data) ;
    T*      pop(bool& success) ;
private:
    boost::lockfree::queue<T*>  mQueue;
};



template <class T>
QueueLockFree<T>::QueueLockFree()
    : WimpQueue<T>  ()
    , mQueue        (128)
{
    BLOG ( "Is_lock_free=" << mQueue.is_lock_free()  );
}

template <class T>
QueueLockFree<T>::~QueueLockFree()
{
}

template <class T>
bool QueueLockFree<T>::push(T* data)
{
    BLOG ( "QueueLockFree::Pushing item.... " << data  );
    return mQueue.push(data);
}


template <class T>
T*   QueueLockFree<T>::pop(bool& success)
{
    //BLOG ( "QueueLockFree::Popping item.... "  );
    T * dd = new T;
    while (!mQueue.pop(dd));
    success = true;
    //BLOG ( "QueueLockFree::pop() : data= " << dd << ";value=" << *dd << "; found=" << success  );
    return dd;
}






template <class T>
class RingBufferQueue: public WimpQueue<T>
{
public:
    RingBufferQueue() 
        : head_ (0)
        , tail_ (0)
    {
    }

    virtual ~RingBufferQueue() {}

    bool push(T* value)
    {
        size_t head = head_.load(boost::memory_order_relaxed);
        size_t next_head = next(head);
        if (next_head == tail_.load(boost::memory_order_acquire))
            return false;
        ring_[head] = value;
        head_.store(next_head, boost::memory_order_release);
        return true;
    }

    T* pop(bool& success)
    {
        T* value = new T;
        size_t tail = tail_.load(boost::memory_order_relaxed);
        if (tail == head_.load(boost::memory_order_acquire))
        {
            success = false;
            return NULL;
        }
        value = ring_[tail];
        tail_.store(next(tail), boost::memory_order_release);
        BLOG ( "RingBufferQueue::pop() : Value=" << value  );
        success = true;
        return value;
    }


private:
    //boost::lockfree::queue<T*>  mQueue;
    T* ring_[MAX_QUEUE_SIZE];
    boost::atomic<size_t> head_, tail_;
    size_t next(size_t current)
    {
        return (current + 1) % MAX_QUEUE_SIZE;
    }
};
/*

template <typename T>
class RingBufferQueue
{
public:
    RingBufferQueue() : head_(0), tail_(0) {}

    bool push(const T & value)
    {
        size_t head = head_.load(boost::memory_order_relaxed);
        size_t next_head = next(head);
        if (next_head == tail_.load(boost::memory_order_acquire))
            return false;
        ring_[head] = value;
        head_.store(next_head, boost::memory_order_release);
        return true;
    }

    bool pop(T & value)
    {
        size_t tail = tail_.load(boost::memory_order_relaxed);
        if (tail == head_.load(boost::memory_order_acquire))
            return false;
        value = ring_[tail];
        tail_.store(next(tail), boost::memory_order_release);
        return true;
    }

private:
    size_t next(size_t current)
    {
        return (current + 1) % MAX_QUEUE_SIZE;
    }

    T ring_[MAX_QUEUE_SIZE];
    boost::atomic<size_t> head_, tail_;
};

*/


