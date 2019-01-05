#include "fb_allocator.h"
#include "StateMachine.h"
#include "Motor.h"

// Define motor objects
static Motor motorObj1;
static Motor motorObj2;

// Define two public Motor state machine instances
SM_DEFINE(Motor1Public, &motorObj1)
SM_DEFINE(Motor2Public, &motorObj2)

int main()
{
    ALLOC_Init();

    MotorData* data;
    
    // Create event data
    data = SM_XAlloc(sizeof(MotorData));
    data->speed = 100;

    // Call MTR_SetSpeed event function to start motor
    SM_Event(Motor1Public, MTR_SetSpeed, data);

    // Call MTR_SetSpeed event function to change motor speed
    data = SM_XAlloc(sizeof(MotorData));
    data->speed = 200;
    SM_Event(Motor1Public, MTR_SetSpeed, data);

    // Stop the motor
    SM_Event(Motor1Public, MTR_Halt, NULL);

    // Stop motor again will be ignored
    SM_Event(Motor1Public, MTR_Halt, NULL);

    // Call Motor2Public state machine instance
    data = SM_XAlloc(sizeof(MotorData));
    data->speed = 300;
    SM_Event(Motor2Public, MTR_SetSpeed, data);
    SM_Event(Motor2Public, MTR_Halt, NULL);

    // Call MotorPrivate state machine instance
    data = SM_XAlloc(sizeof(MotorData));
    data->speed = 400;
    SM_Event(MotorPrivate, MTR_SetSpeed, data);
    SM_Event(MotorPrivate, MTR_Halt, NULL);

    ALLOC_Term();

    return 0;
}

