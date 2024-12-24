#include "utils/log.hpp"

#include <cstdio>
#include <string_view>

#include <fmt/color.h>
#include <fmt/format.h>
#include <unistd.h>

#include "utils/iequal.hpp"

namespace verre::log {
namespace {

void format_level(fmt::memory_buffer& buf, Level level) {
    using namespace std::string_view_literals;

    switch (level) {
    case Level::trace:
        buf.append(with_colors() ? "\033[94m"sv : "<7>"sv);
        buf.append("TRACE: "sv);
        break;
    case Level::debug:
        buf.append(with_colors() ? "\033[36m"sv : "<7>"sv);
        buf.append("DEBUG: "sv);
        break;
    case Level::info:
        break;
    case Level::warning:
        buf.append(with_colors() ? "\033[1;93m"sv : "<4>"sv);
        buf.append("WARNING: "sv);
        break;
    case Level::error:
        buf.append(with_colors() ? "\033[1;91m"sv : "<3>"sv);
        buf.append("ERROR: "sv);
        break;
    case Level::dev:
        buf.append(with_colors() ? "\033[1;95m"sv : "<5>"sv);
        buf.append("DEV: "sv);
        break;
    }
}

Level& verbosity_mutable() {
    static Level r = [] {
        if (const char* env = std::getenv("LOGLEVEL"); env) {
            if (iequal(env, "trace"))
                return Level::trace;
            if (iequal(env, "debug"))
                return Level::debug;
            if (iequal(env, "warning"))
                return Level::warning;
            if (iequal(env, "error"))
                return Level::error;
        }
        return Level::info;
    }();
    return r;
}

}  // namespace


bool with_colors() {
    static const bool r = [] {
        int fd = fileno(stderr);
        if (fd == -1)
            return false;
        return isatty(fd) == 1;
    }();
    return r;
}

Level verbosity() {
    return verbosity_mutable();
}

void set_verbosity(Level level) {
    verbosity_mutable() = level;
}


bool vlog(Level level, fmt::string_view fmt, const fmt::format_args& args) {
    if (level < verbosity())
        return false;

    fmt::memory_buffer buf;
    format_level(buf, level);
    fmt::vformat_to(fmt::appender{buf}, fmt, args);
    if (with_colors())
        buf.append(std::string_view{"\033[0m"});
    buf.push_back('\n');
    (void)std::fwrite(buf.data(), 1, buf.size(), stderr);
    return true;
}

}  // namespace verre::log
