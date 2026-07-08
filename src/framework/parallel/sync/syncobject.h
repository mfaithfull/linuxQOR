// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_SYNC_OBJECT
#define QOR_PP_H_SYNC_OBJECT


namespace qor{
		
    class SyncObject
    {
    public:

        SyncObject() = default;
        virtual ~SyncObject() = default;

        virtual void Acquire(void) = 0;
        virtual void Release(void) = 0;
        virtual bool IsLocked(void) const {return false;}

    };

}//qor

#endif//QOR_PP_H_SYNC_OBJECT
