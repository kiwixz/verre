#pragma once

#include <sycl/sycl.hpp>

namespace verre::core {

template <typename Range>
sycl::event zero(Range&& range, sycl::queue& queue) {
    return queue.fill(range.data(), decltype(auto{*range.data()}){}, range.size());
}

}  // namespace verre::core
