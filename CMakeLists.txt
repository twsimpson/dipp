cmake_minimum_required(VERSION 3.23.0)
project(DIPP VERSION 0.1.0 LANGUAGES CXX)

add_library(dipp)

set_target_properties(dipp
    PROPERTIES CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_sources(dipp PRIVATE
    "src/dependency-container.cpp"
    "src/dependency-provider.cpp"
    "src/dependency-resolver.cpp"
    "src/errors.cpp"
    "src/simple-dependency-container.cpp"
    "src/simple-dependency-provider.cpp"
)

target_sources(dipp PUBLIC
    FILE_SET public_headers TYPE HEADERS BASE_DIRS "include" FILES
    "include/dipp/common.hpp"
    "include/dipp/dependency-container.hpp"
    "include/dipp/dependency-not-found-error.hpp"
    "include/dipp/dependency-provider.hpp"
    "include/dipp/dependency-resolver.hpp"
    "include/dipp/error.hpp"
    "include/dipp/make-dependency-resolver.hpp"
    "include/dipp/simple-dependency-container.hpp"
    "include/dipp/simple-dependency-provider.hpp"
)

# target_sources(dipp PRIVATE
#     FILE_SET internal_headers TYPE HEADERS BASE_DIRS "src/dipp" FILES
#     "src/dipp/internal.hpp"
# )
