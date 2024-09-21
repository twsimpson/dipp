#pragma once

#include <dipp/dependency-not-found-error.hpp>
#include <dipp/dependency-provider.hpp>
#include <dipp/dependency-resolver.hpp>
#include <functional>
#include <ranges>
#include <utility>

namespace DIPP
{
    namespace _detail
    {
        template<typename T>
        struct is_shared_ptr : std::false_type {};

        template<typename T>
        struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};

        template<typename T>
        static inline constexpr const bool is_shared_ptr_v = is_shared_ptr<T>::value;

        template<typename T>
        struct shared_ptr_type;

        template<typename T>
            requires is_shared_ptr_v<T>
        struct shared_ptr_type<T>
        {
            using type = T;
        };

        template<typename T>
        using shared_ptr_type_t = typename shared_ptr_type<T>::type;
    }

    template<DependencyType T>
    std::unique_ptr<DependencyResolver> make_dependency_resolver(std::function<std::shared_ptr<T>(DependencyProvider&)> factory)
    {
        struct resolver final
            :   public DependencyResolver
        {
            resolver(std::function<std::shared_ptr<T>(DependencyProvider&)> factory) noexcept
                :   DependencyResolver{},
                    factory(std::move(factory))
            {
            }
            ~resolver() = default;
            std::shared_ptr<void> resolve(DependencyProvider& provider) override
            {
                return factory(provider);
            }
            std::function<std::shared_ptr<T>(DependnecyProvider&)> factory;
        };
        return std::make_unique<resolver>(std::move(factory));
    }

    template<DependencyType T>
    std::unique_ptr<DependencyResolver> make_dependency_resolver(std::function<std::shared_ptr<T>()> factory)
    {
        struct resolver final
            :   public DependencyResolver
        {
            resolver(std::function<std::shared_ptr<T>()> factory) noexcept
                :   DependencyResolver{},
                    factory(std::move(factory))
            {
            }
            ~resolver() = default;
            std::shared_ptr<void> resolve(DependencyProvider&) override
            {
                return factory();
            }
            std::function<std::shared_ptr<T>()> factory;
        };
        return std::make_unique<resolver>(std::move(factory));
    }

    template<typename FactoryT>
        requires (std::is_class_v<FactoryT> && !std::is_final_v<FactoryT> && std::invocable<FactoryT, DependencyResolver&> && _detail::is_shared_ptr_v<std::invoke_result_t<FactoryT, DependencyProvider&>> && DependencyType<_detail::shared_ptr_type_t<std::invoke_result_t<FactoryT, DependencyProvider&>>>)
    std::unique_ptr<DependencyResolver> make_dependency_resolver(FactoryT&& factory)
    {
        struct resolver final
            :   private FactoryT,
                public DependencyResolver
        {
            resolver(FactoryT&& factory) noexcept(std::is_nothrow_move_constructible_v<FactoryT>)
                :   FactoryT(std::move(factory)),
                    DependencyResolver{}
            {
            }
            ~resolver() = default;
            std::shared_ptr<void> resolve(DependencyProvider& provider) override
            {
                return std::invoke(static_cast<FactoryT&>(*this), provider);
            }
        };
        return std::make_unique<resolver>(std::move(factory));
    }

    template<typename FactoryT>
        requires (std::is_class_v<FactoryT> && !std::is_final_v<FactoryT> && std::invocable<FactoryT> && _detail::is_shared_ptr_v<std::invoke_result_t<FactoryT>> && DependencyType<_detail::shared_ptr_type_t<std::invoke_result_t<FactoryT>>>)
    std::unique_ptr<DependencyResolver> make_dependency_resolver(FactoryT&& factory)
    {
        struct resolver final
            :   private FactoryT,
                public DependencyResolver
        {
            resolver(FactoryT&& factory) noexcept(std::is_nothrow_move_constructible_v<FactoryT>)
                :   FactoryT(std::move(factory)),
                    DependencyResolver{}
            {
            }
            ~resolver() = default;
            std::shared_ptr<void> resolve(DependencyProvider&) override
            {
                return std::invoke(static_cast<FactoryT&>(*this));
            }
        };
        return std::make_unique<resolver>(std::move(factory));
    }

    template<typename FactoryT>
        requires ((!std::is_class_v<FactoryT> || std::is_final_v<FactoryT>) && std::invocable<FactoryT, DependencyResolver&> && _detail::is_shared_ptr_v<std::invoke_result_t<FactoryT, DependencyProvider&>> && DependencyType<_detail::shared_ptr_type_t<std::invoke_result_t<FactoryT, DependencyProvider&>>>)
    std::unique_ptr<DependencyResolver> make_dependency_resolver(FactoryT&& factory)
    {
        struct resolver final
            :   public DependencyResolver
        {
            resolver(FactoryT&& factory) noexcept(std::is_nothrow_constructible_v<FactoryT, FactoryT>)
                :   DependencyResolver{},
                    factory(std::forward<FactoryT>(factory))
            {
            }
            ~resolver() = default;
            std::shared_ptr<void> resolve(DependencyProvider& provider) override
            {
                return std::invoke(factory, provider);
            }
            FactoryT factory;
        };
        return std::make_unique<resolver>(std::forward<FactoryT>(factory));
    }

    template<typename FactoryT>
        requires ((!std::is_class_v<FactoryT> || std::is_final_v<FactoryT>) && std::invocable<FactoryT> && _detail::is_shared_ptr_v<std::invoke_result_t<FactoryT>> && DependencyType<_detail::shared_ptr_type_t<std::invoke_result_t<FactoryT>>>)
    std::unique_ptr<DependencyResolver> make_dependency_resolver(FactoryT&& factory)
    {
        struct resolver final
            :   public DependencyResolver
        {
            resolver(FactoryT&& factory) noexcept(std::is_nothrow_constructible_v<FactoryT, FactoryT>)
                :   DependencyResolver{},
                    factory(std::forward<FactoryT>(factory))
            {
            }
            ~resolver() = default;
            std::shared_ptr<void> resolve(DependencyProvider&) override
            {
                return std::invoke(factory);
            }
            FactoryT factory;
        };
        return std::make_unique<resolver>(std::forward<FactoryT>(factory));
    }

    template<typename FactoryT, std::ranges::input_range RangeT>
        requires ((std::is_class_v<FactoryT> && !std::is_final_v<FactoryT>) && std::invocable<FactoryT, std::vector<std::shared_ptr<void>>&&> && _detail::is_shared_ptr_v<std::invoke_result_t<FactoryT, std::vector<std::shared_ptr<void>>&&>> && std::convertible_to<std::ranges::range_reference_t<RangeT>, DependnecyKey<>>)
    std::unique_ptr<DependencyResolver> make_dependency_resolver(FactoryT&& factory, RangeT&& dependencies)
    {
        struct resolver final
            :   private FactoryT,
                public DependencyResolver
        {
            resolver(FactoryT&& factory, std::vector<DependnecyKey<>> dependencies) noexcept
                :   FactoryT(std::move(factory)),
                    DependencyResolver{},
                    dependencies(std::move(dependencies))
            {
            }
            ~resolver() = default;
            std::shared_ptr<void> resolve(DependencyProvider& provider) override
            {
                std::vector<std::shared_ptr<void>> resolvedDependencies;
                resolvedDependencies.reserve(dependencies.size());
                for (auto key : dependencies)
                {
                    if (!resolvedDependencies.emplace_back(provider.resolve(key))) {
                        throw DependencyNotRegistered(key);
                    }
                }
                return std::invoke(static_cast<FactoryT&>(*this), std::move(resolvedDependencies));
            }
            std::vector<DependnecyKey<>> dependencies;
        };
        std::vector<DependnecyKey<>> dependencyKeys;
        if constexpr (std::ranges::sized_range<RangeT>) {
            dependencyKeys.reserve(std::ranges::size(dependencies));
        }
        for (auto&& key : dependencies)
        {
            dependencyKeys.emplace_back(static_cast<DependnecyKey<>>(std::forward<decltype(key)>(key)));
        }
        return std::make_unique<resolver>(std::move(factory), std::move(dependencyKeys));
    }

    template<typename FactoryT, std::ranges::input_range RangeT>
        requires ((!std::is_class_v<FactoryT> || std::is_final_v<FactoryT>) && std::invocable<FactoryT, std::vector<std::shared_ptr<void>>&&> && _detail::is_shared_ptr_v<std::invoke_result_t<FactoryT, std::vector<std::shared_ptr<void>>&&>> && std::convertible_to<std::ranges::range_reference_t<RangeT>, DependnecyKey<>>)
    std::unique_ptr<DependencyResolver> make_dependency_resolver(FactoryT&& factory, RangeT&& dependencies)
    {
        struct resolver final
            :    public DependencyResolver
        {
            resolver(FactoryT&& factory, std::vector<DependnecyKey<>> dependencies) noexcept
                :   DependencyResolver{},
                    factory(std::forward<FactoryT>(factory)),
                    dependencies(std::move(dependencies))
            {
            }
            ~resolver() = default;
            std::shared_ptr<void> resolve(DependencyProvider& provider) override
            {
                std::vector<std::shared_ptr<void>> resolvedDependencies;
                resolvedDependencies.reserve(dependencies.size());
                for (auto key : dependencies)
                {
                    if (!resolvedDependencies.emplace_back(provider.resolve(key))) {
                        throw DependencyNotRegistered(key);
                    }
                }
                return std::invoke(factory, std::move(resolvedDependencies));
            }
            FactoryT factory;
            std::vector<DependnecyKey<>> dependencies;
        };
        std::vector<DependnecyKey<>> dependencyKeys;
        if constexpr (std::ranges::sized_range<RangeT>) {
            dependencyKeys.reserve(std::ranges::size(dependencies));
        }
        for (auto&& key : dependencies)
        {
            dependencyKeys.emplace_back(static_cast<DependnecyKey<>>(std::forward<decltype(key)>(key)));
        }
        return std::make_unique<resolver>(std::move(factory), std::move(dependencyKeys));
    }
}
