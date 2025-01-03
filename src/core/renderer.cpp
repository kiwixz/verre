#include "core/renderer.hpp"

#include <cstddef>
#include <cstdint>
#include <limits>
#include <span>
#include <utility>

#include <sycl/sycl.hpp>

#include "core/sycl_buffer.hpp"
#include "utils/log.hpp"

namespace verre::core {

Renderer::Result::Result(sycl::event event, std::span<const uint64_t> data) :
        event_{std::move(event)},
        data_{data} {
}

std::span<const uint64_t> Renderer::Result::get() {
    event_.wait_and_throw();
    return data_;
}


Renderer::Renderer() {
    log::info("Rendering on {}", queue_kernel_.get_device().get_info<sycl::info::device::name>());

    size_t image_size = 100 * 100;
    for (SyclBufferDevice<uint64_t>& buf : pixels_) {
        buf.resize(image_size);
    }
    for (SyclBufferHost<uint64_t>& buf : pixels_host_) {
        buf.resize(image_size);
    }
}

Renderer::~Renderer() {
    queue_copy_.wait();
}

Renderer::Result Renderer::render() {
    int pixels_buffer = (last_pixels_buffer_ + 1) % pixels_.size();

    sycl::event kernel = queue_kernel_.submit([&](sycl::handler& handler) {
        uint16_t rand = pixels_buffer / static_cast<double>(pixels_.size() - 1) * std::numeric_limits<uint16_t>::max();

        // const uint64_t* last_pixels = pixels_[last_pixels_buffer_].device_data();
        uint64_t* pixels = pixels_[pixels_buffer].device_data();

        handler.parallel_for(pixels_[pixels_buffer].size(), [pixels, rand](sycl::id<> id_) {
            int id = id_;
            int x = id % 100;
            int y = id / 100;

            uint16_t r = rand;
            uint16_t g = sycl::round(x / static_cast<float>(100 - 1) * std::numeric_limits<uint16_t>::max());
            uint16_t b = sycl::round(y / static_cast<float>(100 - 1) * std::numeric_limits<uint16_t>::max());
            pixels[id] = static_cast<uint64_t>(b) << 32
                         | static_cast<uint64_t>(g) << 16
                         | r;
        });
    });

    sycl::event copy = queue_copy_.submit([&](sycl::handler& handler) {
        handler.depends_on(kernel);
        handler.copy(pixels_[pixels_buffer].device_data(),
                     pixels_host_[pixels_buffer].data(),
                     pixels_[pixels_buffer].size());
    });

    last_pixels_buffer_ = pixels_buffer;

    return {std::move(copy), pixels_host_[pixels_buffer]};
}

}  // namespace verre::core
