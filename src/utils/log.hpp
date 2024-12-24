#pragma once

#include <fmt/format.h>

namespace verre::log {

enum class Level {
    trace,
    debug,
    info,
    warning,
    error,
    dev,
};


bool with_colors();
Level verbosity();
void set_verbosity(Level level);


bool vlog(Level level, fmt::string_view fmt, const fmt::format_args& args);

template <typename... Args>
bool log(Level level, fmt::format_string<Args...> fmt, Args&&... args) {
    return vlog(level, fmt, fmt::make_format_args(args...));
}

template <typename... Args>
bool trace(fmt::format_string<Args...> fmt, Args&&... args) {
    return log(Level::trace, fmt, std::forward<Args>(args)...);
}
template <typename... Args>
bool debug(fmt::format_string<Args...> fmt, Args&&... args) {
    return log(Level::debug, fmt, std::forward<Args>(args)...);
}
template <typename... Args>
bool info(fmt::format_string<Args...> fmt, Args&&... args) {
    return log(Level::info, fmt, std::forward<Args>(args)...);
}
template <typename... Args>
bool warn(fmt::format_string<Args...> fmt, Args&&... args) {
    return log(Level::warning, fmt, std::forward<Args>(args)...);
}
template <typename... Args>
bool error(fmt::format_string<Args...> fmt, Args&&... args) {
    return log(Level::error, fmt, std::forward<Args>(args)...);
}
template <typename... Args>
bool dev(fmt::format_string<Args...> fmt, Args&&... args) {
    return log(Level::dev, fmt, std::forward<Args>(args)...);
}

}  // namespace verre::log
