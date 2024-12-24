cmake_minimum_required(VERSION 3.20)
file(WRITE "${CMAKE_BINARY_DIR}/.gitignore" "*\n")

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Release, RelWithDebInfo, Debug or None")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" CACHE PATH "")
set(QT_QML_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qml" CACHE PATH "")

set(cxx_flags_warnings "-Wall -Wextra -Wpedantic -Walloc-zero -Wcast-qual -Wdate-time -Wduplicated-branches -Wduplicated-cond -Wformat=2 -Wmismatched-tags -Wredundant-tags -Wmissing-declarations -Wunused-macros")
set(cxx_flags_base "${cxx_flags_warnings} -march=x86-64-v2 -ffile-prefix-map=${CMAKE_SOURCE_DIR}/=")
set(cxx_flags_harden "-fstack-protector-strong -D_GLIBCXX_ASSERTIONS -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3")
set(cxx_flags_sanitize "-fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS_RELEASE "${cxx_flags_base} -Ofast -flto=auto -DNDEBUG" CACHE STRING "")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${cxx_flags_base} ${cxx_flags_harden} -Ofast -g3 -fno-omit-frame-pointer" CACHE STRING "")
set(CMAKE_CXX_FLAGS_DEBUG "${cxx_flags_base} ${cxx_flags_harden} ${cxx_flags_sanitize} -g3 -fno-omit-frame-pointer" CACHE STRING "")

option(VCPKG "Install dependencies with vcpkg" ON)
if (VCPKG)
    set(VCPKG_BOOTSTRAP_OPTIONS "-disableMetrics" CACHE STRING "")
    set(VCPKG_INSTALL_OPTIONS "--no-print-usage" CACHE STRING "")
    set(CMAKE_TOOLCHAIN_FILE "vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "")
endif ()

project(verre CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_JOB_POOLS "multi=2")
set(CMAKE_JOB_POOL_LINK "multi")
set(CMAKE_OPTIMIZE_DEPENDENCIES ON)
set(CMAKE_SKIP_TEST_ALL_DEPENDENCY OFF)  # cmake 3.29

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

include_directories("src")

add_subdirectory("src/utils")

add_subdirectory("src/ui")
