/*******************************************************************************
 * tlx/digest/md5.hpp
 *
 * Public domain implementation of MD-5 processor. Based on LibTomCrypt from
 * https://github.com/libtom/libtomcrypt.git
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_DIGEST_MD5_HEADER
#define TLX_DIGEST_MD5_HEADER

#include <tlx/container/string_view.hpp>
#include <cstddef>
#include <cstdint>
#include <string>

namespace tlx {

//! \addtogroup tlx_digest
//! \{

/*!
 * MD-5 processor without external dependencies.
 */
class MD5
{
public:
    //! construct empty object.
    MD5();
    //! construct context and process data range
    MD5(const void* data, std::uint32_t size);
    //! construct context and process string
    explicit MD5(tlx::string_view str);

    //! process more data
    void process(const void* data, std::uint32_t size);
    //! process more data
    void process(tlx::string_view str);

    //! digest length in bytes
    static constexpr size_t kDigestLength = 16;

    //! finalize computation and output 16 byte (128 bit) digest
    void finalize(void* digest);

    //! finalize computation and return 16 byte (128 bit) digest
    std::string digest();
    //! finalize computation and return 16 byte (128 bit) digest hex encoded
    std::string digest_hex();
    //! finalize computation and return 16 byte (128 bit) digest upper-case hex
    std::string digest_hex_uc();

private:
    std::uint64_t length_ = 0;
    std::uint32_t state_[4];
    std::uint32_t curlen_ = 0;
    std::uint8_t buf_[64];
};

//! process data and return 16 byte (128 bit) digest hex encoded
std::string md5_hex(const void* data, std::uint32_t size);
//! process data and return 16 byte (128 bit) digest hex encoded
std::string md5_hex(tlx::string_view str);

//! process data and return 16 byte (128 bit) digest upper-case hex encoded
std::string md5_hex_uc(const void* data, std::uint32_t size);
//! process data and return 16 byte (128 bit) digest upper-case hex encoded
std::string md5_hex_uc(tlx::string_view str);

//! \}

} // namespace tlx

#endif // !TLX_DIGEST_MD5_HEADER

/******************************************************************************/
