/*******************************************************************************
 * tlx/algorithm/exclusive_scan.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Michael Axtmann <michael.axtmann@kit.edu>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_EXCLUSIVE_SCAN_HEADER
#define TLX_EXCLUSIVE_SCAN_HEADER

#include <iterator>

namespace tlx {

template< typename InputIterator,
          typename OutputIterator,
          typename T,
          typename BinaryOperation>
OutputIterator exclusive_scan(InputIterator first, InputIterator last, OutputIterator result,
                              T init, BinaryOperation binary_op) {
    *result++ = init;
    if (first != last) {
        typename std::iterator_traits<InputIterator>::value_type val = binary_op(init, *first);
        *result = val;
        while (++first != last) {
            val = binary_op(val, *first);
            *++result = val;
        }
        ++result;
    }
    return result;
}

template<typename InputIterator, typename OutputIterator, typename T>
OutputIterator exclusive_scan(InputIterator first, InputIterator last,
                              OutputIterator result, T init) {
    *result++ = init;
    if (first != last) {
        typename std::iterator_traits<InputIterator>::value_type val = init + *first;
        *result = val;
        while (++first != last) {
            val = val + *first;
            *++result = val;
        }
        ++result;
    }
    return result;
}

} // namespace tlx

#endif // !TLX_EXCLUSIVE_SCAN_HEADER
