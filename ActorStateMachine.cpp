#include "ActorStateMachine.h"


ActorStateMachine::ActorStateMachine(void)
{
}


ActorStateMachine::~ActorStateMachine(void)
{
}

ActorStateMachine::ActorStateMachine(ACTORid character){
	this->character = character;
	this->state = STATEIDLE;
}

BOOL ActorStateMachine::CanBeControl(){
	if (this->state == STATEATTACK || this->state == STATEBEATTACK){
		return FALSE;
	}else{
		return TRUE;
	}
}

int ActorStateMachine::ChangeState(ActorState s){
	this->state = s;
	return 0;
}