// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_CODEPAGEREGENTRY
#define QOR_PP_H_CODEPAGEREGENTRY

#include "codepageregistry.h"
#include "codepage.h"

namespace qor {

    template< typename C, Mib M = Mib::C >
    class CodePageRegEntry final
    {
    public:

        inline CodePageRegEntry(CodePage< C, M >* cp)
        {
            TheCodePageRegistry()->Register( M, TypedAny< AbstractCharacterCodec< C > >(cp));
        }

        ~CodePageRegEntry()
        {
            TheCodePageRegistry()->Unregister(M);
        }
    };

}//qor

#endif//QOR_PP_H_CODEPAGEREGENTRY
