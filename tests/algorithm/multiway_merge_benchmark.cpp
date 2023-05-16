/*******************************************************************************
 * tests/algorithm/multiway_merge_benchmark.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2014 Thomas Keh <thomas.keh@student.kit.edu>
 * Copyright (C) 2014-2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include <tlx/algorithm/multiway_merge.hpp>
#include <tlx/algorithm/parallel_multiway_merge.hpp>
#include <tlx/cmdline_parser.hpp>
#include <tlx/die.hpp>
#include <tlx/logger.hpp>
#include <tlx/string/format_si_iec_units.hpp>
#include <tlx/timestamp.hpp>

#if defined(_OPENMP)
#include <parallel/algorithm>
#endif

// number of repetitions of each benchmark
unsigned int g_outer_repeat = 3;

// number of inner repetitions of each benchmark
unsigned int g_inner_repeat = 1;

// factor to multiply while increasing number of arrays
unsigned int g_factor = 32;

// run only a few quick benchmark runs
bool g_quick = false;

struct DataStruct {
    unsigned int key;
    char payload[32];

    explicit DataStruct(unsigned int k = 0) noexcept
        : key(k)
    { }

    bool operator < (const DataStruct& other) const {
        return key < other.key;
    }

    bool operator == (const DataStruct& other) const {
        return (key == other.key);
    }

    friend std::ostream& operator << (std::ostream& os, const DataStruct& s) {
        return os << '(' << s.key << ",...)";
    }
};

enum benchmark_type {
    SEQ_MWM_LT,
    SEQ_MWM_LT_STABLE,
    SEQ_MWM_LT_COMBINED,
    SEQ_MWM_BB,
    SEQ_GNU_MWM,

    PARA_MWM_EXACT_LT,
    PARA_MWM_EXACT_LT_STABLE,
    PARA_MWM_SAMPLING_LT,
    PARA_MWM_SAMPLING_LT_STABLE,
    PARA_GNU_MWM_EXACT,
    PARA_GNU_MWM_SAMPLING
};

/*!
 * Simple scoped timer, which takes a text message and prints the duration
 * until the scope is destroyed.
 */
class scoped_print_timer
{
protected:
    //! message
    std::string message_;

    //! bytes processed
    std::uint64_t bytes_;

    //! timer
    double ts_;

public:
    //! save message and start timer
    explicit scoped_print_timer(const std::string& message, const std::uint64_t bytes = 0)
        : message_(message),
          bytes_(bytes),
          ts_(tlx::timestamp()) {
        LOG1 << "Starting " << message;
    }

    //! on destruction: tell the time
    ~scoped_print_timer() {
        double elapsed = tlx::timestamp() - ts_;

        if (bytes_ == 0) {
            LOG1 << "Finished "
                 << message_
                 << " after " << elapsed << " seconds";
        }
        else {
            double bps = static_cast<double>(bytes_) / elapsed;

            LOG1 << "Finished "
                 << message_
                 << " after " << elapsed << " seconds. "
                 << "Processed " << tlx::format_iec_units(bytes_) << "B"
                 << " @ " << tlx::format_iec_units(static_cast<std::uint64_t>(bps)) << "B/s";
        }
    }
};

template <typename ValueType, benchmark_type Method>
void test_multiway_merge(size_t seq_count, const size_t seq_size) {
    // we allocate a list of blocks, each block being a sequence of items.
    static const size_t item_size = sizeof(ValueType);
    const size_t seq_items = seq_size / item_size;

    using sequence_type = std::vector<ValueType>;

    size_t total_size = seq_count * seq_items;
    size_t total_bytes = seq_count * seq_size;

    std::less<ValueType> cmp;

    std::vector<sequence_type> seqs(seq_count);

    {
        scoped_print_timer spt(
            "Filling sequences with random numbers in parallel", total_bytes);

        for (size_t i = 0; i < seq_count; ++i)
            seqs[i].resize(seq_items);

#if defined(_OPENMP)
#pragma omp parallel
#endif
        {
            std::uniform_int_distribution<std::uint64_t> distr;

#if defined(_OPENMP)
            unsigned int seed = 1234 * omp_get_thread_num() + seq_count + seq_size;
            std::mt19937_64 randgen(seed);
#pragma omp for
#else
            std::mt19937_64 randgen(1234 + seq_count + seq_size);
#endif
            for (long i = 0; i < static_cast<long>(seq_count); ++i)
            {
                for (size_t j = 0; j < seq_items; ++j)
                    seqs[i][j] = ValueType(distr(randgen));

                std::sort(seqs[i].begin(), seqs[i].end(), cmp);
            }
        }
    }

    std::vector<ValueType> out;

    {
        scoped_print_timer spt("Allocating output buffer", total_bytes);
        out.resize(total_size);
    }

    {
        scoped_print_timer spt("Merging", total_bytes);
        double ts1 = tlx::timestamp();

        const char* method_name = nullptr;
        using sequence_iterator_pair_type = std::pair<
            typename sequence_type::iterator,
            typename sequence_type::iterator>;

        std::vector<sequence_iterator_pair_type> iterpairs(seq_count);

        for (unsigned int r = 0; r < g_inner_repeat; ++r)
        {
            // (re-)set sequence iterators
            for (size_t i = 0; i < seq_count; ++i) {
                iterpairs[i] =
                    sequence_iterator_pair_type(seqs[i].begin(), seqs[i].end());
            }

            switch (Method)
            {
            case SEQ_MWM_LT:
                method_name = "seq_mwm_lt";

                tlx::multiway_merge(
                    iterpairs.begin(), iterpairs.end(),
                    out.begin(), total_size, cmp,
                    tlx::MWMA_LOSER_TREE);
                break;

            case SEQ_MWM_LT_COMBINED:
                method_name = "seq_mwm_lt_combined";

                tlx::multiway_merge(
                    iterpairs.begin(), iterpairs.end(),
                    out.begin(), total_size, cmp,
                    tlx::MWMA_LOSER_TREE_COMBINED);
                break;

            case SEQ_MWM_LT_STABLE:
                method_name = "seq_mwm_lt_stable";

                tlx::stable_multiway_merge(
                    iterpairs.begin(), iterpairs.end(),
                    out.begin(), total_size, cmp,
                    tlx::MWMA_LOSER_TREE);
                break;

            case SEQ_MWM_BB:
                method_name = "seq_mwm_bb";

                tlx::multiway_merge(
                    iterpairs.begin(), iterpairs.end(),
                    out.begin(), total_size, cmp,
                    tlx::MWMA_BUBBLE);
                break;

#if defined(_OPENMP)
            case SEQ_GNU_MWM:
                method_name = "seq_gnu_mwm";

                __gnu_parallel::multiway_merge(
                    iterpairs.begin(), iterpairs.end(),
                    out.begin(), total_size, cmp,
                    __gnu_parallel::sequential_tag());
                break;
#endif
            case PARA_MWM_EXACT_LT:
                method_name = "para_mwm_exact_lt";

                tlx::parallel_multiway_merge(
                    iterpairs.begin(), iterpairs.end(),
                    out.begin(), total_size, cmp,
                    tlx::MWMA_LOSER_TREE, tlx::MWMSA_EXACT);
                break;

            case PARA_MWM_EXACT_LT_STABLE:
                method_name = "para_mwm_exact_lt_stable";

                tlx::stable_parallel_multiway_merge(
                    iterpairs.begin(), iterpairs.end(),
                    out.begin(), total_size, cmp,
                    tlx::MWMA_LOSER_TREE, tlx::MWMSA_EXACT);
                break;

            case PARA_MWM_SAMPLING_LT:
                method_name = "para_mwm_sampling_lt";

                tlx::parallel_multiway_merge(
                    iterpairs.begin(), iterpairs.end(),
                    out.begin(), total_size, cmp,
                    tlx::MWMA_LOSER_TREE, tlx::MWMSA_SAMPLING);
                break;

            case PARA_MWM_SAMPLING_LT_STABLE:
                method_name = "para_mwm_sampling_lt_stable";

                tlx::stable_parallel_multiway_merge(
                    iterpairs.begin(), iterpairs.end(),
                    out.begin(), total_size, cmp,
                    tlx::MWMA_LOSER_TREE, tlx::MWMSA_SAMPLING);
                break;

#if defined(_OPENMP)
            case PARA_GNU_MWM_EXACT: {
                method_name = "para_gnu_mwm_exact";

                __gnu_parallel::_Settings s = __gnu_parallel::_Settings::get();
                s.multiway_merge_splitting = __gnu_parallel::EXACT;
                __gnu_parallel::_Settings::set(s);

                __gnu_parallel::multiway_merge(
                    iterpairs.begin(), iterpairs.end(),
                    out.begin(), total_size, cmp);
                break;
            }

            case PARA_GNU_MWM_SAMPLING: {
                method_name = "para_gnu_mwm_sampling";

                __gnu_parallel::_Settings s = __gnu_parallel::_Settings::get();
                s.multiway_merge_splitting = __gnu_parallel::SAMPLING;
                __gnu_parallel::_Settings::set(s);

                __gnu_parallel::multiway_merge(
                    iterpairs.begin(), iterpairs.end(),
                    out.begin(), total_size, cmp);
                break;
            }
#endif
            default:
                LOG1 << "Error: method " << Method << " is not available "
                    "in this compilation.";
                break;
            }
        }

        double ts2 = tlx::timestamp();

        std::cout
            << "RESULT"
            << " seq_count=" << seq_count
            << " method=" << method_name
            << " item_size=" << item_size
            << " seq_items=" << seq_items
            << " total_size=" << total_size
            << " total_bytes=" << total_bytes
#if defined(_OPENMP)
            << " num_threads=" << omp_get_max_threads()
#else
            << " num_threads=" << std::thread::hardware_concurrency()
#endif
            << " time=" << (ts2 - ts1)
            << " inner_repeats=" << g_inner_repeat
            << " outer_repeats=" << g_outer_repeat
            << " time/item[ns]="
            << (ts2 - ts1) / static_cast<double>(g_inner_repeat) / total_size * 1e9
            << std::endl;
    }

    die_unless(std::is_sorted(out.cbegin(), out.cend(), cmp));
}

template <typename ValueType, benchmark_type Method>
void test_repeat(size_t seq_count, const size_t seq_size) {
    for (unsigned int r = 0; r < g_outer_repeat; ++r)
        test_multiway_merge<ValueType, Method>(seq_count, seq_size);
}

template <typename ValueType, benchmark_type Method>
void test_seqnum(const size_t seq_size = 2* 1024* 1024) {
    for (unsigned int s = 1; s < 64; s += 1 * g_factor)
        test_repeat<ValueType, Method>(s, seq_size);

    for (unsigned int s = 64; s < 128; s += 2 * g_factor)
        test_repeat<ValueType, Method>(s, seq_size);

    for (unsigned int s = 128; s < 256; s += 4 * g_factor)
        test_repeat<ValueType, Method>(s, seq_size);

    if (g_quick) return;

    for (unsigned int s = 256; s < 512; s += 16 * g_factor)
        test_repeat<ValueType, Method>(s, seq_size);

    for (unsigned int s = 512; s < 1024; s += 64 * g_factor)
        test_repeat<ValueType, Method>(s, seq_size);

    for (unsigned int s = 1024; s < 4 * 1024; s += 128 * g_factor)
        test_repeat<ValueType, Method>(s, seq_size);
}

template <typename ValueType>
void test_seqnum_sequential() {
    test_seqnum<ValueType, SEQ_MWM_LT>();
    test_seqnum<ValueType, SEQ_MWM_LT_STABLE>();
    test_seqnum<ValueType, SEQ_MWM_LT_COMBINED>();
    test_seqnum<ValueType, SEQ_MWM_BB>();
    test_seqnum<ValueType, SEQ_GNU_MWM>();
}

template <typename ValueType>
void test_seqnum_parallel() {
    test_seqnum<ValueType, PARA_MWM_EXACT_LT>();
    test_seqnum<ValueType, PARA_MWM_EXACT_LT_STABLE>();
    test_seqnum<ValueType, PARA_MWM_SAMPLING_LT>();
    test_seqnum<ValueType, PARA_MWM_SAMPLING_LT_STABLE>();
    test_seqnum<ValueType, PARA_GNU_MWM_EXACT>();
    test_seqnum<ValueType, PARA_GNU_MWM_SAMPLING>();
}

template <typename ValueType, benchmark_type Method>
void test_seqsize(const size_t seq_num = 64) {
    for (size_t b = 1024; b < 1024 * 1024 / seq_num; b *= 2)
        test_repeat<ValueType, Method>(seq_num, b);

    for (size_t b = 1024 * 1024 / seq_num;
         b < 1024 * 1024 * static_cast<size_t>(1024) / seq_num; b *= 2)
        test_repeat<ValueType, Method>(seq_num, b);
}

int main(int argc, char* argv[]) {
    std::string benchset;

    tlx::CmdlineParser cp;
    cp.set_description("TLX (parallel) multiway_merge benchmark");

    cp.add_param_string("seq/para/both/seqsize/all", benchset,
                        "benchmark set: seq(uential), para(llel) or both; "
                        "seqsize; or all");

    cp.add_uint('r', "inner-repeat", g_inner_repeat,
                "number of inner repetitions within each benchmark");

    cp.add_uint('R', "outer-repeat", g_outer_repeat,
                "number of repetitions of each benchmark");

    cp.add_uint('f', "factor", g_factor,
                "factor to multiply while increasing number of arrays");

    cp.add_flag('q', "quick", g_quick,
                "run only a few quick benchmark runs");

    if (!cp.process(argc, argv))
        return EXIT_FAILURE;

    // run individually for debugging
    if (0)
    {
        test_repeat<std::uint64_t, PARA_GNU_MWM_EXACT>(2, 2 * 1024 * 1024);
        test_repeat<std::uint64_t, PARA_MWM_EXACT_LT>(2, 2 * 1024 * 1024);
        // test_repeat<std::uint64_t, PARA_MWM_EXACT_LT>(1024, 2 * 1024 * 1024);
        // test_repeat<std::uint64_t, SEQ_MWM_LT>(2, 2 * 1024 * 1024);
        // test_repeat<std::uint64_t, SEQ_GNU_MWM>(2, 2 * 1024 * 1024);
        return 0;
    }

    if (benchset == "seq" || benchset == "sequential" ||
        benchset == "both" || benchset == "all")
    {
        test_seqnum_sequential<std::uint64_t>();
        test_seqnum_sequential<DataStruct>();
    }
    if (benchset == "para" || benchset == "parallel" ||
        benchset == "both" || benchset == "all")
    {
        test_seqnum_parallel<std::uint64_t>();
        test_seqnum_parallel<DataStruct>();
    }
    if (benchset == "seqsize" || benchset == "all")
    {
        // fastest sequential multiway merge
        test_seqsize<std::uint64_t, SEQ_MWM_LT_COMBINED>();
        test_seqsize<DataStruct, SEQ_MWM_LT_COMBINED>();
        // and parallel multiway merge
        test_seqsize<std::uint64_t, PARA_MWM_EXACT_LT>();
        test_seqsize<DataStruct, PARA_MWM_EXACT_LT>();
    }

    return 0;
}

/******************************************************************************/
