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
    buffer_.resize(image_size);
    for (SyclBufferHost<uint64_t>& buf : host_buffers_) {
        buf.resize(image_size);
    }
}

Renderer::~Renderer() {
    queue_copy_.wait();
}

Renderer::Result Renderer::render() {
    last_host_buffer_ = (last_host_buffer_ + 1) % host_buffers_.size();

    SyclBufferHost<uint64_t>& host_buf = host_buffers_[last_host_buffer_];

    sycl::event kernel = queue_kernel_.submit([&](sycl::handler& handler) {
        uint16_t rand = last_host_buffer_ / static_cast<double>(host_buffers_.size() - 1) * std::numeric_limits<uint16_t>::max();

        std::span<uint64_t> buf = buffer_.device_span();
        uint64_t* pixels = buf.data();

        handler.parallel_for(buf.size(), [pixels, rand](sycl::id<> id_) {
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
        handler.copy(buffer_.device_data(), host_buf.data(), buffer_.size());
    });

    return {std::move(copy), host_buf};
}

}  // namespace verre::core
