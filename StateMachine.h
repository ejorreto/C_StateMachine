// The StateMachine module is a C language implementation of a finite state 
// machine (FSM).
//
// All event data must be created on the heap using SM_XAlloc. Use a fixed 
// block allocator or the heap as desired. 
//
// Macros are used to assist in creating the state machine machinery. 

#ifndef _STATE_MACHINE_H
#define _STATE_MACHINE_H

#include "DataTypes.h"
#include "Fault.h"

#ifdef __cplusplus
extern "C" {
#endif

// Define USE_SM_ALLOCATOR to use the fixed block allocator instead of heap
#define USE_SM_ALLOCATOR
#ifdef USE_SM_ALLOCATOR
    #include "sm_allocator.h"
    #define SM_XAlloc(size)    SMALLOC_Alloc(size)
    #define SM_XFree(ptr)      SMALLOC_Free(ptr)
#else
    #include <stdlib.h>
    #define SM_XAlloc(size)    malloc(size)
    #define SM_XFree(ptr)      free(ptr)
#endif

enum { EVENT_IGNORED = 0xFE, CANNOT_HAPPEN = 0xFF };

// State machine constant data
typedef struct
{
    const CHAR* name;
    const BYTE maxStates;
    const struct SM_StateStruct* stateMap;
} SM_StateMachineConst;

// State machine instance data
typedef struct 
{
    const CHAR* name;
    void* pInstance;
    BYTE currentState;
    BOOL eventGenerated;
    void* pEventData;
} SM_StateMachine;

// Generic state function signature
typedef void(*SM_StateFunc)(SM_StateMachine* self, void* eventData);

typedef struct SM_StateStruct
{
    SM_StateFunc pStateFunc;
} SM_StateStruct;

// Public functions
#define SM_Event(_smName_, _eventFunc_, _eventData_) \
    _eventFunc_(&_smName_##Obj, _eventData_)

// Protected functions
#define SM_InternalEvent(_newState_, _eventData_) \
    _SM_InternalEvent(self, _newState_, _eventData_)
#define SM_GetInstance(_instance_) \
    (_instance_*)(self->pInstance);

// Private functions
void _SM_ExternalEvent(SM_StateMachine* self, SM_StateMachineConst* selfConst, BYTE newState, void* pEventData);
void _SM_InternalEvent(SM_StateMachine* self, BYTE newState, void* pEventData);
void _SM_StateEngine(SM_StateMachine* self, SM_StateMachineConst* selfConst);

#define SM_DECLARE(_smName_) \
    extern SM_StateMachine _smName_##Obj; 

#define SM_DEFINE(_smName_, _instance_) \
    SM_StateMachine _smName_##Obj = { #_smName_, _instance_, \
        0, 0, 0 }; 

#define EVENT_DECLARE(_eventFunc_, _eventData_) \
    void _eventFunc_(SM_StateMachine* self, _eventData_* pEventData);

#define EVENT_DEFINE(_eventFunc_, _eventData_) \
    void _eventFunc_(SM_StateMachine* self, _eventData_* pEventData)

#define STATE_DECLARE(_stateFunc_, _eventData_) \
    static void _stateFunc_(SM_StateMachine* self, _eventData_* pEventData);

#define STATE_DEFINE(_stateFunc_, _eventData_) \
    static void _stateFunc_(SM_StateMachine* self, _eventData_* pEventData)

#define BEGIN_STATE_MAP(_smName_) \
    static const SM_StateStruct _smName_##StateMap[] = { 

#define STATE_MAP_ENTRY(stateFunc)\
    { stateFunc },

#define END_STATE_MAP(_smName_) \
    }; \
    SM_StateMachineConst _smName_##Const = { #_smName_, \
        (sizeof(_smName_##StateMap)/sizeof(_smName_##StateMap[0])), \
        _smName_##StateMap };

#define BEGIN_TRANSITION_MAP \
    static const BYTE TRANSITIONS[] = { \

#define TRANSITION_MAP_ENTRY(entry) \
    entry,

#define END_TRANSITION_MAP(_smName_, _eventData_) \
    }; \
    _SM_ExternalEvent(self, &_smName_##Const, TRANSITIONS[self->currentState], _eventData_); \
    C_ASSERT((sizeof(TRANSITIONS)/sizeof(BYTE)) == (sizeof(_smName_##StateMap)/sizeof(_smName_##StateMap[0])));

#ifdef __cplusplus
}
#endif
 
#endif // _STATE_MACHINE_H