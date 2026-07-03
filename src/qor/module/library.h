// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_LIBRARY
#define QOR_PP_H_LIBRARY

namespace qor{
    
    //A base class for static and dynamic library representative objects
	class Library
    {
    public:

        Library( const char* name, const char* version, bool bRegister = true);
        virtual ~Library() noexcept = default;

		inline const char* Name() const { return m_Name; }

        inline const char* Version() const { return m_Version; }

        void Append(const Library* last);                   //Append to the end of the chain

		inline const Library* Next() const                  //Get the next library in the chain
        {
            return m_Next;
        }

		inline void SetNext(const Library* next)            //Set the next library in the chain
        {
            m_Next = next;
        }

	protected:

		const char* m_Name;							        //The internal library name, not necessarily related to any file name
        const char* m_Version;
        const Library* m_Next;								//Pointer to next static library forming a chain
	
	private:
	
        Library(const Library&) = delete;                   //Library objects can't be copied or assinged
        Library& operator = (const Library&) = delete;
    };

}//qor

#endif//QOR_PP_H_LIBRARY