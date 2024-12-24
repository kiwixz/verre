find_package(fmt REQUIRED)

file(GLOB_RECURSE src "*.cpp")
add_library(utils ${src})
add_library(verre::utils ALIAS utils)
target_link_libraries(utils PUBLIC
    fmt::fmt
)
