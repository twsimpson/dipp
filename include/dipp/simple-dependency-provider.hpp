#pragma once

#include <dipp/dependency-container.hpp>
#include <dipp/dependency-provider.hpp>

namespace DIPP
{
    class SimpleDependencyProvider final
        :   public DependencyProvider
    {
    public:
        SimpleDependencyProvider(std::shared_ptr<DependencyContainer> container) noexcept
            :   DependencyProvider{},
                m_container(std::move(container))
        {
        }
        ~SimpleDependencyProvider();
    public:
        bool can_resolve(DependnecyKey<> key) const noexcept override
        {
            return m_container->contains(key);
        }
        using DependencyProvider::can_resolve;
    public:
        std::shared_ptr<void> resolve(DependnecyKey<> key) override
        {
            DependencyResolver& resolver = m_container->get(key);
            return resolver.resolve(*this);
        }
        using DependencyProvider::resolve;
    private:
        std::shared_ptr<DependencyContainer> m_container;
    };
}
