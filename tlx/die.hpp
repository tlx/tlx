/*******************************************************************************
 * tlx/die.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2016-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_DIE_HEADER
#define TLX_DIE_HEADER

#include <cstring>
#include <sstream>
#include <string>

namespace tlx {

/******************************************************************************/
// die macros

//! die with message - either throw an exception or die via abort()
void die_with_message(const std::string& msg);

//! die with message - either throw an exception or die via abort()
void die_with_message(const char* msg, const char* file, size_t line);

//! die with message - either throw an exception or die via abort()
void die_with_message(const std::string& msg, const char* file, size_t line);

//! Instead of abort(), throw the output the message via an exception.
#define die_with_sstream(msg)                                 \
    do {                                                      \
        std::ostringstream oss__;                             \
        oss__ << msg << " @ " << __FILE__ << ':' << __LINE__; \
        ::tlx::die_with_message(oss__.str());                 \
    } while (false)

//! Instead of abort(), throw the output the message via an exception.
#define die(msg)                          \
    do {                                  \
        die_with_sstream("DIE: " << msg); \
    } while (false)

/******************************************************************************/
// die_unless() and die_if()

//! Check condition X and die miserably if false. Same as assert() except this
//! is also active in Release mode.
#define die_unless(X)                                                     \
    do {                                                                  \
        if (!(X)) {                                                       \
            ::tlx::die_with_message(                                      \
                "DIE: Assertion \"" #X "\" failed!", __FILE__, __LINE__); \
        }                                                                 \
    } while (false)

//! Check condition X and die miserably if true. Opposite of assert() except
//! this is also active in Release mode.
#define die_if(X)                                                            \
    do {                                                                     \
        if (X) {                                                             \
            ::tlx::die_with_message(                                         \
                "DIE: Assertion \"" #X "\" succeeded!", __FILE__, __LINE__); \
        }                                                                    \
    } while (false)

/******************************************************************************/
// die_unequal()

//! helper method to compare two values in die_unequal()
template <typename TypeA, typename TypeB>
inline bool die_equal_compare(TypeA a, TypeB b) {
    return a == b;
}

template <>
inline bool die_equal_compare(const char* a, const char* b) {
    // compare string contents
    return std::strcmp(a, b) == 0;
}

template <>
inline bool die_equal_compare(float a, float b) {
    // special case for NAN
    return a != a ? b != b : a == b;
}

template <>
inline bool die_equal_compare(double a, double b) {
    // special case for NAN
    return a != a ? b != b : a == b;
}

//! Check that X == Y or die miserably, but output the values of X and Y for
//! better debugging.
#define die_unequal(X, Y)                                                \
    do {                                                                 \
        auto x__ = (X);                                     /* NOLINT */ \
        auto y__ = (Y);                                     /* NOLINT */ \
        if (!::tlx::die_equal_compare(x__, y__))                         \
            die_with_sstream("DIE-UNEQUAL: " #X " != " #Y " : "          \
                             "\"" << x__ << "\" != \"" << y__ << "\"");  \
    } while (false)

/******************************************************************************/
// die_noexcept()

//! Define to check that [code] throws and exception of given type
#define die_noexcept(code, exception_type)                             \
    do {                                                               \
        bool test__ = false;                                           \
        try {                                                          \
            code;                                                      \
        }                                                              \
        catch (const exception_type&) {                                \
            test__ = true;                                             \
        }                                                              \
        if (test__)                                                    \
            break;                                                     \
        ::tlx::die_with_message(                                       \
            "DIE-NOEXCEPT: " #code " - NO EXCEPTION " #exception_type, \
            __FILE__, __LINE__);                                       \
    } while (false)

} // namespace tlx

#endif // !TLX_DIE_HEADER

/******************************************************************************/
