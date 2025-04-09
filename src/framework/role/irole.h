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

#ifndef QOR_PP_H_ROLE_INTERFFACE
#define QOR_PP_H_ROLE_INTERFFACE

#include "ifeature.h"
#include "src/qor/injection/typeidentity.h"
#include "src/qor/reference/reference.h"
#include "src/framework/thread/currentthread.h"
#include "src/qor/objectcontext/anyobject.h"
#include "src/qor/reference/newref.h"

namespace qor{ namespace framework{

    class IRole
    {
    public:

        IRole() = default;
        virtual ~IRole() = default;
        virtual void Setup() = 0;
        virtual void Shutdown() = 0;
        virtual void AddFeature( const GUID* id, ref_of<IFeature>::type feature) = 0;
        virtual ref_of<IFeature>::type GetFeature(const GUID* id) = 0;
    };

}}//qor::framework

#endif//QOR_PP_H_WORKFLOW_INTERFFACE
