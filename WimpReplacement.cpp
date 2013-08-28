#include "WimpReplacement.h"


WimpReplacement::WimpReplacement(void)
{
}


WimpReplacement::~WimpReplacement(void)
{
}

void WimpReplacement::test()
{
    LineData* data = new LineData();
    data->setSeqNum(7767);

    MessageHandlerSubscriber mhSubscriber1("MHSubscriber1");
    MessageHandlerSubscriber mhSubscriber2("MHSubscriber2");
    LineHandlerPublisher lhPublisher ("LH_Publisher", false);
    lhPublisher.addSubscription(&mhSubscriber1);
    lhPublisher.addSubscription(&mhSubscriber2);

    lhPublisher.publish(*data);
    delete data;
    /*
    std::cout << "String PUBLISHER........... " << std::endl;
    StringSubscriber subscriber1("String_Subscriber_1");
    StringSubscriber subscriber2("String_Subscriber_2");
    StringPublisher publisher("Publisher_Of_String");
    publisher.addSubscription(&subscriber1);
    publisher.addSubscription(&subscriber2);
    std::string str = "Hello World";
    publisher.publish(str);
    */




}

