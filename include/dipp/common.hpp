#pragma once

#include <concepts>
#include <typeindex>
#include <typeinfo>
#include <type_traits>

namespace DIPP
{
    template<typename T>
    concept DependencyType = std::is_class_v<T>; // Also ensures std::same_as<std::decat_t<T>, T>

    template<typename T = void>
    class DependnecyKey;

    template<>
    class DependnecyKey<void>
    {
        template<typename T>
        friend class DependnecyKey;
    private:
        DependnecyKey(std::type_info value) noexcept
            :   m_value(value)
        {
        }
    public:
        std::type_index value() const noexcept
        {
            return m_value;
        }
        operator std::type_index() const noexcept
        {
            return m_value;
        }
    private:
        std::type_index m_value;
    };

    template<typename T>
        requires DependencyType<T>
    class DependnecyKey<T>
        :   public DependnecyKey<>
    {
    public:
        DependnecyKey() noexcept
            :   DependnecyKey<>(typeid(T))
        {
        }
    };

    

    class DependencyProvider;

    class DependnecyResolver;
}
