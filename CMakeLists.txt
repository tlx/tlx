################################################################################
# CMakeLists.txt
#
# Part of tlx - http://panthema.net/tlx
#
# Copyright (C) 2015-2017 Timo Bingmann <tb@panthema.net>
#
# All rights reserved. Published under the Boost Software License, Version 1.0
################################################################################

cmake_minimum_required(VERSION 2.8.12)

if(NOT TARGET tlx)

# custom cmake scripts
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/misc/cmake)

# project
project(tlx)

# default to Debug building for single-config generators
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message("Defaulting CMAKE_BUILD_TYPE to Debug")
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type")
endif()

################################################################################
### Set Version -- synchronized with <tlx/version.hpp>

#[[perl
#  return "keep" if $ENV{USER} ne "tb";
#  use POSIX qw(strftime);
#  my $date = strftime("%Y%m%d", localtime);
#  open(F, "tlx/version.hpp") or die;
#  my @lines = grep(m/^#define TLX_MINOR_VERSION /, <F>);
#  close(F);
#  die unless @lines == 1;
#  $lines[0] =~ m/TLX_MINOR_VERSION\s+([0-9]+)/;
#  print("set(TLX_VERSION \"0.$1.$date\")\n");
#]]
set(TLX_VERSION "0.5.20210401")
#[[end]]
set(TLX_SOVERSION "0")

################################################################################
### Options and Switches

option(TLX_BUILD_TESTS
  "Build and run tlx's tests." OFF)

option(TLX_USE_GCOV
  "Compile and run tests with gcov for coverage analysis." OFF)

option(TLX_TRY_COMPILE_HEADERS
  "Test header files for self-sufficiency: try to compile them." OFF)

option(TLX_MORE_TESTS
  "Run more extensive test." OFF)

option(TLX_WARNINGS_ARE_ERRORS
  "Compile with flags such that warnings are error." OFF)

### building shared and/or static libraries

# by default we currently only build a static library
option(TLX_BUILD_STATIC_LIBS "Build static library version of libtlx" ON)
option(TLX_BUILD_SHARED_LIBS "Build shared library version of libtlx" OFF)

### allow user to change installation paths relative to CMAKE_INSTALL_PREFIX

include(GNUInstallDirs)

set(TLX_INSTALL_BIN_DIR ${CMAKE_INSTALL_BINDIR}
  CACHE PATH "Installation directory for executables")
set(TLX_INSTALL_LIB_DIR ${CMAKE_INSTALL_LIBDIR}
  CACHE PATH "Installation directory for libraries")
set(TLX_INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR}
  CACHE PATH "Installation directory for header files")
set(TLX_INSTALL_PKGCONFIG_DIR "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
  CACHE PATH "Installation directory for pkg-config file")

if(WIN32 AND NOT CYGWIN)
  set(TLX_DEFAULT_INSTALL_CMAKE_DIR "CMake")
else()
  set(TLX_DEFAULT_INSTALL_CMAKE_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/tlx")
endif()
set(TLX_INSTALL_CMAKE_DIR "${TLX_DEFAULT_INSTALL_CMAKE_DIR}"
  CACHE PATH "Installation directory for cmake files")

################################################################################

# variables to collect compile-time definitions, include dirs, and libraries
set(TLX_DEFINITIONS "")
set(TLX_DEPEND_INCLUDE_DIRS "")
set(TLX_DEPEND_LIBRARIES "")

if(TLX_MORE_TESTS)
  list(APPEND TLX_DEFINITIONS "TLX_MORE_TESTS")
endif()

if(TLX_BUILD_STRING_SORTING)
  list(APPEND TLX_DEFINITIONS "TLX_BUILD_STRING_SORTING")
endif()

if(NOT MSVC)
  ### Linux/Unix-like Build Environment ########################################

  # enable warnings
  set(CMAKE_CXX_FLAGS "-g -W -Wall -Wextra -fPIC ${CMAKE_CXX_FLAGS}")

  # test availability of -std=c++XX version
  include(CheckCXXCompilerFlag)

  check_cxx_compiler_flag("-std=c++20" TLX_CXX_HAS_CXX20)
  if(TLX_CXX_HAS_CXX20)
    set(CMAKE_CXX_FLAGS "-std=c++20 ${CMAKE_CXX_FLAGS}")
    set(TLX_CXX_HAS_CXX17 ON)
    set(TLX_CXX_HAS_CXX14 ON)
    set(TLX_CXX_HAS_CXX11 ON)
    set(TLX_CXX_HAS_CXX0X ON)
  else()
    check_cxx_compiler_flag("-std=c++17" TLX_CXX_HAS_CXX17)
    if(TLX_CXX_HAS_CXX17)
      set(CMAKE_CXX_FLAGS "-std=c++17 ${CMAKE_CXX_FLAGS}")
      set(TLX_CXX_HAS_CXX14 ON)
      set(TLX_CXX_HAS_CXX11 ON)
      set(TLX_CXX_HAS_CXX0X ON)
    else()
      check_cxx_compiler_flag("-std=c++14" TLX_CXX_HAS_CXX14)
      if(TLX_CXX_HAS_CXX14)
        set(CMAKE_CXX_FLAGS "-std=c++14 ${CMAKE_CXX_FLAGS}")
        set(TLX_CXX_HAS_CXX11 ON)
        set(TLX_CXX_HAS_CXX0X ON)
      else()
        check_cxx_compiler_flag("-std=c++11" TLX_CXX_HAS_CXX11)
        if(TLX_CXX_HAS_CXX11)
          set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
          set(TLX_CXX_HAS_CXX0X ON)
        else()
          check_cxx_compiler_flag("-std=c++0x" TLX_CXX_HAS_CXX0X)
          if(TLX_CXX_HAS_CXX0X)
            set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")
          endif()
        endif()
      endif()
    endif()
  endif()

  # enable -Wshadow and -Wold-style-cast only for gcc >= 5
  if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 5.0)
    set(CMAKE_CXX_FLAGS "-Wshadow -Wold-style-cast ${CMAKE_CXX_FLAGS}")
  endif()

  # remove -rdynamic from linker flags (smaller binaries which cannot be loaded
  # with dlopen() -- something no one needs)
  string(REGEX REPLACE "-rdynamic" ""
    CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_C_FLAGS}")
  string(REGEX REPLACE "-rdynamic" ""
    CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS}")

  # warn on conversions
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wconversion -Werror")

  # enable AddressSanitizer
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

  # enable ThreadSanitizer
  if(OFF)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -pie -fPIC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTLX_HAVE_THREAD_SANITIZER=1")
  endif()

  # enable UndefinedBehaviorSanitizer
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")

  # enable STL debug
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_DEBUG")

  if(TLX_WARNINGS_ARE_ERRORS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
  endif()

  # enable extra warnings on gcc
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wcast-qual")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Winit-self -Wnoexcept")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Woverloaded-virtual -Wredundant-decls")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wstrict-null-sentinel -Wstrict-overflow=5")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wtautological-compare")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fipa-pure-const -Wsuggest-attribute=const")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsign-conversion")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsign-promo")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsuggest-override")
  endif()
  # enable extra warnings on clang
  if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdeprecated")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wabstract-vbase-init")
  endif()

elseif(MSVC)
  ### Visual Studio Build Environment ##########################################

  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
  # raise warnings as errors
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")

  ### disable verbose warnings:
  # warning C4589: Constructor of abstract class '...' ignores initializer for
  # virtual base class '...' (false positive warnings)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4589")
  # warning C4127: conditional expression is constant
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4127")
  # warning C4458: declaration of '...' hides class member
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4458")
  # warning C4459: declaration of '...' hides global declaration
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4459")
  # warning C4702: unreachable code
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4702")
  # warning C4250: ABC inherits XYZ via dominance
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4250")
  # warning C4503: decorated name length exceeded, name was truncated
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4503")
  # disable lots of warnings about "unsecure" C runtime function
  list(APPEND TLX_DEFINITIONS "_CRT_SECURE_NO_WARNINGS")
  # disable "The POSIX name for this item is deprecated. Instead, use the ISO C
  # and C++ conformant name.", Nope. We will not.
  list(APPEND TLX_DEFINITIONS "_CRT_NONSTDC_NO_DEPRECATE")
  # disable lots of warnings about "unsecure" STL functions
  list(APPEND TLX_DEFINITIONS "_SCL_SECURE_NO_WARNINGS")
  # windef.h bizzarly defines min and max as macros, unless this is defined.
  list(APPEND TLX_DEFINITIONS "NOMINMAX")
endif()

if(TLX_USE_LTO)
  # build with link-time optimization
  include(CheckCXXCompilerFlag)
  check_cxx_compiler_flag(-flto CXX_HAS_LTO_FLAG)

  if(CMAKE_BUILD_TYPE MATCHES Release AND CXX_HAS_LTO_FLAG)
    find_program(CMAKE_GCC_AR
      NAMES ${_CMAKE_TOOLCHAIN_PREFIX}gcc-ar${_CMAKE_TOOLCHAIN_SUFFIX}
      HINTS ${_CMAKE_TOOLCHAIN_LOCATION})

    find_program(CMAKE_GCC_NM
      NAMES ${_CMAKE_TOOLCHAIN_PREFIX}gcc-nm
      HINTS ${_CMAKE_TOOLCHAIN_LOCATION})

    find_program(CMAKE_GCC_RANLIB
      NAMES ${_CMAKE_TOOLCHAIN_PREFIX}gcc-ranlib
      HINTS ${_CMAKE_TOOLCHAIN_LOCATION})

    if(CMAKE_GCC_AR AND CMAKE_GCC_NM AND CMAKE_GCC_RANLIB)
      set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto")
      set(CMAKE_AR "${CMAKE_GCC_AR}")
      set(CMAKE_NM "${CMAKE_GCC_NM}")
      set(CMAKE_RANLIB "${CMAKE_GCC_RANLIB}")
    else()
      message(WARNING "GCC indicates LTO support, but binutils wrappers could not be found. Disabling LTO.")
    endif()
  endif()
endif()

message(STATUS "TLX CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

###############################################################################
# enable gcov coverage analysis with gcc

if(TLX_USE_GCOV)
  # find programs
  find_program(GENHTML genhtml)
  find_program(LCOV lcov)

  if(NOT LCOV OR NOT GENHTML)
    message(SEND_ERROR "Coverage analysis requires lcov and genhtml programs.")
  endif()

  # add coverage anaylsis compile and link flags
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgcov")

  # add cached variable containing parameters for lcov/genhtml
  set(LCOV_FLAGS "" CACHE STRING "parameters for lcov")
  set(GENHTML_FLAGS --legend --no-branch-coverage
    CACHE STRING "parameters for genhtml")

  # custom target to run before tests
  add_custom_target(lcov-reset
    COMMAND ${LCOV} -q --directory ${CMAKE_BINARY_DIR} --zerocounters
    COMMENT "Resetting code coverage counters")

  # custom lcov target to run tests
  add_custom_target(lcov-runtests
    COMMAND ${CMAKE_CTEST_COMMAND} \${ARGS} || true
    DEPENDS lcov-reset
    COMMENT "Running all unit tests")

  # get git version description
  execute_process(COMMAND git describe --tags
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    OUTPUT_VARIABLE GITDESC
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  # command sequence to gather, clean and generate HTML coverage report
  add_custom_target(lcov-html
    COMMAND ${LCOV} -q --directory . --capture --output-file lcov.info
    COMMAND ${LCOV} -q --remove lcov.info '/usr/*' '*/extlib/*' ${LCOV_FLAGS} --output-file lcov-clean.info
    COMMAND ${GENHTML} -q -o coverage --title "tlx ${GITDESC}" --prefix ${PROJECT_SOURCE_DIR} ${GENHTML_FLAGS} lcov-clean.info
    DEPENDS lcov-runtests
    COMMENT "Capturing code coverage counters and create HTML coverage report"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

  # top-level target to run tests and generate coverage report
  add_custom_target(test-coverage
    COMMENT "Generate HTML coverage report "
    DEPENDS lcov-html)

endif(TLX_USE_GCOV)

###############################################################################
# enable ctest framework for running tests

enable_testing()

################################################################################
### Find Required Libraries

# find pthreads

find_package(Threads REQUIRED)
list(APPEND TLX_DEPEND_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})

################################################################################
### Descend into Subdirectories

# descend into library source
add_subdirectory(tlx)

if(TLX_BUILD_TESTS)
  # descend into testsuite
  add_subdirectory(tests)
endif()

set(TLX_LIBRARIES tlx)

# install header files
install(DIRECTORY tlx
  DESTINATION ${TLX_INSTALL_INCLUDE_DIR}
  COMPONENT Development
  FILES_MATCHING PATTERN "*.hpp")

###############################################################################
### cmake script TLX_TRY_COMPILE_HEADERS to compile all tlx header files

if(TLX_TRY_COMPILE_HEADERS)

  file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/try_compile")

  file(GLOB_RECURSE header_files
    RELATIVE "${PROJECT_SOURCE_DIR}" FOLLOW_SYMLINKS "tlx/*.hpp")

  if(NOT TLX_CXX_HAS_CXX14)
    list(REMOVE_ITEM header_files "tlx/meta.hpp")
    list(REMOVE_ITEM header_files "tlx/meta/apply_tuple.hpp")
    list(REMOVE_ITEM header_files "tlx/meta/fold_left.hpp")
    list(REMOVE_ITEM header_files "tlx/meta/fold_left_tuple.hpp")
    list(REMOVE_ITEM header_files "tlx/meta/fold_right.hpp")
    list(REMOVE_ITEM header_files "tlx/meta/fold_right_tuple.hpp")
    list(REMOVE_ITEM header_files "tlx/meta/function_chain.hpp")
    list(REMOVE_ITEM header_files "tlx/meta/function_stack.hpp")
    list(REMOVE_ITEM header_files "tlx/meta/vmap_foreach.hpp")
    list(REMOVE_ITEM header_files "tlx/meta/vmap_foreach_tuple.hpp")
    list(REMOVE_ITEM header_files "tlx/meta/vmap_foreach_with_index.hpp")
    list(REMOVE_ITEM header_files "tlx/meta/vmap_foreach_tuple_with_index.hpp")
    list(REMOVE_ITEM header_files "tlx/meta/vmap_for_range.hpp")
  endif()

  foreach(header_file ${header_files})
    # replace / and . with _ to get a valid target and file name
    string(REPLACE "/" "_" compile_name "${header_file}")
    string(REPLACE "." "_" compile_name "${compile_name}")

    set(target_name "tlx_try_compile_${compile_name}")
    set(source_name "${PROJECT_BINARY_DIR}/try_compile/${target_name}.cpp")

    file(WRITE ${source_name}
      "#include <${header_file}>
       int main() { return 0; }")

    add_executable(${target_name} ${source_name})
    target_link_libraries(${target_name} ${TLX_LIBRARIES})
    set_property(TARGET ${target_name}
      PROPERTY RUNTIME_OUTPUT_DIRECTORY "try_compile/")

  endforeach()

  # add special definitions for try_compile targets
  if(CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9)
    set_target_properties(
      tlx_try_compile_tlx_thread_barrier_spin_hpp
      PROPERTIES COMPILE_DEFINITIONS "_GLIBCXX_USE_SCHED_YIELD")
  endif()

endif(TLX_TRY_COMPILE_HEADERS)

################################################################################
# export macros for building with tlx to global CACHE
# NOTE: this is for old-school cmake, better just add tlx to link_libaries

set(TLX_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}")
set(TLX_LIBRARIES "tlx")

set(TLX_INCLUDE_DIRS "${TLX_INCLUDE_DIRS}" CACHE STRING "Include paths for TLX")
set(TLX_LIBRARIES "${TLX_LIBRARIES}" CACHE STRING "Libraries to link for TLX")

###############################################################################
# export targets to cmake project config file

# register package for use from the global CMake-registry
export(PACKAGE tlx)

# add tlx library targets to the build tree export set
export(TARGETS ${TLX_EXPORTED_LIBS}
  FILE "${PROJECT_BINARY_DIR}/tlx-targets.cmake")

include(CMakePackageConfigHelpers)

# create common tlx-version.cmake file
configure_file(misc/cmake/tlx-version.cmake.in
  "${PROJECT_BINARY_DIR}/tlx-version.cmake" @ONLY)

# create tlx-version.cmake file for the install tree
configure_package_config_file(
  misc/cmake/tlx-config.cmake.in tlx-config.cmake
  INSTALL_DESTINATION "${TLX_INSTALL_CMAKE_DIR}"
  PATH_VARS TLX_INSTALL_INCLUDE_DIR)

# install the tlx-config.cmake and tlx-version.cmake
install(FILES
  "${PROJECT_BINARY_DIR}/tlx-version.cmake"
  "${PROJECT_BINARY_DIR}/tlx-config.cmake"
  DESTINATION "${TLX_INSTALL_CMAKE_DIR}")

# Install the export set for use with the install-tree
install(EXPORT tlx-targets DESTINATION "${TLX_INSTALL_CMAKE_DIR}")

###############################################################################
# prepare pkg-config file

configure_file(misc/cmake/tlx.pc
  "${PROJECT_BINARY_DIR}/${TLX_LIBNAME}.pc" @ONLY)

# copy the tlx.pc file into lib/pkgconfig
if(TLX_INSTALL_PKGCONFIG_DIR)
  install(FILES ${PROJECT_BINARY_DIR}/${TLX_LIBNAME}.pc
    DESTINATION ${TLX_INSTALL_PKGCONFIG_DIR})
endif()

################################################################################

endif(NOT TARGET tlx)

################################################################################
