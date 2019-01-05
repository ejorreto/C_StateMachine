#include "Motor.h"
#include "StateMachine.h"
#include <stdio.h>

// Define private instance of motor state machine
Motor motorPrivate;
SM_DEFINE(MotorPrivate, &motorPrivate)

// State enumeration order must match the order of state
// method entries in the state map
enum States
{
    ST_IDLE,
    ST_STOP,
    ST_START,
    ST_CHANGE_SPEED,
    ST_MAX_STATES
};

// State machine state functions
STATE_DECLARE(ST_Idle, void)
STATE_DECLARE(ST_Stop, void)
STATE_DECLARE(ST_Start, MotorData)
STATE_DECLARE(ST_ChangeSpeed, MotorData)

// State map to define state function order
BEGIN_STATE_MAP(Motor)
    STATE_MAP_ENTRY(ST_Idle)
    STATE_MAP_ENTRY(ST_Stop)
    STATE_MAP_ENTRY(ST_Start)
    STATE_MAP_ENTRY(ST_ChangeSpeed)
END_STATE_MAP(Motor)

// Set motor speed external event
EVENT_DEFINE(MTR_SetSpeed, MotorData)
{
    // Given the SetSpeed event, transition to a new state based upon 
    // the current state of the state machine
    BEGIN_TRANSITION_MAP                     // - Current State -
        TRANSITION_MAP_ENTRY(ST_START)       // ST_Idle       
        TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)  // ST_Stop       
        TRANSITION_MAP_ENTRY(ST_CHANGE_SPEED)// ST_Start      
        TRANSITION_MAP_ENTRY(ST_CHANGE_SPEED)// ST_ChangeSpeed
    END_TRANSITION_MAP(Motor, pEventData)
}

// Halt motor external event
EVENT_DEFINE(MTR_Halt, void)
{
    // Given the Halt event, transition to a new state based upon 
    // the current state of the state machine
    BEGIN_TRANSITION_MAP                     // - Current State -
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)  // ST_Idle
        TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)  // ST_Stop
        TRANSITION_MAP_ENTRY(ST_STOP)        // ST_Start
        TRANSITION_MAP_ENTRY(ST_STOP)        // ST_ChangeSpeed
    END_TRANSITION_MAP(Motor, NULL)
}

// State machine sits here when motor is not running
STATE_DEFINE(ST_Idle, void)
{
    printf("%s ST_Idle\n", self->name);
}

// Stop the motor 
STATE_DEFINE(ST_Stop, void)
{
    // Get pointer to the instance data and update currentSpeed
    Motor* pInstance = SM_GetInstance(Motor);
    pInstance->currentSpeed = 0;

    // Perform the stop motor processing here
    printf("%s ST_Stop: %d\n", self->name, pInstance->currentSpeed);

    // Transition to ST_Idle via an internal event
    SM_InternalEvent(ST_IDLE, NULL);
}

// Start the motor going
STATE_DEFINE(ST_Start, MotorData)
{
    // Get pointer to the instance data and update currentSpeed
    Motor* pInstance = SM_GetInstance(Motor);
    pInstance->currentSpeed = pEventData->speed;

    printf("%s ST_Start: %d\n", self->name, pInstance->currentSpeed);
    // Set initial motor speed processing here
}

// Changes the motor speed once the motor is moving
STATE_DEFINE(ST_ChangeSpeed, MotorData)
{
    // Get pointer to the instance data and update currentSpeed
    Motor* pInstance = SM_GetInstance(Motor);
    pInstance->currentSpeed = pEventData->speed;

    printf("%s ST_ChangeSpeed: %d\n", self->name, pInstance->currentSpeed);
    // Perform the change motor speed to data->speed here
}

