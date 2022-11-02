#pragma once

#include <cstddef>
#include <vector>

template <typename T, class Comp = std::less<T>>
class Heap {
public:
    void Push(const T& val) {
        heap_.push_back(val);
        for (size_t i = heap_.size() - 1; i != 0;) {
            if (Comp()(heap_[i], heap_[Parent(i)])) {
                std::swap(heap_[i], heap_[Parent(i)]);
                i = Parent(i);
            } else {
                break;
            }
        }
    }

    T Pop() {
        T result = heap_.front();
        std::swap(heap_.front(), heap_.back());
        heap_.pop_back();
        for (size_t i = 0; Left(i) < heap_.size();) {
            size_t son = (Right(i) >= heap_.size() || Comp()(heap_[Left(i)], heap_[Right(i)]) ? Left(i) : Right(i));
            if (Comp()(heap_[son], heap_[i])) {
                std::swap(heap_[son], heap_[i]);
                i = son;
            } else {
                break;
            }
        }
        return result;
    }

    T Top() const {
        return heap_.front();
    }

    size_t Size() const {
        return heap_.size();
    }

private:
    static inline size_t Parent(size_t i) {
        return (i - 1) >> 1;
    }

    static inline size_t Left(size_t i) {
        return i << 1 | 1;
    }

    static inline size_t Right(size_t i) {
        return (i << 1) + 2;
    }

    std::vector<T> heap_;
};
