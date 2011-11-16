#pragma once
#include "TheFlyWin32.h"

#define STATEATTACK 0
#define STATEBEATTACK 1
#define STATEIDLE 2
#define STATERUN 3

typedef int ActorState;



// actor free meaning it can do anything by the controller.
// actor stay meaning that it can't be move beacuse of being attacked.

class ActorStateMachine
{
public:
	ActorState state;
	ACTORid character;
	int life;
public:
	ActorStateMachine(void);
	~ActorStateMachine(void);
	ActorStateMachine(ACTORid character);
	int ChangeStateAndAction();
	int ChangeState(ActorState s);
	BOOL CanBeControl();
};

