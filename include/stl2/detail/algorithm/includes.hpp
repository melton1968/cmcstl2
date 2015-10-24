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
#ifndef STL2_DETAIL_ALGORITHM_INCLUDES_HPP
#define STL2_DETAIL_ALGORITHM_INCLUDES_HPP

#include <stl2/functional.hpp>
#include <stl2/iterator.hpp>
#include <stl2/detail/fwd.hpp>
#include <stl2/detail/concepts/callable.hpp>

///////////////////////////////////////////////////////////////////////////
// includes [includes]
//
STL2_OPEN_NAMESPACE {
  template <InputIterator I1, Sentinel<I1> S1, InputIterator I2, Sentinel<I2> S2,
            class Proj1 = identity, class Proj2 = identity,
            IndirectCallableStrictWeakOrder<Projected<I1, Proj1>, Projected<I2, Proj2>> Comp = less<>>
  bool includes(I1 first1, S1 last1, I2 first2, S2 last2, Comp&& comp_ = Comp{},
                Proj1&& proj1_ = Proj1{}, Proj2&& proj2_ = Proj2{}) {
    auto comp = ext::make_callable_wrapper(__stl2::forward<Comp>(comp_));
    auto proj1 = ext::make_callable_wrapper(__stl2::forward<Proj1>(proj1_));
    auto proj2 = ext::make_callable_wrapper(__stl2::forward<Proj2>(proj2_));

    while (true) {
      if (first2 == last2) {
        return true;
      }
      if (first1 == last1) {
        return false;
      }
      if (comp(proj2(*first2), proj1(*first1))) {
        return false;
      }
      if (!comp(proj1(*first1), proj2(*first2))) {
        ++first2;
      }
      ++first1;
    }
  }

  template <InputRange Rng1, InputRange Rng2, class Proj1 = identity,
            class Proj2 = identity,
            IndirectCallableStrictWeakOrder<Projected<IteratorType<Rng1>, Proj1>,
              Projected<IteratorType<Rng2>, Proj2>> Comp = less<>>
  bool includes(Rng1&& rng1, Rng2&& rng2, Comp&& comp = Comp{},
                Proj1&& proj1 = Proj1{}, Proj2&& proj2 = Proj2{}) {
    return __stl2::includes(
      __stl2::begin(rng1), __stl2::end(rng1),
      __stl2::begin(rng2), __stl2::end(rng2),
      __stl2::forward<Comp>(comp),
      __stl2::forward<Proj1>(proj1),
      __stl2::forward<Proj2>(proj2));
  }
} STL2_CLOSE_NAMESPACE

#endif
