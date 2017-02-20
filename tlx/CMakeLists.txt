################################################################################
# tlx/CMakeLists.txt
#
# Part of tlx - http://panthema.net/tlx
#
#
# All rights reserved. Published under the Boost Software License, Version 1.0
################################################################################

set(LIBTLX_SOURCES

  die.cpp
  string/base64.cpp
  string/ends_with.cpp
  string/erase_all.cpp
  string/hexdump.cpp
  string/replace.cpp
  string/starts_with.cpp
  string/to_lower.cpp
  string/to_upper.cpp
  string/trim.cpp
  string/word_wrap.cpp

  )

# tell top-level cmakelists which library we build
set(TLX_EXPORTED_LIBS tlx)

# we name debug library builds "tlx_debug" and release builds "tlx"
string(TOLOWER "tlx_${CMAKE_BUILD_TYPE}" TLX_LIBNAME)
if(TLX_LIBNAME STREQUAL "tlx_release" OR TLX_LIBNAME STREQUAL "tlx_")
  set(TLX_LIBNAME "tlx")
endif()

if(BUILD_SHARED_LIBS)

  # build tlx SHARED library and link all programs again it
  add_library(tlx SHARED ${LIBTLX_SOURCES})
  set_target_properties(tlx PROPERTIES OUTPUT_NAME "${TLX_LIBNAME}")
  set_target_properties(tlx PROPERTIES VERSION "${TLX_VERSION_STRING}")

  target_compile_definitions(tlx PUBLIC ${TLX_DEFINITIONS})
  target_include_directories(tlx PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/>
    $<INSTALL_INTERFACE:/>)
  #target_include_directories(tlx SYSTEM PUBLIC ${TLX_INCLUDE_DIRS})
  target_link_libraries(tlx ${TLX_EXTRA_LIBRARIES})

  install(TARGETS tlx
    EXPORT tlx-targets
    ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
    LIBRARY DESTINATION ${INSTALL_LIB_DIR})

  if(BUILD_STATIC_LIBS)
    # but also build STATIC library
    add_library(tlx_static STATIC ${LIBTLX_SOURCES})
    set_target_properties(tlx_static PROPERTIES OUTPUT_NAME "${TLX_LIBNAME}")
    set_target_properties(tlx_static PROPERTIES VERSION "${TLX_VERSION_STRING}")

    target_compile_definitions(tlx_static PUBLIC ${TLX_DEFINITIONS})
    target_include_directories(tlx_static PUBLIC
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/>
      $<INSTALL_INTERFACE:/>)
    #target_include_directories(tlx_static SYSTEM PUBLIC ${TLX_INCLUDE_DIRS})
    target_link_libraries(tlx_static ${TLX_EXTRA_LIBRARIES})

    install(TARGETS tlx_static
      EXPORT tlx-targets
      ARCHIVE DESTINATION ${INSTALL_LIB_DIR})

    # we build both shared and static, export both
    set(TLX_EXPORTED_LIBS tlx tlx_static)
  endif()

else()
  # build tlx STATIC library and link all programs again it
  add_library(tlx STATIC ${LIBTLX_SOURCES})
  set_target_properties(tlx PROPERTIES OUTPUT_NAME "${TLX_LIBNAME}")
  set_target_properties(tlx PROPERTIES VERSION "${TLX_VERSION_STRING}")

  target_compile_definitions(tlx PUBLIC ${TLX_DEFINITIONS})
  target_include_directories(tlx PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/>
    $<INSTALL_INTERFACE:/>)
  #target_include_directories(tlx SYSTEM PUBLIC ${TLX_INCLUDE_DIRS})
  target_link_libraries(tlx ${TLX_EXTRA_LIBRARIES})

  install(TARGETS tlx
    EXPORT tlx-targets
    ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
    LIBRARY DESTINATION ${INSTALL_LIB_DIR})

endif()

# export variables to top-level cmakelists
set(TLX_LIBNAME ${TLX_LIBNAME} PARENT_SCOPE)
set(TLX_EXPORTED_LIBS ${TLX_EXPORTED_LIBS} PARENT_SCOPE)

################################################################################
