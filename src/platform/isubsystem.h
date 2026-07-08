// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_PLATFORM_SUBSYSTEM_INTERFACE
#define QOR_PP_H_PLATFORM_SUBSYSTEM_INTERFACE

//Base interface for platform sub systems

namespace qor{

    class ISubsystem
    {
    public:

        ISubsystem() = default;
        virtual ~ISubsystem() = default;

        virtual void Setup(){};
        virtual void Shutdown(){};

     private:

        ISubsystem(const ISubsystem&) = delete;
        ISubsystem& operator = (const ISubsystem&) = delete;

    };

}//qor

#endif//QOR_PP_H_PLATFORM_SUBSYSTEM_INTERFACE
