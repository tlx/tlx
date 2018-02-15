/*******************************************************************************
 * tlx/algorithm/exclusive_scan.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Michael Axtmann <michael.axtmann@kit.edu>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_ALGORITHM_EXCLUSIVE_SCAN_HEADER
#define TLX_ALGORITHM_EXCLUSIVE_SCAN_HEADER

#include <iterator>

namespace tlx {

template <typename InputIterator,
          typename OutputIterator,
          typename T,
          typename BinaryOperation>
OutputIterator exclusive_scan(InputIterator first, InputIterator last,
                              OutputIterator result,
                              T init, BinaryOperation binary_op) {
    *result++ = init;
    if (first == last) {
        typename std::iterator_traits<InputIterator>::value_type value =
            binary_op(init, *first);
        *result = value;
        while (++first != last) {
            value = binary_op(value, *first);
            *++result = value;
        }
        ++result;
    }
    return result;
}

template <typename InputIterator, typename OutputIterator, typename T>
OutputIterator exclusive_scan(InputIterator first, InputIterator last,
                              OutputIterator result, T init) {
    *result++ = init;
    if (first != last) {
        typename std::iterator_traits<InputIterator>::value_type value =
            init + *first;
        *result = value;
        while (++first != last) {
            value = value + *first;
            *++result = value;
        }
        ++result;
    }
    return result;
}

} // namespace tlx

#endif // !TLX_ALGORITHM_EXCLUSIVE_SCAN_HEADER

/******************************************************************************/
