/*******************************************************************************
 * tests/container/splay_tree_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2016-2019 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/container/splay_tree.hpp>

#include <tlx/die.hpp>

#include <algorithm>
#include <deque>
#include <iterator>
#include <random>
#include <set>
#include <vector>

struct MyStruct {
    int i1, i2;

    MyStruct(int _i1, int _i2)
        : i1(_i1), i2(_i2) { }
};

struct MyStructCompare {
    bool operator () (const MyStruct& a, const MyStruct& b) const {
        return a.i1 < b.i1;
    }
};

template class tlx::SplayTree<size_t>;
template class tlx::SplayTree<MyStruct, MyStructCompare>;

void compare(const tlx::SplayTree<size_t>& tree,
             const std::set<size_t>& check) {
    die_unless(tree.check());

    std::vector<size_t> preorder;
    tree.traverse_preorder(
        [&](const size_t& t) { preorder.push_back(t); });

    std::vector<size_t> check_vec(check.begin(), check.end());
    die_unless(check_vec == preorder);
}

void test1() {

    using Tree = tlx::SplayTree<size_t>;

    Tree tree;
    std::set<size_t> check;

    for (size_t i = 0; i < 100; i++) {
        size_t value = (541 * i) & 1023;
        tree.insert(value);
        check.insert(value);
    }

    compare(tree, check);

    for (size_t i = 0; i < 100; i++) {
        size_t value = (541 * i) & 1023;
        tree.erase(value);
        check.erase(value);

        compare(tree, check);
    }

    for (size_t i = 0; i < 100; i++) {
        size_t value = (541 * i) & 1023;
        tree.insert(value);
        check.insert(value);
    }

    std::vector<size_t> check_vec(check.begin(), check.end());

    for (size_t i = 0; i < 1000; i = i + 20) {
        const Tree::Node* t = tree.find(i);
        die_unequal(check.count(i) == 1, t->key == i);
    }

    for (size_t i = 0; i < 100; i++) {
        tree.erase((541 * i) & 1023);
    }
}

void test2_random() {

    tlx::SplayTree<size_t> tree;
    std::deque<size_t> check;

    std::default_random_engine rng(1234);

    size_t limit = 1000;
    for (size_t i = 0; i < limit; ++i) {
        size_t op = rng() % 2;
        if (op == 0 && limit > i + check.size())
        {
            size_t v = rng();
            tree.insert(v);
            check.insert(std::lower_bound(check.begin(), check.end(), v), v);
        }
        else if (check.size())
        {
            size_t idx = rng() % check.size();
            auto it = check.begin() + idx;

            die_unless(tree.exists(*it));
            tree.erase(*it);
            check.erase(it);
        }

        std::deque<size_t> preorder;
        tree.traverse_preorder(
            [&](const size_t& t) { preorder.push_back(t); });

        die_unless(check == preorder);
    }
}

void test3_empty() {

    tlx::SplayTree<size_t> tree;

    for (size_t i = 0; i < 100; i++) {
        size_t value = (541 * i) & 1023;
        tree.insert(value);
    }
}

int main() {
    test1();
    test2_random();
    test3_empty();

    return 0;
}

/******************************************************************************/
