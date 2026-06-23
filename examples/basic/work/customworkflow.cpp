// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include <iostream>
#include "customworkflow.h"

using namespace qor;
using namespace qor::workflow;

//Custom workflows need to override the default constructor 
//All the States must be initialised and linked to the workflow.

CustomWorkflow::CustomWorkflow() : 
    beggining(new_ref<State>(this)),
    middle(new_ref<State>(this)),
    end(new_ref<State>(this))
{
    /*Each state has four events that can be optionally overriden
    by assigning new functions to what happens on that event.*/

    /*Always set the Enter function on each State. 
    This is where the work gets done.
    Consider this the contents of the main loop*/
    beggining->Enter = [this]()->void
    {
        std::cout << "This function is called when the begginging state is entered." << std::endl;
        
        /*We have access to the other states and the content of the workflow due to
        capturing [this] on the lambda*/

        /*Set state takes the flow directly into another state when this function ends*/
        if(std::rand() > RAND_MAX/2)
        {
            SetState(middle);
        }

        /*Otherwise this Enter function will be called in a loop as long as begginging is the current state*/
    };

    //Set this only if you need special handling for Suspension
    beggining->Suspend = [this]()->void{

        std::cout << "Suspend is called on a State when it does a PushState.\n PushState puts a state on the Workflow stack and this state is suspended until everything above it on the stack is finished." << std::endl;
    };

    //Set this only if you need special handling for Resuming
    beggining->Resume = [this]()->void{
        std::cout << "Resume is called when a State is once again at the top of the stack as states pushed on top of it have completed." << std::endl;
    };

    //Set this if you need special handling for Leaving a state
    beggining->Leave = [this]()->void{
        std::cout << "Leave is called when a state is about to be popped from the stack." << std::endl;
    };

    middle->Enter = [this]()->void {
        std::cout << "Entered the middle state." << std::endl;
        if(std::rand() > RAND_MAX/2)
        {
            PushState(end);
        }
    };

    middle->Resume = [this]()->void {
        std::cout << "middle state resumed." << std::endl;
        PopState();
    };

    end->Enter = [this]()->void {
        std::cout << "We've reached the end state." << std::endl;
        PopState();
    };

    //Always set an initial state on your workflow or ot won't operate.
    SetInitialState(beggining);
}
