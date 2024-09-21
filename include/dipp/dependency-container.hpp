#pragma once

#include <dipp/dependency-resolver.hpp>
#include <dipp/dependency-not-found-error.hpp>

namespace DIPP
{
    class DependencyContainer
    {
    protected:
        DependencyContainer() noexcept;
    public:
        virtual ~DependencyContainer();
    public:
        virtual bool contains(DependnecyKey<> key) const noexcept = 0;
        template<DependencyType T>
        bool contains(DependnecyKey<T>) const noexcept
        {
            return contains(typeid(T));
        }
        template<DependencyType T>
        bool contains() const noexcept
        {
            return contains(typeid(T));
        }
        virtual DependencyResolver* try_get(DependnecyKey<> key) const noexcept = 0;
        template<DependencyType T>
        DependencyResolver* try_get(DependnecyKey<T>) const noexcept
        {
            return try_get(typeid(T));
        }
        template<DependencyType T>
        DependencyResolver* try_get() const noexcept
        {
            return try_get(typeid(T));
        }
        DependencyResolver& get(DependnecyKey<> key) const
        {
            if (DependencyResolver* ptr = try_get(key)) {
                return *ptr;
            }
            throw DependencyNotRegistered(key);
        }
        template<DependencyType T>
        DependencyResolver& get(DependnecyKey<T>);
    public:
        virtual bool insert(std::unique_ptr<DependencyResolver>&& resolver) = 0;
        virtual bool remove(DependnecyKey<> key) noexcept = 0;
        template<DependencyType T>
        bool remove(DependnecyKey<T>) noexcept
        {
            return remove(typeid(T));
        }
        template<DependencyType T>
        bool remove() noexcept
        {
            return remove(typeid(T));
        }
    };
}
