/*******************************************************************************
 * tlx/string/bitdump.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2019 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_STRING_BITDUMP_HEADER
#define TLX_STRING_BITDUMP_HEADER

#include <cstdint>
#include <string>

namespace tlx {

//! \addtogroup tlx_string
//! \{
//! \name Bitdump Methods
//! \{

/******************************************************************************/
// Bitdump Methods

/*!
 * Dump a (binary) string as a sequence of 8-bit little-endian bytes.
 *
 * \param data  binary data to output as bits
 * \param size  length of binary data
 * \return      string of binary digits
 */
std::string bitdump_le8(const void* const data, size_t size);

/*!
 * Dump a (binary) string as a sequence of 8-bit little-endian bytes.
 *
 * \param str  binary data to output as bits
 * \return     string of binary digits
 */
std::string bitdump_le8(const std::string& str);

/*!
 * Dump a (binary) item as a sequence of 8-bit little-endian bytes.
 *
 * \param t  binary data to output as bits
 * \return   string of binary digits
 */
template <typename Type>
std::string bitdump_le8_type(const Type& t) {
    return bitdump_le8(&t, sizeof(t));
}

/*----------------------------------------------------------------------------*/

/*!
 * Dump a (binary) string as a sequence of 8-bit big-endian bytes.
 *
 * \param data  binary data to output as bits
 * \param size  length of binary data
 * \return      string of binary digits
 */
std::string bitdump_be8(const void* const data, size_t size);

/*!
 * Dump a (binary) string as a sequence of 8-bit big-endian bytes.
 *
 * \param str  binary data to output as bits
 * \return     string of binary digits
 */
std::string bitdump_be8(const std::string& str);

/*!
 * Dump a (binary) item as a sequence of 8-bit big-endian bytes.
 *
 * \param t  binary data to output as bits
 * \return   string of binary digits
 */
template <typename Type>
std::string bitdump_be8_type(const Type& t) {
    return bitdump_be8(&t, sizeof(t));
}

//! \}
//! \}

} // namespace tlx

#endif // !TLX_STRING_BITDUMP_HEADER

/******************************************************************************/
