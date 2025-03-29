// Copyright Querysoft Limited 2008 - 2025
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef QOR_PP_H_GUID
#define QOR_PP_H_GUID

#include <stdint.h>
#include "../../platform/compiler/compiler.h"

namespace qor{
    
    struct GUID;
    
    bool qor_pp_module_interface(QOR_DATASTRUCTURES) IsEqualGUID(const GUID& rguid1, const GUID& rguid2);

	struct qor_pp_module_interface(QOR_DATASTRUCTURES) GUID //A structure for globally unique identification
	{
		uint32_t Data1;
		uint16_t Data2;
		uint16_t Data3;
		uint8_t Data4[8];

		bool operator == (const GUID& guidOther) const
        {
            return IsEqualGUID(*this, guidOther) ? true : false;
        }
    
		bool operator != (const GUID& guidOther) const
        {
            return !IsEqualGUID(*this, guidOther) ? true : false;
        }

    };

    constexpr GUID null_guid = {0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0}};

	typedef const GUID IID;

	qor_pp_module_interface(QOR_DATASTRUCTURES) bool operator < (const GUID& guidOne, const GUID& guidOther);
    qor_pp_module_interface(QOR_DATASTRUCTURES) bool operator > (const GUID& guidOne, const GUID& guidOther);

}//qor

#endif//QOR_PP_H_GUID
