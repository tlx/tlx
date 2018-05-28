/*******************************************************************************
 * tests/container/lru_cache_test.cpp
 *
 * Borrowed from https://github.com/lamerman/cpp-lru-cache by Alexander
 * Ponomarev under BSD license and modified for Thrill's block pool.
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2016-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/container/lru_cache.hpp>
#include <tlx/die.hpp>

namespace tlx {

// instantiations
template class tlx::LruCacheSet<size_t>;
template class tlx::LruCacheMap<size_t, size_t>;

} // namespace tlx

/******************************************************************************/
// LruCacheSet

static void test_set_simple_put() {
    tlx::LruCacheSet<size_t> cache;
    cache.put(7);
    die_unless(cache.exists(7));
    die_unequal(1u, cache.size());
}

static void test_set_missing_value() {
    tlx::LruCacheSet<size_t> cache;
    die_unless_throws(cache.touch(7), std::range_error);
}

static void test_set_keep_all_values_within_capacity() {
    tlx::LruCacheSet<size_t> cache;

    static constexpr size_t test_size = 100;
    static constexpr size_t capacity = 50;

    // put more items into cache than capacity
    for (size_t i = 0; i < test_size; ++i) {
        cache.put(i);

        while (cache.size() > capacity) {
            auto x = cache.pop();
            die_unequal(x, i - capacity);
        }
    }

    // check state of cache
    for (size_t i = 0; i < test_size - capacity; ++i) {
        die_if(cache.exists(i));
    }
    for (size_t i = test_size - capacity; i < test_size; ++i) {
        die_unless(cache.exists(i));
    }

    die_unequal(capacity, cache.size());

    // touch three existing keys, only two exist

    cache.touch(70);
    cache.put(75);
    die_unless(cache.touch_if_exists(80));
    die_if(cache.touch_if_exists(20));

    // erase some keys

    cache.erase(90);
    die_unless(cache.erase_if_exists(95));
    die_if(cache.erase_if_exists(45));

    // check contents of cache

    die_unequal(capacity - 2, cache.size());

    for (size_t i = 0; i < test_size - capacity; ++i) {
        die_if(cache.exists(i));
    }
    for (size_t i = test_size - capacity; i < test_size; ++i) {
        if (i == 90 || i == 95) {
            die_if(cache.exists(i));
        }
        else {
            die_unless(cache.exists(i));
        }
    }

    // check order in which items are evicted, 70 and 80 must be at the end

    die_unequal(capacity - 2, cache.size());

    for (size_t i = 50; i < 100; ++i) {
        if (i == 70 || i == 75 || i == 80 || i == 90 || i == 95) ++i;
        die_unequal(cache.pop(), i);
    }

    die_unequal(cache.pop(), 70u);
    die_unequal(cache.pop(), 75u);
    die_unequal(cache.pop(), 80u);
    die_unequal(0u, cache.size());
}

/******************************************************************************/
// LruCacheMap

static void test_map_simple_put() {
    tlx::LruCacheMap<size_t, size_t> cache;
    cache.put(7, 777);
    die_unless(cache.exists(7));
    die_unequal(777u, cache.get(7));
    die_unequal(1u, cache.size());
}

static void test_map_missing_value() {
    tlx::LruCacheMap<size_t, size_t> cache;
    die_unless_throws(cache.get(7), std::range_error);
    die_unless_throws(cache.touch(7), std::range_error);
}

static void test_map_keep_all_values_within_capacity() {
    tlx::LruCacheMap<size_t, size_t> cache;

    static constexpr size_t test_size = 100;
    static constexpr size_t capacity = 50;

    // put more items into cache than capacity
    for (size_t i = 0; i < test_size; ++i) {
        cache.put(i, i);

        while (cache.size() > capacity) {
            auto x = cache.pop();
            die_unequal(x.first, i - capacity);
        }
    }

    // check state of cache
    for (size_t i = 0; i < test_size - capacity; ++i) {
        die_if(cache.exists(i));
    }
    for (size_t i = test_size - capacity; i < test_size; ++i) {
        die_unless(cache.exists(i));
        die_unequal(i, cache.get(i));
    }

    die_unequal(capacity, cache.size());

    // touch three existing keys, only two exist

    cache.touch(70);
    cache.put(75, 75);
    die_unless(cache.touch_if_exists(80));
    die_if(cache.touch_if_exists(20));

    // erase some keys

    cache.erase(90);
    die_unless(cache.erase_if_exists(95));
    die_if(cache.erase_if_exists(45));

    // check contents of cache

    die_unequal(capacity - 2, cache.size());

    for (size_t i = 0; i < test_size - capacity; ++i) {
        die_if(cache.exists(i));
    }
    for (size_t i = test_size - capacity; i < test_size; ++i) {
        if (i == 90 || i == 95) {
            die_if(cache.exists(i));
        }
        else {
            die_unless(cache.exists(i));
            die_unequal(i, cache.get(i));
        }
    }

    // check order in which items are evicted, 70 and 80 must be at the end

    die_unequal(capacity - 2, cache.size());

    for (size_t i = 50; i < 100; ++i) {
        if (i == 70 || i == 75 || i == 80 || i == 90 || i == 95) ++i;
        die_unequal(cache.pop().first, i);
    }

    die_unequal(cache.pop().first, 70u);
    die_unequal(cache.pop().first, 75u);
    die_unequal(cache.pop().first, 80u);
    die_unequal(0u, cache.size());
}

int main() {

    test_set_simple_put();
    test_set_missing_value();
    test_set_keep_all_values_within_capacity();

    test_map_simple_put();
    test_map_missing_value();
    test_map_keep_all_values_within_capacity();

    return 0;
}

/******************************************************************************/
