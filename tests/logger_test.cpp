/*******************************************************************************
 * tests/logger_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/die.hpp>
#include <tlx/logger.hpp>
#include <tlx/logger/all.hpp>

template <typename Lambda>
void check(const char* output, Lambda lambda) {
    tlx::LoggerCollectOutput collect;
    lambda();
    die_unequal(output, collect.get());
}

//! printable struct
struct MyPrintable {
    int a = 42;
};

std::ostream& operator << (std::ostream& os, const MyPrintable& p) {
    return os << p.a;
}

//! unprintable struct
struct MyUnprintable {
    int a = 42;
};

int main() {

    check("42 - abc\n",
          []() {
              LOG1 << 42 << " - abc";
          });

    check("42 abc\n",
          []() {
              sLOG1 << 42 << "abc";
          });

    check("(42,abc)\n(42,abc)\n",
          []() {
              LOG1 << std::make_pair(42, "abc");
              sLOG1 << std::make_pair(42, "abc");
          });

    check("(42,abc,5)\n(42,abc,5)\n",
          []() {
              LOG1 << std::make_tuple(42, "abc", 5);
              sLOG1 << std::make_tuple(42, "abc", 5);
          });

    check("(42,abc)\n(42,abc)\n",
          []() {
              LOG1 << std::make_tuple(42, "abc");
              sLOG1 << std::make_tuple(42, "abc");
          });

    check("(42)\n(42)\n",
          []() {
              LOG1 << std::make_tuple(42);
              sLOG1 << std::make_tuple(42);
          });

    check("()\n()\n",
          []() {
              LOG1 << std::make_tuple();
              sLOG1 << std::make_tuple();
          });

    check("(42,(abc,10),5)\n(42,(abc,10),5)\n",
          []() {
              LOG1 << std::make_tuple(42, std::make_pair("abc", 10), 5);
              sLOG1 << std::make_tuple(42, std::make_pair("abc", 10), 5);
          });

    check("[42,5,31]\n[42,5,31]\n",
          []() {
              std::vector<int> v {
                  { 42, 5, 31 }
              };
              LOG1 << v;
              sLOG1 << v;
          });

    check("[42,5,31]\n[42,5,31]\n",
          []() {
              std::array<int, 3> a {
                  { 42, 5, 31 }
              };
              LOG1 << a;
              sLOG1 << a;
          });

    check("[42,5,31]\n[42,5,31]\n",
          []() {
              std::deque<int> d {
                  { 42, 5, 31 }
              };
              LOG1 << d;
              sLOG1 << d;
          });

    check("{5,31,42}\n{5,31,42}\n",
          []() {
              std::set<int> s {
                  { 42, 5, 31 }
              };
              LOG1 << s;
              sLOG1 << s;
          });

    check("{5,5,31,42}\n{5,5,31,42}\n",
          []() {
              std::multiset<int> s {
                  { 42, 5, 31, 5 }
              };
              LOG1 << s;
              sLOG1 << s;
          });

    check("{5=b,31=c,42=a}\n{5=b,31=c,42=a}\n",
          []() {
              std::map<int, const char*> m {
                  { 42, "a" }, { 5, "b" }, { 31, "c" }
              };
              LOG1 << m;
              sLOG1 << m;
          });

    check("{5=b,31=c,42=a}\n{5=b,31=c,42=a}\n",
          []() {
              std::multimap<int, const char*> m {
                  { 42, "a" }, { 5, "b" }, { 31, "c" }
              };
              LOG1 << m;
              sLOG1 << m;
          });

    check("{42}\n{42}\n",
          []() {
              std::unordered_set<int> s { 42 };
              LOG1 << s;
              sLOG1 << s;
          });

    check("{42,42}\n{42,42}\n",
          []() {
              std::unordered_multiset<int> s { 42, 42 };
              LOG1 << s;
              sLOG1 << s;
          });

    check("{42=a}\n{42=a}\n",
          []() {
              std::unordered_map<int, const char*> m {
                  { 42, "a" }
              };
              LOG1 << m;
              sLOG1 << m;
          });

    check("{42=a}\n{42=a}\n",
          []() {
              std::unordered_multimap<int, const char*> m {
                  { 42, "a" }
              };
              LOG1 << m;
              sLOG1 << m;
          });

    check("42\n42\n",
          []() {
              MyPrintable m;
              LOG1 << tlx::wrap_unprintable(m);
              sLOG1 << tlx::wrap_unp(m);
          });

    check("<unprintable>\n<unprintable>\n",
          []() {
              MyUnprintable m;
              LOG1 << tlx::wrap_unprintable(m);
              sLOG1 << tlx::wrap_unp(m);
          });

    return 0;
}

/******************************************************************************/
