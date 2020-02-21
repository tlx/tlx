################################################################################
# tests/CMakeLists.txt
#
# Part of tlx - http://panthema.net/tlx
#
# Copyright (C) 2015-2017 Timo Bingmann <tb@panthema.net>
#
# All rights reserved. Published under the Boost Software License, Version 1.0
################################################################################

# macro for building test target programs with correct libraries
macro(tlx_build_target TARGETNAME)

  add_executable(tlx_${TARGETNAME} ${ARGN})
  target_link_libraries(tlx_${TARGETNAME} tlx)

endmacro(tlx_build_target)

# macro for building test programs, without main() in gtest_main
macro(tlx_build_plain PROGNAME)

  string(REPLACE "/" "_" TESTNAME "${PROGNAME}") # replace slashes

  tlx_build_target(${TESTNAME} ${PROGNAME}.cpp ${ARGN})

endmacro(tlx_build_plain)

# macro for building test programs, adding gtest runner in gtest_main
macro(tlx_build_only PROGNAME)

  # append gtest runner program.
  tlx_build_plain(${PROGNAME} ${ARGN})

endmacro(tlx_build_only)

# macro for registering test programs: maybe prepend valgrind
macro(tlx_test_only TESTNAME)

  set(TARGETNAME ${TESTNAME} ${ARGN})
  string(REPLACE "/" "_" TARGETNAME "${TARGETNAME}") # replace slashes
  string(REPLACE ";" "_" TARGETNAME "${TARGETNAME}") # stringify list

  add_test(
    NAME tlx_${TARGETNAME}
    COMMAND tlx_${TESTNAME} ${ARGN})

endmacro(tlx_test_only)

# macro for building and running test programs
macro(tlx_build_test PROGNAME)

  tlx_build_only(${PROGNAME})

  string(REPLACE "/" "_" TESTNAME "${PROGNAME}") # replace slashes
  tlx_test_only(${TESTNAME})

endmacro(tlx_build_test)

### list of tests in subdirectories

tlx_build_only(algorithm/multiway_merge_benchmark)
tlx_build_only(cmdline_parser_example)
tlx_build_only(container/btree_speedtest)
tlx_build_only(container/d_ary_heap_speedtest)
tlx_build_only(sort_strings_example)

tlx_build_test(algorithm/multiway_merge_test)
tlx_build_test(algorithm/random_bipartition_shuffle)
tlx_build_test(algorithm_test)
tlx_build_test(backtrace_test)
tlx_build_test(cmdline_parser_test)
tlx_build_test(container/btree_test)
tlx_build_test(container/d_ary_heap_test)
tlx_build_test(container/loser_tree_test)
tlx_build_test(container/lru_cache_test)
tlx_build_test(container/radix_heap_test)
tlx_build_test(container/ring_buffer_test)
tlx_build_test(container/simple_vector_test)
tlx_build_test(container/splay_tree_test)
tlx_build_test(container/string_view_test)
tlx_build_test(counting_ptr_test)
tlx_build_test(delegate_test)
tlx_build_test(deprecated_test)
tlx_build_test(die_test)
tlx_build_test(digest_test)
tlx_build_test(logger_test)
tlx_build_test(math/aggregate_test)
tlx_build_test(math/polynomial_regression_test)
tlx_build_test(math_test)
tlx_build_test(meta/has_member_test)
tlx_build_test(meta/log2_test)
tlx_build_test(multi_timer_test)
tlx_build_test(semaphore_test)
tlx_build_test(siphash_test)
tlx_build_test(sort_networks_test)
tlx_build_test(sort_parallel_mergesort_test)
tlx_build_test(sort_strings_parallel_test)
tlx_build_test(sort_strings_test)
tlx_build_test(stack_allocator_test)
tlx_build_test(string_test)
tlx_build_test(thread_barrier_test)
tlx_build_test(thread_pool_test)
if(TLX_CXX_HAS_CXX14)
  tlx_build_test(meta/call_for_test)
  tlx_build_test(meta/fold_test)
  tlx_build_test(meta/function_chain_test)
  tlx_build_test(meta/function_stack_test)
  tlx_build_test(meta/vmap_for_test)
endif()

# disable -Wshadow on source with FunctionStack or FunctionChain
if(NOT MSVC)
  set_source_files_properties(
    meta/function_chain_test.cpp
    meta/function_stack_test.cpp
    PROPERTIES COMPILE_FLAGS "-Wno-shadow")

  set_source_files_properties(
    deprecated_test.cpp
    PROPERTIES COMPILE_FLAGS "-Wno-deprecated-declarations")
endif()
if(MSVC)
  # requires /bigobj flag to build
  set_target_properties(tlx_container_btree_test PROPERTIES COMPILE_FLAGS /bigobj)
endif()
if(CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9)
  # failed with a weird exception without -pthreads
  foreach(target
      tlx_algorithm_multiway_merge_test
      tlx_semaphore_test
      tlx_sort_parallel_mergesort_test
      tlx_sort_strings_parallel_test
      tlx_thread_barrier_test
      tlx_thread_pool_test
      )
    set_target_properties(${target} PROPERTIES COMPILE_FLAGS -pthread)
    set_target_properties(${target} PROPERTIES LINK_FLAGS -pthread)
  endforeach()
endif()

################################################################################
