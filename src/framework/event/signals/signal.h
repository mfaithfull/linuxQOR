// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

/***********************************************************************
*
* Copyright (c) 2016-2021 Barbara Geller
* Copyright (c) 2016-2021 Ansel Sermersheim
*
* This file is part of CsSignal.
*
* CsSignal is free software, released under the BSD 2-Clause license.
* For license details refer to LICENSE provided with this project.
*
* CopperSpice is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* https://opensource.org/licenses/BSD-2-Clause
*
***********************************************************************/

#ifndef QOR_PP_H_SIGNALS_SIGNAL
#define QOR_PP_H_SIGNALS_SIGNAL

#include "do_signal.h"
#include "slotbase.h"

#define SIGNAL_1(...)  \
   __VA_ARGS__ {
// do not remove the "{", this is required for part two of the macro


#define SIGNAL_2(signalName, ...) \
      activate(*this, &std::remove_reference<decltype(*this)>::type::signalName, ##__VA_ARGS__); \
   }


#define SIGNAL_3(_CLASS, _RET, signalName, ...) \
      activate(*this, &std::remove_reference<decltype(*this)>::type::signalName, ##__VA_ARGS__); \
   }\
   typedef _RET(_CLASS::*signalName##Ptr)(__VA_ARGS__);\
   signalName##Ptr signalName##_Address();


#define qor_pp_signal_address(_CLASS, signalName)\
	_CLASS::signalName##Ptr _CLASS::signalName##_Address()\
	{\
		return &std::remove_reference<decltype(*this)>::type::signalName;\
	}

#define qor_pp_signal_func void qor_pp_noinline

#define qor_pp_signal(signalName, ...) \
      activate(*this, &std::remove_reference<decltype(*this)>::type::signalName, ##__VA_ARGS__)

#define qor_pp_emit(signalName, ...) \
      activate(*this, &std::remove_reference<decltype(*this)>::type::signalName, ##__VA_ARGS__)

#define qor_pp_signal_from(signalName) signalName##_Address

#define qor_pp_signal_ret( TYPE, CLASS, SIGNAL) \
SIGNAL_1(TYPE qor_pp_noinline SIGNAL())\
SIGNAL_3(CLASS, TYPE, SIGNAL)


#endif//QOR_PP_H_SIGNALS_SIGNAL