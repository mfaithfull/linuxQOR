// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "customrole.h"
#include "src/framework/parallel/thread/threadpool.h"

using namespace qor;

//This Role comes with the ThreadPool feature baked in
//before customiztion. 
CustomRole::CustomRole()
{
    app::IRole::template AddFeature<thread::ThreadPool>(   //We add a ThreadPool.

        //Added features can be customised by passing a lamda
        //as a customisation point just like for Applications and Roles
        [](ref_of<thread::ThreadPool>::type threadPool)->void
        {                    
            threadPool->SetThreadCount(4);  //Customise the thread pool with 4 threads
            /*Now that we're going to have more than one thread showing up in the debugger
            It's time to name the one we're running on now, to idenify it.*/
            CurrentThread::Get().SetName("Main thread");
        }
    );
}

void CustomRole::Setup()
{
    /*Code here runs before any features are Setup. 
    Warning: No services will be available*/

    /*Always call the base. 
    Usually call this first and then do extra customisation
    using the features already setup*/
    app::Role::Setup();

    /*Code here runs after all featues are Setup. This is the first point where all
    services will be available*/
}

void CustomRole::Shutdown()
{
    /*At this point all features are still available but should only be used to
    clean up*/

    /* Always call the base. 
    Almost always call this last.*/    
    app::Role::Shutdown();

    /*Code here runs after all features are shutdown. There is very little it is 
    safe to do here unless it only uses the Standard Library and statically linked
    QOR libraries*/
}