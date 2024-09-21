#pragma once

#include <dipp/common.hpp>
#include <memory>

namespace DIPP
{
    class DependencyProvider
    {
    protected:
        DependencyProvider() noexcept;
    public:
        virtual ~DependencyProvider();
    public:
        virtual bool can_resolve(DependnecyKey<> key) const noexcept = 0;
        template<DependencyType T>
        bool can_resolve(DependnecyKey<T>) const noexcept
        {
            return can_resolve(typeid(T));
        }
        template<DependencyType T>
        bool can_resolve() const noexcept
        {
            return can_resolve(typeid(T));
        }
    public:
        virtual std::shared_ptr<void> resolve(DependnecyKey<> key) = 0;
        template<DependencyType T>
        std::shared_ptr<T> resolve(DependnecyKey<T>)
        {
            return std::static_pointer_cast<T>(resolve(typeid(T)));
        }
        template<DependencyType T>
        std::shared_ptr<T> resolve()
        {
            return std::static_pointer_cast<T>(resolve(typeid(T)));
        }
    };
}
