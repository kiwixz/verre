#include "ui/context.hpp"

#include <chrono>
#include <functional>
#include <span>
#include <stop_token>
#include <thread>

#include <QImage>

#include "utils/log.hpp"

namespace verre::ui {

Context::Context() {
    render_thread_ = std::jthread{std::bind_front(&Context::render_thread, this)};
}

void Context::render_thread(const std::stop_token& stop_token) noexcept {
    try {
        core::Renderer::Result next_result = renderer_.render();
        while (!stop_token.stop_requested()) {
            core::Renderer::Result result = std::exchange(next_result, renderer_.render());
            std::span<const uint64_t> image_data = result.get();
            image_ = {reinterpret_cast<const uchar*>(image_data.data()), 100, 100, QImage::Format_RGBX64};
            emit imageChanged();

            std::this_thread::sleep_for(std::chrono::milliseconds{200});
        }
    }
    catch (std::exception& ex) {
        log::error("Render thread failed: {}", ex.what());
    }
}

}  // namespace verre::ui
