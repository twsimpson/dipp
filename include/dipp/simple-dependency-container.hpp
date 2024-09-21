#pragma once

#include <dipp/dependency-container.hpp>
#include <algorithm>
#include <vector>

namespace DIPP
{
    class SimpleDependencyContainer final
        :   public DependencyContainer
    {
    public:
        SimpleDependencyContainer() noexcept = default;
        SimpleDependencyContainer(std::vector<std::unique_ptr<DependencyResolver>> resolvers) noexcept
            :   DependencyContainer{},
                m_resolvers(std::move(resolvers))
        {
        }
        ~SimpleDependencyContainer() = default;
    private:
        struct key_comparer
        {
            bool operator()(DependnecyKey<> key, const std::unique_ptr<DependencyResolver>& ptr) const noexcept
            {
                return key.value() < ptr->key().value();
            }
            bool operator()(const std::unique_ptr<DependencyResolver>& ptr, DependnecyKey<> key) const noexcept
            {
                return ptr->key().value() < key.value();
            }
        };
        std::pair<bool, typename std::vector<std::unique_ptr<DependencyResolver>>::const_iterator> find(DependnecyKey<> key) const noexcept
        {
            std::size_t left = 0;
            std::size_t right = m_resolvers.size() - 1;
            const typename std::vector<std::unique_ptr<DependencyResolver>>::const_iterator head = m_resolvers.begin();
            while (left < right)
            {
                const std::size_t mid = (left + right) / 2;
                const key_comparer comparer;
                const std::unique_ptr<DependencyResolver>& item = head[mid];
                if (comparer(key, item)) {
                    right = mid - 1;
                } else if (comparer(item, key)) {
                    left = mid + 1;
                } else {
                    return std::make_pair(true, head + mid);
                }
            }
            if (right > left) {
                return std::make_pair(false, head + left);
            }
            return std::make_pair(false, head + right);
        }
    public:
        bool contains(DependnecyKey<> key) const noexcept override
        {
            return find(key).first;
        }
        using DependencyContainer::contains;
        DependencyResolver* try_get(DependnecyKey<> key) const noexcept override
        {
            auto pos = find(key);
            if (!pos.first) {
                return nullptr;
            }
            return pos.second->get();
        }
        using DependencyContainer::try_get;
    public:
        bool insert(std::unique_ptr<DependencyResolver>&& resolver) override
        {
            auto pos = find(resolver->key());
            if (pos.first) {
                return false;
            }
            m_resolvers.insert(pos.second, std::move(resolver));
            return true;
        }
        bool remove(DependnecyKey<> key) noexcept override
        {
            auto pos = find(key);
            if (!pos.first) {
                return false;
            }
            m_resolvers.erase(pos.second);
            return true;
        }
        using DependencyContainer::remove;
    private:
        std::vector<std::unique_ptr<DependencyResolver>> m_resolvers;
    };
}
