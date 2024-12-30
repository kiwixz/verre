#pragma once

#include <span>
#include <vector>

#include <sycl/sycl.hpp>

#include "utils/assume.hpp"

namespace verre::core {

template <typename T>
using SyclBufferHost = std::vector<T, sycl::usm_allocator<T, sycl::usm::alloc::host>>;

template <typename T>
using SyclBufferShared = std::vector<T, sycl::usm_allocator<T, sycl::usm::alloc::shared>>;


template <typename T>
struct SyclBufferDevice {
    SyclBufferDevice(const sycl::queue& queue) :
            queue_{queue} {
    }

    ~SyclBufferDevice() {
        free();
    }

    SyclBufferDevice(const SyclBufferDevice&) = delete;
    SyclBufferDevice& operator=(const SyclBufferDevice&) = delete;

    SyclBufferDevice(SyclBufferDevice&& other) noexcept {
        *this = std::move(other);
    }

    SyclBufferDevice& operator=(SyclBufferDevice&& other) noexcept {
        free();
        ptr_ = other.ptr_;
        size_ = other.size_;
        other.size_ = 0;
        return *this;
    }

    std::span<const T> device_span() const {
        return {device_data(), size_};
    }
    std::span<T> device_span() {
        return {device_data(), size_};
    }

    const T* device_data() const {
        assume(size_ > 0);
        return ptr_;
    }
    T* device_data() {
        assume(size_ > 0);
        return ptr_;
    }

    size_t size() const {
        return size_;
    }

    void resize(size_t new_size) {
        if (new_size == size_)
            return;

        free();
        ptr_ = sycl::malloc_device<T>(new_size, queue_);
        size_ = new_size;
    }

private:
    sycl::queue queue_;
    T* ptr_;
    size_t size_ = 0;

    void free() {
        if (size_ > 0)
            sycl::free(ptr_, queue_);
    }
};

}  // namespace verre::core
