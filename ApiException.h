#ifndef ApiExceptionH__
#define ApiExceptionH__

#include <stdexcept>
#include <string>

/**
 * General ApiException.
 */
class ApiException : public std::runtime_error
{
public:
    ApiException (const std::string& msg)
        : std::runtime_error (msg)
    {}

    virtual ~ApiException () throw ()
    {}
};

#endif // ApiExceptionH__
