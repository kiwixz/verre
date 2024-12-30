cmake_minimum_required(VERSION 3.20)
file(WRITE "${CMAKE_BINARY_DIR}/.gitignore" "*\n")

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Release, RelWithDebInfo, Debug or None")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" CACHE PATH "")
set(QT_QML_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qml" CACHE PATH "")

set(cxx_flags_base "-Wall -Wextra -Wpedantic -march=x86-64-v2 -ffast-math -ffile-prefix-map=${CMAKE_SOURCE_DIR}/=")
set(cxx_flags_harden "-fstack-protector-strong -D_GLIBCXX_ASSERTIONS -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3")
set(cxx_flags_sanitize "-fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS_RELEASE "${cxx_flags_base} -O3 -DNDEBUG" CACHE STRING "")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${cxx_flags_base} ${cxx_flags_harden} -O3 -g3 -fno-omit-frame-pointer" CACHE STRING "")
set(CMAKE_CXX_FLAGS_DEBUG "${cxx_flags_base} ${cxx_flags_harden} ${cxx_flags_sanitize} -g3 -fno-omit-frame-pointer" CACHE STRING "")

project(verre CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
set(CMAKE_JOB_POOLS "multi=2")
set(CMAKE_JOB_POOL_LINK "multi")
set(CMAKE_OPTIMIZE_DEPENDENCIES ON)
set(CMAKE_SKIP_TEST_ALL_DEPENDENCY OFF)  # cmake 3.29

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)


find_package(Python3 REQUIRED COMPONENTS Interpreter)
file(GLOB_RECURSE src RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "src/*")
file(GLOB src_root RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.py")
foreach (file IN LISTS src src_root)
    add_custom_command(OUTPUT "lint/${file}"
        COMMAND Python3::Interpreter "${CMAKE_CURRENT_SOURCE_DIR}/lint.py" "${CMAKE_CURRENT_SOURCE_DIR}/${file}"
        COMMAND "${CMAKE_COMMAND}" -E touch "lint/${file}"
        DEPENDS "lint.py" "${file}"
        COMMENT "Linting ${file}"
        VERBATIM
    )
    list(APPEND lint_commands "lint/${file}")
endforeach ()
add_custom_target(lint DEPENDS ${lint_commands})


include_directories("src")

add_subdirectory("src/utils")
add_subdirectory("src/core")

add_subdirectory("src/ui")
