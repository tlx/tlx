/*******************************************************************************
 * tlx/string/word_wrap.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2016-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_STRING_WORD_WRAP_HEADER
#define TLX_STRING_WORD_WRAP_HEADER

#include <tlx/container/string_view.hpp>
#include <string>

namespace tlx {

//! \addtogroup tlx_string
//! \{

/*!
 * Try to wrap a string to 80 columns without split words. All newlines are
 * kept, new newline characters are inserted only at spaces, hence, words are
 * never split. If words longer than 80 columns occur they are NOT broken.
 */
std::string word_wrap(tlx::string_view str, unsigned int wrap = 80);

//! \}

} // namespace tlx

#endif // !TLX_STRING_WORD_WRAP_HEADER

/******************************************************************************/
