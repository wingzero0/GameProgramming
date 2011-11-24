#include "ActorStateMachine.h"

extern char debug[1024];

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
	if (this->state == s){
		return 0;// keep the past action play
	}else{
		this->state = s;
	}

	if (s == STATEIDLE || s == STATERUN){
		FnActor actor;
		actor.Object(this->character);
		ACTIONid actionID;
		if (s == STATEIDLE){
			actionID = actor.GetBodyAction(NULL,"Idle");
		}else if (s == STATERUN){
			actionID = actor.GetBodyAction(NULL,"RUN");
		}
		if (actionID == FAILED_ID){
			sprintf(debug, "%s get action failed\n", debug);
			return -1;
		}
	
		if (actor.MakeCurrentAction(0,NULL,actionID) == FAILED_ID){
			sprintf(debug, "%s make current fail\n", debug);
		}
	
		if (actor.Play(0,START, 0.0, FALSE,TRUE) == FALSE){
			sprintf(debug, "%s play action failed\n", debug);
		}
	}
	return 0;
}