#ifndef DeleterH__
#define DeleterH__

#include <algorithm>

struct Deleter
{
    template <typename T>
    void operator()(T*& t) const
    {
        delete t;
        t = NULL;
    }
};

struct Deleter2nd
{
    template <typename T>
    void operator()(T& t) const
    {
        delete t.second;
        t.second = NULL;
    }
};

template <typename T>
void foreachDeleter (T& t)
{
    std::for_each (t.begin (),
                   t.end (),
                   Deleter ());

    t.clear ();
}

template <typename T>
void foreachDeleter2nd (T& t)
{
    std::for_each (t.begin (),
                   t.end (),
                   Deleter2nd ());

    t.clear ();
}

#endif // DeleterH__
