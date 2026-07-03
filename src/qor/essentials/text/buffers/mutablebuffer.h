// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TEXT_MUTABLEBUFFER
#define QOR_PP_H_TEXT_MUTABLEBUFFER

#include <string>
#include <string.h>
#include "src/platform/compiler/compiler.h"
#include "src/qor/essentials/text/buffers/constbuffer.h"
#include "src/qor/essentials/iterators/iterators.h"

#ifdef min
#undef min
#endif//min

namespace qor{

    namespace text{
        qor_pp_module_interface(QOR_TEXT) void OutOfRangeError(size_t index, size_t length, size_t elementSize, const void* bufferAddress);
        qor_pp_module_interface(QOR_TEXT) void BufferFreeWhileSharedError(const void* bufferAddress, unsigned short refCount);
        qor_pp_module_interface(QOR_TEXT) void BufferOverrunError(const void* bufferAddress, unsigned short headerRefCount, unsigned short footerRefCount);
    }

    struct sBufferHeader
    {
        size_t Alloc;								//Byte length of allocation for units, header, footer and all
        size_t RefCount;							//Reference count for CoW
        size_t Len;									//Unit count in use
    };

    struct sBufferFooter
    {
        unsigned short RefCount;
    };

    struct MutableBufferLayout
    {
    public:
        static const unsigned short HeaderSize{sizeof(sBufferHeader)};
        static const unsigned short FooterSize{sizeof(sBufferFooter)};
    };

    template< typename T, typename LayoutT = MutableBufferLayout >
    class MutableBuffer
    {
    public:

        typedef T value_type;
        typedef size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T* pointer;
        typedef const T* const_pointer;

        typedef rawiterator<T> iterator;
        typedef rawiterator<const T> const_iterator;
        typedef rawreverseiterator<T> reverse_iterator;
        typedef rawreverseiterator<const T> const_reverse_iterator;

        static const size_t scRounding = 0x10;//ensure buffer sizes are rounded up to nearest 16

        //A smart pointer to the buffer that can be handed to APIs like system calls to be written to and then validated
        //with the length written.
        class Data
        {
        public:

            Data(MutableBuffer<T, LayoutT>& buffer) : m_buffer(buffer)
            {
                m_buffer.PrepareToModify();
                m_buffer.AddRef();
            }

            T* operator -> () const
            {
                return m_buffer.m_p;
            }

            operator T*()
            {
                return m_buffer.m_p;
            }

            T& operator[](size_t index) const
            {
                if (m_buffer.m_p == nullptr || index >= m_buffer.Capacity())
                {
                    text::OutOfRangeError(index, m_buffer.Capacity(), sizeof(T), m_buffer.m_p);
                }
                return m_buffer.m_p[index];
            }

            bool IsNotNull()
            {
                T* p = operator T*();
                return p == nullptr ? false : true;
            }

            virtual ~Data()
            {
                if(m_validatedLength)
                {
                    m_buffer.Validate(m_validatedLength);
                }
                else
                {
                    m_buffer.Release();
                }
            }

            inline void Validate(size_t charCount)
            {
                m_validatedLength = charCount;
            }

        private:
            size_t m_validatedLength = 0;
            MutableBuffer<T, LayoutT>& m_buffer;
        };

        friend class Data;

        //std::basic_string_view adapter
        class View : public std::basic_string_view< T >
        {
        public:

            View() noexcept : std::basic_string_view< T >(){ }
            View(const MutableBuffer<T, LayoutT>& buffer) : std::basic_string_view< T >(buffer.GetData(), buffer.Length()), m_buffer((buffer)){ }
            View(const View& src) : std::basic_string_view< T >(src), m_buffer(src.m_buffer){ }
            virtual ~View(){ }
            
        private:            
            const MutableBuffer<T, LayoutT>& m_buffer;            
        };

        friend class View;

        MutableBuffer() : m_p{nullptr}{ }

        /*When a buffer is copied we just point at the same data and increment the reference count
        If neither 'copy' is changed the copying is essentially free. If either is changed a real
        copy will be made because the RefCount will be > 1*/
        MutableBuffer(const MutableBuffer& src) : m_p{src.m_p}
        {
            AddRef();            
        }

        //Whan a buffer is moved we steal the pointer. The move source is left empty.
        MutableBuffer(MutableBuffer&& src) noexcept : m_p{src.m_p}
        {
            src.m_p = nullptr;
        }

        //To get a mutable buffer from a const buffer we must copy the contents.
        MutableBuffer(const ConstBuffer<T>& src) : m_p{nullptr}
        {
            if(src.Length() > 0)
            {
                auto ptr = data(src.Length());
                memcpy(static_cast<T*>(ptr), src.GetData(), src.ByteLength());
                ptr.Validate(src.Length());
            }
        }

        MutableBuffer(const ConstBuffer<T>&& src) : m_p{nullptr}
        {
            if(src.Length() > 0)
            {
                auto ptr = data(src.Length());
                memcpy(static_cast<T*>(ptr), src.GetData(), src.ByteLength());
                ptr.Validate(src.Length());
            }
        }

        template<size_t N>
        MutableBuffer(T(&str)[N]) : m_p{nullptr}
        {
            if(N > 1)
            {
                auto ptr = data(N - 1);
                memcpy(static_cast<T*>(ptr), str, (N - 1) * sizeof(T));
                ptr.Validate(N - 1);
            }            
        }

        MutableBuffer(const T* pBuffer, size_t unitCount ) : m_p{nullptr}
        {
            if(unitCount > 0)
            {
                //NOTE: Source strings beginning with null are not allowed, no copy will be made.
                //However capacity will be reserved.
                if(pBuffer == nullptr || pBuffer[0] == T(0))
                {                
                    SetCapacity(unitCount);                    
                }
                else
                {
                    auto ptr = data(unitCount);
                    memcpy(static_cast<T*>(ptr), pBuffer, unitCount * sizeof(T));
                    ptr.Validate(unitCount);
                }
            }
        }

        MutableBuffer(size_t initialUnitCapacity) : m_p{nullptr}
        {
            SetCapacity(initialUnitCapacity);
        }

        MutableBuffer& operator = ( MutableBuffer&& src ) noexcept
        {
            if( this != &src )
            {
                m_p = src.m_p;
                src.m_p = nullptr;
            }
            return *this;
        }

        MutableBuffer& operator = ( const MutableBuffer& src )
        {
            if( this != &src )
            {        
                m_p = src.m_p;
                AddRef();
            }
            return *this;
        }

        virtual ~MutableBuffer()
        {
            Release();
        }

        inline size_t GetUnitCount() const
        {
            return Length();
        }

        inline size_t AllocationByteCount() const
        {
            return m_p ? InternalBaseHeader()->Alloc : 0;
        }

        //In use unit count.
        inline size_t Length() const
        {
            return m_p ? InternalBaseHeader()->Len : 0;
        }

        inline size_t size() const //for iterators
        {
            return Length();
        }

        inline size_t ByteLength() const
        {
            return Length() * sizeof(T);
        }

        inline bool IsEmpty(void) const
        {
            return Length() == 0;
        }

        //Returns unit capacity. How many units this buffer can store
        inline size_t Capacity() const
        {
            return m_p ?
                (AllocationByteCount() - (ClassHeaderByteSize() + ClassFooterByteSize())) / sizeof(T)
                : 0;
        }

        void Reset(void)
        {
            if (m_p)
            {
                if (InternalRefCount() > 1)
                {
                    SetCapacity(0);
                }
                InternalLen() = 0;
            }
        }

        //Reserve memory for the content in advance if you plan to make many small appends
        inline void Reserve(size_t unitCount)
        {
            if (unitCount > Capacity())
            {
                SetCapacity(unitCount);
            }
        }

        //Returns view to space for unitCount elements. Existing contents up to that count are preserved.
        inline Data data(size_t unitCount = 0)
        {
            if( unitCount == 0)
            {
                unitCount = Capacity();
            }
            else
            {
                SetCapacity(unitCount);
            }
            return Data(*this);
        }
        
        inline View view() const
        {                        
            return View(*this);
        }
        
        //Returns a raw Read Only pointer to the content or nullptr if the buffer is empty        
        inline const T* GetData() const
        {
            return m_p;
        }

        inline const T* c_str() const
        {
            return m_p;
        }

        T At(size_t index) const
        {
            if (m_p == nullptr || index >= Length())
            {
                text::OutOfRangeError(index, Length(), sizeof(T), m_p);
                return T(0);
            }				
            else
            {
                return m_p[index];
            }
        }

        bool Set(size_t index, const T& item)
        {
            if (index < Length())
            {
                if (InternalRefCount() > 1)
                {	//Copy On Write
                    Reallocate(Capacity());
                }
                m_p[index] = item;
                return true;
            }
            return false;
        }

        T operator[](size_t index) const
        {
            if (index >= Length() || m_p == nullptr)
            {
                text::OutOfRangeError(index, Length(), sizeof(T), m_p);
                return T(0);
            }
            else
            {
                return m_p[index];
            }
        }

        template<typename C = T>
        const std::basic_string<C> ToStdString() const
        {
            std::basic_string<C> s;
            s.assign((const C*)m_p, Length());
            return s;
        }

        operator std::basic_string<T>() const
        {
            return ToStdString();
        }

        //Write with CoW semantics as a resizable buffer.
        void Write(const T* sourceData, size_t sourceUnitCount)
        {
            if(sourceUnitCount == 0)
            {
                return;
            }
            if( m_p == nullptr )
            {
                SetCapacity(sourceUnitCount);
            }
            else if( sourceUnitCount > (Capacity()-InternalLen()) || InternalRefCount() > 1)
            {
                SetCapacity(InternalLen() + sourceUnitCount);
            }

            for(size_t counter = 0; counter < sourceUnitCount; counter++)
            {
                m_p[InternalLen()++] = sourceData[counter];
            }
        }

        inline MutableBuffer<T, LayoutT>& Append(const T* itemArray, size_t srcUnitCount)
        {
            Write(itemArray, srcUnitCount);
            return *this;
        }

        inline MutableBuffer<T, LayoutT>& Append(const T item)
        {
            return Append(const_cast<const T*>(&item), 1);
        }
        
        inline MutableBuffer<T, LayoutT>& Append(const MutableBuffer<T, LayoutT>& src)
        {
            return Append(src.GetData(), src.Length());
        }

        MutableBuffer<T, LayoutT>& Insert(size_t index, const MutableBuffer<T, LayoutT>& src)
        {
            if (!m_p)
            {
                return operator = (src);
            }

            if (index < Length())
            {
                size_t srcLen = src.Length();
                size_t oldSize = InternalLen();
                size_t newSize = static_cast<size_t>((oldSize + srcLen));		//New units required after insertion

                if (InternalRefCount() > 1 || (newSize > Capacity()))	//CoW 
                {
                    Reallocate(newSize);
                }

                size_t counter = oldSize;
                while (counter >= index)
                {
                    m_p[counter + srcLen] = m_p[counter];			//Move the tail to its new position
                    --counter;
                }

                counter = 0;
                while (counter < srcLen)
                {
                    m_p[index + counter] = src.At(counter);		//Insert elements from the source buffer
                    ++counter;
                }
                InternalLen() += srcLen;
            }
            else
            {
                return Append(src);
            }
            return *this;
        }

        MutableBuffer<T, LayoutT>& Remove(size_t index, size_t itemCount)
        {
            if (m_p && (index < Length()))
            {
                if (InternalRefCount() > 1)
                {	
                    Reallocate(Capacity());
                }

                if ((index + itemCount) < Length())
                {
                    for (size_t it = index; (it + itemCount) <= Length(); it++)
                    {
                        m_p[it] = m_p[it + itemCount];
                    }
                    InternalLen() -= itemCount;
                }
                else
                {
                    InternalLen() = index;
                }
            }
            return *this;
        }

        inline iterator begin() const
        {
            return iterator(m_p);
        }

        inline const_iterator cbegin() const
        {
            return const_iterator(m_p);
        }

        inline iterator end() const
        {
            return iterator(m_p + InternalLen());
        }

        inline const_iterator cend() const
        {
            return const_iterator(m_p + InternalLen());
        }

        inline reverse_iterator rbegin() const
        {
            if(InternalLen() == 0)
            {
                return rend();
            }
            return reverse_iterator(m_p + InternalLen() - 1);
        }

        inline const_reverse_iterator crbegin() const
        {
            if(InternalLen() == 0)
            {
                return crend();
            }
            return const_reverse_iterator(m_p + InternalLen() - 1);
        }

        inline reverse_iterator rend() const
        {
            return reverse_iterator(m_p - 1);
        }

        inline const_reverse_iterator crend() const
        {
            return const_reverse_iterator(m_p - 1);
        }

        //Overrride this to write any additional Header data
        virtual void HeaderOnCopy(byte* /*oldHeader*/, byte* /*newHeader*/){ }

        //Overrride this to write any additional Footer data
        virtual void FooterOnCopy(byte* /*oldFooter*/, byte* /*newFooter*/){ }

    protected:

        T* m_p;

        inline byte* GetHeader() const
        {
            return m_p ? ((reinterpret_cast<byte*>(const_cast<T*>(m_p))) - ClassHeaderByteSize()) : nullptr;
        }

        inline byte* GetFooter() const
        {
            return m_p ? 
                (((reinterpret_cast<byte*>(const_cast<T*>(m_p))) - ClassHeaderByteSize()) + AllocationByteCount() - ClassFooterByteSize())
                : nullptr;
        }

    private:

        static inline constexpr unsigned short ClassHeaderByteSize(void)
        {
            return LayoutT::HeaderSize;
        }
              
        static inline constexpr unsigned short ClassFooterByteSize(void)
        {
            return LayoutT::FooterSize;
        }

        inline sBufferHeader* InternalBaseHeader() const
        {
            return m_p ? reinterpret_cast<sBufferHeader*>((reinterpret_cast<byte*>(const_cast<T*>(m_p))) - sizeof(sBufferHeader)): nullptr;
        }

        inline sBufferFooter* InternalBaseFooter(void) const
        {
            return m_p ? 
                (reinterpret_cast<sBufferFooter*>(((reinterpret_cast<byte*>(m_p)) - ClassHeaderByteSize()) + AllocationByteCount())) - 1 
                : nullptr;
        }

        //Get the unit count in use from the header
        inline size_t& InternalLen() const
        {
            return InternalBaseHeader()->Len;
        }

        //Get the reference count from the footer
        inline unsigned short InternalRefCount() const
        {
            return m_p ? InternalBaseFooter()->RefCount : 0;
        }

        //Increment the reference count to share content between owning instances
        inline unsigned short AddRef() const
        {
            return m_p ? static_cast<unsigned short>(InternalBaseFooter()->RefCount++), static_cast<unsigned short>(InternalBaseHeader()->RefCount++) : 0;
        }

        //If you use the internal memory as a raw buffer the in use length will not be tracked.
        //Call Validate( unitCount ) to set the in use length. 
        //Use Validate() to set the in use length to Capacity.
        //Returns true if the Length is successfully set.
        bool Validate(size_t unitCount = (size_t)(-1))
        {
            bool bResult = false;

            if(m_p)
            {                
                if (unitCount != (size_t)(-1))
                {
                    if (unitCount <= Capacity())
                    {
                        InternalLen() = unitCount;
                        bResult = true;
                    }
                    else
                    {
                        InternalLen() = Capacity();
                    }
                }
                Release();
            }
            return bResult;
        }

        //Release a previously retrieved buffer
        unsigned short Release()
        {
            if (m_p)
            {
                BufferOverrunCheck();
                unsigned short usOldRefCount = InternalBaseFooter()->RefCount;
                if (usOldRefCount == 0)
                {
                    return 0;
                }

                unsigned short usRefCount = --(InternalBaseFooter()->RefCount);
                InternalBaseHeader()->RefCount = static_cast<size_t>(usRefCount);
                if (usRefCount == 0)
                {
                    Free();
                }
                return usRefCount;
            }
            else
            {
                return 0;
            }
        }

        //Sets or resets the allocation based on the required unit count.
        void SetCapacity(size_t unitCount)
        {
            Reallocate(unitCount);
        }

        //Free the entire buffer memory, header, content and footer
        void Free()
        {
            if (m_p)
            {
                if (InternalBaseFooter()->RefCount != 0)
                {
                    text::BufferFreeWhileSharedError(m_p, InternalBaseFooter()->RefCount);
                }
                byte* pData = (reinterpret_cast<byte*>(const_cast<T*>(m_p))) - ClassHeaderByteSize();
                delete[] pData;
            }
            m_p = nullptr;
        }

        //Before we hand out a modifiable buffer. 
        //make sure we aren't sharing a buffer and that any string we do have is null terminated
        inline void PrepareToModify()
        {
            if (InternalRefCount() > 1)
            {
                Reallocate(Capacity());
            }
            EnsureNullTermined();
        }

        //Write the header for a newly (re)allocated buffer
        void WriteHeader(byte* pNewHeader, size_t size, size_t refCount, size_t charCount)
        {
            memset(pNewHeader, 0, ClassHeaderByteSize());
            sBufferHeader* pHeader = reinterpret_cast<sBufferHeader*>(pNewHeader + ClassHeaderByteSize() - sizeof(sBufferHeader));
            pHeader->Alloc = size;
            pHeader->RefCount = refCount;
            pHeader->Len = charCount;
        }

        //Write the footer for a newly (re)allocated buffer
        void WriteFooter(byte* pNewFooter, unsigned short refCount)//Initial reference count of 1
        {
            memset(pNewFooter, 0, ClassFooterByteSize());
            sBufferFooter* pFooter = reinterpret_cast<sBufferFooter*>(pNewFooter + ClassFooterByteSize() - sizeof(sBufferFooter));
            pFooter->RefCount = refCount;
        }

        inline void EnsureNullTermined()
        {
            if(m_p && Length() && m_p[Length()-1] != T(0))
            {
                Append(T(0));                
            }
        }

        //If the buffer has written over the footer the footer and header ref counts will no longer match.
        void BufferOverrunCheck()
        {
            if(InternalBaseHeader()->RefCount != InternalBaseFooter()->RefCount)
            {
                text::BufferOverrunError(m_p, static_cast<unsigned short>(InternalBaseHeader()->RefCount), InternalBaseFooter()->RefCount);                
            }
        }

        void Reallocate(size_t unitCount)
        {
            //Allocation size is size of content + header + footer
            size_t size = (ClassHeaderByteSize() + ClassFooterByteSize() + (sizeof(T) * unitCount));
            size |= scRounding;
            byte* pAlloc = new byte[size];
#ifndef NDEBUG
            memset(pAlloc, 0, size);//clear the buffer to zero for easier debugging
#endif //!NDEBUG

            //Write the new header, copying any old header
            byte* pOldHeader = (m_p == nullptr) ? nullptr : GetHeader();
            WriteHeader(pAlloc, size, 1, pOldHeader ? std::min(Length(), unitCount) : 0);
            
            //Write the new footer
            byte* pOldFooter = m_p == nullptr ? nullptr : GetFooter();
            byte* pNewFooter = pAlloc + size - ClassFooterByteSize();            
            WriteFooter(pNewFooter, 1);//Initial reference count of 1
        
            //Derived classes that may extend the header and footer get an oportunity to copy extra data
            HeaderOnCopy(pOldHeader, pAlloc);
            FooterOnCopy(pOldFooter, pNewFooter);

            //Copy data, if any, from the old buffer to the new
            byte* pOldData = reinterpret_cast<byte*>(m_p);//Get the bytes of the old buffer
            byte* pNewData = pAlloc + ClassHeaderByteSize();//The new content will go after the header
            
            if (pOldData)
            {
                //Only copy up to the length of the shorter of the two buffers
                memcpy(pNewData, pOldData, std::min(Length(), unitCount) * sizeof(T));					
                Release();//Release the old data
            }

            //The new data becomes THE data here.
            m_p = reinterpret_cast<T*>(pNewData);
        }

    };

    typedef MutableBuffer<char> MutableBuffer7;
    typedef MutableBuffer<char8_t> MutableBuffer8;
    typedef MutableBuffer<char16_t> MutableBuffer16;
    typedef MutableBuffer<char32_t> MutableBuffer32;
}//qor

#endif//QOR_PP_H_TEXT_MUTABLEBUFFER
