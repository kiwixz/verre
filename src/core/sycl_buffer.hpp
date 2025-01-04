#pragma once

#include <span>

#include <sycl/sycl.hpp>

#include "utils/assume.hpp"

namespace verre::core {

template <typename T, sycl::usm::alloc kind>
struct SyclBuffer {
    SyclBuffer() = default;

    ~SyclBuffer() {
        free();
    }

    SyclBuffer(const SyclBuffer&) = delete;
    SyclBuffer& operator=(const SyclBuffer&) = delete;

    SyclBuffer(SyclBuffer&& other) noexcept {
        *this = std::move(other);
    }

    SyclBuffer& operator=(SyclBuffer&& other) noexcept {
        free();
        ptr_ = other.ptr_;
        size_ = other.size_;
        other.size_ = 0;
        return *this;
    }

    operator std::span<const T>() const {
        return span();
    }
    operator std::span<T>() {
        return span();
    }

    T& operator[](size_t i) const {
        return span()[i];
    }
    T& operator[](size_t i) {
        return span()[i];
    }

    std::span<const T> span() const {
        return {data(), size_};
    }
    std::span<T> span() {
        return {data(), size_};
    }

    const T* data() const {
        assume(size_ > 0);
        return ptr_;
    }
    T* data() {
        assume(size_ > 0);
        return ptr_;
    }

    size_t size() const {
        return size_;
    }

    void resize(size_t new_size, sycl::queue& queue) {
        if (new_size == size_)
            return;

        free();
        context_ = queue.get_context();
        ptr_ = sycl::malloc<T>(new_size, queue, kind);
        size_ = new_size;
    }

private:
    sycl::context context_;
    T* ptr_;
    size_t size_ = 0;

    void free() {
        if (size_ > 0)
            sycl::free(ptr_, context_);
        size_ = 0;
    }
};


template <typename T>
using SyclBufferHost = SyclBuffer<T, sycl::usm::alloc::host>;

template <typename T>
using SyclBufferShared = SyclBuffer<T, sycl::usm::alloc::shared>;

template <typename T>
using SyclBufferDevice = SyclBuffer<T, sycl::usm::alloc::device>;

}  // namespace verre::core
