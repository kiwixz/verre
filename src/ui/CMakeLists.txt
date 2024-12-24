find_package(fmt REQUIRED)
find_package(Qt6 REQUIRED Quick)

file(GLOB_RECURSE src "*.cpp")
list(REMOVE_ITEM src "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
add_library(ui_ STATIC ${src})
set_target_properties(ui_ PROPERTIES AUTOMOC ON)
target_compile_options(ui_ PRIVATE "-Wno-missing-declarations")
target_link_libraries(ui_ PUBLIC
    fmt::fmt
    Qt6::Quick

    verre::utils
)

qt_add_qml_module(ui_
    URI verre
    VERSION 1.0
    RESOURCE_PREFIX "qt/qml"
    QML_FILES
        "main.qml"
)

add_executable(ui "main.cpp")
set_target_properties(ui PROPERTIES OUTPUT_NAME "verre")
target_link_libraries(ui PRIVATE ui_ ui_plugin)
