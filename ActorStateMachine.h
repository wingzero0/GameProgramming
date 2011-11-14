#pragma once
#include "TheFlyWin32.h"

#define ACTORFREE 0
#define ACTORSTAY 1
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
};

