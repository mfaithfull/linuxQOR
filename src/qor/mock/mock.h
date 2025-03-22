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

//Derived from HippoMocks
//Copyright (C) 2008, Bas van Tiel, Christian Rexwinkel, Mike Looijmans, Peter Bindels
//under GNU LGPL v2.1

#ifndef QOR_PP_H_TESTMOCK_MOCK
#define QOR_PP_H_TESTMOCK_MOCK

#include<memory>
#include<map>
#include<vector>

namespace qor{ namespace mock{
    
    template <class T>
    class mock : public base_mock
    {
    private:

        typedef void (*funcptr)();
        friend class MockRepository;
        unsigned char remaining[sizeof(T)];

        void NotImplemented()
        {
            MockRepoInstanceHolder<0>::reporter->UnknownFunction(*MockRepoInstanceHolder<0>::instance);
        }

        std::unique_ptr<RttiInfo> rttiinfo;

    protected:

        std::map<int, void (**)()> funcTables;
        void (*notimplementedfuncs[qor_pp_mock_virt_func_limit])();

    public:

        bool isZombie;
        std::vector<std::unique_ptr<TypeDestructable>> members;
        MockRepository* repo;
        std::map<std::pair<int, int>, int> funcMap;
        
        mock(MockRepository* repository) : isZombie(false) , repo(repository)
        {
            for (int i = 0; i < qor_pp_mock_virt_func_limit; i++)
            {
                notimplementedfuncs[i] = getNonvirtualMemberFunctionAddress<void (*)()>(&mock<T>::NotImplemented);
            }
            funcptr* funcTable = new funcptr[qor_pp_mock_virt_func_limit + 4] + 2;
            memcpy(funcTable, notimplementedfuncs, sizeof(funcptr) * qor_pp_mock_virt_func_limit);
            ((void**)funcTable)[qor_pp_mock_virt_func_limit] = this;
            ((void**)funcTable)[qor_pp_mock_virt_func_limit + 1] = *(void**)this;

            rttiinfo.reset(new RttiInfo(typeid(*this), typeid(T)));
            ((void**)funcTable)[-1] = rttiinfo.get();
            ((void**)funcTable)[-2] = 0;

            funcTables[0] = funcTable;
            *(void**)this = funcTable;
            for (unsigned int i = 1; i < sizeof(remaining) / sizeof(funcptr); i++)
            {
                ((void**)this)[i] = (void*)notimplementedfuncs;
            }
        }
        
        ~mock()
        {
            for (auto& p : funcTables)
            {
                delete[](p.second - 2);
            }
        }
        
        mock<T>* getRealThis()
        {
            void*** base = (void***)this;
            return (mock<T> *)((*base)[qor_pp_mock_virt_func_limit]);
        }

        std::pair<int, int> translateX(int x)
        {
            for (auto& f : funcMap)
                if (f.second == x + 1) return f.first;
            return std::pair<int, int>(-1, 0);
        }

        template <int X>
        void mockedDestructor(int);
    };

}}//qor::mock

#endif//QOR_PP_H_TESTMOCK_MOCK
