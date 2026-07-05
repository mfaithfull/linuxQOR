// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_TFLINK
#define QOR_PP_H_TFLINK

#include <stdint.h>
#include <type_traits>

namespace qor{
    
	template< class T > requires std::is_default_constructible_v<T>
	class tflink
	{
	public:	

		tflink() : m_Item( T() ){ }

		tflink( T t ) : m_Item( t ){ }
		
		//A slow but simple append implementation.
		//Appending to any item in the list always appends at the end.
		void Append(tflink< T >* next )
		{
			tflink< T >* end = this;
			while( end->m_Next != nullptr )
			{
				end = end->m_Next;
			}
			end->m_Next = next;
		}

		T& Item()
		{
			return m_Item;
		}

		T m_Item;

	protected:

		tflink< T >* m_Next{nullptr};
		
	};

}//qor

#endif//QOR_PP_H_TFLINK
