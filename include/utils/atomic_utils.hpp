#pragma once

#include <atomic>

template<typename T>
bool compare_and_swap(std::atomic<T>& obj, T& expected, T desired) {
    return obj.compare_exchange_weak(expected, desired);
}
