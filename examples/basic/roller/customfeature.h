// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include <string>
#include <iostream>

#include "src/framework/app/role/ifeature.h"

class CustomFeature : public qor::app::IFeature               //To create a custom feature derive from IFeature
{
public:

    CustomFeature() = default;
    virtual ~CustomFeature() = default;

    //Override Setup and shutdown for your feature these will be called by the Role

    virtual void Setup() 
    {
        //Do one time setup for CustomFeature here
    }

    virtual void Shutdown() 
    {
        //Shutdown and cleanup CustomFeature here
    }

    //Add whatever else your feature requires
    void DoThatConfigurationThing(const std::string& message)
    {
        m_message = message;
    }

    void SayHello()
    {
        std::cout << m_message << std::endl;
    }

private:

    std::string m_message;
};

//Provide a unique identifier for your feature. This is used as the key in the Role's feature container.

constexpr qor::GUID CustomFeatureGUID = { 0x0b8962d2, 0x9b64, 0x4e86, {0xb4, 0x66, 0x9d, 0x64, 0x5c, 0x8a, 0x24, 0xe5}};
namespace qor { qor_pp_declare_guid_of(CustomFeature, CustomFeatureGUID); }//qor

//The GUID can be used to access your custom feature from the Role with Role::GetFeature(const GUID* id)