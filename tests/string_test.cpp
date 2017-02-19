/*******************************************************************************
 * tests/string_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <random>
#include <stdexcept>

#include <tlx/die.hpp>
#include <tlx/string.hpp>

//! Returns an initialized unsigned char[] array inside an std::string
#define ARRAY_AS_STRING(array) \
    std::string(reinterpret_cast<const char*>(array), sizeof(array))

/*!
 * Generate a random binary string of given length. Any byte from 0-256 is
 * equally probable. Uses the pseudo-random number generator from stdlib; take
 * care to seed it using srand() before calling this function.
 *
 * \param size  length of result
 * \return      random binary string of given length
 */
static inline
std::string random_binary(std::string::size_type size) {
    static std::random_device random_device;
    std::minstd_rand prng(random_device());

    std::string out;
    out.resize(size);

    for (size_t i = 0; i < size; ++i)
        out[i] = static_cast<unsigned char>(prng() % 256);

    return out;
}

static void test_base64() {
    // take some static hex data and dump it using base64 encoding, then decode
    // it again.
    const unsigned char rand1data[42] = {
        0x16, 0x35, 0xCA, 0x03, 0x90, 0x6B, 0x47, 0x11,
        0x85, 0x02, 0xE7, 0x40, 0x9E, 0x3A, 0xCE, 0x43,
        0x0C, 0x57, 0x3E, 0x35, 0xE7, 0xA6, 0xB2, 0x37,
        0xEC, 0x6D, 0xF6, 0x68, 0xF6, 0x0E, 0x74, 0x0C,
        0x44, 0x3F, 0x0F, 0xD4, 0xAA, 0x56, 0xE5, 0x2F,
        0x58, 0xCC
    };

    std::string rand1 = ARRAY_AS_STRING(rand1data);

    std::string rand1base64 = tlx::base64_encode(rand1);

    die_unequal(rand1base64,
                "FjXKA5BrRxGFAudAnjrOQwxXPjXnprI37G32aPYOdAxEPw/UqlblL1jM");

    die_unequal(tlx::base64_decode(rand1base64), rand1);

    // check line-splitting
    std::string rand1base64lines = tlx::base64_encode(rand1, 16);

    die_unequal(rand1base64lines,
                "FjXKA5BrRxGFAudA\n" "njrOQwxXPjXnprI3\n"
                "7G32aPYOdAxEPw/U\n" "qlblL1jM");

    // take three random binary data string with different sizes and run
    // the base64 encoding->decoding->checking drill.

    std::string rand12 = random_binary(12);
    die_unequal(tlx::base64_decode(tlx::base64_encode(rand12)), rand12);

    std::string rand13 = random_binary(13);
    die_unequal(tlx::base64_decode(tlx::base64_encode(rand13)), rand13);

    std::string rand14 = random_binary(14);
    die_unequal(tlx::base64_decode(tlx::base64_encode(rand14)), rand14);

    // run a larger set of random tests
    for (unsigned int ti = 0; ti < 1000; ++ti)
    {
        unsigned int randlen = ti; // rand() % 1000;
        std::string randstr = random_binary(randlen);

        die_unequal(
            tlx::base64_decode(tlx::base64_encode(randstr)), randstr);
    }

    die_noexcept(
        tlx::base64_decode("FjXKA5!!RxGFAudA"), std::runtime_error);
}

static void test_hexdump() {

    // take hex data and dump it into a string, then parse back into array
    const unsigned char hexdump[8] = {
        0x8D, 0xE2, 0x85, 0xD4, 0xBF, 0x98, 0xE6, 0x03
    };

    std::string hexdata = ARRAY_AS_STRING(hexdump);
    std::string hexstring = tlx::hexdump(hexdata);

    die_unequal(hexstring, "8DE285D4BF98E603");
    die_unequal(tlx::hexdump(hexdump, sizeof(hexdump)), "8DE285D4BF98E603");

    std::string hexparsed = tlx::parse_hexdump(hexstring);
    die_unequal(hexparsed, hexdata);

    // dump random binary string into hex and parse it back
    std::string rand1 = random_binary(42);
    die_unequal(tlx::parse_hexdump(tlx::hexdump(rand1)), rand1);

    // take the first hex list and dump it into source code format, then
    // compare it with correct data (which was also dumped with
    // hexdump_sourcecode())
    std::string hexsource = tlx::hexdump_sourcecode(hexdata, "abc");

    const unsigned char hexsourcecmp[68] = {
        0x63, 0x6F, 0x6E, 0x73, 0x74, 0x20, 0x75, 0x69,
        0x6E, 0x74, 0x38, 0x5F, 0x74, 0x20, 0x61, 0x62,
        0x63, 0x5B, 0x38, 0x5D, 0x20, 0x3D, 0x20, 0x7B,
        0x0A, 0x30, 0x78, 0x38, 0x44, 0x2C, 0x30, 0x78,
        0x45, 0x32, 0x2C, 0x30, 0x78, 0x38, 0x35, 0x2C,
        0x30, 0x78, 0x44, 0x34, 0x2C, 0x30, 0x78, 0x42,
        0x46, 0x2C, 0x30, 0x78, 0x39, 0x38, 0x2C, 0x30,
        0x78, 0x45, 0x36, 0x2C, 0x30, 0x78, 0x30, 0x33,
        0x0A, 0x7D, 0x3B, 0x0A
    };

    die_unequal(hexsource, ARRAY_AS_STRING(hexsourcecmp));

    // test parse_hexdump with illegal strings
    die_noexcept(tlx::parse_hexdump("illegal"), std::runtime_error);
    die_noexcept(tlx::parse_hexdump("8DE285D4BF98E60"), std::runtime_error);
}

void test_replace() {
    // copy variants
    die_unequal(
        tlx::replace_first_copy("abcdef abcdef", "abc", "a"), "adef abcdef");
    die_unequal(
        tlx::replace_first_copy("abcdef abcdef", "cba", "a"), "abcdef abcdef");
    die_unequal(
        tlx::replace_all_copy("abcdef abcdef", "abc", "a"), "adef adef");
    die_unequal(
        tlx::replace_all_copy("abcdef abcdef", "cba", "a"), "abcdef abcdef");

    die_unequal(
        tlx::replace_first_copy("abcdef abcdef", "a", "aaa"),
        "aaabcdef abcdef");
    die_unequal(
        tlx::replace_all_copy("abcdef abcdef", "a", "aaa"),
        "aaabcdef aaabcdef");

    // in-place variants
    std::string str1 = "abcdef abcdef";
    std::string str2 = "abcdef abcdef";
    die_unequal(
        tlx::replace_first(&str1, "abc", "a"), "adef abcdef");
    die_unequal(
        tlx::replace_first(&str2, "cba", "a"), "abcdef abcdef");

    str1 = "abcdef abcdef";
    str2 = "abcdef abcdef";
    die_unequal(
        tlx::replace_all(&str1, "abc", "a"), "adef adef");
    die_unequal(
        tlx::replace_all(&str2, "cba", "a"), "abcdef abcdef");

    str1 = "abcdef abcdef";
    str2 = "abcdef abcdef";
    die_unequal(
        tlx::replace_first(&str1, "a", "aaa"), "aaabcdef abcdef");
    die_unequal(
        tlx::replace_all(&str2, "a", "aaa"), "aaabcdef aaabcdef");
}

static void test_starts_with_ends_with() {

    die_unless(tlx::starts_with("abcdef", "abc"));
    die_unless(!tlx::starts_with("abcdef", "def"));
    die_unless(tlx::ends_with("abcdef", "def"));
    die_unless(!tlx::ends_with("abcdef", "abc"));

    die_unless(!tlx::starts_with("abcdef", "ABC"));

    die_unless(tlx::starts_with_icase("abcdef", "ABC"));
    die_unless(!tlx::starts_with_icase("abcdef", "DEF"));
    die_unless(tlx::ends_with_icase("abcdef", "DEF"));
    die_unless(!tlx::ends_with_icase("abcdef", "ABC"));

    die_unless(tlx::starts_with("abcdef", ""));
    die_unless(tlx::ends_with("abcdef", ""));

    die_unless(!tlx::starts_with("", "abc"));
    die_unless(!tlx::ends_with("", "abc"));

    die_unless(tlx::starts_with("", ""));
    die_unless(tlx::ends_with("", ""));
}

int main() {

    test_base64();
    test_hexdump();
    test_replace();
    test_starts_with_ends_with();

    return 0;
}

/******************************************************************************/
