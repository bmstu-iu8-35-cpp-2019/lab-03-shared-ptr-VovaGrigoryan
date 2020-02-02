// Copyright 2018 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <atomic>
#include <exception>
#include <iostream>
#include <utility>

class Counter {
private:
    std::atomic_int counter;

public:
    Counter() : counter(0){}

    void inc() {
        counter++;
    }

    void dec() {
        counter--;
    }

    int get() {
        return counter;
    }

    ~Counter() = default;
};

template<typename T>
class SharedPtr {
private:
    T *data;
    Counter *control;

public:
    SharedPtr() {
        data = nullptr;
        control = nullptr;
    }

    SharedPtr(const SharedPtr &r) {
        control = r.control;
        if (r == true)
            control->inc();

        data = r.data;
    }

    SharedPtr(SharedPtr &&r) noexcept {
        control = r.control;
        if (r == true)
            control->inc();

        data = r.data;
    }

    explicit SharedPtr(T *ptr) {
        control = new Counter();
        control->inc();
        data = ptr;
    }

    auto operator=(const SharedPtr &r) -> SharedPtr & {
        if (*this == true) {
            control->dec();
            if (control->get() == 0) {
                delete control;
                delete data;
            }
        }
        control = r.control;
        data = r.data;
        if (*this == true)
            control->inc();

        return *this;
    }

    auto operator=(SharedPtr &&r) -> SharedPtr & {
        if (*this == true) {
            control->dec();
            if (control->get() == 0) {
                delete control;
                delete data;
            }
        }
        control = r.control;
        data = r.data;
        if (*this == true)
            control->inc();

        return *this;
    }

    operator bool() const {
        return control != nullptr;
    }

    auto operator*() const -> T & {
        if (*this == false)
            throw std::invalid_argument("no object");

        return *data;
    }

    auto operator->() const -> T * {
        return data;
    }

    auto get() -> T * {
        return data;
    }

    void reset() {
        if (*this == true) {
            control->dec();
            if (control->get() == 0) {
                delete data;
                delete control;
            }
            data = nullptr;
            control = nullptr;
        }
    }

    void reset(T *ptr) {
        if (*this == true) {
            control->dec();
            if (control->get() == 0) {
                delete data;
                delete control;
            }
            control = new Counter();
            control->inc();
            data = ptr;
        }
    }

    void swap(SharedPtr &r) {
        if (data != r.data)
            std::swap(r, *this);
    }

    auto use_count() const -> size_t {
        if (*this == false)
            return 0;

        return static_cast<size_t>(control->get());
    }

    ~SharedPtr() {
        if (*this == true) {
            control->dec();
            if (control->get() == 0) {
                delete control;
                delete data;
            }
        }
    }
};

#endif // INCLUDE_HEADER_HPP_
