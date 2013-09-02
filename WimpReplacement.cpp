#include "WimpReplacement.h"

/*
#include <boost/thread/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/atomic.hpp>
*/
WimpReplacement::WimpReplacement(void)
{
}


WimpReplacement::~WimpReplacement(void)
{
}

void WimpReplacement::test()
{



    using namespace std;
    QueueLockFree<std::string>* lockQueue = new QueueLockFree<std::string>();

    if (true)
    {
        BLOG ( "String PUBLISHER........... "  );

        StringSubscriber subscriber1("String_Subscriber_1");
        StringSubscriber subscriber2("String_Subscriber_2");

        //QueueLockFree<std::string>* lockQueue = new QueueLockFree<std::string>();

        //StringPublisher publisher("Publisher_Of_String", lockQueue);
        //InterfacePublisher<std::string> publisher ("Publisher_Of_String", lockQueue);
        LockFreePublisher<std::string> publisher("reslk");
        publisher.addSubscription(&subscriber1);
        publisher.addSubscription(&subscriber2);
        
        std::string s1 = "LOCKFREEEEEEEEEEEEEEEEEEEE";
        std::string* str = new std::string(s1);
        publisher.publish(str);

        usleep (10000000);


    }

    if (false)
    {
        RingBufferQueue<std::string>* ringBufferQ = new RingBufferQueue<std::string>();
        std::string* s1 = new std::string ("abhi");
        std::string* s2 = NULL;
        BLOG ( "Added: " << s1 << "; or value=" << *s1 );
        ringBufferQ->push (s1);

        bool success = false;
        s2 = ringBufferQ->pop (success);

        BLOG ( "Got value: " << s2 << "; value=" << *s2 << "; success=" << success ) ;
    }

    if (false)
    {
        QueueLockFree<std::string>* mWimpQueue = lockQueue;
        using namespace std;
        std::string* s2 = new std::string("abhi");
        BLOG ( "QueueLockFree:: Enqueueing data.......") ;
        while (!mWimpQueue->push(s2));

        BLOG ( "QueueLockFree:: Enqueued data....... memlocation=" << s2  );

        std::string* dd = NULL;

        bool success = false;

        dd  = mWimpQueue->pop(success);
            //BLOG ( "Popped...... " << dd  );
            BLOG ( "QueueLockFree:: Got value: " << dd << "; value=" << *dd << "; success=" << success  );
    }
    if (true)
    {
        boost::lockfree::queue<std::string*>*  bLock = new boost::lockfree::queue<std::string*>(128);

        using namespace std;
        std::string* s2 = new std::string("abhi");
        BLOG ( "lockfree:: Enqueueing data.......") ;
        while (!bLock->push(s2));

        BLOG ( "lockfree:: Enqueued data....... memlocation=" << s2  );

        std::string* dd = new std::string("abhi");
        while (bLock->pop(dd))
        {
            BLOG ( "lockfree:: Popped...... " << dd  );
        }
    }
    /*
       LineData* data = new LineData();
       data->setSeqNum(7767);

       MessageHandlerSubscriber mhSubscriber1("MHSubscriber1");
       MessageHandlerSubscriber mhSubscriber2("MHSubscriber2");
       LineHandlerPublisher lhPublisher ("LH_Publisher", false);
       lhPublisher.addSubscription(&mhSubscriber1);
       lhPublisher.addSubscription(&mhSubscriber2);

       lhPublisher.publish(*data);
       delete data;
     */




    /*
       BLOG ( "String PUBLISHER........... "  );
       StringSubscriber subscriber1("String_Subscriber_1");
       StringSubscriber subscriber2("String_Subscriber_2");
       StringPublisher publisher("Publisher_Of_String");
       publisher.addSubscription(&subscriber1);
       publisher.addSubscription(&subscriber2);
       std::string str = "Hello World";
       publisher.publish(str);
     */




}

