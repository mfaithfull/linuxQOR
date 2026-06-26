// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//An Application with a Custom Role using the QOR SDK

//For a simpler example of the flow of a QOR application see the outline project

#include "sdk/using_framework.h"
#include "app.h"

/*QOR Applications each have a Role. The Role is a container of Features.
The Role manages the Setup and Shutdown of the Features*/

/*Include a custom Role class. This is not usually needed as the 
base Role class is usually sufficient*/
#include "customrole.h"

/*Include a custom Feature class. You can implement the features
of your application as custom QOR Features or just use the builtin ones
and add your own way to manage custom functionality*/
#include "customfeature.h"

qor_pp_implement_module(App::Name)

/*To persude the linker to import what we need to use the ICurrentThread
interface we explcitly import the implementation
Note: If you ever get an - 'External Factory cannot Construct an unregistered type'
when you run your app, you may have missed one of these.
This is needed because ICurrentThread is platform specifc so it has more than one
implementation*/
qor_pp_module_requires(ICurrentThread)

int main()
{
   return AppBuilder().Build<App>(          //The Build function gives us an App...
        App::Name,
        [](ref_of<App>::type app)
        {               
            app->CustomConfigure();         //Here we can do early configuration of the App
        }
    )->SetRole<CustomRole>(                 //...on which we can call SetRole with our CustomRole class
                
        [](ref_of<app::IRole>::type role)        //Note: Whatever the derived type of your Role you'll always get an IRole interface on this customisation point
        {
            //Here we configure the Role by adding features
            //You can add whatever Features you need including custom features
            role->AddFeature<CustomFeature>(
                //These get customisation points just like builtin Featues
                [](ref_of<CustomFeature>::type customFeature)->void
                {
                    customFeature->DoThatConfigurationThing("Hello world.");
                }
            );
        }
    ).Run(
        []()->int
        {
            /*The Application Role and Features are all shared objects
            so we need to access them with synchronisation. 
            Application is a singleton that is really our App class.
            The AppBuilder provides a way to get the instance from anywhere.
            We have to request conversion to an App reference
            Then get the Role and retrieve from it our CustomFeature by
            it's unique ID. We know it's real type so we can ask for the
            reference to be converted into a CustomFeature reference.
            Finally this gives us access to the SayHello function*/
            
            AppBuilder().TheApplication().//Gets the global Application
            AsRef<App>(qor_shared)->//Converts the reference into an App reference
            GetRole(qor_shared)->//Gets the IRole interface on the CustomRole instance
            GetFeature(&CustomFeatureGUID).//Lookup the CustomFeature by it's unique ID
            AsRef<CustomFeature>(qor_shared)->//Convert the reference from an IFeature to a CustomFeature
            SayHello();

            /*This is clearly massive overkill for a one line program but
            you likely aren't building a one line program.*/

            return EXIT_SUCCESS;
        });
}