/*******************************************************************************
 * tlx/digest/sha256.hpp
 *
 * Public domain implementation of SHA-256 (SHA-2) processor. Copied from
 * https://github.com/kalven/sha-2, which is based on LibTomCrypt.
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_DIGEST_SHA256_HEADER
#define TLX_DIGEST_SHA256_HEADER

#include <cstdint>
#include <string>

namespace tlx {

//! \addtogroup tlx_digest
//! \{

/*!
 * SHA-256 processor without external dependencies.
 */
class SHA256
{
public:
    //! construct empty object.
    SHA256();
    //! construct context and process data range
    SHA256(const void* data, uint32_t size);
    //! construct context and process string
    explicit SHA256(const std::string& str);

    //! process more data
    void process(const void* data, uint32_t size);
    //! process more data
    void process(const std::string& str);

    //! digest length in bytes
    static constexpr size_t kDigestLength = 32;

    //! finalize computation and output 32 byte (256 bit) digest
    void finalize(void* digest);

    //! finalize computation and return 32 byte (256 bit) digest
    std::string digest();
    //! finalize computation and return 32 byte (256 bit) digest hex encoded
    std::string digest_hex();
    //! finalize computation and return 32 byte (256 bit) digest upper-case hex
    std::string digest_hex_uc();

private:
    uint64_t length_;
    uint32_t state_[8];
    uint32_t curlen_;
    uint8_t buf_[64];
};

//! process data and return 32 byte (256 bit) digest hex encoded
std::string sha256_hex(const void* data, uint32_t size);
//! process data and return 32 byte (256 bit) digest hex encoded
std::string sha256_hex(const std::string& str);

//! process data and return 32 byte (256 bit) digest upper-case hex encoded
std::string sha256_hex_uc(const void* data, uint32_t size);
//! process data and return 32 byte (256 bit) digest upper-case hex encoded
std::string sha256_hex_uc(const std::string& str);

//! \}

} // namespace tlx

#endif // !TLX_DIGEST_SHA256_HEADER

/******************************************************************************/
