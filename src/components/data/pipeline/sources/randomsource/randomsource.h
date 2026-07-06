// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_RANDOM
#define QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_RANDOM

#include <random>

#include "src/framework/data/pipeline/source.h"

namespace qor{ namespace pipeline{ namespace components{ 

    class qor_pp_module_interface(QOR_RANDOMSOURCE) RandomSource : public pipeline::Source
    {
    public:

        RandomSource();
        virtual ~RandomSource();

    protected:

        virtual size_t ReadBytes(byte* buffer, size_t bytesToRead);
        unsigned int Next();

    private:
      
        std::random_device m_rnd;
        int m_start;
    };

}}}//qor::pipeline::components

#endif//QOR_PP_H_COMPONENTS_PIPELINE_SOURCES_RANDOM
