// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_SYNC_NULLSECTION
#define QOR_PP_H_SYNC_NULLSECTION

#include "syncobject.h"

namespace qor{
		
	class NullSection final : public SyncObject 
	{
	public:

		NullSection(){};
		virtual ~NullSection(){};
		virtual void Acquire(){}
		virtual void Release(){}
		virtual bool IsLocked(void) const {return false;}
	};

}//qor

#endif//QOR_PP_H_SYNC_NULLSECTION
