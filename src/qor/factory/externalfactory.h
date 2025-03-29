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

#ifndef QOR_PP_H_EXTERNALFACTORY
#define QOR_PP_H_EXTERNALFACTORY

#include <new>

//The purpose of a factory is create and dispose of instances of a type.

namespace qor
{
    template< class T >
    class ExternalFactory final
    {
    private:

        ExternalFactory() = delete;
		~ExternalFactory() = delete;

    public:

        static void Destruct(T* pt, size_t count = 1)
        {
            if(count == 1)
            {
                delete pt;
            }
            else
            {
                delete [] pt;
            }
        }
        
        static void TearDown(T* pt, size_t count = 1)
        {
            if(count == 1)
            {
                delete pt;
            }
            else
            {
                for( size_t del = 0; del < count; del++)
                {
                    (pt+del)->~T();
                }

                uint8_t* pMem = reinterpret_cast<uint8_t*>(pt);
                delete pMem;
            }
        }

        static T* Construct(size_t count = 1)
        {
            if(count == 1)
            {
                return new T();
            }
            else
            {
                T* result = new T[count];
                return result;
            }
        }

        template< typename... _p >
        static T* Build(size_t count, _p&&... p1)
        {
            if(count == 1)
            {
                return new T(p1...);
            }
            else
            {
                uint8_t* pMem = new uint8_t(sizeof(T)* count);                
                T* result = reinterpret_cast<T*>(pMem);
                for( size_t init = 0; init < count; init++)
                {
                    new(result + init)T(p1...);
                }
                return result;
            }
        }
    };
}//qor

#endif//QOR_PP_H_EXTERNALFACTORY