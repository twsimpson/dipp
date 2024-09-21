#pragma once

#include <exception>

namespace DIPP
{
    class Error
        :   public std::exception
    {
    protected:
        Error() noexcept;
    public:
        ~Error();
    };
}
