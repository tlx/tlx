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

- `CountingPtr` – an intrusive reference counting pointer
- `die()` - Simple Invariant Testing : `die()`, `die_unless()`, `die_if()`, `die_unequal()`.
- logger.hpp : `LOG`, `LOG1`, `LOGC`, `sLOG`, `wrap_unprintable()`.
- Miscellaneous: `timestamp()`, `unused()`, `vector_free()`.
- Algorithms : `merge_combine()`, `exclusive_scan()`, `multiway_merge()`, `parallel_multiway_merge()`, `multisequence_selection()`, `multisequence_partition()`.
- Data Structures : `RingBuffer`, `SimpleVector`, `StringView`, B+ Trees, Loser Trees, `RadixHeap`, `(Addressable) D-Ary Heap`
- Defines and Macros : `TLX_LIKELY`, `TLX_UNLIKELY`, `TLX_ATTRIBUTE_PACKED`, `TLX_ATTRIBUTE_ALWAYS_INLINE`, `TLX_ATTRIBUTE_FORMAT_PRINTF`, `TLX_DEPRECATED_FUNC_DEF`.
- Message Digests : `MD5`, `md5_hex()`, `SHA1`, `sha1_hex()`, `SHA256`, `sha256_hex()`, `SHA512`, `sha512_hex()`.
- Math Functions : `integer_log2_floor()`, `is_power_of_two()`, `round_up_to_power_of_two()`, `round_down_to_power_of_two()`, `ffs()`, `clz()`, `ctz()`, `abs_diff()`, `bswap32()`, `bswap64()`, `popcount()`, `power_to_the`, `Aggregate`, `PolynomialRegression`.
- String Algorithms : `starts_with()`, `ends_with()`, `contains()`, `contains_word()`, `trim()`, `replace_all()`, `erase_all()`, `join()`, `split()`, `split_view()`, `split_words()`, `union_words()`, `split_quoted()`, `join_quoted()`, `to_lower()`, `hexdump()`, `bitdump_le8()`, `word_wrap()`, `escape_html()`, `parse_uri()`, `parse_uri_form_data()`, `parse_si_iec_units()`, `format_iec_units()`, `ssprintf()`, `expand_environment_variables()`, `levenshtein()`, `hash_djb2()`, `hash_sdbm()`.
- Meta-Template Programming : `call_foreach()`, `apply_tuple()`, `vmap_foreach()`, `Log2Floor`, `FunctionChain`, `FunctionStack`, `is_std_pair`, `is_std_tuple`, `is_std_vector`, `is_std_array`, `TLX_MAKE_HAS_MEMBER`.
- Sorting Algorithms : `parallel_mergesort()` (experimental parallel merge sort from MCSTL), `sort_strings()` (using radix sort and multikey quicksort), `sort_strings_parallel()` (using parallel super scalar string sample sort), sorting network for up to sixteen elements.
- Backtrace Printing : `print_cxx_backtrace()`.
- Command Line Parsing : `CmdlineParser`.
- Multi-Phase Timer: `MultiTimer`, `ScopedMultiTimerSwitch`, `ScopedMultiTimer`.
- Fast Delegates : `Delegate` - a better `std::function<>` replacement.
- SipHash : simple string hashing `siphash()`
- StackAllocator : stack-local allocations
- Threading : `ThreadPool`, `Semaphore`, `ThreadBarrierMutex`, `ThreadBarrierSpin`.

## Bugs

Please report bugs via the [github issue tracking system](https://github.com/tlx/tlx/issues).

## License

tlx is licensed under the [Boost Software License - Version 1.0](https://github.com/tlx/tlx/blob/master/LICENSE).

If you use tlx in an academic context or publication, please cite it as

```
@Misc{TLX,
  title = 	 {{TLX}: Collection of Sophisticated {C++} Data Structures, Algorithms, and Miscellaneous Helpers},
  author = 	 {Timo Bingmann},
  year = 	 2018,
  note = 	 {\url{https://panthema.net/tlx}, retrieved {Oct.} 7, 2020},
}
```
