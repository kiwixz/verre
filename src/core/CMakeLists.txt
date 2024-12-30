file(GLOB_RECURSE src "*.cpp")
add_library(core ${src})
add_library(verre::core ALIAS core)
target_link_libraries(core PUBLIC
    verre::utils
)
