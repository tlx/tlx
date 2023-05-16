/*******************************************************************************
 * tests/deprecated_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Manuel Penschuck <tlx@manuel.jetzt>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <iostream>
#include <tlx/define/deprecated.hpp>

TLX_DEPRECATED(void do_not_use_me_anymore());
void do_not_use_me_anymore() {
}

TLX_DEPRECATED_FUNC_DEF(int also_do_not_use()) {
    return 0;
}

int main() {
    do_not_use_me_anymore();
    also_do_not_use();

    std::cout << "This test takes place during compilation.\n"
              << "Nothing to see here, move on!"
              << std::endl;
    return 0;
}

/******************************************************************************/
