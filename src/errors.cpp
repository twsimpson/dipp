#include <dipp/error.hpp>
#include <dipp/dependency-not-found-error.hpp>

DIPP::Error::Error() noexcept = default;

DIPP::Error::~Error() = default;

DIPP::DependencyNotRegistered::DependencyNotRegistered(DependnecyKey<> key) noexcept
    :   Error{},
        m_key{key}
{
}

DIPP::DependencyNotRegistered::~DependencyNotRegistered() = default;

const char* DIPP::DependencyNotRegistered::what() const noexcept
{
    return "DIPP::DependencyNotRegistered";
}
