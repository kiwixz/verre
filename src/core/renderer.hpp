#pragma once

#include <array>
#include <cstdint>
#include <span>

#include <sycl/sycl.hpp>

#include "core/sycl_buffer.hpp"

namespace verre::core {

struct Renderer {
    struct Result {
        Result(sycl::event event, std::span<const uint64_t> data);

        std::span<const uint64_t> get();

    private:
        sycl::event event_;
        std::span<const uint64_t> data_;
    };

    Renderer();
    ~Renderer();
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer(Renderer&& other) noexcept = default;
    Renderer& operator=(Renderer&& other) noexcept = default;

    Result render();

private:
    sycl::queue queue_kernel_{sycl::property::queue::in_order{}};
    sycl::queue queue_copy_{sycl::property::queue::in_order{}};
    std::array<SyclBufferDevice<uint64_t>, 2> pixels_ = {{queue_kernel_, queue_kernel_}};
    std::array<SyclBufferHost<uint64_t>, 2> pixels_host_ = {{{{}, queue_kernel_}, {{}, queue_kernel_}}};
    int last_pixels_buffer_ = 0;
};

}  // namespace verre::core
