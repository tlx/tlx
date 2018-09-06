################################################################################
# tlx/CMakeLists.txt
#
# Part of tlx - http://panthema.net/tlx
#
#
# All rights reserved. Published under the Boost Software License, Version 1.0
################################################################################

set(LIBTLX_SOURCES

  algorithm/parallel_multiway_merge.cpp
  backtrace.cpp
  cmdline_parser.cpp
  die/core.cpp
  digest/md5.cpp
  digest/sha1.cpp
  digest/sha256.cpp
  digest/sha512.cpp
  logger/core.cpp
  math/integer_log2.cpp
  math/is_power_of_two.cpp
  math/round_to_power_of_two.cpp
  port/setenv.cpp
  sort/strings1.cpp
  sort/strings2.cpp
  sort/strings3.cpp
  string/base64.cpp
  string/compare_icase.cpp
  string/contains.cpp
  string/contains_word.cpp
  string/ends_with.cpp
  string/equal_icase.cpp
  string/erase_all.cpp
  string/escape_html.cpp
  string/escape_uri.cpp
  string/expand_environment_variables.cpp
  string/extract_between.cpp
  string/format_si_iec_units.cpp
  string/hexdump.cpp
  string/index_of.cpp
  string/join.cpp
  string/join_quoted.cpp
  string/less_icase.cpp
  string/pad.cpp
  string/parse_si_iec_units.cpp
  string/replace.cpp
  string/split.cpp
  string/split_quoted.cpp
  string/split_words.cpp
  string/starts_with.cpp
  string/to_lower.cpp
  string/to_upper.cpp
  string/trim.cpp
  string/union_words.cpp
  string/word_wrap.cpp
  thread_pool.cpp

  )

# tell top-level cmakelists which library we build: assume only static
set(TLX_EXPORTED_LIBS tlx)

# we name debug library builds "tlx_debug" and release builds "tlx"
string(TOLOWER "tlx_${CMAKE_BUILD_TYPE}" TLX_LIBNAME)
if(TLX_LIBNAME STREQUAL "tlx_release" OR TLX_LIBNAME STREQUAL "tlx_")
  set(TLX_LIBNAME "tlx")
endif()

if(TLX_BUILD_SHARED_LIBS)

  # build tlx SHARED library and link all programs again it
  add_library(tlx SHARED ${LIBTLX_SOURCES})
  set_target_properties(tlx PROPERTIES OUTPUT_NAME "${TLX_LIBNAME}")
  set_target_properties(tlx PROPERTIES VERSION "${TLX_VERSION}")

  target_compile_definitions(tlx PUBLIC ${TLX_DEFINITIONS})
  target_include_directories(tlx PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/>
    $<INSTALL_INTERFACE:${TLX_INSTALL_INCLUDE_DIR}>)
  target_link_libraries(tlx ${TLX_LINK_LIBRARIES})

  install(TARGETS tlx
    EXPORT tlx-targets
    ARCHIVE DESTINATION ${TLX_INSTALL_LIB_DIR}
    LIBRARY DESTINATION ${TLX_INSTALL_LIB_DIR})

  if(TLX_BUILD_STATIC_LIBS)
    # but also build STATIC library
    add_library(tlx_static STATIC ${LIBTLX_SOURCES})
    set_target_properties(tlx_static PROPERTIES OUTPUT_NAME "${TLX_LIBNAME}")
    set_target_properties(tlx_static PROPERTIES VERSION "${TLX_VERSION}")

    target_compile_definitions(tlx_static PUBLIC ${TLX_DEFINITIONS})
    target_include_directories(tlx_static PUBLIC
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/>
      $<INSTALL_INTERFACE:${TLX_INSTALL_INCLUDE_DIR}>)
    target_link_libraries(tlx_static ${TLX_LINK_LIBRARIES})

    install(TARGETS tlx_static
      EXPORT tlx-targets
      ARCHIVE DESTINATION ${TLX_INSTALL_LIB_DIR}
      LIBRARY DESTINATION ${TLX_INSTALL_LIB_DIR})

    # tell top-level cmakelists which library we build: both shared and static
    set(TLX_EXPORTED_LIBS tlx tlx_static)
  endif()

else()

  # build tlx STATIC library and link all programs again it
  add_library(tlx STATIC ${LIBTLX_SOURCES})
  set_target_properties(tlx PROPERTIES OUTPUT_NAME "${TLX_LIBNAME}")
  set_target_properties(tlx PROPERTIES VERSION "${TLX_VERSION}")

  target_compile_definitions(tlx PUBLIC ${TLX_DEFINITIONS})
  target_include_directories(tlx PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/>
    $<INSTALL_INTERFACE:${TLX_INSTALL_INCLUDE_DIR}>)
  target_link_libraries(tlx ${TLX_LINK_LIBRARIES})

  install(TARGETS tlx
    EXPORT tlx-targets
    ARCHIVE DESTINATION ${TLX_INSTALL_LIB_DIR}
    LIBRARY DESTINATION ${TLX_INSTALL_LIB_DIR})

endif()

# export variables to top-level cmakelists
set(TLX_LIBNAME ${TLX_LIBNAME} PARENT_SCOPE)
set(TLX_EXPORTED_LIBS ${TLX_EXPORTED_LIBS} PARENT_SCOPE)

# disable -Wshadow on source with FunctionStack or FunctionChain
if(NOT MSVC)
  set_source_files_properties(
    function_chain.hpp function_stack.hpp
    PROPERTIES COMPILE_FLAGS "-Wno-shadow")
endif()

################################################################################
