/*******************************************************************************
 * tlx/meta/has_member.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2016-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_META_HAS_MEMBER_HEADER
#define TLX_META_HAS_MEMBER_HEADER

namespace tlx {

//! \addtogroup meta
//! \{

/******************************************************************************/
// SFINAE check whether a class member exists.

// based on http://stackoverflow.com/questions/257288/is-it-possible
// -to-write-a-c-template-to-check-for-a-functions-existence

//! macro template for class member / attribute SFINAE test
#define TLX_MAKE_MEMBER_TEST(Member)                                       \
    template <typename Type>                                               \
    class has_member_ ## Member                                            \
    {                                                                      \
        template <typename C>                                              \
        static char test(decltype(&C::Member));                            \
        template <typename C>                                              \
        static int test(...);                                              \
    public:                                                                \
        static const bool value = (sizeof(test<Type>(0)) == sizeof(char)); \
    };

//! macro template for class template member SFINAE test
#define TLX_MAKE_TEMPLATE_MEMBER_TEST(Member)                              \
    template <typename Type, typename Param>                               \
    class has_member_ ## Member                                            \
    {                                                                      \
        template <typename C>                                              \
        static char test(decltype(&C::template Member<Param>));            \
        template <typename C>                                              \
        static int test(...);                                              \
    public:                                                                \
        static const bool value = (sizeof(test<Type>(0)) == sizeof(char)); \
    };

//! \}

} // namespace tlx

#endif // !TLX_META_HAS_MEMBER_HEADER

/******************************************************************************/
