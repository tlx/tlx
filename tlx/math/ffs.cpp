/*******************************************************************************
 * tlx/math/ffs.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/math/ffs.hpp>

namespace tlx {

//! ffs (find first set bit) - generic implementation
template <typename Integral>
static inline unsigned ffs_template(Integral x) {
    if (x == 0) return 0u;
    unsigned r = 1;
    while ((x & 1) == 0)
        x >>= 1, ++r;
    return r;
}

/******************************************************************************/

#if defined(__GNUC__) || defined(__clang__)

unsigned ffs(int i) {
    return static_cast<unsigned>(__builtin_ffs(i));
}
unsigned ffs(unsigned i) {
    return ffs(static_cast<int>(i));
}
unsigned ffs(long i) {
    return static_cast<unsigned>(__builtin_ffsl(i));
}
unsigned ffs(unsigned long i) {
    return ffs(static_cast<long>(i));
}
unsigned ffs(long long i) {
    return static_cast<unsigned>(__builtin_ffsll(i));
}
unsigned ffs(unsigned long long i) {
    return ffs(static_cast<long long>(i));
}

#else

unsigned ffs(int i) {
    return ffs_template(i);
}
unsigned ffs(unsigned int i) {
    return ffs_template(i);
}
unsigned ffs(long i) {
    return ffs_template(i);
}
unsigned ffs(unsigned long i) {
    return ffs_template(i);
}
unsigned ffs(long long i) {
    return ffs_template(i);
}
unsigned ffs(unsigned long long i) {
    return ffs_template(i);
}

#endif

} // namespace tlx

/******************************************************************************/
