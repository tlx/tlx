/*******************************************************************************
 * tests/container/radix_heap_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Manuel Penschuck <tlx@manuel.jetzt>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <algorithm>
#include <cstdint>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <random>
#include <tuple>
#include <vector>

#include <tlx/container/radix_heap.hpp>
#include <tlx/die.hpp>
#include <tlx/logger.hpp>

/******************************************************************************/

// Idiot-proof reference implementation of flag field against which we test
class naive_bitarray
{
public:
    naive_bitarray(size_t n) : bits_(n, false) { }

    void set_bit(size_t i) {
        bits_.at(i) = true;
    }

    void clear_bit(size_t i) {
        bits_.at(i) = false;
    }

    bool is_set(size_t i) const {
        return bits_.at(i);
    }

    void clear_all() {
        for (size_t i = 0; i < bits_.size(); i++)
            bits_[i] = false;
    }

    bool empty() {
        for (size_t i = 0; i < bits_.size(); i++)
            if (bits_[i])
                return false;

        return true;
    }

    size_t find_lsb() {
        for (size_t i = 0; i < bits_.size(); i++)
            if (bits_[i])
                return i;

        die("find_lsb on empty field");
    }

    void swap(naive_bitarray& o) {
        std::swap(bits_, o.bits_);
    }

protected:
    std::vector<bool> bits_;
};

// Perform the same tests for a number of different compile-time settings
template <typename Field, size_t Size>
void bitarray_test(std::mt19937& rng) {
    Field f;

    // set each (for large instances, a random subset) bit, search it, clear it
    {
        std::uniform_int_distribution<int> skip_dist(0, Size / 4096);

        for (size_t i = 0; i < Size; i++) {
            die_unless(f.empty());
            f.set_bit(i);
            die_if(f.empty());
            die_unequal(i, f.find_lsb());
            f.clear_bit(i);
            die_unless(f.empty());

            if (Size > 64 * 64 + 1) {
                i += skip_dist(rng);
            }
        }
    }

    Field f0;
    std::swap(f0, f);

    // randomly flip bits
    std::uniform_int_distribution<unsigned> distr(0, Size - 1);
    std::uniform_real_distribution<float> rdist;

    naive_bitarray nf(Size);
    for (size_t i = 0; i < std::min<size_t>(100, Size * 10); i++) {
        const auto idx = distr(rng);

        die_unequal(nf.is_set(idx), f.is_set(idx));

        if (nf.is_set(idx)) {
            nf.clear_bit(idx);
            f.clear_bit(idx);
        }
        else {
            nf.set_bit(idx);
            f.set_bit(idx);
        }

        die_unequal(nf.empty(), f.empty());

        if (f.empty()) continue;

        die_unequal(nf.find_lsb(), f.find_lsb());

        // clear lsb to increase chance of observing changes on more
        // significant bits
        if (rdist(rng) > 0.3) {
            const auto first = f.find_lsb();
            nf.clear_bit(first);
            f.clear_bit(first);
        }
    }
    f.set_bit(0);
    die_unless(f0.empty());

    Field f1;
    die_unless(f1.empty());
    std::swap(f1, f);
    die_unless(f.empty());
    die_if(f1.empty());

    f1.clear_all();
    die_unless(f1.empty());
}

void test_main_bitarray(std::mt19937& rng) {
    bitarray_test<tlx::radix_heap_detail::BitArrayRecursive<32, true>, 32>(rng);
    bitarray_test<tlx::radix_heap_detail::BitArrayRecursive<64, true>, 64>(rng);

    // 1 layer
    bitarray_test<tlx::radix_heap_detail::BitArray<32>, 32>(rng);
    bitarray_test<tlx::radix_heap_detail::BitArray<33>, 33>(rng);
    bitarray_test<tlx::radix_heap_detail::BitArray<63>, 63>(rng);
    bitarray_test<tlx::radix_heap_detail::BitArray<64>, 64>(rng);

    // 2 layers
    bitarray_test<tlx::radix_heap_detail::BitArray<65>, 65>(rng);
    bitarray_test<tlx::radix_heap_detail::BitArray<500>, 500>(rng);
    bitarray_test<tlx::radix_heap_detail::BitArray<1001>, 1001>(rng);
    bitarray_test<tlx::radix_heap_detail::BitArray<4004>, 4004>(rng);
    bitarray_test<tlx::radix_heap_detail::BitArray<4096>, 4096>(rng);

    // 3 layers
    bitarray_test<tlx::radix_heap_detail::BitArray<64 * 64 + 1>, 64 * 64 + 1>(
        rng);
    bitarray_test<tlx::radix_heap_detail::BitArray<1 << 18>, 1 << 18>(rng);
}

/******************************************************************************/

template <typename T>
void int_rank_test(std::mt19937& prng) {
    using IR = tlx::radix_heap_detail::IntegerRank<T>;

    constexpr T min = std::numeric_limits<T>::min();
    constexpr T max = std::numeric_limits<T>::max();
    constexpr T zero = 0;
    constexpr T one = 1;

    // Check that int_at_rank and rank_of_int are inverse
    die_unequal(min, IR::int_at_rank(IR::rank_of_int(min)));
    die_unequal(min + one, IR::int_at_rank(IR::rank_of_int(min + one)));
    die_unequal(zero, IR::int_at_rank(IR::rank_of_int(zero)));
    die_unequal(one, IR::int_at_rank(IR::rank_of_int(one)));
    die_unequal(max - one, IR::int_at_rank(IR::rank_of_int(max - one)));
    die_unequal(max, IR::int_at_rank(IR::rank_of_int(max)));

    // Check that random pairs keep their relative order
    std::uniform_int_distribution<T> dist;
    for (size_t i = 0; i < 1000; i++) {
        // draw two random numbers a, b s.t. a < b
        T a = dist(prng);
        T b;
        do {
            b = dist(prng);
        } while (b == a);
        if (a > b) std::swap(a, b);

        die_unless(IR::rank_of_int(a) < IR::rank_of_int(b));

        // Check inverse for random numbers
        die_unequal(a, IR::int_at_rank(IR::rank_of_int(a)));
        die_unequal(b, IR::int_at_rank(IR::rank_of_int(b)));
    }
}

void test_main_int_rank(std::mt19937& prng) {
    int_rank_test<std::uint16_t>(prng);
    int_rank_test<std::uint32_t>(prng);
    int_rank_test<std::uint64_t>(prng);

    int_rank_test<std::int16_t>(prng);
    int_rank_test<std::int32_t>(prng);
    int_rank_test<std::int64_t>(prng);
}

/******************************************************************************/

//! Check that the computation of lower- and upper bounds of each bucket
//! works properly, and that they are mapped back to bucket indices properly
template <unsigned Radix, typename T>
void test_bucket_bounds() {
    constexpr bool debug = false;
    using Comp = tlx::radix_heap_detail::BucketComputation<Radix, T>;
    Comp comp;

    LOG << "test_bucket_bounds<"
        "Radix=" << Radix << ", sizeof(T)=" << sizeof(T) << ">: "
        "num_buckets=" << size_t(comp.num_buckets) << "\n";

    die_unequal(comp.lower_bound(0u), std::numeric_limits<T>::min());
    die_unequal(comp.upper_bound(comp.num_buckets - 1u),
                std::numeric_limits<T>::max());

    for (size_t i = 1; i < comp.num_buckets; i++) {
        die_unless(comp.lower_bound(i - 1) < comp.lower_bound(i));
    }

    for (size_t i = 0; i < comp.num_buckets; i++) {
        const auto lb = comp.lower_bound(i);
        const auto ub = comp.upper_bound(i);
        const auto mid = (ub - lb) / 2 + lb;

        die_unless(lb >= ub - lb);

        die_unless(lb <= mid);
        die_unless(mid <= ub);

        die_unequal(comp(lb, 0u), i);
        die_unequal(comp(mid, 0u), i);
        die_unequal(comp(ub, 0u), i);
    }
}

void test_main_bucket(std::mt19937&) {
    test_bucket_bounds<2, std::uint32_t>();
    test_bucket_bounds<8, std::uint32_t>();
    test_bucket_bounds<64, std::uint32_t>();
    test_bucket_bounds<2, std::uint64_t>();
    test_bucket_bounds<8, std::uint64_t>();
    test_bucket_bounds<64, std::uint64_t>();
}

/******************************************************************************/

namespace tlx {

using payload_type = std::uint64_t;
using value_type = std::tuple<std::int64_t, payload_type>;

class KeyExtractor
{
public:
    std::int64_t operator () (const value_type& p) const { return std::get<0>(p); }
};

// force instantiations
template class RadixHeap<value_type, KeyExtractor, std::int64_t, /* Radix */ 64>;

} // namespace tlx

/******************************************************************************/

//! Fill radix heap completely and then check whether values are output
//! in the correct order. This function also checks that the heap is stable
template <typename KeyType, unsigned Radix>
void allin_allout(std::mt19937& prng, KeyType min, KeyType max, size_t n) {
    constexpr bool debug = false;

    LOG << "allin_allout<"
        "sizeof(T)=" << sizeof(KeyType) << ", Radix=" << Radix << ">("
        "min=" << min << ", "
        "max=" << max << ", "
        "n=" << n << ")\n";

    using payload_type = std::uint64_t;
    using value_type = std::tuple<KeyType, payload_type>;

    auto heap = tlx::make_radix_heap<value_type, Radix>(
        [](const value_type& p) { return std::get<0>(p); });

    std::vector<std::pair<KeyType, payload_type> > values;
    values.reserve(n);

    std::uniform_int_distribution<KeyType> distr(min, max);

    die_unless(heap.empty());

    KeyType running_min = std::numeric_limits<KeyType>::max();

    for (size_t i = 0; i < n; i++) {
        die_unequal(heap.size(), i);

        const auto key = distr(prng);
        heap.push(value_type{ key, i });
        values.emplace_back(key, i);

        running_min = std::min(running_min, key);
        die_unequal(running_min, heap.peak_top_key());

        die_if(heap.empty());
    }

    std::sort(values.begin(), values.end());

    for (size_t i = 0; i < n; i++) {
        die_unequal(heap.size(), n - i);

        const auto exp_key = values[i].first;
        die_unequal(heap.peak_top_key(), exp_key);

        die_unequal(std::get<0>(heap.top()), exp_key);
        heap.pop();
    }

    die_unless(heap.empty());
}

//! Fill radix heap completely and then check whether values are output
//! in the correct order. This function also checks that the heap is stable
template <typename KeyType, unsigned Radix>
void allin_allout_pair(std::mt19937& prng, KeyType min, KeyType max, size_t n) {
    constexpr bool debug = false;

    LOG << "allin_allout_pair<"
        "sizeof(T)=" << sizeof(KeyType) << ", Radix=" << Radix << ">("
        "min=" << min << ", "
        "max=" << max << ", "
        "n=" << n << ")\n";

    using payload_type = std::uint64_t;

    tlx::RadixHeapPair<KeyType, payload_type, Radix> heap;
    std::vector<std::pair<KeyType, payload_type> > values;
    values.reserve(n);

    std::uniform_int_distribution<KeyType> distr(min, max);

    die_unless(heap.empty());

    KeyType running_min = std::numeric_limits<KeyType>::max();

    for (size_t i = 0; i < n; i++) {
        die_unequal(heap.size(), i);

        const auto key = distr(prng);
        heap.push({ key, i });
        values.emplace_back(key, i);

        running_min = std::min(running_min, key);
        die_unequal(running_min, heap.peak_top_key());

        die_if(heap.empty());
    }

    std::sort(values.begin(), values.end());

    for (size_t i = 0; i < n; i++) {
        die_unequal(heap.size(), n - i);

        const auto exp_key = values[i].first;
        die_unequal(heap.peak_top_key(), exp_key);

        die_unequal(heap.top().first, exp_key);
        heap.pop();
    }

    die_unless(heap.empty());
}

enum class PullMode {
    Single, AllOfKey, ExtractBucket
};

//! Performs interleaved insertions and pop operations and checks it against
//! the STL priority queue
template <typename KeyType, unsigned Radix>
void random_inout(std::mt19937& prng, const KeyType min, const KeyType max,
                  const KeyType iters, const double insert_prob,
                  const size_t prefill_n, const PullMode pm) {
    constexpr bool debug = false;

    LOG << "random_inout<"
        "sizeof(T)=" << sizeof(KeyType) << ", Radix=" << Radix << ">("
        "min=" << min << ", "
        "max=" << max << ", "
        "iters=" << iters << ", "
        "insert_prob=" << insert_prob << ", "
        "prefill_n=" << prefill_n << ", "
        "pm=" << static_cast<int>(pm) << ")\n";

    using payload_type = std::uint32_t;
    using value_type = std::tuple<KeyType, payload_type>;

    auto heap = tlx::make_radix_heap<value_type, Radix>(
        [](const value_type& p) { return std::get<0>(p); });
    using bucket_type = typename decltype(heap)::bucket_data_type;

    using pq_type = std::pair<KeyType, payload_type>;
    std::priority_queue<pq_type,
                        std::vector<pq_type>,
                        std::greater<pq_type> > pq;

    auto insert = [&](KeyType minv, KeyType maxv) {
                      std::uniform_int_distribution<KeyType> kdistr(minv, maxv);
                      std::uniform_int_distribution<payload_type> pdistr;

                      auto key = kdistr(prng);
                      auto pay = pdistr(prng);
                      pq.emplace(key, pay);
                      heap.emplace(key, key, pay);
                  };

    for (size_t i = 0; i < prefill_n; i++) {
        insert(min, max - iters);
    }

    std::uniform_real_distribution<double> rdist;

    KeyType running_min = min;

    std::vector<payload_type> ref_data, heap_data;

    for (std::int64_t i = 0; i < static_cast<std::int64_t>(iters) || !pq.empty(); i++) {
        while (i < static_cast<std::int64_t>(iters) && rdist(prng) < insert_prob)
            insert(running_min, max - std::max<std::int64_t>(0, iters - i));

        die_unequal(pq.empty(), heap.empty());

        if (pq.empty()) continue;

        if (pm == PullMode::Single) {
            die_unequal(pq.top().first, std::get<0>(heap.top()));
            running_min = std::get<0>(heap.top());
            pq.pop();
            heap.pop();
        }
        else {
            const KeyType key = pq.top().first;
            for (ref_data.clear();
                 !pq.empty() && pq.top().first == key;
                 pq.pop())
            {
                ref_data.push_back(pq.top().second);
            }

            running_min = key;

            die_unequal(std::get<0>(heap.top()), key);

            if (pm == PullMode::AllOfKey) {
                for (heap_data.clear();
                     !heap.empty() && heap.peak_top_key() == key;
                     heap.pop())
                {
                    heap_data.push_back(std::get<1>(heap.top()));
                }
            }
            else {
                bucket_type bucket;
                heap.swap_top_bucket(bucket);
                heap_data.clear();
                heap_data.reserve(bucket.size());

                for (const auto& x : bucket) {
                    heap_data.push_back(std::get<1>(x));
                }
            }

            std::sort(ref_data.begin(), ref_data.end());
            std::sort(heap_data.begin(), heap_data.end());

            die_unequal(ref_data.size(), heap_data.size());

            for (size_t j = 0; j < heap_data.size(); j++)
                die_unequal(heap_data[j], ref_data[j]);
        }
    }
}

//! Performs interleaved insertions and pop operations and checks it against
//! the STL priority queue
template <typename KeyType, unsigned Radix>
void random_inout_pair(std::mt19937& prng,
                       const KeyType min, const KeyType max,
                       const KeyType iters, const double insert_prob,
                       const size_t prefill_n, const PullMode pm) {
    constexpr bool debug = false;

    LOG << "random_inout_pair<"
        "sizeof(T)=" << sizeof(KeyType) << ", Radix=" << Radix << ">("
        "min=" << min << ", "
        "max=" << max << ", "
        "iters=" << iters << ", "
        "insert_prob=" << insert_prob << ", "
        "prefill_n=" << prefill_n << ", "
        "pm=" << static_cast<int>(pm) << ")\n";

    using payload_type = std::uint32_t;
    using pq_type = std::pair<KeyType, payload_type>;
    std::priority_queue<pq_type, std::vector<pq_type>, std::greater<pq_type> > pq;
    using heap_type = tlx::RadixHeapPair<KeyType, payload_type, Radix>;
    using bucket_type = typename
                        tlx::RadixHeapPair<KeyType, payload_type, Radix>::bucket_data_type;
    heap_type heap;

    auto insert = [&](KeyType minv, KeyType maxv) {
                      std::uniform_int_distribution<KeyType> kdistr(minv, maxv);
                      std::uniform_int_distribution<payload_type> pdistr;

                      auto key = kdistr(prng);
                      auto pay = pdistr(prng);
                      pq.emplace(key, pay);
                      heap.emplace_keyfirst(key, pay);
                  };

    for (size_t i = 0; i < prefill_n; i++) {
        insert(min, max - iters);
    }

    std::uniform_real_distribution<double> rdist;

    KeyType running_min = min;

    std::vector<payload_type> ref_data, heap_data;

    for (std::int64_t i = 0; i < static_cast<std::int64_t>(iters) || !pq.empty(); i++) {
        while (i < static_cast<std::int64_t>(iters) && rdist(prng) < insert_prob) {
            insert(running_min,
                   static_cast<KeyType>(max - std::max<std::int64_t>(0, iters - i)));
        }

        die_unequal(pq.empty(), heap.empty());

        if (pq.empty()) continue;

        if (pm == PullMode::Single) {
            die_unequal(pq.top().first, heap.top().first);
            running_min = heap.top().first;
            pq.pop();
            heap.pop();
        }
        else {
            const KeyType key = pq.top().first;
            for (ref_data.clear();
                 !pq.empty() && pq.top().first == key;
                 pq.pop())
            {
                ref_data.push_back(pq.top().second);
            }

            running_min = key;

            die_unequal(heap.top().first, key);

            if (pm == PullMode::AllOfKey) {
                for (heap_data.clear();
                     !heap.empty() && heap.peak_top_key() == key;
                     heap.pop())
                {
                    heap_data.push_back(heap.top().second);
                }
            }
            else {
                bucket_type bucket;
                heap.swap_top_bucket(bucket);
                heap_data.clear();
                heap_data.reserve(bucket.size());

                for (const auto& x : bucket) {
                    heap_data.push_back(x.second);
                }
            }

            std::sort(ref_data.begin(), ref_data.end());
            std::sort(heap_data.begin(), heap_data.end());

            die_unequal(ref_data.size(), heap_data.size());

            for (size_t j = 0; j < heap_data.size(); j++)
                die_unequal(heap_data[j], ref_data[j]);
        }
    }
}

void random_inout_all(std::mt19937& prng,
                      const std::int64_t min, const std::int64_t max,
                      const size_t iters, const double insert_prob,
                      const size_t prefill_n, const PullMode pm) {
#define RADIXHEAP_TESTSET(T)                                                 \
    random_inout<T, 2>(prng, static_cast<T>(min), static_cast<T>(max),       \
                       iters, insert_prob, prefill_n, pm);                   \
    random_inout<T, 64>(prng, static_cast<T>(min), static_cast<T>(max),      \
                        iters, insert_prob, prefill_n, pm);                  \
    random_inout_pair<T, 2>(prng, static_cast<T>(min), static_cast<T>(max),  \
                            iters, insert_prob, prefill_n, pm);              \
    random_inout_pair<T, 64>(prng, static_cast<T>(min), static_cast<T>(max), \
                             iters, insert_prob, prefill_n, pm);

    die_unless(min < max);

    if (min >= 0 && max >= 0) {
        RADIXHEAP_TESTSET(std::uint32_t)
        RADIXHEAP_TESTSET(std::uint64_t)
    }

    RADIXHEAP_TESTSET(std::int32_t)
    RADIXHEAP_TESTSET(std::int64_t)

#undef RADIXHEAP_TESTSET
}

void allin_allout_all(std::mt19937& prng, std::int64_t min, std::int64_t max, size_t n) {
#define RADIXHEAP_TESTSET(T)                                                    \
    allin_allout<T, 2>(prng, static_cast<T>(min), static_cast<T>(max), n);      \
    allin_allout<T, 64>(prng, static_cast<T>(min), static_cast<T>(max), n);     \
    allin_allout_pair<T, 2>(prng, static_cast<T>(min), static_cast<T>(max), n); \
    allin_allout_pair<T, 64>(prng, static_cast<T>(min), static_cast<T>(max), n);

    if (min >= 0) {
        RADIXHEAP_TESTSET(std::uint32_t);
        RADIXHEAP_TESTSET(std::uint64_t);
    }

    RADIXHEAP_TESTSET(std::int32_t);
    RADIXHEAP_TESTSET(std::int64_t);

#undef RADIXHEAP_TESTSET
}

void test_main_radix_heap_pair(std::mt19937& prng) {
    for (std::int64_t min : { 0, 1000000, -1000000 }) {
        for (size_t x : { 1000, 1000000 }) {
            allin_allout_all(prng, min, min + x, 500);
        }
    }

    for (PullMode pm : { PullMode::Single, PullMode::AllOfKey,
                         PullMode::ExtractBucket }) {
        for (std::int64_t prefill : { 0, 100 }) {
            for (std::int64_t min : { 0, 10000000, -10000000, -250 }) {
                for (size_t length : { 500 }) {
                    random_inout_all(prng, min, min + length, 200, 0.9, prefill, pm);
                }
            }
        }
    }
}

/******************************************************************************/

int main() {
    std::mt19937 prng(1);

    test_main_bitarray(prng);
    test_main_int_rank(prng);
    test_main_bucket(prng);
    test_main_radix_heap_pair(prng);

    return 0;
}

/******************************************************************************/
