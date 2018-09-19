# tlx - Collection of C++ Data Structures, Algorithms, and Miscellaneous Helpers

tlx is a collection of C++ helpers and extensions universally needed, but not found in the STL.The most important design goals and conventions are:

- high modularity with as little dependencies between modules as possible.
- attempt to never break existing interfaces.
- compile on all platforms with C++ – smartphones, supercomputers, windows, etc.
- zero external dependencies: no additional libraries are required.
- warning and bug-freeness on all compilers.
- keep overhead down – small overall size such that is can be included without bloating applications.

--->> **[Doxygen Documentation](https://tlx.github.io/)** <<---

[![Build Status](https://travis-ci.org/tlx/tlx.svg?branch=master)](https://travis-ci.org/tlx/tlx)
[![Build status](https://ci.appveyor.com/api/projects/status/xxwj7usfjfs3h9id/branch/master?svg=true)](https://ci.appveyor.com/project/bingmann/tlx/branch/master)
[![Coverage Status](https://coveralls.io/repos/github/tlx/tlx/badge.svg)](https://coveralls.io/github/tlx/tlx)

## List of the most commonly used components:

- CountingPtr – an intrusive reference counting pointer
- die() - Simple Invariant Testing : die(), die_unless(), die_if(), die_unequal().
- logger.hpp : LOG, LOG1, LOGC, sLOG.
- unused.hpp : unused.
- Algorithms : merge_combine(), exclusive_scan(), multiway_merge(), parallel_multiway_merge(), multisequence_selection(), multisequence_partition().
- Data Structures : RingBuffer, SimpleVector, B+ Trees, Loser Trees, RadixHeap
- Defines and Macros : TLX_LIKELY, TLX_UNLIKELY, TLX_ATTRIBUTE_PACKED, TLX_ATTRIBUTE_ALWAYS_INLINE, TLX_ATTRIBUTE_FORMAT_PRINTF, TLX_DEPRECATED_FUNC_DEF.
- Message Digests : MD5, md5_hex(), SHA1, sha1_hex(), SHA256, sha256_hex(), SHA512, sha512_hex().
- Math Functions : integer_log2_floor(), is_power_of_two(), round_up_to_power_of_two(), round_down_to_power_of_two(), ffs(), clz(), abs_diff(), bswap32, bswap64.
- String Algorithms : starts_with(), ends_with(), contains(), contains_word(), trim(), replace_all(), erase_all(), join(), split(), split_words(), union_words(), split_quoted(), join_quoted(), to_lower(), hexdump(), word_wrap(), escape_html(), parse_si_iec_units(), format_iec_units(), expand_environment_variables().
- Meta-Template Programming : call_foreach(), apply_tuple(), vmap_foreach(), Log2Floor, FunctionChain, FunctionStack, is_std_pair, is_std_tuple, is_std_vector, is_std_array, TLX_MAKE_HAS_MEMBER.
- Sorting Algorithms : sort_strings() (using radix sort and multikey quicksort), parallel_mergesort() (experimental mergesort from MCSTL).
- Backtrace Printing : print_cxx_backtrace().
- Command Line Parsing : CmdlineParser.
- Fast Delegates : Delegate - a better std::function<> replacement.
- SipHash : simple string hashing
- StackAllocator : stack-local allocations
- Threading : ThreadPool, Semaphore.
