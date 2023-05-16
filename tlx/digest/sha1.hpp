/*******************************************************************************
 * tlx/digest/sha1.hpp
 *
 * Public domain implementation of SHA-1 processor. Based on LibTomCrypt from
 * https://github.com/libtom/libtomcrypt.git
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_DIGEST_SHA1_HEADER
#define TLX_DIGEST_SHA1_HEADER

#include <cstdint>
#include <string>

namespace tlx {

//! \addtogroup tlx_digest
//! \{

/*!
 * SHA-1 processor without external dependencies.
 */
class SHA1
{
public:
    //! construct empty object.
    SHA1();
    //! construct context and process data range
    SHA1(const void* data, std::uint32_t size);
    //! construct context and process string
    explicit SHA1(const std::string& str);

    //! process more data
    void process(const void* data, std::uint32_t size);
    //! process more data
    void process(const std::string& str);

    //! digest length in bytes
    static constexpr size_t kDigestLength = 20;

    //! finalize computation and output 20 byte (160 bit) digest
    void finalize(void* digest);

    //! finalize computation and return 20 byte (160 bit) digest
    std::string digest();
    //! finalize computation and return 20 byte (160 bit) digest hex encoded
    std::string digest_hex();
    //! finalize computation and return 20 byte (160 bit) digest upper-case hex
    std::string digest_hex_uc();

private:
    std::uint64_t length_;
    std::uint32_t state_[5];
    std::uint32_t curlen_;
    std::uint8_t buf_[64];
};

//! process data and return 20 byte (160 bit) digest hex encoded
std::string sha1_hex(const void* data, std::uint32_t size);
//! process data and return 20 byte (160 bit) digest hex encoded
std::string sha1_hex(const std::string& str);

//! process data and return 20 byte (160 bit) digest upper-case hex encoded
std::string sha1_hex_uc(const void* data, std::uint32_t size);
//! process data and return 20 byte (160 bit) digest upper-case hex encoded
std::string sha1_hex_uc(const std::string& str);

//! \}

} // namespace tlx

#endif // !TLX_DIGEST_SHA1_HEADER

/******************************************************************************/
