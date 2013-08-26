#include <string>
#include <iostream>


class TestModule
{
public:
    TestModule()
    {
        std::cout << "TestModule:ctor" 
            << std::endl;
    }

    void testFn()
    {
        std::cout << "TestModule::testFn"
            << std::endl;
    }
};
