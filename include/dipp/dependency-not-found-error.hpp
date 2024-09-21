#pragma once

#include <dipp/common.hpp>
#include <dipp/error.hpp>

namespace DIPP
{
    class DependencyNotRegistered final
        :   public Error
    {
    public:
        DependencyNotRegistered(DependnecyKey<> key) noexcept;
        ~DependencyNotRegistered();
    public:
        DependnecyKey<> key() const noexcept
        {
            return m_key;
        }
        const char* what() const noexcept override;
    private:
        DependnecyKey<> m_key;
    };
}
