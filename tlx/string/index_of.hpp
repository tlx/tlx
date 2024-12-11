/*******************************************************************************
 * tlx/string/index_of.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_STRING_INDEX_OF_HEADER
#define TLX_STRING_INDEX_OF_HEADER

#include <tlx/container/string_view.hpp>
#include <cstddef>
#include <string>
#include <vector>

namespace tlx {

//! \addtogroup tlx_string
//! \{

/*!
 * Attempts to find str in the list and return the index. Throws a
 * std::runtime_error if it is not found.
 */
size_t index_of(const std::vector<std::string>& list, tlx::string_view str);

/*!
 * Attempts to find str in the list and return the index. Throws a
 * std::runtime_error if it is not found.
 */
size_t index_of(const std::vector<tlx::string_view>& list, tlx::string_view str);

/*!
 * Attempts to find str in the list and return the index using case-insensitive
 * comparisons. Throws a std::runtime_error if it is not found.
 */
size_t index_of_icase(const std::vector<std::string>& list,
                      tlx::string_view str);

/*!
 * Attempts to find str in the list and return the index using case-insensitive
 * comparisons. Throws a std::runtime_error if it is not found.
 */
size_t index_of_icase(const std::vector<tlx::string_view>& list,
                      tlx::string_view str);

//! \}

} // namespace tlx

#endif // !TLX_STRING_INDEX_OF_HEADER

/******************************************************************************/
