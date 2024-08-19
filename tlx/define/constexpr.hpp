/*******************************************************************************
 * tlx/define/constexpr.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2021 Lorenz Hübschle-Schneider <lorenz@4z2.de>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_DEFINE_CONSTEXPR_HEADER
#define TLX_DEFINE_CONSTEXPR_HEADER

namespace tlx {

//! \addtogroup tlx_define
//! \{

#if __cplusplus >= 201402L
#define TLX_ADVANCED_CONSTEXPR constexpr
#else
#define TLX_ADVANCED_CONSTEXPR inline
#endif

//! \}

} // namespace tlx

#endif // !TLX_DEFINE_CONSTEXPR_HEADER

/******************************************************************************/
