// cmcstl2 - A concept-enabled C++ standard library
//
//  Copyright Casey Carter 2015
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/caseycarter/cmcstl2
//
#ifndef STL2_DETAIL_TEMPORARY_VECTOR_HPP
#define STL2_DETAIL_TEMPORARY_VECTOR_HPP

#include <memory>
#include <stl2/type_traits.hpp>
#include <stl2/utility.hpp>
#include <stl2/detail/construct_destruct.hpp>
#include <stl2/detail/fwd.hpp>
#include <stl2/detail/algorithm/for_each.hpp>
#include <stl2/detail/concepts/object.hpp>

STL2_OPEN_NAMESPACE {
  namespace detail {
    struct temporary_buffer_deleter {
      void operator()(auto* ptr) const {
        std::return_temporary_buffer(ptr);
      }
    };

    template <class T>
    struct temporary_buffer {
      std::unique_ptr<T, temporary_buffer_deleter> alloc_;
      std::ptrdiff_t size_ = 0;

      temporary_buffer() = default;
      temporary_buffer(pair<T*, std::ptrdiff_t> buf) :
        alloc_{buf.first}, size_{buf.second} {}
      temporary_buffer(std::ptrdiff_t n) :
        temporary_buffer(std::get_temporary_buffer<T>(n)) {}

      std::ptrdiff_t size() const {
        return size_;
      }
    };

    template <Destructible T>
    class temporary_vector {
      T* begin_ = nullptr;
      T* end_ = nullptr;
      T* alloc_ = nullptr;

      void _clear() noexcept {
        __stl2::for_each(begin_, end_, destruct);
      }

    public:
      using value_type = T;

      ~temporary_vector() {
        _clear();
      }

      temporary_vector() = default;
      temporary_vector(temporary_buffer<T>& buf) :
        begin_{buf.alloc_.get()}, end_{begin_},
        alloc_{begin_ + buf.size_} {}
      temporary_vector(temporary_vector&&) = delete;
      temporary_vector& operator=(temporary_vector&& that) = delete;

      void clear() noexcept {
        _clear();
        end_ = begin_;
      }

      constexpr bool empty() const noexcept {
        return begin_ == end_;
      }

      constexpr std::ptrdiff_t capacity() const noexcept {
        return alloc_ - begin_;
      }
      constexpr std::ptrdiff_t size() const noexcept {
        return end_ - begin_;
      }

      constexpr T* begin() noexcept { return begin_; }
      constexpr T* end() noexcept { return end_; }
      constexpr const T* begin() const noexcept { return begin_; }
      constexpr const T* end() const noexcept { return end_; }

      constexpr T& operator[](std::ptrdiff_t i) noexcept {
        STL2_ASSERT(0 <= i && i < end_ - begin_);
        return begin_[i];
      }

      template <class...Args>
        requires Constructible<T, Args...>()
      void emplace_back(Args&&...args)
        noexcept(is_nothrow_constructible<T, Args...>::value) {
        STL2_ASSERT(end_ < alloc_);
        __stl2::detail::construct(*end_, std::forward<Args>(args)...);
        ++end_;
      }
      void push_back(const T& t)
        noexcept(is_nothrow_copy_constructible<T>::value)
        requires CopyConstructible<T>() {
        emplace_back(t);
      }
      void push_back(T&& t)
        noexcept(is_nothrow_move_constructible<T>::value)
        requires MoveConstructible<T>() {
        emplace_back(__stl2::move(t));
      }
    };

    Destructible{T}
    temporary_vector<T> make_temporary_vector(temporary_buffer<T>& buf) {
      return {buf};
    }
  }
} STL2_CLOSE_NAMESPACE

#endif