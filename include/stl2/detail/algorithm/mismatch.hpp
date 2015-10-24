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
#ifndef STL2_DETAIL_ALGORITHM_MISMATCH_HPP
#define STL2_DETAIL_ALGORITHM_MISMATCH_HPP

#include <stl2/functional.hpp>
#include <stl2/iterator.hpp>
#include <stl2/utility.hpp>
#include <stl2/detail/fwd.hpp>
#include <stl2/detail/concepts/callable.hpp>

///////////////////////////////////////////////////////////////////////////
// mismatch [mismatch]
//
STL2_OPEN_NAMESPACE {
  template <InputIterator I1, Sentinel<I1> S1,
            InputIterator I2, Sentinel<I2> S2,
            class Proj1 = identity, class Proj2 = identity,
            IndirectCallablePredicate<Projected<I1, Proj1>,
                                      Projected<I2, Proj2>> Pred = equal_to<>>
  tagged_pair<tag::in1(I1), tag::in2(I2)>
  mismatch(I1 first1, S1 last1, I2 first2, S2 last2, Pred&& pred_ = Pred{},
           Proj1&& proj1_ = Proj1{}, Proj2&& proj2_ = Proj2{}) {
    auto pred = ext::make_callable_wrapper(__stl2::forward<Pred>(pred_));
    auto proj1 = ext::make_callable_wrapper(__stl2::forward<Proj1>(proj1_));
    auto proj2 = ext::make_callable_wrapper(__stl2::forward<Proj2>(proj2_));

    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
      if (!pred(proj1(*first1), proj2(*first2))) {
        break;
      }
    }
    return {first1, first2};
  }

  template <InputRange Rng1, InputRange Rng2,
            class Proj1 = identity, class Proj2 = identity,
            IndirectCallablePredicate<Projected<IteratorType<Rng1>, Proj1>,
                                      Projected<IteratorType<Rng2>, Proj2>> Pred = equal_to<>>
  tagged_pair<tag::in1(safe_iterator_t<Rng1>), tag::in2(safe_iterator_t<Rng2>)>
  mismatch(Rng1&& rng1, Rng2&& rng2, Pred&& pred = Pred{},
           Proj1&& proj1 = Proj1{}, Proj2&& proj2 = Proj2{}) {
    return __stl2::mismatch(
      __stl2::begin(rng1), __stl2::end(rng1),
      __stl2::begin(rng2), __stl2::end(rng2),
      __stl2::forward<Pred>(pred),
      __stl2::forward<Proj1>(proj1),
      __stl2::forward<Proj2>(proj2));
  }

  template <InputIterator I1, Sentinel<I1> S1, WeakInputIterator I2,
            class Proj1 = identity, class Proj2 = identity,
            IndirectCallablePredicate<Projected<I1, Proj1>,
                                      Projected<I2, Proj2>> Pred = equal_to<>>
  tagged_pair<tag::in1(I1), tag::in2(I2)>
  mismatch(I1 first1, S1 last1, I2 first2, Pred&& pred_ = Pred{},
           Proj1&& proj1_ = Proj1{}, Proj2&& proj2_ = Proj2{}) {
    auto pred = ext::make_callable_wrapper(__stl2::forward<Pred>(pred_));
    auto proj1 = ext::make_callable_wrapper(__stl2::forward<Proj1>(proj1_));
    auto proj2 = ext::make_callable_wrapper(__stl2::forward<Proj2>(proj2_));

    for (; first1 != last1; ++first1, ++first2) {
      if (!pred(proj1(*first1), proj2(*first2))) {
        break;
      }
    }
    return {first1, first2};
  }

  template <InputRange Rng1, class I2_, class Proj1 = identity,
            class Proj2 = identity, WeakInputIterator I2 = __uncvref<I2_>,
            IndirectCallablePredicate<Projected<IteratorType<Rng1>, Proj1>,
                                      Projected<I2, Proj2>> Pred = equal_to<>>
  tagged_pair<tag::in1(safe_iterator_t<Rng1>), tag::in2(I2)>
  mismatch(Rng1&& rng1, I2_&& first2, Pred&& pred = Pred{},
           Proj1&& proj1 = Proj1{}, Proj2&& proj2 = Proj2{}) {
    return __stl2::mismatch(
      __stl2::begin(rng1), __stl2::end(rng1), first2,
      __stl2::forward<Pred>(pred), __stl2::forward<Proj1>(proj1),
      __stl2::forward<Proj2>(proj2));
  }
} STL2_CLOSE_NAMESPACE

#endif
