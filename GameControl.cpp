#include "GameControl.h"

extern char debug[1024];

GameControl::GameControl(void)
{
}


GameControl::~GameControl(void)
{
}

GameControl::GameControl(ActorStateMachine * character, OBJECTid camera){
	this->camera = camera;
	this->mainChar = character;
}

BOOL GameControl::CharacterMoveForward(){
	if (this->mainChar->CanBeControl() == FALSE){
		return FALSE;
	}
	FnActor actor;
	actor.Object(this->mainChar->character);
	int ret = actor.MoveForward(1.0,TRUE, FALSE, 0.0, TRUE);
	this->mainChar->ChangeState(STATERUN);
	sprintf(debug ,"%d FY_W ret = %d\n",this->mainChar->character, ret);
	return TRUE;
}

BOOL GameControl::CharacterNormalAttack(){
	if (this->mainChar->state == STATEBEATTACK){
		return FALSE;
	}
	this->mainChar->ChangeState(STATEATTACK);
	sprintf(debug ,"%d attacking\n",this->mainChar->character);
	return TRUE;
}

BOOL GameControl::CharacterSetIdle(){
	this->mainChar->ChangeState(STATEIDLE);
	return TRUE;
}
/*
GameControl::GameControl(ACTORid character, OBJECTid camera){
	this->camera = camera;
	this->mainChar = character;
}*/