#pragma once

#include <dipp/common.hpp>
#include <memory>

namespace DIPP
{
    class DependencyResolver
    {
    protected:
        DependencyResolver() noexcept;
    public:
        virtual ~DependencyResolver();
    public:
        virtual DependnecyKey<> key() const noexcept = 0;
    public:
        virtual std::shared_ptr<void> resolve(DependencyProvider& provider) = 0;
    };
}
