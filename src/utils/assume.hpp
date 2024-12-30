#pragma once

#include <cstdlib>
#include <source_location>
#include <type_traits>

#include "utils/log.hpp"

namespace verre {

constexpr void assume(bool condition, std::source_location from = std::source_location::current()) {
#ifdef NDEBUG
    constexpr bool ndebug = true;
#else
    constexpr bool ndebug = false;
#endif

    if (!std::is_constant_evaluated() && ndebug) {
        [[assume(condition)]];
        return;
    }

    if (condition)
        return;

    log::dev("Assumption failed in {} at {}:{}:{}", from.function_name(), from.file_name(), from.line(), from.column());
    std::abort();
}

}  // namespace verre
