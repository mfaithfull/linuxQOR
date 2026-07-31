// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include <iostream>
#include "customfastflow.h"

using namespace qor;
using namespace qor::fastflow;

//Custom fastflows need to override the default constructor 
//All the States must be initialised and linked to the fastflow.

CustomFastflow::CustomFastflow() : 
    beggining(new_ref<Step>(this)),
    middle(new_ref<Step>(this)),
    end(new_ref<Step>(this))
{
    /*Each step has four events that can be optionally overriden
    by assigning new functions to what happens on that event.*/

    /*Always set the Enter function on each Step. 
    This is where the work gets done.
    Consider this the contents of the main loop*/
    beggining->Enter = [this]()->void
    {
        std::cout << "This function is called when the begginging step is entered." << std::endl;
        
        /*We have access to the other states and the content of the workflow due to
        capturing [this] on the lambda*/

        /*SetStep takes the flow directly into another step when this function ends*/
        if(std::rand() > RAND_MAX/2)
        {
            SetStep(middle);
        }

        /*Otherwise this Enter function will be called in a loop as long as it's at the top of the Fastflow stack*/
    };

    //Set this only if you need special handling for Resuming
    beggining->Resume = [this]()->void{
        std::cout << "Resume is called when a Step is once again at the top of the stack as steps pushed on top of it have completed." << std::endl;
    };

    //Set this if you need special handling for Leaving a state
    beggining->Leave = [this]()->void{
        std::cout << "Leave is called when a Step is about to be popped from the stack." << std::endl;
    };

    middle->Enter = [this]()->void {
        std::cout << "Entered the middle step." << std::endl;
        if(std::rand() > RAND_MAX/2)
        {
            PushStep(end);
        }
    };

    middle->Resume = [this]()->void {
        std::cout << "middle step resumed." << std::endl;
        PopStep();
    };

    end->Enter = [this]()->void {
        std::cout << "We've reached the end step." << std::endl;
        PopStep();
    };

    //Always set an initial step on your fastflow or it won't operate.
    SetInitialStep(beggining);
}
